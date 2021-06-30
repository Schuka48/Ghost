#ifndef TC_H
#define TC_H

#include "Interface.h"

#include <QObject>
#include <QPair>


typedef QPair<int, int> pair;

class Interface;

class tc
{
    static uint tc_id;
    uint id;
    bool is_new;
    pair  jitter;
    pair loss;
    pair package_order;
    int noise;
    Interface* interface;

    QJsonArray get_arr(pair params);

public:
    tc();
    tc(Interface* inetrface, pair jitter = pair(0,0), pair loss = pair(0,0), pair package_order = pair(0,0), int noise = 0);
    void set_is_new();
    Interface* get_int();
    void set_param(pair jitter, pair loss, pair package_order, int noise);
    QJsonObject toObject();
};

#endif // TC_H
