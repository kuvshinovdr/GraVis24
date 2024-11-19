/// @file  edge_list_unsorted_vector.cpp
/// @brief ���������� EditableEdgeList �� ������ ���������������� �������.

#include "../include/edge_list.hpp"

#include <vector>
#include <algorithm>

namespace gravis24
{

    class EdgeListUnsortedVector 
        : public EditableEdgeList
    {
    public:
        /////////////////////////////////////////////////////
        // ���������� ���������� EdgeListView

        [[nodiscard]] auto getEdges() const noexcept
            -> std::span<Arc const> override
        {
            return _arcs;
        }

        [[nodiscard]] auto getIntAttributeCount() const noexcept
            -> int override
        {
            return static_cast<_intAttrs.size()>;
        }

        [[nodiscard]] auto getIntAttributes(int attributeIndex) const noexcept
            -> std::span<int const> override
        {
            auto const i = static_cast<size_t>(attributeIndex);
            if (i < _intAttrs.size())
                return _intAttrs[i];
            return {};
        }

        [[nodiscard]] auto getFloatAttributeCount() const noexcept
            -> int override
        {
            return static_cast<_floatAttrs.size()>;
        }

        [[nodiscard]] auto getFloatAttributes(int attributeIndex) const noexcept
            -> std::span<float const> override
        {
            auto const i = static_cast<size_t>(attributeIndex);
            if (i < _floatAttrs.size())
                return _floatAttrs[i];
            return {};
        }

        /////////////////////////////////////////////////////
        // ���������� ���������� EditableEdgeList

        // �� ��������� ��������� ������������ ���.
        int connect(int source, int target) override
        {
            auto const result = static_cast<int>(_arcs.size());

            _arcs.emplace_back(source, target);
            attributesResize();

            return result;
        }

        [[nodiscard]] auto getIntAttributes(int attributeIndex) noexcept
            -> std::span<int> override
        {
            auto const i = static_cast<size_t>(attributeIndex);
            if (i < _intAttrs.size())
                return _intAttrs[i];
            return {};
        }

        [[nodiscard]] auto getFloatAttributes(int attributeIndex) noexcept
            -> std::span<float> override
        {
            auto const i = static_cast<size_t>(attributeIndex);
            if (i < _floatAttrs.size())
                return _floatAttrs[i];
            return {};
        }

        [[nodiscard]] bool areConnected(int source, int target) const noexcept override
        {
            return std::ranges::contains(_arcs, Arc{source, target});
        }

        bool disconnect(int source, int target) override
        {
            return std::erase(_arcs, Arc{source, target}) != 0;
        }

        //////////////////////////////////////////////
        // ������������

        EdgeListUnsortedVector() noexcept = default;

        explicit EdgeListUnsortedVector(
            int arcsCount, int intAttrsCount = 0, int floatAttrsCount = 0)
            : _intAttrs(intAttrsCount)
            , _floatAttrs(floatAttrsCount)
        {
            _arcs.reserve(arcsCount);
            for (auto& attrs: _intAttrs)
                attrs.reserve(arcsCount);
            for (auto& attrs: _floatAttrs)
                attrs.reserve(arcsCount);
        }

    private:
        std::vector<Arc>                _arcs;
        std::vector<std::vector<int>>   _intAttrs;
        std::vector<std::vector<float>> _floatAttrs;

        void attributesResize()
        {
            auto const requiredSize = _arcs.size();
            for (auto& attrs: _intAttrs)
                attributesResize(attrs, requiredSize);
            for (auto& attrs: _floatAttrs)
                attributesResize(attrs, requiredSize);
        }

        static void attributesResize(auto& attrs, size_t requiredSize)
        {
            if (attrs.capacity() < requiredSize)
                attrs.reserve(requiredSize + (requiredSize >> 1));
            attrs.resize(requiredSize);
        }
    };


    auto newEdgeListUnsortedVector(
            int preallocArcsCount,
            int intAttrsCount, 
            int floatAttrsCount
        ) -> std::unique_ptr<EditableEdgeList>
    {
        if (preallocArcsCount == 0
         && intAttrsCount     == 0
         && floatAttrsCount   == 0)
            return std::make_unique<EdgeListUnsortedVector>();

        return std::make_unique<EdgeListUnsortedVector>(
            preallocArcsCount, intAttrsCount, floatAttrsCount);
    }

}
