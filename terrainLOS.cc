
#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include <vector>

#include "inet/physicallayer/wireless/common/medium/RadioMedium.h"

#include "inet/physicallayer/wireless/common/analogmodel/unitdisk/UnitDiskListening.h"
#include "inet/physicallayer/wireless/common/analogmodel/unitdisk/UnitDiskReceiverAnalogModel.h"
#include "inet/physicallayer/wireless/common/analogmodel/unitdisk/UnitDiskReceptionAnalogModel.h"

using namespace inet;
using namespace physicallayer;
using namespace std;


class terrainLOS : public cSimpleModule {
    public:
        //terrainLOS();
        bool has_los(const Coord t_pos, Coord r_pos) const;
    protected:
        virtual void initialize() override;
        // virtual bool isInCommunicationRange(const ITransmission *transmission, const Coord& startPosition, const Coord& endPosition) const override;
        // vector<vector<double>> h_map(0, vector<double>(0, 0));
        int east_width;
        int south_width;
        vector<vector<double>> h_map;
        // vector<vector<double>> view_map;
};

Define_Module(terrainLOS);

bool terrainLOS::has_los(const Coord t_pos, Coord r_pos) const {
    const double d = t_pos.distance(r_pos);
    EV_INFO << "here :)\n";
    EV_INFO << to_string(d);
    EV_INFO << "\n";

    vector<vector<double>> view_map(east_width, vector<double>(south_width, 0.0));

    // EV_INFO << to_string(h_map[50][0]);
    // EV_INFO << " ";
    // EV_INFO << to_string(h_map[50][50]);
    // EV_INFO << " ";
    // EV_INFO << to_string(h_map[99][99]);
    // EV_INFO << "\n";

    // double srcX = t_pos.x
    // double srcY = t_pos.y 
    int x0 = (int) t_pos.x;
    int y0 = (int) t_pos.y;

    double x0f = t_pos.x;
    double y0f = t_pos.y;
    EV_INFO << to_string(x0f);
    EV_INFO << "\n";
    EV_INFO << to_string(y0f);
    EV_INFO << "\n";

    // // Wang, Robinson, and White's Algorithm for finding line of sight
    // // ported from https://github.com/TerrainLOS/TerrainLOS/blob/release-2-7/java/edu/ucsc/terrainlos/TerrainLOSMedium.java
    double z0 = h_map[x0f][y0f];

    int x1;
    int y1;
    int x2;
    int y2;
    double z1;
    double z2;
    double xf;
    double yf;
    double visible = 0;
    double actual = 0;

    // copy height map to view map
    for(int x = x0 - 1; x <= x0 + 1; x++) {
        for(int y = y0 - 1; y <= y0 + 1; y++) {
            if(x < 0 || x >= east_width || y < 0 || y >= south_width) {
            continue;
            }
            view_map[x][y] = h_map[x][y];
        }
    }

    EV_INFO << "here 1\n";
    // Divide into 8 octets and 8 axes 
    // E 
    for(int x = x0 + 2; x < east_width; x++) {
        x1 = x - 1;
        z1 = view_map[x1][y0];
        xf = (double) x;
        visible = (z1 - z0)*((xf - x0f)/(xf - x0f - 1)) + z0;
        actual = h_map[x][y0];
        view_map[x][y0] = (visible > actual) ? visible : actual; // real
    }
    /* S */
    for(int y = y0 + 2; y < south_width; y++) {
        y1 = y - 1;
        z1 = view_map[x0][y1];
        yf = (double)y;
        visible = (z1 - z0)*((yf - y0f)/(yf - y0f - 1)) + z0;
        actual = h_map[x0][y];
        view_map[x0][y] = (visible > actual) ? visible : actual;
    }
    /* W */
    for(int x = x0 - 2; x >= 0; x--) {
        x1 = x + 1;
        z1 = view_map[x1][y0];
        xf = (double) x;
        visible = (z1 - z0)*((x0f - xf)/(x0f - xf - 1)) + z0;
        actual = h_map[x][y0];
        view_map[x][y0] = (visible > actual) ? visible : actual;
    }
    /* N */
    for(int y = y0 - 2; y >= 0; y--) {
        y1 = y + 1;
        z1 = view_map[x0][y1];
        yf = (double) y;
        visible = (z1 - z0)*((y0f - yf)/(y0f - yf - 1)) + z0;
        actual = h_map[x0][y];
        view_map[x0][y] = (visible > actual) ? visible : actual;
    }
    /* SE */
    for(int x = x0 + 2, y = y0 + 2; x < east_width && y < south_width;
            x++, y++) {
        x1 = x - 1;
        y1 = y - 1;
        z1 = view_map[x1][y1];
        xf = (double) x;
        visible = (z1 - z0)*((xf - x0f)/(xf - x0f - 1)) + z0;
        actual = h_map[x][y];
        view_map[x][y] = (visible > actual) ? visible : actual;
    }
    /* SW */
    for(int x = x0 - 2, y = y0 + 2; x >= 0 && y < south_width;
            x--, y++) {
        x1 = x + 1;
        y1 = y - 1;
        z1 = view_map[x1][y1];
        xf = (double) x;
        visible = (z1 - z0)*((x0f - xf)/(x0f - xf - 1)) + z0;
        actual = h_map[x][y];
        view_map[x][y] = (visible > actual) ? visible : actual;
    }
    /* NW */
    for(int x = x0 - 2, y = y0 - 2; x >= 0 && y >= 0;
            x--, y--) {
        x1 = x + 1;
        y1 = y + 1;
        z1 = view_map[x1][y1];
        xf = (double) x;
        visible = (z1 - z0)*((x0f - xf)/(x0f - xf - 1)) + z0;
        actual = h_map[x][y];
        view_map[x][y] = (visible > actual) ? visible : actual;
    }
    /* NE */
    for(int x = x0 + 2, y = y0 - 2; x < east_width && y >= 0;
            x++, y--) {
        x1 = x - 1;
        y1 = y + 1;
        z1 = view_map[x1][y1];
        xf = (double) x;
        visible = (z1 - z0)*((xf - x0f)/(xf - x0f - 1)) + z0;
        actual = h_map[x][y];
        view_map[x][y] = (visible > actual) ? visible : actual;
    }
    /* E-SE */
    for(int x = x0 + 2; x < east_width; x++) {
        for(int y = y0 + 1; ((y - y0) < (x - x0)) && (y < south_width); y++) {
            x1 = x - 1;
            y1 = y - 1;
            z1 = view_map[x1][y1];
            x2 = x - 1;
            y2 = y;
            z2 = view_map[x2][y2];
            xf = (double) x;
            yf = (double) y;
            visible = (z1 - z0)*((yf - y0f)/(xf - x0f - 1)) 
                    + (z2 - z0)*((xf - x0f - (yf - y0f))/(xf - x0f - 1)) + z0;
            actual = h_map[x][y];
            view_map[x][y] = (visible > actual) ? visible : actual;
        }
    }
    /* S-SE */
    for(int y = y0 + 2; y < south_width; y++) {
        for(int x = x0 + 1; ((x - x0) < (y - y0)) && x < east_width; x++) {
            x1 = x - 1;
            y1 = y - 1;
            z1 = view_map[x1][y1];
            x2 = x;
            y2 = y - 1;
            z2 = view_map[x2][y2];
            xf = (double) x;
            yf = (double) y;
            visible = (z1 - z0)*((xf - x0f)/(yf - y0f - 1)) 
                    + (z2 - z0)*((yf - y0f - (xf - x0f))/(yf - y0f - 1)) + z0; 
            actual = h_map[x][y];
            view_map[x][y] = (visible > actual) ? visible : actual;
        }
    }
    /* S-SW */
    for(int y = y0 + 2; y < south_width; y++) {
        for(int x = x0 - 1; ((x0 - x) < (y - y0)) && (x >= 0); x--) {
            x1 = x + 1;
            y1 = y - 1;
            z1 = view_map[x1][y1];
            x2 = x;
            y2 = y - 1;
            z2 = view_map[x2][y2];
            xf = (double) x;
            yf = (double) y;
            visible = (z1 - z0)*(-(xf - x0f)/(yf - y0f - 1)) 
                    + (z2 - z0)*((xf - x0f + yf - y0f)/(yf - y0f - 1)) + z0;
            actual = h_map[x][y];
            view_map[x][y] = (visible > actual) ? visible : actual;
        }
    }
    /* W-SW */
    for(int x = x0 - 2; x >= 0; x--) {
        for(int y = y0 + 1; ((y - y0) < (x0 - x)) && y < south_width; y++) {
            x1 = x + 1;
            y1 = y - 1;
            z1 = view_map[x1][y1];
            x2 = x + 1;
            y2 = y;
            z2 = view_map[x2][y2];
            xf = (double) x;
            yf = (double) y;
            visible = (z1 - z0)*(-(yf - y0f)/(xf - x0f + 1))
                    + (z2 - z0)*((xf - x0f + yf - y0f)/(xf - x0f + 1)) + z0;
            actual = h_map[x][y];
            view_map[x][y] = (visible > actual) ? visible : actual;
        }
    }
    /* W-NW */
    for(int x = x0 - 2; x >= 0; x--) {
        for(int y = y0 - 1; ((y0 - y) < (x0 - x)) && y >= 0; y--) {
            x1 = x + 1;
            y1 = y + 1;
            z1 = view_map[x1][y1];
            x2 = x + 1;
            y2 = y;
            z2 = view_map[x2][y2];
            xf = (double) x;
            yf = (double) y;
            visible = (z1 - z0)*((yf - y0f)/(xf - x0f + 1))
                    + (z2 - z0)*((xf - x0f - (yf - y0f))/(xf - x0f + 1)) + z0;
            actual = h_map[x][y];
            view_map[x][y] = (visible > actual) ? visible : actual;
        }
    }
    /* N-NW */
    for(int y = y0 - 2; y >= 0; y--) {
        for(int x = x0 - 1; ((x0 - x) < (y0 - y)) && (x >= 0); x--) {
            x1 = x + 1;
            y1 = y + 1;
            z1 = view_map[x1][y1];
            x2 = x;
            y2 = y + 1;
            z2 = view_map[x2][y2];
            xf = (double) x;
            yf = (double) y;
            visible = (z1 - z0)*((xf - x0f)/(yf - y0f + 1))
                    + (z2 - z0)*((yf - y0f - (xf - x0f))/(yf - y0f + 1)) + z0;
            actual = h_map[x][y];
            view_map[x][y] = (visible > actual) ? visible : actual;
        }
    }
    /* N-NE */
    for(int y = y0 - 2; y >= 0; y--) {
        for(int x = x0 + 1; (x - x0 < y0 - y) && (x < east_width); x++) {
            x1 = x - 1;
            y1 = y + 1;
            z1 = view_map[x1][y1];
            x2 = x;
            y2 = y + 1;
            z2 = view_map[x2][y2];
            xf = (double) x;
            yf = (double) y;
            visible = (z1 - z0)*(-(xf - x0f)/(yf - y0f + 1))
                    + (z2 - z0)*((xf - x0f + yf - y0f)/(yf - y0f + 1)) + z0;
            actual = h_map[x][y];
            view_map[x][y] = (visible > actual) ? visible : actual;
        }
    }
    /* E-NE */
    for(int x = x0 + 2; x < east_width; x++) {
        for(int y = y0 - 1; (y0 - y < x - x0) && (y >= 0); y--) {
            x1 = x - 1;
            y1 = y + 1;
            z1 = view_map[x1][y1];
            x2 = x - 1;
            y2 = y;
            z2 = view_map[x2][y2];
            xf = (double) x;
            yf = (double) y;
            visible = (z1 - z0)*(-(yf - y0f)/(xf - x0f - 1))
                    + (z2 - z0)*((xf - x0f + yf - y0f)/(xf - x0f - 1)) + z0;
            actual = h_map[x][y];
            view_map[x][y] = (visible > actual) ? visible : actual;
        }
    }

    // get results:
    // TODO: host in corner, to high?
    if(h_map[(int) r_pos.x][(int) r_pos.y] >= view_map[(int) r_pos.x][(int) r_pos.y]) {
        EV_INFO << "Has LOS\n";
        return true;
    } else {
        EV_INFO << to_string(view_map[(int) r_pos.x][(int) r_pos.y]);
        EV_INFO << "\nDoes not have LOS\n";
        return false;
    }

    return true;
}

// TODO: get path of dem from ned and initilize with it

vector<vector<double>> make_bowl(int east_width, int south_width) {
    vector<vector<double>> h_map(east_width, vector<double>(south_width, 0.0));
    // vector<vector<double>> view_map(east_width, vector<double>(south_width, 0.0));
    double max = 0.0;
    double min = 100.0;
    for(int i = 0; i < east_width; i++) {
        for(int j = 0; j < south_width; j++) {
            if(i > east_width/6 && i < (east_width - east_width/6) &&
                j > south_width/6 && j < (south_width - south_width/6)) {
                h_map[i][j] = min;
            } else {
              h_map[i][j] = max;
            }

        }
    }

    return h_map;
}

void terrainLOS::initialize() {
    east_width = par("east_width");
    south_width = par("south_width");
    h_map = make_bowl(east_width, south_width);
    EV << "radioexp created\n";
    // EV << this->getNedTypeAndFullPath();
}

bool UnitDiskReceiverAnalogModel::computeIsReceptionPossible(const IListening *listening, const IReception *reception, W sensitivity) const
{
    // sensitivity is ignored

    auto power = check_and_cast<const UnitDiskReceptionAnalogModel *>(reception->getAnalogModel())->getPower();

    bool los = false;

    // Get a pointer to the top-level network module
    cModule *networkModule = getSimulation()->getSystemModule();
    // Retrieve the instance name
    const string networkName = networkModule->getName();
    const string moduleName = ".terrainLOS";
    const string p = networkName + moduleName;

    const Coord t_pos = reception->getTransmission()->getStartPosition();
    const Coord r_pos = reception->getStartPosition();
    cModule* mod = getModuleByPath(p.c_str()); // TODO: change to find, handle failure
    if (mod){
        terrainLOS* terrain = dynamic_cast<terrainLOS*>(mod);
        if (terrain) {
            EV_INFO << "Checking LOS...\n";
            los = terrain->has_los(t_pos, r_pos);
        } else {
            EV_INFO << "terrainLOS module cast failed\n";
        }
    }


    return power == UnitDiskReceptionAnalogModel::POWER_RECEIVABLE && los;
}


// bool RadioMedium::isPotentialReceiver(const IRadio *radio, const ITransmission *transmission) const {
//     EV_INFO << "here :)\n";
//     const Radio *receiverRadio = dynamic_cast<const Radio *>(radio);
//     if (radioModeFilter && receiverRadio != nullptr && receiverRadio->getRadioMode() != IRadio::RADIO_MODE_RECEIVER && receiverRadio->getRadioMode() != IRadio::RADIO_MODE_TRANSCEIVER)
//         return false;
//     else if (listeningFilter && radio->getReceiver() != nullptr && !radio->getReceiver()->computeIsReceptionPossible(getListening(radio, transmission), transmission))
//         return false;
//     // TODO where is the tag?
//     else if (macAddressFilter && !matchesMacAddressFilter(radio, transmission->getPacket()))
//         return false;
//     else if (rangeFilter == RANGE_FILTER_INTERFERENCE_RANGE) {
//         EV_INFO << "here 1\n";
//         const IArrival *arrival = getArrival(radio, transmission);
//         return isInInterferenceRange(transmission, arrival->getStartPosition(), arrival->getEndPosition());
//     }
//     else if (rangeFilter == RANGE_FILTER_COMMUNICATION_RANGE) {
//         EV_INFO << "here 2\n";
//         return false; //--
//         const IArrival *arrival = getArrival(radio, transmission);
//         return isInCommunicationRange(transmission, arrival->getStartPosition(), arrival->getEndPosition());
//     }
//     else
//         EV_INFO << "here 3\n";
//         // EV_INFO << to_string(h_map[50][0]);
//         // EV_INFO << " ";
//         // EV_INFO << to_string(h_map[50][50]);
//         // EV_INFO << " ";
//         // EV_INFO << to_string(h_map[99][99]);
//         // EV_INFO << "\n";
//         const Coord r_pos = radio->getAntenna()->getMobility()->getCurrentPosition();
//         const IArrival *arrival = getArrival(radio, transmission);
//         double d = transmission->getStartPosition().distance(arrival->getEndPosition());
//         EV_INFO << to_string(d);
//         EV_INFO << "\n";
//         cModule* mod = getModuleByPath("WirelessA2.terrainLOS");
//         if(mod) {
//             EV_INFO << "found\n";
//         } else{
//             EV_INFO << "not_found\n";
//         }
//         return true;
// }

