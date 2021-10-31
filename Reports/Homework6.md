# CAGD 作业 6

刘紫檀 SA21229063

## 目标

进行 piecewise cubic 的 B 样条曲线交互式插值程序的编写。

## 原理

B 样条选用如下基函数：

$$
N_{i, 1} = \left\{ 
\begin{aligned}
& 1 & t \in [t_i, t_{i+1}] \\
& 0 & \text{otherwise}
\end{aligned}
\right.

\\
N_{i, k} = \frac{t-t_i}{t_{i+k-1}-t_i} N_{i, k-1} + \frac{t_{i+k} - t}{t_{i+k} - t_{i+1}} N_{i+1, k-1}
$$

此时，曲线可以表示成为如下形式
$$
x(t) = \sum_{i=0}^n N_{i,k}(t) d_i \quad t \in \cap_{i}\ \text{supp}\{N_{i,k}\}
$$
其中 $ d_i $ 为第 $ i $ 个控制点（$ i = 0, \dots, n$）

### de Boor 算法

展开上面的表达式，可以得到如下算法：

$$
\begin{aligned}
&d_i^{j} = ( 1 - \alpha_i^j ) d_{i-1}^{j-1} + \alpha_i^j d_{i}^{j-1} \\
&\text{where} \ \alpha_i^j = \frac{t-t_i}{t_{i+k-j} - t_i}\ \text{and}\ d_i^0 = d_i \\
\end{aligned}
$$

用上面的递推式求得 $ d_r^{k-1} $ 即为 $ x(t) $ 的值。

### 分片 $ C^2 $ 连续插值

给定 $ \{d_i\}_{i=0}^n $ ，我们需要获得如下问题的解：

$$
\begin{aligned}
&x(s_i) = d_i & i = 0, \dots, n \\
&x(s) \in C^2 & s \in [s_0, s_n]
&
\end{aligned}
$$
且我们希望我们的函数 $ x(t) $ 为 B 样条。

由于 $ k $ 阶 B 样条为 $ C^{k-2} $ 阶连续，为了达到 $ C^2 $ 连续，我们需要让 $ k = 4 $。

同时，我们希望 $ x(s_0) = d_0 $ 且 $ x(s_n) = d_n $，但是我们知道对于一般的 B 样条来说，$ t $ 的定义域位于 $ [t_{k}, t_{k+n}] $，这样我们取节点函数为 $ (s_0, \dots , s_0, s_1, \dots, s_n, \dots, s_n) $，其中 $ s_0 $ 和 $ s_n $ 分别重复 $ k $ 次，这样就可以达到 $ x(s_0) = d_0 $ 和 $ x(s_n) = d_n $ 且 $t \in [s_0, s_n ] $ 都有 $ C^{k-2} $ 连续的效果了。

此时，我们对于 $ k = 4 $ 的问题，有如下方程成立：

$$
\left\{
\begin{aligned}

x(s_0) &= d_0 \\
x(s_i) &= N_{i, 4}(s_i) d_i + N_{i+1, 4}(s_{i}) d_{i+1} + N_{i+2, 4}(s_{i}) d_{i+2} \\
x(s_n) &= d_{n+2}

\end{aligned}
\right.
$$
> 因为 $ N_{i-1}(s_i) d_i = 0 $，所以只有三项

解此方程即可。

$ N_{i, 4} $ 计算如下：

$$
\begin{aligned}
N_{i, 2} &= \frac{t-t_i}{t_{i+1}-t_i} N_{i, 1} + \frac{t_{i+2} - t}{t_{i+2} - t_{i+1}} N_{i+1, 1} \\
&= \frac{t-t_i}{t_{i+1}-t_i} N_{i, 1} + \frac{t_{i+2} - t}{t_{i+2} - t_{i+1}} N_{i+1, 1} \\
&= \left\{
\begin{aligned}
&\frac{t-t_i}{t_{i+1}-t_i} &t \in [t_i, t_{i+1}] \\
&\frac{t_{i+2} - t}{t_{i+2} - t_{i+1}} &t \in [t_{i+1}, t_{i+2}] \\
&0 & \text{otherwise}
\end{aligned}
\right.
\\

N_{i ,3} &= \frac{t-t_i}{t_{i+2}-t_i} N_{i, 2} + \frac{t_{i+3} - t}{t_{i+3} - t_{i+1}} N_{i+1, 2} \\
&= \left\{
\begin{aligned}
&\frac{t-t_i}{t_{i+2}-t_i} \frac{t-t_i}{t_{i+1}-t_i} &t \in [t_i, t_{i+1}] \\
&\frac{t-t_i}{t_{i+2}-t_i} \frac{t_{i+2} - t}{t_{i+2} - t_{i+1}} + \frac{t_{i+3} - t}{t_{i+3} - t_{i+1}} \frac{t-t_{i+1}}{t_{i+2}-t_{i+1}} &t \in [t_{i+1}, t_{i+2}] \\
&\frac{t_{i+3} - t}{t_{i+3} - t_{i+1}} \frac{t_{i+3} - t}{t_{i+3} - t_{i+2}} &t \in [t_{i+2}, t_{i+3}] \\
&0 & \text{otherwise}
\end{aligned}
\right.
\\

N_{i, 4} &= \frac{t-t_i}{t_{i+3}-t_i} N_{i, 3} + \frac{t_{i+4} - t}{t_{i+4} - t_{i+1}} N_{i+1, 3} \\
&= \text{太长了，就不写了吧}
\end{aligned}
$$

> 写完才意识到其实这个可以在程序里面动态求，没必要全展开再编程，也未必快

