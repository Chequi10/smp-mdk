#ifndef MESSAGE_H
#define MESSAGE_H

struct Message
{
    Message() {}
    uint32_t originator_id;
    uint32_t Can_id;
    uint32_t Len;
    char Payload[8];
};

#endif //