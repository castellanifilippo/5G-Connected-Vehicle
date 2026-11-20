#include "BaseStation.h"

namespace _5g_connectedvehicle {

Define_Module(BaseStation);

void BaseStation::handleMessage(cMessage *msg)
{
    // Check if we receved a VehiclePacket or a HandoverResponse message
    if(VehiclePacket *pkt = dynamic_cast<VehiclePacket *>(msg)) {
        if (pkt->getType() == APP_PACKET) {
            // Forward application packets to the Application Server
            send(pkt, "asIO");
        } else if (pkt->getType() == HANDOVER_PACKET) {
            // Forward handover packets to the Mobility Server
            send(pkt, "msIO");
        } else {
            throw cRuntimeError("[BS]: received an unknown packet type");
        }
    } else if(HandoverResponse *res = dynamic_cast<HandoverResponse *>(msg)) {
        // Forward handover response packets to the vehicle with a 1ms delay
        cModule *targetVehicle = getParentModule()->getSubmodule("vehicle", res->getTargetVehicleId());
        sendDirect(res, SimTime(1, SIMTIME_MS), SimTime::ZERO, targetVehicle, "wirelessGate");
    } else {
        throw cRuntimeError("[BS]: received a non-VehiclePacket message");
    }
}

}
