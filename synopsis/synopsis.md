# **Synopsis of the "Real-Time Rendering" book**

# 2 Graphics Rendering Pipeline

## 2.1 Architecture

Nonpipelined system that is then divided into *n* pipelined stages could give a speedup of a factor of *n*
by parallelizing. The slowest execution stage is the *bottleneck*, it determines the speed of the entire
application.

## 2.2 The Application Stage

The part of the application that executes on CPU and over which a programmer has full control. The most
important task is feeding rendering primitives to the geometry stage. Also includes collision detection,
user input, animations etc.

## 2.3 The Geometry Stage

Per-polygon and per-vertex operations. It is divided into model and view transform, vertex shadeing,
projection, clipping and screen mapping. 

### 2.3.1 Model and view transform

Model matrix describes model's position, orientation and scaling in world space. View matrix places 
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

## 2.4 The Raterizer Stage

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

# 3 Graphics Processing Unit (GPU)

## 3.1 GPU Pipeline Overview

GPU implementation of the rendering pipeline: vertex shader -> geometry shader -> clipping -> screen
mapping -> triangle setup -> triangle transversal -> pixel(fragment) shader -> merger. 
Some of them are fully programmable, others configurable and the rest are completely fixed.

## 3.2 The Programmable Shader Stage

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

## 3.3 The Evolution of Programmable Shading

```

```

## 3.4 The Vertex Shader

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

## 3.5 The Geometry Shader

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

## 3.6 The Pixel Shader

Typically sets the fragment color and some operations such as fog, alpha testing. 

## 3.7 The Merging Stage

Same as [2.4.4](#244-merging)

## 3.8 Effects

Effects languages, such as HLSL FX, CgFX and others were invented to set states variables of particular configurations.

# 4 Transforms

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

| Notation              | Name                    | Characteristics |
| :-------------------- | :---------------------- | :---- |
| $$T(t)$$              | Translation matrix      | Moves a point affine <br> Affine. |
| $$R_x(\rho)$$         | Rotation matrix         | Rotates *ρ* radians around the x-axis <br> Orthogonal & affine. |
| $$R$$                 | Rotation matrix         | Any rotation matrix. <br> Orthogonal & affine. |
| $$S(s)$$              | Scaling matrix          | Scales alongall axes according to *s*. <br> Affine. |
| $$H_{ij}(s)$$         | Shear matrix            | Shears component *i* by a factor *s*, with respect to component *j*. <br> Affine |
| $$E(h,\ p,\ r)$$      | Euler transform         | Orientation matrix given by head(yaw), pitch, roll. <br> Orthogonal & affine.|
| $$P_o(s)$$            | Orthographic projection | Parallel projects onto some plane or volume. <br> Affine. |
| $$P_p(s)$$            | Perspective projection  | Projects with perspecive onto a plane or volume |
| $$slepr(q,\ r,\ t)$$  | Slerp transform         | Creates an interpolated quarternion with respect to the quarternions *q* and *r*, and the parameter *t* |

**Table 4.1** Summary of most of the transforms in this chapter.

## 4.1 Basic Transforms

Translation, rotation are rigid-body transforms. *Rigid-body* transform preserves the distances between transformed points and
preserves handedness (never causes left and right to swap sides). These two matrices are useful for positioning and orienting objects.

### 4.1.1 Translation

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

### 4.1.2 Rotation

A rotation transform rotates a vector by a given angle around a given axis.

$$
    \begin{align*}
        R_x(\phi) &= 
        \begin{pmatrix}
            1 & 0         &  0         & 0 \\
            0 & \cos \phi & -\sin \phi & 0 \\
            0 & \sin \phi &  \cos \phi & 0 \\
            0 & 0         &  0         & 1
        \end{pmatrix},
    \\
    \\
        R_y(\phi) &= 
        \begin{pmatrix}
             \cos \phi & 0 & \sin \phi & 0 \\
             0         & 1 & 0         & 0 \\
            -\sin \phi & 0 & \cos \phi & 0 \\
             0         & 0 & 0         & 1
        \end{pmatrix},
    \\
    \\
        R_z(\phi) &= 
        \begin{pmatrix}
            \cos \phi & -\sin \phi & 0 & 0 \\
            \sin \phi &  \cos \phi & 0 & 0 \\
            0         & 0          & 1 & 0 \\
            0         & 0          & 0 & 1
        \end{pmatrix}.
    \end{align*}
$$

For every 3 x 3 rotation matrix that rotates $\phi$ radians around any axis, the trace is constant independent 
of the axis and is computed as:

$$ tr(R) = 1 + 2 \cos \phi $$

*Trace* of matrix $tr(M)$ is the sum of the diagonal elements of square matrix:

$$ tr(M) = \sum_{i=0}^{n-1} m_{ii} $$

All rotation matrices have a determenant of one and are orthogonal. To rotate in te opposite direction around the same axis the
inverse can be used:

$$ R_i^{-1} (\phi) = R_i(-\phi) $$

Usage example: to rotate an object around a point $p$ we need to translate the object so that $p$ coincides with the origin which is done using $T(-p)$. Then actual rotation $R_z(\phi)$ and finally object has tobe translated back using $T(p)$. The resulting transform will be: 

$$ X = T(P)R_z(\phi)T(-p) $$

### 4.1.3 Scaling

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

### 4.1.4 Shearing
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

There are six basic shearing matrices: $H_{xy}(s),\ H_{xz}(s),\ H_{yx}(s),\ H_{yz}(s),\ H_{zx}(s),\ H_{zy}(s)$. The first subscript
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

The effect of multiplying this matrix with a point $p$ yields a point: $(p_x + sp_z, p_y, p_z)^T$. The inverse of shearing matrix
is shearing in the opposite direction: $H_{ij}^{-1}(s) = H_{ij}(-s)$. Shearing is volume preserving transformation, since $|H| = 1$.

### 4.1.5 Concatenation of Transforms

Matrix operations are noncommutativity, the order in which the matrices occur matters.

$$ M_1M_2 \neq M_2M_1 $$

### 4.1.6 Rigid-Body Transform

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

The inverse of $X$ is computed as $X^{-1} = (T(t)R)^{-1} = R^{-1}T(t)^{-1} = R^T T(-t)$. Thus, to compute it, the upper left 3 x 3
matrix of $R$ is transposed, the translation values of $T$ change sign and multiplication order becomes opposite.

Another way to calculate the inverse of $X$ is to consider $R$ and X in the following notation:

$$
    \bar{R} = (r,_0\ r,_1\ r,_2) = \begin{pmatrix} r_0^T \\\\ r_1^T \\\\ r_2^T \end{pmatrix},
    \\
    X =
    \begin{pmatrix}
        \overrightarrow{\bar{R}} & t \\
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

### 4.1.7 Normal Transform

A single matrix can be used to transform objects and their geometry. The same matrix can also transform tangent vectors following
along these objects. However it cannot always be used to transform the surface normal. The proper way isto use matrix's adjoint. 
The adjoint is always guaranteed to exist. Normal is not guaranteed to be of unit lenth after being transformed, so typically
needs to be normalized.

> Adjoint matrix calculation starts by defining the subdeterminant (cofactor) $d^M_{ij}$ of an n x n
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

The traditional way to transform the normal is computing the transpose of the inverse. 

### 4.1.8 Computation of Inverses

Since inverse computation is expensive, the purpose of it should be taken into account when optimizing. For example,
if the inverse is to be used for transforming vectors, then only 3 x 3 upper left part needs to be inverted.

## 4.2 Special Matrix Transforms and Operations

### 4.2.1 The Euler Transform

This transform is an intuitive way to construct a matrix to orient an object in a ceratin direction. The Euler transform is
the multiplication of three matrices, denoted as $E$ and given by:

$$ E(h,p,r) = R_z(r) R_x(p) R_y(h) $$

The Euler angles $h, p$ and $r$ represent in which order and how much the head, pitch and roll should rotate around their 
respective axes.

This method has a lot of limitations and issues, so it is better using quaternions.

### 4.2.2 Extracting Parameters from the Euler Transform

It is useful to have a procedure that extracts the Euler parameters from an orthogonal matrix:

$$
    F =
    \begin{pmatrix}
        f_{00} & f_{01} & f_{02} \\
        f_{10} & f_{11} & f_{12} \\
        f_{20} & f_{21} & f_{22}
    \end{pmatrix} = R_z(r) R_x(p) R_y(h) = E(h,p,r).
$$

Contatenating the three rotation matricex yields:

$$
    F =
    \begin{pmatrix}
        \cos r\cos h - \sin r\sin p\sin h & -\sin r\cos p & \cos r\sin h+\sin r\sin p\cos h \\
        \sin r\cos h + \cos r\sin p\sin h & \cos r\cos p & \sin r\sin h-\cos r\sin p\cos h \\
        -\cos p\sin h & \sin p & \cos p\cos h
    \end{pmatrix}.
$$
$$ \frac{f_{01}}{f_{11}} = \frac{-\sin r}{\cos r} = -\tan r, $$
$$ \frac{f_{20}}{f_{22}} = \frac{-\sin h}{\cos h} = -\tan h. $$

Thus, the Euler parameters are extracted from a matrix $F$ isung *atan2(y, x)*:

$$ 
    \begin{align*}
    h &= atan2(f_{20}, f_{22}), \\
    p &= arcsin(f_{21}), \\
    r &= atan2(-f_{01}, f_{11}).
    \end{align*}
$$

However, there is a special case when $\cos p = 0$, because then $f_{01} = f_{11} = 0$, and so the *atan2* function cannot be used.
Having $\cos p = 0$ implies that $\sin p = \pm 1$ and so $F$ simplifies to:

$$
    F =
    \begin{pmatrix}
        \cos(r \pm h) & 0     & \sin(r \pm h)  \\
        \sin(r \pm h) & 0     & -\cos(r \pm h) \\
        0             & \pm 1 & 0
    \end{pmatrix}.
$$

### 4.2.3 Matrix Decomposition

The task of retrieving various transforms froma a concatenated matrix is called *matrix decomposition*. There are many reasons to
retrieve a set if transformations:

* Extracting just the scaling factors for an object.
* Finding transforms needed for a particular system.
* Determining whether a model has undergone only rigid-body transforms.
* Interpolating between keyframes in an animation where only the matrix for the object is available.
* Removing shears form rotation matrix.

### 4.2.4 Rotation about an Arbitrary Axis

To rotate $\alpha$ radians arond $r$ which is a normalized rotation axis, first find two more arbitrary axes of unit length that are
mutually orthogonal with themselves and with $r$. The idea is to change bases from standard basis to this new one, then rotate and
finally transform back to the standard basis.

<p align="center">
  <img src="arbitrary_axis_rotation.png">
</p>

The first step is to compute orthonormal axes of the basis. The first axis is $r$. The third axis $t$ will be the cross product
of the first and second axes: $t = r \times s$. A numerically stable way to do this is ti find the smallest component of $r$, and
set it to 0. Swap the two remaining components, and then negate the first of these.

$$
    \bar{s} =
    \begin{cases}
        (0, -r_z, -r_y), & \text{if } |r_x| < |r_y|\ \text{and } |r_x| < |r_z|, \\
        (-r_z, 0, -r_x), & \text{if } |r_y| < |r_x|\ \text{and } |r_y| < |r_z|, \\
        (-r_y, r_x, 0),  & \text{if } |r_z| < |r_x|\ \text{and } |r_z| < |r_y|,
    \end{cases}
$$
$$
    s = \bar{s} / \|\bar{s}\|, \\
    t = r \times s.
$$

We use these three vectors as the rows in a matrix:

$$ 
    M =
    \begin{pmatrix}
        r^T \\
        s^T \\
        t^T
    \end{pmatrix}.
$$

This matrix transforms the vector $r$ into the x-axis, $s$ into y-axis and $t$ into z-axis. So the fnal transform
is then: $$X = M^TR_x(\alpha)M.$$

> Look at *arbitrary_axis_rotation* in *transforms.py*, there are also the second method.

## 4.3 Quaternions

Quaternions can be used for stable and constant interpolation of orientations, something that cannot
be done well with Euler angles.

Quaternions have 4 parts, the first three values are closely related to axis of rotation, with the
angle of rotation affecting all four parts. Each quaternion is represented by four real number, each
associated with a different part.

### 4.3.1 Mathematical Background

A quaternion $\hat{q}$ can be defined in the following ways, all equivalent.

$$
    \begin{align*}
        \hat{q} &= (q_v, q_w) = iq_x + jq_y + kq_z + q_w = q_v + q_w, \\
        q_v &= iq_x + jq_y + kq_z = (q_x, q_y, q_z), \\
        i^2 &= j^2 = k^2 = -1,\ jk = -kj = i,\ ki = -ik = j,\ ij = -ji = k.
    \end{align*}
$$

The variable $q_w$ is called the real part of a quaternion, $\hat{q}$. The imaginary part is $q_v$,
and $i$, $j$ and $k$ are called imaginary units.

For the imaginary part, $q_v$, we can use all the normal vector operations, such as addition, scaling,
dot product, cross product and more. Using the definition of the quaternion, the multiplication
operation between two quaternions, $\hat{q}$ and $\hat{r}$, is derived as shown below. Note that the
multiplication of the imaginary units is noncommutative.

**Multiplication** :

$$
    \begin{align*}
        \hat{q}\hat{r} =\ &(iq_x + jq_y + kq_z + q_w)(ir_x + jr_y + kr_z + r_w) \\
                       =\ &i(q_y r_z - q_z r_y + r_w q_x + q_w r_x) \\
                        &+ j(q_z r_x - q_x r_z + r_w q_y + q_w r_y) \\
                        &+ k(q_x r_y - q_y r_x + r_w q_z + q_w r_z) \\
                        &+ q_w r_w - q_x r_x - q_y r_y - q_z r_z = \\
                       = &(q_v \times r_v + r_w q_v  + q_w r_v, q_w r_w - q_b \cdot r_v)

    \end{align*}
$$

**Addition** : $\hat{q} + \hat{r} = (q_v, q_w) + (r_v, r_w) = (q_v + r_v, q_w + r_w).$

**Conjugate** : $\hat{q}^* = (q_v, q_w)^* = (-q_v, q_w).$

**Norm** : $n(\hat{q}) = \sqrt{\hat{q}\hat{q}^*} = \sqrt{\hat{q}^*\hat{q}} = \sqrt{q_v \cdot q_v + q_w^2} = \sqrt{q_x^2 + q_y^2 + q_z^2 + q_w^2}.$

**Identity** : $\hat{i} = (0, 1).$

**Inverse** : $\hat{q}^{-1} = \dfrac{1}{n(\hat{q}^2)} \hat{q}^*.$

**Conjugate rules** :
$$
    \begin{align*}
        (\hat{q}^*)^* &= \hat{q}, \\
        (\hat{q} + \hat{r})^* &= \hat{q}^* + \hat{r}^*, \\
        (\hat{q}\hat{r})^* &= \hat{r}^*\hat{q}^*.
    \end{align*}
$$

**Norm rules** :
$$
    \begin{align*}
        n(\hat{q}^*) &= n(\hat{q}), \\
        n(\hat{q}\hat{r}) &= n(\hat{q}) n(\hat{r}).
    \end{align*}
$$

**Laws of multiplication** :

**Linearity**:
$$
    \begin{align*}
        \hat{p}(s\hat{q} + t\hat{r}) &= s\hat{p}\hat{q} + t\hat{p}\hat{r}, \\
        (s\hat{p} + t\hat{q})\hat{r} &= s\hat{p}\hat{r} + t\hat{q}\hat{r}. \\
    \end{align*}
$$

**Associativity** : $$\hat{p}(\hat{q}\hat{r}) = (\hat{p}\hat{q})\hat{r}.$$

A unit quaternion, $\hat{q} = (q_v, q_w)$, is such that $n(\hat{q}) = 1$. From this it follows that $\hat{q}$ may be written as

$$\hat{q} = (\sin \phi u_q, \cos \phi) = \sin \phi u_q + \cos \phi,$$

for some three-dimensional vector $u_q$, such that $||u_q|| = 1$.

Unit quaternions are perfectly suited for creating rotations and orientations in a most efficient way. But before that,
some extra operations will be introduced for unit quaternions.

For complex numbers, a two-dimensional unit vector can be written as $\cos \phi + i\sin \phi = e^{i\phi}$. The equivalent for
quaternion is $$\hat{q} = \sin \phi u_q + \cos \phi = e^{\phi u_q}.$$

**Logarithm** : $\log(\hat{q}) = \log(e^{\phi u_q}) = \phi u_q$,

**Power** : $\hat{q}^t = (\sin \phi u_q + \cos \phi)^t = e^{\phi t u_q} = \sin (\phi t)u_q + \cos (\phi t)$.

### 4.3.2 Quaternion Transforms

Unit quaternions can represent any three-dimensional rotation extremely compact and simple.

If we put the four coordinates of a point orvector $p = (p_x\ p_y\ p_z\ p_w)^T$ into the components of a
quaternion $\hat{p}$, and assume that we have a unit quaternion $\hat{q} = (\sin \phi u_q, \cos \phi)$.
Then $$\hat{q}\hat{p}\hat{q}^{-1}$$ rotates $\hat{p}$ (and thus the point $p$) around the axis $u_q$ by an 
angle $2\phi$.

*Matrix Conversion* <br>
A quaternion $\hat{q}$ can be converted into a matrix $M^q$:

$$
    M^q = 
    \begin{pmatrix}
        1 - s(q_y^2 + q_z^2) & s(q_x q_y - q_w q_z) & s(q_x q_z + q_w q_y) & 0 \\
        s(q_x q_y + q_w q_z) & 1 - s(q_x^2 + q_z^2) & s(q_y q_z - q_w q_x) & 0 \\
        s(q_x q_z - q_w q_y) & s(q_y q_z + q_w q_x) & 1 - s(q_x^2 + q_y^2) & 0 \\ 
        0 & 0 & 0 & 1
    \end{pmatrix}.
$$

Here, the scalar is $s = 2/n(\hat{q})$. For unit quaternions $s = 2$.

*Spherical Linear Interpolation* <br>
Spherical linear interpolation is an operation that computes an interpolated quaternion. This is useful for
animating objects, but not the camera, as the camera's up vector can become tilted during interpolation.

The algebraic form $$\hat{s}(\hat{q},\hat{r},\hat{t}) = (\hat{r}\hat{q}^{-1})^t \hat{q}$$
Software implementation from 
$$
    \hat{s}(\hat{q},\hat{r},\hat{t}) = \text{slerp(q, r, t)} = 
        \frac{\sin(\phi(1-t))}{\sin \phi}\hat{q} + \frac{\sin(\phi t)}{\sin \phi}\hat{r}
$$ 

To compute $\phi$ which is needed in this equation, the following can be used: 
$\cos \phi = q_x r_x + q_y r_y + q_z r_z + q_w r_w$. For $t \in [0, 1]$, the slerp function computes unique interpolated 
quaternions that together constitute the shortest arc on a four-dimensional unit sphere from $\hat{q}(t = 0)$ to
$\hat{r}(t = 1)$ if $\hat{q}$ and $\hat{r}$ are not opposite. The arc is located on the circle that is formed from the
intersection between the plane given by $\hat{q}, \hat{r}$, the origin and the four-dimensional unit sphere. The computed
rotation quaternion rotates around a fixed axis at constant speed. A curve that has constant speed and thus zero 
acceleration, is called a *geodesic curve*.

Computing a slerp directly is expensive operation involving calling trigonometric functions. A better way to interpolate is
to use some sort of spline, introducing quaternions $\hat{a}_i$ and $\hat{a}_{i+1}$ between $\hat{q}_i$ and $\hat{q}_{i+1}$.
Spherical cubic interpolation can be defined within the set of these quaternions, they are computed as

$$
    \hat{a}_i = \hat{q}_i \exp[-\frac{\log(\hat{q}_i^{-1}\hat{q}_{i-1} + \hat{q}_i^{-1}\hat{q}_{i+1})}{4}].
$$

The $\hat{q}_i$ and $\hat{a}_i$ will be used to spherically interpolate the quaternions using a smooth cubic spline

$$
    \text{squad}(\hat{q}_i, \hat{q}_{i+1}, \hat{a}_i, \hat{a}_{i+1}, t) = \\
        \text{slerp}(\text{slerp}(\hat{q}_i, \hat{q}_{i+1}, t), \text{slerp}(\hat{a}_i, \hat{a}_{i+1}, t), 2t(1-t)).
$$

The squad function is constructed from repeated spherical interpolation using slerp. The interpolation will pass through
the initial orientations $\hat{q}_i, i \in [0, ..., n-1]$, but not through $\hat{a}_i$ - these are used to indicate the
tangent orientations at the initial orientations.

*Rotation from One Vector to Another* <br>
A common operation is transforming from one direction $s$ to another direction $t$ via the shortest path possible. First,
normalize $s$ and $t$. Then compute the unit rotation axis, called $u$, $u = (s \times t)/||s \times t||$. Next,
$e = s \cdot t = \cos(2\phi)$ and $||s \times t|| = \sin(2\phi)$, where $2\phi$ is the angle between $s$ and $t$. The
quaternion $\hat{q} = (\sin \phi u, \cos \phi)$ represents rotation between $s$ and $t$ or 

$$
    \hat{q} = (q_v, q_w) = (\frac{1}{\sqrt{2(1+e)}}(s \times t), \frac{\sqrt{2(1+e)}}{2}).
$$

Sometimes we need the matrix representation of this rotation

$$
    R(s, t) =
    \begin{pmatrix}
        e + h v_x^2     & h v_x v_y - v_z & h v_x v_z + v_y & 0 \\
        h v_x v_y + v_z & e + h v_y^2     & h v_y v_z - v_x & 0 \\
        h v_x v_z - v_y & h v_y v_z + v_x & e + h v_z^2     & 0 \\
        0 & 0 & 0 & 1
    \end{pmatrix}
$$
Where 
$$
    \begin{align*}
    v &= s \times t \\
    e &= \cos(2\phi) = s \cdot t \\
    h &= \frac{1-cos(2\phi)}{\sin^2(2\phi)} = \frac{1 - e}{v \cdot v} = \frac{1}{1 + e}.
    \end{align*}
$$

## 4.4 Vertex Blending

*Vertex blending* has several other names, such as *skinning*, *enveloping* and 
*skeleton-subspace deformation*. The main idea is defining bones and having skin react to changes.
In its simpliest form, two objects are animated separately, but at the joint, the two part are
connected through an elastic "skin". This elastic part will have one set of vertices that are
transformed by first object's matrix and another set by second. This basic technique is sometimes
called stitching.

<p align="center">
  <img src="vertex_blending.png">
</p>

A single vertex can be transformed by several different matrices, with the resulting locations weighted
and blended together.

