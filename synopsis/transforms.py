# sketch script for testing different linalg methods with visualizing

import numpy as np
import matplotlib.pyplot as plt
from dataclasses import dataclass

ax = plt.figure().add_subplot(projection='3d')

@dataclass
class Point:
    x: float
    y: float
    z: float
    w: float = 1

@dataclass
class Vector:
    x: float
    y: float
    z: float
    w: float = 0

def point(x, y, z):
    return np.array([[x], [y], [z], [1]])

def point_p(p):
    return np.array([[p.x], [p.y], [p.z], [1]])

def vector(x, y, z):
    return np.array([[x], [y], [z], [0]])

def vector_v(v):
    return np.array([[v.x], [v.y], [v.z], [0]])

def is_orthogonal(M):
    
    # 1) |R| = +- 1

    # 2) R^-1 = R^T

    # 3) R^T is also orthogonal

    # 4) ||Mu|| = ||u||

    pass

# Translation matrix
def T(t_x=1, t_y=1, t_z=1):
    return np.matrix([
        [1, 0, 0, t_x],
        [0, 1, 0, t_y],
        [0, 0, 1, t_z],
        [0, 0, 0, 1]
    ])

def T_p(point):
    return np.matrix([
        [1, 0, 0, float(point[0])],
        [0, 1, 0, float(point[1])],
        [0, 0, 1, float(point[2])],
        [0, 0, 0, 1]
    ])

def translation_test():
    print("translation_test:\n")

    # Just translates a point by 1 along x-axis
    p = point(1, 3, 0)
    point_translated = T(0, 2, 0) * p
    print(point_translated)

    # T does not affect direction vector
    v = vector(1, 2, 3)
    vector_translated = T(5, 2, 0) * v
    print(vector_translated)


# Rotation matrices (X)
def R_x(phi):
    return np.matrix([
        [1, 0,            0,           0],
        [0, np.cos(phi), -np.sin(phi), 0],
        [0, np.sin(phi),  np.cos(phi), 0],
        [0, 0,            0,           1]
    ])

# Rotation matrices (Y)
def R_y(phi):
    return np.matrix([
        [ np.cos(phi), 0, np.sin(phi), 0],
        [ 0,           1, 0,           0],
        [-np.sin(phi), 0, np.cos(phi), 0],
        [ 0,           0, 0,           1]
    ])

# Rotation matrices (Z)
def R_z(phi):
    return np.matrix([
        [np.cos(phi), -np.sin(phi), 0, 0],
        [np.sin(phi),  np.cos(phi), 0, 0],
        [0,            0,           1, 0],
        [0,            0,           0, 1]
    ])

def rotation_matrix_test():
    print("rotation_test:\n")

    # test that 
    # > All rotation matrices have a determenant of one and are orthogonal
    R = R_x(60)

    # Definition
    # R * R^T = R^T * R = I
    M1 = R * R.T
    M2 = R.T * R
    I = np.identity(4)

    print(np.allclose(M1, M2))
    print(np.allclose(M1, I)) 
    print(np.allclose(M2, I))

    # Determinant
    print(np.linalg.det(R) == 1)

def rotation_test():
    ax = plt.figure().add_subplot(projection='3d')

    VERTICES = np.array([
        [ 1, 1, 0],
        [ 1,-1, 0],
        [-1,-1, 0],
        [-1, 1, 0],
    ])

    x = []
    y = []
    z = []

    for v in VERTICES:
        x.append(v[0])
        y.append(v[1])
        z.append(v[2])

    x.append(VERTICES[0][0])
    y.append(VERTICES[0][1])
    z.append(VERTICES[0][2])

    rotated = []

    for v in VERTICES:
        mul = T(0, 2, 0) * R_z(np.pi / 4) * point(v[0], v[1], v[2])
        res = [mul[0].A1[0], mul[1].A1[0], mul[2].A1[0]]
        rotated.append(res)

    ax.plot(x, y, z)
    x = []
    y = []
    z = []

    for r in rotated:
        x.append(r[0])
        y.append(r[1])
        z.append(r[2])

    x.append(rotated[0][0])
    y.append(rotated[0][1])
    z.append(rotated[0][2])

    ax.plot(x, y, z)

    ax.set_xlim(-5, 5)
    ax.set_ylim(-5, 5)
    ax.set_zlim(-5, 5)
    plt.show()

def example_usage_test():
    p = point(2, 2, 0)
    
    X = T(p) * R_z(np.pi / 4) * T(-p)
    print(X)

def plot_vec(p: Point or Vector, color = 'green'):
    ax.plot([0, p.x],
            [0, p.y],
            [0, p.z], color=color)

def arbitrary_axis_rotation():
    rn = Point(0.6, 0.7, 0.5)
    l_rn = np.sqrt(rn.x**2 + rn.y**2 + rn.z**2)

    r = Point(rn.x / l_rn, rn.y / l_rn, rn.z / l_rn)

    sn: Point
    if np.abs(r.x) < np.abs(r.y) and np.abs(r.x) < np.abs(r.z):
        sn = Point(0, -r.z, r.y)
    elif np.abs(r.y) < np.abs(r.x) and np.abs(r.y) < np.abs(r.z):
        sn = Point(-r.z, 0, r.x)
    elif np.abs(r.z) < np.abs(r.x) and np.abs(r.z) < np.abs(r.y):
        sn = Point(-r.y, r.x, 0)

    l_sn = np.sqrt(sn.x**2 + sn.y**2 + sn.z**2)
    s = Point(sn.x / l_sn, sn.y / l_sn, sn.z / l_sn)

    t = np.cross(point_p(r)[:-1].flatten(), point_p(s)[:-1].flatten())
    t = Point(t[0], t[1], t[2])

    M = np.matrix([
        [r.x, r.y, r.z, 0],
        [s.x, s.y, s.z, 0],
        [t.x, t.y, t.z, 0],
        [0, 0, 0, 1]
    ])

    alpha = np.pi / 2
    X = M.T * R_x(alpha) * M

    v = Vector(0.6, 1, 0.3)
    v_a = vector_v(v)

    v_r = np.dot(X, v_a)

    plot_vec(Vector(
        float(v_r[0]),
        float(v_r[1]),
        float(v_r[2])))

    # plotting
    for p in [Point(1, 0, 0), Point(0, 1, 0), Point(0, 0, 1)]:
        plot_vec(p)

    plot_vec(v, 'blue')
    plot_vec(r, 'red')
    # plot_vec(s, 'blue')
    # plot_vec(t, 'yellow')

    limit = 2
    ax.set_xlim(-limit, limit)
    ax.set_ylim(-limit, limit)
    ax.set_zlim(-limit, limit)
    plt.show()

    # Matrix for 2-nd method (p. 82)
    phi = np.pi
    R = np.matrix([
        [np.cos(phi)+(1-np.cos(phi))*r.x**2, (1-np.cos(phi))*r.x*r.y-r.z*np.sin(phi), (1-np.cos(phi))*r.x*r.z+r.y*np.sin(phi)],
        [(1-np.cos(phi))*r.x*r.y+r.z*np.sin(phi), np.cos(phi)+(1-np.cos(phi))*r.y**2, (1-np.cos(phi))*r.y*r.z-r.x*np.sin(phi)], 
        [(1-np.cos(phi))*r.x*r.z-r.y*np.sin(phi), (1-np.cos(phi))*r.y*r.z+r.x*np.sin(phi), np.cos(phi)+(1-np.cos(phi))*r.z**2]
    ])

if __name__ == '__main__':
    # translation_test()
    # rotation_test()
    # example_usage_test()
    # rotation_test()

    arbitrary_axis_rotation()