import numpy as np
import matplotlib.pyplot as plt

def B(n, k, t):
    assert(n == 2)
    if k == 0:
        return (1-t)*(1-t)
    elif k == 1:
        return 2*t*(1-t)
    elif k == 2:
        return t * t
    else:
        assert(False)

def gen_criss_cross():
    """Gen criss-cross schemed triangles"""

def calc_control_points():
    Bdim1 = np.matrix([
        [1, -2, 1],  # (1-t)^2
        [0, 2, -2],  # 2t(1-t)
        [0, 0,  1]   # t^2
    ])

    Bdim2 = np.ndarray((3, 3, 9), dtype=np.float64)
    for i in range(0, 3):
        for j in range(0, 3):
            prod = Bdim1[i].T @ Bdim1[j]
            squeezed = prod.flatten()
            Bdim2[i, j] = squeezed
    
    cand = np.asarray([
    #    1,  v, v^2,  u,  uv, uv^2, u^2, u^2v, u^2v^2 
        [1,  0,  -1,  0,   0,    0,  -1,  0,   1],
        [0,  2,   0,  0,   0,    0,   0, -2,   0],
        [0,  0,   0,  2,   0,    2,   0,  0,   0],
        [1,  0,   1,  0,   0,    0,   1,  0,   1]
    ])

    A = np.ndarray((9, 9))
    b = np.ndarray((9,))

    pAggregated = np.ndarray((3, 3, 4))
    for coord in range(0, 4):
        # Build A
        for i in range(0, 9):
            A[:, i] = Bdim2[i // 3, i % 3]
        # Build b
        b = cand[coord]

        p = np.linalg.solve(A, b)
        assert(np.allclose(A @ p, b))

        print(p)
        for i in range(0, 9):
            pAggregated[i // 3, i % 3, coord] = p[i]
    
    #print(pAggregated)
    for i in range(0, 3):
        for j in range(0, 3):
            msg = f"p_-{i}{j}|={pAggregated[i, j]}"
            msg = msg.replace("-", "{")
            msg = msg.replace("|", "}")
            msg = msg.replace("[", "\\begin{pmatrix} ")
            msg = msg.replace(".", "\\\\ ")
            msg = msg.replace("]", "\\end{pmatrix} \\\\")
            print(msg)
    
    return pAggregated.copy()

def calc_dual_control_points():
    pAggregated = calc_control_points()
    pAggregated[0, 1] *= -1
    pAggregated[1, 0] *= -1
    pAggregated[2, 1] *= -1
    pAggregated[1, 2] *= -1
    return pAggregated

def plot_prob1(samplePerAxis):
    u = np.linspace(0, 1, samplePerAxis)
    v = np.linspace(0, 1, samplePerAxis)

    # https://stackoverflow.com/questions/36013063/what-is-the-purpose-of-meshgrid-in-python-numpy
    UU, VV = np.meshgrid(u, v)
    XX, YY = np.meshgrid(u, v)
    XXDual, YYDual = np.meshgrid(u, v)

    points = np.zeros((samplePerAxis, samplePerAxis, 4), dtype=np.float64)
    pointsDual = np.zeros_like(points)
    Z = np.zeros((samplePerAxis, samplePerAxis), dtype=np.float64)
    ZDual = np.zeros_like(Z)

    ctrlPoints = calc_control_points()
    ctrlPointsDual = calc_dual_control_points()
    # ctrlPointsArray = [
    #     [(1, 0, 0, 1), (1, 0, 0, 1), (0, 0, 0, 2)],
    #     [(1, 0, 0, 1), (1, 0, 0, 1), (0, 0, 0, 2)],
    #     [(0, 0, 0, 2), (0, 0, 0, 2), (0, 0, 0, 4)]
    # ]

    # ctrlPoints = np.asarray(ctrlPointsArray, dtype=np.float64)

    for xIdx in range(0, samplePerAxis):
        for yIdx in range(0, samplePerAxis):
            gridU = UU[yIdx][xIdx]
            gridV = VV[yIdx][xIdx]

            for b1Idx in range(0, 3):
                for b2Idx in range(0, 3):
                    weight = B(2, b1Idx, gridU) * B(2, b2Idx, gridV) * ctrlPoints[b1Idx, b2Idx]
                    points[xIdx, yIdx] += weight
    
            XX[yIdx][xIdx] = points[xIdx, yIdx, 0] / points[xIdx, yIdx, 3]
            YY[yIdx][xIdx] = points[xIdx, yIdx, 1] / points[xIdx, yIdx, 3]
            Z[yIdx][xIdx] = points[xIdx, yIdx, 2] / points[xIdx, yIdx, 3]

            #print(f"f({gridU},{gridV})=({XX[yIdx][xIdx]},{YY[yIdx][xIdx]},{Z[yIdx][xIdx]})")

            for b1Idx in range(0, 3):
                for b2Idx in range(0, 3):
                    weight = B(2, b1Idx, gridU) * B(2, b2Idx, gridV) * ctrlPointsDual[b1Idx, b2Idx]
                    pointsDual[xIdx, yIdx] += weight
    
            XXDual[yIdx][xIdx] = pointsDual[xIdx, yIdx, 0] / pointsDual[xIdx, yIdx, 3]
            YYDual[yIdx][xIdx] = pointsDual[xIdx, yIdx, 1] / pointsDual[xIdx, yIdx, 3]
            ZDual[yIdx][xIdx] = pointsDual[xIdx, yIdx, 2] / pointsDual[xIdx, yIdx, 3]
    
    fig = plt.figure()
    ax = fig.add_subplot(projection='3d')

    #ax.plot_wireframe(XX, YY, Z)
    ax.plot_wireframe(XXDual, YYDual, ZDual)
    plt.show()

# calc_control_points()
plot_prob1(30)