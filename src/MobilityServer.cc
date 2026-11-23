#include "MobilityServer.h"
#include "VehiclePacket_m.h"
#include "HandoverResponse_m.h"
#include <cmath>
#include <limits>

namespace _5g_connectedvehicle {

Define_Module(MobilityServer);

void MobilityServer::initialize()
{
    L = getParentModule()->par("L");
    N = getParentModule()->par("N");
}

void MobilityServer::handleMessage(cMessage *msg)
{
    VehiclePacket *pkt = check_and_cast<VehiclePacket *>(msg);
    if(pkt->getType() != HANDOVER_PACKET)
        throw cRuntimeError("[MS]: received a non-handover packet");

    // Calculate the new serving BS
    int newServingBS = closestBS(pkt->getGenTimePosition(), pkt->getCurrentBS());
    
    // Create a HandoverCommand message to send back to the vehicle
    HandoverResponse *res = new HandoverResponse("handoverResponse");
    res->setNewBaseStationId(newServingBS);
    res->setRequestTime(pkt->getGenTime());
    
    // Send the response back to the vehicle using the same gate index as the incoming packet
    send(res, "io$o", pkt->getArrivalGate()->getIndex());
    delete pkt;
}

// Helper function to find the closest BSù
// Optimized to avoid unnecessary calculations and loops
// It calculates the distance to the current BS and the next BS in both directions
// Then it checks if the closest BS can be found in the adjacent BSs or if it needs to loop through the BSs to find the closest one
int MobilityServer::closestBS(double position, int currentBS)
{
    // To find the nearest BS, we calculate the distance to the current BS and the next BS in both directions
    // We do not consider the road/bs offset, as the BSs are assumed to be evenly spaced along the highway
    double distance[3];
    distance[1] = fabs(position - (currentBS * (L / (N - 1)))); // Distance to current BS
    if(currentBS > 0)
        distance[0] = fabs(position - ((currentBS - 1) * (L / (N - 1)))); // Distance to previous BS
    else
        distance[0] = std::numeric_limits<double>::infinity(); // No previous BS, set to infinity
    if(currentBS < (N - 1))
        distance[2] = fabs(position - ((currentBS + 1) * (L / (N - 1)))); // Distance to next BS
    else
        distance[2] = std::numeric_limits<double>::infinity(); // No next BS, set to infinity

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
    int incr = (closestBSIndex == 0) ? -1 : (closestBSIndex == 2) ? 1 : 0;
    int i = currentBS + incr;
    double distanceToClosest = distance[closestBSIndex];
    while(i < N && i >= 0)
    {
        double dist = fabs(position - (i * (L / (N - 1))));
        if(dist > distanceToClosest)
            break;
        distanceToClosest = dist;
        i += incr;
    }

    // Return the closest BS found
    return i - incr; // Subtract incr to get the last valid BS index
}

}