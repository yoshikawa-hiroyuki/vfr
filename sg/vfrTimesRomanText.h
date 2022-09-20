//
// vfrTimesRomanText
//   Times-Roman face Stroke Text Object Class
//
#ifndef _VFR_TIMESROMAN_TEXT_H_
#define _VFR_TIMESROMAN_TEXT_H_

#include "vfrLetters.h"

class EXPORTED_CLASS vfrTimesRomanText : public vfrLetters {
public:
  vfrTimesRomanText(const std::string& =std::string(VFR_NONAME),
		const Bool =FALSE);
  virtual ~vfrTimesRomanText();

  void drawLetter(const char);

private:
  void reset();
  virtual GLfloat drawLetter(const char) const;
  GLfloat getTextWidth() const;
  GLfloat getTextHeight() const;

  static GLfloat _fontSize[2];
};

#endif /* _VFR_TIMESROMAN_TEXT_H_ */
