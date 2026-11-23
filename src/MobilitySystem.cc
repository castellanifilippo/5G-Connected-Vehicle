#include "MobilitySystem.h"
#include <cmath>

namespace _5g_connectedvehicle {

Define_Module(MobilitySystem);

void MobilitySystem::initialize()
{
    speed = par("speed").doubleValue(); 
    idBS = par("idBS").intValue();
    L = par("L").doubleValue();
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
}
