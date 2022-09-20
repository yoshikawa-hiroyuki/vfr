//
// vtCamera
//     Basic Camera Object Class
//

#include "vfrCamera.h"
#define FEEDBACK_SIZE_FACTOR    8
#define VIEW_FOG_START    9.0f
#define VIEW_FOG_END     15.0f
#define VIEW_WIDE_HALF    5.0f
#define FGV_NEAR_FAR     50.0f

#ifndef WINDOWS
#define VFR_CAMERA_ERR(x) \
 fprintf(stderr,"vfrCamera: %s\n",(x));fflush(stderr)
#else
#define VFR_CAMERA_ERR(x) \
 MessageBox(NULL, (x), TEXT("vfrCamera error message"), MB_OK)
#endif

//static
Bool vfrCamera::s_fbkSort = TRUE;


vfrCamera::vfrCamera(const Bool ssm)
  : vfrBase(ssm),
    _scene(NULL), _front(NULL), _screen(NULL),
    _selected(NULL), _selectedBbox(NULL), _feedbacked(NULL),
    _antiAlias(TRUE)
{
  init();
}

vfrCamera::~vfrCamera() {
  if ( _scene ) {
    _scene->remRef(this);
    _scene = NULL;
  }
  if ( _front ) {
    _front->remRef(this);
    _front = NULL;
  }
  if ( _screen )
    _screen->rumor();
  init();
}

void
vfrCamera::init() {
  setBgColor(0.0f, 0.0f, 0.0f);

  vfrFrustum tmpf; _frustum = tmpf;

  setFogMode(FALSE, VIEW_FOG_START, VIEW_FOG_END);
 
  _clickSpotSize = VFR_SELECT_SUITE_SPOT;
  relaxSelect();
  relaxFeedback();
  if ( _selectedBbox ) DeAllocate((void*)_selectedBbox);
  _selectedBbox = NULL;

  resetTrans();
}

void
vfrCamera::relaxSelect() {
  if ( _selected ) {
    DeAllocate(_selected);
    _selected = NULL;
  }
}

void
vfrCamera::relaxFeedback() {
  if ( _feedbacked ) {
    DeAllocate(_feedbacked);
    _feedbacked = NULL;
  }
}

void
vfrCamera::setScene(vfrScene* scene1) {
  if ( _scene )
    _scene->remRef(this);
  _scene = scene1;
  if ( _scene )
    _scene->addRef(this);
  notice();
}

void
vfrCamera::setFrontNode(vfrNode* fobj) {
  if ( _front )
    _front->remRef(this);
  _front = fobj;
  if ( _front )
    _front->addRef(this);
  notice();
}

void
vfrCamera::setScreen(vfrScreen* s) {
  if ( _screen == s )
    return;
  if ( _screen )
    _screen->rumor();
  _screen = s;
  notice();
}

void
vfrCamera::setBgColor(const vector4 cv) {
  if ( ! cv )
    return;
  _bgColor[0] = VFR_CLAMP(0.0f, 1.0f, cv[0]);
  _bgColor[1] = VFR_CLAMP(0.0f, 1.0f, cv[1]);
  _bgColor[2] = VFR_CLAMP(0.0f, 1.0f, cv[2]);
  _bgColor[3] = VFR_CLAMP(0.0f, 1.0f, cv[3]);
  notice();
}
void
vfrCamera::setBgColor(const float r, const float g, const float b) {
  vector4 c = {r, g, b, 1.0f};
  setBgColor(c);
}
void
vfrCamera::setBgColor(const float r, const float g, const float b,
		      const float a) {
  vector4 c = {r, g, b, a};
  setBgColor(c);
}

void
vfrCamera::getBgColor(vector4 cv) const {
  if ( cv ) {
    cv[0] = _bgColor[0];
    cv[1] = _bgColor[1];
    cv[2] = _bgColor[2];
    cv[3] = _bgColor[3];
  }
}

void
vfrCamera::setFogMode(const Bool f) {
  if ( _fogMode != f ) {
    _fogMode = f;
    notice();
  }
}
void
vfrCamera::setFogMode(const Bool f, const float s, const float e) {
  setFogMode(f);
  setFogParam(s, e);
}

void
vfrCamera::setFogParam(const float s, const float e) {
  _fogStart = s;
  _fogEnd = e;
  notice();
}

void
vfrCamera::getFogParam(float &s, float &e) const {
  s = _fogStart;
  e = _fogEnd;
}

void
vfrCamera::setClickSpotSize(const int size) {
  if ( size < 1 )
    return;
  _clickSpotSize = size;
}

void
vfrCamera::resetTrans() {
  vfrFrustum tmpf;
  _frustum = tmpf;

  notice();
}

Bool
vfrCamera::accumMatrix(const unsigned int tid, vfrMatrix& M) const {
  if ( ! _scene )
    return FALSE;
  M = M * getModelMatrix();
  return _scene->accumMatrix(tid, M);
}

vfrNode*
vfrCamera::getNode(const unsigned int s) {
  vfrNode* pnode = NULL;
  if ( _scene ) pnode = _scene->getNode(s);
  if ( pnode ) return pnode;
  if ( _front ) pnode = _front->getNode(s);
  if ( pnode ) return pnode;
  return NULL;
}

vfrNode*
vfrCamera::getNode(const std::string& s) {
  vfrNode* pnode = NULL;
  if ( _scene ) pnode = _scene->getNode(s);
  if ( pnode ) return pnode;
  if ( _front ) pnode = _front->getNode(s);
  if ( pnode ) return pnode;
  return NULL;
}

void
vfrCamera::clearDispList() {
  if ( _scene )
    _scene->clearDispList();
  if ( _front )
    _front->clearDispList();
}

void
vfrCamera::bgPaint() {
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  glDisable(GL_DEPTH_TEST);
  glDisable(GL_LIGHTING);
  glPolygonMode(GL_FRONT, GL_FILL);

  glColor4fv(_bgColor);
  glBegin(GL_POLYGON);
  glVertex2f(-1.0f, -1.0f);
  glVertex2f( 1.0f, -1.0f);
  glVertex2f( 1.0f,  1.0f);
  glVertex2f(-1.0f,  1.0f);
  glEnd();

  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
}

void
vfrCamera::fgPaint() {
  if ( ! _front )
    return;

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  Point2 psz = _screen->getSize();
  GLdouble asp;
  if ( psz.y <= 0 )
    asp = 1.0f;
  else
    asp = (GLdouble)psz.x / (GLdouble)psz.y;
  glOrtho(-asp, asp, -1.0f, 1.0f, -FGV_NEAR_FAR, FGV_NEAR_FAR);

  glClear(GL_DEPTH_BUFFER_BIT);
  glEnable(GL_LIGHT0);
  glDisable(GL_LIGHT1);
  glDisable(GL_LIGHT2);
  glDisable(GL_LIGHT3);
  const vector4 positionPara  = {0.0f, 0.0f, 1.0f, 0.0f};
  glLightfv(GL_LIGHT0, GL_POSITION, positionPara);

  // Non-Transparent front-object
  _front->render(FALSE);

  // Transparent front-object
  glDepthMask(GL_FALSE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  _front->render(TRUE);
  glDepthMask(GL_TRUE);
  glDisable(GL_BLEND);

  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
}

void
vfrCamera::redraw() {
  redraw((GLdouble)1.0);
}

void
vfrCamera::redraw(const GLdouble aspect) {
  if ( ! _screen )
    return;

  // Background Painting
  bgPaint();

  // Set OpenGL modes
  glDepthFunc(GL_LEQUAL);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
  glEnable(GL_NORMALIZE);
  glEnable(GL_LINE_STIPPLE);

  // Lighting Model
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

  // AntiAlias Lines
  if ( _antiAlias ) {
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POINT_SMOOTH);
  } else {
    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_POINT_SMOOTH);
  }

  // Fog
  if ( _fogMode ) {
    glEnable(GL_FOG);
    glFogi(GL_FOG_MODE, GL_LINEAR);
    glFogfv(GL_FOG_COLOR, _bgColor);
    glFogf(GL_FOG_START, _fogStart);
    glFogf(GL_FOG_END, _fogEnd);
  } else
    glDisable(GL_FOG);
  glHint(GL_FOG_HINT, GL_FASTEST);

  // Polygon Offset
#if GL_VERSION_1_2
  glPolygonOffset(1.0, 0.000001);
  glEnable(GL_POLYGON_OFFSET_FILL);
#endif /* GL_VERSION_1_2 */

  // Camera Projection
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  projection(aspect);

  // Initialize Model Coordinates
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  _frustum.ApplyModelview();
  //glMultMatrixf(_matrix.value);

  // Scene Rendering
  if ( _scene )
    _scene->render();

  // FrontObj Rendering
  fgPaint();

  // Relax AntiAlias mode
  if ( _antiAlias )
    glDisable(GL_LINE_SMOOTH);

}

unsigned int*
vfrCamera::clickSelect(const int cx, const int cy) {
  relaxSelect();
  return selection(cx, cy, _clickSpotSize, _clickSpotSize);
}

unsigned int*
vfrCamera::sweepSelect(const int x, const int y, const int w, const int h) {
  int ww = (w < _clickSpotSize) ? _clickSpotSize : w;
  int hh = (h < _clickSpotSize) ? _clickSpotSize : h;
  int cx = x + w/2;
  int cy = y + h/2;
  relaxSelect();
  return selection(cx, cy, ww, hh);
}

unsigned int*
vfrCamera::selectionBbox(const int cx, const int cy, const int w, const int h,
                        const GLint* vp) {
  // (cx, cy) : center in window coordinate.
  //        : (0, 0) as upper-left point.

  GLint hits;
  GLint Viewp[4];
  int x, y;
  GLuint* selectBuf = NULL;

  // Relax
  if ( _selectedBbox ) DeAllocate((void*)_selectedBbox);
  _selectedBbox = NULL;

  Viewp[0] = vp[0]; Viewp[1] = vp[1]; Viewp[2] = vp[2]; Viewp[3] = vp[3];
  x = Viewp[0] + cx;
  y = Viewp[1] + Viewp[3] - cy;

  int selectBufSize = (vfrNode::getNodeSequence() % 20000) * 5;
  selectBuf = (GLuint*)Allocate(sizeof(GLuint)*selectBufSize);
  if ( ! selectBuf ) {
    _selectedBbox = (unsigned int*)Allocate(sizeof(unsigned int));
    _selectedBbox[0] = 0;
    return _selectedBbox;
  }
  glSelectBuffer(selectBufSize, selectBuf);
  (void)glRenderMode(GL_SELECT);

  glInitNames();

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPickMatrix((GLdouble)x, (GLdouble)y, (GLdouble)w, (GLdouble)h, Viewp);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  projection((GLdouble)Viewp[2]/(GLdouble)Viewp[3]);
  _frustum.ApplyModelview();
  //glMultMatrixf(_matrix.value);

  if ( _scene )
    _scene->renderBbox();

  if ( _front ) {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    Point2 psz = _screen->getSize();
    GLdouble asp = (psz.y <= 0) ? 1.0 : ((GLdouble)psz.x / (GLdouble)psz.y);
    glOrtho(-asp, asp, -1.0, 1.0, -50.0, 50.0);

    _front->renderBbox();

    glPopMatrix();
  }

#ifdef unix
  glFlush();
#endif

  GLenum error;
  while((error = glGetError()) != GL_NO_ERROR)
#ifndef WINDOWS
    fprintf(stderr, " vfrCamera: selectionBbox: GL error: %s\n",
            gluErrorString(error));
#else
  ;
#endif

  if ( (hits = glRenderMode(GL_RENDER)) < 1 ) {
    _selectedBbox = (unsigned int*)Allocate(sizeof(unsigned int));
    _selectedBbox[0] = 0;
    if ( selectBuf ) DeAllocate(selectBuf);
    return _selectedBbox;
  }
  register int i, j;
  GLuint* ptr;
  _selectedBbox = (unsigned int*)Allocate(sizeof(unsigned int)*(hits +1));
  _selectedBbox[0] = 0;
  ptr = selectBuf;
  for ( i = 0, j = 1; i < hits; i++ ) {
    int names = *ptr++;
    ptr++; ptr++;    // skip depth1, depth2
    for ( int n = 0; n < names -1; n++ )
      ptr++;
    if ( *ptr != 0 ) {
      _selectedBbox[j++] = *ptr;
      _selectedBbox[0]++;
    }
    ptr++;
  }

  if ( selectBuf ) DeAllocate(selectBuf);
  return _selectedBbox;
}


unsigned int*
vfrCamera::selection(const int cx, const int cy, const int w, const int h) {
  // (cx, cy) : center in window coordinate.
  //          : (0, 0) as upper-left point.

  GLint hits;
  GLint Viewp[4];
  int x, y;
  GLuint* selectBuf = NULL;

  if ( _selected ) {
    // Not Relaxed.
    return _selected;
  }

  // Get Viewport of Screen
  vfrScreen *scrn = getScreen();
  if ( scrn == NULL ) {
    _selected = (unsigned int*)Allocate(sizeof(unsigned int));
    _selected[0] = 0;
    return _selected;
  }
  scrn->getPort(Viewp);

  x = Viewp[0] + cx;
  y = Viewp[1] + Viewp[3] - cy;

  int selectBufSize = (vfrNode::getNodeSequence() % 20000) * 5;
  selectBuf = (GLuint*)Allocate(sizeof(GLuint)*selectBufSize);
  if ( ! selectBuf ) {
    _selected = (unsigned int*)Allocate(sizeof(unsigned int));
    _selected[0] = 0;
    return _selected;
  }
  glSelectBuffer(selectBufSize, selectBuf);
  (void)glRenderMode(GL_SELECT);

  glInitNames();

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPickMatrix((GLdouble)x, (GLdouble)y, (GLdouble)w, (GLdouble)h, Viewp);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  projection((GLdouble)Viewp[2]/(GLdouble)Viewp[3]);
  _frustum.ApplyModelview();
  //glMultMatrixf(_matrix.value);

  if ( _scene )
    _scene->render();

  if ( _front ) {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    Point2 psz = _screen->getSize();
    GLdouble asp = (psz.y <= 0) ? 1.0 : ((GLdouble)psz.x / (GLdouble)psz.y);
    glOrtho(-asp, asp, -1.0, 1.0, -50.0, 50.0);

    _front->render(FALSE);
    _front->render(TRUE);

    glPopMatrix();
  }

#ifdef unix
  glFlush();
#endif

  GLenum error;
  while((error = glGetError()) != GL_NO_ERROR)
#ifndef WINDOWS
    fprintf(stderr, " vfrCamera: selection: GL error: %s\n",
            gluErrorString(error));
#else
  ;
#endif

  if ( (hits = glRenderMode(GL_RENDER)) < 1 ) {
    _selected = (unsigned int*)Allocate(sizeof(unsigned int));
    _selected[0] = 0;
    if ( selectBuf ) DeAllocate(selectBuf);
    return _selected;
  }

  register int i, j;
  GLuint* ptr;
  _selected = (unsigned int*)Allocate(sizeof(unsigned int)*(hits +1));
  int sortSize = (hits%2 == 0) ? hits +1 : hits;
  unsigned int *idList
    = (unsigned int*)Allocate(sizeof(unsigned int)*sortSize);
  unsigned int *depthL
    = (unsigned int*)Allocate(sizeof(unsigned int)*sortSize);

  // Sort list according to depth
  ptr = selectBuf;
  for ( i = 0; i < hits; i++ ) {
    int names = *ptr++;
    depthL[i] = *ptr++;    // this is depth1
    ptr++;            // skip depth2
    for ( int n = 0; n < names -1; n++ )
      ptr++;
    idList[i] = *ptr++;
  }
  if ( sortSize != hits ) {
    idList[sortSize-1] = 0;
    depthL[sortSize-1] = (unsigned int)~(0);
  }
  depthSort(depthL, idList, 0, sortSize-1);

  _selected[0] = 0;
  for ( i = 0, j = 1; i < hits; i++ )
    if ( idList[i] != 0 ) {
      _selected[j++] = idList[i];
      _selected[0]++;
    }
  if ( idList ) DeAllocate(idList);
  if ( depthL ) DeAllocate(depthL);

#ifdef _SELECT_DEBUG
#ifndef WINDOWS
  fprintf(stderr, "selection hits = %d:", _selected[0]);
  for ( i = 0; i < _selected[0]; i++ )
    fprintf(stderr, " %d", _selected[i+1]);
  fprintf(stderr, "\n");
#endif // !WINDOWS
#endif // _SELECT_DEBUG

  if ( selectBuf ) DeAllocate(selectBuf);
  return _selected;
}

void
vfrCamera::depthSwap(unsigned int* list, const int l, const int r) {
  register unsigned int temp = list[l];
  list[l] = list[r];
  list[r] = temp;
}
void
vfrCamera::depthSwap(int* list, const int l, const int r) {
  register int temp = list[l];
  list[l] = list[r];
  list[r] = temp;
}
void
vfrCamera::depthSwap(float* list, const int l, const int r) {
  register float temp = list[l];
  list[l] = list[r];
  list[r] = temp;
}

void
vfrCamera::depthSort(unsigned int* list1,
                    unsigned int* list2,
                    const int left, const int right) {
  if ( left >= right )
    return;

  register int i, last;
  depthSwap(list1, left, (left + right)/2);
  depthSwap(list2, left, (left + right)/2);
  last = left;
  for ( i = left +1; i <= right; i++ )
    if ( list1[i] < list1[last] ) {
      depthSwap(list1, ++last, i);
      depthSwap(list2, last, i);
    }
  depthSwap(list1, left, last);
  depthSwap(list2, left, last);
  depthSort(list1, list2, left, last-1);
  depthSort(list1, list2, last+1, right);
}
void
vfrCamera::depthSort(float* list1, int* list2,
                    const int left, const int right) {
  if ( left >= right )
    return;

  register int i, last;
  depthSwap(list1, left, (left + right)/2);
  depthSwap(list2, left, (left + right)/2);
  last = left;
  for ( i = left +1; i <= right; i++ )
    if ( list1[i] < list1[last] ) {
      depthSwap(list1, ++last, i);
      depthSwap(list2, last, i);
    }
  depthSwap(list1, left, last);
  depthSwap(list2, left, last);
  depthSort(list1, list2, left, last-1);
  depthSort(list1, list2, last+1, right);
}

int*
vfrCamera::clickFeedback(const int cx, const int cy,
                        const unsigned int target) {
  relaxFeedback();
  return feedback(cx, cy, _clickSpotSize, _clickSpotSize, target);
}

int*
vfrCamera::sweepFeedback(const int x, const int y, const int w, const int h,
                        const unsigned int target) {
  int ww = (w < _clickSpotSize) ? _clickSpotSize : w;
  int hh = (h < _clickSpotSize) ? _clickSpotSize : h;
  int cx = x + w/2;
  int cy = y + h/2;
  relaxFeedback();
  return feedback(cx, cy, ww, hh, target);
}

int*
vfrCamera::feedback(const int cx, const int cy, const int w, const int h,
                   const unsigned int target) {
  // (cx, cy) : center in window coordinate.
  //          : (0, 0) as upper-left point.

  GLsizei feedBufSize;
  GLint hits;
  GLint Viewp[4];
  int x, y;

  if ( _feedbacked ) {
    // Not Relaxed.
    return _feedbacked;
  }

  vfrScreen* scrn = getScreen();
  if ( scrn == NULL ) {
    _feedbacked = (int*)Allocate(sizeof(int));
    _feedbacked[0] = 0;
    return _feedbacked;
  }
  scrn->getPort(Viewp);

  x = Viewp[0] + cx;
  y = Viewp[1] + Viewp[3] - cy;

  // Alloc FeedBack Buffer
  vfrNode* tobj = getNode(target);
  if ( tobj == NULL ) {
    _feedbacked = (int*)Allocate(sizeof(int));
    _feedbacked[0] = 0;
    return _feedbacked;
  }
  int NumVerts = tobj->getNumVerts();
  int NumIndices = tobj->getNumIndices();
  if ( NumVerts < 1 && NumIndices < 1 ) {
    _feedbacked = (int*)Allocate(sizeof(int));
    _feedbacked[0] = 0;
    return _feedbacked;
  }
  feedBufSize = tobj->getFeedbackSize();
  register FeedbackType fbkType = tobj->getFeedbackMode();
  GLfloat* feedBuf = (GLfloat*)Allocate(sizeof(GLfloat)*feedBufSize);
  if ( feedBuf == NULL ) {
    VFR_CAMERA_ERR("feedback buffer allocation failed");
    _feedbacked = (int*)Allocate(sizeof(int));
    _feedbacked[0] = 0;
    return _feedbacked;
  }

  // Go FeedBack
  glFeedbackBuffer(feedBufSize, GL_3D, feedBuf);
  (void)glRenderMode(GL_FEEDBACK);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPickMatrix((GLdouble)x, (GLdouble)y, (GLdouble)w, (GLdouble)h, Viewp);
  projection((GLdouble)Viewp[2]/(GLdouble)Viewp[3]);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  _frustum.ApplyModelview();
  //glMultMatrixf(_matrix.value);

  if ( _scene )
    _scene->renderFeedBack(target);

  if ( _front ) {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    Point2 psz = _screen->getSize();
    GLdouble asp = (psz.y <= 0) ? 1.0 : ((GLdouble)psz.x / (GLdouble)psz.y);
    glOrtho(-asp, asp, -1.0, 1.0, -50.0, 50.0);

    _front->renderFeedBack(target);

    glPopMatrix();
  }

#ifdef unix
  glFlush();
#endif

  GLenum error;
  while((error = glGetError()) != GL_NO_ERROR)
#ifndef WINDOWS
    fprintf(stderr, " vfrCamera: feedback: GL error: %s\n",
            gluErrorString(error));
#else
  ;
#endif

  if ( (hits = glRenderMode(GL_RENDER)) < 1 ) {
    if ( feedBuf ) DeAllocate(feedBuf);
    _feedbacked = (int*)Allocate(sizeof(int));
    _feedbacked[0] = 0;
    return _feedbacked;
  }

  // Alloc FeedBacked area
  _feedbacked
    = (int*)Allocate(sizeof(int)*(feedBufSize/FEEDBACK_SIZE_FACTOR +1));
  if ( _feedbacked == NULL ) {
    if ( feedBuf ) DeAllocate(feedBuf);
    _feedbacked = (int*)Allocate(sizeof(int));
    _feedbacked[0] = 0;
    return _feedbacked;
  }
  float* distbuf
    = (float*)Allocate(sizeof(float)*(feedBufSize/FEEDBACK_SIZE_FACTOR +1));
  if ( distbuf ) distbuf[0] = 0;

  _feedbacked[0] = 0;
  register int i, index = 1;
  register int count = hits;
  register Bool getPath = FALSE;
  register float token, zval;
  while ( count ) {
    token = feedBuf[hits - count]; count--;
    if ( token == GL_PASS_THROUGH_TOKEN ) {
      getPath = TRUE;
      _feedbacked[index] = (int)feedBuf[hits - count];
      count--;
    } else if ( getPath &&
               (
                (token == GL_POINT_TOKEN && fbkType == FB_VERTEX) ||
                (token == GL_LINE_TOKEN && fbkType == FB_EDGE) ||
                (token == GL_LINE_RESET_TOKEN && fbkType == FB_EDGE) ||
                (token == GL_POLYGON_TOKEN && fbkType == FB_FACE)
                )
               ) {
      if ( distbuf ) {
        if ( fbkType == FB_VERTEX ) {
          distbuf[index] = feedBuf[hits-count +2];
        } else if ( fbkType == FB_EDGE ) {
          distbuf[index]
	    = (feedBuf[hits-count +2] + feedBuf[hits-count +5]) * 0.5f;
        } else if ( fbkType == FB_FACE && feedBuf[hits-count] > 0 ) {
          for ( zval = 0.f, i = 0; i < feedBuf[hits-count]; i++ )
            zval += feedBuf[hits-count+1 + i*3+2];
          distbuf[index] = zval / feedBuf[hits-count];
        } else
          distbuf[index] = 1.0f;
      }
      _feedbacked[0]++;
      index++;
      getPath = FALSE;
    } else {
      getPath = FALSE;
    }
  }
  if ( distbuf && s_fbkSort ) {
    depthSort(distbuf, _feedbacked, 1, _feedbacked[0]);
  }

#ifdef _SELECT_DEBUG
#ifndef WINDOWS
  fprintf(stderr, " feedback target = %d\n", target);
  fprintf(stderr, " feedback count = %d :", _feedbacked[0]);
  for ( int i = 0; i < _feedbacked[0]; i++ )
    fprintf(stderr, " %d", _feedbacked[i+1]);
  fprintf(stderr, "\n");
#endif // !WINDOWS
#endif // _SELECT_DEBUG

  if ( distbuf ) DeAllocate(distbuf);
  if ( feedBuf ) DeAllocate(feedBuf);
  return _feedbacked;
}

void
vfrCamera::rumor(vfrBase* bp) {
  if ( ! bp ) return;
  if ( bp == _front ) {
    _front = NULL;
    notice();
    return;
  }
  if ( bp == _scene ) {
    _scene = NULL;  // don't use setScene(NULL)
    notice();
    return;
  }
}

void
vfrCamera::notice() {
  if ( _screen )
    _screen->notice();
}

void
vfrCamera::chkNotice() {
  if ( _screen )
    _screen->chkNotice();
}

void
vfrCamera::dragRot(const float dx, const float dy) {
  GLint Viewp[4];
  float rx, ry;
  if ( _screen == NULL ) return;
    _screen->getPort(Viewp);
  GLint l = ((Viewp[2] > Viewp[3]) ? Viewp[2] : Viewp[3]) / 2;
  if ( l < 1 ) return;

  rx = dx * 45.f / l;
  ry = dy * 45.f / l;

  if ( dx != 0.0f ) _frustum.m_hpr[0] += rx;
  if ( dy != 0.0f ) _frustum.m_hpr[1] += ry;

  notice();
}

void
vfrCamera::dragTrans(const float dx, const float dy) {
  GLint Viewp[4];
  float tx, ty;
  if ( dx == 0.0f && dy == 0.0f ) return;
  if ( _screen == NULL ) return;
  _screen->getPort(Viewp);
  if ( Viewp[3] < 1 ) return;
  tx = -2 * _frustum.m_halfH * dx / Viewp[3];
  ty =  2 * _frustum.m_halfH * dy / Viewp[3];

  CES::Vec3<float> x0(1,0,0), y0(0,1,0);
  CES::Mat4<float> RM;
  RM.RotY(CES::Deg2Rad(_frustum.m_hpr[0]));
  RM.RotX(CES::Deg2Rad(_frustum.m_hpr[1]));
  RM.RotZ(CES::Deg2Rad(_frustum.m_hpr[2]));
  CES::Vec3<float> x1 = RM * x0;
  CES::Vec3<float> y1 = RM * y0;

  x1 = x1 * tx; y1 = y1 * ty;
  _frustum.m_eye = _frustum.m_eye + x1;
  _frustum.m_eye = _frustum.m_eye + y1;

  notice();
}

void
vfrCamera::dragTransZ(const float dx, const float dy) {
  GLint Viewp[4];
  float tz;

  if ( dy == 0.0f )
    return;
  if ( _screen == NULL )
    return;
  _screen->getPort(Viewp);
  if ( Viewp[3] < 1 )
    return;

  tz = 2 * _frustum.m_halfH * dy / Viewp[3];

  CES::Mat4<float> RM;
  RM.RotY(CES::Deg2Rad(_frustum.m_hpr[0]));
  RM.RotX(CES::Deg2Rad(_frustum.m_hpr[1]));
  RM.RotZ(CES::Deg2Rad(_frustum.m_hpr[2]));
  CES::Vec3<float> dz = RM * CES::Vec3<float>(0,0,tz);

  _frustum.m_eye = _frustum.m_eye + dz;

#if 0
  float scale = (float)Viewp[3] / (float)(Viewp[3] + 2.0*fabs(dy));
  if ( dy < 0.0f )
    setOrthoHint(scale * _frustum.m_halfH);
  else
    setOrthoHint(_frustum.m_halfH / scale);
#endif
  notice();
}

void
vfrCamera::rotx(const float r) {
  _frustum.m_hpr[1] += CES::Rad2Deg(r);
  notice();
}

void
vfrCamera::roty(const float r) {
  _frustum.m_hpr[0] += CES::Rad2Deg(r);
  notice();
}

void
vfrCamera::rotz(const float r) {
  _frustum.m_hpr[2] += CES::Rad2Deg(r);
  notice();
}

void
vfrCamera::rotation(const float a, const vector3 v) {
  // not supported.
}

void
vfrCamera::trans(const vector3 v) {
  _frustum.m_eye = _frustum.m_eye + CES::Vec3<float>(v);
  notice();
}

void
vfrCamera::identity() {
  _matrix.Identity();
  resetTrans();
}

//static
void
vfrCamera::SetFeedbackSortMode(const Bool mode) {
  s_fbkSort = mode;
}

//static
Bool
vfrCamera::GetFeedbackSortMode() {
  return s_fbkSort;
}

// EOF
