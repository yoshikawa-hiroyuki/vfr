#ifndef _VFRU_WF_OBJ_H_
#define _VFRU_WF_OBJ_H_

#include <deque>
#include <string>


//--------------------------------------------------------------------
//  vfruWfMtl - structure for Material file of WaveFront Obj
//--------------------------------------------------------------------
struct vfruWfMtl {
  std::string m_name;
  float m_shininess;
  float m_ambent[3];
  float m_diffuse[3];
  float m_specular[3];
  std::string m_texture;

  void reset() {
    m_shininess = 5.f;
    m_ambent[0] = m_ambent[1] = m_ambent[2] = .2f;
    m_diffuse[0] = m_diffuse[1] = m_diffuse[2] = 1.f;
    m_specular[0] = m_specular[1] = m_specular[2] = 0.f;
  }
  void operator=(const vfruWfMtl& org) {
    m_name = org.m_name;
    m_shininess = org.m_shininess;
    memcpy(m_ambent, org.m_ambent, sizeof(float)*3);
    memcpy(m_diffuse, org.m_diffuse, sizeof(float)*3);
    memcpy(m_specular, org.m_specular, sizeof(float)*3);
    m_texture = org.m_texture;
  }

  vfruWfMtl() {reset();}
  vfruWfMtl(const vfruWfMtl& org) {*this = org;}
  ~vfruWfMtl() {}

  static int ReadMtl(std::deque<vfruWfMtl>& ml,
                     const char* path, const char* refpath =NULL);
};


//--------------------------------------------------------------------
//  vfruWfObj - structure for WaveFront Obj
//--------------------------------------------------------------------
struct vfruWfObj {
  struct V3 {
    float xyz[3];
    V3() {xyz[0]=xyz[1]=xyz[2]=0.f;}
    V3(const V3& org) {*this = org;}
    void operator=(const V3& org) {
      memcpy(xyz, org.xyz, sizeof(float)*3);
    }
  };

  struct V2 {
    float uv[2];
    V2() {uv[0]=uv[1]=0.f;}
    V2(const V3& org) {*this = org;}
    void operator=(const V2& org) {
      memcpy(uv, org.uv, sizeof(float)*2);
    }
  };

  struct I3 {
    int idx[3];
    I3() {idx[0]=idx[1]=idx[2]=0;}
    I3(const I3& org) {*this = org;}
    void operator=(const I3& org) {
      memcpy(idx, org.idx, sizeof(int)*3);
    }
    bool IsValid() const {
      return (idx[0]>=0 && idx[1]>=0 && idx[2]>=0);
    }
  };

  static class vfrNode* ReadObj(const char* path,
				std::deque<vfruWfMtl>* pml =NULL,
				std::deque<std::string>* pmpl =NULL);
};

#endif // _VFRU_WF_OBJ_H_

