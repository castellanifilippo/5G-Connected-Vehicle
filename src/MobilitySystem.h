#ifndef ___5G_CONNECTEDVEHICLE_MOBILITYSYSTEM_H_
#define ___5G_CONNECTEDVEHICLE_MOBILITYSYSTEM_H_

#include <omnetpp.h>

using namespace omnetpp;

namespace _5g_connectedvehicle {
private:
    double speed; // Speed of the vehicle in m/s
    int idBS; // ID of the base station the vehicle is currently connected to
    double L; // Length of the highway
class MobilitySystem : public cSimpleModule
{
protected:
    virtual void initialize() override;
public:
    double getSpeed() const { return speed; }
    double getIdBS() const { return idBS; }
    void setIdBS(int newIdBS) { idBS = newIdBS; }
    // Calculate on time the position so we don't have to update it every time we move the vehicle
    // We can use the speed and the simulation time to calculate the position of the vehicle on the highway
    double getPosition();
};

}

#endif
