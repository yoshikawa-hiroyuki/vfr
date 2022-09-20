/*
 * CES Utilities
 *
 * Copyright(c) FUJITSU NAGANO SYSTEMS ENGINEERING LIMITED
 *      CES Project, 2002-2004, All Right Reserved.
 */
#ifndef _CES_UTIL_OGL_OBJ_H_
#define _CES_UTIL_OGL_OBJ_H_

#if defined(_WIN32) && !defined(__CYGWIN__)
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#include <GL/gl.h>
#else // WIN32
#define GL_GLEXT_PROTOTYPES 1
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif
#endif // WIN32

#include <map>


namespace CES {

  //--------------------------------------------------------------------
  // class DispList : for OpenGL display list
  //--------------------------------------------------------------------
  class DispList {
  public:
    GLuint dl_no;

    DispList() : dl_no(0) {}
    virtual ~DispList() {DelList();}

    bool IsValid() const {return (dl_no > 0);}

    bool CallList() const {
      if ( ! IsValid() ) return false;
      glCallList(dl_no); return true;
    }
    bool NewList(const GLenum mode =GL_COMPILE_AND_EXECUTE) {
      if ( ! GenList() ) return false;
      glNewList(dl_no, mode); return true;
    }
    void EndList() const {if ( IsValid() ) glEndList();}

    bool GenList() {
      if ( ! IsValid() ) dl_no = glGenLists(1);
      return IsValid();
    }
    void DelList() {
      if ( IsValid() ) glDeleteLists(dl_no, 1);
      dl_no = 0;
    }
  };

  //--------------------------------------------------------------------
  // class TexObj : for OpenGL texture object
  //--------------------------------------------------------------------
  class TexObj {
  public:
    GLuint tex_id;
    GLenum target;

    TexObj(const GLenum targ =GL_TEXTURE_2D) : target(targ), tex_id(0) {}
    virtual ~TexObj() {DelObj();}

    bool IsValid() const {return (tex_id > 0 && target);}
    GLenum GetTargetType() const {return target;}

    bool Bind() {
      if ( ! GenObj() ) return false;
      glBindTexture(target, tex_id); return true;
    }
    void Enable()  const {glEnable(target);}
    void Disable() const {glDisable(target);}

    bool GenObj() {
      if ( ! IsValid() ) glGenTextures(1, &tex_id);
      return IsValid();
    }
    void DelObj() {
      if ( IsValid() ) glDeleteTextures(1, &tex_id);
      tex_id = 0;
    }

    bool SetParam(const GLenum pname, const GLint val) const {
      if ( ! IsValid() ) return false;
      glTexParameteri(target, pname, val); return true;
    }
    bool SetParam(const GLenum pname, const GLfloat val) const {
      if ( ! IsValid() ) return false;
      glTexParameterf(target, pname, val); return true;
    }
    bool SetParam(const GLenum pname, const GLint* val) const {
      if ( ! IsValid() ) return false;
      glTexParameteriv(target, pname, val); return true;
    }
    bool SetParam(const GLenum pname, const GLfloat* val) const {
      if ( ! IsValid() ) return false;
      glTexParameterfv(target, pname, val); return true;
    }
  };

  //-------------- TexObj class for TEXTURE_1D --------------
  class TexObj1D : public TexObj {
  public:
    TexObj1D() : TexObj(GL_TEXTURE_1D) {}
  };

  //-------------- TexObj class for TEXTURE_2D --------------
  class TexObj2D : public TexObj {
  public:
    TexObj2D() : TexObj(GL_TEXTURE_2D) {}
  };

  //-------------- TexObj class for TEXTURE_3D --------------
  class TexObj3D : public TexObj {
  public:
#if defined(GL_TEXTURE_3D)
    TexObj3D() : TexObj(GL_TEXTURE_3D) {}
#elif defined(GL_TEXTURE_3D_EXT)
    TexObj3D() : TexObj(GL_TEXTURE_3D_EXT) {}
#else
    TexObj3D() : TexObj(0) {}
#endif
  };

  //-------------- TexObj class for TEXTURE_RECTANGLE --------------
  class TexObj2D_RECT : public TexObj {
  public:
#if defined(GL_EXT_texture_rectangle)
    TexObj2D_RECT() : TexObj(GL_TEXTURE_RECTANGLE_EXT) {}
#elif defined(GL_NV_texture_rectangle)
    TexObj2D_RECT() : TexObj(GL_TEXTURE_RECTANGLE_NV) {}
#else
    TexObj2D_RECT() : TexObj(0) {}
#endif
  };


  //--------------------------------------------------------------------
  // struct OglObjManager : 
  //--------------------------------------------------------------------
  struct OglObjManager {
    typedef std::multimap<unsigned, DispList*>::iterator DispListItr;
    typedef std::multimap<unsigned, TexObj*>::iterator TexObjItr;

    std::multimap<unsigned, DispList*> m_dispLists;
    std::multimap<unsigned, TexObj*> m_texObjs;

    bool AddDispList(const unsigned key, DispList* dlst) {
      if ( ! dlst ) return false;
      std::multimap<unsigned, DispList*>::iterator r =
        m_dispLists.insert(std::make_pair(key, dlst));
      return (r != m_dispLists.end());
    }
    bool AddTexObj(const unsigned key, TexObj* tobj) {
      if ( ! tobj ) return false;
      std::multimap<unsigned, TexObj*>::iterator r =
        m_texObjs.insert(std::make_pair(key, tobj));
      return (r != m_texObjs.end());
    }

    void DelDispList(const unsigned key) {
      std::multimap<unsigned, DispList*>::iterator it;
      it = m_dispLists.find(key);
      while ( it != m_dispLists.end() ) {
        m_dispLists.erase(it);
        it = m_dispLists.find(key);
      }
    }
    void DelTexObj(const unsigned key) {
      std::multimap<unsigned, TexObj*>::iterator it;
      it = m_texObjs.find(key);
      while ( it != m_texObjs.end() ) {
        m_texObjs.erase(it);
        it = m_texObjs.find(key);
      }
    }

    DispListItr FindDispList(const unsigned key) {
      return m_dispLists.lower_bound(key);
    }
    std::pair<DispListItr, DispListItr> FindDispLists(const unsigned key) {
      return m_dispLists.equal_range(key);
    }
    TexObjItr FindTexObj(const unsigned key) {
      return m_texObjs.lower_bound(key);
    }
    std::pair<TexObjItr, TexObjItr> FindTexObjs(const unsigned key) {
      return m_texObjs.equal_range(key);
    }

    void DelAllDispLists() {
      DispListItr it;
      for ( it = m_dispLists.begin(); it != m_dispLists.end(); it++ )
	if ( it->second ) it->second->DelList();
    }
    void DelAllTexObjs() {
      TexObjItr it;
      for ( it = m_texObjs.begin(); it != m_texObjs.end(); it++ )
	if ( it->second ) it->second->DelObj();
    }

    bool GenAllDispLists() {
      DispListItr it;
      for ( it = m_dispLists.begin(); it != m_dispLists.end(); it++ ) {
	if ( ! it->second ) continue;
	if ( ! it->second->GenList() ) return false;
      }
      return true;
    }

    bool GenAllTexObjs() {
      TexObjItr it;
      for ( it = m_texObjs.begin(); it != m_texObjs.end(); it++ ) {
	if ( ! it->second ) continue;
	if ( ! it->second->GenObj() ) return false;
      }
      return true;
    }
  };

};

#endif // _CES_UTIL_OGL_OBJ_H_

