/// @file graph.cpp
#include "../include/graph.hpp"

#include <vector>
#include <algorithm>
#include <ranges>

namespace gravis24
{ 

    /////////////////////////////////////////////////////
    // Класс AdjacencyMatrixViewImpl

    class AdjacencyMatrixViewImpl
        : public AdjacencyMatrixView
    {
    public:
        /////////////////////////////////////////////////////
        // Реализация интерфейса AdjacencyMatrixView

        [[nodiscard]] auto getVertexCount() const noexcept
            -> int override
        {
            return _vertexCount;
        }

        /// @brief Получить строку матрицы смежности
        /// @param index < getVertexCount() или возвращает пустой (невалидный) Row
        [[nodiscard]] auto getRow(int index) const noexcept
            -> RowView override
        {
            auto const bitIndex   = size_t(index) * _vertexCount;
            auto const chunkIndex = bitIndex / chunkBits;
            auto const bitOffset  = bitIndex % chunkBits;
            return { _bits.data() + chunkIndex, bitOffset };
        }


        /////////////////////////////////////////////////////
        // Операции конструирования

        AdjacencyMatrixViewImpl() noexcept = default;

        explicit AdjacencyMatrixViewImpl(int vertexCount)
            : _bits((size_t(vertexCount) * vertexCount + chunkBits - 1) / chunkBits)
            , _vertexCount(vertexCount)
        {
            // Пусто.
        }

        class Row
        {
        public:
            using Chunk = uint32_t;
            static constexpr auto chunkBits = 8 * sizeof(Chunk);

            Row() noexcept = default;

            explicit Row(Chunk* data, int firstBitOffset = 0) noexcept
                : _data(data)
                , _offset(firstBitOffset)
            {
                // Пусто.
            }

            [[nodiscard]] operator RowView() const noexcept
            {
                return { _data, _offset };
            }

            [[nodiscard]] bool isValid() const noexcept
            {
                return _data != nullptr;
            }

            [[nodiscard]] bool getBit(int index) const noexcept
            {
                auto const bitIndex  = unsigned(index + _offset);
                auto const chunk     = bitIndex / chunkBits;
                auto const bitOffset = bitIndex % chunkBits;
                return ((_data[chunk] >> bit_offset) & 1) == 1;
            }

            void resetBit(int index) noexcept
            {
                auto const bitIndex  = unsigned(index + _offset);
                auto const chunk     = bitIndex / chunkBits;
                auto const bitOffset = bitIndex % chunkBits;
                _data[chunk] &= ~(Chunk(1) << bitOffset);
            }

            void reset(int firstBit, int untilBit) noexcept
            {
                auto const beginBit    = unsigned(firstBit + _offset);
                auto const endBit      = unsigned(untilBit + _offset);
                auto const beginChunk  = beginBit / chunkBits;
                auto const beginOffset = beginBit % chunkBits;
                auto const endChunk    = endBit / chunkBits;
                auto const endOffset   = endBit % chunkBits;
                if (beginChunk == endChunk)
                {
                    auto const chunk = _data[beginChunk];
                    auto const m1    = (Chunk{1} << beginOffset) - 1;
                    auto const m2    = (Chunk{1} << (endOffset + 1)) - 1;
                    auto const m3    = ~m1 | m2;
                    _data[beginChunk] &= m3;
                }
            }

            void flipBit(int index) noexcept
            {
                auto const bitIndex  = unsigned(index + _offset);
                auto const chunk     = bitIndex / chunkBits;
                auto const bitOffset = bitIndex % chunkBits;
                _data[chunk] ^= Chunk(1) << bitOffset;
            }

            void setBit(int index) noexcept
            {
                auto const bitIndex  = unsigned(index + _offset);
                auto const chunk     = bitIndex / chunkBits;
                auto const bitOffset = bitIndex % chunkBits;
                _data[chunk] |= Chunk(1) << bitOffset;
            }

            void setBit(int index, bool value) noexcept
            {
                // Оптимизированная версия.                         // x86
                auto const bitIndex    = unsigned(index + _offset); // add
                auto const chunk       = bitIndex / chunkBits;      // shr
                auto const bitOffset   = bitIndex % chunkBits;      // and
                auto const bitMask     = Chunk(1) << bitOffset;     // shl
                auto &     chunkRef    = _data[chunk];              // lea... mem load
                auto const chunkValue  = chunkRef;                  // mov[]
                auto const bitWasSet   = chunkValue | bitMask;      // or
                auto const bitWasReset = chunkValue &~bitMask;      // andn
                chunkRef = value? bitWasSet: bitWasReset;           // cmov... mem store
            }

        private:
            Chunk* _data   {};
            int    _offset {};
        };


        [[nodiscard]] auto getRow(int index) noexcept
            -> Row
        {
            auto const bitIndex   = size_t(index) * _vertexCount;
            auto const chunkIndex = bitIndex / chunkBits;
            auto const bitOffset  = bitIndex % chunkBits;
            return { _bits.data() + chunkIndex, bitOffset };
        }

        using AdjacencyListView::operator[];
        [[nodiscard]] auto operator[](int index) noexcept
            -> Row
        {
            return getRow(index);
        }

        // UB если неверные i, j!
        [[nodiscard]] bool get(int i, int j) const noexcept
        {
            return getRow(i).getBit(j);
        }

        void set(int i, int j) noexcept
        {
            getRow(i).setBit(j);
        }

        void set(int i, int j, bool value) noexcept
        {
            getRow(i).setBit(j, value);
        }

        void reset(int i, int j) noexcept
        {
            getRow(i).resetBit(j);
        }

        void flip(int i, int j) noexcept
        {
            getRow(i).flipBit(j);
        }

    private:
        std::vector<Chunk> _bits;
        int                _vertexCount {};
    };


    /////////////////////////////////////////////////////
    // Класс AdjacencyListViewImpl

    class AdjacencyListViewImpl
        : public AdjacencyListView
    {
    public:
        /////////////////////////////////////////////////////
        // Реализация интерфейса AdjacencyListView

        [[nodiscard]] auto getVertexCount() const noexcept
            -> int override
        {
            return static_cast<int>(_adj.size());
        }

        [[nodiscard]] bool areConnected(int source, int target) const noexcept override
        {
            if (_adj.size() <= static_cast<size_t>(source))
                return false;

            for (auto v: _adj[source])
                if (v == target)
                    return true;

            return false;
        }

        [[nodiscard]] auto neighborsCount(int vertex) const noexcept
            -> int override
        {
            if (_adj.size() <= static_cast<size_t>(vertex))
                return 0;

            return static_cast<int>(_adj[vertex].size());
        }

        /// @brief Если вершины нет (неверный индекс), возвращает пустой span.
        [[nodiscard]] auto getNeighbors(int vertex) const noexcept
            -> std::span<int const> override
        {
            if (_adj.size() <= static_cast<size_t>(vertex))
                return {};

            return _adj[vertex];
        }

        /////////////////////////////////////////////////////
        // Операции конструирования

        AdjacencyListViewImpl() noexcept = default;

        explicit AdjacencyListViewImpl(int vertexCount)
            : _adj(vertexCount)
        {
            // Пусто.
        }



    private:
        using Adjacency = std::vector<int>;
        std::vector<Adjacency> _adj;
    };
}
