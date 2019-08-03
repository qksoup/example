#ifndef EXAMPLE_COMMON_HPP
#define EXAMPLE_COMMON_HPP
// c include
#include <string.h>
#include <stdlib.h>
#include <assert.h>
// c++ include
#include <iostream>
#include <sstream>

#define ABORT(msg) do {                                                 \
    int errnum = errno;                                                 \
    char buf[256];                                                      \
    std::stringstream ss;                                               \
    ss << __FILE__ << ':' << __LINE__ << ' ' << msg;                    \
    std::cerr << ss.str() << std::endl;                                 \
    if (errnum != 0) {                                                  \
        char* errmsg = strerror_r(errnum, buf, sizeof(buf));            \
        std::cerr << "sys error: " << errmsg << std::endl;              \
    }                                                                   \
    ::abort();                                                          \
} while (0);

enum {
    AlignMask = sizeof(void*) - 1
};

inline size_t align(size_t n) 
{
    return (n + AlignMask) & ~(AlignMask);
}

inline void rmembar()
{
    asm volatile ("" : : : "memory" );
}
    
inline void wmembar()
{
    asm volatile ("" : : : "memory" );
}
    
inline void membar()
{
    asm volatile ("" : : : "memory" );
}

class RingBuf {
public:
    RingBuf(size_t bufsz);
    virtual ~RingBuf();
    void* alloc(size_t n);
    void freeze(size_t n);
    static void dealloc(void* ptr);
private:
    struct Block {
        int used;
        int size;
    };
    int m_bufsz;
    int m_navail;
    char* m_buf;
    char* m_head;
    char* m_tail;
    void* tryAlloc(size_t n);
    void reclaim();
};

#endif
