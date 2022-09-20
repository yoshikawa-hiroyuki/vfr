//
// vfrGroup
//   Object Grouping class
//
#ifndef _VFR_GROUP_H_
#define _VFR_GROUP_H_

#include "vfrNode.h"

class EXPORTED_CLASS vfrGroup : public vfrNode {
public:
  vfrGroup(const std::string& =std::string(VFR_NONAME), const Bool =FALSE);
  virtual ~vfrGroup();

  void render(vfrMaterialStack* =NULL);
  void render(const Bool, vfrMaterialStack* =NULL);
  void renderBbox();
  void renderFeedBack(const unsigned int);

  Bool addChild(vfrNode*);
  Bool remChild(vfrNode*);
  void remAllChildren();
  vfrNode* getChild(const int);
  int getNumChildren() const {return nChild;}

  vfrNode* getNode(const unsigned int);
  vfrNode* getNode(const std::string&);

  void renderSolid();
  void renderWire();
  void renderPoint();

  void notice();
  void rumor(vfrBase*);
  Bool accumMatrix(const unsigned int, vfrMatrix&) const;

  void invalidateDispList();
  void clearDispList();

protected:
  int nChild;
  vfrNode** _children;

  void generateBbox();
  void generateBbox(CES::Vec3<float>&) {generateBbox();}
};

#endif /* _VFR_GROUP_H_ */
