#include "tc.h"

uint tc::tc_id = 1;

QJsonArray tc::get_arr(pair params)
{
    QJsonArray jArr;
    jArr.append(params.first);
    jArr.append(params.second);
    return jArr;
}

tc::tc()
{
    this->interface = nullptr;
    this->id = tc_id++;
    this->is_new = true;
    this->jitter = pair(0,0);
    this->loss = pair(0,0);
    this->package_order = pair(0,0);
    this->noise = 0;
}

tc::tc(Interface* interface, pair jitter, pair loss, pair package_order, int noise)
{
    this->interface = interface;
    this->id = tc_id++;
    this->is_new = true;
    this->jitter.first = jitter.first;
    this->jitter.second = jitter.second;
    this->loss.first = loss.first;
    this->loss.second = loss.second;
    this->package_order.first = package_order.first;
    this->package_order.second = package_order.second;
    this->noise = noise;
}

void tc::set_is_new()
{
    this->is_new = false;
}

Interface *tc::get_int()
{
    return this->interface;
}

void tc::set_param(pair jitter, pair loss, pair package_order, int noise)
{
    this->jitter = jitter;
    this->loss = loss;
    this->package_order = package_order;
    this->noise = noise;
}

QJsonObject tc::toObject()
{
    QJsonObject jObj;
    jObj.insert("interface_name", interface->get_int_name());
    jObj.insert("jitter", get_arr(jitter));
    jObj.insert("loss", get_arr(loss));
    jObj.insert("package_order", get_arr(package_order));
    jObj.insert("noise", noise);
    return jObj;
}


