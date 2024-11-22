/// @file adjacency_list.hpp
#ifndef GRAVIS24_ADJACENCY_LIST_HPP
#define GRAVIS24_ADJACENCY_LIST_HPP

#include <memory>

namespace gravis24
{

    ///////////////////////////////////////////////////////
    // Интерфейс AdjacencyListView

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


    ///////////////////////////////////////////////////////
    // Интерфейс EditableAdjacencyList

    class EditableAdjacencyList
        : public AdjacencyListView
    {
    public:
        /// @brief  Добавить новую вершину (получает наибольший индекс).
        /// @return индекс добавленной вершины
        virtual auto addVertex() -> int = 0;

        /// @brief        Добавить target в окрестность source.
        /// @param source исходная вершина дуги
        /// @param target целевая вершина дуги
        /// @return       true, если дуга была добавлена, false иначе
        virtual bool connect(int source, int target) = 0;

        /// @brief        Удалить target из окрестности source.
        /// @param source исходная вершина дуги
        /// @param target целевая вершина дуги
        /// @return       true, если дуга была удалена, false иначе
        virtual bool disconnect(int source, int target) = 0;
    };


    //////////////////////////////////////////////////
    // Функции для создания объектов, реализующих
    // EditableAdjacencyList

    [[nodiscard]] auto newAdjacencyListVector(int vertexCount = 0)
        -> std::unique_ptr<EditableAdjacencyList>;

}

#endif//GRAVIS24_ADJACENCY_LIST_HPP
