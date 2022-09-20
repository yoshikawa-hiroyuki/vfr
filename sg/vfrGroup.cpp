//
// vfrGroup
//   Object Grouping Class
//
#include "vfrGroup.h"

vfrGroup::vfrGroup(const std::string& nm, const Bool ssm) : vfrNode(nm, ssm) {
  _dlNodeType = FALSE;

  nChild = 0;
  _children = NULL;
}

vfrGroup::~vfrGroup() {
  for ( register int i = 0; i < nChild; i++ )
    _children[i]->remRef(this);
  if ( _children ) {
    DeAllocate(_children);
    _children = NULL;
  }
  nChild = 0;
}

void
vfrGroup::rumor(vfrBase* bp) {
  for ( register int i = 0; i < nChild; i++ ) {
    if ( _children[i] == bp ) {
      for ( register int j = i; j < nChild-1; j++ )
        _children[j] = _children[j+1];
      _children[nChild-1] = NULL;
      nChild--;
      notice();
      return;
    }
  }
  vfrNode::rumor(bp);
}

void
vfrGroup::render(vfrMaterialStack* mstk) {
  render(TRUE, mstk);
  render(FALSE, mstk);
}

void
vfrGroup::render(const Bool transpMode, vfrMaterialStack *mstk) {
  if ( mstk ) {
    if ( _material )
      mstk->push(_material);
  }

  RenderType renderMode = getRenderMode();
  if ( renderMode == RT_NONE ) {
    if ( mstk && _material )
      mstk->pop();
    return;
  }

  if ( _pickable & PT_OBJECT )
    glPushName((GLuint)_currentSeq);

  // Apply local coodinate
  applyMatrix();

  // Apply material
  const vfrMaterial* mp = getMaterial();
  if ( mp )
    mp->apply();

  // Rendering Children
  for ( register int c = 0; c < nChild; c++ ) {
    _children[c]->render(transpMode, mstk);
  }

  // Un-Apply material
  if ( mp )
    mp->unApply();

  // Draw Bounding Box
  if ( _pickable & PT_BBOX || !_pickable )
    ;
  else
    glLoadName((GLuint)0);
  drawBbox();

  if ( _pickable & PT_OBJECT )
    glPopName();

  unApplyMatrix();

  if ( mstk && _material )
    mstk->pop();
}

void
vfrGroup::renderBbox() {
  glPushName((GLuint)_currentSeq);

  // Apply local coodinate
  applyMatrix();

  // Draw Self-Bounding Box
  //vfrNode::renderBbox();

  // Draw Bounding Box of Children
  for ( register int c = 0; c < nChild; c++ )
    _children[c]->renderBbox();

  glPopName();
  unApplyMatrix();
}

void
vfrGroup::renderFeedBack(const unsigned int oid) {
  if ( oid == _currentSeq )
    return;
  if ( getNode(oid) == NULL )
    return;

  applyMatrix();
  for ( register int c = 0; c < nChild; c++ )
    _children[c]->renderFeedBack(oid);
  unApplyMatrix();
}

Bool
vfrGroup::addChild(vfrNode* ptr) {
  if ( ! ptr )
    return FALSE;
  if ( nChild < 0 ) {
    nChild = 0;
    _children = NULL;
  }
  _children = (vfrNode**)ReAllocate(_children,
                 sizeof(vfrNode*)*(nChild +1));
  if ( _children == NULL ) {
    nChild = 0;
    return FALSE;
  }
  _children[nChild] = ptr;
  _children[nChild]->addRef(this);
  nChild++;
  generateBbox();
  notice();
  return TRUE;
}

Bool
vfrGroup::remChild(vfrNode* delPtr) {
  for ( register int i = 0; i < nChild; i++ ) {
    if ( _children[i] == delPtr ) {
      _children[i]->remRef(this);        // Remove reference from child
      for ( register int j = i; j < nChild-1; j++ )
        _children[j] = _children[j+1];
      _children[nChild-1] = NULL;
      nChild--;
      generateBbox();
      notice();
      return TRUE;
    }
  }
  return FALSE;
}

void
vfrGroup::remAllChildren() {
  register int n = nChild;
  for ( register int i = n-1; i >= 0; i-- ) {
    if ( _children[i] ) {
      _children[i]->remRef(this);        // Remove reference from child
      _children[i] = NULL;
    }
  }
  nChild = 0;
  generateBbox();
  notice();
}

vfrNode*
vfrGroup::getChild(const int order) {
  if ( order < 0 || order >= nChild )
    return NULL;
  return _children[order];
}

vfrNode*
vfrGroup::getNode(const unsigned int seq) {
  if ( seq == _currentSeq )
    return this;

  for ( register int c = 0; c < nChild; c++ ) {
    vfrNode* a = _children[c]->getNode(seq);
    if ( a )
      return a;
  }
  return NULL;
}

vfrNode*
vfrGroup::getNode(const std::string& s) {
  if ( s == VFR_NONAME )
    return NULL;

  if ( _name == s )
    return this;

  for ( register int c = 0; c < nChild; c++ ) {
    vfrNode* a = _children[c]->getNode(s);
    if ( a )
      return a;
  }
  return NULL;
}

Bool
vfrGroup::accumMatrix(const unsigned int tid, vfrMatrix& M) const {
  if ( tid == _currentSeq ) {
    M = M * _matrix;
    return TRUE;
  }
  for ( register int i = 0; i < nChild; i++ )
    if ( _children[i]->getNode(tid) ) {
      M = M * _matrix;
      return _children[i]->accumMatrix(tid, M);
    }
  return FALSE;
}

void
vfrGroup::invalidateDispList() {
  for ( register int c = 0; c < nChild; c++ ) {
    _children[c]->invalidateDispList();
  }
}

void
vfrGroup::clearDispList() {
  for ( register int c = 0; c < nChild; c++ ) {
    _children[c]->clearDispList();
  }
}

void
vfrGroup::generateBbox() {
  if ( nChild < 1 )
    return;

  CES::Vec3<float> cbox[2];
  register int j;

  _children[0]->getMatrixBbox(cbox);
  for ( j = 0; j < 3; j++ ) {
    _bbox[0][j] = cbox[0][j];
    _bbox[1][j] = cbox[1][j];
  }

  for ( j = 1; j < nChild; j++ ) {
    _children[j]->getMatrixBbox(cbox);

    if ( _bbox[0][0] > cbox[0][0] )
      _bbox[0][0] = cbox[0][0];
    if ( _bbox[1][0] < cbox[1][0] )
      _bbox[1][0] = cbox[1][0];

    if ( _bbox[0][1] > cbox[0][1] )
      _bbox[0][1] = cbox[0][1];
    if ( _bbox[1][1] < cbox[1][1] )
      _bbox[1][1] = cbox[1][1];

    if ( _bbox[0][2] > cbox[0][2] )
      _bbox[0][2] = cbox[0][2];
    if ( _bbox[1][2] < cbox[1][2] )
      _bbox[1][2] = cbox[1][2];
  }

  checkBbox();
}

void
vfrGroup::notice() {
  generateBbox();
  for ( register int i = 0; i < _nRef; i++ )
    if ( _Ref[i] )
      _Ref[i]->notice();
}

// for vfrPrimSet
void
vfrGroup::renderSolid() {
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  // call renderSolid() of children
  for ( register int c = 0; c < nChild; c++ ) {
    _children[c]->applyMatrix();
    vfrMaterial* pm = _children[c]->getPrivateMaterial();
    if ( pm ) pm->apply();
    RenderType renderMode = _children[c]->getRenderMode();
    if ( renderMode & RT_NOLIGHT ) glDisable(GL_LIGHTING);
    vfrTexture* ptex = _children[c]->getTexture();
    if ( ptex && !(renderMode & RT_NOTEXTURE) ) ptex->apply();

    _children[c]->renderSolid();

    if ( ptex && !(renderMode & RT_NOTEXTURE) ) ptex->unApply();
    if ( renderMode & RT_NOLIGHT ) glEnable(GL_LIGHTING);
    if ( pm ) pm->unApply();
    _children[c]->unApplyMatrix();
  }
}

void
vfrGroup::renderWire() {
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glDisable(GL_LIGHTING);
  vector4 auxc;

  // call renderWire() of children
  for ( register int c = 0; c < nChild; c++ ) {
    _children[c]->applyMatrix();
    vfrMaterial* pm = _children[c]->getPrivateMaterial();
    if ( pm ) pm->apply();
    if ( _children[c]->useAuxLineColor(auxc) ) glColor4fv(auxc);

    _children[c]->renderWire();

    if ( pm ) pm->unApply();
    _children[c]->unApplyMatrix();
  }
  glEnable(GL_LIGHTING);
}

void
vfrGroup::renderPoint() {
  glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
  glDisable(GL_LIGHTING);
  vector4 auxc;

  // call renderPoint() of children
  for ( register int c = 0; c < nChild; c++ ) {
    _children[c]->applyMatrix();
    vfrMaterial* pm = _children[c]->getPrivateMaterial();
    if ( pm ) pm->apply();
    if ( _children[c]->useAuxPointColor(auxc) ) glColor4fv(auxc);

    _children[c]->renderPoint();

    if ( pm ) pm->unApply();
    _children[c]->unApplyMatrix();
  }
  glEnable(GL_LIGHTING);
}

// EOF
