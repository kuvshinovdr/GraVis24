/// @file graph.hpp
#ifndef GRAVIS24_GRAPH_HPP
#define GRAVIS24_GRAPH_HPP

#include <span>
#include <climits>
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
        class RowView
        {
        public:
            using Chunk = uint32_t;
            static constexpr auto chunkBits = 8 * sizeof(Chunk);

            RowView() noexcept = default;

            explicit RowView(Chunk const* data, int firstBitOffset = 0) noexcept
                : _data(data)
                , _offset(firstBitOffset)
            {
                // Пусто.
            }
            
            [[nodiscard]] bool getBit(int index) const noexcept
            {
                auto const bitIndex  = unsigned(index + _offset);
                auto const chunk     = bitIndex / chunkBits;
                auto const bitOffset = bitIndex % chunkBits;
                return ((_data[chunk] >> bit_offset) & 1) == 1;
            }

            [[nodiscard]] bool operator[](int index) const noexcept
            {
                return get(index);
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

        [[nodiscard]] virtual auto getVertexCount() const noexcept
            -> int = 0;
        
        /// @brief Получить строку матрицы смежности
        /// @param index < getVertexCount() или возвращает пустой (невалидный) Row
        [[nodiscard]] virtual auto getRow(int index) const noexcept
            -> RowView = 0;

        [[nodiscard]] auto operator[](int index) const noexcept
            -> RowView
        {
            return getRow(index);
        }
    };


    class AdjacencyListView
    {
    public:
        virtual ~AdjacencyListView() = default;

        [[nodiscard]] virtual auto getVertexCount() const noexcept
            -> int = 0;

        [[nodiscard]] virtual bool areConnected(int source, int target) const noexcept 
            = 0;

        [[nodiscard]] virtual auto neighborsCount(int vertex) const noexcept
            -> int = 0;

        /// @brief Если вершины нет (неверный индекс), возвращает пустой span.
        [[nodiscard]] virtual auto getNeighbors(int vertex) const noexcept
            -> std::span<int const> = 0;
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
            -> AdjacencyMatrixView const& = 0;

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

        friend class ChangeableVertexPositions
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

    private:
        virtual void onVertexPositionsChange() noexcept = 0;
    };

}

#endif//GRAVIS24_GRAPH_HPP
