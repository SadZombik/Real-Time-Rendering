# Synopsis of the "Real-Time Rendering" book

## 1 Introduction

## 2 Graphics Rendering Pipeline

### 2.1 Architecture

Nonpipelined system that is then divided into *n* pipelined stages could give a speedup of a factor of *n*
by parallelizing. The slowest execution stage is the *bottleneck*, it determines the speed of the entire
application.

### 2.2 The Application Stage

The part of the application that executes on CPU and over which a programmer has full control. The most
important task is feeding rendering primitives to the geometry stage. Also includes collision detection,
user input, animations etc.

### 2.3 The Geometry Stage

Per-polygon and per-vertex operations. It is divided into model and view transform, vertex shadeing,
projection, clipping and screen mapping. 

### 2.3.1 Model and view transform

Model matrix describes model's position, orientation and scalling in world space. View matrix places 
the camera at the origin and aim it.

### 2.3.2 Vertex shading

Computing shading equations by using point location, normal, color etc. The result (colors, vectors,
texture coordinates) are then sent to the rasterization stage to be interpolated. 

### 2.3.3 Projection

Transforms the view volume into a unit cube (canonical view volume) i.e. transforms the models in 
normalized device coordinates.

### 2.3.4 Clipping

Removes parts of the primitives that are outside the view volume.

### 2.3.5 Screen mapping

The x and y coordinates od each primitive are transformed to form screen coordinates.    

### 2.4 The Raterizer Stage

Computes and sets colors for the pixels. It is divided into triangle setup, triangle transversal, 
pixel shading and merging.

### 2.4.1 Triangle setup

Scan conversion and iterpolation of the various shading data produced by geometry stage.

### 2.4.2 Triangle transversal

Finding which pixels are inside a triangle.

### 2.4.3 Pixel shading

Per-pixel shading computations, texturing.

### 2.4.4 Merging

Handling color buffer, Z-buffer, alpha channel, stencil buffer, double buffering.

## 3 Graphics Processing Unit (GPU)

### 3.1 GPU Pipeline Overview

GPU implementation of the rendering pipeline: vertex shader -> geometry shader -> clipping -> screen
mapping -> triangle setup -> triangle transversal -> pixel(fragment) shader -> merger. 
Some of them are fully programmable, others configurable and the rest are completely fixed.

### 3.2 The Programmable Shader Stage

Each programmable shader stage has two types of inputs: *uniform* (contant during drawcall) and 
*varying* inputs. A compiled shader is stored as string, which is passed tothe GPU via the driver.

```
                                4096 registers
                                 _____________
                                |             |
                                |  Temporary  |
                                |  Registers  |
                                |_____________|
                                       ^
16/16/32 registers                     |                    16/32/8 registers
 _________________        _____________v____________        _________________ 
|                 |      |                          |      |                 |
|  Varying Input  |----->|          Shader          |----->|     Output      |
|    Registers    |      |      Virtual Machine     |      |    Registers    |
|_________________|      |__________________________|      |_________________|
                            ^                    ^   
                            |                    |      
                    ________|________    ________|________ 
                   |                 |  |                 |
                   |    Constant     |  |    Textures     |
                   |    Registers    |  |                 |
                   |_________________|  |_________________|
                      16 buffers of        128 arrays of
                      4096 registers       512 textures

```
Figure 3.2. Common-shader core VM architecture and register layout, under DirectX 10.

### 3.3 The Evolution of Programmable Shading

```

```

### 3.4 The Vertex Shader

Some data manipulations happens before vertex shader by *input assembler*, it prepares sets of vertices,
primitives, colors and arrays for instancing, meshes etc.

In general, vertex shader provides a way to modify, create or ignore values associated with each polygon's
vertex, such as its color, normal, texture coordinates and position.

Other uses for the vertex shader include :

* Lens effect - screen appears fish-eyed, underwater or otherwise distorted.
* Mesh deformations.
* Object twist, bend and taper operations.
* Procedural deformations (flags, cloth, water...).
* Primitive creation.
* Page curls, heat haze, water ripples and other effects.
* Vertex texture fetch.

The output can be consumed in various ways.

### 3.5 The Geometry Shader

Located immediately after the vertex shader and itsuse is optional. Main usage is modifying incoming data,
making copies, creating various sized particles, finding object edges for shadow alghoritms.

The input is a single object (triangle, line segment or just a point) and its associated vertices.
In addition, the geometry shader can process extended primitives, additional vertices outside a triangle
and two adjacent vertices on a polyline. The output is inthe form of points, polylines and 
triangle strips or nothing.

After vertex and geometry shaders perform their operations, the primitive is clipped and set up for rasterization.

### 3.5.1 Stream Output

The data always passed throught the pipeline andintermediate results could not be accessed, 
but rasterization stage could be turned off entirely. In this case processed data can be 
sent back throught the pipeline for *iterative processing*. It is usefull for simulating 
water and other particle effects.

### 3.6 The Pixel Shader

Typically sets the fragment color and some operations such as fog, alpha testing. 

### 3.7 The Merging Stage

Same as [2.4.4](#244-merging)

### 3.8 Effects

Effects languages, such as HLSL FX, CgFX and others were invented to set states variables of particular configurations.

## 4 Transforms

A *tranform* is an operation that takes entities(pointss, vectors or colors) and converts them in some way to position,
reshape and animate objects, lights and cameras.

A *linear transform* is one that preserves vector addition and scalar multiplication.

$$
    \begin{align*}
        f(x) + f(y) &= f(x + y) \\
        kf(x) &= f(kx)
    \end{align*}
$$

Combining linear transforms and translations canbe done using an *affine transform* storead as 4x4 matrix.
An affine transform is one that performs a linear transform and then translation. To represent 4-element vector we use
*homogeneous notation*. All translation, rotation, scalling, reflection and shearing matrices are affine. 

$$
    \begin{align*}
        \text{v} &= (v_x,\ x_y,\ v_z,\ 0)^T - \text{direction vector} \\
        \text{v} &= (v_x,\ x_y,\ v_z,\ 1)^T - \text{point}
    \end{align*}
$$

> [!IMPORTANT]
> The main characteristic of an affine matrix is that it preserves the parallelism of lines, but necessarily lengths and angles.

> [!IMPORTANT]
> Orthogonal matrix is a matrix whose inverse is its transposal.

| Notation               | Name                    | Characteristics |
| :--------------------- | :---------------------- | :---- |
| $$T(t) $$              | Translation matrix      | Moves a point affine <br> Affine. |
| $$ R_x(\rho) $$        | Rotation matrix         | Rotates *ρ* radians around the x-axis <br> Orthogonal & affine. |
| $$ R $$                | Rotation matrix         | Any rotation matrix. <br> Orthogonal & affine. |
| $$ S(s) $$             | Scalling matrix         | Scales alongall axes according to *s*. <br> Affine. |
| $$ H_{ij}(s) $$        | Shear matrix            | Shears component *i* by a factor *s*, with respect to component *j*. <br> Affine |
| $$ E(h,\ p,\ r) $$     | Euler transform         | Orientation matrix given by head(yaw), pitch, roll. <br> Orthogonal & affine.|
| $$ P_o(s) $$           | Orthographic projection | Parallel projects onto some plane or volume. <br> Affine. |
| $$ P_p(s) $$           | Perspective projection  | Projects with perspecive onto a plane or volume |
| $$ slepr(q,\ r,\ t) $$ | Slerp transform         | Creates an interpolated quarternion with respect to the quarternions *q* and *r*, and the parameter *t* |

**Table 4.1.** Summary of most of the transforms in this chapter.

### 4.1 Basic Transforms

### 4.1.1. Translation

### 4.1.2. Rotation

### 4.1.3. Scalling

### 4.1.4. Shearing

### 4.1.5. Concatenation of Transforms

### 4.1.6. Rigid-Body Transform

### 4.1.7. Normal Transform

### 4.1.8. Computation of Inverses
