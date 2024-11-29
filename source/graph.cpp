/// @file graph.cpp
#include "../include/graph.hpp"

#include <vector>

namespace gravis24
{ 

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

        }

        [[nodiscard]] auto getAdjacencyMatrixView() const
            -> DenseAdjacencyMatrixView const& override
        {

        }

        [[nodiscard]] bool hasAdjacencyListView() const noexcept override
        {

        }

        [[nodiscard]] auto getAdjacencyListView() const
            -> AdjacencyListView const& override
        {

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

        bool areConnected(int source, int target) const noexcept override
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
        
        mutable std::unique_ptr<EditableEdgeList>             _el;
        mutable std::unique_ptr<EditableDenseAdjacencyMatrix> _am;
        mutable std::unique_ptr<EditableAdjacencyList>        _al;
    };


}
