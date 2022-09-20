//
// vfrPrimSet
//  Primitive-set Class
//

#include "vfrPrimSet.h"


vfrPrimSet::vfrPrimSet(const std::string& nm, const Bool ss)
  : vfrNode(nm, ss), _prim(NULL) {
  _rsMode = (VFR_PRIMSET_ROT | VFR_PRIMSET_SCALE);
}

vfrPrimSet::~vfrPrimSet() {
  if ( _prim ) _prim->remRef(this);
}

void vfrPrimSet::setPrimitive(vfrNode* p) {
  if ( p == _prim ) return;
  if ( _prim ) _prim->remRef(this);
  _prim = p;
  if ( _prim ) _prim->addRef(this);
  generateBbox();
  notice();
}


// Bbox methods
inline bool
vfrPrimSet::calcBboxPoints(const int idx, CES::Vec3<float>* xbb) {
  if ( idx < 0 || idx >= nVerts ) return false;
  if ( ! xbb || ! _prim ) return false;
  const CES::Vec3<float>* pbb = _prim->getBbox();

  float scFac = 1.f;
  if ( (_rsMode & VFR_PRIMSET_SCALE) && idx < nNormals )
    scFac = CES::Vec3<float>(_normals[idx]).Length();

  xbb[0] = CES::Vec3<float>(_verts[idx]) + pbb[0]*scFac;
  xbb[1] = CES::Vec3<float>(_verts[idx]) + pbb[1]*scFac;
  return true;
}

void vfrPrimSet::rumor(vfrBase* bp) {
  if ( _prim && _prim == bp ) {
    _prim = NULL;
    notice();
    return;
  }
  vfrNode::rumor(bp);
}

void vfrPrimSet::generateBbox() {
  if ( ! _prim ) {
    vfrNode::generateBbox();
    return;
  }

  if ( nVerts < 1 ) {
    _bbox[0] = CES::Vec3<float>();
    _bbox[1] = _bbox[0];
    checkBbox();
    return;
  }

  CES::Vec3<float> xbb[2];
  if ( ! calcBboxPoints(0, _bbox) ) {
    _bbox[0] = CES::Vec3<float>(_verts[0]);
    _bbox[1] = _bbox[0];
  }
  for ( register int i = 1; i < nVerts; i++ ) {
    if ( ! calcBboxPoints(i, xbb) ) {
      xbb[0] = CES::Vec3<float>(_verts[i]);
      xbb[1] = xbb[0];
    }

    if ( _bbox[0][0] > xbb[0][0] ) _bbox[0][0] = xbb[0][0];
    if ( _bbox[1][0] < xbb[1][0] ) _bbox[1][0] = xbb[1][0];

    if ( _bbox[0][1] > xbb[0][1] ) _bbox[0][1] = xbb[0][1];
    if ( _bbox[1][1] < xbb[1][1] ) _bbox[1][1] = xbb[1][1];

    if ( _bbox[0][2] > xbb[0][2] ) _bbox[0][2] = xbb[0][2];
    if ( _bbox[1][2] < xbb[1][2] ) _bbox[1][2] = xbb[1][2];
  }
  checkBbox();
}

void vfrPrimSet::generateBbox(const CES::Vec3<float>& newVert) {
  if ( ! _prim ) {
    vfrNode::generateBbox(newVert);
    return;
  }

  generateBbox();
}


// Rendering methods
void vfrPrimSet::callPrimRender(const RenderType rt,
				const vector3 pos, const vector3 vec) {
  if ( ! _prim ) return;

  // ignore material and matrix of the primitive

  // apply local transform
  glPushMatrix();
  glTranslatef(pos[0], pos[1], pos[2]);
  if ( vec && _rsMode ) {
    CES::Vec3<float> v(vec);
    float l = v.Length(); if ( l < EPSF ) l = EPSF;
    v.UnitVec();
    float ang = (float)acos(v[2]);
    if ( _rsMode & VFR_PRIMSET_ROT && (v[0] != 0.f || v[1] != 0.f) )
      glRotatef(CES::Rad2Deg(ang), -v[1], v[0], 0.f);
    if ( _rsMode & VFR_PRIMSET_SCALE )
      glScalef(l, l, l);
  }

  switch ( rt ) {
  case RT_SMOOTH: case RT_NOLIGHT: case RT_FLAT:
    _prim->renderSolid();
    break;
  case RT_WIRE:
    _prim->renderWire();
    break;
  case RT_POINT:
    _prim->renderPoint();
    break;
  }

  glPopMatrix();
}


void vfrPrimSet::renderSolid() {
  if ( ! _prim ) return;

  for ( register int index = 0; index < nVerts; index++ ) {
    if ( _colorMode == AT_PER_VERTEX && index < nColors )
      glColor4fv(_colors[index]);
    if ( index < nNormals )
      callPrimRender(RT_SMOOTH, _verts[index], _normals[index]);
    else
      callPrimRender(RT_SMOOTH, _verts[index]);
  }
}

void vfrPrimSet::renderWire() {
  if ( ! _prim ) return;

  Bool auxMode = useAuxLineColor();

  for ( register int index = 0; index < nVerts; index++ ) {
    if ( ! auxMode && _colorMode == AT_PER_VERTEX && index < nColors )
      glColor4fv(_colors[index]);
    if ( index < nNormals )
      callPrimRender(RT_WIRE, _verts[index], _normals[index]);
    else
      callPrimRender(RT_WIRE, _verts[index]);
  }
}

void vfrPrimSet::renderPoint() {
  Bool auxMode = useAuxPointColor();
  if ( ! auxMode && _colorMode == AT_WHOLE )
    glColor4fv(_colors[0]);

  register int index;

  if ( _prim ) {
    for ( index = 0; index < nVerts; index++ ) {
      if ( ! auxMode && _colorMode == AT_PER_VERTEX && index < nColors )
	glColor4fv(_colors[index]);
      if ( index < nNormals )
	callPrimRender(RT_POINT, _verts[index], _normals[index]);
      else
	callPrimRender(RT_POINT, _verts[index]);
    }
  }
  else {
    glBegin(GL_POINTS);
    for ( index = 0; index < nVerts; index++ ) {
      if ( ! auxMode && _colorMode == AT_PER_VERTEX && index < nColors )
        glColor4fv(_colors[index]);
      glVertex3fv(_verts[index]);
    }
    glEnd();
  }
}
