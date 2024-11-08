/// @file graph.hpp
#ifndef GRAVIS24_GRAPH_HPP
#define GRAVIS24_GRAPH_HPP

#include <span>
#include <cstdint>

namespace gravis24
{

    struct Arc
    {
        int source;
        int target;
    };

    struct XYZ
    {
        float x;
        float y;
        float z;
    };


    class EdgeListView
    {
    public:
        virtual ~EdgeListView() = default;

        /// @brief Получить все рёбра.
        [[nodiscard]] virtual auto getEdges() const noexcept
            -> std::span<Arc const> = 0;

        /// @brief Узнать сколько целочисленных атрибутов назначено на рёбра.
        [[nodiscard]] virtual auto getIntAttributeCount() const noexcept
            -> int = 0;
        
        /// @brief Получить массив целочисленных атрибутов для всех рёбер (индекс ребра как в getEdges).
        /// @param attributeIndex < getIntAttributeCount() или возвращает пустой span
        [[nodiscard]] virtual auto getIntAttributes(int attributeIndex) const noexcept
            -> std::span<int const> = 0;

        /// @brief Узнать сколько атрибутов типа float назначено на рёбра.
        [[nodiscard]] virtual auto getFloatAttributeCount() const noexcept
            -> int = 0;

        /// @brief Получить массив атрибутов float для всех рёбер (индекс ребра как в getEdges).
        /// @param attributeIndex < getFloatAttributeCount() или возвращает пустой span
        [[nodiscard]] virtual auto getFloatAttributes(int attributeIndex) const noexcept
            -> std::span<float const> = 0;
    };


    class AdjacencyMatrixView
    {
    public:
        class Row
        {
        public:
            using Chunk = uint32_t;

            Row() noexcept = default;

            explicit Row(Chunk const* data, int first_bit_offset = 0) noexcept
                : _data(data)
                , _offset(first_bit_offset)
            {
                // Пусто.
            }
            
            [[nodiscard]] bool operator[](int index) const noexcept
            {
                constexpr auto chunk_bits = 8 * sizeof(Chunk);
                auto const bit_index  = unsigned(index + _offset);
                auto const chunk      = bit_index / chunk_bits;
                auto const bit_offset = bit_index % chunk_bits;
                return ((_data[chunk] >> bit_offset) & 1) == 1;
            }

            [[nodiscard]] bool isValid() const noexcept
            {
                return _data != nullptr;
            }

        private:
            Chunk const* _data   {};
            int          _offset {};
        };

        virtual ~AdjacencyMatrixView() = default;

        virtual auto getVertexCount() const noexcept
            -> int = 0;
        
        /// @brief Получить строку матрицы смежности
        /// @param index < getVertexCount() или возвращает пустой (невалидный) Row
        virtual auto getRow(int index) const noexcept
            -> Row = 0;
    };


    class Graph
    {
    public:
        virtual ~Graph() = default;

        [[nodiscard]] virtual auto getVertexCount() const noexcept
            -> int = 0;

        friend class ChangeableVertexPositions
        {
        public:
            ChangeableVertexPositions(Graph& graph, std::span<XYZ> vertexPositions)
                : _graph(graph)
                , _vertexPositions(vertexPositions)
            {
                // Пусто.
            }

            [[nodiscard]] auto operator()() const noexcept
            {
                return _vertexPositions;
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


        [[nodiscard]] virtual auto getVertexPositions() const noexcept
            -> std::span<XYZ const> = 0;
        [[nodiscard]] virtual auto getVertexPositions() noexcept
            -> ChangeableVertexPositions = 0;


        [[nodiscard]] virtual auto getEdgeListView() const
            -> EdgeListView const& = 0;

        [[nodiscard]] virtual auto getAdjacencyMatrixView() const
            -> AdjacencyMatrixView const& = 0;


    private:
        virtual void onVertexPositionsChange() = 0;
    };

}

#endif//GRAVIS24_GRAPH_HPP
