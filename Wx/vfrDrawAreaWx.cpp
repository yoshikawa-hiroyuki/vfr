//
// vfrDrawAreaWx
//   Drawing Area Class with wxWidgets library
//

#ifdef __GNUG__
#pragma implementation
#pragma interface
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif
#include "wx/version.h"

#include "vfrDrawAreaWx.h"

#ifdef __WXMAC__
#  ifdef __DARWIN__
#    include <OpenGL/glu.h>
#  else
#    include <glu.h>
#  endif
#else
#  include <GL/glu.h>
#endif

#include "vfrKeyCode.h"

#include <assert.h>

#if !defined(WINDOWS)
#define VFR_DRAW_AREA_ERR(x) \
 fprintf(stderr,"vfrDrawAreaWx: %s\n",(x));fflush(stderr);
#else
#define VFR_DRAW_AREA_ERR(x)
#endif

#define VFRWX_MEV_SHIFT (1L<<0)
#define VFRWX_MEV_CNTL  (1L<<1)
#define VFRWX_MEV_LEFTDC   (1L<<2)
#define VFRWX_MEV_RIGHTDC  (1L<<3)
#define VFRWX_MEV_MIDDLEDC (1L<<4)


/*-------------------------------------------------------------------------
  class vfrWxCanvas : public wxGLCanvas
  -------------------------------------------------------------------------*/
BEGIN_EVENT_TABLE(vfrWxCanvas, wxGLCanvas)
  EVT_SIZE(vfrWxCanvas::resizeCB)
  EVT_PAINT(vfrWxCanvas::paintCB)
  EVT_ERASE_BACKGROUND(vfrWxCanvas::erasebgCB)
  EVT_MOUSE_EVENTS(vfrWxCanvas::mouseCB)
  EVT_LEFT_UP(vfrWxCanvas::mouseLBUpCB)
  EVT_LEFT_DOWN(vfrWxCanvas::mouseLBDownCB)
  EVT_MIDDLE_UP(vfrWxCanvas::mouseMBUpCB)
  EVT_MIDDLE_DOWN(vfrWxCanvas::mouseMBDownCB)
  EVT_RIGHT_UP(vfrWxCanvas::mouseRBUpCB)
  EVT_RIGHT_DOWN(vfrWxCanvas::mouseRBDownCB)
  EVT_MOTION(vfrWxCanvas::mouseMoveCB)
  EVT_LEAVE_WINDOW(vfrWxCanvas::mouseLeaveCB)
  EVT_KEY_DOWN(vfrWxCanvas::keyinCB)
  EVT_LEFT_DCLICK(vfrWxCanvas::dblClickCB)
  EVT_MIDDLE_DCLICK(vfrWxCanvas::dblClickCB)
  EVT_RIGHT_DCLICK(vfrWxCanvas::dblClickCB)
  EVT_MOUSEWHEEL(vfrWxCanvas::wheelCB)
END_EVENT_TABLE()

vfrWxCanvas::vfrWxCanvas(vfrDrawAreaWx* pda, wxWindow *parent,
			 wxGLContext* sharedContext, const wxWindowID id,
			 const wxPoint& pos, const wxSize& size,
			 long style, const wxString& name, int* attr)
: wxGLCanvas(parent, id, attr, pos, size, style, name),
  p_da(pda), m_context(NULL)
{
  assert(p_da);
  m_context = new wxGLContext(this, sharedContext);
}

vfrWxCanvas::~vfrWxCanvas()
{
  p_da = NULL;
  if ( m_context ) {
    delete m_context;
    m_context = NULL;
  }
}

void vfrWxCanvas::paintCB(wxPaintEvent& event)
{
  /* must always be here */
  MakeCurrent();
  wxPaintDC dc(this);

  /* check and redraw */
  if ( p_da ) {
    p_da->redraw();
  }

  /* flush */
  glFlush();

  /* swap */
  SwapBuffers();
}

void vfrWxCanvas::resizeCB(wxSizeEvent& event)
{
  // this is also necessary to update the context on some platforms
  //wxGLCanvas::OnSize(event);
    
  if ( p_da ) {
    p_da->remapScreen();
    //Update();
    Refresh();
  }
}

void vfrWxCanvas::erasebgCB(wxEraseEvent& event)
{
  /* Do nothing, to avoid flashing on MSW */
}

void vfrWxCanvas::mouseCB(wxMouseEvent& event)
{
  if ( event.Entering() ) {
    SetFocus(); // need on windows to receive wheel events
  }
  event.Skip();
}
void vfrWxCanvas::mouseLBUpCB(wxMouseEvent& event)
{
  if ( p_da ) {
    int nFlag = 0;
    if ( event.m_shiftDown ) nFlag |= VFRWX_MEV_SHIFT;
    if ( event.m_controlDown ) nFlag |= VFRWX_MEV_CNTL;
    double sf = p_da->getScaleFactor();
    Point2 mp; mp.x = event.m_x*sf; mp.y = event.m_y*sf;
    p_da->OnLButtonUp(nFlag, mp);
  }
}
void vfrWxCanvas::mouseLBDownCB(wxMouseEvent& event)
{
  if ( p_da ) {
    double sf = p_da->getScaleFactor();
    Point2 mp; mp.x = event.m_x*sf; mp.y = event.m_y*sf;
    p_da->OnLButtonDown(mp);
  }
}

void vfrWxCanvas::mouseMBUpCB(wxMouseEvent& event)
{
  if ( p_da ) {
    int nFlag = 0;
    if ( event.m_shiftDown ) nFlag |= VFRWX_MEV_SHIFT;
    if ( event.m_controlDown ) nFlag |= VFRWX_MEV_CNTL;
    double sf = p_da->getScaleFactor();
    Point2 mp; mp.x = event.m_x*sf; mp.y = event.m_y*sf;
    p_da->OnMButtonUp(nFlag, mp);
  }
}
void vfrWxCanvas::mouseMBDownCB(wxMouseEvent& event)
{
  if ( p_da ) {
    double sf = p_da->getScaleFactor();
    Point2 mp; mp.x = event.m_x*sf; mp.y = event.m_y*sf;
    p_da->OnMButtonDown(mp);
  }
}

void vfrWxCanvas::mouseRBUpCB(wxMouseEvent& event)
{
  if ( p_da ) {
    int nFlag = 0;
    if ( event.m_shiftDown ) nFlag |= VFRWX_MEV_SHIFT;
    if ( event.m_controlDown ) nFlag |= VFRWX_MEV_CNTL;
    double sf = p_da->getScaleFactor();
    Point2 mp; mp.x = event.m_x*sf; mp.y = event.m_y*sf;
    p_da->OnRButtonUp(nFlag, mp);
  }
}
void vfrWxCanvas::mouseRBDownCB(wxMouseEvent& event)
{
  if ( p_da ) {
    double sf = p_da->getScaleFactor();
    Point2 mp; mp.x = event.m_x*sf; mp.y = event.m_y*sf;
    p_da->OnRButtonDown(mp);
  }
}

void vfrWxCanvas::mouseMoveCB(wxMouseEvent& event)
{
  if ( p_da ) {
    int nFlag = 0;
    if ( event.m_shiftDown ) nFlag |= VFRWX_MEV_SHIFT;
    if ( event.m_controlDown ) nFlag |= VFRWX_MEV_CNTL;
    double sf = p_da->getScaleFactor();
    Point2 mp; mp.x = event.m_x*sf; mp.y = event.m_y*sf;
    p_da->OnMouseMove(nFlag, mp);
  }
}

void vfrWxCanvas::mouseLeaveCB(wxMouseEvent& event)
{
  if ( p_da ) {
    double sf = p_da->getScaleFactor();
    Point2 mp; mp.x = event.m_x*sf; mp.y = event.m_y*sf;
    p_da->OnMouseLeave(mp);
  }
}

void vfrWxCanvas::keyinCB(wxKeyEvent& event)
{
  if ( p_da ) {
    int nFlag = 0;
    if ( event.m_shiftDown ) nFlag |= VFRWX_MEV_SHIFT;
    if ( event.m_controlDown ) nFlag |= VFRWX_MEV_CNTL;
    int kc = event.GetKeyCode();
    p_da->OnKeyDown(nFlag, vfrDrawAreaWx::convKey_WXtoVFR(kc));
  }  
}

void vfrWxCanvas::dblClickCB(wxMouseEvent& event)
{
  if ( ! event.ButtonDClick() ) return;
  if ( p_da ) {
    int nFlag = 0;
    if ( event.m_shiftDown ) nFlag |= VFRWX_MEV_SHIFT;
    if ( event.m_controlDown ) nFlag |= VFRWX_MEV_CNTL;
    if ( event.LeftDClick() ) nFlag |= VFRWX_MEV_LEFTDC;
    if ( event.RightDClick() ) nFlag |= VFRWX_MEV_RIGHTDC;
    if ( event.MiddleDClick() ) nFlag |= VFRWX_MEV_MIDDLEDC;
    double sf = p_da->getScaleFactor();
    Point2 mp; mp.x = event.m_x*sf; mp.y = event.m_y*sf;
    p_da->OnMouseDClick(nFlag, mp);
  }
}

void vfrWxCanvas::wheelCB(wxMouseEvent& event)
{
  if ( p_da ) {
    int nFlag = 0;
    if ( event.m_shiftDown ) nFlag |= VFRWX_MEV_SHIFT;
    if ( event.m_controlDown ) nFlag |= VFRWX_MEV_CNTL;
    int wr = event.GetWheelRotation() > 0 ? 10 : -10;
    double sf = p_da->getScaleFactor();
    Point2 mp; mp.x = event.m_x*sf; mp.y = event.m_y*sf;
    p_da->OnMouseWheel(nFlag, wr, mp);
  }  
}



/*-------------------------------------------------------------------------
  class vfrDrawAreaWx : public vfrDrawArea
  -------------------------------------------------------------------------*/

///// STATIC MEMBERS /////
std::set<vfrDrawAreaWx*> vfrDrawAreaWx::s_daList;
wxGLContext* vfrDrawAreaWx::s_masterContext = NULL;


///// STATIC METHOD /////
vfrDrawAreaWx*
vfrDrawAreaWx::GetInstance(wxWindow *parent,
			   const wxPoint& pos, const wxSize& size,
			   long style, const wxString& name)
{
  vfrDrawAreaWx* pda = new vfrDrawAreaWx();
  assert(pda);

  int gl_attrib[] = {WX_GL_RGBA, WX_GL_MIN_RED, 8, WX_GL_MIN_GREEN, 8,
                     WX_GL_MIN_BLUE, 8, WX_GL_MIN_ALPHA, 8,
                     WX_GL_DEPTH_SIZE, 1,
                     WX_GL_DOUBLEBUFFER,
                     0};

  vfrWxCanvas* pCanvas = new vfrWxCanvas(pda, parent, s_masterContext, -1,
					 pos, size, style, name, gl_attrib);
  if ( ! s_masterContext ) {
    s_masterContext = pCanvas->getContext();
  }
  pda->da_pCanvas = pCanvas;
  if ( wxMAJOR_VERSION >= 3 && wxMINOR_VERSION >= 1 )
    pda->m_scaleFactor = pCanvas->GetContentScaleFactor();

  s_daList.insert(pda);
  return pda;
}


vfrDrawAreaWx::vfrDrawAreaWx() :
  noticeFlag(FALSE), modeState(WaitAny), da_pCanvas(NULL),
  m_scaleFactor(1.0)
{
}

vfrDrawAreaWx::~vfrDrawAreaWx()
{
  if ( da_pCanvas ) {
    delete da_pCanvas;
    da_pCanvas = NULL;
  }
  std::set<vfrDrawAreaWx*>::iterator it
    = s_daList.find(this);
  if ( it != s_daList.end() )
    s_daList.erase(it);
}


Point2
vfrDrawAreaWx::getSize()
{
  Point2 size;
  if ( da_pCanvas ) {
    wxSize wsz = da_pCanvas->GetSize();
    wsz *= m_scaleFactor;
    size.x = wsz.GetWidth(); size.y = wsz.GetHeight();
  }
  return size;
}

double
vfrDrawAreaWx::getScaleFactor() const
{
  return m_scaleFactor;
}


void
vfrDrawAreaWx::redraw()
{
  GLenum error;
  noticeFlag = FALSE;

  //======== Canvas ========
  if ( ! da_pCanvas ) return;

  //======== Rendering ========
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  for ( register int i = 0; i < nScreen; i++ ) {
    da_screen[i]->redraw();
  }

  //======== Rubber Box ========
  rb_draw();

  //======== Post rendering ========
  while ( (error = glGetError()) != GL_NO_ERROR )
    VFR_DRAW_AREA_ERR(gluErrorString(error));
}

void
vfrDrawAreaWx::notice()
{
  noticeFlag = TRUE;
}

//// STATIC METHOD ////
void
vfrDrawAreaWx::CheckNoticeAll()
{
  std::set<vfrDrawAreaWx*>::iterator it;
  for ( it = s_daList.begin(); it != s_daList.end(); it++ ) {
    if ( *it && (*it)->noticeFlag ) {
      vfrWxCanvas* pCanvas = (*it)->getCanvas();
      if ( pCanvas ) pCanvas->Refresh(false);
    }
  } // end of for(it)
}

void
vfrDrawAreaWx::rumor(vfrScreen* s)
{
  remScreen(s);
}

void
vfrDrawAreaWx::clearDispList()
{
  for ( register int i = 0; i < VFR_MAX_SCREEN_NUM; i++ ) {
    if ( da_screen[i] )
      da_screen[i]->clearDispList();
  }
}


void
vfrDrawAreaWx::drawImmediate()
{
  Bool bkIM = vfrNode::isImmediateMode();
  if ( ! bkIM ) vfrNode::setImmediateMode(TRUE);
  redraw();
  if ( ! bkIM ) vfrNode::setImmediateMode(FALSE);
}


///// STATIC METHOD /////
VFRkeyCode
vfrDrawAreaWx::convKey_WXtoVFR(const int wxKey)
{
  VFRkeyCode ret = 0;

#define X_CONV_KEY(s, x) case (s): ret=(x); break

  switch ( wxKey ) {
    X_CONV_KEY(WXK_BACK, VFRKC_BACKSPACE);
    X_CONV_KEY(WXK_TAB, VFRKC_TAB);
    X_CONV_KEY(WXK_CLEAR, VFRKC_CLEAR);
    X_CONV_KEY(WXK_RETURN, VFRKC_RETURN);
    X_CONV_KEY(WXK_PAUSE, VFRKC_PAUSE);
    X_CONV_KEY(WXK_ESCAPE, VFRKC_ESCAPE);
    X_CONV_KEY(WXK_SPACE, VFRKC_SPACE);
    X_CONV_KEY('!', VFRKC_EXCLAIM);
    X_CONV_KEY('\"', VFRKC_QUOTEDBL);
    X_CONV_KEY('#', VFRKC_HASH);
    X_CONV_KEY('$', VFRKC_DOLLAR);
    X_CONV_KEY('&', VFRKC_AMPERSAND);
    X_CONV_KEY('\'', VFRKC_QUOTE);
    X_CONV_KEY('(', VFRKC_LEFTPAREN);
    X_CONV_KEY(')', VFRKC_RIGHTPAREN);
    X_CONV_KEY('*', VFRKC_ASTERISK);
    X_CONV_KEY('+', VFRKC_PLUS);
    X_CONV_KEY(',', VFRKC_COMMA);
    X_CONV_KEY('-', VFRKC_MINUS);
    X_CONV_KEY('.', VFRKC_PERIOD);
    X_CONV_KEY('/', VFRKC_SLASH);
    X_CONV_KEY('0', VFRKC_0);
    X_CONV_KEY('1', VFRKC_1);
    X_CONV_KEY('2', VFRKC_2);
    X_CONV_KEY('3', VFRKC_3);
    X_CONV_KEY('4', VFRKC_4);
    X_CONV_KEY('5', VFRKC_5);
    X_CONV_KEY('6', VFRKC_6);
    X_CONV_KEY('7', VFRKC_7);
    X_CONV_KEY('8', VFRKC_8);
    X_CONV_KEY('9', VFRKC_9);
    X_CONV_KEY(':', VFRKC_COLON);
    X_CONV_KEY(';', VFRKC_SEMICOLON);
    X_CONV_KEY('<', VFRKC_LESS);
    X_CONV_KEY('=', VFRKC_EQUALS);
    X_CONV_KEY('>', VFRKC_GREATER);
    X_CONV_KEY('?', VFRKC_QUESTION);
    X_CONV_KEY('@', VFRKC_AT);
    X_CONV_KEY('[', VFRKC_LEFTBRACKET);
    X_CONV_KEY('\\', VFRKC_BACKSLASH);
    X_CONV_KEY(']', VFRKC_RIGHTBRACKET);
    X_CONV_KEY('^', VFRKC_CARET);
    X_CONV_KEY('_', VFRKC_UNDERSCORE);
    X_CONV_KEY('`', VFRKC_BACKQUOTE);
    X_CONV_KEY('a', VFRKC_a);
    X_CONV_KEY('b', VFRKC_b);
    X_CONV_KEY('c', VFRKC_c);
    X_CONV_KEY('d', VFRKC_d);
    X_CONV_KEY('e', VFRKC_e);
    X_CONV_KEY('f', VFRKC_f);
    X_CONV_KEY('g', VFRKC_g);
    X_CONV_KEY('h', VFRKC_h);
    X_CONV_KEY('i', VFRKC_i);
    X_CONV_KEY('j', VFRKC_j);
    X_CONV_KEY('k', VFRKC_k);
    X_CONV_KEY('l', VFRKC_l);
    X_CONV_KEY('m', VFRKC_m);
    X_CONV_KEY('n', VFRKC_n);
    X_CONV_KEY('o', VFRKC_o);
    X_CONV_KEY('p', VFRKC_p);
    X_CONV_KEY('q', VFRKC_q);
    X_CONV_KEY('r', VFRKC_r);
    X_CONV_KEY('s', VFRKC_s);
    X_CONV_KEY('t', VFRKC_t);
    X_CONV_KEY('u', VFRKC_u);
    X_CONV_KEY('v', VFRKC_v);
    X_CONV_KEY('w', VFRKC_w);
    X_CONV_KEY('x', VFRKC_x);
    X_CONV_KEY('y', VFRKC_y);
    X_CONV_KEY('z', VFRKC_z);
    X_CONV_KEY('A', VFRKC_A);
    X_CONV_KEY('B', VFRKC_B);
    X_CONV_KEY('C', VFRKC_C);
    X_CONV_KEY('D', VFRKC_D);
    X_CONV_KEY('E', VFRKC_E);
    X_CONV_KEY('F', VFRKC_F);
    X_CONV_KEY('G', VFRKC_F);
    X_CONV_KEY('H', VFRKC_H);
    X_CONV_KEY('I', VFRKC_I);
    X_CONV_KEY('J', VFRKC_J);
    X_CONV_KEY('K', VFRKC_K);
    X_CONV_KEY('L', VFRKC_L);
    X_CONV_KEY('M', VFRKC_M);
    X_CONV_KEY('N', VFRKC_N);
    X_CONV_KEY('O', VFRKC_O);
    X_CONV_KEY('P', VFRKC_P);
    X_CONV_KEY('Q', VFRKC_Q);
    X_CONV_KEY('R', VFRKC_R);
    X_CONV_KEY('S', VFRKC_S);
    X_CONV_KEY('T', VFRKC_T);
    X_CONV_KEY('U', VFRKC_U);
    X_CONV_KEY('V', VFRKC_V);
    X_CONV_KEY('W', VFRKC_W);
    X_CONV_KEY('X', VFRKC_X);
    X_CONV_KEY('Y', VFRKC_Y);
    X_CONV_KEY('Z', VFRKC_Z);
    X_CONV_KEY(WXK_DELETE, VFRKC_DELETE);
    X_CONV_KEY(WXK_UP, VFRKC_UP);
    X_CONV_KEY(WXK_DOWN, VFRKC_DOWN);
    X_CONV_KEY(WXK_RIGHT, VFRKC_RIGHT);
    X_CONV_KEY(WXK_LEFT, VFRKC_LEFT);
    X_CONV_KEY(WXK_INSERT, VFRKC_INSERT);
    X_CONV_KEY(WXK_HOME, VFRKC_HOME);
    X_CONV_KEY(WXK_END, VFRKC_END);
    X_CONV_KEY(WXK_PAGEUP, VFRKC_PAGEUP);
    X_CONV_KEY(WXK_PAGEDOWN, VFRKC_PAGEDOWN);
    X_CONV_KEY(WXK_F1, VFRKC_F1);
    X_CONV_KEY(WXK_F2, VFRKC_F2);
    X_CONV_KEY(WXK_F3, VFRKC_F3);
    X_CONV_KEY(WXK_F4, VFRKC_F4);
    X_CONV_KEY(WXK_F5, VFRKC_F5);
    X_CONV_KEY(WXK_F6, VFRKC_F6);
    X_CONV_KEY(WXK_F7, VFRKC_F7);
    X_CONV_KEY(WXK_F8, VFRKC_F8);
    X_CONV_KEY(WXK_F9, VFRKC_F9);
    X_CONV_KEY(WXK_F10, VFRKC_F10);
    X_CONV_KEY(WXK_F11, VFRKC_F11);
    X_CONV_KEY(WXK_F12, VFRKC_F12);
    X_CONV_KEY(WXK_PRINT, VFRKC_PRINT);
  }
#undef X_CONV_KEY

  return ret;
}

void
vfrDrawAreaWx::OnKeyDown(int nFlags, VFRkeyCode nChar)
{
  vfrDispatch& dp = vfrDispatch::instance(*this);
  vfrEvent& event = dp.KeyIn();
  event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
  event.setKey(nChar);
  getCanvas()->MakeCurrent();

  vfrEvKeyIn* pEvKeyIn = dynamic_cast<vfrEvKeyIn*>(&event);
  if ( pEvKeyIn ) {
    pEvKeyIn->setKeyShifted(nFlags & VFRWX_MEV_SHIFT);
    pEvKeyIn->setKeyControlled(nFlags & VFRWX_MEV_CNTL);
  }

  event.execAction();
  chkNotice();
}

void
vfrDrawAreaWx::OnLButtonDown(Point2 point)
{
  m_0.x = m_1.x = point.x;
  m_0.y = m_1.y = point.y;
  setCurScreen(whichScreen(point.x, point.y));
  modeState = LBPressed;
}

void
vfrDrawAreaWx::OnRButtonDown(Point2 point)
{
  m_0.x = m_1.x = point.x;
  m_0.y = m_1.y = point.y;
  setCurScreen(whichScreen(point.x, point.y));
  modeState = RBPressed;
}

void
vfrDrawAreaWx::OnMButtonDown(Point2 point)
{
  m_0.x = m_1.x = point.x;
  m_0.y = m_1.y = point.y;
  setCurScreen(whichScreen(point.x, point.y));
  modeState = MBPressed;
}

void
vfrDrawAreaWx::OnLButtonUp(int nFlags, Point2 point)
{
  vfrDispatch& dp = vfrDispatch::instance(*this);
  Point2 mv;
  Point2 cs = getSize();
  getCanvas()->MakeCurrent();

  switch ( modeState ) {
  case LBPressed:
    if ( (nFlags & VFRWX_MEV_SHIFT) && (nFlags & VFRWX_MEV_CNTL) ) {
      // SCClick
      modeState = WaitAny;
      vfrEvent& event = dp.SCClick();
      setCurScreen(whichScreen(point.x, point.y));
      event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
      event.setMPoint(point);
      mv.x = mv.y = 0;
      event.setMMove(mv);
      event.execAction();
    }
    else if ( nFlags & VFRWX_MEV_SHIFT ) {
      // SClick
      modeState = WaitAny;
      vfrEvent& event = dp.SClick();
      setCurScreen(whichScreen(point.x, point.y));
      event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
      event.setMPoint(point);
      mv.x = mv.y = 0;
      event.setMMove(mv);
      event.execAction();
    }
    else if ( nFlags & VFRWX_MEV_CNTL ) {
      // CClick
      modeState = WaitAny;
      vfrEvent& event = dp.CClick();
      setCurScreen(whichScreen(point.x, point.y));
      event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
      event.setMPoint(point);
      mv.x = mv.y = 0;
      event.setMMove(mv);
      event.execAction();
    }
    else {
      // Click
      modeState = WaitAny;
      vfrEvent& event = dp.Click();
      setCurScreen(whichScreen(point.x, point.y));
      event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
      event.setMPoint(point);
      mv.x = mv.y = 0;
      event.setMMove(mv);
      event.execAction();
    }
    break;

  case LDraggingSolo:
    { // DragEnd
      modeState = WaitAny;
      vfrEvent& event = dp.DragEnd();
      event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
      event.setMPoint(point);
      m_1 = point;
      mv.x = m_1.x - m_0.x; mv.y = m_1.y - m_0.y;
      event.setMMove(mv);
      event.execAction();
    }
    break;

  case LDraggingShift:
    { // SDragEnd
      modeState = WaitAny;
      vfrEvent& event = dp.SDragEnd();
      event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
      event.setMPoint(point);
      m_1 = point;
      mv.x = m_1.x - m_0.x; mv.y = m_1.y - m_0.y;
      event.setMMove(mv);
      event.execAction();
    }
    break;

  case LDraggingCntl:
    { // CDragEnd
      modeState = WaitAny;
      vfrEvent& event = dp.CDragEnd();
      event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
      event.setMPoint(point);
      m_1 = point;
      mv.x = m_1.x - m_0.x; mv.y = m_1.y - m_0.y;
      event.setMMove(mv);
      event.execAction();
    }
    break;

  case LDraggingShiftCntl:
    { // SCDragEnd
      modeState = WaitAny;
      vfrEvent& event = dp.SCDragEnd();
      event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
      event.setMPoint(point);
      m_1 = point;
      mv.x = m_1.x - m_0.x; mv.y = m_1.y - m_0.y;
      event.setMMove(mv);
      event.execAction();
    }
    break;

  default:
    modeState = WaitAny;
  }
  chkNotice();
}

void
vfrDrawAreaWx::OnRButtonUp(int nFlags, Point2 point)
{
  vfrDispatch& dp = vfrDispatch::instance(*this);
  Point2 mv;
  Point2 cs = getSize();
  getCanvas()->MakeCurrent();

  switch ( modeState ) {
  case RBPressed:
    if ( (nFlags & VFRWX_MEV_SHIFT) && (nFlags & VFRWX_MEV_CNTL) ) {
      // RSCClick
      modeState = WaitAny;
      vfrEvent& event = dp.RSCClick();
      setCurScreen(whichScreen(point.x, point.y));
      event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
      event.setMPoint(point);
      mv.x = mv.y = 0;
      event.setMMove(mv);
      event.execAction();
    }
    else if ( nFlags & VFRWX_MEV_SHIFT ) {
      // RSClick
      modeState = WaitAny;
      vfrEvent& event = dp.RSClick();
      setCurScreen(whichScreen(point.x, point.y));
      event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
      event.setMPoint(point);
      mv.x = mv.y = 0;
      event.setMMove(mv);
      event.execAction();
    }
    else if ( nFlags & VFRWX_MEV_CNTL ) {
      // RCClick
      modeState = WaitAny;
      vfrEvent& event = dp.RCClick();
      setCurScreen(whichScreen(point.x, point.y));
      event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
      event.setMPoint(point);
      mv.x = mv.y = 0;
      event.setMMove(mv);
      event.execAction();
    }
    else {
      // RClick
      modeState = WaitAny;
      vfrEvent& event = dp.RClick();
      setCurScreen(whichScreen(point.x, point.y));
      event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
      event.setMPoint(point);
      mv.x = mv.y = 0;
      event.setMMove(mv);
      event.execAction();
    }
    break;

  case RDraggingSolo:
    { // RDragEnd
      modeState = WaitAny;
      vfrEvent& event = dp.RDragEnd();
      event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
      event.setMPoint(point);
      m_1 = point;
      mv.x = m_1.x - m_0.x; mv.y = m_1.y - m_0.y;
      event.setMMove(mv);
      event.execAction();
    }
    break;

  case RDraggingShift:
    { // RSDragEnd
      modeState = WaitAny;
      vfrEvent& event = dp.RSDragEnd();
      event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
      event.setMPoint(point);
      m_1 = point;
      mv.x = m_1.x - m_0.x; mv.y = m_1.y - m_0.y;
      event.setMMove(mv);
      event.execAction();
    }
    break;

  case RDraggingCntl:
    { // RCDragEnd
      modeState = WaitAny;
      vfrEvent& event = dp.RCDragEnd();
      event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
      event.setMPoint(point);
      m_1 = point;
      mv.x = m_1.x - m_0.x; mv.y = m_1.y - m_0.y;
      event.setMMove(mv);
      event.execAction();
    }
    break;

  case RDraggingShiftCntl:
    { // RSCDragEnd
      modeState = WaitAny;
      vfrEvent& event = dp.RSCDragEnd();
      event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
      event.setMPoint(point);
      m_1 = point;
      mv.x = m_1.x - m_0.x; mv.y = m_1.y - m_0.y;
      event.setMMove(mv);
      event.execAction();
    }
    break;

  default:
    modeState = WaitAny;
  }
  chkNotice();
}

void
vfrDrawAreaWx::OnMButtonUp(int nFlags, Point2 point)
{
  vfrDispatch& dp = vfrDispatch::instance(*this);
  Point2 mv;
  Point2 cs = getSize();
  getCanvas()->MakeCurrent();

  switch ( modeState ) {
  case MBPressed:
    if ( (nFlags & VFRWX_MEV_SHIFT) && (nFlags & VFRWX_MEV_CNTL) ) {
      // MSCClick
      modeState = WaitAny;
      vfrEvent& event = dp.MSCClick();
      setCurScreen(whichScreen(point.x, point.y));
      event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
      event.setMPoint(point);
      mv.x = mv.y = 0;
      event.setMMove(mv);
      event.execAction();
    }
    else if ( nFlags & VFRWX_MEV_SHIFT ) {
      // MSClick
      modeState = WaitAny;
      vfrEvent& event = dp.MSClick();
      setCurScreen(whichScreen(point.x, point.y));
      event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
      event.setMPoint(point);
      mv.x = mv.y = 0;
      event.setMMove(mv);
      event.execAction();
    }
    else if ( nFlags & VFRWX_MEV_CNTL ) {
      // MCClick
      modeState = WaitAny;
      vfrEvent& event = dp.MCClick();
      setCurScreen(whichScreen(point.x, point.y));
      event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
      event.setMPoint(point);
      mv.x = mv.y = 0;
      event.setMMove(mv);
      event.execAction();
    }
    else {
      // MClick
      modeState = WaitAny;
      vfrEvent& event = dp.MClick();
      setCurScreen(whichScreen(point.x, point.y));
      event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
      event.setMPoint(point);
      mv.x = mv.y = 0;
      event.setMMove(mv);
      event.execAction();
    }
    break;

  case MDraggingSolo:
    { // MDragEnd
      modeState = WaitAny;
      vfrEvent& event = dp.MDragEnd();
      event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
      event.setMPoint(point);
      m_1 = point;
      mv.x = m_1.x - m_0.x; mv.y = m_1.y - m_0.y;
      event.setMMove(mv);
      event.execAction();
    }
    break;

  case MDraggingShift:
    { // MSDragEnd
      modeState = WaitAny;
      vfrEvent& event = dp.MSDragEnd();
      event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
      event.setMPoint(point);
      m_1 = point;
      mv.x = m_1.x - m_0.x; mv.y = m_1.y - m_0.y;
      event.setMMove(mv);
      event.execAction();
    }
    break;

  case MDraggingCntl:
    { // MCDragEnd
      modeState = WaitAny;
      vfrEvent& event = dp.MCDragEnd();
      event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
      event.setMPoint(point);
      m_1 = point;
      mv.x = m_1.x - m_0.x; mv.y = m_1.y - m_0.y;
      event.setMMove(mv);
      event.execAction();
    }
    break;

  case MDraggingShiftCntl:
    { // MSCDragEnd
      modeState = WaitAny;
      vfrEvent& event = dp.MSCDragEnd();
      event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
      event.setMPoint(point);
      m_1 = point;
      mv.x = m_1.x - m_0.x; mv.y = m_1.y - m_0.y;
      event.setMMove(mv);
      event.execAction();
    }
    break;

  default:
    modeState = WaitAny;
  }
  chkNotice();
}

void
vfrDrawAreaWx::OnMouseMove(int nFlags, Point2 point)
{
  vfrDispatch& dp = vfrDispatch::instance(*this);
  getCanvas()->MakeCurrent();
  
  Point2 mv;
  switch ( modeState ) {
  case WaitAny:
    break;

  // ---------------------- LeftButton ---------------------- //
  case LBPressed:
    if ( (nFlags & VFRWX_MEV_SHIFT) && (nFlags & VFRWX_MEV_CNTL) ) {
      // SCDragStart
      modeState = LDraggingShiftCntl;
      m_1 = point;
      vfrEvent& event = dp.SCDragStart();
      setCurScreen(whichScreen(m_1.x, m_1.y));
      event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
      event.setMPoint(m_0);
      mv.x = m_1.x - m_0.x;
      mv.y = m_1.y - m_0.y;
      event.setMMove(mv);
      event.execAction();
    }
    else if ( nFlags & VFRWX_MEV_SHIFT ) {
       // SDragStart
      modeState = LDraggingShift;
      m_1 = point;
      vfrEvent& event = dp.SDragStart();
      setCurScreen(whichScreen(m_1.x, m_1.y));
      event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
      event.setMPoint(m_0);
      mv.x = m_1.x - m_0.x;
      mv.y = m_1.y - m_0.y;
      event.setMMove(mv);
      event.execAction();
    }
    else if ( nFlags & VFRWX_MEV_CNTL ) {
      // CDragStart
      modeState = LDraggingCntl;
      m_1 = point;
      vfrEvent& event = dp.CDragStart();
      setCurScreen(whichScreen(m_1.x, m_1.y));
      event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
      event.setMPoint(m_0);
      mv.x = m_1.x - m_0.x;
      mv.y = m_1.y - m_0.y;
      event.setMMove(mv);
      event.execAction();
    }
    else {
      // DragStart
      modeState = LDraggingSolo;
      m_1 = point;
      vfrEvent& event = dp.DragStart();
      setCurScreen(whichScreen(m_1.x, m_1.y));
      event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
      event.setMPoint(m_0);
      mv.x = m_1.x - m_0.x;
      mv.y = m_1.y - m_0.y;
      event.setMMove(mv);
      event.execAction();
    }
    break;

  case LDraggingSolo:
    m_0 = m_1;
    m_1 = point;
    {
      // Drag
      vfrEvent& event = dp.Drag();
      event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
      event.setMPoint(point);
      mv.x = m_1.x - m_0.x;
      mv.y = m_1.y - m_0.y;
      event.setMMove(mv);
      event.execAction();
    }
    break;

  case LDraggingShift:
    m_0 = m_1;
    m_1 = point;
    if ( nFlags & VFRWX_MEV_SHIFT ) {
      // SDrag
      vfrEvent& event = dp.SDrag();
      event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
      event.setMPoint(point);
      mv.x = m_1.x - m_0.x;
      mv.y = m_1.y - m_0.y;
      event.setMMove(mv);
      event.execAction();
    }
    else {
      // SDragEnd
      modeState = WaitAny;
      vfrEvent& event = dp.SDragEnd();
      event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
      event.setMPoint(point);
      mv.x = m_1.x - m_0.x; mv.y = m_1.y - m_0.y;
      event.setMMove(mv);
      event.execAction();
    }
    break;

  case LDraggingCntl:
    m_0 = m_1;
    m_1 = point;
    if ( nFlags & VFRWX_MEV_CNTL ) {
      // CDrag
      vfrEvent& event = dp.CDrag();
      event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
      event.setMPoint(point);
      mv.x = m_1.x - m_0.x;
      mv.y = m_1.y - m_0.y;
      event.setMMove(mv);
      event.execAction();
    }
    else {
      // CDragEnd
      modeState = WaitAny;
      vfrEvent& event = dp.CDragEnd();
      event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
      event.setMPoint(point);
      mv.x = m_1.x - m_0.x; mv.y = m_1.y - m_0.y;
      event.setMMove(mv);
      event.execAction();
    }
    break;

  case LDraggingShiftCntl:
    m_0 = m_1;
    m_1 = point;
    if ( (nFlags & VFRWX_MEV_SHIFT) && (nFlags & VFRWX_MEV_CNTL) ) {
      // SCDrag
      vfrEvent& event = dp.SCDrag();
      event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
      event.setMPoint(point);
      mv.x = m_1.x - m_0.x;
      mv.y = m_1.y - m_0.y;
      event.setMMove(mv);
      event.execAction();
    }
    else {
      // SCDragEnd
      modeState = WaitAny;
      vfrEvent& event = dp.SCDragEnd();
      event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
      event.setMPoint(point);
      mv.x = m_1.x - m_0.x; mv.y = m_1.y - m_0.y;
      event.setMMove(mv);
      event.execAction();
    }
    break;

  // ---------------------- RightButton ---------------------- //
  case RBPressed:
    if ( (nFlags & VFRWX_MEV_SHIFT) && (nFlags & VFRWX_MEV_CNTL) ) {
      // RSCDragStart
      modeState = RDraggingShiftCntl;
      m_1 = point;
      vfrEvent& event = dp.RSCDragStart();
      setCurScreen(whichScreen(m_1.x, m_1.y));
      event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
      event.setMPoint(m_0);
      mv.x = m_1.x - m_0.x;
      mv.y = m_1.y - m_0.y;
      event.setMMove(mv);
      event.execAction();
    }
    else if ( nFlags & VFRWX_MEV_SHIFT ) {
      // RSDragStart
      modeState = RDraggingShift;
      m_1 = point;
      vfrEvent& event = dp.RSDragStart();
      setCurScreen(whichScreen(m_1.x, m_1.y));
      event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
      event.setMPoint(m_0);
      mv.x = m_1.x - m_0.x;
      mv.y = m_1.y - m_0.y;
      event.setMMove(mv);
      event.execAction();
    }
    else if ( nFlags & VFRWX_MEV_CNTL ) {
      // RCDragStart
      modeState = RDraggingCntl;
      m_1 = point;
      vfrEvent& event = dp.RCDragStart();
      setCurScreen(whichScreen(m_1.x, m_1.y));
      event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
      event.setMPoint(m_0);
      mv.x = m_1.x - m_0.x;
      mv.y = m_1.y - m_0.y;
      event.setMMove(mv);
      event.execAction();
    }
    else {
      // RDragStart
      modeState = RDraggingSolo;
      m_1 = point;
      vfrEvent& event = dp.RDragStart();
      setCurScreen(whichScreen(m_1.x, m_1.y));
      event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
      event.setMPoint(m_0);
      mv.x = m_1.x - m_0.x;
      mv.y = m_1.y - m_0.y;
      event.setMMove(mv);
      event.execAction();
    }
    break;

  case RDraggingSolo:
    m_0 = m_1;
    m_1 = point;
    {
      // RDrag
      vfrEvent& event = dp.RDrag();
      event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
      event.setMPoint(point);
      mv.x = m_1.x - m_0.x;
      mv.y = m_1.y - m_0.y;
      event.setMMove(mv);
      event.execAction();
    }
    break;

  case RDraggingShift:
    m_0 = m_1;
    m_1 = point;
    if ( nFlags & VFRWX_MEV_SHIFT ) {
      // RSDrag
      vfrEvent& event = dp.RSDrag();
      event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
      event.setMPoint(point);
      mv.x = m_1.x - m_0.x;
      mv.y = m_1.y - m_0.y;
      event.setMMove(mv);
      event.execAction();
    }
    else {
      // RSDragEnd
      modeState = WaitAny;
      vfrEvent& event = dp.RSDragEnd();
      event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
      event.setMPoint(point);
      mv.x = m_1.x - m_0.x; mv.y = m_1.y - m_0.y;
      event.setMMove(mv);
      event.execAction();
    }
    break;

  case RDraggingCntl:
    m_0 = m_1;
    m_1 = point;
    if ( nFlags & VFRWX_MEV_CNTL ) {
      // RCDrag
      vfrEvent& event = dp.RCDrag();
      event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
      event.setMPoint(point);
      mv.x = m_1.x - m_0.x;
      mv.y = m_1.y - m_0.y;
      event.setMMove(mv);
      event.execAction();
    }
    else {
      // RCDragEnd
      modeState = WaitAny;
      vfrEvent& event = dp.RCDragEnd();
      event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
      event.setMPoint(point);
      mv.x = m_1.x - m_0.x; mv.y = m_1.y - m_0.y;
      event.setMMove(mv);
      event.execAction();
    }
    break;

  case RDraggingShiftCntl:
    m_0 = m_1;
    m_1 = point;
    if ( (nFlags & VFRWX_MEV_SHIFT) && (nFlags & VFRWX_MEV_CNTL) ) {
      // RSCDrag
      vfrEvent& event = dp.RSCDrag();
      event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
      event.setMPoint(point);
      mv.x = m_1.x - m_0.x;
      mv.y = m_1.y - m_0.y;
      event.setMMove(mv);
      event.execAction();
    }
    else {
      // RSCDragEnd
      modeState = WaitAny;
      vfrEvent& event = dp.RSCDragEnd();
      event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
      event.setMPoint(point);
      mv.x = m_1.x - m_0.x; mv.y = m_1.y - m_0.y;
      event.setMMove(mv);
      event.execAction();
    }
    break;

  // ---------------------- MiddleButton ---------------------- //
  case MBPressed:
    if ( (nFlags & VFRWX_MEV_SHIFT) && (nFlags & VFRWX_MEV_CNTL) ) {
      // MSCDragStart
      modeState = MDraggingShiftCntl;
      m_1 = point;
      vfrEvent& event = dp.MSCDragStart();
      setCurScreen(whichScreen(m_1.x, m_1.y));
      event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
      event.setMPoint(m_0);
      mv.x = m_1.x - m_0.x;
      mv.y = m_1.y - m_0.y;
      event.setMMove(mv);
      event.execAction();
    }
    else if ( nFlags & VFRWX_MEV_SHIFT ) {
      // MSDragStart
      modeState = MDraggingShift;
      m_1 = point;
      vfrEvent& event = dp.MSDragStart();
      setCurScreen(whichScreen(m_1.x, m_1.y));
      event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
      event.setMPoint(m_0);
      mv.x = m_1.x - m_0.x;
      mv.y = m_1.y - m_0.y;
      event.setMMove(mv);
      event.execAction();
    }
    else if ( nFlags & VFRWX_MEV_CNTL ) {
      // MCDragStart
      modeState = MDraggingCntl;
      m_1 = point;
      vfrEvent& event = dp.MCDragStart();
      setCurScreen(whichScreen(m_1.x, m_1.y));
      event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
      event.setMPoint(m_0);
      mv.x = m_1.x - m_0.x;
      mv.y = m_1.y - m_0.y;
      event.setMMove(mv);
      event.execAction();
    }
    else {
      // MDragStart
      modeState = MDraggingSolo;
      m_1 = point;
      vfrEvent& event = dp.MDragStart();
      setCurScreen(whichScreen(m_1.x, m_1.y));
      event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
      event.setMPoint(m_0);
      mv.x = m_1.x - m_0.x;
      mv.y = m_1.y - m_0.y;
      event.setMMove(mv);
      event.execAction();
    }
    break;

  case MDraggingSolo:
    m_0 = m_1;
    m_1 = point;
    {
      // MDrag
      vfrEvent& event = dp.MDrag();
      event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
      event.setMPoint(point);
      mv.x = m_1.x - m_0.x;
      mv.y = m_1.y - m_0.y;
      event.setMMove(mv);
      event.execAction();
    }
    break;

  case MDraggingShift:
    m_0 = m_1;
    m_1 = point;
    if ( nFlags & VFRWX_MEV_SHIFT ) {
      // MSDrag
      vfrEvent& event = dp.MSDrag();
      event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
      event.setMPoint(point);
      mv.x = m_1.x - m_0.x;
      mv.y = m_1.y - m_0.y;
      event.setMMove(mv);
      event.execAction();
    }
    else {
      // MSDragEnd
      modeState = WaitAny;
      vfrEvent& event = dp.MSDragEnd();
      event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
      event.setMPoint(point);
      mv.x = m_1.x - m_0.x; mv.y = m_1.y - m_0.y;
      event.setMMove(mv);
      event.execAction();
    }
    break;

  case MDraggingCntl:
    m_0 = m_1;
    m_1 = point;
    if ( nFlags & VFRWX_MEV_CNTL ) {
      // MCDrag
      vfrEvent& event = dp.MCDrag();
      event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
      event.setMPoint(point);
      mv.x = m_1.x - m_0.x;
      mv.y = m_1.y - m_0.y;
      event.setMMove(mv);
      event.execAction();
    }
    else {
      // MCDragEnd
      modeState = WaitAny;
      vfrEvent& event = dp.MCDragEnd();
      event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
      event.setMPoint(point);
      mv.x = m_1.x - m_0.x; mv.y = m_1.y - m_0.y;
      event.setMMove(mv);
      event.execAction();
    }
    break;

  case MDraggingShiftCntl:
    m_0 = m_1;
    m_1 = point;
    if ( (nFlags & VFRWX_MEV_SHIFT) && (nFlags & VFRWX_MEV_CNTL) ) {
      // MSCDrag
      vfrEvent& event = dp.MSCDrag();
      event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
      event.setMPoint(point);
      mv.x = m_1.x - m_0.x;
      mv.y = m_1.y - m_0.y;
      event.setMMove(mv);
      event.execAction();
    }
    else {
      // MSCDragEnd
      modeState = WaitAny;
      vfrEvent& event = dp.MSCDragEnd();
      event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
      event.setMPoint(point);
      mv.x = m_1.x - m_0.x; mv.y = m_1.y - m_0.y;
      event.setMMove(mv);
      event.execAction();
    }
    break;

  }

  chkNotice();
}

void
vfrDrawAreaWx::OnMouseLeave(Point2 point)
{
  vfrDispatch& dp = vfrDispatch::instance(*this);
  getCanvas()->MakeCurrent();
  
  Point2 mv;
  switch ( modeState ) {
  case WaitAny:
    break;

  // ---------------------- LeftButton ---------------------- //
  case LDraggingSolo:
    { // DragEnd
      modeState = WaitAny;
      vfrEvent& event = dp.DragEnd();
      event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
      event.setMPoint(point);
      m_1 = point;
      mv.x = m_1.x - m_0.x; mv.y = m_1.y - m_0.y;
      event.setMMove(mv);
      event.execAction();
    }
    break;

  case LDraggingShift:
    { // SDragEnd
      modeState = WaitAny;
      vfrEvent& event = dp.SDragEnd();
      event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
      event.setMPoint(point);
      m_1 = point;
      mv.x = m_1.x - m_0.x; mv.y = m_1.y - m_0.y;
      event.setMMove(mv);
      event.execAction();
    }
    break;

  case LDraggingCntl:
    { // CDragEnd
      modeState = WaitAny;
      vfrEvent& event = dp.CDragEnd();
      event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
      event.setMPoint(point);
      m_1 = point;
      mv.x = m_1.x - m_0.x; mv.y = m_1.y - m_0.y;
      event.setMMove(mv);
      event.execAction();
    }
    break;

  case LDraggingShiftCntl:
    { // SCDragEnd
      modeState = WaitAny;
      vfrEvent& event = dp.SCDragEnd();
      event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
      event.setMPoint(point);
      m_1 = point;
      mv.x = m_1.x - m_0.x; mv.y = m_1.y - m_0.y;
      event.setMMove(mv);
      event.execAction();
    }
    break;

  // ---------------------- RightButton ---------------------- //
  case RDraggingSolo:
    { // RDragEnd
      modeState = WaitAny;
      vfrEvent& event = dp.RDragEnd();
      event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
      event.setMPoint(point);
      m_1 = point;
      mv.x = m_1.x - m_0.x; mv.y = m_1.y - m_0.y;
      event.setMMove(mv);
      event.execAction();
    }
    break;

  case RDraggingShift:
    { // RSDragEnd
      modeState = WaitAny;
      vfrEvent& event = dp.RSDragEnd();
      event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
      event.setMPoint(point);
      m_1 = point;
      mv.x = m_1.x - m_0.x; mv.y = m_1.y - m_0.y;
      event.setMMove(mv);
      event.execAction();
    }
    break;

  case RDraggingCntl:
    { // RCDragEnd
      modeState = WaitAny;
      vfrEvent& event = dp.RCDragEnd();
      event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
      event.setMPoint(point);
      m_1 = point;
      mv.x = m_1.x - m_0.x; mv.y = m_1.y - m_0.y;
      event.setMMove(mv);
      event.execAction();
    }
    break;

  case RDraggingShiftCntl:
    { // RSCDragEnd
      modeState = WaitAny;
      vfrEvent& event = dp.RSCDragEnd();
      event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
      event.setMPoint(point);
      m_1 = point;
      mv.x = m_1.x - m_0.x; mv.y = m_1.y - m_0.y;
      event.setMMove(mv);
      event.execAction();
    }
    break;

  // ---------------------- MiddleButton ---------------------- //
  case MDraggingSolo:
    { // MDragEnd
      modeState = WaitAny;
      vfrEvent& event = dp.MDragEnd();
      event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
      event.setMPoint(point);
      m_1 = point;
      mv.x = m_1.x - m_0.x; mv.y = m_1.y - m_0.y;
      event.setMMove(mv);
      event.execAction();
    }
    break;

  case MDraggingShift:
    { // MSDragEnd
      modeState = WaitAny;
      vfrEvent& event = dp.MSDragEnd();
      event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
      event.setMPoint(point);
      m_1 = point;
      mv.x = m_1.x - m_0.x; mv.y = m_1.y - m_0.y;
      event.setMMove(mv);
      event.execAction();
    }
    break;

  case MDraggingCntl:
    { // MCDragEnd
      modeState = WaitAny;
      vfrEvent& event = dp.MCDragEnd();
      event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
      event.setMPoint(point);
      m_1 = point;
      mv.x = m_1.x - m_0.x; mv.y = m_1.y - m_0.y;
      event.setMMove(mv);
      event.execAction();
    }
    break;

  case MDraggingShiftCntl:
    { // MSCDragEnd
      modeState = WaitAny;
      vfrEvent& event = dp.MSCDragEnd();
      event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
      event.setMPoint(point);
      m_1 = point;
      mv.x = m_1.x - m_0.x; mv.y = m_1.y - m_0.y;
      event.setMMove(mv);
      event.execAction();
    }
    break;

  default:
    modeState = WaitAny;
  }

  chkNotice();
}

// STATIC
#if !defined(WINDOWS)
#include "vfr.xpm"
#endif
const wxIcon& vfrDrawAreaWx::GetVFRIcon() {
  static wxIcon ivfr;
  if ( ivfr.Ok() ) return ivfr;
  ivfr = wxICON(vfr);
  return ivfr;
}

void
vfrDrawAreaWx::OnMouseDClick(int nFlag, Point2 point)
{
  vfrDispatch& dp = vfrDispatch::instance(*this);
  getCanvas()->MakeCurrent();

  modeState = WaitAny;
  setCurScreen(whichScreen(point.x, point.y));

  if ( nFlag & VFRWX_MEV_LEFTDC ) {
    vfrEvent& event = dp.Click();
    event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
    event.setMPoint(point);
    ((vfrEvClick*)(&event))->setDblClickMode(TRUE);
    event.execAction();
    ((vfrEvClick*)(&event))->setDblClickMode(FALSE);
  } else if ( nFlag & VFRWX_MEV_RIGHTDC ) {
    vfrEvent& event = dp.RClick();
    event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
    event.setMPoint(point);
    ((vfrEvRClick*)(&event))->setDblClickMode(TRUE);
    event.execAction();
    ((vfrEvRClick*)(&event))->setDblClickMode(FALSE);
  } else if ( nFlag & VFRWX_MEV_MIDDLEDC ) {
    vfrEvent& event = dp.MClick();
    event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
    event.setMPoint(point);
    ((vfrEvMClick*)(&event))->setDblClickMode(TRUE);
    event.execAction();
    ((vfrEvMClick*)(&event))->setDblClickMode(FALSE);
  }

  chkNotice();
}

void
vfrDrawAreaWx::OnMouseWheel(int nFlag, int wr, Point2 point)
{
  vfrDispatch& dp = vfrDispatch::instance(*this);
  getCanvas()->MakeCurrent();
  //modeState = WaitAny;
  setCurScreen(whichScreen(point.x, point.y));

  vfrEvent& event = dp.Wheel();
  event.setScreen(cScreen < 0 ? NULL : da_screen[cScreen]);
  event.setWheelRot(wr);
  event.setMPoint(point);

  vfrEvWheel* pEvWheel = dynamic_cast<vfrEvWheel*>(&event);
  if ( pEvWheel ) {
    pEvWheel->setKeyShifted(nFlag & VFRWX_MEV_SHIFT);
    pEvWheel->setKeyControlled(nFlag & VFRWX_MEV_CNTL);
  }

  event.execAction();
  chkNotice();
}


// EOF
