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

def T(point):
    return np.matrix([
        [1, 0, 0, float(point[0])],
        [0, 1, 0, float(point[1])],
        [0, 0, 1, float(point[2])],
        [0, 0, 0, 1]
    ])

def translation_test():
    print("translation_test:\n")

    # Just translates a point by 1 along x-axis
    p = point(1, 2, 3)
    point_translated = T(1, 0, 0) * p
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

def rotation_test():
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

def example_usage_test():
    p = point(2, 2, 0)
    
    X = T(p) * R_z(np.pi / 4) * T(-p)
    print(X)

if __name__ == '__main__':
    # translation_test()
    # rotation_test()
    example_usage_test()