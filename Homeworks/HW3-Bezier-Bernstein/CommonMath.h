#pragma once
#include <algorithm>
#include <cassert>
#include <cmath>
#include <type_traits>
#include <cstring>

namespace CAGD
{
template <typename T, std::size_t N>
class vec
{
private:
	T M_instance[N];

	template <typename Integer,
		std::size_t startPos,
		typename... Args>
		typename std::enable_if<
		!bool(std::is_floating_point<Integer>::value),
		void>::type
		_vec_assign(Integer arg, Args... Remaining)
	{
		static_assert(startPos < N, "Assignment out of range");
		M_instance[startPos] = arg;
		_vec_assign<Integer, startPos + 1>(Remaining...);
	}

	template <typename _,
		std::size_t startPos>
		inline void _vec_assign()
	{
		// no-op
	}

	template <typename Floating,
		std::size_t startPos,
		typename... Args>
		typename std::enable_if<
		std::is_floating_point<Floating>::value,
		void>::type
		_vec_assign(Floating arg, Args... Remaining)
	{
		static_assert(startPos < N, "Assignment out of range");
		assert(std::isfinite(arg));
		M_instance[startPos] = arg;
		_vec_assign<Floating, startPos + 1>(Remaining...);
	}

public:
	vec()
	{
		for (std::size_t i = 0; i < N; i++)
		{
			M_instance[i] = 0;
		}
	}

	// 0. Check for Float or Int
	// 1. Check if we've got sufficient args
	// 2. Pass to _vec_initialize private function
	template <typename... Args> vec(Args... init)
	{
		static_assert(sizeof...(init) == N,
				"Initialization shall be N arguments in total");
		_vec_assign<
			T,
			0
		>(init...);
	}

	vec(const vec<T, N>& other)
	{
		for (std::size_t i = 0; i < N; i++)
		{
			M_instance[i] = other.M_instance[i];
		}
	}

	const vec<T, N>& operator=(const vec<T, N>& other)
	{
		for (std::size_t i = 0; i < N; i++)
		{
			M_instance[i] = other.M_instance[i];
		}
		return *this;
	}

#define _VecAccessor(name, size, pos) T& name(){ static_assert(N >= size, "N >= " #size " required"); return M_instance[pos]; }

	_VecAccessor(x, 1, 0)
		_VecAccessor(y, 2, 1)
		_VecAccessor(z, 3, 2)
		_VecAccessor(w, 4, 3)
		_VecAccessor(R, 1, 0)
		_VecAccessor(G, 2, 1)
		_VecAccessor(B, 3, 2)
		_VecAccessor(A, 4, 3)

#undef _VecAccessor

#define _ConstVecAccessor(name, size, pos) const T& name()const{ static_assert(N >= size, "N >= " #size " required"); return M_instance[pos];}

		_ConstVecAccessor(x, 1, 0)
		_ConstVecAccessor(y, 2, 1)
		_ConstVecAccessor(z, 3, 2)
		_ConstVecAccessor(w, 4, 3)
		_ConstVecAccessor(R, 1, 0)
		_ConstVecAccessor(G, 2, 1)
		_ConstVecAccessor(B, 3, 2)
		_ConstVecAccessor(A, 4, 3)

#undef _ConstVecAccessor

	inline T& operator[](std::size_t idx) { return M_instance[idx]; }
	inline const T& operator[](std::size_t idx) const { return M_instance[idx]; }

	inline const T* data() const { return M_instance; }
	inline T* data() { return M_instance; }

	inline vec<T, N> operator+(const vec<T, N>& other)const
	{
		vec<T, N> ret;
		for (std::size_t i = 0; i < N; i++)
		{
			ret[i] = M_instance[i] + other.M_instance[i];
		}
		return ret;
	}

	inline vec<T, N>& operator+=(const vec<T, N>& other)
	{
		for (std::size_t i = 0; i < N; i++)
		{
			M_instance[i] += other.M_instance[i];
		}
		return *this;
	}

	inline vec<T, N> operator-(const vec<T, N>& other)const
	{
		vec<T, N> ret;
		for (std::size_t i = 0; i < N; i++)
		{
			ret[i] = M_instance[i] - other.M_instance[i];
		}
		return ret;
	}

	inline vec<T, N>& operator-=(const vec<T, N>& other)
	{
		for (std::size_t i = 0; i < N; i++)
		{
			M_instance[i] -= other[i];
		}
		return *this;
	}

	inline vec<T, N> operator*(T val)const
	{
		vec<T, N> ret;
		for (std::size_t i = 0; i < N; i++)
		{
			ret[i] = M_instance[i] * val;
		}
		return ret;
	}

	inline vec<T, N>& operator*=(T val)
	{
		for (std::size_t i = 0; i < N; i++)
		{
			M_instance[i] *= val;
		}
		return *this;
	}

	inline vec<T, N> operator-()const
	{
		vec<T, N> ret;
		for (std::size_t i = 0; i < N; i++)
		{
			ret[i] = -M_instance[i];
		}
		return ret;
	}

	template <typename Floating> inline
		typename std::enable_if<
		std::is_floating_point<Floating>::value,
		vec<T, N>
		>::type operator/(Floating val)const
	{
		vec<T, N> ret;
		for (std::size_t i = 0; i < N; i++)
		{
			ret[i] = M_instance[i] / val;
		}
		return ret;
	}

	template <typename Floating> inline
		typename std::enable_if<
		std::is_floating_point<Floating>::value,
		vec<T, N>
		>::type& operator/=(Floating val)
	{
		for (std::size_t i = 0; i < N; i++)
		{
			M_instance[i] /= val;
		}
		return *this;
	}

	inline bool operator==(const vec<T, N>& other)const
	{
		for (std::size_t i = 0; i < N; i++)
		{
			if (M_instance[i] != other.M_instance[i])
				return false;
		}
		return true;
	}

	inline T dot(const vec<T, N>& other) const
	{
		T dot_product = 0;
		for (std::size_t i = 0; i < N; i++)
		{
			dot_product += M_instance[i] * other.M_instance[i];
		}
		return dot_product;
	}

	inline T squared_norm() const
	{
		T res = 0;
		for (std::size_t i = 0; i < N; i++)
		{
			res += M_instance[i] * M_instance[i];
		}
		return res;
	}

	inline T norm() const
	{
		return std::sqrt(squared_norm());
	}

	inline vec<T, N> normalize()const
	{
		T vec_norm = norm();
		if (vec_norm == 0.0 || !std::isfinite(vec_norm))
			return *this;
		else
			return *this / vec_norm;
	}

	inline vec<T, N>& normalized()
	{
		T vec_norm = norm();
		if (vec_norm != 0.0 && std::isfinite(vec_norm))
			*this /= vec_norm;
		return *this;
	}

	template <typename U> vec<U, N> cast_to()const
	{
		vec<U, N> ret;
		for (std::size_t i = 0; i < N; i++)
		{
			ret[i] = static_cast<U>(M_instance[i]);
		}
		return ret;
	}

	// upcast to dimension N + 1 with last element as 1
	inline vec<T, N + 1> cast_to_homo() const
	{
		vec<T, N + 1> ret;
		for (std::size_t i = 0; i < N; i++)
		{
			ret[i] = M_instance[i];
		}
		ret[N] = 1;
		return ret;
	}

	// Divide by last dim for all dims
	inline vec<T, N - 1> cast_to_non_homo() const
	{
		static_assert(N > 1, "N need to be greater than 1.");
		vec<T, N - 1> ret;
		for (std::size_t i = 0; i < N - 1; i++)
		{
			ret[i] = M_instance[i];
		}
		if (M_instance[N - 1] != 0)
			ret /= M_instance[N - 1];
		return ret;
	}

	// element-wise minimize
	inline vec<T, N>& minimize(const vec<T, N>& right)
	{
		for (std::size_t i = 0; i < N; i++)
		{
			M_instance[i] = std::min(M_instance[i], right.M_instance[i]);
		}
		return *this;
	}

	// element-wise maximize
	inline vec<T, N>& maximize(const vec<T, N>& right)
	{
		for (std::size_t i = 0; i < N; i++)
		{
			M_instance[i] = std::max(M_instance[i], right.M_instance[i]);
		}
		return *this;
	}

	// dimension of this vector
	static inline size_t size() { return N; }
	static inline size_t byte_length() { return N * sizeof(T); }

	// Lazy implementations
	bool all_close(vec<T, 3> other, float margin = 0.0000001f)const
	{
		static_assert(N == 3, "N shall be 3");
		return std::fabs(other.M_instance[0] - this->M_instance[0]) < margin &&
			std::fabs(other.M_instance[1] - this->M_instance[1]) < margin &&
			std::fabs(other.M_instance[2] - this->M_instance[2]) < margin;
	}

	bool identical_in_tolerance(vec<T, 3> rhs, double tolerace = 1E-3)const
	{
		static_assert(N == 3, "N shall be 3");
		return (*this - rhs).norm() < tolerace;
	}

	static inline vec<T, 3> cross(const vec<T, 3>& left, const vec<T, 3>& right)
	{
		vec<T, 3> result;

		result.M_instance[0] = left.M_instance[1] * right.M_instance[2] -
			left.M_instance[2] * right.M_instance[1];
		result.M_instance[1] = left.M_instance[2] * right.M_instance[0] -
			left.M_instance[0] * right.M_instance[2];
		result.M_instance[2] = left.M_instance[0] * right.M_instance[1] -
			left.M_instance[1] * right.M_instance[0];

		return result;
	}
};

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