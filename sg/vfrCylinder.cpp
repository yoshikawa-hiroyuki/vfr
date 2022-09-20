//
// vfrCylinder
//   Circular Cylinder Object Class
//

#include "vfrCylinder.h"

/*  static members */

const vector3 vfrCylinder::_surface[10] = {
  { 0.0f, 1.0f, 0.0f}, { 1.0f, 1.0f, 0.0f}, { 0.0f, 1.0f, 1.0f},
  {-1.0f, 1.0f, 0.0f}, { 0.0f, 1.0f,-1.0f},
  { 1.0f,-1.0f, 0.0f}, { 0.0f,-1.0f, 1.0f}, {-1.0f,-1.0f, 0.0f},
  { 0.0f,-1.0f,-1.0f}, { 0.0f,-1.0f, 0.0f}
};
const vector3 vfrCylinder::_snormal[2] = {
  {0.0f, 1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}
};
const int vfrCylinder::_tindex[8][3] = {
  {0, 2, 1}, {0, 3, 2}, {0, 4, 3}, {0, 1, 4},
  {9, 5, 6}, {9, 6, 7}, {9, 7, 8}, {9, 8, 5}
};
const int vfrCylinder::_qindex[4][4] = {
  {1, 2, 6, 5}, {2, 3, 7, 6}, {3, 4, 8, 7}, {4, 1, 5, 8}
};


vfrCylinder::vfrCylinder(const std::string& nm, const Bool ssm)
  : vfrNode(nm, ssm), _showBottom(TRUE),_showTop(TRUE) {
  makeCylinder(0.5f, 1.0f);
  setSubdiv(2);
}

vfrCylinder::vfrCylinder(const float r, const float h,
			 const std::string& nm, const Bool ssm)
  : vfrNode(nm, ssm), _showBottom(TRUE),_showTop(TRUE) {
  makeCylinder(r, h);
  setSubdiv(2);
}

void
vfrCylinder::makeCylinder(const float r, const float h) {
  setRadius(r);
  setHeight(h);
  vector3 v0[1];
  v0[0][0] = v0[0][1] = v0[0][2] = 0.0f;
  setVerts(1, v0);

  generateBbox();
}

vfrCylinder::~vfrCylinder() {
}

void
vfrCylinder::setRadius(const float r) {
  if ( _radius == r ) return;
  _radius = r > EPSF ? r : EPSF;
  generateBbox();
  notice();
}

void
vfrCylinder::setHeight(const float h) {
  if ( _height == h ) return;
  _height = h > EPSF ? h : EPSF;
  generateBbox();
  notice();
}

void
vfrCylinder::setSubdiv(const int si) {
  if ( _subdiv == si ) return;
  if ( si < 0 ) return;
  _subdiv = si;
  notice();
}

void
vfrCylinder::setShowBottom(const Bool sb) {
  if ( _showBottom == sb ) return;
  _showBottom = sb;
  notice();
}

void
vfrCylinder::setShowTop(const Bool st) {
  if ( _showTop == st ) return;
  _showTop = st;
  notice();
}

void
vfrCylinder::generateBbox() {
  _bbox[0][0] = -_radius;
  _bbox[0][1] = -_height*0.5f;
  _bbox[0][2] = -_radius;
  _bbox[1][0] =  _radius;
  _bbox[1][1] =  _height*0.5f;
  _bbox[1][2] =  _radius;
}

void
vfrCylinder::drawTri(const vector3 v1, const vector3 v2, const vector3 v3) {
  Bool cylTex = FALSE;
  if ( _texture && _texture->getMapType() == vfrTexture::UVC ) cylTex = TRUE;

  RenderType renderMode = getRenderMode();
  switch ( renderMode ) {
  case RT_NONE:
    return;
  case RT_WIRE:
    cylTex = FALSE;
    glBegin(GL_LINE_LOOP);
    break;
  default:
    glBegin(GL_POLYGON);
  }

  if ( v1[1] > 0.0f )
    glNormal3fv(_snormal[0]);
  else
    glNormal3fv(_snormal[1]);

  if ( cylTex ) {
    vector2 uv;
    if ( v1[1] > 0.0f ) {
      uv[0] =  v1[0] * 0.5f + 0.5f;
      uv[1] = -v1[2] * 0.5f + 0.5f;
      glTexCoord2fv(uv);
      glVertex3f(v1[0]*_radius, v1[1]*_height*0.5f, v1[2]*_radius);

      uv[0] =  v2[0] * 0.5f + 0.5f;
      uv[1] = -v2[2] * 0.5f + 0.5f;
      glTexCoord2fv(uv);
      glVertex3f(v2[0]*_radius, v2[1]*_height*0.5f, v2[2]*_radius);

      uv[0] =  v3[0] * 0.5f + 0.5f;
      uv[1] = -v3[2] * 0.5f + 0.5f;
      glTexCoord2fv(uv);
      glVertex3f(v3[0]*_radius, v3[1]*_height*0.5f, v3[2]*_radius);
    }
    else {
      uv[0] = v1[0] * 0.5f + 0.5f;
      uv[1] = v1[2] * 0.5f + 0.5f;
      glTexCoord2fv(uv);
      glVertex3f(v1[0]*_radius, v1[1]*_height*0.5f, v1[2]*_radius);

      uv[0] = v2[0] * 0.5f + 0.5f;
      uv[1] = v2[2] * 0.5f + 0.5f;
      glTexCoord2fv(uv);
      glVertex3f(v2[0]*_radius, v2[1]*_height*0.5f, v2[2]*_radius);

      uv[0] = v3[0] * 0.5f + 0.5f;
      uv[1] = v3[2] * 0.5f + 0.5f;
      glTexCoord2fv(uv);
      glVertex3f(v3[0]*_radius, v3[1]*_height*0.5f, v3[2]*_radius);
    }
  }
  else {
    glVertex3f(v1[0]*_radius, v1[1]*_height*0.5f, v1[2]*_radius);
    glVertex3f(v2[0]*_radius, v2[1]*_height*0.5f, v2[2]*_radius);
    glVertex3f(v3[0]*_radius, v3[1]*_height*0.5f, v3[2]*_radius);
  }

  glEnd();
}

void
vfrCylinder::drawQuad(const vector3 v1, const vector3 v2,
		      const vector3 v3, const vector3 v4, const int phase) {
  Bool cylTex = FALSE;
  if ( _texture && _texture->getMapType() == vfrTexture::UVC ) cylTex = TRUE;

  RenderType renderMode = getRenderMode();
  switch ( renderMode ) {
  case RT_NONE:
    return;
  case RT_WIRE:
    cylTex = FALSE;
    glBegin(GL_LINE_LOOP);
    break;
  default:
    glBegin(GL_POLYGON);
  }

  if ( cylTex ) {
    vector2 uv;

    getUVC(phase, v1, uv); glTexCoord2fv(uv);
    glNormal3f(v1[0], 0.0f, v1[2]);
    glVertex3f(v1[0]*_radius, v1[1]*_height*0.5f, v1[2]*_radius);

    getUVC(phase, v2, uv); glTexCoord2fv(uv);
    glNormal3f(v2[0], 0.0f, v2[2]);
    glVertex3f(v2[0]*_radius, v2[1]*_height*0.5f, v2[2]*_radius);

    getUVC(phase, v3, uv); glTexCoord2fv(uv);
    glNormal3f(v3[0], 0.0f, v3[2]);
    glVertex3f(v3[0]*_radius, v3[1]*_height*0.5f, v3[2]*_radius);

    getUVC(phase, v4, uv); glTexCoord2fv(uv);
    glNormal3f(v4[0], 0.0f, v4[2]);
    glVertex3f(v4[0]*_radius, v4[1]*_height*0.5f, v4[2]*_radius);
  }
  else {
    glNormal3f(v1[0], 0.0f, v1[2]);
    glVertex3f(v1[0]*_radius, v1[1]*_height*0.5f, v1[2]*_radius);

    glNormal3f(v2[0], 0.0f, v2[2]);
    glVertex3f(v2[0]*_radius, v2[1]*_height*0.5f, v2[2]*_radius);

    glNormal3f(v3[0], 0.0f, v3[2]);
    glVertex3f(v3[0]*_radius, v3[1]*_height*0.5f, v3[2]*_radius);

    glNormal3f(v4[0], 0.0f, v4[2]);
    glVertex3f(v4[0]*_radius, v4[1]*_height*0.5f, v4[2]*_radius);
  }

  glEnd();
}

void
vfrCylinder::subdivTri(const vector3 v1, const vector3 v2, const vector3 v3,
		       const int level) {
  if ( level <= 0 ) {
    drawTri(v1, v2, v3);
    return;
  }

  CES::Vec3<float> v23;
  v23[0] = v2[0] + v3[0];
  v23[1] = 0.0f;
  v23[2] = v2[2] + v3[2];
  v23.UnitVec();
  v23[1] = v2[1];

  subdivTri(v1, v2, v23.m_v, level -1);
  subdivTri(v1, v23.m_v, v3, level -1);
}

void
vfrCylinder::subdivQuad(const vector3 v1, const vector3 v2,
			const vector3 v3, const vector3 v4,
			const int level, const int phase) {
  if ( level <= 0 ) {
    drawQuad(v1, v2, v3, v4, phase);
    return;
  }

  CES::Vec3<float> v12, v34;

  v12[0] = v1[0] + v2[0];
  v12[1] = 0.0f;
  v12[2] = v1[2] + v2[2];
  v12.UnitVec();
  v12[1] = v1[1];

  v34[0] = v3[0] + v4[0];
  v34[1] = 0.0f;
  v34[2] = v3[2] + v4[2];
  v34.UnitVec();
  v34[1] = v3[1];

  subdivQuad(v1, v12.m_v, v34.m_v, v4, level -1, phase);
  subdivQuad(v12.m_v, v2, v3, v34.m_v, level -1, phase);
}

void
vfrCylinder::renderSolid() {
  // display-list check
  if ( beginDispList(DLF_SOLID) ) return;

  register int i;
  if ( _showTop ) {
    for ( i = 0; i < 4; i++ )
      subdivTri(_surface[_tindex[i][0]],
                _surface[_tindex[i][1]],
                _surface[_tindex[i][2]], _subdiv);
  }
  if ( _showBottom ) {
    for ( i = 4; i < 8; i++ )
      subdivTri(_surface[_tindex[i][0]],
                _surface[_tindex[i][1]],
                _surface[_tindex[i][2]], _subdiv);
  }

  for ( i = 0; i < 4; i++ )
    subdivQuad(_surface[_qindex[i][0]],
               _surface[_qindex[i][1]],
               _surface[_qindex[i][2]],
               _surface[_qindex[i][3]], _subdiv, i);

  // end display-list definition
  endDispList(DLF_SOLID);
}

void
vfrCylinder::renderWire() {
  // display-list check
  if ( beginDispList(DLF_WIRE) ) return;

  register int i;
  if ( _showTop ) {
    for ( i = 0; i < 4; i++ )
      subdivTri(_surface[_tindex[i][0]],
                _surface[_tindex[i][1]],
                _surface[_tindex[i][2]], _subdiv);
  }
  if ( _showBottom ) {
    for ( i = 4; i < 8; i++ )
      subdivTri(_surface[_tindex[i][0]],
                _surface[_tindex[i][1]],
                _surface[_tindex[i][2]], _subdiv);
  }

  for ( i = 0; i < 4; i++ )
    subdivQuad(_surface[_qindex[i][0]],
               _surface[_qindex[i][1]],
               _surface[_qindex[i][2]],
               _surface[_qindex[i][3]], _subdiv, i);

  // end display-list definition
  endDispList(DLF_WIRE);
}

// STATIC
void vfrCylinder::getUVC(const int phase, const vector3 p, vector2 uv) {
  // phase must be [0 .. 4], p must be unit-vector

  double th = atan2((double)p[0], (double)p[2]);
  switch ( phase ) {
  case 3:
    if ( th < 0.0 ) th = -th;
    break;
  case 2:
    if ( th > 0.0 ) th = -th;
    break;
  }
  uv[0] = (float)(th * 0.5 / M_PI + 0.5);
  uv[1] = p[1] * 0.5f + 0.5f;
}

// EOF
