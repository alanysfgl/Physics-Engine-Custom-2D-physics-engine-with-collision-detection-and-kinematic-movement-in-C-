#pragma once
#include<iostream>
 
class Vector2D
{public:
	float X, Y;

	Vector2D() : X(0.0f), Y(0.0f) {}
	Vector2D(float x, float y):X(x),Y(y){}
	Vector2D operator +(const Vector2D& other)const{return { X + other.X, Y + other.Y };}
	Vector2D operator -(const Vector2D& other)const{return { X - other.X, Y - other.Y };}
	Vector2D operator *(const Vector2D& other)const{return { X * other.X, Y * other.Y };}
	Vector2D operator /(const Vector2D& other)const{return { X / other.X, Y / other.Y };}
	
	Vector2D operator*(float scalar) const {return { X * scalar, Y * scalar };}
	Vector2D& operator+=(const Vector2D& other) {X += other.X;   Y += other.Y;  return *this;}
    bool operator==(const Vector2D& other) const {return X == other.X && Y == other.Y;}
	friend std::ostream& operator <<(std::ostream& os, const Vector2D& Nother)
	{os << "(" << Nother.X << "," << Nother.Y << ")";
	return os;}



	float VecLength(Vector2D vec) { return sqrt(vec.X * vec.X + vec.Y * vec.Y); }
	float vecDistance(Vector2D a, Vector2D b) { Vector2D diff = { a.X - b.X, a.Y - b.Y };   return VecLength(diff); }
	Vector2D Normalize(Vector2D vec)
	{
		float length = VecLength(vec);
		if (length == 0.0f) return { 0, 0 }; // Sýfýr vektörse normalize edilemez.
		return vec * (1.0f / length);
	}
	float Dot(Vector2D vecA, Vector2D vecB) { return vecA.X * vecB.X + vecA.Y * vecB.Y; }
	float cross(Vector2D vecA, Vector2D vecB) { return vecA.X * vecB.Y + vecA.Y * vecB.X; }
};
namespace std {
	template <>
	struct hash<Vector2D> {
		std::size_t operator()(const Vector2D& v) const {
			std::size_t hx = std::hash<float>()(v.X);
			std::size_t hy = std::hash<float>()(v.Y);
			return hx ^ (hy << 1); 
		}
	};
}
