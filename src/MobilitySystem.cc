#include "MobilitySystem.h"
#include <cmath>

namespace _5g_connectedvehicle {

Define_Module(MobilitySystem);

void MobilitySystem::initialize()
{
    speed = par("speed").doubleValue(); 
    idBS = par("idBS").intValue();
    L = par("L").doubleValue();
    N = par("N").intValue();
    isd = L / (N - 1); // Calculate the inter-site distance
    optimalBS = par("idBS").intValue();
    if(speed > 0) {
        direction = 1; // Forward
    } else {
        direction = -1; // Backward
    }
    
    subOptimalSignal = registerSignal("subOptimal");
    emit(subOptimalSignal, 0L);

    boundaryEvent = new cMessage("boundaryEvent");
    // Schedule the first boundary event based on the vehicle's speed
    if(speed > 0 && N > 1) {
        scheduleAt(simTime() + ((0.5 * isd) / std::abs(speed)), boundaryEvent);
    }
}

// Function that return the current position of the vehicle based on its speed and the simulation time
double MobilitySystem::getPosition()
{
    simtime_t currentTime = simTime();
    double totalDistance = speed * currentTime.dbl();
    // use fmod to ensure the position wraps around the highway length
    double position = fmod(totalDistance, 2 * L);
    // Reflect the position if it exceeds the highway length
    if(position > L) {
        position = 2 * L - position;
    }
    return position;
}

void MobilitySystem::setIdBS(int newIdBS)
{
    idBS = newIdBS;
    // Emit the sub-optimal signal whenever the base station ID changes
    emit(subOptimalSignal, (idBS != optimalBS) ? 1L : 0L);
}

void MobilitySystem::handleMessage(cMessage *msg)
{
    if(msg == boundaryEvent) {
        optimalBS += direction; // Move to the next base station in the direction of travel
        if(optimalBS == N-1){
            direction = -1; // Change direction to backward
        } else if(optimalBS == 0){
            direction = 1; // Change direction to forward
        }
        // emit signal to indicate non optimal situation
        emit(subOptimalSignal, (idBS != optimalBS) ? 1L : 0L);
        scheduleAt(simTime() + (isd / speed), boundaryEvent);
    } else {
        throw cRuntimeError("[Vehicle:MS]: received an unknown message type");
    }
}
}