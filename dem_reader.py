from pathlib import Path
from dted import Tile
from geopy import distance
import numpy as np
import cv2

# Get DEM file
dted_file = Path("colorado_test_1arc_v3.dt2")
tile = Tile(dted_file)

# Lots of print statements to make sure orentation is correct and for debugging 
print(tile.dsi.north_west_corner)
print(tile.dsi.south_east_corner)
nw_height = tile.get_elevation(tile.dsi.north_west_corner)
print(f"elevation at NW corner: {nw_height}")
sw_height = tile.get_elevation(tile.dsi.south_west_corner)
print(f"elevation at SW corner: {sw_height}")
ne_height = tile.get_elevation(tile.dsi.north_east_corner)
print(f"elevation at NE corner: {ne_height}")
se_height = tile.get_elevation(tile.dsi.south_east_corner)
print(f"elevation at SE corner: {se_height}")

print(tile.data[0])
nw = (tile.dsi.north_west_corner.latitude, tile.dsi.north_west_corner.longitude)
ne = (tile.dsi.north_east_corner.latitude, tile.dsi.north_east_corner.longitude)
sw = (tile.dsi.south_west_corner.latitude, tile.dsi.south_west_corner.longitude)
se = (tile.dsi.south_east_corner.latitude, tile.dsi.south_east_corner.longitude)

dem_h = distance.geodesic(nw, sw).m
dem_w_n = distance.geodesic(nw, ne).m
# desired dimensions of result map (starting from north west corner)
# TODO: add offset options
array_h = 1000
array_w = 1000

ratio = dem_h / len(tile.data[0]) # meters / DEM unit ratio for verticle
dem_entries = len(tile.data) # currently assuming square map
print(f"meter ratio being used: {ratio}")
print(f"lat meter ratio at top: {dem_w_n / len(tile.data)}")
print(f"lat meter ratio at bottom {distance.geodesic(sw, se).m / len(tile.data)}")

# find DEM matrix cords from meter map cords
def get_bucket_distorted(m_x, m_y): 
    # return (int(m_x / ratio), dem_entries - int(m_y / ratio) - 1)
    # return (int(m_x / ratio), int(m_y / ratio))
    return (dem_entries - int(m_x / ratio) - 1, int(m_y / ratio)) # TODO: sperate ratio for lat and long

# get height at x and y meters from data DEM matrix
# (0, 0) shifted to north west corner
def height_at_meters(x, y, data):
    bucket = get_bucket_distorted(x, y)
    # return data[bucket[0]][bucket[1]]
    return data[bucket[1]][bucket[0]]

# build the meter scale map matrix
def build_array(array_h, array_w, data):
    array = []
    for x in range(array_w):
        array.append([])
        for y in range(array_h):
            array[x].append(height_at_meters(x, y, data))
    return array

# make greyscale png representation of map matrix
def make_png(array):
    np_array = np.array(array)
    low = np.min(np_array)
    high = np.max(np_array)

    # some information about the terrain
    print(f"average: {np.mean(array)}")
    print(f"lowest: {low}m")
    print(f"highest: {high}m")

    # previous way of getting highest and lowest point fast (downscale, data is lost)
    # TODO: check if this is faster than numpy for large matrix
    # low = 99999
    # high = 0
    scale = 20
    # col_num = 0
    # v_num = 0
    # for col in array:
    #     if (col_num % scale == 0):
    #         for v in col:
    #             if (v_num % scale == 0):
    #                 if v < low:
    #                     low = v 
    #                 if v > high:
    #                     high = v
    #             v_num += 1
    #     col_num += 1

    # make png matrix. downscales to go faster
    png_array = []
    col_num = 0
    v_num = 0
    for col in array:
        if (col_num % scale == 0):
            png_array.append([])
            for v in col:
                if (v_num % scale == 0):
                    color_v = (v - low) / ((high - low) / 255)
                    png_array[int(col_num / scale)].append([color_v, color_v, color_v])
                v_num += 1
        col_num += 1

    cv2.imwrite("map.png", np.array(png_array))

# build array
array = build_array(array_h, array_w, tile.data)
make_png(array) # make png (optional)
print(array[0][0]) # confirm array (0,0) is same as DEM northwest corner (printed earlier)

# build string for c++ header file
# matrix is represented with c++ vectors 
string = "#include <stdio.h>\n#include <vector>\nstd::vector<std::vector<double>> height_map = "
string += "{ "
for col in array:
    string += "{ "
    string += ", ".join(map(str, col))

    string += "}, "
string += "};\n"

# write string to file
with open("height_map.h", "w") as f:
    f.write(string)
