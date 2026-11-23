#include "HandoverSystem.h"
#include "VehiclePacket_m.h"
#include "HandoverResponse_m.h"
#include "MobilitySystem.h"

namespace _5g_connectedvehicle {

Define_Module(HandoverSystem);

void HandoverSystem::initialize()
{
    H = par("H").doubleValue();
    this->handoverTimer = new cMessage("handoverTimer");
    scheduleAt(simTime() + H, handoverTimer);
    this->handoverDelaySignal = registerSignal("handoverDelay");
}

void HandoverSystem::handleMessage(cMessage *msg)
{   
    // Get the MobilitySystem module from the parent module
    MobilitySystem *mobilitySystem = check_and_cast<MobilitySystem *>(getParentModule()->getSubmodule("mobilitySystem"));
    if (mobilitySystem == nullptr) {
        throw cRuntimeError("[HandoverSystem]: MobilitySystem module not found");
    }

    if (msg == handoverTimer) {
        // Create a new HandoverPacket and send it to the RadioInterface
        VehiclePacket *handoverPacket = new VehiclePacket("HandoverPacket");
        handoverPacket->setType(PacketType::HANDOVER_PACKET);
        handoverPacket->setGenTime(simTime());
        handoverPacket->setGenTimePosition(mobilitySystem->getPosition());
        handoverPacket->setCurrentBS(mobilitySystem->getIdBS());
        send(handoverPacket, "io$o");

        // Reschedule the handover timer for the next interval
        scheduleAt(simTime() + H, handoverTimer);
        
    } else if (HandoverResponse *handoverPacket = dynamic_cast<HandoverResponse *>(msg)) {
        int BSId = handoverPacket->getNewBaseStationId();
        // Update the current base station in the MobilitySystem
        mobilitySystem->setIdBS(BSId);
        // Emit the handover delay signal
        simtime_t handoverDelay = simTime() - handoverPacket->getRequestTime();
        emit(handoverDelaySignal, handoverDelay);
        delete handoverPacket; // Clean up the received packet
    } else {
        throw cRuntimeError("[HandoverSystem]: received an unknown message type");
    }
}

void HandoverSystem::finish()
{
    cancelAndDelete(handoverTimer);
}

}