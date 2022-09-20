//
// vfrNode
// 	Common Node Base class
//
#include "vfrNode.h"

// static members
#ifdef _MPK
Bool vfrNode::_useDispList = FALSE;
#else
Bool vfrNode::_useDispList = TRUE;
#endif // _MPK
unsigned int vfrNode::_sequence = 1;

// bit patterns for point symbol
const GLubyte symbolBits[][8] = {
  { 0x18, 0x18, 0x18, 0xff, 0xff, 0x18, 0x18, 0x18 }, // Plus
  { 0x81, 0x42, 0x24, 0x18, 0x18, 0x24, 0x42, 0x81 }, // Cross
  { 0x3c, 0x42, 0x81, 0x81, 0x81, 0x81, 0x42, 0x3c }, // Circle
  { 0x3c, 0x7e, 0xff, 0xff, 0xff, 0xff, 0x7e, 0x3c }, // Circle(Filled)
  { 0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff }, // Square
  { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff }, // Square(Filled)
  { 0xff, 0x81, 0x42, 0x42, 0x24, 0x24, 0x18, 0x18 }, // Triangle
  { 0xff, 0xff, 0x7e, 0x7e, 0x3c, 0x3c, 0x18, 0x18 }  // Triangle(Filled)
};

#ifndef _MPK
// current material pointer
static vfrMaterial* pCurrentMate = NULL;
#endif // _MPK


vfrNode::vfrNode(const std::string& nm, const Bool ssm)
  : vfrBase(ssm),
    _verts(NULL), _normals(NULL), _indices(NULL), _colors(NULL),
    _transparent(FALSE), _material(NULL), _texture(NULL),
    _dlUpdated(DLF_NONE), _dlNodeType(TRUE)
{
  nPoolVerts = 0;
  nPoolNormals = 0;
  nPoolIndices = 0;
  nPoolColors = 0;

  reset();
  vector4 cv = {0.9f, 0.9f, 0.9f, 1.0f};
  setColors4(1, &cv);
  _name = nm;

  _currentSeq = _sequence++;
}

vfrNode::~vfrNode() {
  reset();
  if ( _verts ) {
    DeAllocate(_verts);
    _verts = NULL;
  }
  if ( _normals ) {
    DeAllocate(_normals);
    _normals = NULL;
  }
  if ( _indices ) {
    DeAllocate(_indices);
    _indices = NULL;
  }
  if ( _colors ) {
    DeAllocate(_colors);
    _colors = NULL;
  }
}

void
vfrNode::reset() {
  nVerts = 0;
  nIndices = 0;
  nNormals = 0;
  nColors = 0;
  if ( _material ) {
    delete _material;
    _material = NULL;
  }
  if ( _texture ) {
    _texture->remRef(this);
    _texture = NULL;
  }
  _bbox[0] = CES::Vec3<float>();
  _bbox[1] = CES::Vec3<float>();
  setNormalMode(AT_WHOLE);
  setColorMode(AT_WHOLE);
  setFeedbackMode(FB_VERTEX);
  setBboxShowMode(FALSE);
  setBboxColor(0.6f, 0.6f, 0.6f, 1.0f);
  setBboxWidth(3.0f);
  setPickMode(PT_NONE); // default: non-pickable

  clearDispList();
}

void
vfrNode::setName(const std::string& s) {
  _name = s;
}

void
vfrNode::setTransparency(const Bool f) {
  if ( f == _transparent )
    return;
  _transparent = f;
  notice();
}

void
vfrNode::setAlpha(const float alpha) {
  for ( int i = 0; i < nColors; i++ )
    _colors[i][3] = alpha;
  notice();
}

void
vfrNode::setAlpha(const Bool use, const float alpha) {
  setAlpha(alpha);
  setTransparency(use);
}

vfrMaterial*
vfrNode::alcMaterial() {
  if ( _material ) {
    _material->reset();
    return _material;
  }
  _material = new vfrMaterial();
  if ( ! _material )
    return NULL;
  _material->setParent(this);
  return _material;
}
vfrMaterial*
vfrNode::alcMaterial(const vfrMaterial& mo) {
  if ( _material )
    delete _material;
  _material = new vfrMaterial(mo);
  if ( ! _material )
    return NULL;
  _material->setParent(this);
  return _material;
}

void
vfrNode::setPrivateMaterial(vfrMaterial* pm) {
  if ( _material == pm ) return;
  if ( _material ) delete _material;
  _material = pm;
  if ( ! _material ) return;
  _material->setParent(this);
}

Bool
vfrNode::alcPools(const int nV, const int nN, const int nI, const int nC) {
  // Pool verts area
  if ( nPoolVerts < nV ) {
    _verts = (vector3*)ReAllocate(_verts, sizeof(vector3)*nV);
    if ( _verts == NULL ) {
      nPoolVerts = nVerts = 0;
      return FALSE;
    }
    nPoolVerts = nV;
  }

  // Pool normals area
  if ( nPoolNormals < nN ) {
    _normals = (vector3*)ReAllocate(_normals, sizeof(vector3)*nN);
    if ( _normals == NULL ) {
      nPoolNormals = nNormals = 0;
      return FALSE;
    }
    nPoolNormals = nN;
  }

  // Pool indices area
  if ( nPoolIndices < nI ) {
    _indices = (int*)ReAllocate(_indices, sizeof(int)*nI);
    if ( _indices == NULL ) {
      nPoolIndices = nIndices = 0;
      return FALSE;
    }
    nPoolIndices = nI;
  }

  // Pool colors area
  if ( nPoolColors < nC ) {
    _colors = (vector4*)ReAllocate(_colors, sizeof(vector4)*nC);
    if ( _colors == NULL ) {
      nPoolColors = nColors = 0;
      return FALSE;
    }
    nPoolColors = nC;
  }

  return TRUE;
}

Bool
vfrNode::alcVerts(const int nv) {
  if ( nVerts < nv ) {
    if ( ! alcPools(nv, -1, -1, -1) )
      return FALSE;
  }
  for ( register int i = nVerts; i < nv; i++ ) {
    _verts[i][0] = 0.0f;
    _verts[i][1] = 0.0f;
    _verts[i][2] = 0.0f;
  }
  nVerts = nv;

  //notice();
  return TRUE;
}

Bool
vfrNode::setVerts(const int nv, const vector3* vv) {
  if ( nVerts < nv ) {
    if ( ! alcPools(nv, -1, -1, -1) )
      return FALSE;
  }
  for ( register int i = 0; i < nv; i++ ) {
    _verts[i][0] = vv[i][0];
    _verts[i][1] = vv[i][1];
    _verts[i][2] = vv[i][2];
  }
  nVerts = nv;

  generateBbox();
  notice();
  return TRUE;
}

Bool
vfrNode::setVert(const int n, const vector3 vv, const Bool updBbox) {
  if ( n >= nVerts )
    return FALSE;
  _verts[n][0] = vv[0];
  _verts[n][1] = vv[1];
  _verts[n][2] = vv[2];
  if ( updBbox )
    generateBbox(vv);
  notice();
  return TRUE;
}

Bool
vfrNode::alcNormals(const int nn) {
  if ( nNormals < nn ) {
    if ( ! alcPools(-1, nn, -1, -1) )
      return FALSE;
  }
  for ( register int i = nNormals; i < nn; i++ ) {
    _normals[i][0] = 0.0f;
    _normals[i][1] = 0.0f;
    _normals[i][2] = 1.0f;
  }
  nNormals = nn;

  //notice();
  return TRUE;
}  

Bool
vfrNode::setNormals(const int nn, const vector3* nv) {
  if ( nNormals < nn ) {
    if ( ! alcPools(-1, nn, -1, -1) )
      return FALSE;
  }
  for ( register int i = 0; i < nn; i++ ) {
    _normals[i][0] = nv[i][0];
    _normals[i][1] = nv[i][1];
    _normals[i][2] = nv[i][2];
  }
  nNormals = nn;

  notice();
  return TRUE;
}

Bool
vfrNode::setNormal(const int n, const vector3 nv) {
  if ( n >= nNormals )
    return FALSE;
  _normals[n][0] = nv[0];
  _normals[n][1] = nv[1];
  _normals[n][2] = nv[2];
  notice();
  return TRUE;
}

Bool
vfrNode::alcIndices(const int n) {
  if ( nIndices < n ) {
    if ( ! alcPools(-1, -1, n, -1) )
      return FALSE;
  }
  for ( register int i = nIndices; i < n; i++ )
    _indices[i] = -1;
  nIndices = n;

  //notice();
  return TRUE;
}

Bool
vfrNode::setIndices(const int n, const int* ind) {
  if ( nIndices < n ) {
    if ( ! alcPools(-1, -1, n, -1) )
      return FALSE;
  }
  for ( register int i = 0; i < n; i++ )
    _indices[i] = ind[i];
  nIndices = n;

  notice();
  return TRUE;
}

Bool
vfrNode::setIndice(const int n, const int ind) {
  if ( n >= nIndices )
    return FALSE;
  _indices[n] = ind;
  notice();
  return TRUE;
}

Bool
vfrNode::alcColors(const int nc) {
  if ( nColors < nc ) {
    if ( ! alcPools(-1, -1, -1, nc) )
      return FALSE;
  }
  for ( register int i = nColors; i < nc; i++ ) {
    _colors[i][0] = 0.9f;
    _colors[i][1] = 0.9f;
    _colors[i][2] = 0.9f;
    _colors[i][3] = 1.0f;
  }
  nColors = nc;

  //notice();
  return TRUE;
}

Bool
vfrNode::setColors3(const int nc, const vector3 *cv) {
  if ( nColors < nc ) {
    if ( ! alcPools(-1, -1, -1, nc) )
      return FALSE;
  }
  for ( register int i = 0; i < nc; i++ ) {
    _colors[i][0] = VFR_CLAMP(0.0f, 1.0f, cv[i][0]);
    _colors[i][1] = VFR_CLAMP(0.0f, 1.0f, cv[i][1]);
    _colors[i][2] = VFR_CLAMP(0.0f, 1.0f, cv[i][2]);
    _colors[i][3] = 1.0f;
  }
  nColors = nc;

  notice();
  return TRUE;
}

Bool
vfrNode::setColors4(const int nc, const vector4 *cv) {
  if ( nColors < nc ) {
    if ( ! alcPools(-1, -1, -1, nc) )
      return FALSE;
  }
  for ( register int i = 0; i < nc; i++ ) {
    _colors[i][0] = VFR_CLAMP(0.0f, 1.0f, cv[i][0]);
    _colors[i][1] = VFR_CLAMP(0.0f, 1.0f, cv[i][1]);
    _colors[i][2] = VFR_CLAMP(0.0f, 1.0f, cv[i][2]);
    _colors[i][3] = VFR_CLAMP(0.0f, 1.0f, cv[i][3]);
  }
  nColors = nc;

  notice();
  return TRUE;
}

Bool
vfrNode::setColor3(const int n, const vector3 cv) {
  if ( n >= nColors )
    return FALSE;
  _colors[n][0] = VFR_CLAMP(0.0f, 1.0f, cv[0]);
  _colors[n][1] = VFR_CLAMP(0.0f, 1.0f, cv[1]);
  _colors[n][2] = VFR_CLAMP(0.0f, 1.0f, cv[2]);
  notice();
  return TRUE;
}

Bool
vfrNode::setColor3(const int n,
    const float r, const float g, const float b) {
  if ( n >= nColors )
    return FALSE;
  _colors[n][0] = VFR_CLAMP(0.0f, 1.0f, r);
  _colors[n][1] = VFR_CLAMP(0.0f, 1.0f, g);
  _colors[n][2] = VFR_CLAMP(0.0f, 1.0f, b);
  notice();
  return TRUE;
}

Bool
vfrNode::setColor4(const int n, const vector4 cv) {
  if ( n >= nColors )
    return FALSE;
  _colors[n][0] = VFR_CLAMP(0.0f, 1.0f, cv[0]);
  _colors[n][1] = VFR_CLAMP(0.0f, 1.0f, cv[1]);
  _colors[n][2] = VFR_CLAMP(0.0f, 1.0f, cv[2]);
  _colors[n][3] = VFR_CLAMP(0.0f, 1.0f, cv[3]);
  notice();
  return TRUE;
}

Bool
vfrNode::setColor4(const int n,
    const float r, const float g, const float b, const float a) {
  if ( n >= nColors )
    return FALSE;
  _colors[n][0] = VFR_CLAMP(0.0f, 1.0f, r);
  _colors[n][1] = VFR_CLAMP(0.0f, 1.0f, g);
  _colors[n][2] = VFR_CLAMP(0.0f, 1.0f, b);
  _colors[n][3] = VFR_CLAMP(0.0f, 1.0f, a);
  notice();
  return TRUE;
}

vfrNode*
vfrNode::getNode(const unsigned int seq) {
  if ( seq == _currentSeq )
    return this;
  else
    return NULL;
}

vfrNode*
vfrNode::getNode(const std::string& s) {
  if ( _name == VFR_NONAME )
    return NULL;
  if ( _name == s )
    return this;
  else
    return NULL;
}

Bool
vfrNode::accumMatrix(const unsigned int tid, vfrMatrix& M) const {
  if ( tid != _currentSeq )
    return FALSE;
  M = M * _matrix;
  return TRUE;
}

void
vfrNode::clearDispList() {
  if ( _dlNodeType ) {
    _dlPoint.DelList();
    _dlWire.DelList();
    _dlSolid.DelList();
    _dlUpdated = DLF_NONE;
  }

  if ( _texture )
    _texture->clearDispList();
}

const vfrMaterial*
vfrNode::getMaterial() const {
#ifndef _MPK
  if ( pCurrentMate )
    return pCurrentMate;
#endif // _MPK
  if ( _material )
    return _material;
  return NULL;
}

GLenum
vfrNode::getFaceMode() const {
#ifndef _MPK
  if ( pCurrentMate )
    return pCurrentMate->getFaceMode();
#endif // _MPK
  if ( _material )
    return _material->getFaceMode();
  return GL_FRONT_AND_BACK;
}

RenderType
vfrNode::getRenderMode() const {
#ifndef _MPK
  if ( pCurrentMate )
    return pCurrentMate->getRenderMode();
#endif // _MPK
  if ( _material )
    return _material->getRenderMode();
  return RT_SMOOTH;
}

Bool
vfrNode::useAuxPointColor(vector4& auxc) const {
#ifndef _MPK
  if ( pCurrentMate )
    return pCurrentMate->useAuxPointColor(auxc);
#endif // _MPK
  if ( _material )
    return _material->useAuxPointColor(auxc);
  return FALSE;
}
Bool
vfrNode::useAuxPointColor() const {
  vector4 dum;
  return useAuxPointColor(dum);
}

Bool
vfrNode::useAuxLineColor(vector4& auxc) const {
#ifndef _MPK
  if ( pCurrentMate )
    return pCurrentMate->useAuxLineColor(auxc);
#endif // _MPK
  if ( _material )
    return _material->useAuxLineColor(auxc);
  return FALSE;
}
Bool
vfrNode::useAuxLineColor() const {
  vector4 dum;
  return useAuxLineColor(dum);
}

void
vfrNode::setTexture(vfrTexture* tex) {
  if ( _texture == tex )
    return;
  if ( _texture )
    _texture->remRef(this);
  _texture = tex;
  if ( _texture )
    _texture->addRef(this);
}

void
vfrNode::setHpr(const float h, const float p, const float r) {
  // set M to R * P * H
  vector3 tv = {_matrix[12], _matrix[13], _matrix[14]};
  _matrix.Identity();
  _matrix.RotY(h);
  _matrix.RotX(p);
  _matrix.RotZ(r);
  _matrix[12] = tv[0];
  _matrix[13] = tv[1];
  _matrix[14] = tv[2];
  _matrix[15] = 1.0f;
  vfrBase::notice();
}

void
vfrNode::render(const Bool transpMode, vfrMaterialStack* mstk) {
  if ( transpMode == TRUE ) {
    // Rendering Transparent Node
    if ( isTransparency() )
      render(mstk);
    return;
  } else {
    // Rendering Non-Transparent Node
    if ( ! isTransparency() )
      render(mstk);
    return;
  }
}

void
vfrNode::render(vfrMaterialStack* mstk) {
  // push material stack
  if ( mstk ) {
    if ( _material )
      mstk->push(_material);
#ifndef _MPK
    pCurrentMate = mstk->top();
#endif // _MPK
  }

  // push name for selection
  if ( _pickable & PT_OBJECT )
    glPushName((GLuint)_currentSeq);

  // transformation matrix
  applyMatrix();

  // apply private material
  if ( _material )
    _material->apply();

  RenderType renderMode;
#ifdef _MPK
  if ( mstk ) {
    vfrMaterial* cmat = mstk->top();
    renderMode = cmat->getRenderMode();
    if ( renderMode != RT_NONE ) {
      RenderType localRM = getRenderMode();
      if ( localRM == RT_POINT || localRM == RT_WIRE )
        renderMode = localRM;
    }
  } else
#endif // _MPK
    renderMode = getRenderMode();

  // rendering
  vector4 auxc;
  if ( renderMode & (RT_SMOOTH | RT_NOLIGHT | RT_FLAT) ) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    if ( renderMode & RT_NOLIGHT )
      glDisable(GL_LIGHTING);
    if ( _texture && !(renderMode & RT_NOTEXTURE) )
      _texture->apply();
    if ( _colorMode == AT_WHOLE )
      glColor4fv(_colors[0]);

    renderSolid();  // <------------ Solid Rendering

    if ( _texture && !(renderMode & RT_NOTEXTURE) )
      _texture->unApply();
    if ( renderMode & RT_NOLIGHT )
      glEnable(GL_LIGHTING);
  }
  if ( renderMode & RT_WIRE ) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDisable(GL_LIGHTING);
    if ( useAuxLineColor(auxc) )
      glColor4fv(auxc);
    else if ( _colorMode == AT_WHOLE )
      glColor4fv(_colors[0]);

    renderWire();  // <------------ Wire Rendering

    glEnable(GL_LIGHTING);
  }
  if ( renderMode & RT_POINT ) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    glDisable(GL_LIGHTING);
    if ( useAuxPointColor(auxc) )
      glColor4fv(auxc);
    else if ( _colorMode == AT_WHOLE )
      glColor4fv(_colors[0]);

    renderPoint();  // <------------ Point Rendering

    glEnable(GL_LIGHTING);
  }

  // pop material
  if ( _material )
    _material->unApply();

  // Bbox rendering
  if ( _pickable & PT_BBOX || !_pickable )
    ;
  else
    glLoadName((GLuint)0);
  drawBbox();

  // pop name for selection
  if ( _pickable & PT_OBJECT )
    glPopName();

  // pop transformation matrix
  unApplyMatrix();

  // pop material stack
  if ( mstk && _material )
    mstk->pop();

#ifndef _MPK
  pCurrentMate = NULL;
#endif // _MPK
}

void
vfrNode::renderFeedBack(const unsigned int oid) {
  if ( _currentSeq != oid )
    return;
  applyMatrix();
  for ( register int i = 0; i < nVerts; i++ ) {
    glPassThrough((GLfloat)i);
    glBegin(GL_POINTS);
    glVertex3fv(_verts[i]);
    glEnd();
  }
  unApplyMatrix();
}

GLsizei
vfrNode::getFeedbackSize() const {
  switch ( _feedbackMode ) {
  case FB_FACE:
  case FB_EDGE:
    return (GLsizei)((nIndices + nVerts) * VFR_FEEDBACK_SIZE_FACTOR);
  case FB_VERTEX:
  default:
    return (GLsizei)(nVerts * VFR_FEEDBACK_SIZE_FACTOR);
  }
}

void
vfrNode::getMatrixBbox(CES::Vec3<float>* box) const {
  CES::Vec3<float> local[8], out;

  local[0][0]=_bbox[1][0]; local[0][1]=_bbox[0][1]; local[0][2]=_bbox[0][2];
  local[1][0]=_bbox[1][0]; local[1][1]=_bbox[1][1]; local[1][2]=_bbox[0][2];
  local[2][0]=_bbox[1][0]; local[2][1]=_bbox[1][1]; local[2][2]=_bbox[1][2];
  local[3][0]=_bbox[1][0]; local[3][1]=_bbox[0][1]; local[3][2]=_bbox[1][2];
  local[4][0]=_bbox[0][0]; local[4][1]=_bbox[0][1]; local[4][2]=_bbox[0][2];
  local[5][0]=_bbox[0][0]; local[5][1]=_bbox[0][1]; local[5][2]=_bbox[1][2];
  local[6][0]=_bbox[0][0]; local[6][1]=_bbox[1][1]; local[6][2]=_bbox[1][2];
  local[7][0]=_bbox[0][0]; local[7][1]=_bbox[1][1]; local[7][2]=_bbox[0][2];

  out = _matrix * local[0];
  box[0] = out; box[1] = out;

  for ( register int i = 1; i < 8; i++ ) {
    out = _matrix * local[i];

    if ( box[0][0] > out[0] )
      box[0][0] = out[0];
    if ( box[1][0] < out[0] )
      box[1][0] = out[0];

    if ( box[0][1] > out[1] )
      box[0][1] = out[1];
    if ( box[1][1] < out[1] )
      box[1][1] = out[1];

    if ( box[0][2] > out[2] )
      box[0][2] = out[2];
    if ( box[1][2] < out[2] )
      box[1][2] = out[2];
  }
}

void
vfrNode::setBboxShowMode(const Bool m) {
  _showBbox = m;
  notice();
}

void
vfrNode::setBboxColor(const vector4 bc) {
  if ( ! bc )
    return;
  _bboxColor[0] = VFR_CLAMP(0.0f, 1.0f, bc[0]);
  _bboxColor[1] = VFR_CLAMP(0.0f, 1.0f, bc[1]);
  _bboxColor[2] = VFR_CLAMP(0.0f, 1.0f, bc[2]);
  _bboxColor[3] = VFR_CLAMP(0.0f, 1.0f, bc[3]);
  if ( _showBbox )
    notice();
}
void
vfrNode::setBboxColor(const float r, const float g, const float b) {
  vector4 bc;
  bc[0] = r;
  bc[1] = g;
  bc[2] = b;
  bc[3] = 1.0f;
  setBboxColor(bc);
}
void
vfrNode::setBboxColor(const float r, const float g, const float b,
		      const float a) {
  vector4 bc;
  bc[0] = r;
  bc[1] = g;
  bc[2] = b;
  bc[3] = a;
  setBboxColor(bc);
}

void
vfrNode::getBboxColor(vector4 bc) const {
  if ( ! bc )
    return;
  bc[0] = _bboxColor[0];
  bc[1] = _bboxColor[1];
  bc[2] = _bboxColor[2];
  bc[3] = _bboxColor[3];
}

void
vfrNode::setBboxWidth(const float bw) {
  if ( bw < 0.0f )
    return;
  _bboxWidth = bw;
  if ( _showBbox )
    notice();
}

void
vfrNode::setPickMode(const PickType m) {
  _pickable = m;
}

void
vfrNode::setNormalMode(const AppearType n) {
  if ( n == _normalMode )
    return;
  _normalMode = n;
  notice();
}

void
vfrNode::setColorMode(const AppearType c) {
  if ( c == _colorMode )
    return;
  _colorMode = c;
  notice();
}

void
vfrNode::setFeedbackMode(const FeedbackType f) {
  if ( f == _feedbackMode )
    return;
  _feedbackMode = f;
}

void
vfrNode::generateBbox() {
  if ( nVerts < 1 ) {
    _bbox[0] = CES::Vec3<float>();
    _bbox[1] = _bbox[0];
    checkBbox();
    return;
  }

  _bbox[0] = CES::Vec3<float>(_verts[0]);
  _bbox[1] = _bbox[0];
  for ( register int i = 1; i < nVerts; i++ ) {
    if ( _bbox[0].m_v[0] > _verts[i][0] ) _bbox[0].m_v[0] = _verts[i][0];
    if ( _bbox[1].m_v[0] < _verts[i][0] ) _bbox[1].m_v[0] = _verts[i][0];

    if ( _bbox[0].m_v[1] > _verts[i][1] ) _bbox[0].m_v[1] = _verts[i][1];
    if ( _bbox[1].m_v[1] < _verts[i][1] ) _bbox[1].m_v[1] = _verts[i][1];

    if ( _bbox[0].m_v[2] > _verts[i][2] ) _bbox[0].m_v[2] = _verts[i][2];
    if ( _bbox[1].m_v[2] < _verts[i][2] ) _bbox[1].m_v[2] = _verts[i][2];
  }
  checkBbox();
}

void
vfrNode::generateBbox(const CES::Vec3<float>& newVert) {
  if ( nVerts < 1 ) {
    _bbox[0] = newVert;
    _bbox[1] = newVert;
    checkBbox();
    return;
  }
  if ( _bbox[0].m_v[0] > newVert[0] ) _bbox[0].m_v[0] = newVert[0];
  if ( _bbox[1].m_v[0] < newVert[0] ) _bbox[1].m_v[0] = newVert[0];

  if ( _bbox[0].m_v[1] > newVert[1] ) _bbox[0].m_v[1] = newVert[1];
  if ( _bbox[1].m_v[1] < newVert[1] ) _bbox[1].m_v[1] = newVert[1];

  if ( _bbox[0].m_v[2] > newVert[2] ) _bbox[0].m_v[2] = newVert[2];
  if ( _bbox[1].m_v[2] < newVert[2] ) _bbox[1].m_v[2] = newVert[2];

  checkBbox();
}

#define _VFR_THIN_BBOX   5e-6
#define _VFR_THIN_BBOX2  (_VFR_THIN_BBOX*0.5)
void
vfrNode::checkBbox() {
  if ( _bbox[1].m_v[0] < _bbox[0].m_v[0] ) {
    float s = _bbox[1].m_v[0];
    _bbox[1].m_v[0] = _bbox[0].m_v[0];
    _bbox[0].m_v[0] = s;
  }
  if ( _bbox[1].m_v[1] < _bbox[0].m_v[1] ) {
    float s = _bbox[1].m_v[1];
    _bbox[1].m_v[1] = _bbox[0].m_v[1];
    _bbox[0].m_v[1] = s;
  }
  if ( _bbox[1].m_v[2] < _bbox[0].m_v[2] ) {
    float s = _bbox[1].m_v[2];
    _bbox[1].m_v[2] = _bbox[0].m_v[2];
    _bbox[0].m_v[2] = s;
  }
  if ( _bbox[1].m_v[0] - _bbox[0].m_v[0] < _VFR_THIN_BBOX ) {
    _bbox[0].m_v[0] -= _VFR_THIN_BBOX2;
    _bbox[1].m_v[0] += _VFR_THIN_BBOX2;
  }
  if ( _bbox[1].m_v[1] - _bbox[0].m_v[1] < _VFR_THIN_BBOX ) {
    _bbox[0].m_v[1] -= _VFR_THIN_BBOX2;
    _bbox[1].m_v[1] += _VFR_THIN_BBOX2;
  }
  if ( _bbox[1].m_v[2] - _bbox[0].m_v[2] < _VFR_THIN_BBOX ) {
    _bbox[0].m_v[2] -= _VFR_THIN_BBOX2;
    _bbox[1].m_v[2] += _VFR_THIN_BBOX2;
  }
}

void
vfrNode::drawBbox() {
  if ( ! _showBbox )
    return;

  glDisable(GL_LIGHTING);
  glLineWidth(_bboxWidth);
  glColor3fv(_bboxColor);

  glBegin(GL_LINE_LOOP);
  glVertex3f(_bbox[1].m_v[0], _bbox[0].m_v[1], _bbox[0].m_v[2]);
  glVertex3f(_bbox[1].m_v[0], _bbox[1].m_v[1], _bbox[0].m_v[2]);
  glVertex3f(_bbox[1].m_v[0], _bbox[1].m_v[1], _bbox[1].m_v[2]);
  glVertex3f(_bbox[1].m_v[0], _bbox[0].m_v[1], _bbox[1].m_v[2]);
  glEnd();

  glBegin(GL_LINE_LOOP);
  glVertex3f(_bbox[0].m_v[0], _bbox[0].m_v[1], _bbox[0].m_v[2]);
  glVertex3f(_bbox[0].m_v[0], _bbox[0].m_v[1], _bbox[1].m_v[2]);
  glVertex3f(_bbox[0].m_v[0], _bbox[1].m_v[1], _bbox[1].m_v[2]);
  glVertex3f(_bbox[0].m_v[0], _bbox[1].m_v[1], _bbox[0].m_v[2]);
  glEnd();

  glBegin(GL_LINES);
  glVertex3f(_bbox[1].m_v[0], _bbox[0].m_v[1], _bbox[0].m_v[2]);
  glVertex3f(_bbox[0].m_v[0], _bbox[0].m_v[1], _bbox[0].m_v[2]);

  glVertex3f(_bbox[1].m_v[0], _bbox[1].m_v[1], _bbox[0].m_v[2]);
  glVertex3f(_bbox[0].m_v[0], _bbox[1].m_v[1], _bbox[0].m_v[2]);

  glVertex3f(_bbox[0].m_v[0], _bbox[1].m_v[1], _bbox[1].m_v[2]);
  glVertex3f(_bbox[1].m_v[0], _bbox[1].m_v[1], _bbox[1].m_v[2]);

  glVertex3f(_bbox[0].m_v[0], _bbox[0].m_v[1], _bbox[1].m_v[2]);
  glVertex3f(_bbox[1].m_v[0], _bbox[0].m_v[1], _bbox[1].m_v[2]);
  glEnd();

  glLineWidth(1.0f);
  glEnable(GL_LIGHTING);
}

void
vfrNode::renderBbox() {
  applyMatrix();
  glPushName((GLuint)_currentSeq);
  RenderType renderMode = getRenderMode();
  switch ( renderMode ) {
  case RT_NONE:
    break;
  default:
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_POLYGON);
    glVertex3f(_bbox[1].m_v[0], _bbox[0].m_v[1], _bbox[0].m_v[2]);
    glVertex3f(_bbox[1].m_v[0], _bbox[1].m_v[1], _bbox[0].m_v[2]);
    glVertex3f(_bbox[1].m_v[0], _bbox[1].m_v[1], _bbox[1].m_v[2]);
    glVertex3f(_bbox[1].m_v[0], _bbox[0].m_v[1], _bbox[1].m_v[2]);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex3f(_bbox[0].m_v[0], _bbox[0].m_v[1], _bbox[0].m_v[2]);
    glVertex3f(_bbox[0].m_v[0], _bbox[0].m_v[1], _bbox[1].m_v[2]);
    glVertex3f(_bbox[0].m_v[0], _bbox[1].m_v[1], _bbox[1].m_v[2]);
    glVertex3f(_bbox[0].m_v[0], _bbox[1].m_v[1], _bbox[0].m_v[2]);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex3f(_bbox[0].m_v[0], _bbox[1].m_v[1], _bbox[0].m_v[2]);
    glVertex3f(_bbox[0].m_v[0], _bbox[1].m_v[1], _bbox[1].m_v[2]);
    glVertex3f(_bbox[1].m_v[0], _bbox[1].m_v[1], _bbox[1].m_v[2]);
    glVertex3f(_bbox[1].m_v[0], _bbox[1].m_v[1], _bbox[0].m_v[2]);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex3f(_bbox[0].m_v[0], _bbox[0].m_v[1], _bbox[0].m_v[2]);
    glVertex3f(_bbox[1].m_v[0], _bbox[0].m_v[1], _bbox[0].m_v[2]);
    glVertex3f(_bbox[1].m_v[0], _bbox[0].m_v[1], _bbox[1].m_v[2]);
    glVertex3f(_bbox[0].m_v[0], _bbox[0].m_v[1], _bbox[1].m_v[2]);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex3f(_bbox[1].m_v[0], _bbox[1].m_v[1], _bbox[1].m_v[2]);
    glVertex3f(_bbox[0].m_v[0], _bbox[1].m_v[1], _bbox[1].m_v[2]);
    glVertex3f(_bbox[0].m_v[0], _bbox[0].m_v[1], _bbox[1].m_v[2]);
    glVertex3f(_bbox[1].m_v[0], _bbox[0].m_v[1], _bbox[1].m_v[2]);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex3f(_bbox[0].m_v[0], _bbox[0].m_v[1], _bbox[0].m_v[2]);
    glVertex3f(_bbox[0].m_v[0], _bbox[1].m_v[1], _bbox[0].m_v[2]);
    glVertex3f(_bbox[1].m_v[0], _bbox[1].m_v[1], _bbox[0].m_v[2]);
    glVertex3f(_bbox[1].m_v[0], _bbox[0].m_v[1], _bbox[0].m_v[2]);
    glEnd();
  }
  glPopName();
  unApplyMatrix();
}

#define _VFR_PT_WIDTH  8
#define _VFR_PT_HEIGHT 8
#define PT_SYM_NORMAL  -1 // Normal(OpenGL Point)
#define PT_SYM_PLUS     0 // Plus
#define PT_SYM_CROSS    1 // Cross
#define PT_SYM_CIRCLE   2 // Circle
#define PT_SYM_CIRCFILL 3 // Circle(Filled)
#define PT_SYM_SQUARE   4 // Square
#define PT_SYM_SQUAFILL 5 // Square(Filled)
#define PT_SYM_TRIANGLE 6 // Triangle
#define PT_SYM_TRIAFILL 7 // Triangle(Filled)

void
vfrNode::renderPoint() {
  // display-list check
  if ( beginDispList(DLF_POINT) ) return;

  Bool auxMode = useAuxPointColor();
  const vfrMaterial *cmat = getMaterial();
  SymbolType symbol = SYM_NORMAL;
  if ( cmat ) symbol = cmat->getPointSymbol();

  if ( symbol == SYM_NORMAL ) {
    glBegin(GL_POINTS);
    for ( register int i = 0; i < nVerts; i++ ) {
      if ( _colorMode == AT_PER_VERTEX && !auxMode && i < nColors ) {
        glColor4fv(_colors[i]);
      }
      glVertex3fv(_verts[i]);
    } // end of for(i)
    glEnd();
  }
  else {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    for ( register int i = 0; i < nVerts; i++ ) {
      if ( _colorMode == AT_PER_VERTEX && !auxMode && i < nColors ) {
        glColor4fv(_colors[i]);
      }
      glRasterPos3fv(_verts[i]);
      switch ( symbol ) {
      case SYM_PLUS:
        glBitmap(_VFR_PT_WIDTH, _VFR_PT_HEIGHT,
                 _VFR_PT_WIDTH * 0.5f, _VFR_PT_HEIGHT*0.5f,
                 0.0f, 0.0f, symbolBits[PT_SYM_PLUS]);
        break;
      case SYM_CROSS:
        glBitmap(_VFR_PT_WIDTH, _VFR_PT_HEIGHT,
                 _VFR_PT_WIDTH * 0.5f, _VFR_PT_HEIGHT*0.5f,
                 0.0f, 0.0f, symbolBits[PT_SYM_CROSS]);
        break;
      case SYM_CIRCLE:
        glBitmap(_VFR_PT_WIDTH, _VFR_PT_HEIGHT,
                 _VFR_PT_WIDTH * 0.5f, _VFR_PT_HEIGHT*0.5f,
                 0.0f, 0.0f, symbolBits[PT_SYM_CIRCLE]);
        break;
      case SYM_CIRCFILL:
        glBitmap(_VFR_PT_WIDTH, _VFR_PT_HEIGHT,
                 _VFR_PT_WIDTH * 0.5f, _VFR_PT_HEIGHT*0.5f,
                 0.0f, 0.0f, symbolBits[PT_SYM_CIRCFILL]);
        break;
      case SYM_SQUARE:
        glBitmap(_VFR_PT_WIDTH, _VFR_PT_HEIGHT,
                 _VFR_PT_WIDTH * 0.5f, _VFR_PT_HEIGHT*0.5f,
                 0.0f, 0.0f, symbolBits[PT_SYM_SQUARE]);
        break;
      case SYM_SQUAFILL:
        glBitmap(_VFR_PT_WIDTH, _VFR_PT_HEIGHT,
                 _VFR_PT_WIDTH * 0.5f, _VFR_PT_HEIGHT*0.5f,
                 0.0f, 0.0f, symbolBits[PT_SYM_SQUAFILL]);
        break;
      case SYM_TRIANGLE:
        glBitmap(_VFR_PT_WIDTH, _VFR_PT_HEIGHT,
                 _VFR_PT_WIDTH * 0.5f, _VFR_PT_HEIGHT*0.5f,
                 0.0f, 0.0f, symbolBits[PT_SYM_TRIANGLE]);
        break;
      case SYM_TRIAFILL:
        glBitmap(_VFR_PT_WIDTH, _VFR_PT_HEIGHT,
                 _VFR_PT_WIDTH * 0.5f, _VFR_PT_HEIGHT*0.5f,
                 0.0f, 0.0f, symbolBits[PT_SYM_TRIAFILL]);
      }
    } // end of for(i)
  } // end of (symbol != SYM_NORMAL)

  // end display-list definition
  endDispList(DLF_POINT);
}

void
vfrNode::notice() {
  invalidateDispList();
  vfrBase::notice();
}

void
vfrNode::rumor(vfrBase* bp) {
  if ( _texture != bp )
    return;
  _texture = NULL;
  notice();
}

unsigned int
vfrNode::getNodeSequence() {
  return _sequence;
}

void
vfrNode::setImmediateMode(const Bool im) {
  if ( im )
    _useDispList = FALSE;
  else
    _useDispList = TRUE;
}

Bool
vfrNode::isImmediateMode() {
  if ( _useDispList )
    return FALSE;
  else
    return TRUE;
}

// EOF
