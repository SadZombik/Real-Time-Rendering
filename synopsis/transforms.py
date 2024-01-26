import numpy as np
import matplotlib.pyplot as plt

def point(x, y, z):
    return np.array([[x], [y], [z], [1]])

def vector(x, y, z):
    return np.array([[x], [y], [z], [0]])

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

if __name__ == '__main__':
    translation_test()
    # rotation_test()
    # example_usage_test()
    # rotation_test()