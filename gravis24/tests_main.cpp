/// @file tests_main.cpp
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include "../include/graph.hpp"


TEST_SUITE("Basic tests")
{
    TEST_CASE("Taking up")
    {
        auto graph = gravis24::newGraph(10);

        REQUIRE(graph != nullptr);
        CHECK(graph->getVertexCount() == 10);
        CHECK(graph->getArcCount()    == 0);

        CHECK(graph->connect(0, 1));
        CHECK(graph->connect(1, 2));
        CHECK(graph->getArcCount() == 2);
        CHECK(graph->areConnected(0, 1));
        CHECK(graph->areConnected(1, 2));
        CHECK(!graph->areConnected(1, 0));
        CHECK(!graph->areConnected(2, 1));
        CHECK(!graph->areConnected(0, 3));

        CHECK(graph->connect(0, 3));
        CHECK(graph->getArcCount() == 3);
        CHECK(graph->disconnect(1, 2));
        CHECK(!graph->areConnected(1, 2));
        CHECK(graph->getArcCount() == 2);

        CHECK(!graph->disconnect(0, 4));
        CHECK(graph->getArcCount() == 2);

        CHECK(graph->disconnect(0, 1));
        CHECK(graph->getArcCount() == 1);
        CHECK(!graph->areConnected(0, 1));
        CHECK(graph->disconnect(0, 3));
        CHECK(graph->getArcCount() == 0);
        CHECK(!graph->areConnected(0, 3));
    }
}

