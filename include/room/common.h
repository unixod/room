#ifndef ROOM_COMMON_H
#define ROOM_COMMON_H

#define STRINGIFY2(x) #x
#define STRINGIFY(x) STRINGIFY2(x)

#define ERROR_MSG \
    __FILE__ ":" STRINGIFY(__LINE__) ", "

#define INTERNAL_ERROR \
    __FILE__ ":" STRINGIFY(__LINE__) ": internal error: "

#endif // ROOM_COMMON_H
