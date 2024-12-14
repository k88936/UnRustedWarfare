//
// Created by root on 11/11/24.
//

#ifndef ATTACHABLE_H
#define ATTACHABLE_H
#include "Drawable.h"
#include "Object.h"


class Attachable {
public:
  explicit Attachable();
  QVector3D slot_translation;
  bool slot_isFixed;
  bool slot_inVisible=false;
  virtual  void updateSlots(QMatrix4x4 transform);
  float relative_rotation=0;
  void setRelativeRotation(float rotation);
  void addRelativeRotation(float rotation);
};



#endif //ATTACHABLE_H
