#include "RadioInterface.h"

namespace _5g_connectedvehicle {

Define_Module(RadioInterface);

void RadioInterface::initialize()
{
    // Initialize the packet queue and the end transmission event
    packetQueue.setName("vehicleQueue");
    endTransmissionEvent = new cMessage("endTransmissionEvent");
    isBusy = false;
}

void RadioInterface::handleMessage(cMessage *msg)
{
    // Check if the message is a VehiclePacket or a HandoverResponse
    if(msg == endTransmissionEvent) {
        // If the message is the end transmission event, call the endTransmission function
        endTransmission();
    } else
    if(VehiclePacket *pkt = dynamic_cast<VehiclePacket *>(msg))
        // Handle VehiclePacket
        handlePacket(pkt);
    else if(HandoverResponse *res = dynamic_cast<HandoverResponse *>(msg))
        // Forward HandoverResponse to the vehicle's Handover System
        transmitPacket(res);
    else
        throw cRuntimeError("[Vehicle:RI]: received an unknown message type");
}

// Handle incoming VehiclePacket
void RadioInterface::handlePacket(VehiclePacket *packet)
{
    if (isBusy) {
        // If the radio interface is busy, enqueue the packet
        packetQueue.insert(packet);
    } else {
        // If the radio interface is free, send the packet immediately
        transmitPacket(packet);
    }

}

// After a transmission send packet in the queue or set not busy
void RadioInterface::endTransmission()
{
    if (!packetQueue.isEmpty()) {
        // If there are packets in the queue, dequeue the next packet and send it
        VehiclePacket *nextPacket = check_and_cast<VehiclePacket *>(packetQueue.pop());
        transmitPacket(nextPacket);
    } else {
        // If there are no packets in the queue, mark the interface as not busy
        isBusy = false;
    }
}

// Function to transmit a packet and hadle trasmission time
void RadioInterface::transmitPacket(VehiclePacket *packet)
{   
    isBusy = true;
    // Get the distance to the base station from the mobility system
    //TODO
    double transmissionTime = 0.1;  //TODO
    // Send the packet to the base station through the antenna output
    sendDirect(packet, transmissionTime, "antenna$o");
    // Schedule the end transmission event after the transmission time
    scheduleAt(simTime() + transmissionTime, endTransmissionEvent);
}

void RadioInterface::finish()
{
    // Clean up the end transmission event
    cancelAndDelete(endTransmissionEvent);
}

}
