//
// vfrDrawArea
//     Drawing-Area base class
//
#ifndef _VFR_DRAW_AREA_H_
#define _VFR_DRAW_AREA_H_

#include "vfrScreen.h"
#include "vfrCamera.h"
#include "vfrDispatch.h"

#define VFR_MAX_SCREEN_NUM  4


class EXPORTED_CLASS vfrDrawArea {
public:
  vfrDrawArea();
  virtual ~vfrDrawArea();

  virtual void redraw() = 0;
  virtual void notice() = 0;
  virtual void rumor(class vfrScreen*) = 0;
  virtual void chkNotice() = 0;
  virtual Point2 getSize() = 0;

  /* screen operation */
  enum ScrMapType {HORIZONTAL =0, VERTICAL};
  Bool addScreen(class vfrScreen*);
  Bool remScreen(class vfrScreen*);
  void remapScreen();
  int whichScreen(const int, const int);
  int setCurScreen(const int);
  void setScrBorderMode(const Bool);
  Bool getScrBorderMode() const;
  void setScreenMapType(const ScrMapType smt);
  ScrMapType getScreenMapType() const;
  vfrScreen* getScreen(const int);
  Bool setFbScale(const float fbs);
  float getFbScalle() const {return fb_scale;}

  /* rubber-box params */
  virtual void drawRubberBox(const Point2&, const Point2&);
  void setRubberBoxColor(const vector4 rbc);
  void getRubberBoxColor(vector4& rbc) const;
  void setRubberBoxLineWidth(const GLfloat rblw);
  GLfloat getRubberBoxLineWidth() const;
  void setRubberBoxLineType(const StippleType rblt);
  StippleType getRubberBoxLineType() const;

  /* event dispatcher interface */
  vfrDispatch& getDispatcher();

protected:
  /* event status type */
  enum StateType {
    WaitAny =0,
    LBPressed,
    LDraggingSolo,
    LDraggingShift,
    LDraggingCntl,
    LDraggingShiftCntl,
    RBPressed,
    RDraggingSolo,
    RDraggingShift,
    RDraggingCntl,
    RDraggingShiftCntl,
    MBPressed,
    MDraggingSolo,
    MDraggingShift,
    MDraggingCntl,
    MDraggingShiftCntl,
    MouseWheel
  };

  /* screen datas */
  vfrScreen*  da_screen[VFR_MAX_SCREEN_NUM];
  int         nScreen;
  int         cScreen;
  ScrMapType  screenMapType;
  Bool        scrBorderMode;
  float       fb_scale;

  /* rubber-box datas */
  Bool        rb_flag;
  Point2      rb_0, rb_sz;
  vector4     rb_color;
  GLfloat     rb_lineWidth;
  StippleType rb_lineType;
  void        rb_draw();

  /* event dispatcher */
  vfrDispatch da_dispatcher;
};

#endif // _VFR_DRAW_AREA_H_
