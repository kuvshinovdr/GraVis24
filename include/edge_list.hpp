/// @file edge_list.hpp
#ifndef GRAVIS24_EDGE_LIST_HPP
#define GRAVIS24_EDGE_LIST_HPP

#include <span>
#include <memory> // unique_ptr

namespace gravis24
{

    struct Arc
    {
        int source;
        int target;

        friend auto operator<=>(Arc const&, Arc const&) noexcept = default;
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


    class EditableEdgeList
        : public EdgeListView
    {
    public:
        /// @brief        Добавить дугу.
        /// @param source исходная вершина
        /// @param target целевая вершина
        /// @return       номер дуги, соединяющей source и target
        virtual auto connect(int source, int target)
            -> int = 0;

        /// @brief        Проверить, есть ли дуга, соединяющая заданные вершины.
        /// @param source исходная вершина
        /// @param target целевая вершина
        [[nodiscard]] virtual bool areConnected(int source, int target) const noexcept = 0;

        /// @brief        Удалить дугу, если она есть.
        /// @param source исходная вершина
        /// @param target целевая вершина
        /// @return       true, если дуга была удалена, иначе false
        virtual bool disconnect(int source, int target) = 0;

        [[nodiscard]] virtual auto getIntAttributes(int attributeIndex) noexcept
            -> std::span<int> = 0;

        [[nodiscard]] virtual auto getFloatAttributes(int attributeIndex) noexcept
            -> std::span<float> = 0;
    };


    //////////////////////////////////////////////////
    // Функции для создания объектов, реализующих
    // EditableEdgeList

    [[nodiscard]] auto newEdgeListUnsortedVector(
            int preallocArcsCount = 0,
            int intAttrsCount     = 0, 
            int floatAttrsCount   = 0
        ) -> std::unique_ptr<EditableEdgeList>;

}

#endif//GRAVIS24_EDGE_LIST_HPP
