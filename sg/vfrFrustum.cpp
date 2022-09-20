//
// vfrFrustum
//     View Frustum class
//

#include "vfrFrustum.h"
using namespace CES;

// static members

float vfrFrustum::s_strOff = 0.05f;


// methods

Bool vfrFrustum::ApplyProjection(const Bool ortho, const double asp) const
{
  if ( m_far - m_near < EPSF ) return FALSE;
  if ( ! ortho && m_near < EPSF ) return FALSE;
  if ( m_halfW < EPSF || m_halfH < EPSF ) return FALSE;

  Vec3<float> eoff = m_eyeOff;
  if ( m_eyeType == LEFT_EYE ) eoff[0] -= s_strOff;
  else if ( m_eyeType == RIGHT_EYE ) eoff[0] += s_strOff;

  double aspect(asp);
  if ( aspect < EPSF ) aspect = 1.0;
  register double wBias = aspect * m_halfH / m_halfW;

  double left, right, top, bottom;
  if ( ! ortho ) {
    double d = m_near / (m_dist + eoff[2]);
    top    =  (m_halfH - eoff[1]) * d;
    bottom = -(m_halfH + eoff[1]) * d;
    right  =  (m_halfW * wBias - eoff[0]) * d;
    left   = -(m_halfW * wBias + eoff[0]) * d;
    glFrustum(left, right, bottom, top, m_near, m_far);
  } else {
    top    =  m_halfH - eoff[1];
    bottom = -(m_halfH + eoff[1]);
    right  =  m_halfW * wBias - eoff[0];
    left   = -(m_halfW * wBias + eoff[0]);
    glOrtho(left, right, bottom, top, m_near, m_far);
  }

  return TRUE;
}

void vfrFrustum::ApplyModelview() const
{
  Vec3<float> eoff = m_eyeOff;
  if ( m_eyeType == LEFT_EYE ) eoff[0] -= s_strOff;
  else if ( m_eyeType == RIGHT_EYE ) eoff[0] += s_strOff;

  glTranslatef(-eoff.m_v[0], -eoff.m_v[1], -eoff.m_v[2]);
  glRotatef(-m_hpr[2], 0.f, 0.f, 1.f);
  glRotatef(-m_hpr[1], 1.f, 0.f, 0.f);
  glRotatef(-m_hpr[0], 0.f, 1.f, 0.f);
  glTranslatef(-m_eye.m_v[0], -m_eye.m_v[1], -m_eye.m_v[2]);
}

CES::Mat4<float> vfrFrustum::GetPM(const Bool ortho, const double asp) const
{
  Mat4<float> PM;
  if ( m_far - m_near < EPSF ) return FALSE;
  if ( ! ortho && m_near < EPSF ) return FALSE;
  if ( m_halfW < EPSF || m_halfH < EPSF ) return PM;

  Vec3<float> eoff = m_eyeOff;
  if ( m_eyeType == LEFT_EYE ) eoff[0] -= s_strOff;
  else if ( m_eyeType == RIGHT_EYE ) eoff[0] += s_strOff;

  double aspect(asp);
  if ( aspect < EPSF ) aspect = 1.0;
  register double wBias = aspect * m_halfH / m_halfW;

  double left, right, top, bottom;
  if ( ! ortho ) {
    double d = m_near / (m_dist + eoff[2]);
    top    =  (m_halfH - eoff[1]) * d;
    bottom = -(m_halfH + eoff[1]) * d;
    right  =  (m_halfW * wBias - eoff[0]) * d;
    left   = -(m_halfW * wBias + eoff[0]) * d;

    PM[ 0] = 2.f*m_near/(right-left);
    PM[ 5] = 2.f*m_near/(top-bottom);
    PM[ 8] = (right+left)/(right-left);
    PM[ 9] = (top+bottom)/(top-bottom);
    PM[10] = -(m_far+m_near)/(m_far-m_near);
    PM[11] = -1.f;
    PM[14] = -2.f*m_far*m_near/(m_far-m_near);
    PM[15] = 0.f;
  }
  else {
    top    =  m_halfH - eoff[1];
    bottom = -(m_halfH + eoff[1]);
    right  =  m_halfW * wBias - eoff[0];
    left   = -(m_halfW * wBias + eoff[0]);

    PM[ 0] = 2.f/(right-left);
    PM[ 5] = 2.f/(top-bottom);
    PM[10] = -2.f/(m_far-m_near);
    PM[12] = -(right+left)/(right-left);
    PM[13] = -(top+bottom)/(top-bottom);
    PM[14] = (m_far+m_near)/(m_far-m_near);
  }

  return PM;
}

CES::Mat4<float> vfrFrustum::GetMVRM() const
{
  Mat4<float> MM;
  MM.RotZ(-Deg2Rad(m_hpr[2]));
  MM.RotX(-Deg2Rad(m_hpr[1]));
  MM.RotY(-Deg2Rad(m_hpr[0]));
  return MM;
}
CES::Mat4<float> vfrFrustum::GetMVM() const
{
  Vec3<float> eye, eoff = m_eyeOff;
  if ( m_eyeType == LEFT_EYE ) eoff[0] -= s_strOff;
  else if ( m_eyeType == RIGHT_EYE ) eoff[0] += s_strOff;

  Mat4<float> MM;
  MM.Translate(eoff * (-1));
  MM.RotZ(-Deg2Rad(m_hpr[2]));
  MM.RotX(-Deg2Rad(m_hpr[1]));
  MM.RotY(-Deg2Rad(m_hpr[0]));
  MM.Translate(m_eye * (-1));

  return MM;
}

Vec3<float> vfrFrustum::GetEye() const
{
  return (m_eye + m_eyeOff);
}

Vec3<float> vfrFrustum::GetViewDirMVM() const
{
  Vec3<float> eye, vdir;
  Mat4<float> MM = GetMVM();
  eye = MM * Vec3<float>(0, 0, 0);
  vdir = MM * Vec3<float>(0, 0, -1) - eye;
  return vdir;
}
