#include "MobilityServer.h"

namespace _5g_connectedvehicle {

Define_Module(MobilityServer);

void MobilityServer::initialize()
{
    L = getParentModule()->par("highway-length");
    N = getParentModule()->par("BS-number");
}

void MobilityServer::handleMessage(cMessage *msg)
{
    VehiclePacket *pkt = check_and_cast<VehiclePacket *>(msg);
    if(pkt->getType() != HANDOVER_PACKET)
        throw cRuntimeError("[MS]: received a non-handover packet");

    // Calculate the new serving BS
    int newServingBS = closestBS(pkt->getReportedPosition(), pkt->getCurrentBS());
    
    // Create a HandoverCommand message to send back to the vehicle
    HandoverResponse *res = new HandoverResponse("handoverResponse");
    res->setNewServingBS(newServingBS);
    res->setGenTime(pkt->getGenTime());
    // Send the response back to the vehicle
    send(res, "out");

    delete pkt;
}

// Helper function to find the closest BSù
// Optimized to avoid unnecessary calculations and loops
// It calculates the distance to the current BS and the next BS in both directions
// Then it checks if the closest BS can be found in the adjacent BSs or if it needs to loop through the BSs to find the closest one
int MobilityServer::closestBS(double position, int currentBS)
{
    // To find the nearest BS, we calculate the distance to the current BS and the next BS in both directions
    int distance[3];
    distance[0] = abs(position - ((currentBS - 1) * (L / N))); // Distance to previous BS
    distance[1] = abs(position - (currentBS * (L / N))); // Distance to current BS
    distance[2] = abs(position - ((currentBS +1) * (L / N))); // Distance to next BS

    // Find the index of the minimum distance
    int closestBSIndex = 0;
    for(int i = 1; i < 3; i++)
    {
        if(distance[i] < distance[closestBSIndex])
            closestBSIndex = i;
    }

    //if the closest BS is the current one, we return it directly
    if(closestBSIndex == 1)
        return currentBS;

    // We check the case where the closet BS is not the adjacent one
    // The above calculation gave us the direction to lock for
    // We loop the BS till we find the min distance
    int incr = (i == 0) ? -1 : (i == 2) ? 1 : 0;
    int i = currentBS + incr;
    int distanceToClosest = distance[closestBSIndex];
    while(i < N && i >= 0)
    {
        double dist = abs(position - (i * (L / N)));
        if(dist > distanceToClosest)
            break;
        distanceToClosest = dist;
        i += incr;
    }

    // Return the closest BS found
    return i;
}
