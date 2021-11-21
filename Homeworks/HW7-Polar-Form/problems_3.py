#!/usr/bin/env python3

import numpy as np
import matplotlib.pyplot as plt

# hardcoded for [t0, t1, t2, t3, t4, t5] and supp = [t2, t3] case (k = 4)
def de_boor(t=2.5):
    r = 3
    k = 4
    d = np.zeros((k+1, k+1, 2))
    tVec = [0, 1, 2, 3, 4, 5]
    kVec = [
        np.asarray((-2, -10)),
        np.asarray((-4, 2)),
        np.asarray((6, 5)),
        np.asarray((4, -7))
    ]

    for i in range(r - k + 1, r+1):
        d[0][i] = kVec[i]

    for j in range(1, k):
        for i in range(r - k + 1 + j, r+1):
            alpha = (t-tVec[i]) / (tVec[i+k-j] - tVec[i])
            d[j][i] = (1 - alpha) * d[j-1][i-1] + alpha * d[j-1][i]

    return d[k-1][r]

px = []
py = []
for t in np.linspace(2, 3, 100):
    res = de_boor(t=t)
    px.append(res[0])
    py.append(res[1])

plt.plot(px, py)


kVec = [
    np.asarray((-2, -10)),
    np.asarray((-4, 2)),
    np.asarray((6, 5)),
    np.asarray((4, -7))
]

plt.scatter(
    [i for i in map(lambda p: p[0], kVec)],
    [i for i in map(lambda p: p[1], kVec)],
    marker='s'
)

plt.show()