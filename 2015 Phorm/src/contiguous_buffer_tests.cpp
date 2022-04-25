#include "contiguous_buffer.h"
#include "unit_tests.h"

#include <cstring>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <vector>


#define BEGIN_TEST( name ) \
    bool ContiguousBuffer_ ## name () \
    { \
        std::cout << __func__ << " ... "; \
        bool result = false;
        
#define END_TEST \
        std::cout << ( result ? "OK" : "FAIL" ) << std::endl; \
        return result; \
    }
    
#define RUN_TEST( name ) ContiguousBuffer_ ## name ()

namespace
{

const std::string testString = "'We called him Tortoise because he taught us,' "
                               "said the Mock Turtle angrily: 'really you are very dull!'";

}

namespace
{

BEGIN_TEST( ConstructorTest_1 )
{
    ContiguousBuffer buffer;
    result = !buffer.Size() && !buffer.Capacity() && !buffer.Data();
                  
#ifndef NDEBUG
    result = result && !buffer.Allocs();
#endif
}
END_TEST

BEGIN_TEST( ConstructorTest_2 )
{
    const size_t size = 50;
    const size_t capacity = 120;
    
    try
    {
        ContiguousBuffer buffer( size, capacity );
        result = buffer.Size() == size &&
                 buffer.Capacity() >= capacity;
#ifndef NDEBUG
        result = result && buffer.Allocs() == 1;
#endif
    }
    catch( const std::exception& e )
    {
        std::cerr << e.what() << std::endl;
        result = false;
    }
}
END_TEST

BEGIN_TEST( ConstructorTest_3 )
{
    try
    {
        ContiguousBuffer buffer( testString.c_str(), testString.size() );
        result = buffer.Size() == testString.size() &&
                 buffer.Capacity() >= testString.size() &&
                 !memcmp( buffer.Data(), testString.c_str(), testString.size() );
#ifndef NDEBUG
        result = result && buffer.Allocs() == 1;
#endif
    }
    catch( const std::exception& e )
    {
        std::cerr << e.what() << std::endl;
        result = false;
    }
}
END_TEST

BEGIN_TEST( ConstructorTest_4 )
{
    const size_t size = std::numeric_limits< size_t >::max();
    
    try
    {
        ContiguousBuffer buffer( size );
    }
    catch( const std::exception& )
    {
        result = true;
    }
}
END_TEST

BEGIN_TEST( ConstructorTest_5 )
{
    try
    {
        ContiguousBuffer buffer( static_cast< const void* >( 0 ), testString.size() );
    }
    catch( const std::exception& )
    {
        result = true;
    }
}
END_TEST

BEGIN_TEST( SizeTest_1 )
{
    try
    {
        ContiguousBuffer buffer( testString.c_str(), testString.size() );
        
        const size_t sizeDecrement = 5;
        buffer.Size( buffer.Size() - sizeDecrement );
        
        result = !memcmp( buffer.Data(),
                          testString.c_str(),
                          std::min( testString.size(), buffer.Size() ) );
        result = result && ( buffer.Size() == testString.size() - sizeDecrement );
#ifndef NDEBUG
        result = result && buffer.Allocs() == 1;
#endif
    }
    catch( const std::exception& e )
    {
        std::cerr << e.what() << std::endl;
        result = false;
    }
}
END_TEST

BEGIN_TEST( SizeTest_2 )
{
    try
    {
        ContiguousBuffer buffer( testString.c_str(), testString.size() );
        
        const size_t sizeMultiply = 5;
        buffer.Size( buffer.Size() * sizeMultiply );
        
        result = !memcmp( buffer.Data(),
                          testString.c_str(),
                          std::min( testString.size(), buffer.Size() ) );
        result = result && ( buffer.Size() == testString.size() * sizeMultiply );
#ifndef NDEBUG
        result = result && buffer.Allocs() == 2;
#endif
    }
    catch( const std::exception& e )
    {
        std::cerr << e.what() << std::endl;
        result = false;
    }
}
END_TEST

BEGIN_TEST( SizeTest_3 )
{
    try
    {
        ContiguousBuffer buffer( testString.c_str(), testString.size() );
        
        std::vector< char > tmp( testString.size() / 4 );
        if( tmp.empty() )
        {
            result = false;
        }
        else
        {
            size_t bytesRead = buffer.Read( &tmp[0], tmp.size() );
            
            const size_t sizeDecrement = 5;
            buffer.Size( buffer.Size() - sizeDecrement );
            
            result = !memcmp( buffer.Data(),
                              testString.c_str() + bytesRead,
                              buffer.Size() );
            result = result && ( buffer.Size() == testString.size() - sizeDecrement - bytesRead );
        }
#ifndef NDEBUG
        result = result && buffer.Allocs() == 1;
#endif
    }
    catch( const std::exception& e )
    {
        std::cerr << e.what() << std::endl;
        result = false;
    }
}
END_TEST

BEGIN_TEST( SizeTest_4 )
{
    try
    {
        ContiguousBuffer buffer( testString.c_str(), testString.size() );
        
        std::vector< char > tmp( testString.size() / 4 );
        if( tmp.empty() )
        {
            result = false;
        }
        else
        {
            size_t bytesRead = buffer.Read( &tmp[0], tmp.size() );
            
            const size_t sizeMultiply = 5;
            buffer.Size( buffer.Size() * sizeMultiply );
            
            result = !memcmp( buffer.Data(),
                              testString.c_str() + bytesRead,
                              testString.size() - bytesRead );
            result = result && ( buffer.Size() == ( testString.size() - bytesRead ) * sizeMultiply );
        }
#ifndef NDEBUG
        result = result && buffer.Allocs() == 2;
#endif
    }
    catch( const std::exception& e )
    {
        std::cerr << e.what() << std::endl;
        result = false;
    }
}
END_TEST

BEGIN_TEST( SizeTest_5 )
{
    try
    {
        ContiguousBuffer buffer( testString.c_str(), testString.size() );
        
        std::vector< char > tmp( 3 * testString.size() / 4 );
        if( tmp.empty() )
        {
            result = false;
        }
        else
        {
            size_t bytesRead = buffer.Read( &tmp[0], tmp.size() );
            
            const size_t sizeDecrement = 5;
            buffer.Size( buffer.Size() - sizeDecrement );
            
            result = !memcmp( buffer.Data(),
                              testString.c_str() + bytesRead,
                              buffer.Size() );
            result = result && ( buffer.Size() == testString.size() - sizeDecrement - bytesRead );
        }
#ifndef NDEBUG
        result = result && buffer.Allocs() == 1;
#endif
    }
    catch( const std::exception& e )
    {
        std::cerr << e.what() << std::endl;
        result = false;
    }
}
END_TEST

BEGIN_TEST( SizeTest_6 )
{
    try
    {
        ContiguousBuffer buffer( testString.c_str(), testString.size() );
        
        std::vector< char > tmp( 3 * testString.size() / 4 );
        if( tmp.empty() )
        {
            result = false;
        }
        else
        {
            size_t bytesRead = buffer.Read( &tmp[0], tmp.size() );
            
            const size_t sizeMultiply = 5;
            buffer.Size( buffer.Size() * sizeMultiply );
            
            result = !memcmp( buffer.Data(),
                              testString.c_str() + bytesRead,
                              testString.size() - bytesRead );
            result = result && ( buffer.Size() == ( testString.size() - bytesRead ) * sizeMultiply );
        }
#ifndef NDEBUG
        result = result && buffer.Allocs() == 2;
#endif
    }
    catch( const std::exception& e )
    {
        std::cerr << e.what() << std::endl;
        result = false;
    }
}
END_TEST

BEGIN_TEST( CapacityTest_1 )
{
    try
    {
        ContiguousBuffer buffer( testString.c_str(),
                                 testString.size(),
                                 testString.size() * 2 );
        
        const size_t capacityDecrement = 5;
        buffer.Capacity( buffer.Size() - capacityDecrement );
        
        result = !memcmp( buffer.Data(),
                          testString.c_str(),
                          testString.size() );
        result = result && ( buffer.Capacity() >= testString.size() * 2 );
#ifndef NDEBUG
        result = result && buffer.Allocs() == 1;
#endif
    }
    catch( const std::exception& e )
    {
        std::cerr << e.what() << std::endl;
        result = false;
    }
}
END_TEST

BEGIN_TEST( CapacityTest_2 )
{
    try
    {
        ContiguousBuffer buffer( testString.c_str(),  testString.size() );
        
        const size_t capacityMultiply = 2;
        buffer.Capacity( buffer.Capacity() * capacityMultiply );
        
        result = !memcmp( buffer.Data(),
                          testString.c_str(),
                          testString.size() );
        result = result && ( buffer.Capacity() >= capacityMultiply * testString.size() );
#ifndef NDEBUG
        result = result && buffer.Allocs() == 2;
#endif
    }
    catch( const std::exception& e )
    {
        std::cerr << e.what() << std::endl;
        result = false;
    }
}
END_TEST

BEGIN_TEST( ReadTest_1 )
{
    try
    {
        ContiguousBuffer buffer( testString.c_str(), testString.size() );
        
        std::vector< char > tmp( testString.size() / 4 );
        if( tmp.empty() )
        {
            result = false;
        }
        else
        {
            size_t bytesRead = buffer.Read( &tmp[0], tmp.size() );
            result = bytesRead == tmp.size() &&
                     !memcmp( &tmp[0], testString.c_str(), bytesRead ) &&
                     buffer.Size() == testString.size() - bytesRead;
        }
    }
    catch( const std::exception& e )
    {
        std::cerr << e.what() << std::endl;
        result = false;
    }
}
END_TEST

BEGIN_TEST( ReadTest_2 )
{
    try
    {
        ContiguousBuffer buffer( testString.c_str(), testString.size() );
        
        std::vector< char > tmp( testString.size() * 2 );
        if( tmp.empty() )
        {
            result = false;
        }
        else
        {
            size_t bytesRead = buffer.Read( &tmp[0], tmp.size() );
            result = bytesRead == tmp.size() / 2 &&
                     !memcmp( &tmp[0], testString.c_str(), bytesRead ) &&
                     !buffer.Size();
        }
    }
    catch( const std::exception& e )
    {
        std::cerr << e.what() << std::endl;
        result = false;
    }
}
END_TEST

BEGIN_TEST( ReadTest_3 )
{
    try
    {
        ContiguousBuffer buffer( testString.c_str(), testString.size() );
        try
        {
            buffer.Read( 0, 10 );
        }
        catch( const std::exception& )
        {
            result = true;
        }
    }
    catch( const std::exception& e )
    {
        std::cerr << e.what() << std::endl;
        result = false;
    }
}
END_TEST

BEGIN_TEST( WriteTest_1 )
{
    ContiguousBuffer buffer;
    
    try
    {
        buffer.Write( testString.c_str(), testString.size() );
        result = buffer.Size() == testString.size() &&
                 !memcmp( buffer.Data(), testString.c_str(), testString.size() );
    }
    catch( const std::exception& )
    {
        result = false;
    }
    
#ifndef NDEBUG
    result = result && buffer.Allocs() == 1;
#endif
}
END_TEST

BEGIN_TEST( WriteTest_2 )
{
    try
    {
        ContiguousBuffer buffer( testString.c_str(), testString.size() );
        buffer.Write( testString.c_str(), testString.size() );
        result = buffer.Size() == 2 * testString.size() &&
                 !memcmp( reinterpret_cast< char* >( buffer.Data() ) + testString.size(),
                          testString.c_str(),
                          testString.size() );
#ifndef NDEBUG
        result = result && buffer.Allocs() == 2;
#endif
    }
    catch( const std::exception& e )
    {
        std::cerr << e.what() << std::endl;
        result = false;
    }
}
END_TEST

BEGIN_TEST( WriteTest_3 )
{
    ContiguousBuffer buffer;
    
    try
    {
        buffer.Write( 0, 10 );
    }
    catch( const std::exception& )
    {
        result = true;
    }
}
END_TEST

BEGIN_TEST( AssignTest_1 )
{
    ContiguousBuffer buffer;
    
    try
    {
        buffer.Assign( testString.c_str(), testString.size() );
        result = buffer.Size() == testString.size() &&
                 !memcmp( buffer.Data(), testString.c_str(), testString.size() );
#ifndef NDEBUG
        result = result && buffer.Allocs() == 1;
#endif
    }
    catch( const std::exception& )
    {
        result = false;
    }
}
END_TEST

BEGIN_TEST( AssignTest_2 )
{
    try
    {
        const std::string tmpString = "This is some kind of temporary string";
        ContiguousBuffer buffer( tmpString.c_str(), tmpString.size() );
        
        buffer.Assign( testString.c_str(), testString.size() );
        result = buffer.Size() == testString.size() &&
                 !memcmp( buffer.Data(), testString.c_str(), testString.size() );
    }
    catch( const std::exception& e )
    {
        std::cerr << e.what() << std::endl;
        result = false;
    }
}
END_TEST

BEGIN_TEST( AssignTest_3 )
{
    ContiguousBuffer buffer;
    
    try
    {
        buffer.Assign( 0, 10 );
    }
    catch( const std::exception& )
    {
        result = true;
    }
}
END_TEST

BEGIN_TEST( ClearTest_1 )
{
    try
    {
        ContiguousBuffer buffer( testString.c_str(), testString.size() );
        buffer.Clear();
        
        result = !buffer.Size() && !buffer.Capacity() && !buffer.Data();
    }
    catch( const std::exception& e )
    {
        std::cerr << e.what() << std::endl;
        result = false;
    }
}
END_TEST

}

bool RunContiguousBufferTests()
{
    std::cout << std::endl;
    std::cout << "******** Contiguous Buffer Tests ********" << std::endl << std::endl;
    
    bool result = true;
    result = RUN_TEST( ConstructorTest_1 ) && result;
    result = RUN_TEST( ConstructorTest_2 ) && result;
    result = RUN_TEST( ConstructorTest_3 ) && result;
    result = RUN_TEST( ConstructorTest_4 ) && result;
    result = RUN_TEST( ConstructorTest_5 ) && result;
    result = RUN_TEST( SizeTest_1 ) && result;
    result = RUN_TEST( SizeTest_2 ) && result;
    result = RUN_TEST( SizeTest_3 ) && result;
    result = RUN_TEST( SizeTest_4 ) && result;
    result = RUN_TEST( SizeTest_5 ) && result;
    result = RUN_TEST( SizeTest_6 ) && result;
    result = RUN_TEST( CapacityTest_1 ) && result;
    result = RUN_TEST( CapacityTest_2 ) && result;
    result = RUN_TEST( ReadTest_1 ) && result;
    result = RUN_TEST( ReadTest_2 ) && result;
    result = RUN_TEST( ReadTest_3 ) && result;
    result = RUN_TEST( WriteTest_1 ) && result;
    result = RUN_TEST( WriteTest_2 ) && result;
    result = RUN_TEST( WriteTest_3 ) && result;
    result = RUN_TEST( AssignTest_1 ) && result;
    result = RUN_TEST( AssignTest_2 ) && result;
    result = RUN_TEST( AssignTest_3 ) && result;
    result = RUN_TEST( ClearTest_1 ) && result;
    
    std::cout << std::endl;
    std::cout << "******** Contiguous Buffer Tests: "
              << ( result ? "OK" : "FAIL" ) << " ********" << std::endl << std::endl;
    
    return result;
}
