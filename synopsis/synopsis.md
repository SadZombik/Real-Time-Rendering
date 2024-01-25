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

#### 2.3.1 Model and view transform

Model matrix describes model's position, orientation and scaling in world space. View matrix places 
the camera at the origin and aim it.

#### 2.3.2 Vertex shading

Computing shading equations by using point location, normal, color etc. The result (colors, vectors,
texture coordinates) are then sent to the rasterization stage to be interpolated. 

#### 2.3.3 Projection

Transforms the view volume into a unit cube (canonical view volume) i.e. transforms the models in 
normalized device coordinates.

#### 2.3.4 Clipping

Removes parts of the primitives that are outside the view volume.

#### 2.3.5 Screen mapping

The x and y coordinates od each primitive are transformed to form screen coordinates.    

### 2.4 The Raterizer Stage

Computes and sets colors for the pixels. It is divided into triangle setup, triangle transversal, 
pixel shading and merging.

#### 2.4.1 Triangle setup

Scan conversion and iterpolation of the various shading data produced by geometry stage.

#### 2.4.2 Triangle transversal

Finding which pixels are inside a triangle.

#### 2.4.3 Pixel shading

Per-pixel shading computations, texturing.

#### 2.4.4 Merging

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

#### 3.5.1 Stream Output

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
*homogeneous notation*. All translation, rotation, scaling, reflection and shearing matrices are affine. 

$$
    \begin{align*}
        \text{v} &= (v_x,\ x_y,\ v_z,\ 0)^T - \text{direction vector} \\
        \text{v} &= (v_x,\ x_y,\ v_z,\ 1)^T - \text{point}
    \end{align*}
$$

> The main characteristic of an affine matrix is that it preserves the parallelism of lines, but necessarily lengths and angles.

> Orthogonal matrix is a matrix whose inverse is its transposal.

| Notation               | Name                    | Characteristics |
| :--------------------- | :---------------------- | :---- |
| $$T(t) $$             | Translation matrix      | Moves a point affine <br> Affine. |
| $$R_x(\rho) $$        | Rotation matrix         | Rotates *ρ* radians around the x-axis <br> Orthogonal & affine. |
| $$R $$                | Rotation matrix         | Any rotation matrix. <br> Orthogonal & affine. |
| $$S(s) $$             | Scaling matrix          | Scales alongall axes according to *s*. <br> Affine. |
| $$H_{ij}(s) $$        | Shear matrix            | Shears component *i* by a factor *s*, with respect to component *j*. <br> Affine |
| $$E(h,\ p,\ r) $$     | Euler transform         | Orientation matrix given by head(yaw), pitch, roll. <br> Orthogonal & affine.|
| $$P_o(s) $$           | Orthographic projection | Parallel projects onto some plane or volume. <br> Affine. |
| $$P_p(s) $$           | Perspective projection  | Projects with perspecive onto a plane or volume |
| $$slepr(q,\ r,\ t) $$ | Slerp transform         | Creates an interpolated quarternion with respect to the quarternions *q* and *r*, and the parameter *t* |

**Table 4.1.** Summary of most of the transforms in this chapter.

### 4.1 Basic Transforms

Translation, rotation are rigid-body transforms. *Rigid-body* transform preserves the distances between transformed points and
preserves handedness (never causes left and right to swap sides). These two matrices are useful for positioning and orienting objects.

#### 4.1.1. Translation

A change form ome location toanother is represented by atranslation matrix **T**.

$$
    T(t) = T(t_x, t_y, t_z) = 
    \begin{pmatrix}
        1 & 0 & 0 & t_x \\
        0 & 1 & 0 & t_y \\
        0 & 0 & 1 & t_z \\
        0 & 0 & 0 & 1
    \end{pmatrix}.
$$

The multiplication of a point $p = (p_x, p_y, p_z, 1)$ with $T(t)$ yields a new point $p' = (p_x + t_x, p_y + t_y, p_z + t_z, 1)$
which is clearly a translation. 

The multiplication of a vector $\text{v}=(v_s, v_y, v_z, 0)$ won't affect it, because a direction vector cannot be translated.

The inverse of a translation matrix is the translation matrix with the opposite signs on each of the translation components. 
$$T^{-1}(t) = T(-t)$$

#### 4.1.2. Rotation

A rotation transform rotates a vector by a given angle around a given axis.

$$
    \begin{align*}
        R_x(\phi) &= 
        \begin{pmatrix}
            1 & 0         &  0         & 0 \\
            0 & cos\ \phi & -sin\ \phi & 0 \\
            0 & sin\ \phi &  cos\ \phi & 0 \\
            0 & 0         &  0         & 1
        \end{pmatrix},
    \\
    \\
        R_y(\phi) &= 
        \begin{pmatrix}
             cos\ \phi & 0 & sin\ \phi & 0 \\
             0         & 1 & 0         & 0 \\
            -sin\ \phi & 0 & cos\ \phi & 0 \\
             0         & 0 & 0         & 1
        \end{pmatrix},
    \\
    \\
        R_z(\phi) &= 
        \begin{pmatrix}
            cos\ \phi & -sin\ \phi & 0 & 0 \\
            sin\ \phi &  cos\ \phi & 0 & 0 \\
            0         & 0          & 1 & 0 \\
            0         & 0          & 0 & 1
        \end{pmatrix}.
    \end{align*}
$$

For every 3 x 3 rotation matrix that rotates $\phi$ radians around any axis, the trace is constant independent 
of the axis and is computed as:

$$ tr(R) = 1 + 2 cos\ \phi $$

*Trace* of matrix $tr(M)$ is the sum of the diagonal elements of square matrix:

$$ tr(M) = \sum_{i=0}^{n-1} m_{ii} $$

All rotation matrices have a determenant of one and are orthogonal.

Usage example: to rotate an object around a point $p$ we need to translate the object so that $p$ coincides with the origin which is done using $T(-p)$. Then actual rotation $R_z(\phi)$ and finally object has tobe translated back using $T(p)$. The resulting transform will be: 

$$ X = T(P)R_z(\phi)T(-p) $$

#### 4.1.3. Scaling

A scaling matrix $S(s) = S(s_x, s_y,s_x)$ scales entity with factors $s_x,\ s_y$ and $s_z$ along the x-, y-, and z-directions respectively.

$$ 
    S(s) = 
    \begin{pmatrix}
        s_x & 0   & 0    &   0 \\
        0   & s_y & 0    &   0 \\
        0   & 0   & s_z  &   0 \\
        0   & 0   & 0    &   1 
    \end{pmatrix}
$$

The scaling operation is called *uniform* if $s_x=s_y=s_z$ and *nonuniform* otherwise.

The inverse is $S^-1(s) = S(1/s_x, 1/s_y, 1/s_z)$.

The element (3,3) of scaling matrix affects w-component of homogeneous coordinate, and so scales every coordinate transformed by
matrix. To scale uniformly by a factor of $s$ the elements (0,0), (1,1), and (2,2) can be set to $s$, or the element (3,3) can be set
to $1/s$

$$ 
    S(s) = 
    \begin{pmatrix}
        s & 0 & 0 & 0 \\
        0 & s & 0 & 0 \\
        0 & 0 & s & 0 \\
        0 & 0 & 0 & 1 
    \end{pmatrix},\
    S'(s) = 
    \begin{pmatrix}
        1 & 0 & 0 & 0 \\
        0 & 1 & 0 & 0 \\
        0 & 0 & 1 & 0 \\
        0 & 0 & 0 & 1/s 
    \end{pmatrix}.
$$

A negative value on one or three components of $s$ geveis a *reflection matrix*, also called *mirror matrix*.


If scalling should be performed in other directions, a compound transform is needed. Construct a matrix $F$ that makes new coordinate system, then scale and transform back:

$$ X = FS(s)F^T $$

#### 4.1.4. Shearing
    z                        z
    ^                        ^
    |                        | s
    |_____                   |<->______
    |     |                  |  /     /
    |     |      H_xz(s) ->  | /     /
    |_____|                  |/_____/
    +----------> x           +----------> x

Figure 4.3. The effect of shearing $H_{xz}(s)$

Can be used to distort an entire scene to create psychedeliceffect or fuzzy reflections. 

There are six basic shearing matrices: $ H_{xy}(s),\ H_{xz}(s),\ H_{yx}(s),\ H_{yz}(s),\ H_{zx}(s),\ H_{zy}(s) $. The first subscript
denotes which coordinate is being changed by shear matrix, the second indicates the coordinate which does the shearing. For example:

$$
    H_{xz}(s) = 
    \begin{pmatrix}
        1 & 0 & s & 0 \\
        0 & 1 & 0 & 0 \\
        0 & 0 & 1 & 0 \\
        0 & 0 & 0 & 1 
    \end{pmatrix}.  
$$

The effect of multiplying this matrix with a point $p$ yields a point: $ (p_x + sp_z, p_y, p_z)^T $. The inverse of shearing matrix
is shearing in the opposite direction: $ H^{-1}_{ij}(s) = H_{ij}(-s) $. Shearing is volume preserving transformation, since $|H| = 1$.

#### 4.1.5. Concatenation of Transforms

Matrix operations are noncommutativity, the order in which the matrices occur matters.

$$ M_1M_2 \neq M_2M_1 $$

#### 4.1.6. Rigid-Body Transform

Rigid-body transform preserves lengths, angles and handedness of the object. Ant rigid-body matrix $X$ can be written as the concatenation of translation and rotation:

$$ 
    X = T(t)R = 
    \begin{pmatrix}
        r_{00} & r_{01} & r_{02} & t_x \\
        r_{10} & r_{11} & r_{12} & t_y \\
        r_{20} & r_{21} & r_{22} & t_z \\
        0      & 0      & 0      & 1
    \end{pmatrix}.
$$

The inverse of $X$ is computed as $ X^{-1} = (T(t)R)^{-1} = R^{-1}T(t)^{-1} = R^T T(-t) $. Thus, to compute it, the upper left 3 x 3
matrix of $R$ is transposed, the translation values of $T$ change sign and multiplication order becomes opposite.

Another way to calculate the inverse of $X$ is to consider $R$ and X in the following notation:

$$
    \bar{R} = (r,_0\ r,_1\ r,_2) = \begin{pmatrix} r_0^T \\\\ r_1^T \\\\ r_2^T \end{pmatrix},
    \\
    X =
    \begin{pmatrix}
        \Overrightarrow{\bar{R}} & t \\
        0^T & 1
    \end{pmatrix},
$$

Here, $0$ is a 3 x 1 column vector filled with zeros. The inverse:

$$
    X^-1 =
    \begin{pmatrix}
        r_0 & r_1 & r_2 & -\bar{R}^T t \\
        0 & 0 & 0 & 1
    \end{pmatrix}.
$$

#### 4.1.7. Normal Transform

A single matrix can be used to transform objects and their geometry. The same matrix can also transform tangent vectors following
along these objects. However it cannot always be used to transform the surface normal. The proper way isto use matrix's adjoint. 
The adjoint is always guaranteed to exist. Normal is not guaranteed to be of unit lenth after being transformed, so typically
needs to be normalized.

> Adjoint matrix calculation starts by defining the subdeterminant (cofactor) $ d^M_{ij} $ of an n x n
> matrix $M$ as the determinant that is obtained by deleting row $i$ and column $j$ and the taking the determinant of the resulting
> (n-1) x (n-1) matrix.
>
>   $$
>       d^M_{02} =
>       \begin{vmatrix}
>           m_{10} & m_{11} \\
>           m_{20} & m_{21}
>       \end{vmatrix}.
>   $$
> 
> For a 3 x 3 matrix, the adjoint is then 
> 
> $$
>       adj(M) = 
>       \begin{pmatrix}
>           d_{00} & -d_{10} & d_{20} \\
>           -d_{01} & d_{11} & -d_{21} \\
>           d_{02} & -d_{12} & d_{22}
>       \end{pmatrix},
>   $$
> Where we have left out the superscript $M$ ofthe subdeterminants for clarity. Note the signs and the order in which the
> subdeterminants appear. If we want to compute the adjoint $A$ of an arbitrary sized matrix $M$, then the component at 
> position $(i,j)$ > is $$ [a_{ij}] = \lbrack (-1)^{(i+j)} d^M_{ij} \rbrack. $$ <br>

...

#### 4.1.8. Computation of Inverses

