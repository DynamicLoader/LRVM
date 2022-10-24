/*  LRVM - Little RISC-V Virtual Machine

        MIT License

Copyright (c) 2022 Lucy2003

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#pragma once

// ========== User Config begin   ===================

// To debug,define it.
#define LRVM_DEBUG_ENABLED 1


// The max core count for each VM; default to 4
#ifndef LRVM_VMCPU_MAX_CORE_COUNT
#define LRVM_VMCPU_MAX_CORE_COUNT 4
#endif



// ========== User Config end     ===================

// Pre-compile check
#if (__cplusplus < 201100L)
#error Compiler with C++ 11 (or higher) standard is required to compile the library! Compile terminated.
#endif

#ifdef LRVM_DEBUG_ENABLED
#include <cstdio>
#define LRVM_DEBUG(x) x
#else
#define LRVM_DEBUG(x) 
#endif

#include "inc/lrvm_impl.hpp"

// Export class...

namespace LRVM {

// Main class for the library;the alias of LRVM::Internal::VMCPU
using VM = Internal::VMCPU;


/* To define a error handling function,follow the template
        CoreErrorCB_t <FunctionName> = [](LRVM_ExecutorArgsDef){

        };
*/
using CoreErrorCB_t = Internal::_lrvm_icb_t;

} // namespace LRVM
