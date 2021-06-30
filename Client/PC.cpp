#include "PC.h"


uint PC::pc_id = 0;

QList<Interface*> &PC::get_connected_interfaces()
{
    return int_connected;
}

QList<VPC *> &PC::get_connected_vpc()
{
    return vpc_connected;
}

PC::PC(Network * net, QString& ip_addr)
{
    this->id = ++pc_id;
    this->net = net;
    this->connected_bridge = nullptr;
    this->ip_addr = ip_addr;
}

PC::PC()
{
    this->id = ++pc_id;
    this->net = nullptr;
    this->connected_bridge = nullptr;
}

uint PC::get_pc_id()
{
    return this->id;
}

bool PC::set_pc_bridge(Bridge *bridge)
{
    if(this->connected_bridge)
    {
        return false;
    }
    else {
        this->connected_bridge = bridge;
        return true;
    }
}

Bridge *PC::get_connected_bridge()
{
    return this->connected_bridge;
}

void PC::set_mask(uint mask)
{
    this->mask = mask;
}

void PC::push_back_vpc(VPC *vpc)
{
    if(vpc) {
        this->vpc_connected.push_back(vpc);
    }
}

void PC::push_back_int(Interface *interface)
{
    this->int_connected.push_back(interface);
}

QString PC::get_ip_addr()
{
    return this->ip_addr;
}

uint PC::get_mask()
{
    return this->mask;
}

