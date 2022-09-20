//
// vfrCamera2D
//   2D Camera Object Class
//

#include "vfrCamera2D.h"
#define VIEW2D_DEPTH_HALF 1000.0f
#define VIEW2D_WIDE_HALF  5.0f


vfrCamera2D::vfrCamera2D(const Bool ssm) : vfrCamera(ssm) {
  identity();
}

vfrCamera2D::~vfrCamera2D() {
}

void
vfrCamera2D::projection(const GLdouble aspect) {
  _frustum.ApplyProjection(TRUE, aspect);
}

void
vfrCamera2D::dragTrans(const float dx, const float dy) {
  vector3 tv;
  float tx, ty;

  if ( dx == 0.0f && dy == 0.0f )
    return;
  vfrScreen* scrn = getScreen();
  if ( ! scrn )
    return;
  Point2 scrnSize = scrn->getSize();
  if ( scrnSize.y < 1 )
    return;

  tx = -2 * (float)_frustum.m_halfH * dx / scrnSize.y;
  ty =  2 * (float)_frustum.m_halfH * dy / scrnSize.y;
  tv[0] = tx;
  tv[1] = ty;
  tv[2] = 0.0f;

  trans(tv);
}

void
vfrCamera2D::dragTransZ(const float, const float dy) {
  double s;

  if ( dy == 0.0f ) return;
  vfrScreen* scrn = getScreen();
  if ( ! scrn ) return;
  Point2 scrnSize = scrn->getSize();
  if ( scrnSize.y < 1 ) return;

  if ( dy < 0.0f ) {
    s = 0.5 * (double)dy / ((double)scrnSize.y *0.5) + 1.0; // < 1.0
  } else {
    s = (double)dy / ((double)scrnSize.y *0.5) + 1.0; // > 1.0
  }
  setOrthoHint(_frustum.m_halfH * s);
}

Bool
vfrCamera2D::sweepZoom(const Point2 p0, const Point2 p1) {
  int nW = (int)fabs((double)(p1.x - p0.x));
  int nH = (int)fabs((double)(p1.y - p0.y));
  if ( nW < 5 || nH < 5 ) return FALSE;
  vfrNode* target = getScene();
  if ( ! target ) return FALSE;
  vfrScreen* screen = getScreen();
  if ( ! screen ) return FALSE;
  GLint oVP[4]; screen->getPort(oVP);

  // Center Point
  //Point2 so0; so0.x = oVP[2] / 2; so0.y = oVP[3] / 2;
  Point2 so0;
  so0.x = oVP[0] + oVP[2]/2; so0.y = oVP[1] + oVP[3]/2;
  Point2 so1;
  so1.x = (p0.x + p1.x) / 2;
  so1.y = (p0.y + p1.y) / 2;
  //so1 = screen->getRelativePoint(so1);
  vector3 vo0, vo1;
  screen->getObjCoord(target->getID(), so0, vo0);
  screen->getObjCoord(target->getID(), so1, vo1);
  vector3 vD = {vo1[0]-vo0[0], vo1[1]-vo0[1], vo1[2]-vo0[2]};
  trans(vD);

  // Viewport Size
  double oH = _frustum.m_halfH;
  if ( nW > nH ) {
    oH *= ((double)nW / (double)oVP[2]);
  } else {
    oH *= ((double)nH / (double)oVP[3]);
  }
  if ( oH > EPSF ) {
    setOrthoHint(oH);
  }
  return TRUE;
}

// EOF
