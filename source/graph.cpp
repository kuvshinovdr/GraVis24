/// @file graph.cpp
#include "../include/graph.hpp"

#include <type_traits>
#include <algorithm>
#include <vector>

#include <doctest/doctest.h>


namespace gravis24
{

    struct ArcDataSizes
    {
        int arcCount;
        int intAttrCount;
        int floatAttrCount;
    };


    [[nodiscard]] auto obtainArcDataSizes(
                EdgeListView const& el) noexcept
        -> ArcDataSizes
    {
        return
        {
            .arcCount       = static_cast<int>(el.getArcs().size()),
            .intAttrCount   = el.getIntAttributeCount(),
            .floatAttrCount = el.getFloatAttributeCount()
        };
    }


    [[nodiscard]] auto obtainArcDataSizes(
                DenseAdjacencyMatrixView const& am) noexcept
        -> ArcDataSizes
    {
        int const vertexCount = am.getVertexCount();
        int arcCount = 0;
        for (int v = 0; v < vertexCount; ++v)
            arcCount += am.getRow(v).computeSetBits(v);

        return
        {
            .arcCount       = arcCount,
            .intAttrCount   = 0,
            .floatAttrCount = 0
        };
    }


    [[nodiscard]] auto obtainArcDataSizes(
                AdjacencyListView const& al) noexcept
        -> ArcDataSizes
    {
        int const vertexCount = al.getVertexCount();
        int arcCount = 0;
        for (int v = 0; v < vertexCount; ++v)
            arcCount += al.getTargetCount(v);

        return
        {
            .arcCount       = arcCount,
            .intAttrCount   = al.getArcIntAttributeCount(),
            .floatAttrCount = al.getArcFloatAttributeCount()
        };
    }


    template <typename F>
    concept ArcAttributesVisitorFunction =
        std::is_invocable_v<F, Arc, std::span<int const>, std::span<float const>>;

    template <typename F>
    concept ArcVisitorFunction =
        std::is_invocable_v<F, Arc>;


    void visitAllArcs(
        EdgeListView const& el,
        ArcAttributesVisitorFunction auto visitArc)
    {
        auto const intAttrsCount   = el.getIntAttributeCount();
        auto const floatAttrsCount = el.getFloatAttributeCount();
        auto const intAttrs        = std::make_unique<int[]>(intAttrsCount);
        auto const floatAttrs      = std::make_unique<float[]>(floatAttrsCount);

        auto const arcs     = el.getArcs();
        auto const arcCount = arcs.size();

        for (size_t i = 0; i < arcCount; ++i)
        {
            for (int a = 0; a < intAttrsCount; ++a)
                intAttrs[a] = el.getIntAttributes(a)[i];
            for (int a = 0; a < floatAttrsCount; ++a)
                floatAttrs[a] = el.getFloatAttributes(a)[i];

            visitArc(arcs[i], 
                std::span(intAttrs.get(), intAttrsCount),
                std::span(floatAttrs.get(), floatAttrsCount));
        }
    }


    template <ArcVisitorFunction VisitArc>
    void visitAllArcs(EdgeListView const& el, VisitArc visitArc)
        requires (!ArcAttributesVisitorFunction<VisitArc>)
    {
        std::ranges::for_each(el.getArcs(), visitArc);
    }


    void visitAllArcs(
        DenseAdjacencyMatrixView const& am, 
        ArcVisitorFunction auto visitArc)
    {
        int const vertexCount = am.getVertexCount();
        for (int s = 0; s < vertexCount; ++s)
        {
            auto const sRow = am.getRow(s);
            for (int t = 0; t < vertexCount; ++t)
                if (sRow.getBit(t))
                    visitArc(Arc{ .source = s, .target = t });
        }
    }


    template <ArcAttributesVisitorFunction VisitArc>
    void visitAllArcs(
            DenseAdjacencyMatrixView const& am,
            VisitArc visitArc)
        requires (!ArcVisitorFunction<VisitArc>)
    {
        visitAllArcs(am, 
            [visitArc](Arc arc) { visitArc(arc, {}, {}); });
    }


    void visitAllArcs(
        AdjacencyListView const& al,
        ArcAttributesVisitorFunction auto visitArc)
    {
        int const vertexCount = al.getVertexCount();        
        for (int s = 0; s < vertexCount; ++s)
        {
            auto const targets = al.getTargets(s);
            for (int t: targets)
            {
                auto arc = al.getArc(s, t);
                visitArc(Arc{ .source = s, .target = t },
                    arc->getIntAttributes(), arc->getFloatAttributes());
            }
        }
    }


    template <ArcVisitorFunction VisitArc>
    void visitAllArcs(AdjacencyListView const& al, VisitArc visitArc)
        requires (!ArcAttributesVisitorFunction<VisitArc>)
    {
        int const vertexCount = al.getVertexCount();
        for (int s = 0; s < vertexCount; ++s)
        {
            auto const targets = al.getTargets(s);
            for (int t: targets)
                visitArc(Arc{ .source = s, .target = t });
        }
    }


    template <typename GraphRepresentation>
    void convertGraphRepresentation(
            GraphRepresentation const&  from,
            EditableEdgeList&           el,
            int                         = 0 // unused
        )
    {
        ArcDataSizes const sizeData = obtainArcDataSizes(from);
        el.reserveArcCount(sizeData.arcCount);
        el.resizeIntAttributes(sizeData.intAttrCount);
        el.resizeFloatAttributes(sizeData.floatAttrCount);
        
        visitAllArcs(from, 
            [&](Arc                    arc, 
                std::span<int const>   intAttrs, 
                std::span<float const> floatAttrs)
            {
                int const arcNo = el.connect(arc.source, arc.target);
                int attr_index = 0;
                for (int attr : intAttrs)
                {
                    auto attrs = el.getIntAttributes(attr_index++);
                    REQUIRE(!attrs.empty());
                    attrs[arcNo] = attr;
                }

                attr_index = 0;
                for (float attr : floatAttrs)
                {
                    auto attrs = el.getFloatAttributes(attr_index++);
                    REQUIRE(!attrs.empty());
                    attrs[arcNo] = attr;
                }
            });
    }


    template <typename GraphRepresentation>
    void convertGraphRepresentation(
            GraphRepresentation const&    from,
            EditableDenseAdjacencyMatrix& am,
            int                           vertexCount
        )
    {
        am.reshape(vertexCount);
        visitAllArcs(from, 
                [&](Arc arc) { am.set(arc.source, arc.target); });
    }


    template <typename GraphRepresentation>
    void convertGraphRepresentation(
            GraphRepresentation const&  from,
            EditableAdjacencyList&      al,
            int                         vertexCount
        )
    {
        auto sizes = obtainArcDataSizes(from);
        al.resize(vertexCount, sizes.intAttrCount, sizes.floatAttrCount);
        visitAllArcs(from, 
            [&](Arc arc,
                std::span<int const>   srcIntAttrs,
                std::span<float const> srcFloatAttrs)
            {
                // TODO: connectMakeHandle возвращающую ArcHandle
                al.connect(arc.source, arc.target);
                auto ah = al.getArc(arc.source, arc.target);
                auto destIntAttrs   = ah->getIntAttributes();
                auto destFloatAttrs = ah->getFloatAttributes();

                REQUIRE(destIntAttrs.size()   == srcIntAttrs.size());
                REQUIRE(destFloatAttrs.size() == srcFloatAttrs.size());
                std::ranges::copy(srcIntAttrs,   destIntAttrs.begin());
                std::ranges::copy(srcFloatAttrs, destFloatAttrs.begin());
            });
    }


    class DefaultGraphImplementation final
        : public Graph
    {
    public:
        DefaultGraphImplementation() noexcept = default;

        explicit DefaultGraphImplementation(int vertexCount)
            : _vertexCount(vertexCount)
        {
            // Пусто.
        }

        [[nodiscard]] auto getVertexCount() const noexcept
            -> int override
        {
            return _vertexCount;
        }

        [[nodiscard]] auto getArcCount() const noexcept
            -> int override
        {
            return _arcCount;
        }

        [[nodiscard]] bool hasEdgeListView() const noexcept override
        {
            return _el != nullptr;
        }

        void removeEdgeList() noexcept override
        {
            _el.reset();
        }

        [[nodiscard]] auto getEdgeListView() const
            -> EdgeListView const& override
        {
            if (!_el)
            {
                _el = newEdgeListUnsortedVector();
                if (_al)
                    convertGraphRepresentation(*_al, *_el, getVertexCount());
                else if (_am)
                    convertGraphRepresentation(*_am, *_el, getVertexCount());
            }

            return *_el;
        }


        [[nodiscard]] bool hasAdjacencyMatrixView() const noexcept override
        {
            return _am != nullptr;
        }

        void removeAdjacencyMatrix() noexcept override
        {
            _am.reset();
        }

        [[nodiscard]] auto getAdjacencyMatrixView() const
            -> DenseAdjacencyMatrixView const& override
        {
            if (!_am)
            {
                auto const vertexCount = getVertexCount();
                _am = newDenseAdjacencyMatrix(vertexCount);
                if (_el)
                    convertGraphRepresentation(*_el, *_am, vertexCount);
                else if (_al)
                    convertGraphRepresentation(*_al, *_am, vertexCount);
            }

            return *_am;
        }


        [[nodiscard]] bool hasAdjacencyListView() const noexcept override
        {
            return _al != nullptr;
        }

        void removeAdjacencyList() noexcept override
        {
            _al.reset();
        }

        [[nodiscard]] auto getAdjacencyListView() const
            -> AdjacencyListView const& override
        {
            if (!_al)
            {
                auto const vertexCount = getVertexCount();
                _al = newAdjacencyListVector(vertexCount);
                if (_el)
                    convertGraphRepresentation(*_el, *_al, vertexCount);
                else if (_am)
                    convertGraphRepresentation(*_am, *_al, vertexCount);
            }

            return *_al;
        }


        /// @brief  Добавить заданное число вершин (по умолчанию одну).
        /// @return индекс последней добавленной вершины
        int addVertex(int addedCount) override
        {
            CHECK(addedCount >= 0);
            _vertexCount += addedCount;
            if (_al)
                _al->resize(_vertexCount);
            if (_am)
                _am.reset();

            return _vertexCount - 1;
        }

        /// @brief        Добавить дугу, если её нет.
        /// @param source исходная вершина
        /// @param target целевая вершина
        /// @return       true, если дуга была добавлена, иначе false (дуга уже была)
        bool connect(int source, int target) override
        {
            if (auto max_vertex = std::max(source, target); max_vertex >= _vertexCount)
                addVertex(max_vertex - _vertexCount + 1);

            if (_am)
            {
                auto row = _am->getRow(source);
                if (row.getBit(target))
                    return false;
                
                row.setBit(target);
                if (_el)
                    _el->connect(source, target);
                if (_al)
                    _al->connect(source, target);

                ++_arcCount;
                return true;
            }

            if (_al)
            {
                if (!_al->connect(source, target))
                    return false;
                
                if (_el)
                    _el->connect(source, target);

                ++_arcCount;
                return true;
            }

            if (!_el)
                _el = newEdgeListUnsortedVector();

            _el->connect(source, target);
            ++_arcCount;
            return true;
        }

        /// @brief        Удалить дугу, если она есть.
        /// @param source исходная вершина
        /// @param target целевая вершина
        /// @return       true, если дуга была удалена, иначе false (дуги уже не было)
        bool disconnect(int source, int target) override
        {
            if (!_arcVerticesAreValid(source, target))
                return false;

            if (_am)
            {
                auto row = _am->getRow(source);
                if (!row.getBit(target))
                    return false;

                row.resetBit(target);
                if (_al)
                    _al->disconnect(source, target);
                if (_el)
                    _el->disconnect(source, target);
                
                --_arcCount;
                return true;
            }

            if (_al)
            {
                if (!_al->disconnect(source, target))
                    return false;
                if (_el)
                    _el->disconnect(source, target);
                
                --_arcCount;
                return true;
            }

            if (_el && _el->disconnect(source, target))
            {
                --_arcCount;
                return true;
            }

            return false;
        }

        [[nodiscard]] bool areConnected(int source, int target) const noexcept override
        {
            if (!_arcVerticesAreValid(source, target))
                return false;

            if (_am)
                return _am->getRow(source).getBit(target);
            if (_al)
                return _al->areConnected(source, target);
            if (_el)
                return _el->areConnected(source, target);

            return false;
        }

        friend class ChangeableVertexPositions;

        [[nodiscard]] auto getVertexPositions() const noexcept
            -> std::span<XYZ const> override
        {
            return _xyz;
        }

        [[nodiscard]] auto getVertexPositions() noexcept
            -> ChangeableVertexPositions override
        {
            return { *this, _xyz };
        }

        // Отдельная обработка атрибутов вершин и рёбер


    private:
        std::vector<XYZ> _xyz;
        
        int _vertexCount = 0;
        int _arcCount    = 0;

        mutable std::unique_ptr<EditableEdgeList>             _el;
        mutable std::unique_ptr<EditableDenseAdjacencyMatrix> _am;
        mutable std::unique_ptr<EditableAdjacencyList>        _al;

        [[nodiscard]] bool _vertexIsValid(int v) const noexcept
        {
            return 0 <= v && v < _vertexCount;
        }

        [[nodiscard]] bool _arcVerticesAreValid(int s, int t) const noexcept
        {
            return _vertexIsValid(s) && _vertexIsValid(t);
        }
    };


    auto newGraph(int vertexCount)
        -> std::unique_ptr<Graph>
    {
        return std::make_unique<DefaultGraphImplementation>(vertexCount);
    }

}
