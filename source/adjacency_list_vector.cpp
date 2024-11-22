/// @file  adjacency_list_vector.cpp
/// @brief Реализация EditableAdjacencyList поверх vector vector int.
#include "../include/adjacency_list.hpp"

#include <vector>
#include <algorithm>

namespace gravis24
{

    class AdjacencyListVector
        : public EditableAdjacencyList
    {
    public:
        /////////////////////////////////////////////////////
        // Реализация интерфейса AdjacencyListView

        [[nodiscard]] auto getVertexCount() const noexcept
            -> int override
        {
            return static_cast<int>(_adj.size());
        }

        [[nodiscard]] bool areConnected(int source, int target) const noexcept override
        {
            if (_adj.size() <= static_cast<size_t>(source))
                return false;

            for (auto v: _adj[source])
                if (v == target)
                    return true;

            return false;
        }

        [[nodiscard]] auto neighborsCount(int vertex) const noexcept
            -> int override
        {
            if (_adj.size() <= static_cast<size_t>(vertex))
                return 0;

            return static_cast<int>(_adj[vertex].size());
        }

        /// @brief Если вершины нет (неверный индекс), возвращает пустой span.
        [[nodiscard]] auto getNeighbors(int vertex) const noexcept
            -> std::span<int const> override
        {
            if (_adj.size() <= static_cast<size_t>(vertex))
                return {};

            return _adj[vertex];
        }

        /////////////////////////////////////////////////////
        // Реализация интерфейса EditableAdjacencyList

        auto addVertex() -> int override
        {
            auto const index = static_cast<int>(_adj.size());
            _adj.emplace_back();
            return index;
        }

        bool connect(int source, int target) override
        {
            auto const max_required_size = std::max<size_t>(source, target) + 1;
            if (_adj.size() < max_required_size)
                _adj.resize(max_required_size);

            _adj[source].push_back(target);
            return true;
        }

        bool disconnect(int source, int target) override
        {
            if (_adj.size() <= static_cast<size_t>(source))
                return false;

            return std::ranges::contains(_adj[source], target);
        }

        /////////////////////////////////////////////////////
        // Операции конструирования

        AdjacencyListVector() noexcept = default;

        explicit AdjacencyListVector(int vertexCount)
            : _adj(vertexCount)
        {
            // Пусто.
        }

    private:
        using Adjacency = std::vector<int>;
        std::vector<Adjacency> _adj;
    };


    auto newAdjacencyListVector(int vertexCount = 0)
        -> std::unique_ptr<EditableAdjacencyList>
    {
        return std::make_unique<AdjacencyListVector>(vertexCount);
    }

}
