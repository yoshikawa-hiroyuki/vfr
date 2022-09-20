#ifndef _VFRU_STL_H_
#define _VFRU_STL_H_


//--------------------------------------------------------------------
//  vfruStl - structure for STL
//--------------------------------------------------------------------
struct vfruStl {
  struct Facet {
    float p[3][3];
    float nv[3];
    unsigned short ext;
    unsigned short _dummy; // for padding

    Facet() : ext(0), _dummy(0) {
      memset(p, 0, sizeof(float)*9); memset(nv, 0, sizeof(float)*3);
    }
    Facet(const Facet& org) {*this = org;}
    void operator=(const Facet& org) {
      memcpy(p, org.p, sizeof(float)*9); memcpy(nv, org.nv, sizeof(float)*3);
      ext = org.ext;
    }
  };

  static bool IsAscii(const char* path);
  static class vfrNode* ReadSla(const char* path);
  static class vfrNode* ReadSlb(const char* path, const bool bigEdn =false);
};

#endif // _VFRU_STL_H_
