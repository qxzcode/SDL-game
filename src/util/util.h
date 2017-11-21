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
    
}