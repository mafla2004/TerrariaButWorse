#ifndef __LINALG__
#define __LINALG__

#include <cmath>
#include <memory>

//---------------------------------------------------
//----------            Vector2            ----------
//---------------------------------------------------

class Vector2
{
private:
	double x;
	double y;

public:
	inline constexpr Vector2(double _x = 0, double _y = 0) : x(_x), y(_y) {}
	inline constexpr Vector2(const Vector2& v) : x(v.x), y(v.y) {}
	inline ~Vector2() = default;

	inline Vector2& operator =(const Vector2& v) { x = v.x; y = v.y; return *this; }

	double X() const;
	double Y() const;

	void SetY(double);
	void SetX(double);

	friend Vector2 operator +(const Vector2&, const Vector2&);
	friend Vector2 operator -(const Vector2&, const Vector2&);
	friend Vector2 operator *(const Vector2&, double);
	friend Vector2 operator /(const Vector2&, double);
	friend Vector2 operator *(double, const Vector2&);

	friend double operator *(const Vector2&, const Vector2&);	// Dot product

	friend bool operator ==(const Vector2&, const Vector2&);
	friend bool operator !=(const Vector2&, const Vector2&);

	friend Vector2 operator -(const Vector2&);
	friend Vector2 operator +(const Vector2&);

	Vector2& operator +=(const Vector2&);
	Vector2& operator -=(const Vector2&);
	Vector2& operator *=(double);
	Vector2& operator /=(double);

	double Length() const;
};

inline double Vector2::X() const { return x; }
inline double Vector2::Y() const { return y; }

inline void Vector2::SetX(double _x) { x = _x; }
inline void Vector2::SetY(double _y) { y = _y; }

inline Vector2 operator +(const Vector2& v) { return v; }
inline Vector2 operator -(const Vector2& v) { return Vector2(-v.X(), -v.Y()); }

inline bool operator ==(const Vector2& v1, const Vector2& v2) { return v1.X() == v2.X() && v1.Y() == v2.Y(); }
inline bool operator !=(const Vector2& v1, const Vector2& v2) { return v1.X() != v2.X() || v1.Y() != v2.Y(); }

inline double operator *(const Vector2& v1, const Vector2& v2) { return v1.X() * v2.X() + v1.Y() * v2.Y(); }

inline Vector2 operator +(const Vector2& v1, const Vector2& v2) { return Vector2(v1.X() + v2.X(), v1.Y() + v2.Y()); }
inline Vector2 operator -(const Vector2& v1, const Vector2& v2) { return Vector2(v1.X() - v2.X(), v1.Y() - v2.Y()); }
inline Vector2 operator *(const Vector2& v, double n) { return Vector2(v.X() * n, v.Y() * n); }
inline Vector2 operator /(const Vector2& v, double n) { return Vector2(v.X() / n, v.Y() / n); }
inline Vector2 operator *(double n, const Vector2& v) { return Vector2(v.X() * n, v.Y() * n); }

inline Vector2& Vector2::operator +=(const Vector2& v) { x += v.X(); y += v.Y(); return *this; }
inline Vector2& Vector2::operator -=(const Vector2& v) { x -= v.X(); y -= v.Y(); return *this; }
inline Vector2& Vector2::operator *=(double n) { x *= n; y *= n; return *this; }
inline Vector2& Vector2::operator /=(double n) { x /= n; y /= n; return *this; }

constexpr Vector2 ZERO2D = Vector2();

//---------------------------------------------------
//----------            Vector3            ----------
//---------------------------------------------------

class Vector3
{
private:
	double x;
	double y;
	double z;

public:
	inline constexpr Vector3(double _x = 0.0, double _y = 0.0, double _z = 0.0) : x(_x), y(_y), z(_z) {}
	inline constexpr Vector3(const Vector3& v) : x(v.x), y(v.y), z(v.z) {}
	inline ~Vector3() = default;

	inline Vector3& operator =(const Vector3& v) { x = v.x; y = v.y; z = v.z; return *this; }

	double X() const;
	double Y() const;
	double Z() const;

	void SetX(double);
	void SetY(double);
	void SetZ(double);

	friend Vector3 operator +(const Vector3&, const Vector3&);
	friend Vector3 operator -(const Vector3&, const Vector3&);
	friend Vector3 operator %(const Vector3&, const Vector3&);	// Cross Product
	friend Vector3 operator *(const Vector3&, double);
	friend Vector3 operator /(const Vector3&, double);
	friend Vector3 operator *(double, const Vector3&);

	friend double operator *(const Vector3&, const Vector3&);	// Dot Product

	friend bool operator ==(const Vector3&, const Vector3&);
	friend bool operator !=(const Vector3&, const Vector3&);

	friend Vector3 operator -(const Vector3&);
	friend Vector3 operator +(const Vector3&);

	Vector3& operator +=(const Vector3&);
	Vector3& operator -=(const Vector3&);
	Vector3& operator *=(double);
	Vector3& operator /=(double);

	double Length() const;
};

inline double Vector3::X() const { return x; }
inline double Vector3::Y() const { return y; }
inline double Vector3::Z() const { return z; }

inline void Vector3::SetX(double _x) { x = _x; }
inline void Vector3::SetY(double _y) { y = _y; }
inline void Vector3::SetZ(double _z) { z = _z; }

inline Vector3 operator +(const Vector3& v) { return v; }
inline Vector3 operator -(const Vector3& v) { return Vector3(-v.X(), -v.Y(), -v.Z()); }

inline bool operator ==(const Vector3& v1, const Vector3& v2) { return v1.X() == v2.X() && v1.Y() == v2.Y() && v1.Z() == v2.Z(); }
inline bool operator !=(const Vector3& v1, const Vector3& v2) { return v1.X() != v2.X() || v1.Y() != v2.Y() || v1.Z() != v2.Z(); }

inline double operator *(const Vector3& v1, const Vector3& v2) { return v1.X() * v2.X() + v1.Y() * v2.Y() + v1.Z() * v2.Z(); }

inline Vector3 operator +(const Vector3& v1, const Vector3& v2) { return Vector3(v1.X() + v2.X(), v1.Y() + v2.Y(), v1.Z() + v2.Z()); }
inline Vector3 operator -(const Vector3& v1, const Vector3& v2) { return Vector3(v1.X() - v2.X(), v1.Y() - v2.Y(), v1.Z() - v2.Z()); }
inline Vector3 operator %(const Vector3& v1, const Vector3& v2) { return Vector3(v1.Y() * v2.Z() - v2.Y() * v1.Z(), v1.Z() * v2.X() - v2.Z() * v1.X(), v1.X() * v2.Y() - v2.X() * v1.Y()); }
inline Vector3 operator *(const Vector3& v, double n) { return Vector3(v.X() * n, v.Y() * n, v.Z() * n); }
inline Vector3 operator /(const Vector3& v, double n) { return Vector3(v.X() / n, v.Y() / n, v.Z() / n); }
inline Vector3 operator *(double n, const Vector3& v) { return Vector3(v.X() * n, v.Y() * n, v.Z() * n); }

inline Vector3& Vector3::operator +=(const Vector3& v) { x += v.X(); y += v.Y(); z += v.Z(); return *this; }
inline Vector3& Vector3::operator -=(const Vector3& v) { x -= v.X(); y -= v.Y(); z -= v.Z(); return *this; }
inline Vector3& Vector3::operator *=(double n) { x *= n; y *= n; z *= n; return *this; }
inline Vector3& Vector3::operator /=(double n) { x /= n; y /= n; z /= n; return *this; }

constexpr Vector3 ZERO3D = Vector3();

#endif