#pragma once

#include <cstdint>

typedef struct RingBufferData {
    uint16_t data;
    struct RingBufferData* next;
}RingBufferData;

typedef struct RingBuffer {
    size_t capacity;
    size_t size;
    size_t readIndex;
    size_t writeIndex;
    bool lockRead;
    bool lockWrite;
    RingBufferData* head;
}RingBuffer;

class RingBufferOne {
public:
    explicit RingBufferOne(size_t capacity);
    ~RingBufferOne();

    void AddInBuffer(uint16_t data);
    void PrintNode();

    void PrintAllBuffer();

private:
    void CheckLockWrite();

    void CheckLockRead();

    RingBuffer* buffer;
};


