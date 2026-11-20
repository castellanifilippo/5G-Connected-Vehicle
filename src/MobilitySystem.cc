#include "MobilitySystem.h"

namespace _5g_connectedvehicle {

Define_Module(MobilitySystem);

void MobilitySystem::initialize()
{
    speed = par("speed").doubleValue(); 
    idBS = par("idBS").intValue();
}

}
