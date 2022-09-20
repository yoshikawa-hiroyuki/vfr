//
// vfrScreen
//   Viewport Screen Class
//
#include "vfrScreen.h"
#include "vfrCamera.h"
#include "vfrDrawArea.h"


vfrScreen::vfrScreen() : _camera(NULL),_parent(NULL) {
  initialize();
}
vfrScreen::vfrScreen(const int x, const int y, const int w, const int h)
 : _camera(NULL),_parent(NULL) {
    initialize();
    setPort(x, y, w, h);
}

vfrScreen::~vfrScreen() {
  if ( _parent )
    _parent->rumor(this);
  if ( _camera )
    _camera->setScreen(NULL);
}

void
vfrScreen::initialize() {
  _viewport[0] = 0;
  _viewport[1] = 0;
  _viewport[2] = 100;
  _viewport[3] = 100;
  setParent(NULL);
  setCamera(NULL);
  _borderColor[0] = _borderColor[1] = _borderColor[2] = _borderColor[3] = 1.0f;
  _borderWidth = 2.0f;
  _useBorder = FALSE;
}

void
vfrScreen::setParent(vfrDrawArea* p) {
  _parent = p;
}

void
vfrScreen::redraw() {
  if ( ! _camera )
    return;

  glEnable(GL_SCISSOR_TEST);
  glViewport(_viewport[0], _viewport[1], _viewport[2], _viewport[3]);
  glScissor(_viewport[0], _viewport[1], _viewport[2], _viewport[3]);
  if ( _viewport[3] < 1 )
    _camera->redraw();
  else {
    GLdouble asp = (GLdouble)_viewport[2] / (GLdouble)_viewport[3];
    _camera->redraw(asp);
  }
  glDisable(GL_SCISSOR_TEST);

  if ( _useBorder )
    drawBorder();
}

void
vfrScreen::drawBorder() {
  if ( _viewport[2] < 1 || _viewport[3] <1 || _borderWidth < 0.0f )
    return;
  float x1p = _borderWidth / (float)_viewport[2];
  float y1p = _borderWidth / (float)_viewport[3];
    
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  glDisable(GL_DEPTH_TEST);
  glDisable(GL_LIGHTING);
  glDisable(GL_LINE_STIPPLE);
  glLineWidth(_borderWidth);
  glColor4fv(_borderColor);

  glBegin(GL_LINE_LOOP);
  glVertex2f(-1.0f, -1.0f +y1p);
  glVertex2f( 1.0f, -1.0f +y1p);
  glEnd();
  glBegin(GL_LINE_LOOP);
  glVertex2f(-1.0f,  1.0f -y1p);
  glVertex2f( 1.0f,  1.0f -y1p);
  glEnd();
  glBegin(GL_LINE_LOOP);
  glVertex2f(-1.0f +x1p, -1.0f);
  glVertex2f(-1.0f +x1p,  1.0f);
  glEnd();
  glBegin(GL_LINE_LOOP);
  glVertex2f( 1.0f -x1p, -1.0f);
  glVertex2f( 1.0f -x1p,  1.0f);
  glEnd();

  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
}

void
vfrScreen::notice() {
  if ( _parent )
    _parent->notice();
}

void
vfrScreen::chkNotice() {
  if ( _parent )
    _parent->chkNotice();
}

void
vfrScreen::rumor() {
  //setCamera(NULL);  // don't use setCamera()
  _camera = NULL;
  notice();
}

void
vfrScreen::setCamera(vfrCamera* v) {
  if ( _camera )
    _camera->setScreen(NULL);
  _camera = v;
  if ( _camera )
    _camera->setScreen(this);
  notice();
}

Bool
vfrScreen::isOnScreen(const Point2 p) {
  return isOnScreen(p.x, p.y);
}
Bool
vfrScreen::isOnScreen(const int x, const int y) {
  Point2 parentSize;
  if ( _parent ) {
    parentSize = _parent->getSize();
  } else {
    parentSize.x = _viewport[2];
    parentSize.y = _viewport[3];
  }
  int xx = x - _viewport[0];
  int yy = y - ( parentSize.y - _viewport[1] - _viewport[3]);
  if ( 0 <= xx && xx < _viewport[2] &&
       0 <= yy && yy < _viewport[3] )
    return TRUE;
  else
    return FALSE;
}

Bool
vfrScreen::setPort(const int x1, const int y1, const int w1, const int h1) {
  if ( w1 < 1 || h1 < 1 )
    return FALSE;

  Point2 parentSize;
  if ( _parent ) {
    parentSize = _parent->getSize();
  } else {
    parentSize.x = w1;
    parentSize.y = h1;
  }
  _viewport[0] = (GLint)x1;
  _viewport[1] = parentSize.y - ((GLint)y1 + (GLint)h1);
  _viewport[2] = (GLint)w1;
  _viewport[3] = (GLint)h1;

  // for oit
  if ( _camera ) {
    vfrScene* p_scene = _camera->getScene();
    if ( p_scene ) p_scene->setPort(_viewport);
  }

  notice();
  return TRUE;
}
Bool
vfrScreen::setPort(const Point2 p0, const Point2 size) {
  return setPort(p0.x, p0.y, size.x, size.y);
}

void
vfrScreen::getPort(GLint* port) const {
  if ( port != NULL ) {
    port[0] = _viewport[0];
    port[1] = _viewport[1];
    port[2] = _viewport[2];
    port[3] = _viewport[3];
  }
}

Bool
vfrScreen::setPosition(const int x1, const int y1) {
  Point2 parentSize;
  if ( _parent ) {
    parentSize = _parent->getSize();
  } else {
    parentSize.x = _viewport[2];
    parentSize.y = _viewport[3];
  }
  _viewport[0] = (GLsizei)x1;
  _viewport[1] = parentSize.y - ((GLsizei)y1 + _viewport[3]);
  notice();
  return TRUE;
}
Bool
vfrScreen::setPosition(const Point2 pos) {
  return setPosition(pos.x, pos.y);
}

Point2
vfrScreen::getPosition() const {
  Point2 p, parentSize;
  if ( _parent ) {
    parentSize = _parent->getSize();
  } else {
    parentSize.x = _viewport[2];
    parentSize.y = _viewport[3];
  }
  p.x = _viewport[0];
  p.y = parentSize.y - (_viewport[1] + _viewport[3]);
  return p;
}

Bool
vfrScreen::setSize(const int w1, const int h1) {
  if ( w1 < 1 || h1 < 1 )
    return FALSE;
  _viewport[2] = (GLsizei)w1;
  _viewport[3] = (GLsizei)h1;
  notice();
  return TRUE;
}
Bool
vfrScreen::setSize(const Point2 size) {
  return setSize(size.x, size.y);
}

Point2
vfrScreen::getSize() const {
  Point2 cp;
  cp.x = _viewport[2];
  cp.y = _viewport[3];
  return cp;
}

Point2
vfrScreen::getRelativePoint(const Point2 org) const {
  Point2 parentSize;
  if ( _parent ) {
    parentSize = _parent->getSize();
  } else {
    parentSize.x = _viewport[2];
    parentSize.y = _viewport[3];
  }
  Point2 rp;
  rp.x = org.x - _viewport[0];
  rp.y = org.y - (parentSize.y - (_viewport[1] + _viewport[3]));
  return rp;
}

unsigned int*
vfrScreen::clickSelect(const int x, const int y) {
  static unsigned int failSafe[1] = {0};
  if ( _camera )
    return _camera->clickSelect(x, y);
  else
    return failSafe;
}

unsigned int*
vfrScreen::sweepSelect(const int x, const int y, const int w, const int h) {
  static unsigned int failSafe[1] = {0};
  if ( _camera )
    return _camera->sweepSelect(x, y, w, h);
  else
    return failSafe;
}

int*
vfrScreen::clickFeedback(const int cx, const int cy, const unsigned int target) {
  static int failSafe[1] = {0};
  if ( _camera )
    return _camera->clickFeedback(cx, cy, target);
  else
    return failSafe;
}

int*
vfrScreen::sweepFeedback(const int x, const int y, const int w, const int h,
                        const unsigned int target) {
  static int failSafe[1] = {0};
  if ( _camera )
    return _camera->sweepFeedback(x, y, w, h, target);
  else
    return failSafe;
}

vfrNode*
vfrScreen::getNode(const unsigned int s) {
  if ( ! _camera )
    return NULL;
  return _camera->getNode(s);
}
vfrNode*
vfrScreen::getNode(const std::string& s) {
  if ( ! _camera )
    return NULL;
  return _camera->getNode(s);
}

Bool
vfrScreen::getWinCoord(const unsigned int tid, const vector3 tpos,
                      Point2& winCoord) const {
  /*
   * return winCoord as Window Coordinates
   */
  if ( ! _camera ) return FALSE;
  double asp = (double)_viewport[2]/(double)_viewport[3];
  vfrMatrix pM(_camera->getProjMatrix(asp));
  vfrMatrix mM;
  if ( ! _camera->accumMatrix(tid, mM) )
    return FALSE;

  GLdouble projMat[16], modelMat[16];
  for ( register int i = 0; i < 16; i++) {
    projMat[i] = (GLdouble)pM[i];
    modelMat[i] = (GLdouble)mM[i];
  }
  GLdouble objx = (GLdouble)tpos[0];
  GLdouble objy = (GLdouble)tpos[1];
  GLdouble objz = (GLdouble)tpos[2];
  GLdouble winx, winy, winz;
  if ( gluProject(objx, objy, objz,
                  modelMat, projMat, _viewport,
                  &winx, &winy, &winz) == GL_FALSE )
    return FALSE;

  winCoord.x = (int)winx;
  winCoord.y = (int)winy;
  return TRUE;
}

Bool
vfrScreen::getObjCoord(const unsigned int tid, const Point2 tpos,
                      vector3& objCoord) const {
  if ( ! _camera ) return FALSE;
  double asp = (double)_viewport[2]/(double)_viewport[3];
  vfrMatrix pM(_camera->getProjMatrix(asp));
  vfrMatrix mM;
  if ( ! _camera->accumMatrix(tid, mM) )
    return FALSE;

  GLdouble projMat[16], modelMat[16];
  for ( register int i = 0; i < 16; i++) {
    projMat[i] = (GLdouble)pM[i];
    modelMat[i] = (GLdouble)mM[i];
  }

  Point2 spos = getRelativePoint(tpos);
  GLdouble winx = (GLdouble)spos.x;
  GLdouble winy = (GLdouble)_viewport[3] - (GLdouble)spos.y;
  GLdouble winz = _camera->getFocusDepth();

  GLdouble wcx, wcy, wcz;
  if ( gluProject(0.0, 0.0, 0.0,
                  modelMat, projMat, _viewport,
                  &wcx, &wcy, &wcz) == GL_TRUE )
    winz = wcz;

  GLdouble objx, objy, objz;
  if ( gluUnProject(winx, winy, winz,
                    modelMat, projMat, _viewport,
                    &objx, &objy, &objz) == GL_FALSE )
    return FALSE;

  objCoord[0] = (float)objx;
  objCoord[1] = (float)objy;
  objCoord[2] = (float)objz;
  return TRUE;
}

void
vfrScreen::startRubberBox(const Point2 p0) {
  if ( ! isOnScreen(p0) )
    return;
  _rb1.x = _rb0.x = p0.x;
  _rb1.y = _rb0.y = p0.y;
  if ( _parent )
    _parent->drawRubberBox(_rb0, _rb1);
}

void
vfrScreen::drawRubberBox(const Point2 p) {
  if ( ! isOnScreen(p) )
    return;
  _rb1.x = p.x;
  _rb1.y = p.y;
  if ( _parent )
    _parent->drawRubberBox(_rb0, _rb1);
}

void
vfrScreen::clearRubberBox() {
  if ( _parent )
    _parent->drawRubberBox(_rb0, _rb1);
}

void
vfrScreen::clearRubberBox(Point2* p0, Point2* p1) {
  // sets p0, p1 to coordinates of cleared RBox
  //   to use selection/feedback
  //   (0, 0) as upper-left corner of SCREEN
  if ( ! _parent )
    return;
  clearRubberBox();
  if ( p0 ) {
    p0->x = _rb0.x;
    p0->y = _rb0.y;
  }
  if ( p1 ) {
    p1->x = _rb1.x;
    p1->y = _rb1.y;
  }
}

Bool
vfrScreen::rotateNode(const unsigned int tid,
		      const Point2 mp0, const Point2 md) {
  vfrNode* selectedObj = getNode(tid);
  if ( ! selectedObj ) return FALSE;
  Point2 vpSize = getSize();
  GLint l = ((vpSize.x > vpSize.y) ? vpSize.x : vpSize.y) / 2;
  if ( l < 1 ) return FALSE;

  Point2 mp1, mp2;
  mp1 = mp2 = mp0;
  mp1.x += 2;
  mp2.y += 2;
  vector3 objp0, objp1, objp2;

  if ( ! getObjCoord(tid, mp0, objp0) )
    return FALSE;
  if ( ! getObjCoord(tid, mp1, objp1) )
    return FALSE;
  if ( ! getObjCoord(tid, mp2, objp2) )
    return FALSE;

  vector3 rAx, rAy;
  float rx, ry;
  rAx[0] = objp0[0] - objp2[0];
  rAx[1] = objp0[1] - objp2[1];
  rAx[2] = objp0[2] - objp2[2];
  rx = md.x * HALF_PI / l;
  rAy[0] = objp1[0] - objp0[0];
  rAy[1] = objp1[1] - objp0[1];
  rAy[2] = objp1[2] - objp0[2];
  ry = md.y * HALF_PI / l;
  if ( md.x < md.y ) {
    if ( md.y )
      selectedObj->rotation(ry, rAy);
    if ( md.x )
      selectedObj->rotation(rx, rAx);
  } else {
    if ( md.x )
      selectedObj->rotation(rx, rAx);
    if ( md.y )
      selectedObj->rotation(ry, rAy);
  }
  return TRUE;
}

Bool
vfrScreen::translateNode(const unsigned int tid,
			 const Point2 mp2, const Point2 md) {
  vfrNode* selectedObj = getNode(tid);
  if ( ! selectedObj ) return FALSE;

  Point2 mp1;
  mp1.x = mp2.x - md.x;
  mp1.y = mp2.y - md.y;
  vector3 objp1, objp2;
  if ( ! getObjCoord(tid, mp1, objp1) )
    return FALSE;
  if ( ! getObjCoord(tid, mp2, objp2) )
    return FALSE;
  selectedObj->trans(objp2[0]-objp1[0],
                     objp2[1]-objp1[1],
                     objp2[2]-objp1[2]);
  return TRUE;
}

Bool
vfrScreen::scaleNode(const unsigned int tid, const Point2, const Point2 md) {
  vfrNode* selectedObj = getNode(tid);
  if ( ! selectedObj ) return FALSE;

  Point2 vpSize = getSize();
  if ( vpSize.y < 1 ) return FALSE;
  if ( md.y == 0 ) return FALSE;

  float s;
  if ( md.y < 0 ) {
    s = -(float)md.y / ((float)vpSize.y *0.5f) + 1.0f;
  } else {
    s = -0.5f * (float)md.y / ((float)vpSize.y *0.5f) + 1.0f;
  }
  selectedObj->scale(s);
  return TRUE;
}

void
vfrScreen::clearDispList() {
  if ( _camera )
    _camera->clearDispList();
}

void
vfrScreen::setBorderMode(const Bool bm) {
  if ( bm == _useBorder )
    return;
  _useBorder = bm;
  notice();
}

void
vfrScreen::setBorderColor(const vector4 bc) {
  if ( ! bc )
    return;
  _borderColor[0] = VFR_CLAMP(0.0f, 1.0f, bc[0]);
  _borderColor[1] = VFR_CLAMP(0.0f, 1.0f, bc[1]);
  _borderColor[2] = VFR_CLAMP(0.0f, 1.0f, bc[2]);
  _borderColor[3] = VFR_CLAMP(0.0f, 1.0f, bc[3]);
  notice();
}
void
vfrScreen::setBorderColor(const float r, const float g, const float b) {
  vector4 c = {r, g, b, 1.0f};
  setBorderColor(c);
}
void
vfrScreen::setBorderColor(const float r, const float g, const float b,
			  const float a) {
  vector4 c = {r, g, b, a};
  setBorderColor(c);
}
void
vfrScreen::getBorderColor(vector4& bc) const {
  bc[0] = _borderColor[0];
  bc[1] = _borderColor[1];
  bc[2] = _borderColor[2];
  bc[3] = _borderColor[3];
}

void
vfrScreen::setBorderWidth(const float bw) {
  if ( bw < 0.0 )
    return;
  _borderWidth = (GLfloat)bw;
  if ( _useBorder )
    notice();
}

// EOF
