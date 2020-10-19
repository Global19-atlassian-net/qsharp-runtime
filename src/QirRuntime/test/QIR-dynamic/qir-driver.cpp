#include <iostream>

#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "QAPI.hpp"

// Can manually add calls to DebugLog in the ll files for debugging.
extern "C" void DebugLog(int64_t value)
{
    std::cout << value << std::endl;
}
extern "C" void DebugLogPtr(char* value)
{
    std::cout << (const void*)value << std::endl;
}

// This VQE sample is taken from https://github.com/msr-quarc/StandaloneVQE
// When executed in Q# it returns -1.0 with single decimal precision (the correct energy value is close to -1.3)
extern "C" double Microsoft__Quantum__Samples__Chemistry__SimpleVQE__GetEnergyHydrogenVQE__body(); // NOLINT
TEST_CASE("QIR: SimpleVQE with full state simulator", "[qir]")
{
    QuantumExecutionContext* ctx = QAPI_CreateContext_Fullstate();

    const double expectedEV = -1.0;
    const double ret = Microsoft__Quantum__Samples__Chemistry__SimpleVQE__GetEnergyHydrogenVQE__body();
    INFO(ret);
    REQUIRE(std::abs(ret - expectedEV) < 0.1);

    QAPI_DestroyContext(ctx);
}