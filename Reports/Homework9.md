# CAGD 作业 9
刘紫檀 SA21229063

## 问题一

将单位球面用一张双 2 次有理 Bezier 曲面来表示，并绘制出来。

### 分析

单位圆的方程 $ x^2 + y^2 + z^2 = 1 $，记 $ r^2 = x^2 + y^2 $ 我们有 $ r^2 + z^2 = 1 $。利用单位圆的参数化我们可以得到

$$
\left\{
\begin{aligned}
r(u) = \frac{1-u^2}{1+u^2} \\
z(u) = \frac{2u}{1+u^2}
\end{aligned}
\right.
$$

用有理二次样条的方式表达如下

同时，我们对 $ x^2 + y^2 = r^2 \Rightarrow (x/r)^2 + (y/r)^2 = 1 $ 参数化，得到

$$
\left\{
\begin{aligned}
x(v) = r \frac{1-v^2}{1+v^2} \\
y(v) = r \frac{2v}{1+v^2}
\end{aligned}
\right.
$$

那么，我们可以得到参数表示

$$
\left\{
\begin{aligned}
&x(u, v) = \frac{1-u^2}{1+u^2} \frac{1-v^2}{1+v^2} &= \frac{(1-u^2)(1-v^2)}{(1+u^2)(1+v^2)} \\
&y(u, v) = \frac{1-u^2}{1+u^2} \frac{2v}{1+v^2} &= \frac{2v(1-u^2)}{(1+u^2)(1+v^2)} \\
&z(u, v) = \frac{2u}{1+u^2} &= \frac{2u(1+v^2)}{(1+u^2)(1+v^2)}
\end{aligned}
\right.
$$

这里有两种做法，一种是用待定系数，因为双 2 次有理 Bezier 曲面 9 个的基函数可以写出来，然后线性组合去拼就可以；另一种办法是用旋转曲面的构造方法（TODO）。

设方程为
$$
\vec f(u, v) = \sum_{i, j} \vec p_{ij} B_i(u) B_j(v)  
$$

则待定系数解得的系数如下：

> 待定系数的计算参见 `calc_control_points` 函数。

$$
p_{00}=\begin{pmatrix} 1\\  0\\  0\\  1\\ \end{pmatrix} 
p_{01}=\begin{pmatrix}  1\\   1\\  0\\   1\\ \end{pmatrix} 
p_{02}=\begin{pmatrix} 0\\   2\\  0\\   2\\ \end{pmatrix} \\
p_{10}=\begin{pmatrix}  1\\  0\\   1\\   1\\ \end{pmatrix} 
p_{11}=\begin{pmatrix} 1\\  1\\  1\\  1\\ \end{pmatrix} 
p_{12}=\begin{pmatrix} 0\\  2\\  2\\  2\\ \end{pmatrix} \\
p_{20}=\begin{pmatrix} 0\\  0\\   2\\   2\\ \end{pmatrix} 
p_{21}=\begin{pmatrix} 0\\  0\\  2\\  2\\ \end{pmatrix} 
p_{22}=\begin{pmatrix} 0\\  0\\  4\\  4\\ \end{pmatrix} \\
$$

### 对偶曲面

做换元
$$
t = \frac{t_{\text{new}}}{2t_{\text{new}}-1}
$$
这样一波操作下来，我们发现 $ t_{\text{new}} $ 从 $[0, 1]$ 跑的时候，正好能跑到 $ t $ 取上面那两段。

观察到

$$
\frac{ \sum_{ij} \begin{pmatrix}p_{ij_x} \\ p_{ij_y} \\ p_{ij_z} \end{pmatrix} B_i^{(2)}(\frac{u_{\text{new}}}{2u_{\text{new}}-1}) B_j^{(2)}(\frac{v_{\text{new}}}{2v_{\text{new}}-1})} { \sum_{ij} p_{ij_w} B_i^{(2)}(\frac{u_{\text{new}}}{2u_{\text{new}}-1}) B_j^{(2)}(\frac{v_{\text{new}}}{2v_{\text{new}}-1})}
$$

的效果就是让 $ \vec p_{01}, \vec p_{10}, \vec p_{21}, \vec p_{12} $ 的符号变成负的，那对偶曲面就比较好画了。

<!-- 
另一种想法是考虑这是一个旋转曲面的事实。
不用复杂的待定系数，其实这个曲面是 X-Z 平面的有理 Bezier 曲线 $ b_1(u) $ 和 X-Y 平面的有理 Bezier 曲线 $ b_2(v) $ 张成的，那么就可以自然写出齐次坐标形式
$$
\vec b_1(u) = \begin{pmatrix}1 \\ 0 \\ 0 \\ 1\end{pmatrix} B_0^{(2)}(u)+ \begin{pmatrix}1 \\ 0 \\ 1 \\ 1\end{pmatrix} B_1^{(2)}(u)+ \begin{pmatrix}0 \\ 0 \\ 2 \\ 2\end{pmatrix} B_2^{(2)}(u) \\
\vec b_2(v) = \begin{pmatrix}1 \\ 0 \\ 0 \\ 1\end{pmatrix} B_0^{(2)}(v)+ \begin{pmatrix}1 \\ 1 \\ 0 \\ 1\end{pmatrix} B_1^{(2)}(v)+ \begin{pmatrix}0 \\ 2 \\ 0 \\ 2\end{pmatrix} B_2^{(2)}(v) 
$$

然后作为向量积曲面 $ b_1(u) b_1(v) $ 就可以了。

$$
\begin{aligned}
b_1(u)b_2(v) = 
&\begin{pmatrix}1 \\ 0 \\ 0 \\ 1\end{pmatrix} B_0^{(2)}(u)B_0^{(2)}(v) + 
\begin{pmatrix}1 \\ 0 \\ 0 \\ 1\end{pmatrix} B_0^{(2)}(u)B_1^{(2)}(v) + 
\begin{pmatrix}0 \\ 0 \\ 0 \\ 2\end{pmatrix} B_0^{(2)}(u)B_2^{(2)}(v) + \\
&\begin{pmatrix}1 \\ 0 \\ 0 \\ 1\end{pmatrix} B_1^{(2)}(u)B_0^{(2)}(v) +
\begin{pmatrix}1 \\ 0 \\ 0 \\ 1\end{pmatrix} B_1^{(2)}(u)B_1^{(2)}(v) +
\begin{pmatrix}0 \\ 0 \\ 0 \\ 2\end{pmatrix} B_1^{(2)}(u)B_2^{(2)}(v) + \\
&\begin{pmatrix}0 \\ 0 \\ 0 \\ 2\end{pmatrix} B_2^{(2)}(u)B_0^{(2)}(v) +
\begin{pmatrix}0 \\ 0 \\ 0 \\ 2\end{pmatrix} B_2^{(2)}(u)B_1^{(2)}(v) +
\begin{pmatrix}0 \\ 0 \\ 0 \\ 4\end{pmatrix} B_2^{(2)}(u)B_2^{(2)}(v)
\end{aligned}
$$ -->


### 结果展示



## 问题二



### 分析


#### 对偶圆锥曲线

观察到不管是椭圆的参数表示
$$
\left\{
\begin{aligned}
&x = a \cos t\\
&y = b \sin t
\end{aligned}
\right.
$$
还是双曲线的参数表示
$$
\left\{
\begin{aligned}
&x = a \sec t\\
&y = b \tan t
\end{aligned}
\right.
$$
如果带入半角公式的时候（$ t = \tan (\varphi/2)$）的换元中的 $ \varphi $ 能跑遍的话，那么参数表示也可以完整表示全部的曲线。一般的有理 Bezier 是定义在 $ [0, 1] $ 上的，现在我们想得到 $ (1, \infty) $ 和 $ (-\infty, 0) $ 的曲线，最简单的方法是做换元
$$
t = \frac{t_{\text{new}}}{2t_{\text{new}}-1}
$$
这样一波操作下来，我们发现 $ t_{\text{new}} $ 从 $[0, 1]$ 跑的时候，正好能跑到 $ t $ 取上面那两段。

带入换元，我们发现只要把 $ \vec p_1 $ 的分量加个负号，就还能用有理 Bezier 的方法来画整根曲线。

#### 绘制

按定义，用如下公式绘制即可

![image-20211123020535322](Homework8.assets/image-20211123020535322.png)

### 结果展示

![image-20211123050409235](Homework8.assets/image-20211123050409235.png)

![image-20211123050433904](Homework8.assets/image-20211123050433904.png)

## 问题三

某二次 Bezier 三角形有顶点参数坐标 $ a = (0, 0), b = (1, 0), c = (0.5, 1) $ 和以下控制点

$$
F(a, a) = \begin{pmatrix} 0 \\ 0 \\ 0 \end{pmatrix}
F(a, b) = \begin{pmatrix} 2 \\ 2 \\ 4 \end{pmatrix}
F(a, c) = \begin{pmatrix} 4 \\ -2 \\ 6 \end{pmatrix} \\
F(b, b) = \begin{pmatrix} 6 \\ -4 \\ 4 \end{pmatrix}
F(b, c) = \begin{pmatrix} 8 \\ 0 \\ 4 \end{pmatrix}
F(c, c) = \begin{pmatrix} 4 \\ 4 \\ 0 \end{pmatrix}
$$

问 $ p_1 = (0.25, 0.5), p_2 = (0.3, 0.75), p_3 = (0.5, 0.5) $ 中，哪个参数在三角形外？对于在三角形内的参数，用 de Casteljau 算法计算曲面 F(p, p) 在该参数处的坐标。

### 分析求解

容易知道 $ p_1 $ 在三角形的边上， $ p_2 $ 在三角形外，$ p_3 $ 在三角形内。

首先计算 $ p_3 $ 的重心坐标。列方程解后易得 
$$
p_3 = 0.25 a + 0.25 b + 0.5 c
$$

所以，
$$
\begin{aligned}
F(a, p_3) &= 0.25 F(a, a) + 0.25 F(a, b) + 0.5 F(a, c) \\
&= 0.25  \begin{pmatrix} 0 \\ 0 \\ 0 \end{pmatrix} + 0.25  \begin{pmatrix} 2 \\ 2 \\ 4 \end{pmatrix} + 0.5  \begin{pmatrix} 4 \\ -2 \\ 6 \end{pmatrix} \\
&= 233

F(b, p_3) &=0.25 F(b, a) + 0.25 F(b, b) + 0.5 F(b, c) \\
&= 0.25  \begin{pmatrix} 2 \\ 2 \\ 4 \end{pmatrix} + 0.25   \begin{pmatrix} 6 \\ -4 \\ 4 \end{pmatrix} + 0.5   \begin{pmatrix} 8 \\ 0 \\ 4 \end{pmatrix} \\
&= 233

F(c, p_3) &= 0.25 F(c, a) + 0.25 F(c, b) + 0.5 F(c, c)  \\
&= 0.25  \begin{pmatrix} 4 \\ -2 \\ 6 \end{pmatrix} + 0.25   \begin{pmatrix} 8 \\ 0 \\ 4 \end{pmatrix} + 0.5  \begin{pmatrix} 4 \\ 4 \\ 0 \end{pmatrix} \\
&= 233

F(p_3, p_3) &= F(0.25a + 0.25b + 0.5c, 0.25a + 0.25b + 0.5c) \\
&= 0.25 F(a, p_3) + 0.25 F(b, p_3) + 0.5 F(c, p_3) \\
&= 

\end{aligned}
$$

