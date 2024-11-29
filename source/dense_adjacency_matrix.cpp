/// @file  dense_adjacency_matrix.cpp
/// @brief Реализация EditableDenseAdjacencyMatrix на основе std::vector.
#include "../include/dense_adjacency_matrix.hpp"
#include <vector>

namespace gravis24
{

    class DenseAdjacencyMatrix
        : public EditableDenseAdjacencyMatrix
    {
    public:
        /////////////////////////////////////////////////////
        // Операции конструирования

        DenseAdjacencyMatrix() noexcept = default;

        explicit DenseAdjacencyMatrix(int vertexCount)
            : _bits((size_t(vertexCount) * vertexCount + chunkBits - 1) / chunkBits)
            , _vertexCount(vertexCount)
        {
            // Пусто.
        }

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
            auto const bitOffset  = static_cast<int>(bitIndex % chunkBits);
            return RowView { _bits.data() + chunkIndex, bitOffset };
        }

        /////////////////////////////////////////////////////
        // Реализация интерфейса EditableAdjacencyMatrix

        [[nodiscard]] auto getRow(int index) noexcept
            -> Row override
        {
            auto const bitIndex   = size_t(index) * _vertexCount;
            auto const chunkIndex = bitIndex / chunkBits;
            auto const bitOffset  = static_cast<int>(bitIndex % chunkBits);
            return Row { _bits.data() + chunkIndex, bitOffset };
        }

    private:
        std::vector<Chunk> _bits;
        int                _vertexCount {};
    };


    auto newDenseAdjacencyMatrix(int vertexCount)
        -> std::unique_ptr<EditableDenseAdjacencyMatrix>
    {
        return std::make_unique<DenseAdjacencyMatrix>(vertexCount);
    }

}
