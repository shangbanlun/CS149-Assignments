#include <immintrin.h>
#include <math.h>


static inline int count_true_elements(__m256 mask) {
    int bitmask = _mm256_movemask_ps(mask);
    return __builtin_popcount(bitmask);
}


static inline __m256 getError(const __m256 &vecGuess, const __m256 &vecX, const __m256 &vec1f, const __m256 &maskSign)
{
    __m256 vecGuessSquare = _mm256_mul_ps(vecGuess, vecGuess);
    __m256 vecError = _mm256_sub_ps(_mm256_mul_ps(vecGuessSquare, vecX), vec1f);
    vecError = _mm256_andnot_ps(maskSign, vecError);
    return vecError;
}


void sqrtAVX2(int N, float initialGuess, float* values, float* output)
{
    __m256 veckThreshold = _mm256_set1_ps(0.00001f);
    __m256 maskSign = _mm256_set1_ps(-0.0f);
    __m256 vec1f = _mm256_set1_ps(1.f);
    __m256 vec3f = _mm256_set1_ps(3.f);
    __m256 vecP5f = _mm256_set1_ps(0.5f);

    for (int i = 0; i < N; i += 8)
    {
        __m256 vecGuess = _mm256_set1_ps(initialGuess);
        __m256 vecX = _mm256_loadu_ps(values + i);
        __m256 vecError = getError(vecGuess, vecX, vec1f, maskSign);
        
        __m256 mask = _mm256_cmp_ps(vecError, veckThreshold, _CMP_GT_OQ);    // * True if need another iteration.
        int numErrGtKT = count_true_elements(mask);

        while (numErrGtKT > 0) 
        {
            __m256 vecGuessCube = _mm256_mul_ps(_mm256_mul_ps(vecGuess, vecGuess), vecGuess);
            __m256 vecvecGuessNew = _mm256_sub_ps(_mm256_mul_ps(vecGuess, vec3f), _mm256_mul_ps(vecX, vecGuessCube));
            vecvecGuessNew = _mm256_mul_ps(vecP5f, vecvecGuessNew);
            vecGuess = _mm256_blendv_ps(vecGuess, vecvecGuessNew, mask);
            vecError = getError(vecGuess, vecX, vec1f, maskSign);

            mask = _mm256_cmp_ps(vecError, veckThreshold, _CMP_GT_OQ);
            numErrGtKT = count_true_elements(mask);
        }

        _mm256_storeu_ps(output + i, _mm256_mul_ps(vecX, vecGuess));
        
    }
}