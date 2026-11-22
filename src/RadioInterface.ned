package _5g_connectedvehicle;

simple RadioInterface
{   
    parameters:
        double L @unit(m); // Highway length
        int N; // Number of BS
        double BSRoadOffset @unit(m); // Distance from the road to the base station
        double alpha @unit(s/m^2); // Proportionality constant for the propagation model
    gates:
        input in; // Gate to communicate with the BaseStation
        input applicationIn; // Gate to communicate with the Application
        inout handoverIO; // Gate to communicate with the Handover System

}
