/*
 * CES Utilities
 *
 * Copyright(c) FUJITSU NAGANO SYSTEMS ENGINEERING LIMITED
 *      CES Project, 2006, All Right Reserved.
 */
#ifndef _CES_UTIL_OGL_SL_H_
#define _CES_UTIL_OGL_SL_H_

#if defined(WIN32) && !defined(__CYGWIN__)
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#define GL_GLEXT_PROTOTYPES 1
#include "glext.h"
#else // WIN32
#define GL_GLEXT_PROTOTYPES 1
#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else // __APPLE__
#include <GL/gl.h>
#include <GL/glu.h>
#endif // __APPLE__
#endif // WIN32

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <vector>

#ifndef GL_VERSION_2_0
/* GL type for program/shader text */
typedef char GLchar;
#endif


namespace CES {

  /*--------------------------------------------------------------------
    struct GLSLProg : for GL_ARB_shading_language_100
   --------------------------------------------------------------------*/
  struct GLSLProg {
    GLuint program;

#if defined(WIN32)
    PFNGLCREATESHADERPROC glCreateShader;
    PFNGLSHADERSOURCEPROC glShaderSource;
    PFNGLCOMPILESHADERPROC glCompileShader;
    PFNGLGETSHADERIVPROC glGetShaderiv;
    PFNGLCREATEPROGRAMPROC glCreateProgram;
    PFNGLATTACHSHADERPROC glAttachShader;
    PFNGLDELETESHADERPROC glDeleteShader;
    PFNGLLINKPROGRAMPROC glLinkProgram;
    PFNGLGETPROGRAMIVPROC glGetProgramiv;
    PFNGLDELETEPROGRAMPROC glDeleteProgram;
    PFNGLUSEPROGRAMPROC glUseProgram;
    PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
    PFNGLUNIFORM1IPROC glUniform1i;
    PFNGLUNIFORM1FPROC glUniform1f;
#endif // WIN32

    //----------------- constructors / destructor -----------------
    GLSLProg() : program(0) {
#if defined(WIN32)
      glCreateShader = NULL;
      glShaderSource = NULL;
      glCompileShader = NULL;
      glGetShaderiv = NULL;
      glCreateProgram = NULL;
      glAttachShader = NULL;
      glDeleteShader = NULL;
      glLinkProgram = NULL;
      glGetProgramiv = NULL;
      glDeleteProgram = NULL;
      glUseProgram = NULL;
      glGetUniformLocation = NULL;
      glUniform1i = NULL;
      glUniform1f = NULL;
#endif // WIN32
    }
    ~GLSLProg() {Delete();}

    void GetShaderInfoLog(GLuint shader) {
#ifdef GL_VERSION_2_0
      GLsizei bufSize;
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH , &bufSize);
      if ( bufSize > 1 ) {
        GLchar *infoLog = (GLchar*)malloc(bufSize);
        if ( infoLog != NULL ) {
	  memset(infoLog, 0, bufSize);
	  GLsizei length;
	  glGetShaderInfoLog(shader, bufSize, &length, infoLog);
	  printf("InfoLog:\n%s\n\n", infoLog);
	  free(infoLog);
        }
        else
	  printf("Could not allocate InfoLog buffer.\n");
      }
#endif
    }

    //----------------- create GLSL program from sources -----------------
    bool Create(const GLchar* vps, const GLchar* fps) {
#ifdef GL_VERSION_2_0
      GLuint vertShader =0, fragShader =0;
      GLint compiled, linked;
      GLint slen;

      Delete();

      if ( vps && (slen = strlen(vps)) > 0 ) {
	vertShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertShader, 1, &vps, &slen);
	glCompileShader(vertShader);
	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &compiled);
	if ( compiled == GL_FALSE ) {
	  GLint maxLength = 0;
	  glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &maxLength);
	  std::vector<GLchar> errorLog(maxLength);
	  glGetShaderInfoLog(vertShader, maxLength, &maxLength, &errorLog[0]);
	  fprintf(stderr, "vertShader error:\n%s\n", &errorLog[0]);
	  glDeleteShader(vertShader);
	  return false;
	}
      }

      if ( fps && (slen = strlen(fps)) > 0 ) {
	fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &fps, &slen);
	glCompileShader(fragShader);
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &compiled);
	if ( compiled == GL_FALSE ) {
	  GLint maxLength = 0;
	  glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &maxLength);
	  std::vector<GLchar> errorLog(maxLength);
	  glGetShaderInfoLog(fragShader, maxLength, &maxLength, &errorLog[0]);
	  fprintf(stderr, "fragShader error:\n%s\n", &errorLog[0]);
	  glDeleteShader(fragShader);
	  return false;
	}
      }

      program = glCreateProgram();
      if ( ! program ) return false;
      if ( vertShader ) {
	glAttachShader(program, vertShader);
	glDeleteShader(vertShader);
      }
      if ( fragShader ) {
	glAttachShader(program, fragShader);
	glDeleteShader(fragShader);
      }

      glLinkProgram(program);
      glGetProgramiv(program, GL_LINK_STATUS, &linked);
      if ( linked == GL_FALSE ) {
	GLint maxLength = 0;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
	std::vector<GLchar> errorLog(maxLength);
	glGetProgramInfoLog(program, maxLength, &maxLength, &errorLog[0]);
	fprintf(stderr, "program link error:\n%s\n", &errorLog[0]);
	Delete();
	return false;
      }

      return true;
#else
      return false;
#endif // GL_VERSION_2_0
    }

    //----------------- delete GLSL program -----------------
    void Delete() {
#ifdef GL_VERSION_2_0
      if ( program ) {
	glDeleteProgram(program);
	program = 0;
      }
#endif // GL_VERSION_2_0
    }

    //----------------- use GLSL program -----------------
    bool Apply() const {
#ifdef GL_VERSION_2_0
      if ( ! program ) return false;
      glUseProgram(program);
      return true;
#else
      return false;
#endif // GL_VERSION_2_0
    }
    void UnApply() const {
#ifdef GL_VERSION_2_0
      glUseProgram(0);
#endif // GL_VERSION_2_0
    }

    //----------------- uniform location -----------------
    GLint GetUnifLoc(const GLchar* name) const {
#ifdef GL_VERSION_2_0
      if ( ! name || strlen(name) < 1 ) return -1;
      return glGetUniformLocation(program, name);
#else
      return -1;
#endif // GL_VERSION_2_0
    }
    bool SetUnifLocI1(const GLchar* name, const GLint val) const {
#ifdef GL_VERSION_2_0
      GLint loc = GetUnifLoc(name);
      if ( loc < 0 ) return false;
      glUniform1i(loc, val);
      return true;
#else
      return false;
#endif // GL_VERSION_2_0
    }
    bool SetUnifLocF1(const GLchar* name, const GLfloat val) const {
#ifdef GL_VERSION_2_0
      GLint loc = GetUnifLoc(name);
      if ( loc < 0 ) return false;
      glUniform1f(loc, val);
      return true;
#else
      return false;
#endif // GL_VERSION_2_0
    }

    //----------------- initialize (for WIN32) -----------------
    bool Init() {
#ifndef GL_VERSION_2_0
      return false;
#endif // GL_VERSION_2_0

      int err = 0;
#if defined(WIN32)
#define XADDR(functype, funcname) \
  ((funcname = (functype)wglGetProcAddress( #funcname )) == 0)
      err |= XADDR(PFNGLCREATESHADERPROC, glCreateShader);
      err |= XADDR(PFNGLSHADERSOURCEPROC, glShaderSource);
      err |= XADDR(PFNGLCOMPILESHADERPROC, glCompileShader);
      err |= XADDR(PFNGLGETSHADERIVPROC, glGetShaderiv);
      err |= XADDR(PFNGLCREATEPROGRAMPROC, glCreateProgram);
      err |= XADDR(PFNGLATTACHSHADERPROC, glAttachShader);
      err |= XADDR(PFNGLDELETESHADERPROC, glDeleteShader);
      err |= XADDR(PFNGLLINKPROGRAMPROC, glLinkProgram);
      err |= XADDR(PFNGLGETPROGRAMIVPROC, glGetProgramiv);
      err |= XADDR(PFNGLDELETEPROGRAMPROC, glDeleteProgram);
      err |= XADDR(PFNGLUSEPROGRAMPROC, glUseProgram);
      err |= XADDR(PFNGLGETUNIFORMLOCATIONPROC, glGetUniformLocation);
      err |= XADDR(PFNGLUNIFORM1IPROC, glUniform1i);
      err |= XADDR(PFNGLUNIFORM1FPROC, glUniform1f);
#undef XADDR
#endif // WIN32
      return (err == 0);
    }

  }; // end of class(GLSLProg)

}; // end of namespace(CES)
#endif // _CES_UTIL_OGL_SL_H_
