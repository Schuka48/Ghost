#include "VPC.h"

uint VPC::vpc_id = 0;

VPC::VPC(QString name, Network* net){
    this->id = ++vpc_id;
    this->vpc_name = name;
    this->net = net;
}

VPC::VPC()
{
    this->id = ++vpc_id;
    this->net = nullptr;
}

void VPC::set_pc_connected(PC* const &pc_connected)
{
    this->pc_connected = pc_connected;
}

PC *VPC::get_connected_pc()
{
    return this->pc_connected;
}

QJsonObject VPC::toObject()
{
    QJsonObject jObj;
    QString name = this->vpc_name;
    jObj.insert("name", name);
    int id_vpc = this->id;
    jObj.insert("id", id_vpc);
    return jObj;
}

QString VPC::get_vpc_name()
{
    QString tmp = "";
    tmp = vpc_name;
    return tmp;
}
void VPC::push_back_interface(const Interface &intreface)
{
    included_interfaces.push_back(intreface);
}

QList<Interface> &VPC::get_int_connected()
{
    return included_interfaces;
}

uint VPC::get_id()
{
    return this->id;
}
