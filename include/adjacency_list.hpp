/// @file adjacency_list.hpp
#ifndef GRAVIS24_ADJACENCY_LIST_HPP
#define GRAVIS24_ADJACENCY_LIST_HPP

#include <span>
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

        [[nodiscard]] virtual auto getTargetCount(int vertex) const noexcept
            -> int = 0;

        /// @brief Если вершины нет (неверный индекс), возвращает пустой span.
        [[nodiscard]] virtual auto getTargets(int vertex) const noexcept
            -> std::span<int const> = 0;

        [[nodiscard]] auto getVertexIntAttributeCount() const noexcept
            -> int = 0;
        [[nodiscard]] virtual auto getVertexFloatAttributeCount() const noexcept
            -> int = 0;
        [[nodiscard]] virtual auto getArcIntAttributeCount() const noexcept
            -> int = 0;
        [[nodiscard]] virtual auto getArcFloatAttributeCount() const noexcept
            -> int = 0;

        /// @brief           Получить массив целочисленных атрибутов вершины.
        /// @param vertex    номер вершины, которой принадлежат атрибуты
        [[nodiscard]] virtual auto getVertexIntAttributes(int vertex) const noexcept
            -> std::span<int const> = 0;

        /// @brief           Получить массив атрибутов типа float заданной вершины.
        /// @param vertex    номер вершины, которой принадлежат атрибуты
        [[nodiscard]] virtual auto getVertexFloatAttributes(int vertex) const noexcept 
            -> std::span<float const> = 0;

        using ArcHandle = void*;

        [[nodiscard]] virtual auto getArc(int source, int target) const noexcept
            -> ArcHandle = 0;
        
        [[nodiscard]] virtual auto getArcIntAttributes(ArcHandle) const noexcept
            -> std::span<int const>;

        [[nodiscard]] virtual auto getArcFloatAttributes(ArcHandle) const noexcept
            -> std::span<float const>;
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

        [[nodiscard]] virtual auto getVertexIntAttributes(int vertex) noexcept
            -> std::span<int> = 0;

        [[nodiscard]] virtual auto getVertexFloatAttributes(int vertex) noexcept 
            -> std::span<float> = 0;

        [[nodiscard]] virtual auto getArcIntAttributes(ArcHandle) noexcept
            -> std::span<int>;

        [[nodiscard]] virtual auto getArcFloatAttributes(ArcHandle) noexcept
            -> std::span<float>;
    };


    //////////////////////////////////////////////////
    // Функции для создания объектов, реализующих
    // EditableAdjacencyList

    [[nodiscard]] auto newAdjacencyListVector(int vertexCount = 0)
        -> std::unique_ptr<EditableAdjacencyList>;

}

#endif//GRAVIS24_ADJACENCY_LIST_HPP
