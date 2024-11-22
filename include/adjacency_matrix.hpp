/// @file adjacency_matrix.hpp
#ifndef GRAVIS24_ADJACENCY_MATRIX_HPP
#define GRAVIS24_ADJACENCY_MATRIX_HPP

namespace gravis24
{

    class AdjacencyMatrixView
    {
    public:
        class RowView
        {
        public:
            using Chunk = uint32_t;
            static constexpr auto chunkBits = 8 * sizeof(Chunk);

            RowView() noexcept = default;

            explicit RowView(Chunk const* data, int firstBitOffset = 0) noexcept
                : _data(data)
                , _offset(firstBitOffset)
            {
                // Пусто.
            }

            [[nodiscard]] bool getBit(int index) const noexcept
            {
                auto const bitIndex  = unsigned(index + _offset);
                auto const chunk     = bitIndex / chunkBits;
                auto const bitOffset = bitIndex % chunkBits;
                return ((_data[chunk] >> bit_offset) & 1) == 1;
            }

            [[nodiscard]] bool operator[](int index) const noexcept
            {
                return get(index);
            }

            [[nodiscard]] bool isValid() const noexcept
            {
                return _data != nullptr;
            }

        private:
            Chunk const* _data   {};
            int          _offset {};
        };

        virtual ~AdjacencyMatrixView() = default;

        [[nodiscard]] virtual auto getVertexCount() const noexcept
            -> int = 0;

        /// @brief Получить строку матрицы смежности
        /// @param index < getVertexCount() или возвращает пустой (невалидный) Row
        [[nodiscard]] virtual auto getRow(int index) const noexcept
            -> RowView = 0;

        [[nodiscard]] auto operator[](int index) const noexcept
            -> RowView
        {
            return getRow(index);
        }
    };


    class EditableAdjacencyMatrix
        : public AdjacencyMatrixView
    {
    public:
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
                // TODO: test this code
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
                else
                {
                    auto const chunk = _data[beginChunk];
                    auto const m1    = (Chunk{1} << beginOffset) - 1;
                    _data[beginChunk] &= ~m1;

                    for (unsigned chunk = beginChunk + 1;
                        chunk < endChunk; ++chunk)
                        _data[chunk] = Chunk{};

                    auto const m2    = (Chunk{1} << (endOffset + 1)) - 1;
                    _data[endChunk] &= m2;
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


        [[nodiscard]] virtual auto getRow(int index) noexcept
            -> Row = 0;

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
    };

}

#endif//GRAVIS24_ADJACENCY_MATRIX_HPP
