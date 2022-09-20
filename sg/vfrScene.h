//
// vfrScene
//   Scene(Grouping) class
//
#ifndef  _VFR_SCENE_H_
#define  _VFR_SCENE_H_

#include "vfrGroup.h"
#include "vfrLight.h"

class EXPORTED_CLASS vfrScene : public vfrGroup {
public:
  vfrScene(const std::string& =std::string(VFR_NONAME), const Bool =FALSE);
  virtual ~vfrScene();

  virtual void rumor(vfrBase*);

  void render(vfrMaterialStack* =NULL);
  void render(const Bool, vfrMaterialStack* =NULL) {}
  void renderBbox();

  int getNumLights() const {return nLight;}
  vfrNode* getNode(const unsigned int);
  vfrNode* getNode(const std::string&);
  Bool accumMatrix(const unsigned int, vfrMatrix&) const;
  void clearDispList();
  vfrLight* getLight(const std::string&);
  vfrLight* getLight(const int);

  // for oit
  virtual Bool setPort(const GLint*) {return TRUE;}

protected:
  int        nLight;
  vfrLight** _light;
};

#endif /* _VFR_SCENE_H_ */
