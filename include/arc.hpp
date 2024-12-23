/// @file arc.hpp
#ifndef GRAVIS24_ARC_HPP
#define GRAVIS24_ARC_HPP

#include <compare>

namespace gravis24
{

    struct Arc
    {
        int source;
        int target;

        friend auto operator<=>(Arc const&, Arc const&) noexcept = default;
    };

}

#endif//GRAVIS24_ARC_HPP
