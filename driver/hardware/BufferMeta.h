//
// Created by grant_ogz8xax on 3/19/2020.
//

#ifndef OPENBB_BUFFERMETA_H
#define OPENBB_BUFFERMETA_H


class BufferMeta {
public:
    explicit BufferMeta(void* start, unsigned length): start{start}, length{length} {}
    void* start;
    unsigned length;
};


#endif //OPENBB_BUFFERMETA_H
