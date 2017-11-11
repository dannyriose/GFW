#ifndef GDSP_H
#define GDSP_H

#include <gMemory/gBaseArray.h>
#include <gMath/gMath.h>
#include <gMath/gComplexBuffer.h>

//Defines a complex number
namespace gfw
{
    //! Utility class that provides digital signal processing functionality
    class SHARED_GFW gDsp
    {
    public:
        //! Time domain signals
        enum TD_SIGNALS
        {
            //! Signal is sine
            SIGNAL_SINE,
            //! Signal is cosine
            SIGNAL_COSINE
        };
        //! Container of the range of frequencies
        struct gFreqDescriptor
        {
            gScalar lowPass;
            gScalar bandPassMin;
            gScalar bandPassMax;
            gScalar highPass;

            gFreqDescriptor():lowPass(0),bandPassMin(0),bandPassMax(0),highPass(0){}
            gFreqDescriptor(const gFreqDescriptor &other)
            {
                lowPass=other.lowPass;
                highPass=other.highPass;
                bandPassMin=other.bandPassMin;
                bandPassMax=other.bandPassMax;
            }
            gFreqDescriptor(gScalar _lowPass,gScalar _bandPassMin,gScalar _bandPassMax,gScalar _highPass)
            {
                lowPass=_lowPass;
                highPass=_highPass;
                bandPassMin=_bandPassMin;
                bandPassMax=_bandPassMax;
            }
            gFreqDescriptor &operator=(const gFreqDescriptor &other)
            {
                lowPass=other.lowPass;
                highPass=other.highPass;
                bandPassMin=other.bandPassMin;
                bandPassMax=other.bandPassMax;
                return *this;
            }
        };
        //! Constructor
        gDsp();
        //! Convolution of two complex vectors
        static gComplexBuffer convolution(const gComplexBuffer &vector1,const gComplexBuffer &vector2);
        //! Fast Fourier Transform
        static gComplexBuffer fft(const gComplexBuffer &vector);
        //! Segmented fast fourier transform for large vectors.
        /** Use this for large signals such as audio.
          \param vector: source of signal.
          \param _size: size of segment, e.g 1024
          \return transformed vector */
        static gComplexBuffer sectfft(const gComplexBuffer &vector,gu32 _size=0);
        static gComplexBuffer fftInv(const gComplexBuffer &vector);
        static gComplexBuffer sectfftInv(const gComplexBuffer &vector,gu32 _size=0);
        //utility functions
        static gScalarArray createTDSignal(int type,int size,gScalar frequency, gScalar amplitude);
        static gScalarArray createMFSignal(int type,int size,const gScalarArray &frequencies, const gScalarArray &amplitudes);
        //! Mixes to signals.
        /** This function uses linear interpolation
            \param signal1: Signal source 1.
            \param signal2: Signal source 2.
            \param out: Out signal
            \param fact: Mixing factor. When this is equal to 1 the resulting signal would be signal1.
            When 0 this would the resulting singnal would be signal2. Two mix half and half make sure this value is 0.5 */

        static void mixSignals(const gScalarArray &signal1,const gScalarArray &signal2,gScalarArray &out,
                               gScalar fact);
        //! Sum two signals.
        static void sumSignals(const gScalarArray &signal1,const gScalarArray &signal2,gScalarArray &out);
        static gComplexBuffer applyFilter(const gComplexBuffer &signal, const gComplexBuffer &filter);
        //! Creates a Time Domain Filter.
        static gComplexBuffer createTDFilter(const gFreqDescriptor &desc, gu32 _size, gu32 kernelSize);
        //! Creates a Frequency Domain Filter.
        static gComplexBuffer createFDFilter(const gFreqDescriptor &desc,gu32 _size, gu32 kernelSize);
    };
}
#endif // GDSP_H
