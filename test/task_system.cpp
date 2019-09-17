#include "catch.hpp"

#include "elements.h"

#include <iostream>

namespace e = elements;

SCENARIO ("Using task system", "[task_system]")
{
    SECTION ("Using notification queue")
    {
        auto test0 = [](){ /*std::cout << "hello 1\n"*/; };
        auto test1 = [](){ /*std::cout << "hello 2\n"*/; };
        auto test2 = [](){ /*std::cout << "hello 3\n"*/; };
        auto test3 = [](int n){ /*std::cout << "hello << " << n << "\n";*/ return n; };

        e::task_system x;

        for (int i = 0; i < 1000; ++i)
        {
            x.async(test0);
            x.async(test1);
            x.async(test2);

            auto f = e::async(test3, 42);
            f.get();
        }
    }
}
