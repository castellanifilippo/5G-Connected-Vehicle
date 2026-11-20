#ifndef ___5G_CONNECTEDVEHICLE_MOBILITYSYSTEM_H_
#define ___5G_CONNECTEDVEHICLE_MOBILITYSYSTEM_H_

#include <omnetpp.h>

using namespace omnetpp;

namespace _5g_connectedvehicle {
private:
    double speed; // Speed of the vehicle in m/s
    int idBS; // ID of the base station the vehicle is currently connected to
class MobilitySystem : public cSimpleModule
{
protected:
    virtual void initialize() override;
public:
    double getSpeed() const { return speed; }
    double setSpeed(double newSpeed) { speed = newSpeed; }
};

}

#endif
