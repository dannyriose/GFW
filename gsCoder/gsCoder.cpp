#include "gsCoderPrivate.h"
using namespace gfw;
#define ca gsCodePrivate *o=(gsCodePrivate *)d
#define co gsCodePrivate *io=(gsCodePrivate *)other.h()
#define acn gassert(d,"Object is empty")


gsCoder::gsCoder():gBaseShared(new gsCodePrivateFactory)
{

}

gsCoder::gsCoder(const gsCoder &other):gBaseShared(other,new gsCodePrivateFactory){


}
gsCoder::~gsCoder(){

}
gsCoder &gsCoder::operator =(const gsCoder &other)
{
    assign(other);
    return *this;
}

bool gsCoder::t_init(gu32 w, gu32 h)
{
    ca;
    if(o)
    {
        o->clear();
        o->alloc(w,h);

    }
    else
    {
        o=new gsCodePrivate();
        d=o;

        o->alloc(w,h);
    }
    return d!=0;
}
//this function creates a base table.
int gsCoder::t_base(bool bRandom, int iRandomSeed)
{

    int x,y,seed=0;
    gu16 val=0;
    int inc=0x00FF;

    t_init(256,256);
    ca;
    if(bRandom)
    {
        if(iRandomSeed>0)//given a seed you can tell the function which value you will start adding to the table
        {
            seed=iRandomSeed;
            val=iRandomSeed;

        }
        else
        {
            //we use a random value
            srand(time(NULL));
            seed=rand();
            RAND_RANGE(seed,0,65535,RAND_MAX);
            val=seed;
        }
     }
     for(x=0;x<256;x++)
     {

        for(y=0;y<256;y++)
        {
            o->table[x][y]=val;
            if(bRandom)
            {
            //  inc*=-1;
                val+=inc;//when random the table is filled starting from our random value
                //adding increments of 255 in this way we make sure table values are unique
                //and variable
            }
            else
            {
                //if not random is specified then values increment is by 1
                val++;
            }
   //            fprintf(stderr,"%d ",val);
        }
     }
     return seed;//return the seed useful when you get rand seed from the srand
}


void gsCoder::t_setvalue(gu32 x, gu32 y, int value)
{
    acn;
    ca;
    gassert(o->table,"Table has not been created");
    o->table[x][y]=value;

}

void gsCoder::setKey(const gString &_key)
{
    acn;
    ca;
    o->key=_key;
}

const gString &gsCoder::key() const
{
    acn;
    ca;
    return (const gString &)o->key;
}

//encoder
/* Encripting a string is really easy, its complexity depends upon how the table values are filled.
  Each ascii character from our string and key value are used coordinates on the table.
  The encoded string is no other thing the value pulled from the table converted to hexadecimal.
  Therefore an encoded string will be always divisible by 4.
  The factor that determines how well a string is encripted is in how the table is filled. Random values
  is the best bet to make sure the string is hard to decode. The best chance is to implement a
  custom algorithm to fill the table. */
gString gsCoder::sh_enc(const gString &value) const{
    acn;
    ca;
    gString temp;
    gString res;//Result
    gu8 *pKey;//Pointer to the key in word size
    gu8 *pSource;//Pointer to the source
    gassert(o->table,"Input Table is empty");

    if(o->key.isEmpty())return gString();

    int iSource,iKey,i,j;//Size of the array in word size
    temp=value; //we copy the source to a temp location
    iSource=temp.size();
    iKey=o->key.size();
    pKey=(gu8 *)o->key.ascii();
    pSource=(gu8 *)temp.ascii();
    j=0;
    for(i=0;i<iSource;i++)
    {
        if(j==iKey)j=0;
        //our source string is our X Coordinate on the table and the encription key the Y coordinate
        res.appendUint16AsHex(o->table[pSource[i]][pKey[j]]);
        j++;
    }
    res.setShared(true);
    return gString(res);
}
//decoder
/* Decoding a string is easy. To decode a string you have to make sure
  that both the encription key and the encription table have the same content when it was encripted or
  else the function will fail returning an empty string. */
gString gsCoder::sh_dec(const gString &enc) const
{
    acn;
    ca;

    gString res;//Result
    gUShortArray indexes;
    gu8 *pKey;//Pointer to the key in word size
    gu32 iKey,i,j,k,klast=0;//Size of the array in word size
    gassert(o->table,"Table is empty");

    if(o->key.isEmpty())return gString();
    //temp=enc; //we copy the source to a temp location

    iKey=o->key.size();
    pKey=(gu8 *)o->key.ascii();
    indexes=gString::fromHexToUint16Array(enc);
    i=0;
    k=0;
    j=0;
    gu16 *ivalues=indexes.data();
    res.alloc(indexes.size(),gString::ASCII);
    char *dstr=res.ascii();
    while(i<indexes.size())
    {
        if(j==o->width)
        {
            if(klast==k)
            {
                return gString();//Invalid key
            }
            j=0;
        }
        if(k==iKey)k=0;
        klast=k;
        if(ivalues[i]==o->table[j][pKey[k]])
        {
            //res+=(char)j;
            dstr[i]=j;
            i++;
            k++;
            j=0;
        }
        else
        {
                j++;
        }
    }
    res.setShared(true);
    return gString(res);
}
