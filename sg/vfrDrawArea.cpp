//
// vfrDrawArea
//     Drawing-Area base class
//
#include "vfrDrawArea.h"
#include "vfrScreen.h"

vfrDrawArea::vfrDrawArea() :
  nScreen(0), cScreen(-1), scrBorderMode(FALSE), screenMapType(HORIZONTAL),
  rb_flag(FALSE), rb_lineWidth(2.f), rb_lineType(ST_SOLID),
  da_dispatcher()
{
  for ( register int i = 0; i < VFR_MAX_SCREEN_NUM; i++ )
    da_screen[i] = (vfrScreen*)NULL;

  rb_color[0] = rb_color[1] = rb_color[2] = 0.8f; rb_color[3] = 1.f;
}

vfrDrawArea::~vfrDrawArea() {
  for ( register int s = 0; s < VFR_MAX_SCREEN_NUM; s++ )
    if ( da_screen[s] )
      da_screen[s]->setParent(NULL);
}

Bool vfrDrawArea::addScreen(vfrScreen* s) {
  if ( ! s ) return FALSE;
  if ( nScreen >= VFR_MAX_SCREEN_NUM ) return FALSE;
  s->setParent(this);
  da_screen[nScreen++] = s;
  s->setBorderMode(scrBorderMode);
  remapScreen();
  setCurScreen(nScreen-1);
  return TRUE;
}

Bool vfrDrawArea::remScreen(vfrScreen* s) {
  for ( register int i = 0; i < nScreen; i++ ) {
    if ( da_screen[i] == s ) {
       for ( register int j = i; j < nScreen -1; j++ ) {
         da_screen[j] = da_screen[j+1];
       }
       nScreen--;
       remapScreen();
       if ( i <= nScreen )
         setCurScreen(i-1);
       else
         setCurScreen(i);
       return TRUE;
    }
  }
  return FALSE;
}

void vfrDrawArea::remapScreen() {
  Point2 size = getSize();
  int w = size.x, h = size.y;
  switch ( nScreen ) {
  case 1:
    da_screen[0]->setPort(0, 0, w, h);
    break;
  case 2:
    if ( screenMapType == HORIZONTAL ) {
      da_screen[0]->setPort(         0,          0, (int)(w/2),          h);
      da_screen[1]->setPort((int)(w/2),          0, (int)(w/2),          h);
    } else {
      da_screen[0]->setPort(         0,          0,          w, (int)(h/2));
      da_screen[1]->setPort(         0, (int)(h/2),          w, (int)(h/2));
    }
    break;
  case 3:
    if ( screenMapType == HORIZONTAL ) {
      da_screen[0]->setPort(         0,          0, (int)(w/2), (int)(h/2));
      da_screen[1]->setPort((int)(w/2),          0, (int)(w/2), (int)(h/2));
      da_screen[2]->setPort(         0, (int)(h/2), (int)(w/2), (int)(h/2));
    } else {
      da_screen[0]->setPort(         0,          0, (int)(w/2), (int)(h/2));
      da_screen[1]->setPort(         0, (int)(h/2), (int)(w/2), (int)(h/2));
      da_screen[2]->setPort((int)(w/2),          0, (int)(w/2), (int)(h/2));
    }
    break;
  case 4:
    if ( screenMapType == HORIZONTAL ) {
      da_screen[0]->setPort(         0,          0, (int)(w/2), (int)(h/2));
      da_screen[1]->setPort((int)(w/2),          0, (int)(w/2), (int)(h/2));
      da_screen[2]->setPort(         0, (int)(h/2), (int)(w/2), (int)(h/2));
      da_screen[3]->setPort((int)(w/2), (int)(h/2), (int)(w/2), (int)(h/2));
    } else {
      da_screen[0]->setPort(         0,          0, (int)(w/2), (int)(h/2));
      da_screen[1]->setPort(         0, (int)(h/2), (int)(w/2), (int)(h/2));
      da_screen[2]->setPort((int)(w/2),          0, (int)(w/2), (int)(h/2));
      da_screen[3]->setPort((int)(w/2), (int)(h/2), (int)(w/2), (int)(h/2));
    }
    break;
  default:
    break;
  }
  notice();
}

int vfrDrawArea::whichScreen(const int x, const int y) {
  for ( register int i = 0; i < nScreen; i++ ) {
    if ( da_screen[i]->isOnScreen(x, y) )
      return i;
  }
  return -1;
}

int vfrDrawArea::setCurScreen(const int cs) {
  const vector4 cCur = {0.9f, 0.9f, 0.9f, 1.0f};
  const vector4 cOth = {0.4f, 0.4f, 0.4f, 1.0f};
  if ( cs < -1 || cs >= nScreen ) // -1 means Nothing Selected.
    return cScreen;
  cScreen = cs;
  if ( scrBorderMode ) {
    for ( register int i = 0; i < nScreen; i++ ) {
      if ( i == cScreen ) {
        da_screen[i]->setBorderColor(cCur);
        da_screen[i]->setBorderWidth(2.5f);
      } else {
        da_screen[i]->setBorderColor(cOth);
        da_screen[i]->setBorderWidth(1.5f);
      }
    }
  } else {
    for ( register int i = 0; i < nScreen; i++ ) {
      da_screen[i]->setBorderColor(cOth);
      da_screen[i]->setBorderWidth(1.0f);
    }
  }
  return cScreen;
}

void vfrDrawArea::setScrBorderMode(const Bool sbm) {
  if ( scrBorderMode == sbm ) return;
  scrBorderMode = sbm;
  for ( register int i = 0; i < nScreen; i++ ) {
    da_screen[i]->setBorderMode(scrBorderMode);
  }
  this->setCurScreen(cScreen);
}

Bool vfrDrawArea::getScrBorderMode() const {
  return scrBorderMode;
}

void vfrDrawArea::setScreenMapType(const vfrDrawArea::ScrMapType smt) {
  if (screenMapType == smt) return;
  screenMapType = smt; remapScreen();
}

vfrDrawArea::ScrMapType vfrDrawArea::getScreenMapType() const {
  return screenMapType;
}

vfrScreen* vfrDrawArea::getScreen(const int ns) {
  if ( ns >= nScreen || ns < 0 )
    return NULL;
  return da_screen[ns];
}


vfrDispatch& vfrDrawArea::getDispatcher() {
  return da_dispatcher;
}


void vfrDrawArea::drawRubberBox(const Point2& p0, const Point2& p1) {
  int x0, y0, w, h;
  int dx = p1.x - p0.x;
  int dy = p1.y - p0.y;
  if ( dx < 0 ) {
    x0 = p1.x;
    w  = -dx;
  } else {
    x0 = p0.x;
    w  = dx;
  }
  if ( dy < 0 ) {
    y0 = p1.y;
    h = -dy;
  } else {
    y0 = p0.y;
    h = dy;
  }

  rb_flag = ! rb_flag;
  rb_0.x = x0; rb_0.y = y0;
  rb_sz.x = w; rb_sz.y = h;

  notice();
}

void vfrDrawArea::setRubberBoxColor(const vector4 rbc) {
  memcpy(rb_color, rbc, sizeof(vector4));
}

void vfrDrawArea::getRubberBoxColor(vector4& rbc) const {
  memcpy(rbc, rb_color, sizeof(vector4));
}

void vfrDrawArea::setRubberBoxLineWidth(const GLfloat rblw) {
  rb_lineWidth = rblw;
}

GLfloat vfrDrawArea::getRubberBoxLineWidth() const {
  return rb_lineWidth;
}

void vfrDrawArea::setRubberBoxLineType(const StippleType rblt) {
  rb_lineType = rblt;
}

StippleType vfrDrawArea::getRubberBoxLineType() const {
  return rb_lineType;
}

void vfrDrawArea::rb_draw() {
  const static GLushort stipplePattern[] = {
    0xFFFF, 0x3333, 0x00FF, 0x88FF, 0x111F
  };

  if ( ! rb_flag ) return;

  Point2 sz = getSize();
  glViewport(0, 0, sz.x, sz.y);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_LIGHTING);
  glColor4fv(rb_color);
  glLineWidth(rb_lineWidth);
  if ( rb_lineType != ST_SOLID ) {
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, stipplePattern[rb_lineType]);
  }

  vector2 p0, p1;
  p0[0] = 2.f * (float)rb_0.x / (float)sz.x  - 1.f;
  p0[1] = 2.f * (float)(sz.y - rb_0.y) / (float)sz.y - 1.f;
  p1[0] = 2.f * (float)(rb_0.x + rb_sz.x) / (float)sz.x  - 1.f;
  p1[1] = 2.f * (float)(sz.y - rb_0.y - rb_sz.y) / (float)sz.y - 1.f;

  glBegin(GL_LINE_STRIP);
  glVertex2f(p0[0], p0[1]);
  glVertex2f(p1[0], p0[1]);
  glVertex2f(p1[0], p1[1]);
  glVertex2f(p0[0], p1[1]);
  glVertex2f(p0[0], p0[1]);
  glEnd();

  if ( rb_lineType != ST_SOLID ) glDisable(GL_LINE_STIPPLE);
}

// EOF
