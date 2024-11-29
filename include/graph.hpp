/// @file graph.hpp
#ifndef GRAVIS24_GRAPH_HPP
#define GRAVIS24_GRAPH_HPP

#include "edge_list.hpp"
#include "dense_adjacency_matrix.hpp"
#include "adjacency_list.hpp"

#include <climits>
#include <cstdint>

namespace gravis24
{

    struct XYZ
    {
        float x;
        float y;
        float z;
    };


    class Graph
    {
    public:
        virtual ~Graph() = default;

        [[nodiscard]] virtual auto getVertexCount() const noexcept
            -> int = 0;
        [[nodiscard]] virtual auto getEdgeCount() const noexcept
            -> int = 0;

        [[nodiscard]] virtual bool hasEdgeListView() const noexcept
            = 0;
        [[nodiscard]] virtual auto getEdgeListView() const
            -> EdgeListView const& = 0;

        [[nodiscard]] virtual bool hasAdjacencyMatrixView() const noexcept
            = 0;
        [[nodiscard]] virtual auto getAdjacencyMatrixView() const
            -> DenseAdjacencyMatrixView const& = 0;

        [[nodiscard]] virtual bool hasAdjacencyListView() const noexcept
            = 0;
        [[nodiscard]] virtual auto getAdjacencyListView() const
            -> AdjacencyListView const& = 0;

        /// @brief        Добавить дугу, если её нет.
        /// @param source исходная вершина
        /// @param target целевая вершина
        /// @return       true, если дуга была добавлена, иначе false (дуга уже была)
        virtual bool connect(int source, int target)
            = 0;

        /// @brief        Удалить дугу, если она есть.
        /// @param source исходная вершина
        /// @param target целевая вершина
        /// @return       true, если дуга была удалена, иначе false (дуги уже не было)
        virtual bool disconnect(int source, int target)
            = 0;

        virtual bool areConnected(int source, int target) const noexcept
            = 0;
        
        friend class ChangeableVertexPositions;

        [[nodiscard]] virtual auto getVertexPositions() const noexcept
            -> std::span<XYZ const> = 0;

        [[nodiscard]] virtual auto getVertexPositions() noexcept
            -> ChangeableVertexPositions = 0;

        // Отдельная обработка атрибутов вершин и рёбер



    private:
        virtual void onVertexPositionsChange() noexcept = 0;
    };


    // Данный класс нужен, чтобы вызвать Graph::onVertexPositionsChange
    // после изменений значений координат вершин (в деструкторе).
    class ChangeableVertexPositions
    {
    public:
        ChangeableVertexPositions(Graph& graph, std::span<XYZ> vertexPositions)
            : _graph(graph)
            , _vertexPositions(vertexPositions)
        {
            // Пусто.
        }

        [[nodiscard]] bool empty() const noexcept
        {
            return _vertexPositions.empty();
        }

        [[nodiscard]] auto size() const noexcept
            -> int
        {
            return static_cast<int>(_vertexPositions.size());
        }

        [[nodiscard]] auto operator[](int index) const noexcept
            -> XYZ const&
        {
            return _vertexPositions[index];
        }

        [[nodiscard]] auto getSpan() const noexcept
        {
            return _vertexPositions;
        }

        [[nodiscard]] auto begin() const noexcept
        {
            return _vertexPositions.begin();
        }

        [[nodiscard]] auto end() const noexcept
        {
            return _vertexPositions.end();
        }

        [[nodiscard]] auto getGraph() const noexcept
            -> Graph&
        {
            return _graph;
        }

        ~ChangeableVertexPositions()
        {
            _graph.onVertexPositionsChange();
        }

    private:
        ChangeableVertexPositions(ChangeableVertexPositions const&) 
            = delete;
        auto operator=(ChangeableVertexPositions const&)
            -> ChangeableVertexPositions& = delete;
        // Также автоматически отключает 
        // перемещающие конструктор и оператор=.

        Graph&         _graph;
        std::span<XYZ> _vertexPositions;
    };

}

#endif//GRAVIS24_GRAPH_HPP
