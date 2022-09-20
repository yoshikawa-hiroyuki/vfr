/*
 * CES Utilities
 *
 * Copyright(c) FUJITSU NAGANO SYSTEMS ENGINEERING LIMITED
 *      CES Project, 2002-2004, All Right Reserved.
 */

#ifndef _CES_UTIL_ALLOCATOR_H_
#define _CES_UTIL_ALLOCATOR_H_

#include <sys/types.h>
#include <string.h>
#include <stdlib.h>

namespace CES {
#ifdef _MPK  // for SGI OpenGL Multipipe SDK
  inline void* Allocate(size_t size) {return mpkMalloc(size);}
  inline void  DeAllocate(void *ptr) {mpkFree(ptr);}
  inline void* CeAllocate(size_t nelem,size_t elsize) {
    return mpkCalloc(nelem, elsize);}
  inline void* ReAllocate(void *ptr,size_t size) {return mpkRealloc(ptr, size);}
  inline char* StrDuplicate(const char *string) {return mpkStrDup(string);}
#else // !_MPK
  inline void* Allocate(size_t size) {return malloc(size);}
  inline void  DeAllocate(void *ptr) {free(ptr);}
  inline void* CeAllocate(size_t nelem,size_t elsize) {
    return calloc(nelem, elsize);}
  inline void* ReAllocate(void *ptr,size_t size) {return realloc(ptr, size);}
  inline char* StrDuplicate(const char *string) {return strdup(string);}
#endif // _MPK
}; // end of namespace CES


#ifndef unix
#ifndef _DEBUG
#define CXX_ALLOCATOR_DEFINITION \
    void* operator new(size_t s) {return CES::Allocate(s);} \
    void  operator delete(void* p) {CES::DeAllocate(p);}
#else
#define CXX_ALLOCATOR_DEFINITION
#endif // !_DEBUG
#else // unix
#define CXX_ALLOCATOR_DEFINITION \
    void* operator new(size_t s) {return CES::Allocate(s);} \
    void  operator delete(void* p) {CES::DeAllocate(p);}
#endif // !unix

#endif // _CES_UTIL_ALLOCATOR_H_
