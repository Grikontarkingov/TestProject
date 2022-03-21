#include "RingBufferOne.h"

#include <iostream>

RingBufferOne::RingBufferOne(size_t capacity) {
    buffer = new RingBuffer;
    buffer->capacity = capacity;
    buffer->readIndex = 0;
    buffer->writeIndex = 0;
    buffer->size = 0;
    buffer->lockRead = false;
    buffer->lockWrite = false;
    buffer->head = nullptr;
}

RingBufferOne::~RingBufferOne() {
    RingBufferData* current = buffer->head;
    RingBufferData* temp;
    for(size_t i = 1; i <= buffer->capacity; ++i) {
        temp = current;
        if(current->next != nullptr) {
            current = current->next;
            delete temp;
        }
        else {
            delete temp;
            break;
        }
    }

    delete buffer;
}

void RingBufferOne::AddInBuffer(uint16_t data) {
    CheckLockWrite();
    if(buffer->lockWrite) {
        std::cout << "Write is lock" << std::endl;
        return;
    }

    RingBufferData* newData = new RingBufferData;
    newData->data = data;
    newData->next = nullptr;

    RingBufferData* current = buffer->head;
    if(current == nullptr){
        buffer->head = newData;
        buffer->writeIndex++;
        buffer->size++;
    }
    else if (buffer->writeIndex == 0){
        for(size_t i = 1; i < buffer->capacity; ++i) {
            current = current->next;
        }

        newData->next = buffer->head->next;
        buffer->head = newData;
        current->next = buffer->head;
        buffer->writeIndex++;
        buffer->size++;
    }
    else if ((buffer->writeIndex + 1) == buffer->capacity) {
        for(size_t i = 1; i < buffer->writeIndex; ++i) {
            current = current->next;
        }
        newData->next = buffer->head;
        current->next = newData;
        buffer->writeIndex++;
        buffer->size++;
    }
    else {
        for(size_t i = 1; i < buffer->writeIndex; ++i) {
            current = current->next;
        }

        current->next = newData;
        buffer->writeIndex++;
        buffer->size++;
    }
}

void RingBufferOne::PrintNode() {
    CheckLockRead();
    if(buffer->lockRead) {
        std::cout << "Read is lock" << std::endl;
        return;
    }

    RingBufferData* current = buffer->head;
    for(size_t i = 1; i <= buffer->readIndex; ++i) {
        current = current->next;
    }
    std::cout << "Data: " << current->data << std::endl;

    if(buffer->readIndex == buffer->capacity) {
        buffer->readIndex = 0;
    }
    else{
        buffer->readIndex++;
    }
    buffer->size--;
}

void RingBufferOne::PrintAllBuffer() {
    RingBufferData* current = buffer->head;
    while(current != nullptr && buffer->size != 0) {
        std::cout << "Data: " << current->data << std::endl;
        current = current->next;
        buffer->size--;
    }
}

void RingBufferOne::CheckLockWrite() {
    if(buffer->size == buffer->capacity) {
        buffer->lockWrite = true;
    }
    else{
        buffer->lockWrite = false;
    }
}

void RingBufferOne::CheckLockRead() {
    if(buffer->size == 0) {
        buffer->lockRead = true;
    }
    else {
        buffer->lockRead = false;
    }
}