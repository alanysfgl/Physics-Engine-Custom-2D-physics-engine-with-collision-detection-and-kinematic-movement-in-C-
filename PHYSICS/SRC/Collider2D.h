#pragma once


#include"Vector2D.h"



#include<vector>
#include<iostream>


enum class ColliderType
{AABB, CIRCLE, POLYGON};

class  BaseCollider2D
{
	public:
		Vector2D Center;
        Vector2D halfSize;
		ColliderType type;

		BaseCollider2D(ColliderType t) :type(t) {}
		virtual ~BaseCollider2D() = default;
};

class Polygon : public BaseCollider2D
{
public:
    std::vector<Vector2D> vertices;

    Polygon(const std::vector<Vector2D>& vertice, const Vector2D& center) :BaseCollider2D(ColliderType::POLYGON), vertices(vertice) { this->Center = center; }

    std::vector<Vector2D> getTransformVertices()const {
        std::vector<Vector2D> result;
        for (const auto& v:vertices) {result.push_back(Center + v);}
        return result;}
};

class CIRCLE:public BaseCollider2D
{
public:
    float Radius;
    CIRCLE(float radius, Vector2D center) :BaseCollider2D(ColliderType::CIRCLE), Radius(radius) { this->Center = center; }
};

class AABB : public BaseCollider2D
{
public:
    AABB(Vector2D center, Vector2D halfsize) :BaseCollider2D(ColliderType::AABB)
    { this->Center = center;   this->halfSize = halfsize; }

    Vector2D Max()const { return{ Center + halfSize }; };
    Vector2D Min()const { return { Center - halfSize }; };
};



class CollisionDispatcher {
public:
  static  bool CheckCollision(BaseCollider2D* a, BaseCollider2D* b,Vector2D& outMTV);
};

class Collision {

public:

    // ayný cisismlerin carpýþmasý
    static bool AABBvsAABB(AABB* a, AABB* b) {return (a->Min().X < b->Max().X && a->Max().X > b->Min().X && a->Min().Y < b->Max().Y && a->Max().Y > b->Min().Y);} 
    static bool CirclevsCircle(CIRCLE* a, CIRCLE* b)
    {
        Vector2D distance = a->Center - b->Center;
        Vector2D length;
        float length1=length.VecLength(distance);
        float radiusPlus = a->Radius + b->Radius;
        
        return length1<radiusPlus;
    }
    static bool PoygonVsPolygon(Polygon* a, Polygon* b, Vector2D outMTV)
    {
        auto VertA = a->getTransformVertices();
        auto VertB = b->getTransformVertices();

        auto project = [](const std::vector<Vector2D>& verts, const Vector2D& axis, float& min, float& max) {
            Vector2D math;
            float proj = math.Dot(verts[0], axis);
            min = max = proj;;

            for (size_t i = 1; i < verts.size(); i++)
            {
                float p = math.Dot(verts[i], axis);
                if (p < min) { p = min; }
                if (p > max) { p = max; }
            }
            };


        auto getAxis = [](const std::vector<Vector2D>& verts)
            {
                std::vector<Vector2D> axes;
                Vector2D math;

                for (size_t i = 0; i < verts.size(); i++)
                {
                    Vector2D p1 = verts[i];
                    Vector2D p2 = verts[(i + 1) % verts.size()];
                    Vector2D edge = p2 - p1;

                    Vector2D normal = { -edge.Y, edge.X };
                    normal = math.Normalize(normal);

                    axes.push_back(normal);
                }
                return axes;
            };


        auto axesA = getAxis(VertA);
        auto axesB = getAxis(VertB);
        axesA.insert(axesA.end(), axesB.begin(), axesB.end());

        float smallestOverlap = INFINITY;
        Vector2D smallestAxis = { 0, 0 };

        for (const auto& axis : axesA) {
            float minA, maxA, minB, maxB;
            project(VertA, axis, minA, maxA);
            project(VertB, axis, minB, maxB);

            if (maxA < minB || maxB < minA)
                return false; // ayrým ekseni bulundu, çarpýþma yok

            float overlap = std::min(maxA, maxB) - std::max(minA, minB);
            if (overlap < smallestOverlap) {
                smallestOverlap = overlap;
                smallestAxis = axis;
            }
            outMTV = smallestAxis * smallestOverlap;
        }
        return true;
    }


    // karma cisim carpýþmasý

    static bool AABBvsCIRCLE(AABB* aabb, CIRCLE* circle )
    {
        Vector2D closest;
        
        closest.X = std::max(aabb->Min().X, std::min(circle->Center.X, aabb->Max().X));
        closest.Y = std::max(aabb->Min().Y, std::min(circle->Center.Y, aabb->Max().Y));

        Vector2D distance = circle->Center - closest;
        float length = distance.X * distance.X + distance.Y * distance.Y;
        float Radius = circle->Radius * circle->Radius;

        return length <= Radius;
    }
   

};



bool CollisionDispatcher::CheckCollision(BaseCollider2D* a,BaseCollider2D* b,Vector2D& outMTV) {
    if (a->type == ColliderType::AABB && b->type == ColliderType::AABB) { outMTV = { 0.0f, 0.0f };   
    return Collision::AABBvsAABB(static_cast<AABB*>(a),static_cast<AABB*>(b));}
    if (a->type==ColliderType::CIRCLE && b->type==ColliderType::CIRCLE) { outMTV = { 0.0f, 0.0f };  return Collision::CirclevsCircle(static_cast<CIRCLE*>(a),static_cast<CIRCLE*>(b));}
    if (a->type == ColliderType::POLYGON && b->type == ColliderType::POLYGON) { return Collision::PoygonVsPolygon(static_cast<Polygon*>(a), static_cast<Polygon*>(b),outMTV); }
    if (a->type == ColliderType::AABB && b->type == ColliderType::CIRCLE) { outMTV = { 0.0f, 0.0f };  return Collision::AABBvsCIRCLE(static_cast<AABB*>(a), static_cast<CIRCLE*>(b)); }
    if (a->type == ColliderType::CIRCLE && b->type == ColliderType::AABB) { outMTV = { 0.0f,0.0f }; return Collision::AABBvsCIRCLE(static_cast<AABB*>(b), static_cast<CIRCLE*>(a)); }
  
    return false;
}