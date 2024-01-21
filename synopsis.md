# Synopsis of the "Real-Time Rendering" book

## 1 Introduction

## 2 Graphics Rendering Pipeline

### 2.1 Architecture

Nonpipelined system that is then divided into *n* pipelined stages could give a speedup of a factor of *n* by parallelizing. The slowest execution stage is the *bottleneck*, it determines the speed of the entire application.

### 2.2 The Application Stage

The part of the application that executes on CPU and over which a programmer has full control. The most important task is feeding rendering primitives to the geometry stage. Also includes collision detection, user input, animations etc.

### 2.3 The Geometry Stage

Per-polygon and per-vertex operations. It is divided into model and view transform, vertex shadeing, projection, clipping and screen mapping. 

- 2.3.1 Model and view transform

    Model matrix describes model's position, orientation and scalling in world space. View matrix places the camera at the origin and aim it.

- 2.3.2 Vertex shading

    Computing shading equations by using point location, normal, color etc. The result (colors, vectors, texture coordinates) are then sent to the rasterization stage to be interpolated. 

- 2.3.3 Projection

    Transforms the view volume into a unit cube (canonical view volume) i.e. transforms the models in normalized device coordinates.

- 2.3.4 Clipping

    Removes parts of the primitives that are outside the view volume.

- 2.3.5 Screen mapping

    The x and y coordinates od each primitive are transformed to form screen coordinates.    

### 2.4 The Raterizer Stage

Computes and sets colors for the pixels. It is divided into triangle setup, triangle transversal, pixel shading and merging.

- 2.4.1 Triangle setup

    Scan conversion and iterpolation of the various shading data produced by geometry stage.

- 2.4.2 Triangle transversal

    Finding which pixels are inside a triangle.

- 2.4.3 Pixel shading

    Per-pixel shading computations, texturing.

- 2.4.4 Merging

    Handling color buffer, Z-buffer, alpha channel, stencil buffer, double buffering.

## 3 Graphics Processing Unit (GPU)

### 3.1 GPU Pipeline Overview


