//
// Created by root on 11/11/24.
//

#ifndef ATTACHABLE_H
#define ATTACHABLE_H
#include "Drawable.h"
#include "Object.h"


class Attachable {
public:
  QVector3D slot_translation;
  bool slot_isFixed;
  bool slot_inVisible;
  explicit Attachable();
  virtual  void updateSlots(QMatrix4x4 transform, float rotation_base);
  float relative_rotation=0;
};



#endif //ATTACHABLE_H
