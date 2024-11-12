/// @file graph.cpp
#include "../include/graph.hpp"
#include <vector>

namespace gravis24
{
    namespace
    {

        class EdgeListViewImpl 
            : public EdgeListView
        {
        public:
            /////////////////////////////////////////////////////
            // Реализация интерфейса EdgeListView

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
            // Операции конструирования

        private:
            std::vector<Arc>                _arcs;
            std::vector<std::vector<int>>   _intAttrs;
            std::vector<std::vector<float>> _floatAttrs;
        };


    }
}
