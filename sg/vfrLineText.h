//
// vfrLineText
//   Line Stroke Text Object Class
//
#ifndef _VFR_LINE_TEXT_H_
#define _VFR_LINE_TEXT_H_

#include "vfrLetters.h"


class EXPORTED_CLASS vfrLineText : public vfrLetters {
public:
  vfrLineText(const std::string& =std::string(VFR_NONAME), const Bool =FALSE);
  virtual ~vfrLineText();

  void drawLetter(const char);

private:
  void reset();
  virtual GLfloat drawLetter(const char) const;
  virtual GLfloat getTextWidth() const;
  virtual GLfloat getTextHeight() const;

  static GLfloat _fontSize[2];
};

#endif /* _VFR_LINE_TEXT_H_ */
