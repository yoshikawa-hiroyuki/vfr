//
// vfrScene
//   Scene(Grouping) class
//
#include "vfrScene.h"

vfrScene::vfrScene(const std::string& nm, const Bool ssm)
  : vfrGroup(nm, ssm), nLight(VFR_LIGHT_MAX), _light(NULL)
{
  _light = (vfrLight**)CeAllocate(nLight, sizeof(vfrLight*));
  for ( register int i = 0; i < nLight; i++ ) {
    char lname[8];
    _light[i] = new vfrLight(i);
    if ( ! _light[i] ) continue;
    _light[i]->addRef(this);
    sprintf(lname, "light%1d", i);
    _light[i]->setName(lname);
    _light[i]->setLightNum(i);
  }
  _light[0]->setOnOff(TRUE);

  this->alcMaterial();
}

vfrScene::~vfrScene() {
  register int i;
  for ( i = 0; i < nLight; i++ )
    delete _light[i];
  DeAllocate(_light);
  nLight = 0;
}

void
vfrScene::rumor(vfrBase* bp) {
  for ( register int i = 0; i < nLight; i++ ) {
    if ( _light[i] == bp ) {
      for ( register int j = i; j < nLight-1; j++ )
        _light[j] = _light[j+1];
      _light[nLight-1] = NULL;
      nLight--;
      notice();
      return;
    }
  }
  vfrGroup::rumor(bp);
}


void
vfrScene::render(vfrMaterialStack*) {
  glPushName((GLuint)0);

  // Apply local coodinate
  applyMatrix();

  // Traverse Material
  vfrMaterialStack mstk;
  if ( _material ) {
    mstk.push(_material);
    _material->apply();
  }

  register int c;
  // Traverse Lights
  for ( c = 0; c < nLight; c++ )
    _light[c]->render();

  // Traverse Non-Transparent Children
  for ( c = 0; c < nChild; c++ )
    _children[c]->render(FALSE, &mstk);

  // Traverse Transparent Children
  glEnable(GL_BLEND);
  glDepthMask(GL_FALSE);
#if 1
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#else // sum-blending
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);
#endif
  for ( c = 0; c < nChild; c++ )
    _children[c]->render(TRUE, &mstk);
  glDepthMask(GL_TRUE);
  glDisable(GL_BLEND);

  if ( _material ) {
    _material->unApply();
    mstk.empty();
  }

  glPopName();

  unApplyMatrix();
}

void
vfrScene::renderBbox() {
  register int c;

  glPushName((GLuint)_currentSeq);

  // Apply local coodinate
  applyMatrix();

  // Traverse Lights
  for ( c = 0; c < nLight; c++ )
    _light[c]->renderBbox();

  // Traverse Children
  for ( c = 0; c < nChild; c++ )
    _children[c]->renderBbox();

  glPopName();
  unApplyMatrix();
}

vfrNode*
vfrScene::getNode(const unsigned int seq) {
  if ( seq == _currentSeq )
    return this;
  register int c;
  for ( c = 0; c < nChild; c++ ) {
    vfrNode* a = _children[c]->getNode(seq);
    if ( a )
      return a;
  }
  for ( c = 0; c < nLight; c++ ) {
    vfrNode* a = _light[c]->getNode(seq);
    if ( a )
      return a;
  }
  return NULL;
}

vfrNode*
vfrScene::getNode(const std::string& s) {
  if ( _name == s )
    return NULL;
  register int c;
  for ( c = 0; c < nChild; c++ ) {
    vfrNode* a = _children[c]->getNode(s);
    if ( a )
      return a;
  }
  for ( c = 0; c < nLight; c++ ) {
    vfrNode* a = _light[c]->getNode(s);
    if ( a )
      return a;
  }
  return NULL;
}

Bool
vfrScene::accumMatrix(const unsigned int tid, vfrMatrix& M) const {
  if ( tid == _currentSeq ) {
    M = M * _matrix;
    return TRUE;
  }
  register int c;
  for ( c = 0; c < nChild; c++ )
    if ( _children[c]->getNode(tid) ) {
      M = M * _matrix;
      return _children[c]->accumMatrix(tid, M);
    }
  for ( c = 0; c < nLight; c++ )
    if ( _light[c]->getNode(tid) ) {
      M = M * _matrix;
      return _light[c]->accumMatrix(tid, M);
    }
  return FALSE;
}

void
vfrScene::clearDispList() {
  register int c;
  for ( c = 0; c < nChild; c++ )
    _children[c]->clearDispList();
  for ( c = 0; c < nLight; c++ )
    _light[c]->clearDispList();
}

vfrLight*
vfrScene::getLight(const std::string& s) {
  for ( register int c = 0; c < nLight; c++ ) {
    vfrLight* a = (vfrLight*)_light[c]->getNode(s);
    if ( a )
      return a;
  }
  return NULL;
}

vfrLight*
vfrScene::getLight(const int n) {
  if ( n < 0 || n >= nLight )
    return NULL;
  return _light[n];
}

// EOF
