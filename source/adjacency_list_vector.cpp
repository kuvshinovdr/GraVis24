/// @file  adjacency_list_vector.cpp
/// @brief Реализация EditableAdjacencyList поверх vector vector int.
#include "../include/adjacency_list.hpp"

#include <vector>
#include <iterator>
#include <algorithm>

namespace gravis24
{

    // Элементы реализации.
    namespace
    {

        // Объединяет в себе наборы типизированных атрибутов
        class AttributesBase
        {
        protected:
            ~AttributesBase() noexcept = default;

        public:
            void resizeIntAttrs(int count)
            {
                _intAttrs.resize(count);
            }

            void reserveIntAttrs(int capacity)
            {
                _intAttrs.reserve(capacity);
            }

            void resizeFloatAttrs(int count)
            {
                _floatAttrs.resize(count);
            }

            void reserveFloatAttrs(int capacity)
            {
                _floatAttrs.reserve(capacity);
            }

            template <typename AttrType>
            void resizeAttrs(int count)
            {
                if constexpr (std::is_same_v<AttrType, int>)
                {
                    resizeIntAttrs(count);
                }
                else
                {
                    static_assert(std::is_same_v<AttrType, float>);
                    resizeFloatAttrs(count);
                }
            }

            template <typename AttrType>
            void reserveAttrs(int count)
            {
                if constexpr (std::is_same_v<AttrType, int>)
                {
                    reserveIntAttrs(count);
                }
                else
                {
                    static_assert(std::is_same_v<AttrType, float>);
                    reserveFloatAttrs(count);
                }
            }

            [[nodiscard]] auto getIntAttrs() noexcept
                -> std::span<int>
            {
                return _intAttrs;
            }

            [[nodiscard]] auto getIntAttrs() const noexcept
                -> std::span<int const>
            {
                return _intAttrs;
            }

            [[nodiscard]] auto getFloatAttrs() noexcept
                -> std::span<float>
            {
                return _floatAttrs;
            }

            [[nodiscard]] auto getFloatAttrs() const noexcept
                -> std::span<float const>
            {
                return _floatAttrs;
            }

            template <typename AttrType, typename Self>
            [[nodiscard]] auto getAttrs(this Self&& self) noexcept
            {
                if constexpr (std::is_same_v<AttrType, int>)
                {
                    return self.getIntAttrs();
                }
                else
                {
                    static_assert(std::is_same_v<AttrType, float>);
                    return self.getFloatAttrs();
                }
            }

        private:
            std::vector<int>   _intAttrs;
            std::vector<float> _floatAttrs;
        };


        class ArcAttributes
            : public AttributesBase
        {
            // Пусто.
        };


        // Инварианты (причина для оформления в отдельный класс):
        // targets.size() == arcsAttrs.size()
        // exists s1, s2 forall x in arcsAttrs:
        //          x.intAttrs.size() == s1 && x.floatAttrs.size() == s2
        // -- у каждой вершины одинаковое число атрибутов.
        class Vertex
            : public AttributesBase
        {
        public:
            void resizeArcs(int count)
            {
                auto const oldSize = _targets.size();
                auto const newSize = static_cast<size_t>(count);

                _targets.resize(newSize);
                _arcsAttrs.resize(newSize);

                if (0 < oldSize && oldSize < newSize)
                {
                    auto const intAttrsCount   = _arcsAttrs[0].getIntAttrs().size();
                    auto const floatAttrsCount = _arcsAttrs[0].getFloatAttrs().size();

                    for (size_t i = oldSize; i < newSize; ++i)
                    {
                        auto& arc = _arcsAttrs[i];
                        arc.resizeIntAttrs(intAttrsCount);
                        arc.resizeFloatAttrs(floatAttrsCount);
                    }
                }
            }

            void reserveArcs(int capacity)
            {
                _targets.reserve(capacity);
                _arcsAttrs.reserve(capacity);
            }

            void resizeArcsIntAttrs(int count)
            {
                for (auto& arc: _arcsAttrs)
                    arc.resizeIntAttrs(count);
            }

            void reserveArcsIntAttrs(int capacity)
            {
                for (auto& arc: _arcsAttrs)
                    arc.reserveIntAttrs(capacity);
            }

            void resizeArcsFloatAttrs(int count)
            {
                for (auto& arc: _arcsAttrs)
                    arc.resizeFloatAttrs(count);
            }

            void reserveArcsFloatAttrs(int capacity)
            {
                for (auto& arc: _arcsAttrs)
                    arc.reserveFloatAttrs(capacity);
            }

            template <typename AttrType>
            void resizeArcsAttrs(int count)
            {
                if constexpr (std::is_same_v<AttrType, int>)
                {
                    resizeArcsIntAttrs(count);
                }
                else
                {
                    static_assert(std::is_same_v<AttrType, float>);
                    resizeArcsFloatAttrs(count);
                }
            }

            template <typename AttrType>
            void reserveArcsAttrs(int count)
            {
                if constexpr (std::is_same_v<AttrType, int>)
                {
                    reserveArcsIntAttrs(count);
                }
                else
                {
                    static_assert(std::is_same_v<AttrType, float>);
                    reserveArcsFloatAttrs(count);
                }
            }

            [[nodiscard]] auto getTargets() const noexcept
                -> std::span<int const>
            {
                return _targets;
            }

            [[nodiscard]] auto getTargets() noexcept
                -> std::span<int>
            {
                return _targets;
            }

            [[nodiscard]] auto getArcsAttrs() const noexcept
                -> std::span<ArcAttributes const>
            {
                return _arcsAttrs;
            }

            [[nodiscard]] auto getArcsAttrs() noexcept
                -> std::span<ArcAttributes>
            {
                return _arcsAttrs;
            }

        private:
            std::vector<int>            _targets;
            std::vector<ArcAttributes>  _arcsAttrs;
            std::vector<int>            _intAttrs;
            std::vector<float>          _floatAttrs;
        };


        // Инварианты:
        // exists s1, s2 forall x in _vd:
        //      x.intAttrs.size() == s1 && x.floatAttrs.size() == s2
        // размеры вложенных в arcsAttrs intAttrs и floatAttrs должны совпадать у разных вершин
        class VertexData
            : private std::vector<Vertex>
        {
            using Base = std::vector<Vertex>;

        public:
            using Base::empty;
            using Base::data;
            using Base::size;
            using Base::begin;
            using Base::end;
            using Base::front;
            using Base::back;
            using Base::operator[];
            using Base::reserve;

            // Задать правильные размеры для новых вложенных массивов.
            void resize(int count)
            {
                auto const oldSize = size();
                auto const newSize = static_cast<size_t>(count);

                Base::resize(newSize);

                if (0 < oldSize && oldSize < newSize)
                {
                    auto const intAttrsCount   = front().getIntAttrs().size();
                    auto const floatAttrsCount = front().getFloatAttrs().size();

                    auto const arcAttrs = front().getArcsAttrs();
                    auto const arcIntAttrsCount = 
                        arcAttrs.empty()? 0: arcAttrs.front().getIntAttrs().size();
                    auto const arcFloatAttrsCount = 
                        arcAttrs.empty()? 0: arcAttrs.front().getFloatAttrs().size();

                    for (size_t i = oldSize; i < newSize; ++i)
                    {
                        auto& vertex = (*this)[i];
                        vertex.resizeIntAttrs(intAttrsCount);
                        vertex.resizeFloatAttrs(floatAttrsCount);
                        vertex.resizeArcsIntAttrs(arcIntAttrsCount);
                        vertex.resizeArcsFloatAttrs(arcFloatAttrsCount);
                    }
                }
            }
        };

    }


    class AdjacencyListVector
        : public EditableAdjacencyList
    {
    public:
        /////////////////////////////////////////////////////
        // Реализация интерфейса AdjacencyListView

        [[nodiscard]] auto getVertexCount() const noexcept
            -> int override
        {
            return static_cast<int>(_vd.size());
        }

        [[nodiscard]] bool areConnected(int source, int target) const noexcept override
        {
            return isValidVertex(source)
                && std::ranges::contains(_vd[source].getTargets(), target);
        }

        [[nodiscard]] auto neighborsCount(int vertex) const noexcept
            -> int override
        {
            return isValidVertex(vertex)?
                static_cast<int>(_vd[vertex].getTargets().size()): 0;
        }

        /// @brief Если вершины нет (неверный индекс), возвращает пустой span.
        [[nodiscard]] auto getNeighbors(int vertex) const noexcept
            -> std::span<int const> override
        {
            return isValidVertex(vertex)?
                _vd[vertex].getTargets(): std::span<int const>{};
        }

        // Как поступать, если неверны параметры (например, индексы)?
        // 1) игнорировать (возможно, протоколировать), считая, что они всегда верны (=> UB);
        // 2) бросить исключение;
        // 3) вернуть особое значение (std::optional, std::expected, nullptr и т.д.);
        // 4) доопределить.
        // -- будем возвращать 0.
        [[nodiscard]] auto getVertexIntAttribute(int vertex, int attribute) const noexcept
            -> int override
        {
            return isValidIntVertexAttr(vertex, attribute)?
                _vd[vertex].getIntAttrs()[attribute]: 0;
        }

        [[nodiscard]] auto getVertexFloatAttribute(int vertex, int attribute) const noexcept
            -> float override
        {
            return isValidFloatVertexAttr(vertex, attribute)?
                _vd[vertex].getFloatAttrs()[attribute]: 0;
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
            auto const index = static_cast<int>(_vd.size());
            _vd.resize(_vd.size() + 1);
            return index;
        }

        bool connect(int source, int target) override
        {
            auto const max_required_size = std::max<size_t>(source, target) + 1;
            if (_vd.size() < max_required_size)
                _vd.resize(max_required_size);

            auto&      vertex   = _vd[source];
            auto const arcIndex = vertex.getTargets().size();
            vertex.resizeArcs(arcIndex + 1);
            vertex.getTargets()[arcIndex] = target;
            return true;
        }

        bool disconnect(int source, int target) override
        {
            if (_vd.size() <= static_cast<size_t>(source))
                return false;

            return std::ranges::contains(_vd[source].getTargets(), target);
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

        void setArcFloatAttribute(
            int source, int target, int attribute, float value) override
        {

        }


        /////////////////////////////////////////////////////
        // Операции конструирования

        AdjacencyListVector() noexcept = default;

        explicit AdjacencyListVector(int vertexCount)
        {
            _vd.resize(vertexCount);
        }

    private:
        VertexData _vd;


        [[nodiscard]] bool isValidVertex(int vertexIndex) const noexcept
        {
            return static_cast<size_t>(vertexIndex) < _vd.size();
        }

        [[nodiscard]] bool isValidIntVertexAttr(int vertexIndex, int attrIndex) const noexcept
        {
            return isValidVertex(vertexIndex) 
                && static_cast<size_t>(attrIndex) < _vd[vertexIndex].getIntAttrs().size();
        }

        [[nodiscard]] bool isValidFloatVertexAttr(int vertexIndex, int attrIndex) const noexcept
        {
            return isValidVertex(vertexIndex) 
                && static_cast<size_t>(attrIndex) < _vd[vertexIndex].getFloatAttrs().size();
        }
        
        template <typename AttrType, typename Self>
        [[nodiscard]] auto getArcAttrs(this Self&& self, int source, int target) noexcept
        {
            if (self.isValidVertex(source))
            {
                auto& sourceData = self._vd[source];
                auto& targets    = sourceData.getTargets();
                auto  it         = std::ranges::find(targets, target);
                if (it != targets.end())
                {
                    auto  index = std::distance(targets.begin(), it);
                    return sourceData.getArcsAttrs()[index]
                }
            }
        }


        [[nodiscard]] bool isValidIntArcAttr(int source, int target, int attrIndex) const noexcept
        {
            
            return static_cast<size_t>(attrIndex) < attrs.size();
        }

        [[nodiscard]] bool isValidFloatArcAttr(int source, int target, int attrIndex) const noexcept
        {
            return isValidVertex(source) 
                && static_cast<size_t>(attrIndex) < _vd[vertexIndex].floatAttrs.size();
        }

        [[nodiscard]] static auto arcIndex(VertexData const& vd, int target) noexcept
        {
            return std::distance(vd.arcs.begin(),
                std::ranges::find(vd.arcs, target,
                    [](ArcData const& ad) { return ad.target; }));
        }
    };


    auto newAdjacencyListVector(int vertexCount = 0)
        -> std::unique_ptr<EditableAdjacencyList>
    {
        return std::make_unique<AdjacencyListVector>(vertexCount);
    }

}
