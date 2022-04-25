#pragma once

#ifndef CONTIGUOUS_BUFFER_H
#define CONTIGUOUS_BUFFER_H

#include <cstddef>
#include <vector>


/// @class ContiguousBuffer
/// @brief Contiguous buffer for storing binary data.
class ContiguousBuffer
{
public:
    /// @brief Default constructor.
    ContiguousBuffer();
    
    /// @brief Constructor, creates buffer with provided size and capacity.
    /// @param[in] size - Initial size of the buffer.
    /// @param[in] capacity - Initial size of the buffer. Optional argument.
    /// @throws std::exception in case of an error.
    ContiguousBuffer( size_t size, size_t capacity = 0 );
    
    /// @brief Constructor, creates buffer with provided size and capacity and fill with provided data.
    /// @param[in] source - Source of initial data.
    /// @param[in] size - Initial size of the buffer.
    /// @param[in] capacity - Initial size of the buffer. Optional argument.
    /// @throws std::exception in case of an error.
    ContiguousBuffer( const void* source, size_t size, size_t capacity = 0 );
    
    /// @brief Copy construstor.
    /// @param[in] another - Another buffer.
    /// @throws std::exception in case of an error.
    ContiguousBuffer( const ContiguousBuffer& another );
    
    /// @brief Destructor.
    ~ContiguousBuffer();
    
    /// @brief Assignment operator.
    /// @param[in] another - Another buffer.
    /// @throws std::exception in case of an error.
    ContiguousBuffer& operator=( const ContiguousBuffer& another );
    
    /// @brief Test whether buffer is empty.
    /// @return true if buffer is empty, false otherwise.
    bool Empty() const;
    
    /// @brief Returns pointer to underlying buffer.
    /// @return 0 if buffer is empty, pointer to underlying buffer otherwise.
    const char* Data() const;
    
    /// @brief Returns pointer to underlying buffer.
    /// @return 0 if buffer is empty, pointer to underlying buffer otherwise.
    char* Data();
    
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
    
#ifndef NDEBUG
    /// @brief Returns total number of allocations and reallocations made by the buffer.
    /// @return Total number of allocations and reallocations.
    unsigned Allocs() const;
#endif
     
private:
    /// @brief Constructor, creates buffer from external storage and takes ownership of that storage.
    /// @details As far as the origin of external storage is unknown, it is compelety
    ///          unsafe to take ownership. Therefore this constructor should not be implemented.
    ContiguousBuffer( const void* data, size_t size, size_t capacity, bool ownership );

    /// @brief Initially allocates buffer with provided size and capacity.
    /// @param[in] size - Initial size of the buffer.
    /// @param[in] capacity - Initial capacity of the buffer.
    /// @throws std::exception in case of an error.
    void Allocate( size_t size, size_t capacity );
    
    /// @brief Reallocate buffer or move data within buffer if needed.
    /// @param[in] size - New size of the buffer.
    /// @param[in] capacity - New capacity of the buffer.
    /// @throws std::exception in case of an error.
    void Reallocate( size_t size, size_t capacity ) const;
    
    /// @brief Returns pointer to underlying buffer.
    /// @return 0 if buffer is empty, pointer to underlying buffer otherwise.
    const char* GetInnerData() const;
     
private:
    /// @brief Data storage.
    mutable std::vector< char > storage_;
    
    /// @brief Current offset of the data storage.
    mutable size_t begin_;
    
#ifndef NDEBUG
    /// @brief Total number of allocations and reallocations.
    mutable unsigned allocs_;
#endif
};

#endif // CONTIGUOUS_BUFFER_H
