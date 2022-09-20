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

#include "TestApp.h"

#include "vfrScreen.h"
#include "vfrCamera3D.h"
#include "vfrCamera2D.h"
#include "vfrScene.h"
#include "vfrCube.h"
#include "vfrDefaultActions.h"
#include "vfrKeyCode.h"

#include "vfrImageReaderWx.h"

/* global */
vfrScene *scene;

#define ID_NEW_WINDOW 10000

class MyKeyIn : public vfrAction {
public:
  void execute(vfrEvent &e) {
    if ( ! e.isKeyEvent() ) return;
    VFRkeyCode kc = e.getKey();
    switch ( kc ) {
    case VFRKC_SPACE: 
    {
      vfrDrawAreaWx* pda = (vfrDrawAreaWx*)(e.getScreen()->getParent());
      TestFrame *frame = (TestFrame*)(pda->getCanvas()->GetParent());
      frame->ShowFullScreen(!frame->IsFullScreen(), wxFULLSCREEN_ALL);
      break;
    }
    case VFRKC_x: case VFRKC_X:
      {
	vfrEvKeyIn* pke = dynamic_cast<vfrEvKeyIn*>(&e);
	if ( ! pke ) {printf("Not EvKeyIn ?\n"); return;}
	printf("x/X: ");
	if ( pke->withShiftKey() ) printf("shifted ");
	if ( pke->withControlKey() ) printf("controlled ");
	printf("\n");
      }
      break;
    case VFRKC_ESCAPE:
      exit(0);
    }
  }
} keyIn;



/* `Main program' equivalent, creating windows and returning main app frame */
bool TestApp::OnInit()
{

  /* Create the main frame window */
  TestFrame *frame = new TestFrame(NULL, wxT("Test vfrWx"),
				   wxPoint(50, 50), wxSize(400, 300));
  
  /* Make a menubar */
  wxMenu *fileMenu = new wxMenu;

  fileMenu->Append(wxID_EXIT, wxT("E&xit"));
  fileMenu->Append(ID_NEW_WINDOW, wxT("&New"));
  wxMenuBar *menuBar = new wxMenuBar;
  menuBar->Append(fileMenu, wxT("&File"));
  frame->SetMenuBar(menuBar);

  vfrDrawAreaWx* da = vfrDrawAreaWx::GetInstance(frame);
  frame->SetCanvas(da->getCanvas());

  /* Show the frame */
  frame->Show(TRUE);

  /* scene graph */
  vfrScreen* screen = new vfrScreen();
  da->addScreen(screen);
  vfrCamera3D *camera = new vfrCamera3D();
  screen->setCamera(camera);
  scene = new vfrScene();
  camera->setScene(scene);
  
  vfrCube* cube1 = new vfrCube();
  cube1->trans(-3, 0, 0);
  cube1->setPickMode(PT_OBJECT);
  cube1->setColor3(0, 1.f, 0.f, 0.f);
  scene->addChild(cube1);

  vfrCube* cube2 = new vfrCube();
  //cube2->setColor3(0, 0.f, 1.f, 0.f);
  cube2->setPickMode(PT_OBJECT);
  scene->addChild(cube2);

  vfrCube* cube3 = new vfrCube();
  cube3->setColor3(0, 0.f, 0.f, 1.f);
  cube3->trans(3, 0, 0);
  cube3->setPickMode(PT_OBJECT);
  scene->addChild(cube3);

  if ( argc > 1 ) {
    vfrImage* img = vfrImageReaderWx::ReadImage(argv[1]);
    vfrTexture* tex = new vfrTexture(*img);
    tex->setMapType(vfrTexture::UVC);
    cube2->setTexture(tex);
  }

  vfrDefaultActions::SetDefaultAction(*da);
  vfrDispatch& dispatcher = vfrDispatch::instance(*da);
  vfrEvent& evK = vfrEvKeyIn::instance(dispatcher);
  evK.regist(&keyIn);
  vfrClickSelectAction* pcsa = dynamic_cast<vfrClickSelectAction*>(dispatcher.Click().getAction());
  if ( pcsa ) pcsa->setMButtonType(VFRDA_MBLEFT|VFRDA_MBRIGHT);
  vfrEndRBoxAction* pera = dynamic_cast<vfrEndRBoxAction*>(dispatcher.DragEnd().getAction());
  if ( pera ) pera->setMButtonType(VFRDA_MBLEFT|VFRDA_MBRIGHT);
  
  return TRUE;
}

IMPLEMENT_APP(TestApp)

BEGIN_EVENT_TABLE(TestFrame, wxFrame)
  EVT_MENU(wxID_EXIT, TestFrame::OnExit)
  EVT_MENU(ID_NEW_WINDOW, TestFrame::OnNewFrame)
END_EVENT_TABLE()

/* My frame constructor */
TestFrame::TestFrame(wxFrame *frame, const wxString& title,
		     const wxPoint& pos, const wxSize& size, long style)
  : wxFrame(frame, -1, title, pos, size, style)
{
  m_canvas = NULL;

  SetIcon(vfrDrawAreaWx::GetVFRIcon());
}

/* Intercept menu commands */
void TestFrame::OnExit(wxCommandEvent& event)
{
  Destroy();
}

void TestFrame::OnNewFrame(wxCommandEvent& event)
{
  TestFrame* frame = new TestFrame(NULL, wxT("Test vfrWx sub"),
				   wxPoint(100, 100), wxSize(300, 300));
  // Give it an icon
  frame->SetIcon(vfrDrawAreaWx::GetVFRIcon());

  vfrDrawAreaWx* da = vfrDrawAreaWx::GetInstance(frame);
  frame->SetCanvas(da->getCanvas());

  // Show the frame
  frame->Show(TRUE);

  vfrScreen* screen = new vfrScreen();
  da->addScreen(screen);
  vfrCamera3D *camera = new vfrCamera3D();
  screen->setCamera(camera);
  camera->setScene(scene);
  vfrDefaultActions::SetDefaultAction(*da);
}

