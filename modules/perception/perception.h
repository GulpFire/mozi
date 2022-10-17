#ifndef MOZI_MODULES_PERCEPTION_PERCEPTION_H
#define MOZI_MODULES_PERCEPTION_PERCEPTION_H

#include "mozi/core/class_loader/component/component_base.h"
#include "mozi/core/class_loader/component/register_macro.h"

#include <iostream>

class Perception : public ComponentBase
{
    public:
        Perception()
        {
            std::cout << "This is Perception Module." << std::endl;
        }
        ~Perception()
        {
            std::cout << "Perception is destructed." << std::endl;
        }

        void Init() override
        {
            std::cout << "Perception Init." << std::endl;
        }

        void Proc() override
        {
            std::cout << "Perception Execute." << std::endl;
        }

        void Shutdown() override
        {
            std::cout << "Perception Shutdown." << std::endl;
        }

        static ComponentBase* Create()
        {
            return new Perdiction();
        }

    private:
        static int count_;
};

REGISTER_COMPONENT("Perception", Perception::Create, __COUNTER__);

#endif
