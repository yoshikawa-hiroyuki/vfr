//
// vfrBall
//   Sphere Ball Object Class
//

#include "vfrBall.h"

/* static members */

const vector3 vfrBall::_surface[6] = {
  { 0.0f, 1.0f, 0.0f}, { 1.0f, 0.0f, 0.0f}, { 0.0f, 0.0f, 1.0f},
  {-1.0f, 0.0f, 0.0f}, { 0.0f, 0.0f,-1.0f}, { 0.0f,-1.0f, 0.0f}
};
const int vfrBall::_sindex[8][3] = {
  {0, 2, 1}, {0, 3, 2}, {0, 4, 3}, {0, 1, 4},
  {5, 1, 2}, {5, 2, 3}, {5, 3, 4}, {5, 4, 1}
};


vfrBall::vfrBall(const std::string& nm, const Bool ssm) : vfrNode(nm, ssm) {
  makeBall(0.5f);
  setSubdiv(2);
}

vfrBall::vfrBall(const float r, const std::string& nm, const Bool ssm)
  : vfrNode(nm, ssm) {
  makeBall(r);
  setSubdiv(2);
}

void
vfrBall::makeBall(const float r) {
  setRadius(r);
  vector3 v0[1] = {0.0f, 0.0f, 0.0f};
  setVerts(1, v0);
  generateBbox();
}

vfrBall::~vfrBall() {
}


void
vfrBall::setRadius(const float r) {
  if ( _radius == r ) return;
  _radius = r > EPSF ? r : EPSF;
  generateBbox();
  notice();
}

void
vfrBall::setSubdiv(const int si) {
  if ( _subdiv == si ) return;
  if ( si < 0 ) return;
  _subdiv = si;
  notice();
}

void
vfrBall::generateBbox() {
  _bbox[0][0] = -_radius;
  _bbox[1][0] =  _radius;
  _bbox[0][1] = -_radius;
  _bbox[1][1] =  _radius;
  _bbox[0][2] = -_radius;
  _bbox[1][2] =  _radius;
}

void
vfrBall::drawFace(const vector3 v1, const vector3 v2, const vector3 v3,
		  const int phase) {
  Bool cylTex = FALSE;
  if ( _texture && _texture->getMapType() == vfrTexture::UVC )
    cylTex = TRUE;

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
    glNormal3fv(v1);
    glVertex3f(v1[0]*_radius, v1[1]*_radius, v1[2]*_radius);

    getUVC(phase, v2, uv); glTexCoord2fv(uv);
    glNormal3fv(v2);
    glVertex3f(v2[0]*_radius, v2[1]*_radius, v2[2]*_radius);

    getUVC(phase, v3, uv); glTexCoord2fv(uv);
    glNormal3fv(v3);
    glVertex3f(v3[0]*_radius, v3[1]*_radius, v3[2]*_radius);
    
  }
  else {
    glNormal3fv(v1);
    glVertex3f(v1[0]*_radius, v1[1]*_radius, v1[2]*_radius);
    glNormal3fv(v2);
    glVertex3f(v2[0]*_radius, v2[1]*_radius, v2[2]*_radius);
    glNormal3fv(v3);
    glVertex3f(v3[0]*_radius, v3[1]*_radius, v3[2]*_radius);
  }

  glEnd();
}

void
vfrBall::subdivFace(const vector3 v1, const vector3 v2, const vector3 v3,
		    const int level, const int phase) {
  if ( level <= 0 ) {
    drawFace(v1, v2, v3, phase);
    return;
  }

  CES::Vec3<float> v12, v23, v31;

  for ( register int i = 0; i < 3; i++ ) {
    v12[i] = v1[i] + v2[i];
    v23[i] = v2[i] + v3[i];
    v31[i] = v3[i] + v1[i];
  }
  v12.UnitVec(); v23.UnitVec(); v31.UnitVec();

  subdivFace(v1,  v12.m_v, v31.m_v, level -1, phase);
  subdivFace(v2,  v23.m_v, v12.m_v, level -1, phase);
  subdivFace(v3,  v31.m_v, v23.m_v, level -1, phase);
  subdivFace(v12.m_v, v23.m_v, v31.m_v, level -1, phase);
}

void
vfrBall::renderSolid() {
  // display-list check
  if ( beginDispList(DLF_SOLID) ) return;

  for ( register int i = 0; i < 8; i++ )
    subdivFace(_surface[_sindex[i][0]], _surface[_sindex[i][1]],
               _surface[_sindex[i][2]], _subdiv, i);

  // end display-list definition
  endDispList(DLF_SOLID);
}

void
vfrBall::renderWire() {
  // display-list check
  if ( beginDispList(DLF_WIRE) ) return;

  for ( register int i = 0; i < 8; i++ )
    subdivFace(_surface[_sindex[i][0]], _surface[_sindex[i][1]],
               _surface[_sindex[i][2]], _subdiv, i);

  // end display-list definition
  endDispList(DLF_WIRE);
}

// STATIC
void
vfrBall::getUVC(const int phase, const vector3 p, vector2 uv) {
  // phase must be [0 .. 7], p must be unit-vector

  if ( p[1] >= 1.f || p[1] <= -1.f ) { // polar case
    switch ( phase ) {
    case 2: case 6: uv[0] = 0.125f; break; 
    case 1: case 5: uv[0] = 0.375f; break; 
    case 0: case 4: uv[0] = 0.625f; break; 
    case 3: case 7: uv[0] = 0.875f; break; 
    default: uv[0] = 0.5f;
    }
  } else {
    double th = atan2((double)p[0], (double)p[2]);
    switch ( phase ) {
    case 3: case 7:
      if ( th < 0.0 ) th = -th;
      break;
    case 2: case 6:
      if ( th > 0.0 ) th = -th;
      break;
    }
    uv[0] = (float)(th * 0.5 / M_PI + 0.5);
  }

  uv[1] = p[1] * 0.5f + 0.5f;
}

// EOF
