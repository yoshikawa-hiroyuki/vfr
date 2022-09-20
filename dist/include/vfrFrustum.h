//
// vfrFrustum
//     View Frustum class
//
#ifndef _VFR_FRUSTUM_H_
#define _VFR_FRUSTUM_H_

#ifdef WINDOWS
#include "stdafx.h"
#endif

#include <stdio.h>
#include <stdlib.h>

#include "utilMath.h"
#include "vfrDefs.h"


class EXPORTED_CLASS vfrFrustum {
public:
  enum EyeType {CYCLOP, LEFT_EYE, RIGHT_EYE};

  EyeType m_eyeType;
  CES::Vec3<float> m_eye, m_eyeOff;
  float m_hpr[3], m_dist, m_halfW, m_halfH;
  double m_near, m_far;

  vfrFrustum() : m_eyeType(CYCLOP), m_eye(0.f, 0.f, 6.f),
		 m_dist(6.f), m_halfW(6.f), m_halfH(6.f),
		 m_near(0.5), m_far(500.5) {
    m_hpr[0] = m_hpr[1] = m_hpr[2] = 0.f;
  }
  vfrFrustum(const vfrFrustum& org) {
    *this = org;
  }
  virtual ~vfrFrustum() {}

  void operator=(const vfrFrustum& org) {
    m_eyeType = org.m_eyeType;
    m_eye = org.m_eye; m_eyeOff = org.m_eyeOff;
    m_hpr[0] = org.m_hpr[0]; m_hpr[1] = org.m_hpr[1]; m_hpr[2] = org.m_hpr[2];
    m_dist = org.m_dist; m_halfW = org.m_halfW; m_halfH = org.m_halfH;
    m_near = org.m_near; m_far = org.m_far;
  }

  Bool ApplyProjection(const Bool ortho =FALSE, const double asp =1.0) const;
  void ApplyModelview() const;

  CES::Mat4<float> GetPM(const Bool ortho =FALSE, const double asp =1.0) const;
  CES::Mat4<float> GetMVRM() const;
  CES::Mat4<float> GetMVM() const;
  CES::Vec3<float> GetEye() const;
  CES::Vec3<float> GetViewDirMVM() const;

  static float s_strOff;
};

#endif // _VFR_FRUSTUM_H_
