#!/usr/bin/env python3

import numpy as np
import matplotlib.pyplot as plt

def q1polar(t1, t2, t3, verbose=True):
    u3 = t1 * t2 * t3
    u2 = (t1*t2+t2*t3+t1*t3)/3
    u1 = (t1+t2+t3)/3
    ret = (
        - 7/8 * u3 + 9 * u2 - (57/2) * u1 + 30,
        - 5/8 * u3 + 15/4 * u2 - (9/2) * u1 - 1
    )
    if verbose:
        print(f"q1polar({t1}, {t2}, {t3})={ret}")
    return ret

c_pnts = []
for t1, t2, t3 in [(2,2,2), (2,2,4), (2,4,4), (4,4,4)]:
    c_pnts.append(q1polar(t1, t2, t3))



b = {}
de_casteljau_verbose = False
def de_casteljau(t, u, v):

    n = len(t)
    for i in range(0, n):
        if t[i] != u and t[i] != v:
            left_t = t.copy()
            right_t = t.copy()
            left_t[i] = u
            right_t[i] = v
            left_res = de_casteljau(left_t, u, v)
            right_res = de_casteljau(right_t, u, v)

            res = right_res * (t[i]- u) / (v - u) + left_res * (v-t[i]) / (v - u)
            if de_casteljau_verbose:
                print(f"b({t[0]},{t[1]},{t[2]}) = {(t[i]- u)/(v - u)} * b({left_t[0]},{left_t[1]},{left_t[2]}) + {(v-t[i])/ (v - u)} * b({right_t[0]},{right_t[1]},{right_t[2]}) = {res}")
            return res


    control_points = {
        (2, 2, 2): np.asarray((2, 0)),
        (2, 2, 4): np.asarray((0, 2)),
        (2, 4, 4): np.asarray((3, 4)),
        (4, 4, 4): np.asarray((4, 1))
    }

    # Symmetry!
    t_sorted = np.sort(t)
    for k, v in control_points.items():
        
        np_k = np.asarray(k)
        #print(np_k)
        if np.allclose(np_k, t_sorted):
            return v

    assert(False)

s_pnts = []
for u in [5/2, 3, 7/2]:
    res = de_casteljau(np.asarray([u, u, u]), 2, 4)
    print(f"{u}: {res}")
    s_pnts.append(res)
    v_pnt = q1polar(u, u, u, verbose=False)
    print(f"{u} validate: {v_pnt}")

plt.plot(
    [i for i in map(lambda p: p[0], c_pnts)],
    [i for i in map(lambda p: p[1], c_pnts)]
)

def plot_bezier():
    T = np.linspace(2, 4, 100)
    x = []
    y = []
    for t in T:
        val = q1polar(t, t, t, verbose=False)
        x.append(val[0])
        y.append(val[1])
    plt.plot(x, y)

plot_bezier()

plt.scatter(
    [i for i in map(lambda p: p[0], s_pnts)],
    [i for i in map(lambda p: p[1], s_pnts)],
    marker='s'
)
plt.show()

seg = {}
seg[1] = [(2,2,2), (2,2,3), (2,3,3), (3,3,3)]
seg[2] = [(3,3,3), (3,3,3.5), (3,3.5,3.5), (3.5,3.5,3.5)]
seg[3] = [(3.5,3.5,3.5), (3.5,3.5,4), (3.5,4,4), (4,4,4)]


for segIdx in [1,2,3]:
    seg_c_pnts = []
    for t1, t2, t3 in seg[segIdx]:
        seg_c_pnts.append(q1polar(t1, t2, t3))
    plt.plot(
        [i for i in map(lambda p: p[0], seg_c_pnts)],
        [i for i in map(lambda p: p[1], seg_c_pnts)],
    )

plot_bezier()
plt.scatter(
    [i for i in map(lambda p: p[0], s_pnts)],
    [i for i in map(lambda p: p[1], s_pnts)],
    marker='s'
)

plt.show()
