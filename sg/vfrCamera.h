//
// vfrCamera
//     Basic Camera Object Class
//
#ifndef _VFR_CAMERA_H_
#define _VFR_CAMERA_H_

#include "vfrScene.h"
#include "vfrScreen.h"
#include "vfrFrustum.h"

#define VFR_SELECT_SUITE_SPOT 5


class EXPORTED_CLASS vfrCamera : public vfrBase {
public:
  virtual ~vfrCamera();

  virtual void redraw();
  virtual void redraw(const GLdouble);

  void setFrustum(const vfrFrustum& ofr) {_frustum = ofr;}
  vfrFrustum getFrustum() {return _frustum;}

  void setScene(vfrScene*);
  vfrScene* getScene() {return _scene;}

  void setFrontNode(vfrNode*);
  vfrNode* getFrontNode() {return _front;}

  void setScreen(vfrScreen*);
  vfrScreen* getScreen() {return _screen;}

  vfrMatrix getProjMatrix(const double asp =1.) const {
    return _frustum.GetPM((getProjection()==PR_ORTHOGONAL), asp);
  }
  vfrMatrix getModelMatrix() const {return _frustum.GetMVM();}

  void setBgColor(const vector4);
  void setBgColor(const float, const float, const float);
  void setBgColor(const float, const float, const float, const float);
  void getBgColor(vector4) const;

  void setFogMode(const Bool);
  void setFogMode(const Bool, const float, const float);
  Bool getFogMode() const {return _fogMode;}
  void setFogParam(const float, const float);
  void getFogParam(float&, float&) const;

  virtual void setProjection(const ProjectType) {}
  virtual ProjectType getProjection() const {return PR_ORTHOGONAL;}

  void setClickSpotSize(const int);
  int getClickSpotSize() const {return _clickSpotSize;}
  unsigned int* clickSelect(const int, const int);
  unsigned int* sweepSelect(const int, const int, const int, const int);
  int* clickFeedback(const int, const int, const unsigned int);
  int* sweepFeedback(const int, const int, const int, const int,
		     const unsigned int);
  void relaxSelect();
  void relaxFeedback();

  virtual float getFocusDepth() const {return 0.98f;}
  virtual void setOrthoHint(const float h) {
    if ( h < EPSF ) return;
    _frustum.m_halfW *= (h / _frustum.m_halfH);
    _frustum.m_halfH = h;
    notice();
  }    

  virtual void dragRot(const int x, const int y) {
    dragRot((float)x, (float)y);}
  virtual void dragRot(const float, const float);
  virtual void dragTrans(const int x, const int y) {
    dragTrans((float)x, (float)y);}
  virtual void dragTrans(const float, const float);
  virtual void dragTransZ(const int x, const int y) {
    dragTransZ((float)x, (float)y);}
  virtual void dragTransZ(const float, const float);
  
  virtual void rotx(const float r);
  virtual void roty(const float r);
  virtual void rotz(const float r);
  virtual void rotation(const float a, const vector3 v);
  virtual void scale(const vector3) {}
  virtual void scale(const float) {}
  virtual void scale(const float, const float, const float) {}
  virtual void trans(const vector3);
  virtual void trans(const float x, const float y, const float z) {
    vector3 v = {x, y, z};
    trans(v);
  }
  virtual void identity();

  vfrNode* getNode(const unsigned int s);
  vfrNode* getNode(const std::string& s);
  Bool accumMatrix(const unsigned int, vfrMatrix&) const;
  void rumor(vfrBase*);
  void notice();
  void chkNotice();
  void clearDispList();

  void setAntiAliasMode(const Bool am) {_antiAlias = am;}
  Bool getAntiAliasMode() const {return _antiAlias;}
  virtual Bool sweepZoom(const Point2 p0, const Point2 p1) = 0;

  static void SetFeedbackSortMode(const Bool mode);
  static Bool GetFeedbackSortMode();

protected:
  vfrCamera(const Bool =FALSE);

  void init();
  virtual void resetTrans();
  virtual void bgPaint();
  virtual void fgPaint();
  virtual void projection(const GLdouble) {}

  vfrFrustum _frustum;
  vector4    _bgColor;
  Bool       _fogMode;
  float      _fogStart, _fogEnd;
  Bool       _antiAlias;
  vfrScene*  _scene;
  vfrNode*   _front;
  vfrScreen* _screen;
  int        _clickSpotSize;

  unsigned int* _selected;
  unsigned int* _selectedBbox;
  unsigned int* selection(const int, const int, const int, const int);
  unsigned int* selectionBbox(const int, const int, const int,
			      const int, const GLint*);

  int* _feedbacked;
  int* feedback(const int, const int, const int, const int,
		const unsigned int);

  void depthSort(unsigned int*, unsigned int*, const int, const int);
  void depthSort(float*, int*, const int, const int);
  void depthSwap(unsigned int*, const int, const int);
  void depthSwap(float*, const int, const int);
  void depthSwap(int*, const int, const int);

  static Bool s_fbkSort;
};

#endif /* _VFR_CAMERA_H_ */
