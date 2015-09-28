#include "CMDList.h"


CMDList::CMDList(int capacity)
{
    list = (CMD*) malloc(sizeof(CMD) * capacity);
    nextFreePos = 0;
    headPos = 0;
    full = false;
    capa = capacity;
}

CMDList::~CMDList()
{
    free((char*)list);
}

bool CMDList::enqueue(typeCMD cmd , int64_t val, int64_t val2)
{
    if (full) {
        return false;
    } else {
        list[nextFreePos].type = cmd;
        list[nextFreePos].value = val;
        list[nextFreePos].secValue = val2;
        nextFreePos = (nextFreePos + 1) % capa;

        if (nextFreePos == headPos) {
            full = true;
        }

        return true;
    }
}


int CMDList::size()
{
    if (full) {
        return capa;
    }

    return nextFreePos - headPos;
}


CMD CMDList::dequeue()
{
    CMD res;
    res.type = CMD_NULL;

    if (!full && nextFreePos == headPos) {
        return res; // List if empty !
    }

    if (nextFreePos == headPos) {
        full = false;
    }

    int returnIndex = headPos;
    headPos = (headPos + 1) % capa;

    res.type = list[returnIndex].type;
    res.value = list[returnIndex].value;
    res.secValue = list[returnIndex].secValue;
    return res;
}
