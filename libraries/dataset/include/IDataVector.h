////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Project:  Embedded Machine Learning Library (EMLL)
//  File:     IDataVector.h (dataset)
//  Authors:  Ofer Dekel
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

// linear
#include "IVector.h"

// stl
#include <cstdint>
#include <functional>
#include <memory>
#include <vector>

namespace emll
{
namespace dataset
{
    /// <summary> Base class for infinite-dimensional vectors of double numbers. Each implementation of
    /// this class has a mathematical dimension of infinity and includes an explicitly specified
    /// prefix followed by an implicit suffix of zeros. </summary>
    class IDataVector : public linear::IVector
    {
    public:
        /// <summary> Adds a value at the end of the std::vector. </summary>
        ///
        /// <param name="index"> Zero-based index of the. </param>
        /// <param name="value"> The value. </param>
        virtual void AppendEntry(uint64_t index, double value = 1.0) = 0;

        /// <summary> Deletes all of the std::vector content and sets its Size to zero, but does not
        /// deallocate its memory. </summary>
        virtual void Reset() = 0;

        /// <summary> Returns the number of non-zeros. </summary>
        ///
        /// <returns> The total number of nonzeros. </returns>
        virtual uint64_t NumNonzeros() const = 0;

        /// <summary> Makes a deep copy of the datavector </summary>
        ///
        /// <returns> A deep copy of the datavector </summary>
        virtual std::unique_ptr<IDataVector> Clone() const = 0;

        /// <summary> Copies the contents of this DataVector into a double array of given size. </summary>
        ///
        /// <returns> The array. </returns>
        virtual std::vector<double> ToArray() const = 0;
    };
}
}
