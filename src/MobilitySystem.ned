package _5g_connectedvehicle;

simple MobilitySystem
{
    parameters:
        double speed @unit(mps); // Vehicle speed
        int idBS;
        double L @unit(m);
        int N;
        bool idealHandover = default(false); // If true, the radio always use the nearest bs
        
        @signal[subOptimal](type=long);
        @statistic[subOptimal](title="Sub-optimal association ratio";
                               interpolationmode=sample-hold;
                               record=timeavg,vector);
}
