#include "gDsp.h"
using namespace gfw;
gDsp::gDsp()
{
}
//Applies the fast fourier transform to a vector
gComplexBuffer gDsp::fft(const gComplexBuffer &vector)
{
    gComplexBuffer res;//This would be our result vector
    gu32 *brindex,n,i,j,k=0;
    gu32 stages=0,rev=0,mask,halfpoints;
    gu32 mstep=1;
    gu32 step=0,index,size;
    float Sr=-1,Si=0,Ur=1,Ui=0,Tr,Ti,next;
    gUIntArray t_brindex;
    gComplexNumber *base,*source;
    //original
    //brindex=new uint32[complex->size];
    //new
    //Alloc memory
    t_brindex.alloc(vector.size());
    brindex=t_brindex.data();
    //n=complex->size;
    n=t_brindex.size();
    size=n;
    halfpoints=n;
    //Get how many stages would have the transform
    while(n!=1)
    {
        stages++;
        n>>=1;
    }
    halfpoints>>=1;
    //get the bit reversal
    for(i=0;i<size-1;i++)//
    {
        brindex[i]=rev;
        mask=halfpoints;
        while(rev>=mask)
        {
            rev-=mask;
            mask>>=1;
        }
        rev+=mask;
     }
     brindex[size-1]=size-1;
     //Original
     /*   res=new COMPLEXF;
        res->img=new float[complex->size];
        res->real=new float[complex->size];
        res->size=complex->size;
        memset(res->img,0,complex->size*sizeof(float));
        memset(res->real,0,complex->size*sizeof(float));*/
     //New
     //intialize our return vector as zeros
     res=gComplexBuffer::zeros(size);
     base=res.data();//get the pointer to make sure this goes fast
     source=vector.data();
     //store in res vector our bitreversal values
    for(i=0;i<size;i++)
    {
        //res->real[i]=complex->real[brindex[i]];
        base[i].real=source[brindex[i]].real;
            /*if(complex->img)
            {
                res->img[i]=complex->img[brindex[i]];
            };*/
        base[i].img=source[brindex[i]].img;
    }
    //delete [] brindex;

    //Sintesis of the transform
    for(i=0;i<stages;i++)//follow all stages
    {
       step=mstep;//Steps to compute the butterfly initially 1
       mstep<<=1; //Jumps to the next element
       Ur=1;//To get the butterfly in R
       Ui=0;//To get the butterfly in I

       for(j=0;j<step;j++)//Pasos para calcular
       {
            for(k=j;k<size;k+=mstep)//Se hace iteraccion en cada punto del espectro
            {
                index=k+step;//Calculamos el indice del segundo elemento de la mariposa
                //Tr= Ur * real[index] - Ui * res->img[index];//Calculamos la mariposa
                Tr = Ur * base[index].real - Ui * base[index].img;
                //Ti= Ur * res->img[index] + Ui * res ->real[index];
                Ti = Ur * base[index].img + Ui * base[index].real;
                //res->real[index]=res->real[k]-Tr;//Asignamos calculo al segundo elemento en R
                base[index].real = base[k].real - Tr;
                //res->img[index]=res->img[k]-Ti;//Asignamos al segundo elemento en i
                base[index].img = base[k].img - Ti;
                //res->real[k]+=Tr;//Asignamos primer elemento
                base[k].real+=Tr;
                //res->img[k]+=Ti;//Asignamos primer elemento en i
                base[k].img+=Ti;
            }

            next=Ur * Sr - Ui * Si; //Se calcula los siguientes coeficientes de la mariposa para los reales
            Ui= Ur *Si + Ui * Sr;  //Para los imaginarios
            Ur=next;
       }
       Si=(float)sqrt((1-Sr)/2);//Calculamos el coeficiente W
       Si=-Si;
       Sr=(float)sqrt((1+Sr)/2);//Calculamos el coeficiente Wo
   }
   res.setShared(true);
   return gComplexBuffer(res);
}

gComplexBuffer gDsp::fftInv(const gComplexBuffer &vector)
{
    gComplexBuffer ncomp,copyofVector;
    gu32 i;
    if(vector.isEmpty())return gComplexBuffer();
    gComplexNumber *source;

    copyofVector=vector;
    source=copyofVector.data();
    for(i=0;i<vector.size();i++)
    {
        //comp->img[i]=(comp->img[i]*(-1));//Se invierten los signos
        source[i].img*=-1;
    }
    ncomp=gDsp::fft(copyofVector);

     /*   for(i=0;i<ncomp->size;i++)
        {
            ncomp->img[i]/=ncomp->size;
            ncomp->real[i]/=ncomp->size;
        };*/
    ncomp.sizeDivide();
    ncomp.setShared(true);
    return gComplexBuffer(ncomp);
}

gComplexBuffer gDsp::sectfft(const gComplexBuffer &vector, gu32 _size)
{
    gComplexBuffer section,result,ret;

    gu32 asize,reminder,i,wsize;

    if(!_size)
    {
        return gDsp::fft(vector);
    }
    asize=vector.size();
    _size=asize / _size;

    reminder= asize % _size;

    wsize=asize-reminder;
    for(i=0;i<wsize;i+=_size)
    {
        section=vector.section(i,_size);
        result=gDsp::fft(section);
        ret.append(result);
    }
    if(reminder)
    {
        section=vector.section(wsize,reminder);
        result=gDsp::fft(section);
        ret.append(result);
    }

    ret.setShared(true);
    return gComplexBuffer(ret);
}


gComplexBuffer gDsp::sectfftInv(const gComplexBuffer &vector, gu32 _size)
{
    gComplexBuffer section,result,ret;

    gu32 asize,reminder,i,wsize;

    if(!_size)
    {
        return gDsp::fftInv(vector);
    }
    asize=vector.size();
    _size=asize / _size;

    reminder= asize % _size;

    wsize=asize-reminder;
    for(i=0;i<wsize;i+=_size)
    {
        section=vector.section(i,_size);
        result=gDsp::fftInv(section);
        ret.append(result);
    }
    if(reminder)
    {
        section=vector.section(wsize,reminder);
        result=gDsp::fftInv(section);
        ret.append(result);
    }

    ret.setShared(true);
    return gComplexBuffer(ret);
}


gScalarArray gDsp::createTDSignal(int type, int size, gScalar frequency,gScalar amplitude)
{
    gScalarArray data;
    gu32 i;
    gScalar *p;
    data.alloc(size);

    p=data.data();
    for(i=0;i<size;i++)
    {
        switch(type)
        {
        case SIGNAL_SINE:
            p[i]=sin(gScalar(i * frequency)) * amplitude;
            break;
        case SIGNAL_COSINE:
            p[i]=cos(gScalar(i * frequency)) * amplitude;
            break;
        }
    }

    data.setShared(true);
    return gScalarArray(data);

}

gScalarArray gDsp::createMFSignal(int type, int size,
                                  const gScalarArray &frequencies,
                                  const gScalarArray &amplitudes)
{
    assert(!frequencies.isEmpty());
    assert(!amplitudes.isEmpty());
    assert(frequencies.size()==amplitudes.size());

    gu32 total=frequencies.size();
    gu32 i;
    gScalarArray signaltemp;
    gScalarArray out;
    gScalar *f,*a;

    f=frequencies.data();
    a=amplitudes.data();
    out.alloc(total);
    out.setMemTo(0);
    for(i=0;i<total;i++)
    {
        signaltemp=createTDSignal(type,size,f[i],a[i]);
        gDsp::mixSignals(out,signaltemp,out,gScalar(0.5f));
    }

    out.setShared(true);
    return gScalarArray(out);
}

void gDsp::mixSignals(const gScalarArray &signal1, const gScalarArray &signal2, gScalarArray &out,gScalar fact)
{
    assert(!signal1.isEmpty());
    assert(!signal2.isEmpty());

    assert(signal1.size()==signal2.size());
    gu32 i;
    if(!out.isEmpty())
    {
        if(signal1.size()!=out.size())
        {
            out.alloc(signal1.size());

        }
    }
    else
        out.alloc(signal1.size());
    gScalar *o,*s1,*s2;

    o=out.data();
    s1=signal1.data();
    s2=signal2.data();
    for(i=0;i<out.size();i++)
    {
        //linear interpolation
        o[i]=(1 - fact) * s2[i] + fact* s1[i];
        //o[i]=s1[i] + s2[i];
    }

}

void gDsp::sumSignals(const gScalarArray &signal1, const gScalarArray &signal2, gScalarArray &out){
    assert(!signal1.isEmpty());
    assert(!signal2.isEmpty());

    assert(signal1.size()==signal2.size());
    gu32 i;
    if(!out.isEmpty())
    {
        if(signal1.size()!=out.size())
        {
            out.alloc(signal1.size());

        }
    }
    else
        out.alloc(signal1.size());
    gScalar *o,*s1,*s2;

    o=out.data();
    s1=signal1.data();
    s2=signal2.data();
    for(i=0;i<out.size();i++)
    {
        o[i]=s1[i] + s2[i];
    }
}

gComplexBuffer gDsp::applyFilter(const gComplexBuffer &signal, const gComplexBuffer &filter)
{
    gu32 asize,reminder,fsize,i;
    gComplexBuffer section,result,ret;
    asize=signal.size();
    fsize=filter.size();
    asize=asize / fsize;
    reminder=asize % fsize;
    asize-=reminder;
    reminder = asize % filter.size();
    for(i=0;i<asize;i+=fsize)
    {
        section=signal.section(i,fsize);
        result=section.cMul(filter);
        ret.append(result);
    }
    if(reminder)
    {
        section=signal.section(asize,reminder);
        result=section.cMul(filter);
        ret.append(result);
    }

    ret.setShared(true);
    return gComplexBuffer(ret);
}


//Filter
gComplexBuffer gDsp::createTDFilter(const gFreqDescriptor &desc,
                              gu32 _size,gu32 kernelSize)
{
    gFreqDescriptor local;
    gu32 mid;
    gComplexBuffer ret,td;
    gComplexNumber *b;
    int i,j;
    local=desc;

    ret.create(_size);
    b=ret.data();
    ret.setAllTo(0,0);
    //first fage find middle points
    mid=(gu32)desc.bandPassMax+desc.bandPassMin+desc.highPass+desc.lowPass;
    assert(mid<=_size);

    mid=_size / 2;
    local.bandPassMax/=2;
    local.bandPassMin/=2;
    local.highPass/=2;
    local.lowPass/=2;

    //Next stage fill the ranges
    for(i=0;i<local.lowPass;i++)
        b[i]=1.0f;
    for(i=local.bandPassMin;i<local.bandPassMax;i++)
        b[i]=1.0f;
    if(local.highPass)
    {
        for(i=mid -1;i>=local.highPass;i--)
            b[i]=1.0f;
    }
    //Now backwards
    for(i=mid,j=mid-1;i<(gs32)_size;i++,j--)
        b[i]=b[j];

    //Next part
    //Since this is a frequency domain filter we proceed to create
    //a time domain
    td=gDsp::fftInv(ret);

    gScalarArray temp;
    gScalar *ptemp;
    gu32 index,midkernel;
    temp.alloc(_size);
    ptemp=temp.data();
    b=td.data();
    //Rotate the filter
    midkernel=kernelSize/2;
    for(i=0;i<(gs32)_size;i++)
    {
        index=i + midkernel;
        if(index>=_size)index-=_size;
        ptemp[index]=b[i].real;
    }
    //Pass this new result to the complex vector
    for(i=0;i<(gs32)_size;i++)
    {
        if(i<=(gs32)kernelSize)
        {
            b[i].real=(ptemp[i]*(0.54 -(0.46 * cos(2 * G_PI * (i/kernelSize)))));
        }
        else
            b[i]=0;
    }
    //We do not need temp anymore so clear


    //proceed with haming windows

    td.setShared(true);
    return gComplexBuffer(td);
}

gComplexBuffer gDsp::createFDFilter(const gFreqDescriptor &desc, gu32 _size, gu32 kernelSize)
{
    gComplexBuffer fd;

    fd=gDsp::createTDFilter(desc,_size,kernelSize);

    return gDsp::fft(fd);
}

