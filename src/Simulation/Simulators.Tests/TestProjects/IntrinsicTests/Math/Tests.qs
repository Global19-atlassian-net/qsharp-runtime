// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

namespace Microsoft.Quantum.Tests {
    open Microsoft.Quantum.Math;
    open Microsoft.Quantum.Diagnostics;

    @Test("QuantumSimulator")
    function AbsDIsCorrect() : Unit {
        EqualityFactD(AbsD(-1.23), 1.23, "AbsD was incorrect for negative numbers.");
        EqualityFactD(AbsD(1.23), 1.23, "AbsD was incorrect for positive numbers.");
        EqualityFactD(AbsD(0.0), 0.0, "AbsD was incorrect for zero.");
    }

    @Test("QuantumSimulator")
    function AbsIIsCorrect() : Unit {
        EqualityFactI(AbsI(-123), 123, "AbsI was incorrect for negative numbers.");
        EqualityFactI(AbsI(123), 123, "AbsI was incorrect for positive numbers.");
        EqualityFactI(AbsI(0), 0, "AbsI was incorrect for zero.");
    }

    @Test("QuantumSimulator")
    function AbsLIsCorrect() : Unit {
        EqualityFactL(AbsL(-123L), 123L, "AbsL was incorrect for negative numbers.");
        EqualityFactL(AbsL(123L), 123L, "AbsL was incorrect for positive numbers.");
        EqualityFactL(AbsL(0L), 0L, "AbsL was incorrect for zero.");
    }

    @Test("QuantumSimulator")
    function Log10IsCorrect() : Unit {
        EqualityWithinToleranceFact(Log10(0.456), -0.341035157335565, 1e-7, "Log10(0.456) was incorrect.");
        EqualityWithinToleranceFact(Log10(1.0), 0.0, 1e-7, "Log10(1.0) was incorrect.");
        EqualityWithinToleranceFact(Log10(1.23), 0.0899051114393979, 1e-7, "Log10(1.23) was incorrect.");
        EqualityWithinToleranceFact(Log10(10.0), 1.0, 1e-7, "Log10(10.0) was incorrect.");
        EqualityWithinToleranceFact(Log10(100.0), 2.0, 1e-7, "Log10(100.0) was incorrect.");
        EqualityWithinToleranceFact(Log10(123.456), 2.09151220162777, 1e-7, "Log10(123.456) was incorrect.");
    }

    @Test("QuantumSimulator")
    function MaxDIsCorrect() : Unit {
        EqualityFactD(MaxD(-1.0, 2.0), 2.0, "MaxD was incorrect.");
    }

    @Test("QuantumSimulator")
    function MaxIIsCorrect() : Unit {
        EqualityFactI(MaxI(-1, 2), 2, "MaxI was incorrect.");
    }

    @Test("QuantumSimulator")
    function CeilingIsCorrect() : Unit {
        EqualityWithinToleranceFact(Ceiling(3.1), 4.0, 1e-7, "Ceiling(3.1) was incorrect.");
        EqualityWithinToleranceFact(Ceiling(3.7), 4.0, 1e-7, "Ceiling(3.7) was incorrect.");
        EqualityWithinToleranceFact(Ceiling(-3.1), -3.0, 1e-7, "Ceiling(-3.1) was incorrect.");
        EqualityWithinToleranceFact(Ceiling(-3.7), -3.0, 1e-7, "Ceiling(-3.7) was incorrect.");
    }

    @Test("QuantumSimulator")
    function FloorIsCorrect() : Unit {
        EqualityWithinToleranceFact(Floor(3.1), 3.0, 1e-7, "Floor(3.1) was incorrect.");
        EqualityWithinToleranceFact(Floor(3.7), 3.0, 1e-7, "Floor(3.7) was incorrect.");
        EqualityWithinToleranceFact(Floor(-3.1), -4.0, 1e-7, "Floor(-3.1) was incorrect.");
        EqualityWithinToleranceFact(Floor(-3.7), -4.0, 1e-7, "Floor(-3.7) was incorrect.");
    }

    @Test("QuantumSimulator")
    function RoundIsCorrect() : Unit {
        EqualityWithinToleranceFact(Round(3.1), 3.0, 1e-7, "Round(3.1) was incorrect.");
        EqualityWithinToleranceFact(Round(3.7), 4.0, 1e-7, "Round(3.7) was incorrect.");
        EqualityWithinToleranceFact(Round(-3.1), -3.0, 1e-7, "Round(-3.1) was incorrect.");
        EqualityWithinToleranceFact(Round(-3.7), -4.0, 1e-7, "Round(-3.7) was incorrect.");
    }

    @Test("QuantumSimulator")
    function PowDIsCorrect() : Unit {
        EqualityWithinToleranceFact(PowD(2.1234, 3.4567), 13.7380892872214, 1e-7, "PowD(2.1234, 3.4567) was incorrect.");
        EqualityWithinToleranceFact(PowD(0.4567, 9.10111213), 0.000798479316935851, 1e-8, "PowD(0.4567, 9.10111213) was incorrect.");
        EqualityWithinToleranceFact(PowD(13.14151617, -1.98765), 0.00597756523034148, 1e-8, "PowD(13.14151617, -1.98765) was incorrect.");
    }

    @Test("QuantumSimulator")
    function ModPowL() : Unit {
        EqualityFactL(ModPowL(117L, 391L, 119L), 110L, "ModPowL(117L, 391L, 119L) was incorrect.");
        EqualityFactL(ModPowL(117L, 5792L, 119L), 18L, "ModPowL(117L, 5792L, 119L) was incorrect.");
        EqualityFactL(ModPowL(8675309L, 5792L, 2345678L), 1936199L, "ModPowL(8675309L, 5792L, 2345678L) was incorrect.");
    }

}
