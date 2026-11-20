package _5g_connectedvehicle;

module Vehicle
{
    gates:
        input wirelessGate;
    submodules:
        radioInterface: RadioInterface {
            @display("p=100,100");
        }
    connections:
        radioInterface.antennaIO <--> wirelessGate;
}
