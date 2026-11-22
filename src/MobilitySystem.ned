package _5g_connectedvehicle;

simple MobilitySystem
{
    parameters:
        double speed @unit(mps); // Vehicle speed
        double BSRoadOffset @unit(m); // Distance from the road to the base station
        int idBS;
        double L @unit(m);
}
