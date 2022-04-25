#pragma once

#ifndef SEGMENTED_BUFFER_H
#define SEGMENTED_BUFFER_H

#include "contiguous_buffer.h"

#include <cstddef>
#include <deque>


/// @class SegmentedBuffer
/// @brief Buffer with multiple memory regions for storing binary data.
class SegmentedBuffer
{
public:
    /// @brief Default constructor.
    SegmentedBuffer();
    
    /// @brief Constructor, creates buffer with provided size and capacity.
    /// @param[in] size - Initial size of the buffer.
    /// @param[in] capacity - Initial size of the buffer. Optional argument.
    /// @throws std::exception in case of an error.
    SegmentedBuffer( size_t size, size_t capacity = 0 );
    
    /// @brief Constructor, creates buffer with provided size and capacity and fill with provided data.
    /// @param[in] source - Source of initial data.
    /// @param[in] size - Initial size of the buffer.
    /// @param[in] capacity - Initial size of the buffer. Optional argument.
    /// @throws std::exception in case of an error.
    SegmentedBuffer( const void* source, size_t size, size_t capacity = 0 );
    
    /// @brief Copy construstor.
    /// @param[in] another - Another buffer.
    /// @throws std::exception in case of an error.
    SegmentedBuffer( const SegmentedBuffer& another );
    
    /// @brief Destructor.
    ~SegmentedBuffer();
    
    /// @brief Assignment operator.
    /// @param[in] another - Another buffer.
    /// @throws std::exception in case of an error.
    SegmentedBuffer& operator=( const SegmentedBuffer& another );
    
    /// @brief Test whether buffer is empty.
    /// @return true if buffer is empty, false otherwise.
    bool Empty() const;
    
    /// @brief Returns size of the buffer, i.e. number of bytes available for reading.
    /// @return Size of the buffer.
    size_t Size() const;
    
    /// @brief Resizes the buffer so that it contains @b size bytes.
    /// @param[in] size - New size of the buffer.
    /// @throws std::exception in case of an error.
    void Size( size_t size );
    
    /// @brief Returns capacity of the buffer, i.e. number of bytes that
    ///        the buffer can store without reallocing.
    /// @return Capacity of the buffer.
    size_t Capacity() const;
    
    /// @brief Requests that the buffer capacity be at least @b capacity bytes.
    /// @param[in] capacity - New capacity of the buffer.
    /// @throws std::exception in case of an error.
    void Capacity( size_t capacity );
    
    /// @brief Reads data from the buffer.
    /// @param[out] destination - Pointer to the destination where the buffer data is to be written.
    /// @param[in] size - Requested size of the buffer data.
    /// @return Number of actually read bytes.
    /// @throws std::exception in case of an error.
    size_t Read( void* destination, size_t size ) const;
    
    /// @brief Writes data into the buffer.
    /// @param[in] source - Pointer to the source of the data.
    /// @param[in] size - Size of the data.
    /// @throws std::exception in case of an error.
    void Write( const void* source, size_t size );
    
    /// @brief Assign buffer's content to the provided source,
    ///        replacing its current content and modifying its size.
    /// @param[in] source - Pointer to the source of the new content.
    /// @param[in] size - Size of the new content.
    /// @throws std::exception in case of an error.
    void Assign( const void* source, size_t size );
    
    /// @brief Clears the buffer, reducing its size and capacity to 0.
    void Clear();
     
private:
    /// @brief Constructor, creates buffer from external storage and takes ownership of that storage.
    /// @details As far as the origin of external storage is unknown, it is compelety
    ///          unsafe to take ownership. Therefore this constructor should not be implemented.
    SegmentedBuffer( const void* data, size_t size, size_t capacity, bool ownership );
    
    /// @brief Reallocate segments of the buffer.
    /// @param[in] size - New size of the buffer.
    /// @param[in] capacity - New capacity of the buffer.
    /// @throws std::exception in case of an error.
    void Reallocate( size_t size, size_t capacity ) const;
    
    /// @brief Test whether @b n-th segment is full.
    /// @return true if segment is full, false otherwise.
    bool SegmentFull( size_t n ) const;

private:
    /// @brief Data storage.
    mutable std::deque< ContiguousBuffer > storage_;
    
    /// @brief Number of currntly last non-empty segment.
    mutable size_t lastNonEmptySegment_;
    
    /// @brief Total size of data currently stored in the buffer.
    mutable size_t size_;
    
    /// @brief Total capacity of the buffer.
    mutable size_t capacity_;

    /// @brief Size of memory page.
    const static size_t memoryPageSize_;
};

#endif // SEGMENTED_BUFFER_H
