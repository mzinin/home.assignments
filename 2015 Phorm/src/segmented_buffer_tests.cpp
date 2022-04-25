#include "segmented_buffer.h"
#include "unit_tests.h"

#include <unistd.h>

#include <cstring>
#include <iostream>
#include <stdexcept>
#include <vector>


#define BEGIN_TEST( name ) \
    bool SegmentedBuffer_ ## name () \
    { \
        std::cout << __func__ << " ... "; \
        bool result = false;
        
#define END_TEST \
        std::cout << ( result ? "OK" : "FAIL" ) << std::endl; \
        return result; \
    }
    
#define RUN_TEST( name ) SegmentedBuffer_ ## name ()

namespace
{

std::string testString = "'We called him Tortoise because he taught us,' "
                         "said the Mock Turtle angrily: 'really you are very dull!'";

void EnlargeTestString()
{
    const std::string tmp = testString;
    const int multiply = getpagesize() / 2;
    testString.reserve( multiply * tmp.size() );
    
    for( int i = 1; i < multiply; ++i )
    {
        testString += tmp;
    }
}
                         
}

namespace
{

BEGIN_TEST( ConstructorTest_1 )
{
    SegmentedBuffer buffer;
    result = !buffer.Size() && !buffer.Capacity();
}
END_TEST

BEGIN_TEST( ConstructorTest_2 )
{
    const size_t size = 50;
    const size_t capacity = 120;
    
    try
    {
        SegmentedBuffer buffer( size, capacity );
        result = buffer.Size() == size &&
                 buffer.Capacity() >= capacity;
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
        SegmentedBuffer buffer( testString.c_str(), testString.size() );
        result = buffer.Size() == testString.size() &&
                 buffer.Capacity() >= testString.size();
                 
        if( result )
        {
            std::vector< char > tmp( testString.size() );
            buffer.Read( &tmp[0], tmp.size() );
            result = !memcmp( &tmp[0], testString.c_str(), testString.size() );
        }
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
    try
    {
        SegmentedBuffer buffer( static_cast< const void* >( 0 ), testString.size() );
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
        SegmentedBuffer buffer( testString.c_str(), testString.size() );
        
        const size_t sizeDecrement = 5;
        buffer.Size( buffer.Size() - sizeDecrement );
        result = ( buffer.Size() == testString.size() - sizeDecrement );
        
        std::vector< char > tmp( std::min( testString.size(), buffer.Size() ) );
        buffer.Read( &tmp[0], tmp.size() );
        result = result && !memcmp( testString.c_str(), &tmp[0], tmp.size() );
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
        SegmentedBuffer buffer( testString.c_str(), testString.size() );
        
        const size_t sizeMultiply = 5;
        buffer.Size( buffer.Size() * sizeMultiply );
        result = ( buffer.Size() == testString.size() * sizeMultiply );
        
        std::vector< char > tmp( std::min( testString.size(), buffer.Size() ) );
        buffer.Read( &tmp[0], tmp.size() );
        result = result && !memcmp( testString.c_str(), &tmp[0], tmp.size() );
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
        SegmentedBuffer buffer( testString.c_str(), testString.size() );
        
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
            result = ( buffer.Size() == testString.size() - sizeDecrement - bytesRead );
            
            std::vector< char > tmp2( buffer.Size() );
            buffer.Read( &tmp2[0], tmp2.size() );
            result = result && !memcmp( testString.c_str() + bytesRead, &tmp2[0], tmp2.size() );
        }
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
        SegmentedBuffer buffer( testString.c_str(), testString.size() );
        
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
            result = ( buffer.Size() == ( testString.size() - bytesRead ) * sizeMultiply );
            
            std::vector< char > tmp2( buffer.Size() );
            buffer.Read( &tmp2[0], tmp2.size() );
            result = result && !memcmp( &tmp2[0],
                                        testString.c_str() + bytesRead,
                                        testString.size() - bytesRead );
        }
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
        SegmentedBuffer buffer( testString.c_str(),
                                testString.size(),
                                testString.size() * 2 );
        
        const size_t capacityDecrement = 5;
        buffer.Capacity( buffer.Size() - capacityDecrement );
        result = ( buffer.Capacity() >= testString.size() * 2 );
        
        std::vector< char > tmp( buffer.Size() );
        buffer.Read( &tmp[0], tmp.size() );
        result = result && !memcmp( testString.c_str(), &tmp[0], tmp.size() );
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
        SegmentedBuffer buffer( testString.c_str(),  testString.size() );
        
        const size_t capacityMultiply = 2;
        buffer.Capacity( buffer.Capacity() * capacityMultiply );
        result = ( buffer.Capacity() >= capacityMultiply * testString.size() );
        
        std::vector< char > tmp( buffer.Size() );
        buffer.Read( &tmp[0], tmp.size() );
        result = result && !memcmp( testString.c_str(), &tmp[0], tmp.size() );
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
        SegmentedBuffer buffer( testString.c_str(), testString.size() );
        
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
        SegmentedBuffer buffer( testString.c_str(), testString.size() );
        
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
        SegmentedBuffer buffer( testString.c_str(), testString.size() );
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
    SegmentedBuffer buffer;
    
    try
    {
        buffer.Write( testString.c_str(), testString.size() );
        result = ( buffer.Size() == testString.size() );
        
        std::vector< char > tmp( buffer.Size() );
        buffer.Read( &tmp[0], tmp.size() );
        result = result && !memcmp( &tmp[0], testString.c_str(), testString.size() );
    }
    catch( const std::exception& )
    {
        result = false;
    }
}
END_TEST

BEGIN_TEST( WriteTest_2 )
{
    try
    {
        SegmentedBuffer buffer( testString.c_str(), testString.size() );
        buffer.Write( testString.c_str(), testString.size() );
        result = buffer.Size() == 2 * testString.size();
        
        std::vector< char > tmp( buffer.Size() );
        buffer.Read( &tmp[0], tmp.size() );
        result = result && !memcmp( &tmp[ testString.size() ],
                                    testString.c_str(),
                                    testString.size() );
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
    SegmentedBuffer buffer;
    
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
    SegmentedBuffer buffer;
    
    try
    {
        buffer.Assign( testString.c_str(), testString.size() );
        result = ( buffer.Size() == testString.size() );
        
        std::vector< char > tmp( buffer.Size() );
        buffer.Read( &tmp[0], tmp.size() );
        result = result && !memcmp( &tmp[0], testString.c_str(), testString.size() );
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
        SegmentedBuffer buffer( tmpString.c_str(), tmpString.size() );
        
        buffer.Assign( testString.c_str(), testString.size() );
        result = ( buffer.Size() == testString.size() );
        
        std::vector< char > tmp( buffer.Size() );
        buffer.Read( &tmp[0], tmp.size() );
        result = result && !memcmp( &tmp[0], testString.c_str(), testString.size() );
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
    SegmentedBuffer buffer;
    
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
        SegmentedBuffer buffer( testString.c_str(), testString.size() );
        buffer.Clear();
        
        result = !buffer.Size() && !buffer.Capacity();
    }
    catch( const std::exception& e )
    {
        std::cerr << e.what() << std::endl;
        result = false;
    }
}
END_TEST

}

bool RunSegmentedBufferTests()
{
    std::cout << std::endl;
    std::cout << "******** Segmented Buffer Tests ********" << std::endl << std::endl;
    
    EnlargeTestString();
    
    bool result = true;
    result = RUN_TEST( ConstructorTest_1 ) && result;
    result = RUN_TEST( ConstructorTest_2 ) && result;
    result = RUN_TEST( ConstructorTest_3 ) && result;
    result = RUN_TEST( ConstructorTest_4 ) && result;
    result = RUN_TEST( SizeTest_1 ) && result;
    result = RUN_TEST( SizeTest_2 ) && result;
    result = RUN_TEST( SizeTest_3 ) && result;
    result = RUN_TEST( SizeTest_4 ) && result;
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
    std::cout << "******** Segmented Buffer Tests: "
              << ( result ? "OK" : "FAIL" ) << " ********" << std::endl << std::endl;
    
    return result;
}
