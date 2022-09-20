//
// vfrIndexPolygons
//   Indexed Polygons Class
//

#include "vfrIndexPolygons.h"

/* static members */

#if VFR_IDXPLGN_TESS
GLUtesselator* vfrIndexPolygons::_tessObj = NULL;
#endif


vfrIndexPolygons::vfrIndexPolygons(const std::string& nm, const Bool ssm)
 : vfrNode(nm, ssm) {
  setTessMode(FALSE);
}

vfrIndexPolygons::~vfrIndexPolygons() {
  clearTessObj();
}

void
vfrIndexPolygons::renderFeedBack(const unsigned int oid) {
  if ( _currentSeq != oid )
    return;

  applyMatrix();

  glDisable(GL_LIGHTING);

  switch ( _feedbackMode ) {
  case FB_VERTEX:
    {
    for ( register int i = 0; i < nVerts; i++ ) {
      glPassThrough((GLfloat)i);
      glBegin(GL_POINTS);
      glVertex3fv(_verts[i]);
      glEnd();
    }
    break;
    }
  case FB_FACE:
    {
    GLenum faceMode = getFaceMode();
    glPolygonMode(faceMode, GL_FILL);
#if VFR_IDXPLGN_TESS
    GLUtesselator* tessObj;
    if ( _useTess )
      tessObj = getTessObj(FALSE);
#endif
    register int index = 0;
    register int findex = 0;
    while ( index < nIndices ) {
      glPassThrough((GLfloat)findex);

#if VFR_IDXPLGN_TESS
      if ( _useTess ) {
        GLdouble tessV[VFR_IDXP_MAX_VERTS_PER_POLYGON][3];
        gluTessBeginPolygon(tessObj, (void*)NULL);
        gluTessBeginContour(tessObj);
        int pidx = 0;
        while ( _indices[index] != VFR_END_OF_ELEM && index < nIndices ) {
          if ( _indices[index] < nVerts &&
	       pidx < VFR_IDXP_MAX_VERTS_PER_POLYGON) {
            tessV[pidx][0] = (GLdouble)_verts[_indices[index]][0];
            tessV[pidx][1] = (GLdouble)_verts[_indices[index]][1];
            tessV[pidx][2] = (GLdouble)_verts[_indices[index]][2];
            gluTessVertex(tessObj, tessV[pidx], tessV[pidx]);
          }
          index++;
          pidx++;
        }
        gluTessEndContour(tessObj);
        gluTessEndPolygon(tessObj);
      } else {
#endif /* VFR_IDXPLGN_TESS */
        glBegin(GL_POLYGON);
        while ( _indices[index] != VFR_END_OF_ELEM && index < nIndices ) {
          if ( _indices[index] < nVerts )
            glVertex3fv(_verts[_indices[index]]);
          index++;
        }
        glEnd();
#if VFR_IDXPLGN_TESS
      }
#endif
      index++;
      findex++;
    }
    break;
    }
  case FB_EDGE:
  default:
    break;    // do nothing.
  }

  glEnable(GL_LIGHTING);
  unApplyMatrix();
}

Bool
vfrIndexPolygons::generateNormals() {
  register int n;

  if ( nVerts < 3 )
    return FALSE;
  if ( nIndices < 4 )
    return FALSE;

  // Count Polygons
  register int index = 0;
  register int findex = 0;
  while ( index < nIndices ) {
    if ( _indices[index++] == VFR_END_OF_ELEM )
      findex++;
  }
  if ( _indices[nIndices -1] != VFR_END_OF_ELEM )
    findex++;

  if ( findex < 1 )
    return FALSE;

  // Allocate Normals' area
  if ( ! alcNormals(findex) )
    return FALSE;
    
  CES::Vec3<float> v1, v2, nv;
  nNormals = findex;
  index = 0;
  findex = 0;
  while ( index < nIndices ) {
    for ( n = index;
	  index < nIndices && _indices[index] != VFR_END_OF_ELEM; index++ );
    if ( index - n < 3 ) {
      _normals[findex][0] = 0.0f;
      _normals[findex][1] = 0.0f;
      _normals[findex][2] = 1.0f;
    } else {
      v1[0] = _verts[_indices[n +1]][0] - _verts[_indices[n]][0];
      v1[1] = _verts[_indices[n +1]][1] - _verts[_indices[n]][1];
      v1[2] = _verts[_indices[n +1]][2] - _verts[_indices[n]][2];

      v2[0] = _verts[_indices[n +2]][0] - _verts[_indices[n]][0];
      v2[1] = _verts[_indices[n +2]][1] - _verts[_indices[n]][1];
      v2[2] = _verts[_indices[n +2]][2] - _verts[_indices[n]][2];

      nv = v1 ^ v2;
      _normals[findex][0] = nv[0];
      _normals[findex][1] = nv[1];
      _normals[findex][2] = nv[2];
    }
    index++;
    findex++;
  }

  setNormalMode(AT_PER_FACE);
  return TRUE;
}


void
vfrIndexPolygons::renderSolid() {
  // display-list check
  if ( beginDispList(DLF_SOLID) ) return;

  vfrTexture::MapType mapType = vfrTexture::NOMAP;
  int num_uvc = 0;
  vector2* uvcs = NULL;

  if ( _texture ) {
    if ( _texture->getMapType() == vfrTexture::UVC ) {
      mapType = vfrTexture::UVC;
      num_uvc = _texture->getNumUVCs();
      uvcs = _texture->getUVCs();
      if ( ! uvcs || num_uvc < 1 ) mapType = vfrTexture::NOMAP;
    }
  }

#if VFR_IDXPLGN_TESS
  GLUtesselator* tessObj;
  if ( _useTess ) {
    if ( mapType == vfrTexture::NOMAP )
      tessObj = getTessObj(FALSE);
    else
      tessObj = getTessObj(TRUE);
  }
#endif

  GLdouble tessV[VFR_IDXP_MAX_VERTS_PER_POLYGON][9];
  register int i;
  for ( i = 0; i < VFR_IDXP_MAX_VERTS_PER_POLYGON; i++ ) {
    tessV[i][3] = _colors[0][0]; tessV[i][4] = _colors[0][1];
    tessV[i][5] = _colors[0][2]; tessV[i][6] = _colors[0][3];
  }

  register int index = 0;
  register int findex = 0;
  while ( index < nIndices ) {
    if ( _colorMode == AT_PER_FACE && findex < nColors ) {
      glColor4fv(_colors[findex]);
      for ( i = 0; i < VFR_IDXP_MAX_VERTS_PER_POLYGON; i++ ) {
        tessV[i][3] = _colors[findex][0]; tessV[i][4] = _colors[findex][1];
        tessV[i][5] = _colors[findex][2]; tessV[i][6] = _colors[findex][3];
      }
    }

    if ( _normalMode == AT_PER_FACE )
      if ( findex < nNormals )
        glNormal3fv(_normals[findex]);

#if VFR_IDXPLGN_TESS
    if ( _useTess ) {
      gluTessBeginPolygon(tessObj, (void*)NULL);
      gluTessBeginContour(tessObj);

      register int pidx = 0;
      while ( _indices[index] != VFR_END_OF_ELEM && index < nIndices ) {
        if ( _colorMode == AT_PER_VERTEX && _indices[index] < nColors ) {
          glColor4fv(_colors[_indices[index]]);
          tessV[pidx][3] = _colors[findex][0];
	  tessV[pidx][4] = _colors[findex][1];
          tessV[pidx][5] = _colors[findex][2];
	  tessV[pidx][6] = _colors[findex][3];
        }
        if ( _normalMode == AT_PER_VERTEX && _indices[index] < nNormals )
          glNormal3fv(_normals[_indices[index]]);
        if ( mapType == vfrTexture::UVC && _indices[index] < num_uvc ) {
          tessV[pidx][7] = uvcs[_indices[index]][0];
          tessV[pidx][8] = uvcs[_indices[index]][1];
        }
        if ( _indices[index] < nVerts ) {
          tessV[pidx][0] = (GLdouble)_verts[_indices[index]][0];
          tessV[pidx][1] = (GLdouble)_verts[_indices[index]][1];
          tessV[pidx][2] = (GLdouble)_verts[_indices[index]][2];
          gluTessVertex(tessObj, tessV[pidx], tessV[pidx]);
        }
        index++;
        pidx++;
      }
      gluTessEndContour(tessObj);
      gluTessEndPolygon(tessObj);
    }
    else {
#endif /* VFR_IDXPLGN_TESS */
      glBegin(GL_POLYGON);
      register int pidx = 0;
      while ( _indices[index] != VFR_END_OF_ELEM && index < nIndices ) {
        if ( _colorMode == AT_PER_VERTEX && _indices[index] < nColors )
          glColor4fv(_colors[_indices[index]]);
        if ( _normalMode == AT_PER_VERTEX && _indices[index] < nNormals )
          glNormal3fv(_normals[_indices[index]]);
        if ( mapType == vfrTexture::UVC && _indices[index] < num_uvc ) {
          glTexCoord2fv(uvcs[_indices[index]]);
        }
        if ( _indices[index] < nVerts )
          glVertex3fv(_verts[_indices[index]]);
        index++;
        pidx++;
      }
      glEnd();
#if VFR_IDXPLGN_TESS
    }
#endif

    index++;
    findex++;
  } // end of while(index)

  // end display-list definition
  endDispList(DLF_SOLID);
}

void
vfrIndexPolygons::renderWire() {
  // display-list check
  if ( beginDispList(DLF_WIRE) ) return;

  Bool auxMode = useAuxLineColor();
  register int index = 0;
  register int findex = 0;
  while ( index < nIndices ) {
    if ( ! auxMode && _colorMode == AT_PER_FACE && findex < nColors )
      glColor4fv(_colors[findex]);

    glBegin(GL_LINE_LOOP);
    while ( _indices[index] != VFR_END_OF_ELEM && index < nIndices ) {
      if ( ! auxMode && _colorMode == AT_PER_VERTEX &&
	   _indices[index] < nColors )
        glColor4fv(_colors[_indices[index]]);

      if ( _indices[index] < nVerts )
        glVertex3fv(_verts[_indices[index]]);

      index++;
    }
    glEnd();
    index++;
    findex++;
  } // end of while(index)

  // end display-list definition
  endDispList(DLF_WIRE);
}

#if VFR_IDXPLGN_TESS

#ifndef CALLBACK
#define CALLBACK
#endif

#if defined (__GNUC__) && defined (__GNUC_MINOR__)
# if defined (__APPLE_CC__)
#  if __APPLE_CC__ >= 5400
typedef void CALLBACK (*glucallback)(void);
#  else
typedef void CALLBACK (*glucallback)(...);
#  endif
# elif __GNUC__ >= 4
#  if __GNUC_MINOR__ >= 1
typedef void CALLBACK (*glucallback)(void);
#  else
typedef void CALLBACK (*glucallback)(...);
#  endif
# else
typedef void CALLBACK (*glucallback)(void);
# endif
#endif


GLUtesselator*
vfrIndexPolygons::getTessObj(const Bool isFull) {
  if ( ! _tessObj ) {
    _tessObj = gluNewTess();
    if ( _tessObj ) {
#ifndef WINDOWS
      gluTessCallback(_tessObj, GLU_TESS_BEGIN,
		      glucallback(beginCallback));
      gluTessCallback(_tessObj, GLU_TESS_ERROR,
		      glucallback(errorCallback));
      gluTessCallback(_tessObj, GLU_TESS_END,
		      glucallback(endCallback));
      if ( isFull ) {
        gluTessCallback(_tessObj, GLU_TESS_VERTEX,
			glucallback(vertexCallback9));
        gluTessCallback(_tessObj, GLU_TESS_COMBINE,
			glucallback(combineCallback9));
      } else {
        gluTessCallback(_tessObj, GLU_TESS_VERTEX,
			glucallback(vertexCallback3));
        // No combine callback.
      }
#else
      gluTessCallback(_tessObj, GLU_TESS_BEGIN,
		      (GLvoid(CALLBACK*)())beginCallback);
      gluTessCallback(_tessObj, GLU_TESS_ERROR,
		      (GLvoid(CALLBACK*)())errorCallback);
      gluTessCallback(_tessObj, GLU_TESS_END,
		      (GLvoid(CALLBACK*)())endCallback);
      if ( isFull ) {
        gluTessCallback(_tessObj, GLU_TESS_VERTEX,
			(GLvoid(CALLBACK*)())vertexCallback9);
        gluTessCallback(_tessObj, GLU_TESS_COMBINE,
			(GLvoid(CALLBACK*)())combineCallback9);
      } else {
        gluTessCallback(_tessObj, GLU_TESS_VERTEX,
			(GLvoid(CALLBACK*)())vertexCallback3);
        // No combine callback.
      }
#endif
    }
  }
  gluTessProperty(_tessObj, GLU_TESS_WINDING_RULE,
                  GLU_TESS_WINDING_POSITIVE);

  return _tessObj;
}

GLvoid
vfrIndexPolygons::clearTessObj() {
  if ( _tessObj ) {
    gluDeleteTess(_tessObj);
    _tessObj = NULL;
  }
}

GLvoid
vfrIndexPolygons::beginCallback(GLenum which) {
  glBegin(which);
}

GLvoid
vfrIndexPolygons::errorCallback(GLenum errorCode) {
  const GLubyte* estring = gluErrorString(errorCode);
#ifndef WINDOWS
  fprintf(stderr, " vfrIndexPolygons:Tessellation Error: %s\n", estring);
#endif
}

GLvoid
vfrIndexPolygons::endCallback() {
  glEnd();
}

GLvoid
vfrIndexPolygons::vertexCallback3(GLvoid* vertex) {
  glVertex3dv((const GLdouble*)vertex);
}

GLvoid
vfrIndexPolygons::vertexCallback9(GLvoid* vertex) {
  // data order is: vertex[3], color[4], texture[2]
  const GLdouble* ptr = (GLdouble*)vertex;
  glColor4dv(ptr + 3);
  glTexCoord2dv(ptr + 7);
  glVertex3dv(ptr);
}

GLvoid
vfrIndexPolygons::combineCallback9(GLdouble coords[3],
				   void* _vertex_data[4],
				   GLfloat weight[4], void **_dataOut)
{
  GLdouble** vertex_data = (GLdouble**)_vertex_data;
  GLdouble** dataOut = (GLdouble**)_dataOut;

  GLdouble* vertex = (GLdouble*)malloc(9 * sizeof(GLdouble));
  vertex[0] = coords[0];
  vertex[1] = coords[1];
  vertex[2] = coords[2];
  for ( int i = 3; i < 9; i++ )
    vertex[i] = weight[0] * vertex_data[0][i] 
              + weight[1] * vertex_data[1][i]
              + weight[2] * vertex_data[2][i] 
              + weight[3] * vertex_data[3][i];
  *dataOut = vertex;
}

#else /* VFR_IDXPLGN_TESS */

// STATIC
void* vfrIndexPolygons::getTessObj(const Bool) {
  return NULL;
}

// STATIC
void vfrIndexPolygons::clearTessObj() {
}

#endif /* VFR_IDXPLGN_TESS */

// EOF
