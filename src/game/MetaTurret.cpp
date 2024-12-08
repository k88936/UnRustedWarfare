//
// Created by root on 11/5/24.
//

#include "MetaTurret.h"

MetaTurret::MetaTurret()
{
}

void MetaTurret::init()
{
    if (image=="NONE")
    {
        this->slot_inVisible=true;
        return;
    }
    MetaDrawable::init();
}
