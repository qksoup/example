#include "common.hpp"

RingBuf::RingBuf(size_t bufsz)
{
    bufsz = align(bufsz);
    m_buf = (char*)::malloc(bufsz);
    if (m_buf == nullptr) {
        ABORT("failed to create chunk " << bufsz);
    } else {
        std::cerr << "created chunk " << m_bufsz << std::endl;
    }
    m_bufsz = m_navail = bufsz;
    m_head = m_tail = m_buf;
}

RingBuf::~RingBuf()
{
    ::free(m_buf);
}

void* RingBuf::tryAlloc(size_t n)
{
    if (m_navail < (int)(sizeof(Block) + n))
        return nullptr;
    Block* block = (Block*)m_head;
    if (m_head >= m_tail) {
        int ravail = m_buf + m_bufsz - m_head;
        assert(ravail >= 0 && ravail <= m_bufsz);
        assert(ravail <= m_navail);
        if (ravail >= (int)(sizeof(Block) + n))
            return block + 1;
        // mark the rest used, but ready for reclaim
        block->used = 0;
        block->size = ravail - sizeof(Block);
        m_navail -= ravail;
        assert(m_navail >= 0 && m_navail <= m_bufsz);
        m_head = m_buf;
        block = (Block*)m_head;
    }
    // now m_head <= m_tail
    if (m_navail < (int)(sizeof(Block) + n))
        return nullptr;
    return block + 1;
}

void RingBuf::reclaim()
{
    int oldn = m_navail;
    rmembar();
    Block* block = (Block*)m_tail;
    while (!block->used && m_navail < m_bufsz) {
        m_navail += sizeof(Block) + block->size;
        m_tail += sizeof(Block) + block->size;
        if (m_tail == m_buf + m_bufsz)
            m_tail = m_buf;
        block = (Block*)m_tail;
    }
    assert(m_navail <= m_bufsz);
    std::cerr << "reclaimed " << (m_navail - oldn) << " bytes" << std::endl;
    if (m_navail == m_bufsz) {
        m_head = m_tail = m_buf;
        std::cerr << "reset to fresh" << std::endl;
    }
}

void* RingBuf::alloc(size_t n)
{
    n = align(n);
    if (n > m_bufsz - sizeof(Block))
        ABORT("ringbuf alloc request is too large " << n);
    void* ptr = tryAlloc(n);
    if (ptr == nullptr) {
        reclaim();
        ptr = tryAlloc(n);
        if (ptr == nullptr)
            ABORT("ringbuf failed to alloc " << n);
    }
    Block* block = (Block*)m_head;
    block->size = n;
    return ptr;
}

void RingBuf::freeze(size_t n)
{
    n = align(n);
    Block* block = (Block*)m_head;
    assert(block->size >= (int)n);
    block->used = 1;
    block->size = n;
    m_navail -= (sizeof(Block) + n);
    m_head += (sizeof(Block) + n);
    if (m_head == m_buf + m_bufsz)
        m_head = m_buf;
}

void RingBuf::dealloc(void* ptr)
{
    Block* block = ((Block*)ptr) -1;
    block->used = 0;
    wmembar();
}
