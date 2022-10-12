#include "core/internal/object_pool.h"
#include "test.h"

#include <vector>

using namespace mozi::core;

constexpr int INVALID = -1;

class Foo
{
    public:
        static int construction_counter;
        static int destrcution_counter;
        int* data{nullptr};

        Foo()
        {
            ++construction_counter;
        }

        Foo(int& data)
            : data_(&data)
        {
            *data_ = ++construction_counter;
        }

        ~Foo()
        {
            if (data_)
            {
                *data_ = INVALID;
            }
            ++destruction_counter;
        }

        static void resetConstructCounter()
        {
            construction_counter = 0;
        }

        static int getConstructionCounter()
        {
            return construction_counter;
        }

        static void resetDestructionCounter()
        {
            destruction_counter = 0;
        }

        static int getDestructionCounter()
        {
            return destruction_counter;
        }

};

int Foo::construction_counter{0};
int Foo::destruction_counter{0};
