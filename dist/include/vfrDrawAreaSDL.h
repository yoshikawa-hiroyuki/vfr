//
// vfrDrawAreaSDL
//   Drawing Area Class with SDL library
//

#ifndef _VFR_DRAW_AREA_SDL_H_
#define _VFR_DRAW_AREA_SDL_H_

#ifdef _WIN32
#include "stdafx.h"
#endif

#if defined(_WIN32) || defined(__APPLE__)
#include <SDL.h>
#else
#include <SDL/SDL.h>
#endif

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif // __APPLE__

#include <stdio.h>
#include <stdlib.h>

#include "vfrDrawArea.h"

#define VFR_MAX_DRAWAREA    1

#define VFR_SDL_RESIZABLE   1
#define VFR_SDL_FULLSCREEN  2


class EXPORTED_CLASS vfrDrawAreaSDL : public vfrDrawArea {
public:
  virtual ~vfrDrawAreaSDL();

  //------------ derived from vfrDawArea ------------
  virtual void redraw();
  virtual void notice();
  virtual void chkNotice();
  virtual void rumor(class vfrScreen*);
  virtual Point2 getSize();


  Bool initialize(const char* title =NULL,
		  const unsigned int w =640, const unsigned int h =480,
		  const Bool dbl =TRUE, const int mode =VFR_SDL_RESIZABLE);
  void clearDispList();
  void drawImmediate();

  Bool ProcessEvent();
  const SDL_VideoInfo* GetSDLVideoInfo() const {return sdl_info;}
  SDL_Surface* GetSDLSurface() {return sdl_surface;}

  static VFRkeyCode convKey_SDLtoVFR(SDLKey);
  static vfrDrawAreaSDL* GetInstance();

  static Bool SetFullScreen(const Bool);
  static Bool GetFullScreenMode() {return s_fullScreen;}
  static void GetFullScreenSize(Point2& p) {
    p.x = s_fullScreenSize.w; p.y = s_fullScreenSize.h;
  }

protected:
  Bool      noticeFlag;
  StateType modeState;
  Point2    m_0, m_1;
  int       k_mod;

  const SDL_VideoInfo* sdl_info;
  SDL_Surface* sdl_surface;
  Point2 surfSize;

  static Bool s_dblBuffer;
  static Bool s_fullScreen;
  static SDL_Rect s_fullScreenSize;
  static int s_sdlVideoFlags;
  static int s_sdlVideoBpp;

  static void inputCB(SDL_Event& event);
  static void resizeCB(SDL_ResizeEvent& event);
  static void destroyCB();

  void OnKeyDown(VFRkeyCode);
  void OnLButtonDown(Point2);
  void OnRButtonDown(Point2);
  void OnMButtonDown(Point2);
  void OnLButtonUp(int, Point2);
  void OnRButtonUp(int, Point2);
  void OnMButtonUp(int, Point2);
  void OnMouseMove(Uint8, int, Point2);

  // this class is a singleton
  vfrDrawAreaSDL();
  static vfrDrawAreaSDL* s_theDrawArea;
};

#endif /* _VFR_DRAW_AREA_SDL_H_ */
