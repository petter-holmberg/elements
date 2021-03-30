#include "catch.hpp"

#include "partition.h"

namespace e = elements;

SCENARIO ("Partition points", "[partition]")
{
    auto is_even = [](int i){ return i % 2 == 0; };

    SECTION ("Checking if an empty array is partitioned")
    {
        int x[]{0};

        REQUIRE (e::is_partitioned(x, x, is_even));
        REQUIRE (e::is_partitioned_n(x, 0, is_even));
        REQUIRE (!e::precedes(e::partition_point(x, x, is_even), +x));
    }

    SECTION ("Checking if a single-element array with an even number is partitioned")
    {
        int x[]{0};

        REQUIRE (e::is_partitioned(x, x + 1, is_even));
        REQUIRE (e::is_partitioned_n(x, 1, is_even));
    }

    SECTION ("Checking if a single-element array with an odd number is partitioned")
    {
        int x[]{1};

        REQUIRE (e::is_partitioned(x, x + 1, is_even));
        REQUIRE (e::is_partitioned_n(x, 1, is_even));
    }

    SECTION ("Checking if an array with an even and an odd number is partitioned")
    {
        int x[]{0, 1};

        REQUIRE (!e::is_partitioned(x, x + 2, is_even));
        REQUIRE (e::is_partitioned_n(x, 2, is_even));
    }

    SECTION ("Checking if an array with an odd and an even number is partitioned")
    {
        int x[]{1, 0};

        REQUIRE (e::is_partitioned(x, x + 2, is_even));
        REQUIRE (e::is_partitioned_n(x, 2, is_even));
    }
}

SCENARIO ("Partitioning", "[partition]")
{
    auto is_odd = [](int i){ return i % 2 == 1; };

    SECTION ("Partitioning an empty array")
    {
        int x[]{0};
        int buf[]{0};

        {
            auto cur = e::partition_semistable(x, x, is_odd);
            REQUIRE (e::is_partitioned(x, x, is_odd));
            REQUIRE (cur == x);
        }

        {
            auto cur = e::partition_stable_with_buffer(x, x, buf, is_odd);
            REQUIRE (e::is_partitioned(x, x, is_odd));
            REQUIRE (cur == x);
        }

        {
            auto cur = e::partition_unstable(x, x, is_odd);
            REQUIRE (e::is_partitioned(x, x, is_odd));
            REQUIRE (cur == x);
        }
    }

    SECTION ("Partitioning an array with one element")
    {
        int buf[]{0};

        {
            int x[]{0};

            {
                auto cur = e::partition_semistable(x, x + 1, is_odd);
                REQUIRE (e::is_partitioned(x, x + 1, is_odd));
                REQUIRE (cur == x + 1);
            }

            {
                auto cur = e::partition_stable_with_buffer(x, x + 1, buf, is_odd);
                REQUIRE (e::is_partitioned(x, x + 1, is_odd));
                REQUIRE (cur == x + 1);
            }

            {
                auto cur = e::partition_unstable(x, x + 1, is_odd);
                REQUIRE (e::is_partitioned(x, x + 1, is_odd));
                REQUIRE (cur == x + 1);
            }
        }

        {
            int x[]{1};

            {
                auto cur = e::partition_semistable(x, x + 1, is_odd);
                REQUIRE (e::is_partitioned(x, x + 1, is_odd));
                REQUIRE (cur == x);
            }

            {
                auto cur = e::partition_stable_with_buffer(x, x + 1, buf, is_odd);
                REQUIRE (e::is_partitioned(x, x + 1, is_odd));
                REQUIRE (cur == x);
            }

            {
                auto cur = e::partition_unstable(x, x + 1, is_odd);
                REQUIRE (e::is_partitioned(x, x + 1, is_odd));
                REQUIRE (cur == x);
            }
        }
    }

    SECTION ("Partitioning an array with two elements")
    {
        int buf[]{0, 0};

        {
            int x[]{0, 0};

            {
                auto cur = e::partition_semistable(x, x + 2, is_odd);
                REQUIRE (e::is_partitioned(x, x + 2, is_odd));
                REQUIRE (cur == x + 2);
                REQUIRE (x[0] == 0);
                REQUIRE (x[1] == 0);
            }

            {
                auto cur = e::partition_stable_with_buffer(x, x + 2, buf, is_odd);
                REQUIRE (e::is_partitioned(x, x + 2, is_odd));
                REQUIRE (cur == x + 2);
                REQUIRE (x[0] == 0);
                REQUIRE (x[1] == 0);
            }

            {
                auto cur = e::partition_unstable(x, x + 2, is_odd);
                REQUIRE (e::is_partitioned(x, x + 2, is_odd));
                REQUIRE (cur == x + 2);
                REQUIRE (x[0] == 0);
                REQUIRE (x[1] == 0);
            }
        }

        {
            int x[]{0, 1};

            {
                auto cur = e::partition_semistable(x, x + 2, is_odd);
                REQUIRE (e::is_partitioned(x, x + 2, is_odd));
                REQUIRE (cur == x + 1);
                REQUIRE (x[0] == 0);
                REQUIRE (x[1] == 1);
            }

            {
                auto cur = e::partition_stable_with_buffer(x, x + 2, buf, is_odd);
                REQUIRE (e::is_partitioned(x, x + 2, is_odd));
                REQUIRE (cur == x + 1);
                REQUIRE (x[0] == 0);
                REQUIRE (x[1] == 1);
            }

            {
                auto cur = e::partition_unstable(x, x + 2, is_odd);
                REQUIRE (e::is_partitioned(x, x + 2, is_odd));
                REQUIRE (cur == x + 1);
                REQUIRE (x[0] == 0);
                REQUIRE (x[1] == 1);
            }
        }

        {
            int x[]{1, 0};

            {
                auto cur = e::partition_semistable(x, x + 2, is_odd);
                REQUIRE (e::is_partitioned(x, x + 2, is_odd));
                REQUIRE (cur == x + 1);
                REQUIRE (x[0] == 0);
                REQUIRE (x[1] == 1);
            }

            {
                auto cur = e::partition_stable_with_buffer(x, x + 2, buf, is_odd);
                REQUIRE (e::is_partitioned(x, x + 2, is_odd));
                REQUIRE (cur == x + 1);
                REQUIRE (x[0] == 0);
                REQUIRE (x[1] == 1);
            }

            {
                auto cur = e::partition_unstable(x, x + 2, is_odd);
                REQUIRE (e::is_partitioned(x, x + 2, is_odd));
                REQUIRE (cur == x + 1);
                REQUIRE (x[0] == 0);
                REQUIRE (x[1] == 1);
            }
        }

        {
            int x[]{1, 1};

            {
                auto cur = e::partition_semistable(x, x + 2, is_odd);
                REQUIRE (e::is_partitioned(x, x + 2, is_odd));
                REQUIRE (cur == x);
                REQUIRE (x[0] == 1);
                REQUIRE (x[1] == 1);
            }

            {
                auto cur = e::partition_stable_with_buffer(x, x + 2, buf, is_odd);
                REQUIRE (e::is_partitioned(x, x + 2, is_odd));
                REQUIRE (cur == x);
                REQUIRE (x[0] == 1);
                REQUIRE (x[1] == 1);
            }

            {
                auto cur = e::partition_unstable(x, x + 2, is_odd);
                REQUIRE (e::is_partitioned(x, x + 2, is_odd));
                REQUIRE (cur == x);
                REQUIRE (x[0] == 1);
                REQUIRE (x[1] == 1);
            }
        }
    }

    SECTION ("Partitioning an array with three elements")
    {
        int buf[]{0, 0, 0};

        {
            int x[]{0, 0, 0};

            {
                auto cur = e::partition_semistable(x, x + 3, is_odd);
                REQUIRE (e::is_partitioned(x, x + 3, is_odd));
                REQUIRE (cur == x + 3);
                REQUIRE (x[0] == 0);
                REQUIRE (x[1] == 0);
                REQUIRE (x[2] == 0);
            }

            {
                auto cur = e::partition_stable_with_buffer(x, x + 3, buf, is_odd);
                REQUIRE (e::is_partitioned(x, x + 3, is_odd));
                REQUIRE (cur == x + 3);
                REQUIRE (x[0] == 0);
                REQUIRE (x[1] == 0);
                REQUIRE (x[2] == 0);
            }

            {
                auto cur = e::partition_unstable(x, x + 3, is_odd);
                REQUIRE (e::is_partitioned(x, x + 3, is_odd));
                REQUIRE (cur == x + 3);
                REQUIRE (x[0] == 0);
                REQUIRE (x[1] == 0);
                REQUIRE (x[2] == 0);
            }
        }

        {
            int x[]{0, 0, 1};

            {
                auto cur = e::partition_semistable(x, x + 3, is_odd);
                REQUIRE (e::is_partitioned(x, x + 3, is_odd));
                REQUIRE (cur == x + 2);
                REQUIRE (x[0] == 0);
                REQUIRE (x[1] == 0);
                REQUIRE (x[2] == 1);
            }

            {
                auto cur = e::partition_stable_with_buffer(x, x + 3, buf, is_odd);
                REQUIRE (e::is_partitioned(x, x + 3, is_odd));
                REQUIRE (cur == x + 2);
                REQUIRE (x[0] == 0);
                REQUIRE (x[1] == 0);
                REQUIRE (x[2] == 1);
            }

            {
                auto cur = e::partition_unstable(x, x + 3, is_odd);
                REQUIRE (e::is_partitioned(x, x + 3, is_odd));
                REQUIRE (cur == x + 2);
                REQUIRE (x[0] == 0);
                REQUIRE (x[1] == 0);
                REQUIRE (x[2] == 1);
            }
        }

        {
            int x[]{0, 1, 0};

            {
                auto cur = e::partition_semistable(x, x + 3, is_odd);
                REQUIRE (e::is_partitioned(x, x + 3, is_odd));
                REQUIRE (cur == x + 2);
                REQUIRE (x[0] == 0);
                REQUIRE (x[1] == 0);
                REQUIRE (x[2] == 1);
            }

            {
                auto cur = e::partition_stable_with_buffer(x, x + 3, buf, is_odd);
                REQUIRE (e::is_partitioned(x, x + 3, is_odd));
                REQUIRE (cur == x + 2);
                REQUIRE (x[0] == 0);
                REQUIRE (x[1] == 0);
                REQUIRE (x[2] == 1);
            }

            {
                auto cur = e::partition_unstable(x, x + 3, is_odd);
                REQUIRE (e::is_partitioned(x, x + 3, is_odd));
                REQUIRE (cur == x + 2);
                REQUIRE (x[0] == 0);
                REQUIRE (x[1] == 0);
                REQUIRE (x[2] == 1);
            }
        }

        {
            int x[]{0, 1, 1};

            {
                auto cur = e::partition_semistable(x, x + 3, is_odd);
                REQUIRE (e::is_partitioned(x, x + 3, is_odd));
                REQUIRE (cur == x + 1);
                REQUIRE (x[0] == 0);
                REQUIRE (x[1] == 1);
                REQUIRE (x[2] == 1);
            }

            {
                auto cur = e::partition_stable_with_buffer(x, x + 3, buf, is_odd);
                REQUIRE (e::is_partitioned(x, x + 3, is_odd));
                REQUIRE (cur == x + 1);
                REQUIRE (x[0] == 0);
                REQUIRE (x[1] == 1);
                REQUIRE (x[2] == 1);
            }

            {
                auto cur = e::partition_unstable(x, x + 3, is_odd);
                REQUIRE (e::is_partitioned(x, x + 3, is_odd));
                REQUIRE (cur == x + 1);
                REQUIRE (x[0] == 0);
                REQUIRE (x[1] == 1);
                REQUIRE (x[2] == 1);
            }
        }

        {
            int x[]{1, 0, 0};

            {
                auto cur = e::partition_semistable(x, x + 3, is_odd);
                REQUIRE (e::is_partitioned(x, x + 3, is_odd));
                REQUIRE (cur == x + 2);
                REQUIRE (x[0] == 0);
                REQUIRE (x[1] == 0);
                REQUIRE (x[2] == 1);
            }

            {
                auto cur = e::partition_stable_with_buffer(x, x + 3, buf, is_odd);
                REQUIRE (e::is_partitioned(x, x + 3, is_odd));
                REQUIRE (cur == x + 2);
                REQUIRE (x[0] == 0);
                REQUIRE (x[1] == 0);
                REQUIRE (x[2] == 1);
            }

            {
                auto cur = e::partition_unstable(x, x + 3, is_odd);
                REQUIRE (e::is_partitioned(x, x + 3, is_odd));
                REQUIRE (cur == x + 2);
                REQUIRE (x[0] == 0);
                REQUIRE (x[1] == 0);
                REQUIRE (x[2] == 1);
            }
        }

        {
            int x[]{1, 0, 1};

            {
                auto cur = e::partition_semistable(x, x + 3, is_odd);
                REQUIRE (e::is_partitioned(x, x + 3, is_odd));
                REQUIRE (cur == x + 1);
                REQUIRE (x[0] == 0);
                REQUIRE (x[1] == 1);
                REQUIRE (x[2] == 1);
            }

            {
                auto cur = e::partition_stable_with_buffer(x, x + 3, buf, is_odd);
                REQUIRE (e::is_partitioned(x, x + 3, is_odd));
                REQUIRE (cur == x + 1);
                REQUIRE (x[0] == 0);
                REQUIRE (x[1] == 1);
                REQUIRE (x[2] == 1);
            }

            {
                auto cur = e::partition_unstable(x, x + 3, is_odd);
                REQUIRE (e::is_partitioned(x, x + 3, is_odd));
                REQUIRE (cur == x + 1);
                REQUIRE (x[0] == 0);
                REQUIRE (x[1] == 1);
                REQUIRE (x[2] == 1);
            }
        }

        {
            int x[]{1, 1, 0};

            {
                auto cur = e::partition_semistable(x, x + 3, is_odd);
                REQUIRE (e::is_partitioned(x, x + 3, is_odd));
                REQUIRE (cur == x + 1);
                REQUIRE (x[0] == 0);
                REQUIRE (x[1] == 1);
                REQUIRE (x[2] == 1);
            }

            {
                auto cur = e::partition_stable_with_buffer(x, x + 3, buf, is_odd);
                REQUIRE (e::is_partitioned(x, x + 3, is_odd));
                REQUIRE (cur == x + 1);
                REQUIRE (x[0] == 0);
                REQUIRE (x[1] == 1);
                REQUIRE (x[2] == 1);
            }

            {
                auto cur = e::partition_unstable(x, x + 3, is_odd);
                REQUIRE (e::is_partitioned(x, x + 3, is_odd));
                REQUIRE (cur == x + 1);
                REQUIRE (x[0] == 0);
                REQUIRE (x[1] == 1);
                REQUIRE (x[2] == 1);
            }
        }

        {
            int x[]{1, 1, 1};

            {
                auto cur = e::partition_semistable(x, x + 3, is_odd);
                REQUIRE (e::is_partitioned(x, x + 3, is_odd));
                REQUIRE (cur == x);
                REQUIRE (x[0] == 1);
                REQUIRE (x[1] == 1);
                REQUIRE (x[2] == 1);
            }

            {
                auto cur = e::partition_stable_with_buffer(x, x + 3, buf, is_odd);
                REQUIRE (e::is_partitioned(x, x + 3, is_odd));
                REQUIRE (cur == x);
                REQUIRE (x[0] == 1);
                REQUIRE (x[1] == 1);
                REQUIRE (x[2] == 1);
            }

            {
                auto cur = e::partition_unstable(x, x + 3, is_odd);
                REQUIRE (e::is_partitioned(x, x + 3, is_odd));
                REQUIRE (cur == x);
                REQUIRE (x[0] == 1);
                REQUIRE (x[1] == 1);
                REQUIRE (x[2] == 1);
            }
        }
    }
}
