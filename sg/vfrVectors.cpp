//
// vfrVectors
//   Vector-set Object Class
//

#include "vfrVectors.h"


vfrVectors::vfrVectors(const std::string& nm, const Bool ssm)
  : vfrNode(nm, ssm), _head(TRUE), _headScale(0.2f), _headWidth(0.05f),
    _scaleFac(1.0f), _pType(NORMAL), _showZero(TRUE) {
}

vfrVectors::~vfrVectors() {
}

void
vfrVectors::setHeadMode(const Bool hm) {
  if ( hm != _head ) {
    _head = hm;
    notice();
  }
}

void
vfrVectors::setHeadScale(const float hs) {
  if ( _headScale == hs ) return;
  _headScale = VFR_CLAMP(0.0f, 1.0f, hs);
  if ( _head )
    notice();
}

void
vfrVectors::setHeadWidth(const float hw) {
  if ( _headWidth == hw ) return;
  _headWidth = VFR_CLAMP(0.0f, 1.0f, hw);
  if ( _head )
    notice();
}

void
vfrVectors::setScaleFac(const float sf) {
  if ( sf != _scaleFac ) {
    _scaleFac = sf;
    notice();
  }
}

void
vfrVectors::setShowZero(const Bool szm) {
  if ( _showZero != szm ) {
    _showZero = szm;
    notice();
  }
}

RenderType
vfrVectors::getRenderMode() const {
  const vfrMaterial *mp = getMaterial();
  if ( ! mp )
    return RT_WIRE;
  RenderType prt = mp->getRenderMode();
  if ( prt == RT_NONE || prt == RT_POINT )
    return prt;
  return RT_WIRE;
}

void
vfrVectors::renderWire() {
  // display-list check
  if ( beginDispList(DLF_WIRE) ) return;

  register Bool auxMode = useAuxLineColor();

  for ( register int i = 0; i < nVerts; i++ ) {
    if ( ! auxMode && _colorMode == AT_PER_VERTEX && i < nColors )
      glColor4fv(_colors[i]);

    if ( i >= nNormals ) break;
    CES::Vec3<float> p(_normals[i]); p = p * _scaleFac;
    float Lp = p.Length();
    if ( Lp < EPSF ) {
      if ( _showZero ) {
	glBegin(GL_POINTS);
	glVertex3fv(_verts[i]);
	glEnd();
      }
      continue;
    }

    CES::Vec3<float> v(_verts[i]);
    CES::Vec3<float> pit, tip;
    switch ( _pType ) {
    case NORMAL:
      tip = v + p;
      pit = v;
      break;
    case CENTER:
      tip = v + (p * 0.5f);
      pit = v - (p * 0.5f);
      break;
    case TIP:
      tip = v;
      pit = v - p;
      break;
    }
    glBegin(GL_LINES);
    glVertex3fv(pit.m_v);
    glVertex3fv(tip.m_v);
    glEnd();

    if ( ! _head ) continue;
    CES::Vec3<float> vfr(-p[1], p[0], 0.0f);
    if ( vfr[0] == 0.0f && vfr[1] == 0.0f ) vfr[0] = 1.0f;
    CES::Vec3<float> v2, v3;
    v2 = p ^ vfr;
    v3 = p ^ v2;
    v2.UnitVec(); v3.UnitVec();

    CES::Vec3<float> p1;
    p1 = p * (1.0f-_headScale);
    p1 = p1 + pit;
    float mw = float(_headWidth * Lp);

    glBegin(GL_LINE_STRIP);
    glVertex3f(p1[0] +v2[0]*mw, p1[1] +v2[1]*mw, p1[2] +v2[2]*mw);
    glVertex3fv(tip.m_v);
    glVertex3f(p1[0] -v2[0]*mw, p1[1] -v2[1]*mw, p1[2] -v2[2]*mw);
    glEnd();

    glBegin(GL_LINE_STRIP);
    glVertex3f(p1[0] +v3[0]*mw, p1[1] +v3[1]*mw, p1[2] +v3[2]*mw);
    glVertex3fv(tip.m_v);
    glVertex3f(p1[0] -v3[0]*mw, p1[1] -v3[1]*mw, p1[2] -v3[2]*mw);
    glEnd();
  } // end of for(i)

  // end display-list definition
  endDispList(DLF_WIRE);
}

void
vfrVectors::renderFeedBack(const unsigned int oid) {
  if ( _currentSeq != oid )
    return;

  applyMatrix();

  register int i;
  switch ( _feedbackMode ) {
  case FB_VERTEX:
    for ( i = 0; i < nVerts; i++ ) {
      glPassThrough((GLfloat)i);
      glBegin(GL_POINTS);
      glVertex3fv(_verts[i]);
      glEnd();
    }
    break;
  case FB_EDGE:
    for ( i = 0; i < nVerts; i++ ) {
      if ( i >= nNormals ) break;
      glPassThrough((GLfloat)i);
      CES::Vec3<float> p(_normals[i]); p = p * _scaleFac;
      CES::Vec3<float> v(_verts[i]);
      CES::Vec3<float> pit, tip;
      switch ( _pType ) {
      case NORMAL:
	tip = v + p;
	pit = v;
        break;
      case CENTER:
	tip = v + (p * 0.5f);
	pit = v - (p * 0.5f);
        break;
      case TIP:
	tip = v;
	pit = v - p;
        break;
      }
      glBegin(GL_LINES);
      glVertex3fv(pit.m_v);
      glVertex3fv(tip.m_v);
      glEnd();
    }
    break;
  case FB_FACE:
  default:
    break; // do nothing.
  }

  unApplyMatrix();
}

// EOF
