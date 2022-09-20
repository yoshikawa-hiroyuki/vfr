//
// vfrLight
//   Light Object class
//
#include "vfrLight.h"

const vector4 positionPara  = {0.0f, 0.0f, 1.0f, 0.0f};
const vector4 positionPoint = {0.0f, 0.0f, 0.0f, 1.0f};
const vector3 directionSpot = {0.0f, 0.0f, -1.0f};
const float cutoffPoint[1]  = {180.0f};


vfrLight::vfrLight() {
  makeLight(0);
}
vfrLight::vfrLight(const int x) {
  makeLight(x);
}

vfrLight::~vfrLight() {
}

void
vfrLight::makeLight(const int x) {
  setLightNum(x);

  setAmbient(0.2f, 0.2f, 0.2f, 1.0f);
  setDiffuse(1.0f, 1.0f, 1.0f, 1.0f);
  setSpecular(1.0f, 1.0f, 1.0f, 1.0f);
  setSpotCutoff(45.0f);

  setLightType(LT_DIRECTIONAL);
  setOnOff(FALSE);
  setShowMode(FALSE);

  generateBbox();
}

void vfrLight::reset() {
  makeLight(_order);
  notice();
}

void vfrLight::setOnOff(const Bool x) {
  _on = x;
  notice();
}

void vfrLight::setLightType(const LightType x) {
  _type = x;
  notice();
}

void vfrLight::setShowMode(const Bool x) {
  _showLight = x;
  notice();
}

void vfrLight::setAmbient(const vector4 amb) {
  _ambient[0] = amb[0];
  _ambient[1] = amb[1];
  _ambient[2] = amb[2];
  _ambient[3] = amb[3];
  notice();
}
void
vfrLight::setAmbient(const float a0, const float a1, const float a2, const float a3) {
  vector4 amb= {a0, a1, a2, a3};
  setAmbient(amb);
}

void
vfrLight::setDiffuse(const vector4 dif) {
  _diffuse[0] = dif[0];
  _diffuse[1] = dif[1];
  _diffuse[2] = dif[2];
  _diffuse[3] = dif[3];
  notice();
}
void
vfrLight::setDiffuse(const float d0, const float d1, const float d2, const float d3) {
  vector4 dif = {d0, d1, d2, d3};
  setDiffuse(dif);
}

void
vfrLight::setSpecular(const vector4 spc) {
  _specular[0] = spc[0];
  _specular[1] = spc[1];
  _specular[2] = spc[2];
  _specular[3] = spc[3];
  notice();
}
void
vfrLight::setSpecular(const float s0, const float s1, const float s2, const float s3) {
  vector4 spc = {s0, s1, s2, s3};
  setSpecular(spc);
}

void vfrLight::setSpotCutoff(const float c) {
  if ( c < 1.0 )
    return;
  _spotCutoff[0] = c;
  if ( getLightType() == LT_SPOT )
    notice();
}

Bool
vfrLight::setLightNum(const int x) {
  if ( x < 0 || x >= VFR_LIGHT_MAX ) {
    _order = 0;
    return FALSE;
  } else {
    _order = x;
    return TRUE;
  }
}

#define  L_PI_PER_180  0.017453293

void
vfrLight::render(vfrMaterialStack*) {
  GLenum l;

  switch ( _order ) {
  case 0:
    l = GL_LIGHT0; break;
  case 1:
    l = GL_LIGHT1; break;
  case 2:
    l = GL_LIGHT2; break;
  case 3:
    l = GL_LIGHT3; break;
  default:
    return;
  }

  if ( isOn() ) {
    glEnable(l);
  } else {
    glDisable(l);
    return;
  }

  applyMatrix();

  glLightfv(l, GL_AMBIENT, _ambient);
  glLightfv(l, GL_DIFFUSE, _diffuse);
  glLightfv(l, GL_SPECULAR, _specular);

  switch ( getLightType() ) {
  case LT_POINT:
    glLightfv(l, GL_POSITION, positionPoint);
    glLightfv(l, GL_SPOT_CUTOFF, cutoffPoint);
    break;
  case LT_SPOT:
    glLightfv(l, GL_POSITION, positionPoint);
    glLightfv(l, GL_SPOT_CUTOFF, _spotCutoff);
    glLightfv(l, GL_SPOT_DIRECTION, directionSpot);
    break;
  case LT_DIRECTIONAL:
  default:
    glLightfv(l, GL_POSITION, positionPara);
  }

  drawLight();

  unApplyMatrix();
}

void
vfrLight::drawLight() {
  if ( ! _showLight ) return;

  if ( _pickable & PT_OBJECT )
    glPushName((GLuint)_currentSeq);

  const vfrMaterial* mp = getMaterial();
  if ( mp )
    mp->apply();
  glDisable(GL_LIGHTING);
  glColor3f(_diffuse[0], _diffuse[1], _diffuse[2]);

  // display-list check
  if ( ! beginDispList(DLF_WIRE) ) {

    switch ( getLightType() ) {
    case LT_POINT: {
      glBegin(GL_LINES);
      glVertex3f(-1.0f, 0.0f, 0.0f);
      glVertex3f( 1.0f, 0.0f, 0.0f);
      glEnd();
      glBegin(GL_LINES);
      glVertex3f(0.0f,-1.0f, 0.0f);
      glVertex3f(0.0f, 1.0f, 0.0f);
      glEnd();
      glBegin(GL_LINES);
      glVertex3f(0.0f, 0.0f,-1.0f);
      glVertex3f(0.0f, 0.0f, 1.0f);
      glEnd();
      break;
    }
    case LT_SPOT: {
      glBegin(GL_LINES); // Axis
      glVertex3f(0.0f, 0.0f, 0.0f);
      glVertex3f(directionSpot[0], directionSpot[1], directionSpot[2]);
      glEnd();
      float ssc = (float)sin(_spotCutoff[0] * L_PI_PER_180);
      float csc = (float)cos(_spotCutoff[0] * L_PI_PER_180);
      glBegin(GL_LINE_STRIP);
      glVertex3f(ssc, 0.0f, -csc);
      glVertex3f(0.0f, 0.0f, 0.0f);
      glVertex3f(-ssc, 0.0f, -csc);
      glEnd();
      glBegin(GL_LINE_STRIP);
      glVertex3f(0.0f, ssc, -csc);
      glVertex3f(0.0f, 0.0f, 0.0f);
      glVertex3f(0.0f, -ssc, -csc);
      glEnd();
      break;
    }
    case LT_DIRECTIONAL:
    default:
      glBegin(GL_LINES);
      glVertex3f(0.0f, 0.0f, 0.0f);
      glVertex3f(positionPara[0], positionPara[1], positionPara[2]);
      glEnd();
      break;
    }

    // end display-list definition
    endDispList(DLF_WIRE);

  } // end of if(!beginDispList(DLF_WIRE))

  glEnable(GL_LIGHTING);
  if ( mp )
    mp->unApply();

  // Bounding Box
  if ( _pickable & PT_BBOX || !_pickable )
    ;
  else
    glLoadName((GLuint)0);
  drawBbox();

  if ( _pickable & PT_OBJECT )
    glPopName();
}

void
vfrLight::generateBbox() {
  _bbox[0][0] = _bbox[0][1] = _bbox[0][2] = -1.0f;
  _bbox[1][0] = _bbox[1][1] = _bbox[1][2] = 1.0f;
}

// EOF
