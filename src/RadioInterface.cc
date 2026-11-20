#include "RadioInterface.h"

namespace _5g_connectedvehicle {

Define_Module(RadioInterface);

void RadioInterface::initialize()
{
    // Initialize the packet queue and the end transmission event
    packetQueue.setName("packetQueue");
    endTransmissionEvent = new cMessage("endTransmissionEvent");
    isBusy = false;
}

void RadioInterface::handleMessage(cMessage *msg)
{
    // Check if the message is a VehiclePacket or a HandoverResponse
    if(VehiclePacket *pkt = dynamic_cast<VehiclePacket *>(msg))
        // Handle VehiclePacket
        handlePacket(pkt);
    else if(HandoverResponse *res = dynamic_cast<HandoverResponse *>(msg))
        // Forward HandoverResponse to the vehicle's Handover System
        send(res, "handoverIO");
    else
        throw cRuntimeError("[Vehicle:RI]: received an unknown message type");
}

void RadioInterface::handlePacket(VehiclePacket *packet)
{
    

}
