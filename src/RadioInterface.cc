#include "RadioInterface.h"
#include "MobilitySystem.h"
#include "HandoverResponse_m.h"
#include "VehiclePacket_m.h"

namespace _5g_connectedvehicle {

Define_Module(RadioInterface);

void RadioInterface::initialize()
{
    L = par("L").doubleValue();
    N = par("N").intValue();
    BSRoadOffset = par("BSRoadOffset").doubleValue();
    alpha = par("alpha").doubleValue();
    // Initialize the packet queue and the end transmission event
    packetQueue.setName("vehicleQueue");
    endTransmissionEvent = new cMessage("endTransmissionEvent");
    isBusy = false;
    // Register the queue length signal
    queueLengthSignal = registerSignal("queueLength");
    emit(queueLengthSignal, packetQueue.getLength());
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
        send(res, "handoverIO");
    else
        throw cRuntimeError("[Vehicle:RI]: received an unknown message type");
}

// Handle incoming VehiclePacket
void RadioInterface::handlePacket(VehiclePacket *packet)
{
    if (isBusy) {
        // If the radio interface is busy, enqueue the packet
        packetQueue.insert(packet);
        emit(queueLengthSignal, packetQueue.getLength());
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
        emit(queueLengthSignal, packetQueue.getLength());
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
    MobilitySystem *mobilitySystem = check_and_cast<MobilitySystem *>(getParentModule()->getSubmodule("mobilitySystem"));
    if (!mobilitySystem) {
        throw cRuntimeError("[Vehicle:RI]: MobilitySystem submodule not found");
    }
    double position = mobilitySystem->getPosition();
    double currentBSPosition = mobilitySystem->getIdBS() * (L / N - 1);
    double distance = std::abs(position - currentBSPosition);
    // Need to account the distance from the road to the base station
    distance = std::sqrt(distance * distance + BSRoadOffset * BSRoadOffset);

    // Calculate the transmission time based on the distance to the base station S = alpha * distance^2
    double transmissionTime = alpha * distance * distance;

    // Send the packet to the base station through the antenna output
    BaseStation *baseStation = getParentModule()->getSubmodule("baseStation", mobilitySystem->getIdBS());
    if (!baseStation) {
        throw cRuntimeError("[Vehicle:RI]: BaseStation submodule not found");
    }
    sendDirect(packet, transmissionTime, SimTime::ZERO, baseStation, "wirelessGate");
    // Schedule the end transmission event after the transmission time
    scheduleAt(simTime() + transmissionTime, endTransmissionEvent);
}

void RadioInterface::finish()
{
    // Clean up the end transmission event
    cancelAndDelete(endTransmissionEvent);
}

}
