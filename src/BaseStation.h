#ifndef ___5G_CONNECTEDVEHICLE_BASESTATION_H_
#define ___5G_CONNECTEDVEHICLE_BASESTATION_H_

#include <omnetpp.h>

using namespace omnetpp;

namespace _5g_connectedvehicle {

class BaseStation : public cSimpleModule
{
  protected:
    virtual void handleMessage(cMessage *msg) override;
};

}

#endif
