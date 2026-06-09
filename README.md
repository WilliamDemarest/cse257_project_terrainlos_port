Port of TerrainLOS to OMNET++ for UCSC CSE257 project spring 2026.  
Original TerrainLOS for COOJA/Contiki: https://github.com/TerrainLOS  

TerrainLOS implemented as an OMNET++ module that must be included in the simulation. c++ files contains an override of the INET UnitDiskReceiverAnalogModel::computeIsReceptionPossible() function to implement TerrainLOS behavior.
  
Folders:  
- **manet-showcase-exp:** OMNET++ simulation modified from "https://inet.omnetpp.org/docs/showcases/routing/manet/doc/", set up to run the AODV simulation with TerrainLOS added. Includes a copy of the TerrrainLOS .cc and .ned files, but will need a terrain map header to be run.  
- **wireless_tutorial:** OMNET++ simulation modified from "https://inet.omnetpp.org/docs/tutorials/wireless/doc/index.html". Used to confirm basic functionality. Meant to be run with automatically generated terrain. **NOTE: The TerrainLOS module .ned and .cc included are unfinished versions.** This was from before I added height map header files. I wanted to include it as is. It should work with the current version if a header with an empty height map is included and "use_height_map" is set to false.  
  
Files:  
- **dem_reader.py:** Process DEM files to generate height map header files. Designed to be used with USGS SRTM DEM files.  
- **terrainLOS.cc:** Implements terrainLOS module behavior. Includes overriden version of "UnitDiskReceiverAnalogModel::computeIsReceptionPossible()" INET function.  
- **terrainLOS.ned:** .ned file for terrainLOS module. Defines parameters.  

