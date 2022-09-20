//
// vfrCamera3D
//   3D Camera Object Class
//

#include "vfrCamera3D.h"
#define VIEW3D_CLIP_NEAR        0.1f
#define VIEW3D_CLIP_FAR         1000.1f
#define VIEW3D_FIELD_OF_VIEW    45.0f
#define VIEW3D_WIDE_HALF        5.0f
#define PI_PER_180              0.017453293f


vfrCamera3D::vfrCamera3D(const Bool ssm) : vfrCamera(ssm) {
  setProjection(PR_PERSPECTIVE);
}

vfrCamera3D::~vfrCamera3D() {
}

void
vfrCamera3D::projection(const GLdouble aspect) {
  _frustum.ApplyProjection((_projMode == PR_ORTHOGONAL), aspect);
}

void
vfrCamera3D::setHpr(const float h, const float p, const float r) {
  _frustum.m_hpr[0] += h;
  _frustum.m_hpr[1] += p;
  _frustum.m_hpr[2] += r;
  notice();
}

Bool
vfrCamera3D::sweepZoom(const Point2 p0, const Point2 p1) {
  int nW = (int)fabs((double)(p1.x - p0.x));
  int nH = (int)fabs((double)(p1.y - p0.y));
  if ( nW < 5 || nH < 5 ) return FALSE;
  vfrNode* target = getScene();
  if ( ! target ) return FALSE;
  vfrScreen* screen = getScreen();
  if ( ! screen ) return FALSE;
  GLint oVP[4]; screen->getPort(oVP);
  CES::Mat4<float> MVM = _frustum.GetMVM();

  // Center Point
  Point2 so0; so0.x = oVP[2] / 2; so0.y = oVP[3] / 2;
  Point2 so1;
  so1.x = (p0.x + p1.x) / 2;
  so1.y = (p0.y + p1.y) / 2;
  so1 = screen->getRelativePoint(so1);
  CES::Vec3<float> vo0, vo1, vW;
  vector3 vO;
  screen->getObjCoord(target->getID(), so0, vO);
  vW[0] = vO[0]; vW[1] = vO[1]; vW[2] = vO[2]; vW[3] = 1.0f;
  vo0 = MVM * vW;
  screen->getObjCoord(target->getID(), so1, vO);
  vW[0] = vO[0]; vW[1] = vO[1]; vW[2] = vO[2]; vW[3] = 1.0f;
  vo1 = MVM * vW;
  vector3 vD = {vo1[0]-vo0[0], vo1[1]-vo0[1], vo1[2]-vo0[2]};
  trans(vD);

  if ( getProjection() == PR_ORTHOGONAL ) {
    // Viewport Size
    double oH = _frustum.m_halfH;
    double oH2 = oH;
    if ( nW > nH ) {
      oH2 *= ((double)nW / (double)oVP[2]);
    } else {
      oH2 *= ((double)nH / (double)oVP[3]);
    }
    if ( oH2 > EPSF )
      setOrthoHint(oH2);

    // Close to Focus (for Perspective)
    CES::Vec3<float> fe(0,0,-1);
    CES::Mat4<float> MVRM = _frustum.GetMVRM();
    fe = MVRM * fe; fe.UnitVec();
    float L1 = _frustum.m_dist;
    float L2 = L1 * (1.0f - (float)(oH2 / oH));
    fe = fe * L2;
    _frustum.m_eye = _frustum.m_eye + fe;
    setFogParam(L1, L1*2);
  }
  return TRUE;
}

// EOF
