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
    // Function that handles incoming VehiclePacket messages incoming
    void handlePacket(VehiclePacket *packet);
    // After a transmission send packet in the queue or set not busy
    void endTransmission();
    // Function to transmit a packet and hadle trasmission time
    void transmitPacket(VehiclePacket *packet);

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;
};

}

#endif
