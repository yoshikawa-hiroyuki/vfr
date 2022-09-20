//
// vfrHelveticaText
//   Helvetica face Stroke Text Object Class
//
#ifndef _VFR_HELVETICA_TEXT_H_
#define _VFR_HELVETICA_TEXT_H_

#include "vfrLetters.h"

class EXPORTED_CLASS vfrHelveticaText : public vfrLetters {
public:
  vfrHelveticaText(const std::string& =std::string(VFR_NONAME),
		   const Bool =FALSE);
  virtual ~vfrHelveticaText();

  void drawLetter(const char);

protected:
  void reset();
  virtual GLfloat drawLetter(const char) const;
  virtual GLfloat getTextWidth() const;
  virtual GLfloat getTextHeight() const;
  virtual void generateBbox();

  static GLfloat _fontSize[2];
};

#endif /* _VFR_HELVETICA_TEXT_H_ */
