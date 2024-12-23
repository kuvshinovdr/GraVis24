/// @file algorithm_dfs.hpp
#ifndef GRAVIS24_ALGORITHM_DFS_HPP
#define GRAVIS24_ALGORITHM_DFS_HPP

#include "event_source.hpp"


namespace gravis24::algorithm
{

    class Dfs
        : public EventSource
    {
    public:
        void subscribe(EventListener&) override;
        void unsubscribe(EventListener&) override;
        [[nodiscard]] bool isSubscribed(EventListener&) const noexcept override;
    };

}

#endif//GRAVIS24_ALGORITHM_DFS_HPP
