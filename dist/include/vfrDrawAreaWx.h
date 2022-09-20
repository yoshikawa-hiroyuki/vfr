//
// vfrDrawAreaWx
//   Drawing Area Class with wxWidgets library
//

#ifndef _VFR_DRAW_AREA_WX_H_
#define _VFR_DRAW_AREA_WX_H_

#include "wx/defs.h"
#include "wx/dcclient.h"
#include "wx/glcanvas.h"

#include "vfrDrawArea.h"
#include <set>


class EXPORTED_CLASS vfrWxCanvas : public wxGLCanvas {
public:
  vfrWxCanvas(class vfrDrawAreaWx* pda, wxWindow *parent,
	      wxGLContext* sharedContext=NULL, const wxWindowID id=-1,
	      const wxPoint& pos=wxDefaultPosition,
	      const wxSize& size=wxDefaultSize,
	      long style = 0, const wxString& name = wxT("vfrWxCanvas"),
	      int* attr =NULL);
  ~vfrWxCanvas();

  void MakeCurrent() const {
    if ( m_context )
      wxGLCanvas::SetCurrent(*m_context);
  }
  wxGLContext* getContext() {return m_context;}

  void paintCB(wxPaintEvent& event);
  void resizeCB(wxSizeEvent& event);
  void erasebgCB(wxEraseEvent& event);
  void mouseCB(wxMouseEvent& event);
  void mouseLBUpCB(wxMouseEvent& event);
  void mouseLBDownCB(wxMouseEvent& event);
  void mouseMBUpCB(wxMouseEvent& event);
  void mouseMBDownCB(wxMouseEvent& event);
  void mouseRBUpCB(wxMouseEvent& event);
  void mouseRBDownCB(wxMouseEvent& event);
  void mouseMoveCB(wxMouseEvent& event);
  void mouseLeaveCB(wxMouseEvent& event);
  void keyinCB(wxKeyEvent& event);
  void dblClickCB(wxMouseEvent& event);
  void wheelCB(wxMouseEvent& event);

protected:
  class vfrDrawAreaWx* p_da;
  wxGLContext* m_context;
  
private:
  DECLARE_EVENT_TABLE()
};


class EXPORTED_CLASS vfrDrawAreaWx : public vfrDrawArea {
public:
  virtual ~vfrDrawAreaWx();

  //------------ derived from vfrDawArea ------------
  virtual void redraw();
  virtual void notice();
  virtual void chkNotice() {CheckNoticeAll();}
  virtual void rumor(class vfrScreen*);
  virtual Point2 getSize();
  virtual double getScaleFactor() const;

  void clearDispList();
  void drawImmediate();

  vfrWxCanvas* getCanvas() {return da_pCanvas;}

  static VFRkeyCode convKey_WXtoVFR(const int);

  static vfrDrawAreaWx*
  GetInstance(wxWindow *parent,
	      const wxPoint& pos = wxDefaultPosition,
	      const wxSize& size = wxDefaultSize,
	      long style = 0, const wxString& name = wxT("vfr"));

  static void CheckNoticeAll();

  static const wxIcon& GetVFRIcon();

protected:
  Bool      noticeFlag;
  StateType modeState;
  Point2    m_0, m_1;
  double    m_scaleFactor;

  vfrWxCanvas* da_pCanvas;

  static std::set<vfrDrawAreaWx*> s_daList;
  static wxGLContext* s_masterContext;

  void OnKeyDown(int, VFRkeyCode);
  void OnLButtonDown(Point2);
  void OnRButtonDown(Point2);
  void OnMButtonDown(Point2);
  void OnLButtonUp(int, Point2);
  void OnRButtonUp(int, Point2);
  void OnMButtonUp(int, Point2);
  void OnMouseMove(int, Point2);
  void OnMouseLeave(Point2);
  void OnMouseDClick(int, Point2);
  void OnMouseWheel(int, int, Point2);

  vfrDrawAreaWx();

  friend class vfrWxCanvas;
};

#endif /* _VFR_DRAW_AREA_SDL_H_ */
