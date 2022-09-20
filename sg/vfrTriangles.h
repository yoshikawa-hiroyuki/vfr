//
// vfrTriangles
//   Triangles Class
//
#ifndef _VFR_TRIANGLES_H_
#define _VFR_TRIANGLES_H_

#include "vfrNode.h"
#include <deque>


class EXPORTED_CLASS vfrTriangles : public vfrNode {
public:
  vfrTriangles(const std::string& =std::string(VFR_NONAME), const Bool =FALSE);
  virtual ~vfrTriangles();

  virtual void renderFeedBack(const unsigned int);
  virtual Bool generateNormals();

protected:
  virtual void renderSolid();
  virtual void renderWire();
};


//--------------- Normal smoother class ---------------
#define TS_HASH_TABLE_SIZE 9967
//  both 283 and 9967 are prime number

class EXPORTED_CLASS vfrTriaSmoother {
public:
  Bool SmoothNorm(vfrTriangles& obj);
  float getTolerance() const {return cosTolerance;}

  struct HASH_ENTRY {
    size_t tri, off;
    HASH_ENTRY(const size_t t =0, const size_t o =0) : tri(t), off(o) {}
  };

  vfrTriaSmoother(const float tolerance =0.f);
  virtual ~vfrTriaSmoother();

private:
  std::deque<HASH_ENTRY> hTbl[TS_HASH_TABLE_SIZE];
  float cosTolerance;

  static size_t hashValue(const vector3 v) {
    return (size_t)fabs(((v[0]*283+v[1])*283)+v[2]) % TS_HASH_TABLE_SIZE;
  }

  void hashInsert(const vector3 v, const size_t t, const size_t o) {
    size_t h = hashValue(v);
    HASH_ENTRY hh(t, o);
    hTbl[h].push_back(hh);
  }
};

#endif /* _VFR_TRIANGLES_H_ */
