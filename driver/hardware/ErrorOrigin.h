//
// Created by grant_ogz8xax on 5/25/2020.
//

#ifndef OPENBB_ERRORORIGIN_H
#define OPENBB_ERRORORIGIN_H
#include <QString>

namespace OpenBB {
    enum ErrorOrigin {

        DRIVER,

        SOCKET,
        
    };

    QString toReadableOrigin(ErrorOrigin origin) {
        switch(origin) {
            case DRIVER:
                return "DRIVER";
            case SOCKET:
                return "SOCKET";
            default:
                return "UNKNOWN ORIGIN";
        }
    }
}

#endif //OPENBB_ERRORORIGIN_H
