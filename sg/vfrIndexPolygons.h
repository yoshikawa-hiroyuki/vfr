//
// vfrIndexPolygons
//   Indexed Polygons Class
//
#ifndef _VFR_INDEXPOLYGONS_H_
#define _VFR_INDEXPOLYGONS_H_

#include "vfrNode.h"

#define VFR_IDXP_MAX_VERTS_PER_POLYGON  32

#ifdef GLU_VERSION_1_2
#define VFR_IDXPLGN_TESS 1
#else
#define VFR_IDXPLGN_TESS 0
#endif


class EXPORTED_CLASS vfrIndexPolygons : public vfrNode {
public:
  vfrIndexPolygons(const std::string& =std::string(VFR_NONAME),
		   const Bool =FALSE);
  virtual ~vfrIndexPolygons();
  
#if VFR_IDXPLGN_TESS
  void setTessMode(const Bool f) {_useTess = f; notice();}
#else
  void setTessMode(const Bool f) {_useTess = FALSE;}
#endif
  Bool getTessMode() const {return _useTess;}
  void renderFeedBack(const unsigned int);
  Bool generateNormals();

protected:
  void renderSolid();
  void renderWire();
  Bool _useTess;

#if VFR_IDXPLGN_TESS
  static GLUtesselator* getTessObj(const Bool);
#else
  static void* getTessObj(const Bool);
#endif
  static void clearTessObj();

private:
#if VFR_IDXPLGN_TESS
  static GLUtesselator* _tessObj;
  static GLvoid beginCallback(GLenum);
  static GLvoid errorCallback(GLenum);
  static GLvoid endCallback();
  static GLvoid vertexCallback3(GLvoid*);
  static GLvoid vertexCallback9(GLvoid*);
  static GLvoid combineCallback9(GLdouble[3], void*[4], GLfloat[4], void**);
#endif
};

#endif /* _VFR_INDEXPOLYGONS_H_ */
