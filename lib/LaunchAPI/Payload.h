#ifndef PAYLOAD_H
#define PAYLOAD_H

#include <stddef.h>

class Payload {
public:
    size_t readFrom(void* data);
    size_t writeTo(void* data);
private:
    // TODO
};

#endif /* PAYLOAD_H */