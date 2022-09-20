//
// vfrScreen
//   Viewport Screen Class
//
#ifndef _VFR_SCREEN_H_
#define _VFR_SCREEN_H_

#include "vfrNode.h"


class EXPORTED_CLASS vfrScreen {
public:
  vfrScreen();
  vfrScreen(const int, const int, const int, const int);
  virtual ~vfrScreen();

  void redraw();
  void notice();
  void chkNotice();
  void rumor();

  void setCamera(class vfrCamera*);
  class vfrCamera* getCamera() {return _camera;}
  void setParent(class vfrDrawArea*);
  class vfrDrawArea* getParent() {return _parent;}

  Bool isOnScreen(const Point2);
  Bool isOnScreen(const int, const int);

  Bool setPort(const int,const int, const int,const int);
  Bool setPort(const Point2, const Point2);
  void getPort(GLint*) const;
  Bool setPosition(const int, const int);
  Bool setPosition(const Point2);
  Point2 getPosition() const;
  Bool setSize(const int, const int);
  Bool setSize(const Point2);
  Point2 getSize() const;
  Point2 getRelativePoint(const Point2) const;

  unsigned int* clickSelect(const int, const int);
  unsigned int* sweepSelect(const int, const int, const int, const int);
  int* clickFeedback(const int, const int, const unsigned int);
  int* sweepFeedback(const int, const int, const int, const int,
		     const unsigned int);

  vfrNode* getNode(const unsigned int);
  vfrNode* getNode(const std::string&);

  Bool getObjCoord(const unsigned int, const Point2, vector3&) const;
  Bool getWinCoord(const unsigned int, const vector3, Point2&) const;

  void startRubberBox(const Point2);
  void drawRubberBox(const Point2);
  void clearRubberBox();
  void clearRubberBox(Point2*, Point2*);

  Bool rotateNode(const unsigned int, const Point2, const Point2);
  Bool translateNode(const unsigned int, const Point2, const Point2);
  Bool scaleNode(const unsigned int, const Point2, const Point2);

  void clearDispList();

  void setBorderMode(const Bool);
  Bool getBorderMode() const {return _useBorder;}
  void setBorderColor(const vector4);
  void setBorderColor(const float, const float, const float);
  void setBorderColor(const float, const float, const float, const float);
  void getBorderColor(vector4&) const;
  void setBorderWidth(const float);
  GLfloat getBorderWidth() const {return (float)_borderWidth;}

protected:
  void initialize();
  void drawBorder();

  class vfrCamera* _camera;
  class vfrDrawArea* _parent;
  GLint _viewport[4];
  Point2 _rb0, _rb1;
  vector4 _borderColor;
  GLfloat _borderWidth;
  Bool _useBorder;
};

#endif /* _VFR_SCREEN_H_ */
