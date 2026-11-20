#ifndef ___5G_CONNECTEDVEHICLE_RADIOINTERFACE_H_
#define ___5G_CONNECTEDVEHICLE_RADIOINTERFACE_H_

#include <omnetpp.h>

using namespace omnetpp;

namespace _5g_connectedvehicle {

class RadioInterface : public cSimpleModule
{
private:
    cQueue packetQueue; // Queue to hold packets waiting to be sent
    cMessage *endTransmissionEvent; // Self-message to indicate the end of a transmission
    bool isBusy; // Flag to indicate if the radio interface is currently busy transmitting
    // Function to send a packet to the BS
    void handlePacket(VehiclePacket *packet);

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;
};

}

#endif
