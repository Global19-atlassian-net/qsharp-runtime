// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

using System;
using Microsoft.Quantum.Simulation.Core;

namespace Microsoft.Quantum.Intrinsic.Interfaces
{
    public interface IGate_Y : IOperationFactory
    {
        void Y_Body(Qubit target);

        void Y_ControlledBody(IQArray<Qubit> controls, Qubit target);
    }
}