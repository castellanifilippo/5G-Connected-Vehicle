#ifndef ___5G_CONNECTEDVEHICLE_MOBILITYSERVER_H_
#define ___5G_CONNECTEDVEHICLE_MOBILITYSERVER_H_

#include <omnetpp.h>

using namespace omnetpp;

namespace _5g_connectedvehicle {

class MobilityServer : public cSimpleModule
{
private:
    double L;  // length of the highway
    int N;  // number of base stations
private:
    int closestBS(double position, int currentBS);
protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

}

#endif
