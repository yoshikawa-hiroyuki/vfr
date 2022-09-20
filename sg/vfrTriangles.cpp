//
// vfrTriangles
//   Triangles Class
//

#include "vfrTriangles.h"


vfrTriangles::vfrTriangles(const std::string& nm, const Bool ssm)
  : vfrNode(nm, ssm) {
  alcNormals(1);
}

vfrTriangles::~vfrTriangles() {
}


void
vfrTriangles::renderFeedBack(const unsigned int oid) {
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
    register int index = 0;
    for ( register int findex = 0; findex < nVerts / 3; findex++ ) {
      glPassThrough((GLfloat)findex);
      glBegin(GL_TRIANGLES);
      for ( register int i = 0; i < 3; i++ )
        glVertex3fv(_verts[index++]);
      glEnd();
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
vfrTriangles::generateNormals() {
  if ( nVerts < 3 )
    return FALSE;
  int numFace = nVerts / 3;

  // Allocate Normals' area
  if ( ! alcNormals(numFace) )
    return FALSE;
    
  CES::Vec3<float> v1, v2, nv;
  for ( register int n = 0; n < numFace; n++ ) {
    v1[0] = _verts[n*3+1][0] - _verts[n*3  ][0];
    v1[1] = _verts[n*3+1][1] - _verts[n*3  ][1];
    v1[2] = _verts[n*3+1][2] - _verts[n*3  ][2];
    v2[0] = _verts[n*3+2][0] - _verts[n*3+1][0];
    v2[1] = _verts[n*3+2][1] - _verts[n*3+1][1];
    v2[2] = _verts[n*3+2][2] - _verts[n*3+1][2];
    nv = v1 ^ v2;
    _normals[n][0] = nv[0];
    _normals[n][1] = nv[1];
    _normals[n][2] = nv[2];
  }
  nNormals = numFace;

  if ( _normalMode == AT_PER_VERTEX ) {
    vfrTriaSmoother smoother(0.25f);
    return smoother.SmoothNorm(*this);
  } else {
    setNormalMode(AT_PER_FACE); // fail-safe
    return TRUE;
  }
}


void
vfrTriangles::renderSolid() {
  if ( nVerts < 3 ) return;

  // display-list check
  if ( beginDispList(DLF_SOLID) ) return;

  if ( _normalMode == AT_WHOLE )
    glNormal3fv(_normals[0]);

  int numFace = nVerts / 3;
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

  register int index = 0;
  glBegin(GL_TRIANGLES);
  for ( register int findex = 0; findex < numFace; findex++ ) {
    if ( _colorMode == AT_PER_FACE && findex < nColors )
      glColor4fv(_colors[findex]);
    if ( _normalMode == AT_PER_FACE && findex < nNormals )
      glNormal3fv(_normals[findex]);

    for ( register int i = 0; i < 3; i++ ) {
      if ( _colorMode == AT_PER_VERTEX && index < nColors )
	glColor4fv(_colors[index]);
      if ( _normalMode == AT_PER_VERTEX && index < nNormals )
	glNormal3fv(_normals[index]);
      if ( mapType == vfrTexture::UVC && index < num_uvc )
	glTexCoord2fv(uvcs[index]);
      glVertex3fv(_verts[index++]);
    } // end of for(i)
  } // end of for(findex)
  glEnd();

  // end display-list definition
  endDispList(DLF_SOLID);
}

void
vfrTriangles::renderWire() {
#if 0
  if ( nVerts < 3 ) return;

  // display-list check
  if ( beginDispList(DLF_WIRE) ) return;

  Bool auxMode = useAuxLineColor();
  int numFace = nVerts / 3;
  register int index = 0;
  for ( register int findex = 0; findex < numFace; findex++ ) {
    if ( ! auxMode && _colorMode == AT_PER_FACE && findex < nColors )
      glColor4fv(_colors[findex]);
    glBegin(GL_LINE_LOOP);
    for ( register int i = 0; i < 3; i++ ) {
      if ( ! auxMode && _colorMode == AT_PER_VERTEX && index < nColors )
        glColor4fv(_colors[index]);
      glVertex3fv(_verts[index++]);
    } // end of for(i)
    glEnd();
  } // end of for(findex)

  // end display-list definition
  endDispList(DLF_WIRE);
#else
  renderSolid();
#endif
}


//--------------- Normal smoother class ---------------//

vfrTriaSmoother::vfrTriaSmoother(const float tolerance)
 : cosTolerance(tolerance)
{
  if ( cosTolerance < 0.f ) cosTolerance = 0.f;
  if ( cosTolerance > 0.99f ) cosTolerance = 0.99f;
}

vfrTriaSmoother::~vfrTriaSmoother()
{
}


Bool
vfrTriaSmoother::SmoothNorm(vfrTriangles& obj)
{
  register size_t i;
  for ( i = 0; i < TS_HASH_TABLE_SIZE; i++ )
    hTbl[i].clear();

  size_t numV = obj.getNumVerts();
  size_t numN = obj.getNumNormals();
  if ( numV < 3 || numN < 1 ) return FALSE;
  if ( numN < numV / 3 ) return FALSE; // Normals not generated

  vector3* newNV = (vector3*)Allocate(sizeof(vector3)*numV);
  if ( ! newNV ) return FALSE;
  vector3* pv = obj.getVerts();
  vector3* pn = obj.getNormals();

  for ( i = 0; i < numN; i++ ) {
    float len = (float)sqrt(pn[i][0]*pn[i][0] +
			    pn[i][1]*pn[i][1] + pn[i][2]*pn[i][2]);
    if ( len <= 1e-8f ) {
      pn[i][0] = 0.f; pn[i][1] = 0.f; pn[i][2] = 1.f;
    } else {
      pn[i][0] /= len; pn[i][1] /= len; pn[i][2] /= len;
    }
    memcpy(newNV[i*3   ], pn[i], sizeof(vector3));
    memcpy(newNV[i*3 +1], pn[i], sizeof(vector3));
    memcpy(newNV[i*3 +2], pn[i], sizeof(vector3));
  }

  // create table
  for ( i = 0; i < numV/3; i++ ) {
    hashInsert(pv[i*3   ], i, 0);
    hashInsert(pv[i*3 +1], i, 1);
    hashInsert(pv[i*3 +2], i, 2);
  }

  // smooth
  for ( i = 0; i < TS_HASH_TABLE_SIZE; i++ ) {
    while ( ! hTbl[i].empty() ) {
      std::deque<HASH_ENTRY> vset;
      std::deque<HASH_ENTRY>::iterator it = hTbl[i].begin();
      CES::Vec3<float> nn, vv;
      float cs;

      vv[0] = pv[it->tri*3 + it->off][0];
      vv[1] = pv[it->tri*3 + it->off][1];
      vv[2] = pv[it->tri*3 + it->off][2];
      nn[0] = pn[it->tri][0];
      nn[1] = pn[it->tri][1];
      nn[2] = pn[it->tri][2];

      it++;
      while ( it != hTbl[i].end() ) {
	if ( fabs(pv[it->tri*3 + it->off][0] - vv[0]) < 1e-6 &&
	     fabs(pv[it->tri*3 + it->off][1] - vv[1]) < 1e-6 &&
	     fabs(pv[it->tri*3 + it->off][2] - vv[2]) < 1e-6 ) {
          cs = CES::Vec3<float>(pn[it->tri]) | nn;
          if ( fabs(cs) > cosTolerance ) {
	    vset.push_back(*it);
	    it = hTbl[i].erase(it);
	    continue;
	  }
	}
	it++;
      } // end of while(it)

      if ( vset.size() == 0 ) {
	hTbl[i].erase(hTbl[i].begin());
	continue;
      }

      /* average the normal */
      //nn[0] = nn[1] = nn[2] = 0.f;
      for ( it = vset.begin(); it != vset.end(); it++ ) {
	nn[0] += pn[it->tri][0];
	nn[1] += pn[it->tri][1];
	nn[2] += pn[it->tri][2];
      } // end of for(it)
      nn.UnitVec();

      /* put it back */
      it = hTbl[i].begin();
      newNV[it->tri*3 + it->off][0] = nn[0];
      newNV[it->tri*3 + it->off][1] = nn[1];
      newNV[it->tri*3 + it->off][2] = nn[2];
      for ( it = vset.begin(); it != vset.end(); it++ ) {
	newNV[it->tri*3 + it->off][0] = nn[0];
	newNV[it->tri*3 + it->off][1] = nn[1];
	newNV[it->tri*3 + it->off][2] = nn[2];
      } // end of for(it)

      hTbl[i].erase(hTbl[i].begin());
    } // end while
  } // end of for(i)

  if ( ! obj.setNormals(numV, newNV) ) {
    DeAllocate(newNV);
    return FALSE;
  }
  obj.setNormalMode(AT_PER_VERTEX);

  DeAllocate(newNV);
  return TRUE;
}

// EOF
