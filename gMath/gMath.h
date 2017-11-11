#ifndef GMATH_H
#define GMATH_H


#include <math.h>
#include <cstdlib>
#include <time.h>
#include <gfw_global.h>
#ifdef __SSE2__
#include <emmintrin.h>
#endif
#ifndef G_PI
#define G_PI 3.14159265359
#define G_RAD_CONST 57.295779513082320876798154814105
#endif
namespace gfw {
///////////////////////////////////////////////////////////////////////////////
/// \struct gMath
/// \brief The gMath struct provides math functions.
///
    struct SHARED_GFW gMath{
        //! Obtains the sine of a value.
        ginline static gScalar sine(gScalar value){
            return (gScalar)sin((double)value);
        }
        //! Obtains the cosine of value
        ginline static gScalar cosine(gScalar value){
            return (gScalar)cos((double)value);
        }
        //! Obtains the tangent of a value
        ginline static gScalar tangent(gScalar value){
            return (gScalar)tan((double)value);
        }
        //! Obtains the arq tangent of a value
        ginline static gScalar atangent(gScalar value){
            return (gScalar)atan((double)value);
        }
        //! Obtains the arc tangent of x/y
        ginline static gScalar atangent2(gScalar y, gScalar x){
            return (gScalar)atan2(y,x);
        }
        ginline static gScalar asine(gScalar value){
            return (gScalar)asin((double)value);
        }
        //! Obtains the square root of a value
        ginline static gScalar squareRoot(gScalar value){
#ifdef __SSE__
#ifndef DOUBLE_PRECISION
            float _res[4];
            __m128 val = _mm_set1_ps((float)value);
            __m128 res = _mm_sqrt_ss(val);
            _mm_store_ss(_res,res);
            return _res[0];
#else
            double _res[2];
            __m128d val = _mm_set1_pd(value);
            __m128d res = _mm_sqrt_sd(0,val);
            _mm_store_sd(_res,res);
            return _res[0];
#endif
#else
            //no sse
            return sqrt(value);
#endif


        }
        //! Obtains the reciprocal square root
        ginline static gScalar reciprocalSquareRoot(gScalar value){
            gScalar val = squareRoot(value);
#ifdef __SSE__
            gf32 ret[4];
            __m128 res;
#ifdef DOUBLE_PRECISION
            res = _mm_set1_ps((float)val);
#else
            res = _mm_set1_ps(val);
            res = _mm_rcp_ss(res);
            _mm_storeu_ps(ret,res);
            return (gScalar)ret[0];
#endif
#else
            return (gScalar)1.0f / val;
#endif
        }
        ///////////////////////////////////////////////////////////////////////
        /// \brief Finds the Square root using the method of the magic number.
        /// Algorithm : The Magic Number.
        /// \param value : Value to get the reciprocal square root from.
        /// \return Square root
        ///
        ginline static gf32 fastSquareRoot32(gf32 value){
            const gf32 half = value * 0.5f;
            union{
                gs32 i;
                gf32 f;
            } u;
            u.f = value;
            u.i = 0x5f3759df - (u.i >> 1);
            return (value *  u.f)  * (1.5f - half * u.f * u.f);
        }
        //////////////////////////////////////////////////////////////////////
        /// \brief Finds the reciprocal square root using the method of the magic number.
        /// Algorithm: The magic number.
        /// \param value : Value to get the reciprocal square root from.
        /// \return Reciprocal square root.
        ///
        inline static gf32 fastReciprocalSquareRoot32(gf32 value)
        {
            const gf32 half = value * 0.5f;
            union{
                gs32 i;
                gf32 f;
            } u;
            u.f = value;
            u.i = 0x5f3759df - (u.i >> 1);
            return (u.f)  * (1.5f - half * u.f * u.f);
        }
        ///////////////////////////////////////////////////////////////////////////
        /// \brief Obtains the fast reciprocal square root. If possible it will use
        /// SSE instructions to speed up this operation even more.
        /// \param value : Value to get square root from.
        /// \return Reciprocal Square Root value.
        ///
        ginline static gScalar fastReciprocalSquareRoot(gScalar value){
            //sse is slighty faster and more accurate than using the magic number method
#ifdef __SSE__
            //on gcc using a asm inline is much much faster
            //asm("rsqrtss %1,%0" : "=x"(res) : "x"(value));
            float result[4];
            __m128 val = _mm_set1_ps(value);
            __m128 res = _mm_rsqrt_ss(val);
            _mm_store_ss(result,res);
            return (gScalar)result[0];
#else
            return fastReciprocalSquareRoot32(gf32(value));
#endif
        }
        ////////////////////////////////////////////////////////////////////////////////
        /// \brief Obtain the Square root using fast aproximation with SSE instructions if possible.
        /// this is faster than using the regular sqrt or sqrtss from SSE but less accurate. Acceptable
        /// on most cases.
        /// \param value : Value to get Square Root from.
        /// \return : Square root value.
        ///
        inline static gScalar fastSquareRoot(gScalar value)
        {
#ifdef __SSE__
            //on gcc using a asm inline is much much faster
            //asm("rsqrtss %1,%0" : "=x"(res) : "x"(value));
            float result[4];
            __m128 val = _mm_set1_ps(value);
            __m128 res = _mm_rsqrt_ss(val);
            __m128 final = _mm_rcp_ss(res);
            _mm_store_ss(result,final);
            return (gScalar)result[0];
#else
            return fastSquareRoot32(gf32(value));
#endif
        }
        //! Obtains the absolute value of a value
        ginline static gScalar absValue(gScalar value){
            if(value < 0)return -value;
            return value;
        }
        //! clamps a value
        template <typename T>
        ginline static const T clamp(const T &value, const T &low, const  T &high){
            return MIN(MAX(value,low),high);
        }
        //! Starts the random number generation seed
        ginline static void randomSeed(gu32 val=0){
            if(val){
                srand(val);
            }else{
                srand(time(0));
            }
        }
        //! Gets a random number
        ginline static gs32 randomInteger(gs32 _min,gs32 _max){
            gs32 val=rand();
            RAND_RANGE(val,_min,_max,RAND_MAX);
            return val;
        }
        ///////////////////////////////////////////////////////////////////////////////////
        /// \brief Generates a random float value between 2 values.
        /// \param _min : Minimum value to generate.
        /// \param _max : Maximum value to generate.
        /// \return Random value.
        ///
        ginline static gScalar randomFloat(gScalar _min,gScalar _max){
            gs32 val= rand();
            gScalar tmin;
            //Normalize values
            tmin = _min / _max;
            //transform values to integer to work with rand
            gs32 maxVal = RAND_MAX;
            gs32 minVal = (gs32)(RAND_MAX * tmin);
            RAND_RANGE(val,minVal,maxVal,RAND_MAX);

            gScalar ret = gScalar(gScalar(val) / gScalar(RAND_MAX)) * _max;

            return ret;
        }
        //////////////////////////////////////////////////////////////////////////////////
        /// \brief Linear Interpolation: Interpolate two values.
        /// \param v1 : Value 1
        /// \param v2 : Value 2
        /// \param fact: Factor.ginline virtual void sort()
        ginline static gScalar lerp(const gScalar v1,const gScalar v2, const gScalar fact){
            return ((1 - fact) * v1) + (fact * v2);
        }
        inline static gScalar bin2polar(gScalar value){
            return (2.0f * value) - 1.0f;
        }
        inline static gScalar polar2bin(gScalar value){
            //polar = 2v -1
            //v = (polar + 1) / 2
            return (value + 1.0f) * 0.5f;
        }
        inline static gScalar reciprocal(gScalar value){
            //this might be ending the same with sse funcs or not
            // the compiler may optimize it for it anyway
#ifdef __SSE__
            float result[4];
            __m128 val = _mm_set1_ps(value);
            __m128 ret = _mm_rcp_ss(val);
            _mm_store_ss(result,ret);
            return result[0];
#else
            return 1.0f / value;
#endif
        }

        inline static gScalar normalize(gScalar value, gScalar max){
            return reciprocal(max) * value;
        }
    };
}
#endif // GMATH_H
