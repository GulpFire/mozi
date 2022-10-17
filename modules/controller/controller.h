#ifndef MOZI_MODULES_CONTROLLER_CONTROLLER_H
#define MOZI_MODULES_CONTROLLER_CONTROLLER_H

#include <mozi/core/class_loader/component/component_base.h>
#include <mozi/core/class_loader/component/register_macro.h>

#include <iostream>

class Controller : ComponentBase
{
    public:
        Controller()
        {
            std::cout << "This is Controller Module." << std::endl;
        }
    
        ~Controller()
        {
            std::cout << "Controller is destrcuted."
        }

        void Init()
        {
            std::cout << "Controller Init." << std::endl;
        }

        void Proc()
        {
            std::cout << "Controller Execute." << std::endl;
        }

        void Shutdown()
        {
            std::cout << "Controller Shutdown." << std::endl;
        }

        static Controller* Create()
        {
            return new Controller();
        }

    private:
        static int count_;
};

REGISTER_COMPONENT("Controller", Controller::Create, __COUNTER__)

#endif
