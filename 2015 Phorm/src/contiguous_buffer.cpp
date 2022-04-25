#include "contiguous_buffer.h"

#include <cassert>
#include <cstring>
#include <stdexcept>


ContiguousBuffer::ContiguousBuffer()
    : storage_()
    , begin_( 0 )
#ifndef NDEBUG
    , allocs_( 0 )
#endif
{
}

ContiguousBuffer::ContiguousBuffer( size_t size, size_t capacity )
    : storage_()
    , begin_( 0 )
#ifndef NDEBUG
    , allocs_( 0 )
#endif
{
    Allocate( size, capacity );
}

ContiguousBuffer::ContiguousBuffer( const void* source, size_t size, size_t capacity )
    : storage_()
    , begin_( 0 )
#ifndef NDEBUG
    , allocs_( 0 )
#endif
{
    if( !source && size )
    {
        throw std::invalid_argument( "ContiguousBuffer::Ctor, source is NULL" );
    }
    
    Allocate( size, capacity );

    if( size )
    {
        memcpy( &storage_[0], source, size );
    }
}

ContiguousBuffer::ContiguousBuffer( const ContiguousBuffer& another )
    : storage_( another.storage_ )
    , begin_( another.begin_ )
#ifndef NDEBUG
    , allocs_( another.storage_.empty() ? 0 : 1 )
#endif
{
}

ContiguousBuffer::~ContiguousBuffer()
{
}

ContiguousBuffer& ContiguousBuffer::operator=( const ContiguousBuffer& another )
{
    if( this == &another )
    {
        return *this;
    }
    
    storage_ = another.storage_;
    begin_ = another.begin_;
#ifndef NDEBUG
    // Arguably.
    ++allocs_;
#endif

    return *this;
}

bool ContiguousBuffer::Empty() const
{
    return Size() == 0;
}

const char* ContiguousBuffer::Data() const
{
    return GetInnerData();
}

char* ContiguousBuffer::Data()
{
    return const_cast< char* >( GetInnerData() );
}

size_t ContiguousBuffer::Size() const
{
    return storage_.size() - begin_;
}

void ContiguousBuffer::Size( size_t size )
{
    // If size is 0, reset offset.
    if( !size )
    {
        begin_ = 0;
    }
    
    // If offset is 0, just resize inner storage.
    if( !begin_ )
    {
#ifndef NDEBUG
        if( size > storage_.capacity() )
        {
            ++allocs_;
        }
#endif
        storage_.resize( size );
    }
    // All other cases may require reallocation.
    else
    {
        Reallocate( size, storage_.capacity() );
    }
}

size_t ContiguousBuffer::Capacity() const
{
    return storage_.capacity() - begin_;
}

void ContiguousBuffer::Capacity( size_t capacity )
{
    // If requested capacity is less than the current one, do nothing.
    if( capacity <= Capacity() )
    {
        return;
    }
    // If offset is 0, just reserve capacity of inner storage.
    else if( !begin_ )
    {
#ifndef NDEBUG
        if( capacity > storage_.capacity() )
        {
            ++allocs_;
        }
#endif
        storage_.reserve( capacity );
    }
    // All other cases may require reallocation.
    else
    {
        Reallocate( Size(), capacity );
    }
}

size_t ContiguousBuffer::Read( void* destination, size_t size ) const
{
    if( !destination && size )
    {
        throw std::invalid_argument( "ContiguousBuffer::Read, destination is NULL" );
    }
    
    size_t bytesToRead = std::min( size, Size() );
    memcpy( destination, &storage_[ begin_ ], bytesToRead );
    begin_ += bytesToRead;
    
    // If number of read but still kept bytes is greater than the number of unread ones,
    // move unread bytes to the beginning of the inner storage.
    if( begin_ >= Size() )
    {
        Reallocate( Size(), storage_.capacity() );
    }
    
    return bytesToRead;
}

void ContiguousBuffer::Write( const void* source, size_t size )
{
    if( !source && size )
    {
        throw std::invalid_argument( "ContiguousBuffer::Write, source is NULL" );
    }
    
    const size_t oldSize = Size();
    
    // Allocate enough space if necessary.
    Reallocate( Size() + size, storage_.capacity() );
    memcpy( &storage_[ oldSize ], source, size );
}

void ContiguousBuffer::Assign( const void* source, size_t size )
{
    if( !source && size )
    {
        throw std::invalid_argument( "ContiguousBuffer::Assign, source is NULL" );
    }
    
    begin_ = 0;
    // Allocate enough space if necessary.
    Reallocate( size, storage_.capacity() );
    memcpy( &storage_[ begin_ ], source, size );
}

void ContiguousBuffer::Clear()
{
    std::vector< char >().swap( storage_ );
    begin_ = 0;
}

#ifndef NDEBUG
unsigned ContiguousBuffer::Allocs() const
{
    return allocs_;
}
#endif

void ContiguousBuffer::Allocate( size_t size, size_t capacity )
{
    storage_.reserve( std::max( size, capacity ) );
    storage_.resize( size );
#ifndef NDEBUG
    ++allocs_;
#endif
}

void ContiguousBuffer::Reallocate( size_t size, size_t capacity ) const
{
    capacity = std::max( size, capacity );
    
    // If requested capacity is greater than the current capacity of inner storage,
    // or new size of the buffer is greater that the actual capacity of the buffer.
    // That are the cases when reallocation is definitely needed.
    if( capacity > storage_.capacity() ||
        Capacity() < size )
    {
        std::vector< char > tmp;
        tmp.reserve( capacity );
        tmp.resize( size );
        if( !storage_.empty() )
        {
            memcpy( &tmp[0], &storage_[ begin_ ], std::min( Size(), size ) );
        }
        storage_.swap( tmp );
        begin_ = 0;
#ifndef NDEBUG
        ++allocs_;
#endif
        return;
    }
    
    // If number of read but still kept bytes is greater than the number of unread ones,
    // just move unread bytes to the beginning of the inner storage.
    if( begin_ >= size )
    {
        memcpy( &storage_[0], &storage_[ begin_ ], std::min( Size(), size ) );
        begin_ = 0;
    }
    
#ifndef NDEBUG
    if( size > storage_.capacity() )
    {
        ++allocs_;
    }
#endif
    storage_.resize( begin_ + size );
}

const char* ContiguousBuffer::GetInnerData() const
{
    return Size() ? &storage_[ begin_ ] : 0;
}
