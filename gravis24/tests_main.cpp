/// @file tests_main.cpp
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include "../include/graph.hpp"

TEST_SUITE("Basic tests")
{
    TEST_CASE("Taking up")
    {
        auto graph = gravis24::newGraph(10);
        CHECK(graph->getVertexCount() == 10);
        CHECK(graph->getArcCount()    == 0);
    }
}

