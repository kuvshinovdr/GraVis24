/// @file graph.cpp
#include "../include/graph.hpp"

#include <type_traits>
#include <algorithm>
#include <vector>


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


    template <typename VisitArc>
    void visitAllArcs(
        EdgeListView const& el, VisitArc visitArc)
    {
        if constexpr (
            std::is_invocable_v<
                VisitArc, Arc, std::span<int const>, std::span<float const>>)
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
        else
        {
            static_assert(std::is_invocable_v<VisitArc, Arc>);
            std::ranges::for_each(el.getArcs(), visitArc);
        }
    }

    template <typename VisitArc>
    void visitAllArcs(
        DenseAdjacencyMatrixView const& am, VisitArc visitArc)
    {
        static_assert(std::is_invocable_v<VisitArc, Arc>);
        int const vertexCount = am.getVertexCount();
        for (int s = 0; s < vertexCount; ++s)
        {
            auto const sRow = am.getRow(s);
            for (int t = 0; t < vertexCount; ++t)
                if (sRow.getBit(t))
                    visitArc(Arc{ .source = s, .target = t });
        }
    }

    template <typename VisitArc>
    void visitAllArcs(
        AdjacencyListView const& al, VisitArc visitArc)
    {
        int const vertexCount = al.getVertexCount();
        if constexpr (
            std::is_invocable_v<
                VisitArc, Arc, std::span<int const>, std::span<float const>>)
        {
            // TODO: добавить интерфейс получени€ атрибутов дуги как span-ов.
            auto const intAttrsCount   = al.getArcIntAttributeCount();
            auto const floatAttrsCount = al.getArcFloatAttributeCount();
            auto const intAttrs        = std::make_unique<int[]>(intAttrsCount);
            auto const floatAttrs      = std::make_unique<float[]>(floatAttrsCount);
            for (int s = 0; s < vertexCount; ++s)
            {
                auto const neighbors = al.getTargets(s);
                for (int t: neighbors)
                {
                    for (int a = 0; a < intAttrsCount; ++a)
                        intAttrs[a] = al.getArcIntAttribute(s, t, a);
                    for (int a = 0; a < floatAttrsCount; ++a)
                        floatAttrs[a] = al.getArcFloatAttribute(s, t, a);

                    visitArc(Arc{ .source = s, .target = t },
                        std::span(intAttrs.get(), intAttrsCount),
                        std::span(floatAttrs.get(), floatAttrsCount));
                }
            }
        }
        else
        {
            static_assert(std::is_invocable_v<VisitArc, Arc>);
            for (int s = 0; s < vertexCount; ++s)
            {
                auto const neighbors = al.getTargets(s);
                for (int t: neighbors)
                    visitArc(Arc{ .source = s, .target = t });
            }
        }
    }


    class DefaultGraphImplementation final
        : public Graph
    {
    public:
        [[nodiscard]] auto getVertexCount() const noexcept
            -> int override
        {

        }

        [[nodiscard]] auto getEdgeCount() const noexcept
            -> int override
        {

        }

        [[nodiscard]] bool hasEdgeListView() const noexcept override
        {
            return _el != nullptr;
        }

        [[nodiscard]] auto getEdgeListView() const
            -> EdgeListView const& override
        {
            if (!_el)
                ;// makeEdgeList
            return *_el;
        }

        [[nodiscard]] bool hasAdjacencyMatrixView() const noexcept override
        {
            return _am != nullptr;
        }

        [[nodiscard]] auto getAdjacencyMatrixView() const
            -> DenseAdjacencyMatrixView const& override
        {
            if (!_am)
                ;
            return *_am;
        }

        [[nodiscard]] bool hasAdjacencyListView() const noexcept override
        {
            return _al != nullptr;
        }

        [[nodiscard]] auto getAdjacencyListView() const
            -> AdjacencyListView const& override
        {
            if (!_al)
                ;
            return *_al;
        }

        /// @brief        ƒобавить дугу, если еЄ нет.
        /// @param source исходна€ вершина
        /// @param target целева€ вершина
        /// @return       true, если дуга была добавлена, иначе false (дуга уже была)
        bool connect(int source, int target) override
        {

        }

        /// @brief        ”далить дугу, если она есть.
        /// @param source исходна€ вершина
        /// @param target целева€ вершина
        /// @return       true, если дуга была удалена, иначе false (дуги уже не было)
        bool disconnect(int source, int target) override
        {

        }

        [[nodiscard]] bool areConnected(int source, int target) const noexcept override
        {

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

        // ќтдельна€ обработка атрибутов вершин и рЄбер


    private:
        std::vector<XYZ> _xyz;
        
        int _vertexCount = 0;
        int _arcCount    = 0;

        mutable std::unique_ptr<EditableEdgeList>             _el;
        mutable std::unique_ptr<EditableDenseAdjacencyMatrix> _am;
        mutable std::unique_ptr<EditableAdjacencyList>        _al;
    };


}
