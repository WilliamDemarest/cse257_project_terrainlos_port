//
// SPDX-License-Identifier: LGPL-3.0-or-later
//


// package inet.tutorials.wireless;

import inet.environment.common.PhysicalEnvironment;


network WirelessC2 extends WirelessB2
{
    submodules:
        physicalEnvironment: PhysicalEnvironment {
            @display("p=580,425");
        }
}