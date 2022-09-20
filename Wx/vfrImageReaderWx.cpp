//-------------------------------------------------------
// vfrImageReaderWx
//   read image file by using wxWidgets image feature
//-------------------------------------------------------

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/image.h"
#include "wx/file.h"

#include "vfrImageReaderWx.h"

/* static members */
bool vfrImageReaderWx::s_handlerInstalled = false;


// STATIC METHOD //
vfrImage*
vfrImageReaderWx::ReadImage(const char* fname, const Bool adjustFlg)
{
  if ( ! fname || strlen(fname) < 1 ) return NULL;

  // install handler
  if ( ! s_handlerInstalled ) {
    wxInitAllImageHandlers();
    s_handlerInstalled = true;
  }

  // prepare image data for return
  vfrImage* retImg = new vfrImage();
  if ( ! retImg ) return NULL;

  // load image by wxImage
  wxImage image;
  if ( ! image.LoadFile(wxString(fname, *wxConvCurrent)) ) {
    delete retImg;
    return NULL;
  }

  // allocate datas
  int w = image.GetWidth();
  int h = image.GetHeight();
  Point2 wsz;
  if ( adjustFlg ) vfrImage::GetWrapSize(w, h, wsz);
  else {
#if 0
    wsz.x = w; wsz.y = h;
#else
    int m4x = w % 4;
    int m4y = h % 4;
    if ( m4x < m4y ) {wsz.x = w; wsz.y = h + (4 - m4y);}
    else {wsz.x = w + (4 - m4x); wsz.y = h;}
#endif
  }
  unsigned char* pix = image.GetData();
  if ( ! retImg->allocateDatas(wsz.x, wsz.y, 3, FALSE) ) {
    delete retImg;
    return NULL;
  }

  // copy(crop) Image data
  unsigned char *bp = retImg->getData();
  //memcpy(bp, pix, wsz.x * wsz.y * 3);
  for ( register int i = 0; i < h; i++ )
    for ( register int j = 0; j < w; j++ )
      for ( register int k = 0; k < 3; k++ )
        bp[3*wsz.x*(h-1-i) + 3*j +k] = pix[3*w*i + 3*j + k];

  // epilogue
  return retImg;
}

