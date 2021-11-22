#pragma once
#include <algorithm>
#include <cassert>
#include <cmath>
#include <type_traits>
#include <cstring>
#include <vector>
#include <string>
#include <MeowGLMath.h>

namespace CAGD
{
template <typename T>
using Vec = std::vector<T>;

using String = std::string;

template <typename T, std::size_t N> using vec = MeowGL::vec<T, N>;
template <typename T> using Vec2 = vec<T, 2>;
using Vec2d = Vec2<double>;
using Vec2f = Vec2<float>;
using Vec2i = Vec2<int>;

template <typename T> using Vec3 = vec<T, 3>;
using Vec3d = Vec3<double>;
using Vec3f = Vec3<float>;
using Vec3i = Vec3<int>;

template <typename T> using Vec4 = vec<T, 4>;
using Vec4d = Vec4<double>;
using Vec4f = Vec4<float>;
using Vec4i = Vec4<int>;

} // namespace CAGD