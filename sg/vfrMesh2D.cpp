//
// vfrMesh2D
//   2D Mesh Object Class
//
#include "vfrMesh2D.h"


vfrMesh2D::vfrMesh2D(const std::string& nm, const Bool ssm)
  : vfrNode(nm, ssm) {
  setMeshSize(0, 0);
  alcNormals(1);
}

vfrMesh2D::vfrMesh2D(const int m, const int n,
		     const std::string& nm, const Bool ssm)
  : vfrNode(nm, ssm) {
  setMeshSize(m, n);
  alcNormals(1);
}

vfrMesh2D::vfrMesh2D(const Point2 ms,
		     const std::string& nm, const Bool ssm)
  : vfrNode(nm, ssm) {
  setMeshSize(ms);
  alcNormals(1);
}

vfrMesh2D::~vfrMesh2D() {
}


void
vfrMesh2D::renderFeedBack(const unsigned int oid) {
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
    register int findex = 0;
    for ( register int j = 0; j < _meshSize.y -1; j++ ) {
      for ( register int i = 0; i < _meshSize.x -1; i++ ) {
        glPassThrough((GLfloat)findex);
        glBegin(GL_QUADS);
        glVertex3fv(_verts[j*_meshSize.x + i]);
        glVertex3fv(_verts[j*_meshSize.x + i+1]);
        glVertex3fv(_verts[(j+1)*_meshSize.x + i+1]);
        glVertex3fv(_verts[(j+1)*_meshSize.x + i]);
        glEnd();
        findex++;
      }
    }
    break;
    }
  case FB_EDGE:
    {
    // Holizontal Lines : 0 ... (M-1)*N -1
    // Vartical Lines   : (M-1)*N + 0 ... (M-1)*N + M*(N-1) -1
    register int i, j, findex = 0;
    // Holizontal Lines
    for ( j = 0; j < _meshSize.y; j++ ) {
      for ( i = 0; i < _meshSize.x -1; i++ ) {
        glPassThrough((GLfloat)findex);
        glBegin(GL_LINES);
        glVertex3fv(_verts[j*_meshSize.x + i]);
        glVertex3fv(_verts[j*_meshSize.x + i+1]);
        glEnd();
        findex++;
      }
    }
    // Vartical Lines
    for ( i = 0; i < _meshSize.x; i++ ) {
      for ( j = 0; j < _meshSize.y -1; j++ ) {
        glPassThrough((GLfloat)findex);
        glBegin(GL_LINES);
        glVertex3fv(_verts[j*_meshSize.x + i]);
        glVertex3fv(_verts[(j+1)*_meshSize.x + i]);
        glEnd();
        findex++;
      }
    }
    break;
    }
  default:
    break;    // do nothing.
  }
  glEnable(GL_LIGHTING);
  unApplyMatrix();
}

Bool
vfrMesh2D::generateNormals() {
  if ( nVerts != _meshSize.x * _meshSize.y )
    return FALSE;
  register int i, j, idx1, idx2, idx0;

  // Normals per FACE
  vector3* faceN
    = (vector3*)Allocate(sizeof(vector3)*(_meshSize.x-1)*(_meshSize.y-1));
  if ( ! faceN )
    return FALSE;

  CES::Vec3<float> v1, v2, nv;
  for ( j = 0; j < _meshSize.y-1; j++ ) {
    for ( i = 0; i < _meshSize.x-1; i++ ) {
      idx1 = (j+1)*_meshSize.x +i;
      idx2 = j*_meshSize.x +i;
      idx0 = j*(_meshSize.x -1) +i;
      v1[0] = _verts[idx1 +1][0] - _verts[idx2   ][0];
      v1[1] = _verts[idx1 +1][1] - _verts[idx2   ][1];
      v1[2] = _verts[idx1 +1][2] - _verts[idx2   ][2];
      v2[0] = _verts[idx1   ][0] - _verts[idx2 +1][0];
      v2[1] = _verts[idx1   ][1] - _verts[idx2 +1][1];
      v2[2] = _verts[idx1   ][2] - _verts[idx2 +1][2];
      nv = v1 ^ v2; nv.UnitVec();
      faceN[idx0][0] = nv[0];
      faceN[idx0][1] = nv[1];
      faceN[idx0][2] = nv[2];
    }
  }
  if ( _normalMode == AT_PER_FACE ) {
    if ( _normals )
      DeAllocate(_normals);
    _normals = faceN;
    nNormals = (_meshSize.x-1)*(_meshSize.y-1);
    return TRUE;
  }

  // Normals per VERTEX
  if ( ! alcNormals(nVerts) ) {
    _normals = faceN;
    nNormals = (_meshSize.x-1)*(_meshSize.y-1);
    setNormalMode(AT_PER_FACE);
    return TRUE;
  }
  for ( j = 0; j < _meshSize.y; j++ ) {
    for ( i = 0; i < _meshSize.x; i++ ) {
      vector3 Nv = {0.0f, 0.0f, 0.0f};
      int numF = 0;

      idx1 = (j-1)*(_meshSize.x-1) + i;
      idx2 = j*(_meshSize.x-1) + i;

      // Fa
      if ( i-1 >= 0 && j-1 >= 0 ) {
        Nv[0] += faceN[idx1-1][0];
        Nv[1] += faceN[idx1-1][1];
        Nv[2] += faceN[idx1-1][2];
        numF++;
      }
      // Fb
      if ( i < _meshSize.x-1 && j-1 >= 0 ) {
        Nv[0] += faceN[idx1][0];
        Nv[1] += faceN[idx1][1];
        Nv[2] += faceN[idx1][2];
        numF++;
      }
      // Fc
      if ( i < _meshSize.x-1 && j < _meshSize.y-1 ) {
        Nv[0] += faceN[idx2][0];
        Nv[1] += faceN[idx2][1];
        Nv[2] += faceN[idx2][2];
        numF++;
      }
      // Fd
      if ( i-1 >= 0 && j < _meshSize.y-1 ) {
        Nv[0] += faceN[idx2-1][0];
        Nv[1] += faceN[idx2-1][1];
        Nv[2] += faceN[idx2-1][2];
        numF++;
      }
      if ( numF <= 0 ) {
        Nv[2] = 1.0f;
        setNormal(j*_meshSize.x +i, Nv);
        continue;
      }
      Nv[0] /= (float)numF;
      Nv[1] /= (float)numF;
      Nv[2] /= (float)numF;
      setNormal(j*_meshSize.x +i, Nv);
    }
  }
  nNormals = nVerts;
  setNormalMode(AT_PER_VERTEX);
  DeAllocate(faceN);
  return TRUE;
}

GLsizei
vfrMesh2D::getFeedbackSize() const {
  switch ( _feedbackMode ) {
  case FB_FACE:
    return (GLsizei)(nVerts * VFR_FEEDBACK_SIZE_FACTOR * 4);
  case FB_EDGE:
    return (GLsizei)(nVerts * VFR_FEEDBACK_SIZE_FACTOR * 2);
  case FB_VERTEX:
  default:
    return (GLsizei)(nVerts * VFR_FEEDBACK_SIZE_FACTOR);
  }
}

Bool
vfrMesh2D::setMeshSize(const int m, const int n) {
  Point2 ns;
  ns.x = m;
  ns.y = n;
  return setMeshSize(ns);
}
Bool
vfrMesh2D::setMeshSize(const Point2 ns) {
  if ( _meshSize == ns ) return TRUE;
  int total = ns.x * ns.y;
  if ( ! alcVerts(total) )
    return FALSE;
  _meshSize = ns;
  notice();
  return TRUE;
}

void
vfrMesh2D::renderSolid() {
  if ( _meshSize.x < 2 || _meshSize.y < 2 )
    return;

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

  if ( _normalMode == AT_WHOLE )
    glNormal3fv(_normals[0]);

  register int index, findex = 0;
  glBegin(GL_QUADS);
  for ( register int j = 0; j < _meshSize.y -1; j++ ) {
    for ( register int i = 0; i < _meshSize.x -1; i++ ) {
      if ( _colorMode == AT_PER_FACE && findex < nColors )
        glColor4fv(_colors[findex]);
      if ( _normalMode == AT_PER_FACE && findex < nNormals )
        glNormal3fv(_normals[findex]);

      // V1
      index = j*_meshSize.x + i;
      if ( _colorMode == AT_PER_VERTEX && index < nColors )
	glColor4fv(_colors[index]);
      if ( _normalMode == AT_PER_VERTEX && index < nNormals )
	glNormal3fv(_normals[index]);
      if ( mapType == vfrTexture::UVC && index < num_uvc )
	glTexCoord2fv(uvcs[index]);
      glVertex3fv(_verts[index]);

      // V2
      index = j*_meshSize.x + i +1;
      if ( _colorMode == AT_PER_VERTEX && index < nColors )
	glColor4fv(_colors[index]);
      if ( _normalMode == AT_PER_VERTEX && index < nNormals )
	glNormal3fv(_normals[index]);
      if ( mapType == vfrTexture::UVC && index < num_uvc )
	glTexCoord2fv(uvcs[index]);
      glVertex3fv(_verts[index]);

      // V3
      index = (j +1)*_meshSize.x + i +1;
      if ( _colorMode == AT_PER_VERTEX && index < nColors )
	glColor4fv(_colors[index]);
      if ( _normalMode == AT_PER_VERTEX && index < nNormals )
	glNormal3fv(_normals[index]);
      if ( mapType == vfrTexture::UVC && index < num_uvc )
	glTexCoord2fv(uvcs[index]);
      glVertex3fv(_verts[index]);

      // V4
      index = (j +1)*_meshSize.x + i;
      if ( _colorMode == AT_PER_VERTEX && index < nColors )
	glColor4fv(_colors[index]);
      if ( _normalMode == AT_PER_VERTEX && index < nNormals )
	glNormal3fv(_normals[index]);
      if ( mapType == vfrTexture::UVC && index < num_uvc )
	glTexCoord2fv(uvcs[index]);
      glVertex3fv(_verts[index]);

      findex++;
    } // end of for(i)
  } // end of for(j)
  glEnd();

  // end display-list definition
  endDispList(DLF_SOLID);
}

void
vfrMesh2D::renderWire() {
  if ( _meshSize.x < 2 || _meshSize.y < 2 )
    return;

  // display-list check
  if ( beginDispList(DLF_WIRE) ) return;

  register int i, j, index, findex;
  Bool auxMode = useAuxLineColor();

  // Holizontal Lines
  for ( j = 0; j < _meshSize.y; j++ ) {
    glBegin(GL_LINE_STRIP);
    for ( i = 0; i < _meshSize.x; i++ ) {
      index = j*_meshSize.x + i;
      if ( j == (_meshSize.y -1) )
        findex = (j -1) * (_meshSize.x -1) + (i==(_meshSize.x -1) ? i -1 : i);
      else
        findex = j * (_meshSize.x -1) + (i==(_meshSize.x -1) ? i -1 : i);
      if ( ! auxMode ) {
        if ( _colorMode == AT_PER_VERTEX && index < nColors )
          glColor4fv(_colors[index]);
        else if ( _colorMode == AT_PER_FACE && findex < nColors )
          glColor4fv(_colors[findex]);
      }
      glVertex3fv(_verts[index]);
    } // end of for(i)
    glEnd();
  } // end of for(j)

  // Vartical Lines
  for ( i = 0; i < _meshSize.x; i++ ) {
    glBegin(GL_LINE_STRIP);
    for ( j = 0; j < _meshSize.y; j++ ) {
      index = j*_meshSize.x + i;
      if ( j == (_meshSize.y -1) )
        findex = (j -1) * (_meshSize.x -1) + (i==(_meshSize.x -1) ? i -1 : i);
      else
        findex = j * (_meshSize.x -1) + (i==(_meshSize.x -1) ? i -1 : i);
      if ( ! auxMode ) {
        if ( _colorMode == AT_PER_VERTEX && index < nColors )
          glColor4fv(_colors[index]);
        else if ( _colorMode == AT_PER_FACE && findex < nColors )
          glColor4fv(_colors[findex]);
      }
      glVertex3fv(_verts[index]);
    } // end of for(j)
    glEnd();
  } // end of for(i)

  // end display-list definition
  endDispList(DLF_WIRE);
}

// EOF
