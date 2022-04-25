#include "segmented_buffer.h"

#include <unistd.h>

#include <cassert>
#include <cstring>
#include <stdexcept>


const size_t SegmentedBuffer::memoryPageSize_ = getpagesize();


SegmentedBuffer::SegmentedBuffer()
    : storage_()
    , lastNonEmptySegment_( -1 )
    , size_( 0 )
    , capacity_( 0 )
{
}

SegmentedBuffer::SegmentedBuffer( size_t size, size_t capacity )
    : storage_()
    , lastNonEmptySegment_( -1 )
    , size_( 0 )
    , capacity_( 0 )
{
    if( size || capacity )
    {
        Reallocate( size, capacity );
    }
}

SegmentedBuffer::SegmentedBuffer( const void* source, size_t size, size_t capacity )
    : storage_()
    , lastNonEmptySegment_( -1 )
    , size_( 0 )
    , capacity_( 0 )
{
    if( !source && size )
    {
        throw std::invalid_argument( "SegmentedBuffer::Ctor, source is NULL" );
    }
    
    if( size || capacity )
    {
        Reallocate( 0, std::max( size, capacity ) );
    }
    
    if( size )
    {
        Write( source, size );
    }
}

SegmentedBuffer::SegmentedBuffer( const SegmentedBuffer& another )
    : storage_( another.storage_ )
    , lastNonEmptySegment_( another.lastNonEmptySegment_ )
    , size_( another.size_ )
    , capacity_( another.capacity_ )
{
}

SegmentedBuffer::~SegmentedBuffer()
{
}

SegmentedBuffer& SegmentedBuffer::operator=( const SegmentedBuffer& another )
{
    if( this == &another )
    {
        return *this;
    }
    
    storage_ = another.storage_;
    lastNonEmptySegment_ = another.lastNonEmptySegment_;
    size_ = another.size_;
    capacity_ = another.capacity_;

    return *this;
}

bool SegmentedBuffer::Empty() const
{
    return Size() == 0;
}

size_t SegmentedBuffer::Size() const
{
    return size_;
}

void SegmentedBuffer::Size( size_t size )
{
    if( size == size_ )
    {
        return;
    }
    Reallocate( size, Capacity() );
}

size_t SegmentedBuffer::Capacity() const
{
    return capacity_;
}

void SegmentedBuffer::Capacity( size_t capacity )
{
    // If requested capacity is less than the current one, do nothing.
    if( capacity <= Capacity() )
    {
        return;
    }
    Reallocate( Size(), capacity );
}

size_t SegmentedBuffer::Read( void* destination, size_t size ) const
{
    if( !destination && size )
    {
        throw std::invalid_argument( "SegmentedBuffer::Read, destination is NULL" );
    }
    
    size_t bytesToRead = std::min( size, Size() );
    size_t bytesRead = 0;
    
    while( bytesToRead && !storage_.empty() && !storage_[0].Empty() )
    {
        const size_t bytes = storage_[0].Read( reinterpret_cast< char* >( destination ) + bytesRead,
                                               bytesToRead );
        bytesToRead -= bytes;
        bytesRead += bytes;
        
        if( storage_[0].Empty() && storage_.size() > 1 && !storage_[1].Empty() )
        {
            capacity_ -= storage_[0].Capacity();
            --lastNonEmptySegment_;
            storage_.pop_front();
        }
    }
    
    size_ -= bytesRead;
    
    return bytesRead;
}

void SegmentedBuffer::Write( const void* source, size_t size )
{
    if( !source && size )
    {
        throw std::invalid_argument( "SegmentedBuffer::Write, source is NULL" );
    }
    
    Capacity( Size() + size );
    
    size_t bytesWritten = 0;
    while( bytesWritten != size )
    {
        if( Empty() || SegmentFull( lastNonEmptySegment_ ) )
        {
            ++lastNonEmptySegment_;
        }
        
        const size_t bytesToWrite = std::min( size - bytesWritten,
            storage_[ lastNonEmptySegment_ ].Capacity() - storage_[ lastNonEmptySegment_ ].Size() );
        
        storage_[ lastNonEmptySegment_ ].Write( reinterpret_cast< const char* >( source ) + bytesWritten,
                                                bytesToWrite );
        bytesWritten += bytesToWrite;
    }
    
    size_ += bytesWritten;
}

void SegmentedBuffer::Assign( const void* source, size_t size )
{
    if( !source && size )
    {
        throw std::invalid_argument( "SegmentedBuffer::Assign, source is NULL" );
    }
    
    Clear();
    Write( source, size );
}

void SegmentedBuffer::Clear()
{
    std::deque< ContiguousBuffer >().swap( storage_ );
    lastNonEmptySegment_ = -1;
    size_ = 0;
    capacity_ = 0;
}

void SegmentedBuffer::Reallocate( size_t size, size_t capacity ) const
{
    capacity = std::max( size, capacity );
    
    // Increase capacity if needed.
    while( capacity > capacity_ )
    {
        storage_.push_back( ContiguousBuffer() );
        ContiguousBuffer& newSegment = storage_.back();
        
        newSegment.Capacity( memoryPageSize_ );
        capacity_ += newSegment.Capacity();
    }
    
    assert( !storage_.empty() );
    
    while( size != size_ )
    {
        // Increase size
        if( size > size_ )
        {
            if( Empty() || SegmentFull( lastNonEmptySegment_ ) )
            {
                ++lastNonEmptySegment_;
                assert( lastNonEmptySegment_ < storage_.size() );
            }
            
            ContiguousBuffer& currentSegment = storage_[ lastNonEmptySegment_ ];
            
            const size_t bytesToAdd = std::min( size - size_,
                                                currentSegment.Capacity() -
                                                currentSegment.Size() );
            currentSegment.Size( currentSegment.Size() + bytesToAdd );
            size_ += bytesToAdd;
        }
        // Decrease size.
        else
        {
            assert( lastNonEmptySegment_ < storage_.size() );
            ContiguousBuffer& currentSegment = storage_[ lastNonEmptySegment_ ];
            
            const size_t bytesToRemove = std::min( size_ - size, currentSegment.Size() );
            currentSegment.Size( currentSegment.Size() - bytesToRemove );
            
            size_ -= bytesToRemove;
            if( currentSegment.Empty() )
            {
                --lastNonEmptySegment_;
            }
        }
    }
    
    assert( capacity <= capacity_ );
}

bool SegmentedBuffer::SegmentFull( size_t n ) const
{
    if( n >= storage_.size() )
    {
        throw std::out_of_range( "SegmentedBuffer::SegmentFull, segment number is out of range" );
    }
    
    return storage_[ n ].Size() == storage_[ n ].Capacity();
}
