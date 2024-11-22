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

        /// @brief           Получить значение целочисленного атрибута вершины.
        /// @param vertex    номер вершины, которой принадлежит атрибут
        /// @param attribute номер атрибута
        /// @return          значение атрибута; возвращает 0, если атрибут не существует
        [[nodiscard]] virtual auto getVertexIntAttribute(int vertex, int attribute) const noexcept
            -> int = 0;

        /// @brief           Получить значение атрибута типа float заданной вершины.
        /// @param vertex    номер вершины, которой принадлежит атрибут
        /// @param attribute номер атрибута
        /// @return          значение атрибута; возвращает 0, если атрибут не существует
        [[nodiscard]] virtual auto getVertexFloatAttribute(int vertex, int attribute) const noexcept 
            -> float = 0;

        /// @brief           Получить значение целочисленного атрибута дуги.
        /// @param source    номер исходной вершины дуги
        /// @param target    номер целевой вершины дуги
        /// @param attribute номер атрибута
        /// @return          значение атрибута; возвращает 0, если атрибут не существует
        [[nodiscard]] virtual auto getArcIntAttribute(
            int source, int target, int attribute) const noexcept
            -> int = 0;

        /// @brief           Получить значение атрибута типа float заданной дуги.
        /// @param source    номер исходной вершины дуги
        /// @param target    номер целевой вершины дуги
        /// @param attribute номер атрибута
        /// @return          значение атрибута; возвращает 0, если атрибут не существует
        [[nodiscard]] virtual auto getArcFloatAttribute(
            int source, int target, int attribute) const noexcept 
            -> float = 0;
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

        /// @brief           Установить значение целочисленного атрибута вершины.
        /// @param vertex    номер вершины, которой принадлежит атрибут
        /// @param attribute номер атрибута
        /// @param value     значение атрибута
        virtual void setVertexIntAttribute(
            int vertex, int attribute, int value) = 0;

        /// @brief           Установить значение атрибута типа float заданной вершины.
        /// @param vertex    номер вершины, которой принадлежит атрибут
        /// @param attribute номер атрибута
        /// @param value     значение атрибута
        virtual void setVertexFloatAttribute(
            int vertex, int attribute, float value) = 0;

        /// @brief           Установить значение целочисленного атрибута дуги.
        /// @param source    номер исходной вершины дуги
        /// @param target    номер целевой вершины дуги
        /// @param attribute номер атрибута
        /// @param value     значение атрибута
        virtual void setArcIntAttribute(
            int source, int target, int attribute, int value) = 0;

        /// @brief           Установить значение атрибута типа float заданной дуги.
        /// @param source    номер исходной вершины дуги
        /// @param target    номер целевой вершины дуги
        /// @param attribute номер атрибута
        /// @param value     значение атрибута
        [[nodiscard]] virtual void getArcFloatAttribute(
            int source, int target, int attribute, float value) = 0;
    };


    //////////////////////////////////////////////////
    // Функции для создания объектов, реализующих
    // EditableAdjacencyList

    [[nodiscard]] auto newAdjacencyListVector(int vertexCount = 0)
        -> std::unique_ptr<EditableAdjacencyList>;

}

#endif//GRAVIS24_ADJACENCY_LIST_HPP
