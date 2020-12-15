﻿// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

using System;
using System.Runtime.InteropServices;
using Microsoft.Quantum.Simulation.Core;

namespace Microsoft.Quantum.Simulation.Simulators
{
    public partial class QuantumSimulator
    {
        public virtual void Z_Body(Qubit target)
        {
            this.CheckQubit(target);

            Z(this.Id, (uint)target.Id);
        }

        public virtual void Z_ControlledBody(IQArray<Qubit> controls, Qubit target)
        {
            this.CheckQubits(controls, target);

            SafeControlled(controls,
                () => Z_Body(target),
                (count, ids) => MCZ(this.Id, count, ids, (uint)target.Id));
        }
    }
}
