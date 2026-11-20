package _5g_connectedvehicle;

simple RadioInterface
{
    gates:
        input in; // Gate to communicate with the BaseStation
        input applicationIn; // Gate to communicate with the Application
        inout handoverIO; // Gate to communicate with the Handover System

}
