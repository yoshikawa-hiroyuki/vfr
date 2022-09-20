//
// vfrCone
//   Circular Cone Object Class
//

#include "vfrCone.h"

/* static members */

const vector3 vfrCone::_surface[6] = {
  { 0.0f, 1.0f, 0.0f}, { 1.0f,-1.0f, 0.0f}, { 0.0f,-1.0f, 1.0f},
  {-1.0f,-1.0f, 0.0f}, { 0.0f,-1.0f,-1.0f}, { 0.0f,-1.0f, 0.0f}
};
const vector3 vfrCone::_snormal[2] = {
  {0.0f, 1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}
};
const int vfrCone::_sindex[8][3] = {
  {0, 2, 1}, {0, 3, 2}, {0, 4, 3}, {0, 1, 4},
  {5, 1, 2}, {5, 2, 3}, {5, 3, 4}, {5, 4, 1}
};


vfrCone::vfrCone(const std::string& nm, const Bool ssm)
 : vfrNode(nm, ssm), _showBottom(TRUE) {
  makeCone(0.5f, 1.0f);
  setSubdiv(2);
}

vfrCone::vfrCone(const float r, const float h,
		 const std::string& nm, const Bool ssm)
 : vfrNode(nm, ssm), _showBottom(TRUE) {
  makeCone(r, h);
  setSubdiv(2);
}

void
vfrCone::makeCone(const float r, const float h) {
  setRadius(r);
  setHeight(h);
  vector3 v0[1];
  v0[0][0] = v0[0][1] = v0[0][2] = 0.0f;
  setVerts(1, v0);

  generateBbox();
}

vfrCone::~vfrCone() {
}

void
vfrCone::setRadius(const float r) {
  if ( _radius == r ) return;
  _radius = r > EPSF ? r : EPSF;
  generateBbox();
  notice();
}

void
vfrCone::setHeight(const float h) {
  if ( _height == h ) return;
  _height = h > EPSF ? h : EPSF;
  generateBbox();
  notice();
}

void
vfrCone::setSubdiv(const int si) {
  if ( _subdiv == si ) return;
  if ( si < 0 ) return;
  _subdiv = si;
  notice();
}

void
vfrCone::setShowBottom(const Bool sb) {
  if ( _showBottom == sb ) return;
  _showBottom = sb;
  notice();
}

void
vfrCone::generateBbox() {
  _bbox[0][0] = -_radius;
  _bbox[0][1] = -_height*0.5f;
  _bbox[0][2] = -_radius;
  _bbox[1][0] =  _radius;
  _bbox[1][1] =  _height*0.5f;
  _bbox[1][2] =  _radius;
}

void
vfrCone::drawFace(const vector3 v1, const vector3 v2, const vector3 v3,
		  const int phase) {
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
    if ( v1[1] > 0.5f ) {
      float u2, u3;

      getUVC(phase, v2, uv); u2 = uv[0];
      glTexCoord2fv(uv);
      glNormal3f(v2[0], 0.0f, v2[2]);
      glVertex3f(v2[0]*_radius, v2[1]*_height*0.5f, v2[2]*_radius);

      getUVC(phase, v3, uv); u3 = uv[0];
      glTexCoord2fv(uv);
      glNormal3f(v3[0], 0.0f, v3[2]);
      glVertex3f(v3[0]*_radius, v3[1]*_height*0.5f, v3[2]*_radius);

      //getUVC(phase, v1, uv);
      uv[0] = (u2 + u3) * 0.5f; uv[1] = 1.f;
      glTexCoord2fv(uv);
      glNormal3fv(_snormal[0]);
      glVertex3f(v1[0]*_radius, v1[1]*_height*0.5f, v1[2]*_radius);
    }
    else {
      glNormal3fv(_snormal[1]);

      getUVC(phase, v1, uv); glTexCoord2fv(uv);
      glVertex3f(v1[0]*_radius, v1[1]*_height*0.5f, v1[2]*_radius);

      getUVC(phase, v2, uv); glTexCoord2fv(uv);
      glVertex3f(v2[0]*_radius, v2[1]*_height*0.5f, v2[2]*_radius);

      getUVC(phase, v3, uv); glTexCoord2fv(uv);
      glVertex3f(v3[0]*_radius, v3[1]*_height*0.5f, v3[2]*_radius);
    }   
  }
  else {
    if ( v1[1] > 0.5f ) {
      glNormal3fv(_snormal[0]);
      glVertex3f(v1[0]*_radius, v1[1]*_height*0.5f, v1[2]*_radius);

      glNormal3f(v2[0], 0.0f, v2[2]);
      glVertex3f(v2[0]*_radius, v2[1]*_height*0.5f, v2[2]*_radius);

      glNormal3f(v3[0], 0.0f, v3[2]);
      glVertex3f(v3[0]*_radius, v3[1]*_height*0.5f, v3[2]*_radius);
    } else {
      glNormal3fv(_snormal[1]);
      glVertex3f(v1[0]*_radius, v1[1]*_height*0.5f, v1[2]*_radius);
      glVertex3f(v2[0]*_radius, v2[1]*_height*0.5f, v2[2]*_radius);
      glVertex3f(v3[0]*_radius, v3[1]*_height*0.5f, v3[2]*_radius);
    }
  }

  glEnd();
}

void
vfrCone::subdivFace(const vector3 v1, const vector3 v2, const vector3 v3,
		    const int level, const int phase) {
  CES::Vec3<float> v23;
  if ( level <= 0 ) {
    drawFace(v1, v2, v3, phase);
    return;
  }
  v23[0] = v2[0] + v3[0];
  v23[1] = 0.0f;
  v23[2] = v2[2] + v3[2];
  v23.UnitVec();
  v23[1] = v2[1];
  subdivFace(v1, v2, v23.m_v, level -1, phase);
  subdivFace(v1, v23.m_v, v3, level -1, phase);
}

void
vfrCone::renderSolid() {
  // display-list check
  if ( beginDispList(DLF_SOLID) ) return;

  register int i;
  for ( i = 0; i < 4; i++ )
    subdivFace(_surface[_sindex[i][0]],
               _surface[_sindex[i][1]],
               _surface[_sindex[i][2]], _subdiv, i);
  if ( _showBottom ) {
    for ( i = 4; i < 8; i++ )
      subdivFace(_surface[_sindex[i][0]],
                 _surface[_sindex[i][1]],
                 _surface[_sindex[i][2]], _subdiv, i);
  }

  // end display-list definition
  endDispList(DLF_SOLID);
}

void
vfrCone::renderWire() {
  // display-list check
  if ( beginDispList(DLF_WIRE) ) return;

  register int i;
  for ( i = 0; i < 4; i++ )
    subdivFace(_surface[_sindex[i][0]],
               _surface[_sindex[i][1]],
               _surface[_sindex[i][2]], _subdiv, i);
  if ( _showBottom ) {
    for ( i = 4; i < 8; i++ )
      subdivFace(_surface[_sindex[i][0]],
                 _surface[_sindex[i][1]],
                 _surface[_sindex[i][2]], _subdiv, i);
  }

  // end display-list definition
  endDispList(DLF_WIRE);
}

// STATIC
void
vfrCone::getUVC(const int phase, const vector3 p, vector2 uv) {
  // phase must be [0 .. 7] : [0 .. 3] is cone side, [4 .. 7] is bottom face

  // bottom face
  if ( phase >= 4 && phase <= 7 ) {
    uv[0] = p[0] * 0.5f + 0.5f;
    uv[1] = p[2] * 0.5f + 0.5f;
    return;
  }

  // tip point
  if ( p[1] > 0.5f ) {
    switch ( phase ) {
    case 2: uv[0] = 0.125f; break; 
    case 1: uv[0] = 0.375f; break; 
    case 0: uv[0] = 0.625f; break; 
    case 3: uv[0] = 0.875f; break; 
    default: uv[0] = 0.5f;
    }
    uv[1] = 1.0f;
    return;
  }

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
  uv[1] = 0.0f;
}

// EOF
