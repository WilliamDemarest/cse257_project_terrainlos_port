//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

// package inet.physicallayer.wireless.common.medium;
import inet.physicallayer.wireless.common.medium.RadioMedium;

//
// This radio medium model provides a very simple but fast and predictable
// physical layer behavior. The idea behind this analog model is to describe
// transmissions with a few distance-based parameters: communication range,
// interference range, and detection range. Whether the reception is successful
// or not depends on the distance between the transmitter and the receiver.
//
// @see UnitDiskMediumAnalogModel, UnitDiskTransmitterAnalogModel, UnitDiskReceiverAnalogModel.
// @see DimensionalRadioMedium, ScalarRadioMedium
//
//module radio_exp extends RadioMedium
simple terrainLOS
{

    parameters:
        bool enabled = default(true);
        bool use_height_map = default(true);
        int priority = default(0);
        int east_width = default(600);
        int south_width = default(600);
        int radio_height = default(0);
        int east_offset = default(0);
        int south_offset = default(0);

}
