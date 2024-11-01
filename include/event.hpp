/// @file event.hpp
/// 
/// @brief Описание возможных событий в Gravis24. Предварительная версия.
/// 
/// (Конкретный уровень)
/// Что можно делать при анимации:
/// 1. Менять атрибут вершины или ребра графа.
/// 2. Менять дополнительные визуализируемые структуры данных.
/// 
/// Возможные атрибуты:
/// 1. Цвет (RGBA, индекс, название?).
/// 2. Размер (радиус или толщина).
/// 3. Координаты вершины.
/// 4. Текст (номер метки, текст).
/// 
/// (Абстрактный уровень)
/// v -- номер вершины;
/// u, v -- номера вершин, соединённых дугой (ребром);
/// n -- номер метки;
/// label -- текст метки (только число?);
/// s -- номер визуализируемого стека;
/// item -- номер элемента.
/// 
/// 1. Вершина посещена (v).
/// 2. Вершина покинута (v).
/// 3. Прямая дуга (u, v).
/// 4. Обратная дуга (u, v).
/// 5. Изменена текстовая метка вершины (v, n, label).
/// 6. Изменена текстовая метка ребра (u, v, n, label).
/// 7. Задан элемент (s, item, label)
/// 8. Удалён элемент (s, item)
/// 9. Помечен произвольный элемент стека (s, item).
/// 
#ifndef GRAVIS24_EVENT_HPP
#define GRAVIS24_EVENT_HPP

#include <variant>
#include <cstdint>


namespace gravis24
{

    namespace events
    {
        
        // Типы вложенных значений

        struct Arc
        {
            int source;
            int target;
        };

        struct RGBA
        {
            uint8_t red;
            uint8_t green;
            uint8_t blue;
            uint8_t alpha;
        };

        struct XYZ
        {
            float x;
            float y;
            float z;
        };


        // "Конкретные" события

        struct VertexColorChanged
        {
            int vertex;
            RGBA rgb;
        };

        struct ArcColorChanged
        {
            Arc arc;
            RGBA rgb;
        };

        struct VertexRadiusChanged
        {
            int vertex;
            float radius;
        };

        struct ArcWidthChanged
        {
            Arc arc;
            float width;
        };

        struct VertexPositionChanged
        {
            int vertex;
            XYZ xyz;
        };


        // "Абстрактные" события

        struct VertexIsOpened
        {
            int vertex;
        };

        struct VertexIsClosed
        {
            int vertex;
        };

        struct ArcIsForward
        {
            Arc arc;
        };

        struct ArcIsBackward
        {
            Arc arc;
        };

        struct VertexLabelIsChanged
        {
            int vertex;
            int label;
            int label_index;
        };

        struct ArcLabelIsChanged
        {
            Arc arc;
            int label;
            int label_index;
        };

        struct ItemIsSet
        {
            int item_value;
            int item_index;
            int array_index;
        };

        struct ItemIsRemoved
        {
            int item_index;
            int array_index;
        };

        struct ItemIsMarked
        {
            int item_index;
            int array_index;
        };

    } // events

    using Event = std::variant<
                        VertexColorChanged,
                        ArcColorChanged,
                        VertexRadiusChanged,
                        ArcWidthChanged,
                        VertexPositionChanged,
                        VertexIsOpened,
                        VertexIsClosed,
                        ArcIsForward,
                        ArcIsBackward,
                        VertexLabelIsChanged,
                        ArcLabelIsChanged,
                        ItemIsSet,
                        ItemIsRemoved,
                        ItemIsMarked
                     >;

}

#endif//GRAVIS24_EVENT_HPP
