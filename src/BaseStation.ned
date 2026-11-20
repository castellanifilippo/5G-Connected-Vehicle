package _5g_connectedvehicle;

simple BaseStation
{
    parameters:
        @display("i=device/antennatower;is=s");
    gates:
        inout msIO; // Gate to communicate with the MobilityServer
        inout asIO; // Gate to communicate with the ApplicationServer
        input wirelessGate @directIn;  // Gate to receive wireless messages from vehicles
}
