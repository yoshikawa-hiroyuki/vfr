//
// vfrLetters
//   Basic Text Object Class
//
#ifndef _VFR_LETTERS_H_
#define _VFR_LETTERS_H_

#include "vfrNode.h"

class EXPORTED_CLASS vfrLetters : public vfrNode {
public:
  virtual ~vfrLetters();

  void setLetters(const std::string& s);
  void setLetters(char* fmt, const int val);
  void setLetters(char* fmt, const float val);
  std::string getLetters() const {return _textBuf;}
  std::string& getLettersBuff() {return _textBuf;}

  void setFontAlign(const AlignType);
  AlignType getFontAlign() const {return _alignType;}
  void setFontScale(const float);
  float getFontScale() const {return _fontScale;}
  void setSpaceRate(const float sr) {_spaceRate=sr; generateBbox(); notice();}
  float getSpaceRate() const {return _spaceRate;}

  int getNumLines() const;

protected:
  vfrLetters(const std::string& =std::string(VFR_NONAME), const Bool =FALSE);

  void resetLetters();
  void renderSolid();
  void renderWire();

  std::string _textBuf;
  AlignType   _alignType;
  float       _fontScale;
  float       _spaceRate;
  
  virtual GLfloat drawLetter(const char) const {return 0.f;}
  virtual GLfloat getTextWidth() const {return 0.5f;}
  virtual GLfloat getTextHeight() const {return 0.5f;}
  virtual void generateBbox();
  virtual void generateBbox(const vector3) {generateBbox();}
};

#endif /* _VFR_LETTERS_H_ */
