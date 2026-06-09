//
// SPDX-License-Identifier: LGPL-3.0-or-later
//


// package inet.tutorials.wireless;

import inet.node.contract.INetworkNode;

network WirelessB2 extends WirelessA2
{
    submodules:
        hostR1: <default("WirelessHost")> like INetworkNode {
            @display("p=254,421");
        }
        hostR2: <default("WirelessHost")> like INetworkNode {
            @display("p=50,551");
        }
        hostR3: <default("WirelessHost")> like INetworkNode {
            @display("p=165,459");
        }
}
