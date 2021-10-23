# CAGD 作业 5

刘紫檀 SA21229063

## 目标

构造交互式的 3 阶 Bezier 样条曲线绘制程序。

## 原理

3 阶 Bezier 样条曲线，即为一条参数曲线 $ \mathbf{f}(t) $，其经过 $ \mathbf{k_0}, \mathbf{k_1}, ..., \mathbf{k_n} $，且满足如下条件
- 任意的 $ \mathbf{k_i} $ 到 $ \mathbf{k_{i+1}} $ 间均为 3 阶 Bezier 曲线
- 曲线满足 $ C^2 $ 连续性

我们设第 $ i $ （$i=0,...,n-1$）段 Bezier 曲线的控制点为 $ \{ p^{(i)}_j \}_{j=0}^3 $（我们要求 $ C^0 $ 连续，则 $ p_3^{(i)} = p_0^{(i+1)}$），同时记第 $ i $ 段 Bezier 的局部参数化形式为 $ \mathbf{y}_{(i)}(t) $，则有
$$
f(t) = \mathbf{y}_{(i)}(\frac{t-t_i}{t_{i+1} - t_{i}}) \quad t \in [t_i, t_{i+1}]
$$
成立，其中我们约定 $ \mathbf{x}(t_i) = \mathbf{k_i} $，$ t_i < t_j $ 如果 $ i < j$。

如此一来，我们一共有 $ 3n+1 $ 个未知数，分别对应该样条曲线的所有控制点 $ \{ p^{(i)}_j \}_{j=0}^3 $。

由 $ \mathbf{x}(t_i) = \mathbf{k_i} $，我们有如下等式成立（共 $ n+1 $ 个）

$$
\begin{aligned}
&p_0^{(i)} = k_i & i = 0, ..., n-1 \\
&p_3^{(n-1)} = k_{n}
\end{aligned}
$$

由 $ C^1 $ 连续，我们有如下等式成立（共 $ n-1 $ 个）
$$
\begin{aligned}
&\frac{p_{3}^{(i-1)}-p_{2}^{(i-1)}}{t_i - t_{i-1}} = \frac{p_{1}^{(i)}-p_{0}^{(i)}}{t_{i+1} - t_{i}} & i = 1, ..., n-1
\end{aligned}
$$
由 $ C^2 $ 连续，我们有如下等式成立（共 $ n - 1 $ 个）
$$
\begin{aligned}
&\frac{p_3^{(i-1)}-2p_2^{(i-1)}+p_1^{(i-1)}}{t_i - t_{i-1}} = \frac{p_2^{(i)}-2p_1^{(i)}+p_0^{(i)}}{t_{i+1} - t_{i}} & i = 1, ..., n-1
\end{aligned}
$$
然后再加上自然边界条件
$$
\begin{aligned}
&x''(t_0) = 0 \iff p_2^{(0)} - 2p_1^{(0)} + p_0^{(0)} = 0 \\
&x''(t_n) = 0 \iff p_1^{(0)} - 2p_2^{(n-1)} + p_3^{(n-1)} = 0 \\
\end{aligned}
$$
或 Bessel 边界条件
$$
TODO
$$
就可以得到所有的控制点，进而画出曲线。控制点共有 $ 3n+1 $ 个，需要 $ (3n+1) \times \dim(\mathbf{k_i}) $ 个方程。不过其实 $ p_x $ 和 $ p_y $ 所做变换完全相同，所以可以看成是对 $ p = [p_x\quad p_y] $ 进行求解，然后再弄出来。