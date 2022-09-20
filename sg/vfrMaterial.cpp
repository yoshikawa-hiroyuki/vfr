//
// vfrMaterial
//   Material Data Class
//

#include "vfrMaterial.h"
#include "vfrNode.h"

const GLushort stipplePattern[] = {
  0xFFFF, 0x3333, 0x00FF, 0x88FF, 0x111F
};


vfrMaterial::vfrMaterial() : _parent(NULL) {
  reset();
}

vfrMaterial::vfrMaterial(const vfrMaterial& org) : _parent(NULL) {
  *this = org;
}

vfrMaterial::~vfrMaterial() {
}

void
vfrMaterial::operator=(const vfrMaterial& org) {
  specular[0] = org.specular[0];
  specular[1] = org.specular[1];
  specular[2] = org.specular[2];
  specular[3] = org.specular[3];

  ambient[0] = org.ambient[0];
  ambient[1] = org.ambient[1];
  ambient[2] = org.ambient[2];
  ambient[3] = org.ambient[3];

  shininess[0] = org.shininess[0];

  emission[0] = org.emission[0];
  emission[1] = org.emission[1];
  emission[2] = org.emission[2];
  emission[3] = org.emission[3];

  lineStipple = org.lineStipple;
  lineWidth = org.lineWidth;
  pointSize = org.pointSize;
  pointSymbol = org.pointSymbol;
  renderMode = org.renderMode;
  faceMode = org.faceMode;

  setAuxPointColor(org.useAuxPC, org.auxPointColor);
  setAuxLineColor(org.useAuxLC, org.auxLineColor);
}

void
vfrMaterial::reset() {
  setSpecular(0.0f, 0.0f, 0.0f, 1.0f);
  setAmbient(0.2f, 0.2f, 0.2f, 1.0f);
  setEmission(0.0f, 0.0f, 0.0f, 1.0f);
  setShininess(5.0f);
  setLineStipple(ST_SOLID);
  setLineWidth(1.0f);
  setPointSize(1.0f);
  setPointSymbol(SYM_NORMAL);
  setRenderMode(RT_SMOOTH);
  setFaceMode(PF_BOTH);
  vector4 auxc = {1.0f, 1.0f, 1.0f, 1.0f};
  setAuxPointColor(FALSE, auxc);
  setAuxLineColor(FALSE, auxc);
};

GLenum
vfrMaterial::getFaceMode() const {
  switch ( faceMode ) {
  case PF_FRONT:
    return GL_FRONT;
  case PF_BACK:
    return GL_BACK;
  case PF_BOTH:
  default:
    return GL_FRONT_AND_BACK;
  }
}

void
vfrMaterial::apply() const {
  GLenum face = getFaceMode();
  glMaterialfv(face, GL_AMBIENT, ambient);
  glMaterialfv(face, GL_SPECULAR, specular);
  glMaterialfv(face, GL_SHININESS, shininess);
  glMaterialfv(face, GL_EMISSION, emission);
  glColorMaterial(face, GL_DIFFUSE);
  glEnable(GL_COLOR_MATERIAL);

  glPointSize(pointSize);
  glLineWidth(lineWidth);
  if ( lineStipple != ST_SOLID )
    glEnable(GL_LINE_STIPPLE);
  switch ( lineStipple ) {
  case ST_DOT:
    glLineStipple(1, stipplePattern[1]);
    break;
  case ST_DASH:
    glLineStipple(1, stipplePattern[2]);
    break;
  case ST_DDASH1:
    glLineStipple(1, stipplePattern[3]);
    break;
  case ST_DDASH2:
    glLineStipple(1, stipplePattern[4]);
    break;
  case ST_SOLID:
  default:
    glLineStipple(1, stipplePattern[0]);
  }

  if ( face != GL_FRONT_AND_BACK ) {
    GLenum cull;
    if ( face == GL_FRONT )
      cull = GL_BACK;
    else
      cull = GL_FRONT;
    glCullFace(cull);
  }

  if ( renderMode ) {
    if ( face != GL_FRONT_AND_BACK )
      glEnable(GL_CULL_FACE);
    else
      glDisable(GL_CULL_FACE);
  }
}

void
vfrMaterial::unApply() const {
  if ( lineStipple != ST_SOLID )
    glDisable(GL_LINE_STIPPLE);
  if ( renderMode ) {
    GLenum face = getFaceMode();
    if ( face != GL_FRONT_AND_BACK )
      glDisable(GL_CULL_FACE);
  }
}

void
vfrMaterial::setLineStipple(const StippleType st) {
  lineStipple = st;
  if ( renderMode & RT_WIRE )
    notice();
}

void
vfrMaterial::setLineWidth(const float lw) {
  if ( lw < 0.0f )
    return;
  lineWidth = lw;
  if ( renderMode & RT_WIRE )
    notice();
}

void
vfrMaterial::setPointSymbol(const SymbolType st) {
  pointSymbol = st;
  if ( renderMode & RT_POINT )
    notice();
}

void
vfrMaterial::setPointSize(const float ps) {
  if ( ps < 0.0f )
    return;
  pointSize = ps;
  if ( renderMode & RT_POINT )
    notice();
}

void
vfrMaterial::setSpecular(const vector4 d) {
  specular[0] = (GLfloat)d[0];
  specular[1] = (GLfloat)d[1];
  specular[2] = (GLfloat)d[2];
  specular[3] = (GLfloat)d[3];
  notice();
}
void
vfrMaterial::setSpecular(const float d0, const float d1,
    const float d2, const float d3) {
  specular[0] = (GLfloat)d0;
  specular[1] = (GLfloat)d1;
  specular[2] = (GLfloat)d2;
  specular[3] = (GLfloat)d3;
  notice();
}
void
vfrMaterial::getSpecular(vector4& d) const {
  d[0] = (float)specular[0];
  d[1] = (float)specular[1];
  d[2] = (float)specular[2];
  d[3] = (float)specular[3];
}

void
vfrMaterial::setAmbient(const vector4 d) {
  ambient[0] = (GLfloat)d[0];
  ambient[1] = (GLfloat)d[1];
  ambient[2] = (GLfloat)d[2];
  ambient[3] = (GLfloat)d[3];
  notice();
}
void
vfrMaterial::setAmbient(const float d0, const float d1,
    const float d2, const float d3) {
  ambient[0] = (GLfloat)d0;
  ambient[1] = (GLfloat)d1;
  ambient[2] = (GLfloat)d2;
  ambient[3] = (GLfloat)d3;
  notice();
}
void
vfrMaterial::getAmbient(vector4& d) const {
  d[0] = (float)ambient[0];
  d[1] = (float)ambient[1];
  d[2] = (float)ambient[2];
  d[3] = (float)ambient[3];
}

void
vfrMaterial::setShininess(const vector4 d) {
  shininess[0] = (GLfloat)d[0];
  notice();
}
void
vfrMaterial::setShininess(const float d) {
  shininess[0] = (GLfloat)d;
  notice();
}
float
vfrMaterial::getShininess() const {
  return (float)shininess[0];
}

void
vfrMaterial::setEmission(const vector4 d) {
  emission[0] = (GLfloat)d[0];
  emission[1] = (GLfloat)d[1];
  emission[2] = (GLfloat)d[2];
  emission[3] = (GLfloat)d[3];
  notice();
}
void
vfrMaterial::setEmission(const float d0, const float d1,
    const float d2, const float d3) {
  emission[0] = (GLfloat)d0;
  emission[1] = (GLfloat)d1;
  emission[2] = (GLfloat)d2;
  emission[3] = (GLfloat)d3;
  notice();
}
void
vfrMaterial::getEmission(vector4& d) const {
  d[0] = (float)emission[0];
  d[1] = (float)emission[1];
  d[2] = (float)emission[2];
  d[3] = (float)emission[3];
}

void
vfrMaterial::setAuxPointColor(const Bool mode, const vector4 auxc) {
  useAuxPC = mode;
  if ( auxc ) {
    auxPointColor[0] = VFR_CLAMP(0.0f, 1.0f, auxc[0]);
    auxPointColor[1] = VFR_CLAMP(0.0f, 1.0f, auxc[1]);
    auxPointColor[2] = VFR_CLAMP(0.0f, 1.0f, auxc[2]);
    auxPointColor[3] = VFR_CLAMP(0.0f, 1.0f, auxc[3]);
  }
  if ( renderMode & RT_POINT )
    notice();
}
void
vfrMaterial::setAuxPointColor(const Bool mode,
   const float r, const float g, const float b) {
  vector4 auxc;
  auxc[0] = r;
  auxc[1] = g;
  auxc[2] = b;
  auxc[3] = 1.0f;
  setAuxPointColor(mode, auxc);
}

Bool
vfrMaterial::useAuxPointColor(vector4& auxc) const {
  auxc[0] = auxPointColor[0];
  auxc[1] = auxPointColor[1];
  auxc[2] = auxPointColor[2];
  auxc[3] = auxPointColor[3];
  return useAuxPC;
}

void
vfrMaterial::setAuxLineColor(const Bool mode, const vector4 auxc) {
  useAuxLC = mode;
  if ( auxc ) {
    auxLineColor[0] = VFR_CLAMP(0.0f, 1.0f, auxc[0]);
    auxLineColor[1] = VFR_CLAMP(0.0f, 1.0f, auxc[1]);
    auxLineColor[2] = VFR_CLAMP(0.0f, 1.0f, auxc[2]);
    auxLineColor[3] = VFR_CLAMP(0.0f, 1.0f, auxc[3]);
  }
  if ( renderMode & RT_WIRE )
    notice();
}
void
vfrMaterial::setAuxLineColor(const Bool mode,
    const float r, const float g, const float b) {
  vector4 auxc;
  auxc[0] = r;
  auxc[1] = g;
  auxc[2] = b;
  auxc[3] = 1.0f;
  setAuxLineColor(mode, auxc);
}

Bool
vfrMaterial::useAuxLineColor(vector4& auxc) const {
  auxc[0] = auxLineColor[0];
  auxc[1] = auxLineColor[1];
  auxc[2] = auxLineColor[2];
  auxc[3] = auxLineColor[3];
  return useAuxLC;
}

void
vfrMaterial::setParent(vfrNode* p) {
  _parent = p;
  notice();
}

void
vfrMaterial::notice() {
  if ( _parent )
    _parent->notice();
}

// EOF
