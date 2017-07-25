# csg

A CSG (Constructive Solid Geometry) editor. Solids are rendered using implicit surfaces.
GUI is made with [ImGui](https://github.com/ocornut/imgui).

![screenshot](/screenshots/screenshot2.png?raw=true)

## Building

```
mkdir build && cd build
cmake ..
cp -r ../shaders shaders
make
```

This will create an executable named `csg` in the `build` directory.
