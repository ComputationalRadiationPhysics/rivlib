/*
 * the/math/pcautils.h
 *
 * Copyright (c) 2012, TheLib Team (http://www.thelib.org/license)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * - Neither the name of TheLib, TheLib Team, nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THELIB TEAM AS IS AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THELIB TEAM BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*
 * pcautils.h
 *
 * Copyright (C) 2006 - 2010 by Visualisierungsinstitut Universitaet Stuttgart. 
 * Alle Rechte vorbehalten.
 */

#ifndef THE_MATH_PCAUTILS_H_INCLUDED
#define THE_MATH_PCAUTILS_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#include <utility>

#include "the/assert.h"

#include "the/collections/array.h"

#include "the/math/abstract_vector.h"
#include "the/math/abstract_matrix.h"
#include "the/math/functions.h"
#include "the/math/vector.h"


namespace the {
namespace math {

    /**
     * Calculate the covariance matrix from a list of relative coordinates
     *
     * T is the type for the matrix type (and the vector type)
     * D is the dimensionality for the matrix type (and the vector type)
     * L is the matrix type layout
     * S is the matrix storage type
     * L2 is the array lock class
     * C is the array constructor class
     * T2 is the array type class (the vector type)
     *
     * @param outMatrix The variable to receive the calculated covariance
     *                  matrix
     * @param relCoords The relative coordinates
     */
    template<class T, unsigned int D, matrix_layout L, class S, class L2,
        class C, class T2>
    inline void calc_covariance_matrix(abstract_matrix<T, D, L, S>& outMatrix,
            const the::collections::array<T2, L2, C>& relCoords) {
        calc_covariance_matrix(outMatrix, relCoords.data(),
            relCoords.size());
    }

    /**
     * Calculate the covariance matrix from a list of relative coordinates
     *
     * T is the type for the matrix type and the vector type
     * D is the dimensionality for the matrix type and the vector type
     * L is the matrix type layout
     * S1 is the matrix storage type
     * S2 is the vector storage type
     *
     * @param outMatrix The variable to receive the calculated covariance
     *                  matrix
     * @param relCoords Pointer to the relative coordinates. Must not be NULL
     * @param relCoordsCnt The number of relative coordinates 'relCoords'
     *                     points to
     */
    template<class T, unsigned int D, matrix_layout L, class S1, class S2>
    void calc_covariance_matrix(abstract_matrix<T, D, L, S1>& outMatrix,
            const abstract_vector<T, D, S2> *relCoords, size_t relCoordsCnt) {
        ASSERT(relCoords != NULL);

        for (unsigned int x = 0; x < D; x++) {
            for (unsigned int y = 0; y < D; y++) {
                outMatrix(x, y) = static_cast<T>(0);
            }
        }

        if (relCoordsCnt > 0) {
            for (size_t i = 0; i < relCoordsCnt; i++) {
                for (unsigned int x = 0; x < D; x++) {
                    for (unsigned int y = 0; y < D; y++) {
                        outMatrix(x, y) += (relCoords[i][x] * relCoords[i][y]);
                    }
                }
            }

            outMatrix /= static_cast<T>(relCoordsCnt);

        }

    }

    /**
     * sorts a list of eigenvectors according to their eigenvalues ascending.
     *
     * @param evec The array of eigenvectors
     * @param eval The array of eigenvalues
     * @param cnt The number of eigenvectors/eigenvalues
     */
    template<class T, unsigned int D, class S>
    void sort_eigenvectors(abstract_vector<T, D, S> *evec, T *eval, size_t cnt) {
        if (cnt < 2) return; // lol

        if (cnt < 5) {
            if (eval[0] > eval[1]) {
                the::swap(eval[0], eval[1]);
                the::swap(evec[0], evec[1]);
            }
            if (cnt > 2) {
                if (eval[0] > eval[2]) {
                    the::swap(eval[0], eval[2]);
                    the::swap(evec[0], evec[2]);
                }
                if (eval[1] > eval[2]) {
                    the::swap(eval[1], eval[2]);
                    the::swap(evec[1], evec[2]);
                }
                if (cnt == 4) {
                    if (eval[0] > eval[3]) {
                        the::swap(eval[0], eval[3]);
                        the::swap(evec[0], evec[3]);
                    }
                    if (eval[1] > eval[3]) {
                        the::swap(eval[1], eval[3]);
                        the::swap(evec[1], evec[3]);
                    }
                    if (eval[2] > eval[3]) {
                        the::swap(eval[2], eval[3]);
                        the::swap(evec[2], evec[3]);
                    }
                }
            }
            return; // simple (and common)
        }

        // more than four dimensions. I so don't care
        the::collections::array<std::pair<T, vector<T, D> > > tb;
        tb.set_size(cnt);
        for (size_t i = 0; i < cnt; i++) {
            tb[i].first = eval[i];
            tb[i].second = evec[i];
        }

        tb.sort(&compare_pairs_first<T, vector<T, D> >);

        for (size_t i = 0; i < cnt; i++) {
            eval[i] = tb[i].first;
            evec[i] = tb[i].second;
        }

    }


} /* end namespace math */
} /* end namespace the */


#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_MATH_PCAUTILS_H_INCLUDED */
