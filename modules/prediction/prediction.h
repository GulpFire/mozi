#ifndef MOZI_MODULES_PREDICTION_PREDICTION_H
#define MOZI_MODULES_PREDICTION_PREDICTION_H

#include <mozi/core/class_loader/component/component_base.h>
#include <mozi/core/class_loader/component/register_macro.h>

#include <iostream>

class Prediction : public ComponentBase
{
    public:
        Prediction()
        {
            std::cout << "This is Prediction Module." << std::endl;
        }
        ~Prediction()
        {
            std::cout << "Prediction is destructed." << std::endl;
        }

        void Init()
        {
            std::cout << "Prediciton Init." << std::endl;
        }

        void Proc()
        {
            std::cout << "Prediciton Execute." << std::endl;
        }

        void Shutdown()
        {
            std::cout << "Prediction Shutdown." << std::endl;
        }

        static ComponentBase* create()
        {
            return new Prediction();
        }
    private:
        static int count_;
};

REGISTER_COMPONENT("Prediction", Prediction::Create, __COUNTER__);

#endif
