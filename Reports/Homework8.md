# CAGD 作业 8
刘紫檀 SA21229063

## 问题一

编程计算并绘出中心为 $ x $, 边长为 $2d$ 的立方体在 2 维平面上的透视投影，其中 $x$ 点和 $d$ 的大小由用户指定，对相机参数和方位进行合理假设。

### 分析

立方体的八个顶点为（$ \vec x = 0 $ 时）
$$
(d, d, d) \ 
(d, d, -d) \ 
(d, -d, d) \ 
(d, -d, -d) \\
(-d, d, d) \ 
(-d, d, -d) \ 
(-d, -d, d)  \ 
(-d, -d, -d) \
$$
设相机位置 $ (0, 0, z_0 ) $ ，像平面位置 $ z = \text{nearZ} $ ，则点 $ (x, y, z) $ 在像平面上的位置为
$$
x' = \text{nearZ} \times \frac{x}{z-z_0} \\
y' = \text{nearZ} \times \frac{y}{z-z_0}
$$
则可以认为每个点经过了这样的变换
$$
\begin{bmatrix}
x' \\ y' \\ z' \\ w'
\end{bmatrix} = 
\begin{bmatrix}
\text{nearZ} & 0 & 0 & 0 \\
0 & \text{nearZ} & 0 & 0 \\
0 & 0 & \text{nearZ} & 0 \\
0 & 0 & 0 & z-z_0 \\
\end{bmatrix}
\begin{bmatrix}
x \\ y \\ z \\ 1
\end{bmatrix}
$$

> 如果想要相机背面的内容不显示出来，只要变换后 clip 掉存在某些 $ w' < 0$ 的顶点的这些图元就行，这里没实现

将齐次坐标 $ (x', y', z', w') $ 转换为 $ (x'/w', y'/w', z'/w') $ 后，  将 $(x'/w', y'/w')$ 画出即可。

## 问题二

用有理二次 Bezier 样条绘制椭圆
$$
\frac{x^2}{a^2} + \frac{y^2}{b^2} = 1
$$
和双曲线
$$
\frac{x^2}{a^2} − \frac{y^2}{b^2} = 1
$$
使得样条分段尽可能少。参数 $ a $ 和 $ b $ 由用户指定。






## 问题三

在 3D 空间中绘制前一题中用齐次坐标表示的 Bezier 曲线（即做投影变换之前的三维曲线）。
