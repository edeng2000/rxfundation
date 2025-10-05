
#ifndef __RX__MATH_H____
#define __RX__MATH_H____

namespace RX
{   
    template<typename T >
    inline const T SDMin(const T & __a, const T & __b)
    {
        return (__b < __a ) ? __b: __a;
    }

 
    template<typename T >
    inline const T SDMax(const T & __a, const T & __b)
    {
        return (__a < __b)? __b : __a;
    }

    /** @} */

} //namespace GNDP

#endif // 


