#include "unit_tests.h"
#include <cstdlib>

int main()
{
    bool result = RunContiguousBufferTests() &&
                  RunSegmentedBufferTests();
    
    return result ? EXIT_SUCCESS : EXIT_FAILURE;
}
