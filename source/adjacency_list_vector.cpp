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
            return _adj.size() <= static_cast<size_t>(source)
                && std::ranges::contains(_adj[source].arcs, 
                    [](ArcData const& ad) { return ad.target; });
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

        [[nodiscard]] auto getVertexIntAttribute(int vertex, int attribute) const noexcept
            -> int override
        {

        }

        [[nodiscard]] auto getVertexFloatAttribute(int vertex, int attribute) const noexcept
            -> float override
        {

        }

        [[nodiscard]] auto getArcIntAttribute(
            int source, int target, int attribute) const noexcept
            -> int override
        {

        }

        [[nodiscard]] auto getArcFloatAttribute(
            int source, int target, int attribute) const noexcept
            -> float override
        {

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

        void setVertexIntAttribute(
            int vertex, int attribute, int value) override
        {

        }

        void setVertexFloatAttribute(
            int vertex, int attribute, float value) override
        {

        }

        void setArcIntAttribute(
            int source, int target, int attribute, int value) override
        {

        }

        virtual void getArcFloatAttribute(
            int source, int target, int attribute, float value) override
        {

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
        struct ArcData
        {
            int                target;
            std::vector<int>   intAttrs;
            std::vector<float> floatAttrs;
        };

        struct Adjacency
        {
            std::vector<ArcData> arcs;
            std::vector<int>     intAttrs;
            std::vector<float>   floatAttrs;
        };

        std::vector<Adjacency> _adj;
    };


    auto newAdjacencyListVector(int vertexCount = 0)
        -> std::unique_ptr<EditableAdjacencyList>
    {
        return std::make_unique<AdjacencyListVector>(vertexCount);
    }

}
