# Copyright (c) Microsoft Corporation. All rights reserved.
# Licensed under the MIT License.

& "$PSScriptRoot/set-env.ps1"
$all_ok = $True

if ($Env:ENABLE_NATIVE -ne "false") {
    $nativeSimulator = (Join-Path $PSScriptRoot "../src/Simulation/Native")
    & "$nativeSimulator/test-native-simulator.ps1"
    if ($LastExitCode -ne 0) {
        $script:all_ok = $False
    }
} else {
    Write-Host "Skipping test of native simulator because ENABLE_NATIVE variable is set to: $Env:ENABLE_NATIVE."
}

if (($Env:ENABLE_NATIVE -ne "false") -and ($Env:ENABLE_QIRRUNTIME -eq "true")) {
    $qirRuntime = (Join-Path $PSScriptRoot "../src/QirRuntime")
    & "$qirRuntime/test-qir-runtime.ps1"
    if ($LastExitCode -ne 0) {
        $script:all_ok = $False
    }
} else {
    Write-Host "Skipping test of qir runtime because ENABLE_QIRRUNTIME variable is set to: $Env:ENABLE_QIRRUNTIME `
                and ENABLE_NATIVE variable is set to: $Env:ENABLE_NATIVE."
}

function Test-One {
    Param($project)

    Write-Host "##[info]Testing $project..."
    if ("" -ne "$Env:ASSEMBLY_CONSTANTS") {
        $args = @("/property:DefineConstants=$Env:ASSEMBLY_CONSTANTS");
    }  else {
        $args = @();
    }
    dotnet test $(Join-Path $PSScriptRoot $project) `
        -c $Env:BUILD_CONFIGURATION `
        -v $Env:BUILD_VERBOSITY `
        --logger trx `
        @args `
        /property:Version=$Env:ASSEMBLY_VERSION

    if ($LastExitCode -ne 0) {
        Write-Host "##vso[task.logissue type=error;]Failed to test $project"
        $script:all_ok = $False
    }
}

Test-One '../Simulation.sln'

if (-not $all_ok) {
    throw "At least one project failed during testing. Check the logs."
}
