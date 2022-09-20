//
// vfrArrow
//   Arrow Object Class
//

#include "vfrArrow.h"


vfrArrow::vfrArrow(const std::string& nm, const Bool ssm) : vfrNode(nm, ssm) {
  vector3 d = {0.0f, 0.0f, 1.0f};
  makeArrow(d, 1.0f);
}

vfrArrow::vfrArrow(const CES::Vec3<float>& d, const float l,
		   const std::string& nm, const Bool ssm)
  : vfrNode(nm, ssm) {
  makeArrow(d, l);
}

vfrArrow::vfrArrow(const CES::Vec3<float>& d,
		   const std::string& nm, const Bool ssm) : vfrNode(nm, ssm) {
  makeArrow(d, 1.0f);
}

vfrArrow::vfrArrow(const float l,
		   const std::string& nm, const Bool ssm) : vfrNode(nm, ssm) {
  vector3 d = {1.0f, 0.0f, 0.0f};
  makeArrow(d, l);
}

void
vfrArrow::makeArrow(const CES::Vec3<float>& d, const float l) {
  vector3 v = {0.0f, 0.0f, 0.0f};
  setVerts(1, &v);

  setHeadMode(TRUE);
  setHeadScale(0.2f);
  setHeadWidth(0.1f);
  setDirection(d);
  setLength(l);

  generateBbox();
}

vfrArrow::~vfrArrow() {
}

void
vfrArrow::setDirection(const CES::Vec3<float>& d) {
  if ( d.Length() < EPSF ) return;
  _direction = d;
  _direction.UnitVec();
  generateBbox();
  notice();
}

void
vfrArrow::setHeadScale(const float hs) {
  _headScale = VFR_CLAMP(0.0f, 1.0f, hs);
  if ( _head ) notice();
}

void
vfrArrow::setHeadWidth(const float hw) {
  _headWidth = VFR_CLAMP(0.0f, 1.0f, hw);
  if ( _head ) notice();
}

void
vfrArrow::setVector(const CES::Vec3<float>& v) {
  float vl = v.Length();
  if ( vl == 0.0f ) {
    setLength(0.0f);
    return;
  }
  setDirection(v);
  setLength(vl);
}

RenderType
vfrArrow::getRenderMode() const {
  const vfrMaterial* mp = getMaterial();
  if ( ! mp )
    return RT_WIRE;
  RenderType prt = mp->getRenderMode();
  if ( prt == RT_NONE || prt == RT_POINT )
    return prt;
  return RT_WIRE;
}

void
vfrArrow::generateBbox() {
  _bbox[0][0] = 0.0f;
  _bbox[0][1] = 0.0f;
  _bbox[0][2] = 0.0f;
  _bbox[1][0] = _direction[0] * _magnitude;
  _bbox[1][1] = _direction[1] * _magnitude;
  _bbox[1][2] = _direction[2] * _magnitude;
  checkBbox();
}

void
vfrArrow::renderWire() {
  // display-list check
  if ( beginDispList(DLF_WIRE) ) return;

  if ( _magnitude == 0.0f ) {
    glBegin(GL_POINTS);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glEnd();
    return;
  }
  vector3 p;
  p[0] = _direction[0] * _magnitude;
  p[1] = _direction[1] * _magnitude;
  p[2] = _direction[2] * _magnitude;

  glBegin(GL_LINES);
  glVertex3f(0.0f, 0.0f, 0.0f);
  glVertex3fv(p);
  glEnd();

  if ( _head ) {
    CES::Vec3<float> vfr, v2, v3;
    vfr[0] = -_direction[1];
    vfr[1] =  _direction[0];
    vfr[2] =  0.0f;
    if ( vfr[0] == 0.0f && vfr[1] == 0.0f )
      vfr[0] = 1.0f;
    v2 = _direction ^ vfr;
    v3 = _direction ^ v2;
    v2.UnitVec(); v3.UnitVec();

    vector3 p1;
    p1[0] = p[0] * (1.0f - _headScale);
    p1[1] = p[1] * (1.0f - _headScale);
    p1[2] = p[2] * (1.0f - _headScale);
    float mw = _magnitude * _headWidth;

    glBegin(GL_LINE_STRIP);
    glVertex3f(p1[0] +v2[0]*mw, p1[1] +v2[1]*mw, p1[2] +v2[2]*mw);
    glVertex3fv(p);
    glVertex3f(p1[0] -v2[0]*mw, p1[1] -v2[1]*mw, p1[2] -v2[2]*mw);
    glEnd();

    glBegin(GL_LINE_STRIP);
    glVertex3f(p1[0] +v3[0]*mw, p1[1] +v3[1]*mw, p1[2] +v3[2]*mw);
    glVertex3fv(p);
    glVertex3f(p1[0] -v3[0]*mw, p1[1] -v3[1]*mw, p1[2] -v3[2]*mw);
    glEnd();
  } // end of if(head)

  // end display-list definition
  endDispList(DLF_WIRE);
}

// EOF
