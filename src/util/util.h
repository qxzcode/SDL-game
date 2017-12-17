#pragma once

namespace util {
    
    class PRNG {
    public:
        PRNG();
        PRNG(const PRNG& other);
        ~PRNG();
        
        PRNG& operator=(PRNG rhs) {
            swap(*this, rhs);
            return *this;
        }
        
        friend void swap(PRNG& a, PRNG& b);
        
        /// random double in the range [0,1)
        double operator()();
        
    protected:
        struct impl_t;
        impl_t* impl;
        
    };
    
    void swap(PRNG& a, PRNG& b);
    
    extern thread_local PRNG rand;
    
    
    // mathematical constants
    constexpr double PI = 3.14159265358979323846264338327950288;
    constexpr double E = 2.71828182845904523536028747135266250;
    constexpr double SQRT2 = 1.41421356237309504880168872420969808;
    constexpr double SQRT1_2 = 0.70710678118654752440084436210484904;
    
}