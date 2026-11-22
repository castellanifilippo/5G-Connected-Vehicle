package _5g_connectedvehicle.simulations;

import _5g_connectedvehicle.*;

network Network_5G
{   
    parameters:
        double L @unit(m); // Highway length
        int N; // Number of BS
    submodules:
        vehicle: Vehicle;
        bs[N]: BaseStation;
        mobilityServer: MobilityServer;
        applicationServer: ApplicationServer;
    connections:
        bs[i].asOut --> { delay = 2ms; } --> applicationServer.in[i];
        bs[i].msIO <--> { delay = 2ms; } <--> mobilityServer.io[i];
}
