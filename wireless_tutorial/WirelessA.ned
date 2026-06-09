//
// SPDX-License-Identifier: LGPL-3.0-or-later
//


// package inet.tutorials.wireless;

import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.node.contract.INetworkNode;
import inet.physicallayer.wireless.common.medium.RadioMedium;
//import inet.physicallayer.wireless.common.medium.UnitDiskRadioMedium;
//import radio_exp;
import inet.visualizer.contract.IIntegratedVisualizer;

network WirelessA2
{
    parameters:
        @display("bgb=600,600;bgg=100,1,grey95");
        @figure[title](type=label; pos=0,-1; anchor=sw; color=darkblue);

        @figure[rcvdPkText](type=indicatorText; pos=380,20; anchor=w; font=,18; textFormat="packets received: %g"; initialValue=0);
        @statistic[packetReceived](source=hostB.app[0].packetReceived; record=figure(count); targetFigure=rcvdPkText);

    submodules:
        visualizer: <default(firstAvailableOrEmpty("IntegratedCanvasVisualizer"))> like IIntegratedVisualizer if typename != "" {
            @display("p=580,125");
        }
        configurator: Ipv4NetworkConfigurator {
            @display("p=580,200");
        }
        radioMedium: RadioMedium {
            // radioMedium: UnitDiskRadioMedium {
            // radioMedium: radio_exp {
            @display("p=580,275");

        }
        terrainLOS: terrainLOS {
            @display("p=429,116");
        }
        hostA: <default("WirelessHost")> like INetworkNode {
            @display("p=50,325");
        }
        hostB: <default("WirelessHost")> like INetworkNode {
            @display("p=320,551");
        }
}
