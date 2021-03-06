﻿// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

using System;
using Microsoft.Quantum.Simulation.Core;

namespace Microsoft.Quantum.Simulation.Simulators
{
    public partial class ToffoliSimulator
    {
        /// <summary>
        /// The implementation of the operation.
        /// For the Toffoli simulator, the implementation throws a run-time error.
        /// </summary>
        public void S__Body(Qubit target) => throw new NotImplementedException();

        public void S__ControlledBody(IQArray<Qubit> controls, Qubit target) => throw new NotImplementedException();

        public void S__AdjointBody(Qubit target) => throw new NotImplementedException();

        public void S__ControlledAdjointBody(IQArray<Qubit> controls, Qubit target) => throw new NotImplementedException();
    }
}
