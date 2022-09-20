//
// vfrMesh2D
//   2D Mesh Object Class
//
#ifndef _VFR_MESH_2D_H_
#define _VFR_MESH_2D_H_

#include "vfrNode.h"

class EXPORTED_CLASS vfrMesh2D : public vfrNode {
public:
  vfrMesh2D(const std::string& =std::string(VFR_NONAME), const Bool =FALSE);
  vfrMesh2D(const int m, const int n,
	    const std::string& =std::string(VFR_NONAME), const Bool =FALSE);
  vfrMesh2D(const Point2 msz,
	    const std::string& =std::string(VFR_NONAME), const Bool =FALSE);
  virtual ~vfrMesh2D();

  virtual void renderFeedBack(const unsigned int);
  virtual GLsizei getFeedbackSize() const;
  virtual Bool generateNormals();

  Bool setMeshSize(const Point2);
  Bool setMeshSize(const int, const int);
  Point2 getMeshSize() const {return _meshSize;}

protected:
  void renderSolid();
  void renderWire();

  Point2 _meshSize;
};

#endif /* _VFR_MESH_2D_H_ */
