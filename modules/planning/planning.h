#ifndef MOZI_MODULES_PLANNING_PLANNING_H
#define MOZI_MODULES_PLANNING_PLANNING_H

#include <iostream>

class Planning : ComponentBase
{
    public:
        Planning()
        {
            std::cout << "This is Planning Module." << std::endl;
        }

        ~Planning()
        {
            std::cout << "Planning is destructed." << std::endl;
        }

        void Init()
        {
            std::cout << "Planning Init." << std::endl;
        }

        void Proc()
        {
            std::cout << "Planning Execute." << std::endl;
        }

        void Shutdown()
        {
            std::cout << "Planning Shutdown." << std::endl;
        }

        static Planning* Create()
        {
            return new Planning();
        }

    private:
        static int count_;

};

REGISTER_COMPONENT("Planning", Planning::Create, __COUNTER__);

#endif
