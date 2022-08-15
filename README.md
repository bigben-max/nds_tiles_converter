NDS Tiles Toolset
=================

A simple c++ implementation of the NDS Tiling Scheme along with some utility classes and functions.
Short feature list:

- Create NDSTiles from coordinates, level+nr, packedId
- Access NDS Tile properties and bounding boxes
- Convert between WGS84 and NDS coordinate formats
- Get Morton codes for NDS Coordinates
- GeoJSON output of all classes

Usage
=====

Install
-------

This is a cmake project.

```bash
mkdir build && cd build
cmake ..
make -j
```



Development
-----------

- https://nds-association.org/
- https://en.wikipedia.org/wiki/Navigation_Data_Standard
- NDS Format Specification, Version 2.5.4

* NDS Tiles: [1, §7.3.1]
* Morton codes: [1, §7.2.1]

## Reference

[nds-tiles](https://github.com/rondiplomatico/nds-tiles)

[configor](https://github.com/Nomango/configor)
