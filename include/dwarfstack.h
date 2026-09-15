/*
 * Copyright (C) 2013-2025 Hannes Domani
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */


#ifndef __DWARFSTACK_H__
#define __DWARFSTACK_H__

#include <stdint.h>


#if defined(DWST_STATIC)
#define EXPORT
#elif defined(DWST_SHARED)
#define EXPORT __declspec(dllexport)
#else
#define EXPORT __declspec(dllimport)
#endif


#ifdef __cplusplus
extern "C" {
#endif


// dwstCallback(): callback function
//   addr:              stack address
//   filename:          source file location
//   lineno:            line number
//   funcname:          function name
//   context:           user-provided pointer (callbackContext)
//   columnno:          column number
// For an address inside inlined code, the callback is called once per
// inlined function (innermost first) and finally for the containing
// function; addr is 0 for all but the first of these calls.
typedef void dwstCallback(
    uint64_t addr,const char *filename,int lineno,const char *funcname,
    void *context,int columnno );

typedef void dwstCallbackW(
    uint64_t addr,const wchar_t *filename,int lineno,const char *funcname,
    void *context,int columnno );

// special values for lineno:

// DWST_BASE_ADDR: inform about the used image base address
//   (important in case it's not the same as the preferred image base address)
//   addr:              used image base address
//   filename:          executable location
#define DWST_BASE_ADDR           0

// DWST_NO_DBG_SYM: no debug information available
//   addr:              stack address
//   filename:          executable location
#define DWST_NO_DBG_SYM         -1

// DWST_NO_SRC_FILE: no source file information available
//   addr:              stack address
//   filename:          executable location
#define DWST_NO_SRC_FILE        -2

// DWST_NOT_FOUND: no information available (invalid address?)
//   addr:              stack address
//   filename:          executable location
#define DWST_NOT_FOUND          -3

// DWST_LINE_ADDR: start address of the source line (only dwstOfModule())
//   addr:              line address
//                        (0 for the outer frames of inlined code)
//   filename:          NULL
//   funcname:          function name
#define DWST_LINE_ADDR          -4

// DWST_FUNC_ADDR: start address of the function (only dwstOfModule())
//   addr:              function address
//   filename:          NULL
//   funcname:          function name
#define DWST_FUNC_ADDR          -5


// dwstOfFile(): stack information of file
//   name:              executable location
//   imageBase:         used image base address
//   addr:              stack addresses
//   count:             number of addresses
//   callbackFunc:      callback function
//   callbackContext:   user-provided pointer (context)
//      (for example see examples/addr2line/)
EXPORT int dwstOfFile(
    const char *name,uint64_t imageBase,
    uint64_t *addr,int count,
    dwstCallback *callbackFunc,void *callbackContext );

EXPORT int dwstOfFileW(
    const wchar_t *name,uint64_t imageBase,
    uint64_t *addr,int count,
    dwstCallbackW *callbackFunc,void *callbackContext );


typedef struct dwst_module dwst_module;

// dwstModuleOpen(): prepare debug information for repeated lookups
//   dbg:               Dwarf_Debug of the executable
//                        (has to stay valid until dwstModuleClose())
//   imageBase_dbg:     preferred image base address of the executable
EXPORT dwst_module *dwstModuleOpen(
    void *dbg,uint64_t imageBase_dbg );

// dwstModuleClose(): free module created by dwstModuleOpen()
//   (dbg itself is not freed)
//   module:            module handle
EXPORT void dwstModuleClose(
    dwst_module *module );

// dwstOfModule(): stack information of module
//   (like dwstOfFile(), but additionally reports DWST_FUNC_ADDR and
//   DWST_LINE_ADDR before each resolved frame)
//   module:            module handle from dwstModuleOpen()
//   name:              executable location
//   imageBase:         used image base address
//   addr:              stack addresses
//   count:             number of addresses
//   callbackFunc:      callback function
//   callbackContext:   user-provided pointer (context)
EXPORT int dwstOfModule(
    dwst_module *module,const char *name,uint64_t imageBase,
    uint64_t *addr,int count,
    dwstCallback *callbackFunc,void *callbackContext );

EXPORT int dwstOfModuleW(
    dwst_module *module,const wchar_t *name,uint64_t imageBase,
    uint64_t *addr,int count,
    dwstCallbackW *callbackFunc,void *callbackContext );


// dwstOfProcess(): stack information of current process
//   addr:              stack addresses
//   count:             number of addresses
//   callbackFunc:      callback function
//   callbackContext:   user-provided pointer (context)
EXPORT int dwstOfProcess(
    uintptr_t *addr,int count,
    dwstCallback *callbackFunc,void *callbackContext );

EXPORT int dwstOfProcessW(
    uintptr_t *addr,int count,
    dwstCallbackW *callbackFunc,void *callbackContext );


// dwstOfLocation(): stack information of current location
//   callbackFunc:      callback function
//   callbackContext:   user-provided pointer (context)
//      (for example see examples/location/)
EXPORT int dwstOfLocation(
    dwstCallback *callbackFunc,void *callbackContext );

EXPORT int dwstOfLocationW(
    dwstCallbackW *callbackFunc,void *callbackContext );


// dwstOfException(): stack information of exception
//   context:           ContextRecord of exception
//   callbackFunc:      callback function
//   callbackContext:   user-provided pointer (context)
//      (for example see examples/exception/)
EXPORT int dwstOfException(
    void *context,
    dwstCallback *callbackFunc,void *callbackContext );

EXPORT int dwstOfExceptionW(
    void *context,
    dwstCallbackW *callbackFunc,void *callbackContext );


// dwstExceptionDialog(): show dialog on unhandled exception
//   extraInfo:         extra information shown in dialog
//      (for example see examples/exception-dialog/)
EXPORT void dwstExceptionDialog(
    const char *extraInfo );

EXPORT void dwstExceptionDialogW(
    const wchar_t *extraInfo );


#ifndef DWST_STATIC
// dwstDemangle(): demangle gcc style c++ symbols
//   mangled:           mangled name
//   demangled:         demangled name
//   length:            size of demangled buffer
EXPORT size_t dwstDemangle(
    const char *mangled,
    char *demangled,size_t length );
#endif


#ifdef __cplusplus
}
#endif

#endif
