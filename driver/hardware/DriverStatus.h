//
// Created by grant_ogz8xax on 5/17/2020.
//

#ifndef OPENBB_DRIVERSTATUS_H
#define OPENBB_DRIVERSTATUS_H

enum DriverStatus {
    CONFIGURING,
    REQUESTING,
    QUERYING,
    SEEDED,
    QUEUING,
    STREAMING,
    UNSTREAMING,
    DEQUEUING,
    CLOSED,
};

#endif //OPENBB_DRIVERSTATUS_H
