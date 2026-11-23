package _5g_connectedvehicle;

simple MobilitySystem
{
    parameters:
        double speed @unit(mps); // Vehicle speed
        int idBS;
        double L @unit(m);
        int N;
        @signal[subOptimal](type=long);
        @statistic[subOptimal](title="Sub-optimal association ratio";
                               interpolationmode=sample-hold;
                               record=timeavg,vector);
}
