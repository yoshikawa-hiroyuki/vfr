#ifndef _VFR_DEFS_H_
#define _VFR_DEFS_H_

#ifdef __APPLE__
#ifndef unix
#define unix
#endif
#endif // __APPLE__

/* dll export */
#ifdef WINDOWS
# include "stdafx.h"
# ifdef _MAKE_DLL
#  define EXPORTED_CLASS AFX_EXT_CLASS
#  define EXPORTED_CFUNC extern "C" __declspec(dllexport)
# else
#  define EXPORTED_CLASS
#  define EXPORTED_CFUNC
# endif
#else
# define EXPORTED_CLASS
# define EXPORTED_CFUNC
#endif

/* OpenGL */
#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else // __APPLE__
#include <GL/gl.h>
#include <GL/glu.h>
#endif // __APPLE__

/* GL context */
#ifdef WINDOWS
typedef HGLRC vfrGlContext;
#else
# ifndef __APPLE__
#include <GL/glx.h>
typedef GLXContext vfrGlContext;
# endif // __APPLE__
#endif 

# ifdef __APPLE__
#include <stdlib.h>
# else // __APPLE__
#include <malloc.h>
# endif // __APPLE__

#include <string.h>


/* Boolean type */
#ifndef Bool
#define Bool  int
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE  !FALSE
#endif

#if 0 // for wx-2.9
#ifndef NONE
#define NONE  0
#endif
#endif


/* Define Small numeric */
#ifndef EPSF
#define EPSF  1e-6f
#endif


/* Define PI/2 */
#ifdef  M_PI_2
#define HALF_PI    M_PI_2
#else
#define HALF_PI    1.5707963f
#endif


namespace VFR {

  /* Vector/Matrix types */
  typedef float matrix44[4][4];
  typedef float matrix4[16];
  typedef float vector4[4];
  typedef float vector3[3];
  typedef float vector2[2];

  /* Point2 structure */
  struct Point2 {
    int x, y;
    Point2() : x(0), y(0) {}
    Point2(const Point2& p) : x(p.x), y(p.y) {}
    Point2 operator+(const Point2& p) {
      Point2 r(*this); r.x += p.x; r.y += p.y; return r;
    }
    Point2 operator-(const Point2& p) {
      Point2 r(*this); r.x -= p.x; r.y -= p.y; return r;
    }
    Bool operator==(const Point2& p) {
      return ((x==p.x) && (y==p.y));
    }
  };

  enum {
    RT_NONE =0, RT_SMOOTH =(0x1<<0), RT_FLAT =(0x1<<1), RT_NOLIGHT =(0x1<<2),
    RT_WIRE =(0x1<<3), RT_POINT =(0x1<<4), RT_NOTEXTURE =(0x1<<5)
  };
  typedef int RenderType;

  enum {PT_NONE =0, PT_OBJECT =(0x1<<0), PT_BBOX =(0x1<<1)};
  typedef int PickType;

  enum AppearType {AT_WHOLE, AT_PER_VERTEX, AT_PER_FACE};
  enum FeedbackType {FB_VERTEX, FB_FACE, FB_EDGE};
  enum StippleType {ST_SOLID, ST_DOT, ST_DASH, ST_DDASH1, ST_DDASH2};
  enum SymbolType {SYM_NORMAL, SYM_PLUS, SYM_CROSS,
		   SYM_CIRCLE, SYM_CIRCFILL, SYM_SQUARE, SYM_SQUAFILL,
		   SYM_TRIANGLE, SYM_TRIAFILL};
  enum LightType {LT_DIRECTIONAL, LT_POINT, LT_SPOT};
  enum PolyFaceType {PF_FRONT, PF_BACK, PF_BOTH};
  enum AlignType {AL_LEFT, AL_RIGHT, AL_CENTER};
  enum ProjectType {PR_PERSPECTIVE, PR_ORTHOGONAL};

  // Allocator 
  inline void* Allocate(size_t size) {return malloc(size);}
  inline void  DeAllocate(void *ptr) {free(ptr);}
  inline void* CeAllocate(size_t nelem,size_t elsize) {
    return calloc(nelem, elsize);
  }
  inline void* ReAllocate(void *ptr,size_t size) {
    return realloc(ptr, size);
  }
  inline char* StrDuplicate(const char *string) {
    return strdup(string);
  }

  // OpenGL utils
  inline Bool QueryGlExt(const char* extName) {
    char* p = (char*)glGetString(GL_EXTENSIONS);
    char* end = p + strlen(p);
    while (p < end) {
      size_t n = strcspn(p, " ");
      if ((strlen(extName) == n) && (strncmp(extName, p, n) == 0))
	return TRUE;
      p += (n + 1);
    }
    return FALSE;
  }

}; // end of namespace VFR


#endif /* _VFR_DEFS_H_ */
