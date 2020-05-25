//
// Created by grant_ogz8xax on 3/19/2020.
//

#ifndef OPENBB_BUFFERMETA_H
#define OPENBB_BUFFERMETA_H

namespace OpenBB {
    class BufferMeta {
    public:
        explicit BufferMeta(void* start, unsigned length, int fd): start{start}, length{length}, fd{fd} {}
        void* start;
        unsigned length;
        int fd;
    };
}



#endif //OPENBB_BUFFERMETA_H
