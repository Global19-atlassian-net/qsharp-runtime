# Copyright (c) Microsoft Corporation. All rights reserved.
# Licensed under the MIT License.

Write-Host "##[info]Build QIR Runtime"
$oldCC = $env:CC
$oldCXX = $env:CXX

if (Test-Path Env:AGENT_OS) {
    if ($Env:AGENT_OS.StartsWith("Win")) {
        $env:CC = "C:\Program Files\LLVM\bin\clang.exe"
        $env:CXX = "C:\Program Files\LLVM\bin\clang++.exe"
        $llvmExtras = (Join-Path $PSScriptRoot "externals/LLVM")
        $env:PATH += ";$llvmExtras"
    } else {
        $env:CC = "/usr/bin/clang"
        $env:CXX = "/usr/bin/clang++"
    }
} else {
    Write-Host "QIR Runtime should be built with Clang. Please set env:CXX to point to clang++"
}

$qirRuntimeBuildFolder = (Join-Path $PSScriptRoot "build\$Env:BUILD_CONFIGURATION")
if (-not (Test-Path $qirRuntimeBuildFolder)) {
    New-Item -Path $qirRuntimeBuildFolder -ItemType "directory"
}

Push-Location $qirRuntimeBuildFolder

cmake -G Ninja -DCMAKE_BUILD_TYPE=$Env:BUILD_CONFIGURATION ../..
cmake --build . --target install

Pop-Location

$env:CC = $oldCC
$env:CXX = $oldCXX

if ($LastExitCode -ne 0) {
    Write-Host "##vso[task.logissue type=error;]Failed to build QIR Runtime."
}

