//
// Created by grant_ogz8xax on 5/17/2020.
//

#ifndef OPENBB_DRIVERSTATUS_H
#define OPENBB_DRIVERSTATUS_H
#include <QString>

namespace OpenBB {
    enum DriverStatus {
        CONFIGURING,
        CONFIGURED,
        REQUESTING,
        REQUESTED,
        QUERYING,
        QUERIED,
        SEEDING,
        SEEDED,
        QUEUING,
        QUEUED,
        STREAM,
        STREAMING,
        UNSTREAM,
        UNSTREAMED,
        DEQUEUING,
        DEQUEUED,
        CLOSED,
    };

    QString toReadableStatus(DriverStatus status) {
        switch(status) {
            case CONFIGURING:
                return "CONFIGURING";
            case CONFIGURED:
                return "CONFIGURED";
            case REQUESTING:
                return "REQUESTING";
            case REQUESTED:
                return "REQUESTED";
            case QUERYING:
                return "QUERYING";
            case QUERIED:
                return "QUERIED";
            case SEEDING:
                return "SEEDING";
            case SEEDED:
                return "SEEDED";
            case QUEUING:
                return "QUEUING";
            case QUEUED:
                return "QUEUED";
            case STREAM:
                return "STREAM";
            case STREAMING:
                return "STREAMING";
            case UNSTREAM:
                return "UNSTREAM";
            case UNSTREAMED:
                return "UNSTREAMED";
            case DEQUEUING:
                return "DEQUEUING";
            case DEQUEUED:
                return "DEQUEUED";
            case CLOSED:
                return "CLOSED";
            default:
                return "UNKNOWN STATUS";
        }
    }
}

#endif //OPENBB_DRIVERSTATUS_H
