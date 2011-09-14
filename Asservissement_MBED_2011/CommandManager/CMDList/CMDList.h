#ifndef CMD_LIST
#define CMD_LIST

#include "../mbed.h"

// Internal Structures
enum typeCMD {
    CMD_NULL, // Erreur....
    CMD_GO,
    CMD_TURN,
    CMD_WAIT,
    CMD_STOP,
    CMD_CIRCLE,
    CMD_GOTO,
    CMD_GOTOANGLE,
};

typedef struct CMD_struct {
    typeCMD type;
    int64_t value;
    int64_t secValue;
} CMD;

class CMDList {
    public:
        CMDList(int capacity);
        bool enqueue(typeCMD cmd , int64_t val, int64_t val2=0);
        CMD dequeue();
        int size();
        
    private:
        CMD *list;
        int capa;
        int nextFreePos;
        int headPos;
        bool full;
};

#endif