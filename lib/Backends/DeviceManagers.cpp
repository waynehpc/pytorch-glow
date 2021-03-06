/**
 * Copyright (c) 2017-present, Facebook, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "glow/Backends/DeviceManager.h"
#include "glow/Backends/DummyDeviceManager.h"

#include "llvm/Support/Casting.h"
#include "llvm/Support/raw_ostream.h"

using namespace glow;
using namespace glow::runtime;

namespace glow {
namespace runtime {
/// NOTE: Please add a declaration of a device-specific `create` method here
/// when you define a new DeviceManager.

/// Create a new instance of the interpreter Device.
DeviceManager *createInterpreterDeviceManager(llvm::StringRef name);

#if defined(GLOW_WITH_CPU)
/// Create a new instance of the CPUBackend DeviceManager.
DeviceManager *createCPUDeviceManager(llvm::StringRef name);
#else
DeviceManager *createCPUDeviceManager(llvm::StringRef name) {
  (void)name;
  GLOW_UNREACHABLE("Must compile with CPU support");
}
#endif

#if defined(GLOW_WITH_OPENCL)
/// Create a new instance of the OpenCL backend.
DeviceManager *createOCLDeviceManager(llvm::StringRef name);
#else
DeviceManager *createOCLDeviceManager(llvm::StringRef name) {
  (void)name;
  GLOW_UNREACHABLE("Must compile with OpenCL support");
}
#endif
} // namespace runtime
} // namespace glow

DeviceManager *DeviceManager::createDeviceManager(BackendKind backendKind,
                                                  llvm::StringRef name) {
  switch (backendKind) {
  case BackendKind::Interpreter:
    return createInterpreterDeviceManager(name);
  case BackendKind::OpenCL:
    return createOCLDeviceManager(name);
  case BackendKind::CPU:
    return createCPUDeviceManager(name);
  default:
    // As a fallback to make developing new Backends easier we'll create a
    // DummyDeviceManager here, but this is not threadsafe and very simplistic.
    // Strongly recommended that you create a DeviceManager customized for your
    // device.
    llvm::errs() << "Warning: Creating a DummyDeviceManager.\n";
    return new DummyDeviceManager(backendKind, name);
  }

  // This is to make compiler happy. It can never reach this point as switch
  // always covers all possible values.
  llvm_unreachable("unreachable");
}
