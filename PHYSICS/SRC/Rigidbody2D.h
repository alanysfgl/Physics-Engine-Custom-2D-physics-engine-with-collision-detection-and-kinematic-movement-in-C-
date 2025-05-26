#pragma once

#include"Vector2D.h"
#include"Collider2D.h"

struct PhysicsMaterial {
    float friction;
    float restitution; // 0 = yapýþkan, 1 = tamamen esnek
};

class Rigidbody2D
{
    public:
       
        Vector2D position;
        Vector2D velocity;
        Vector2D acceleration;
        float mass;
        float linearDamping = 0.98f;
        PhysicsMaterial material;

       
        Rigidbody2D(float m = 1.0f):mass(m),position(0.0f, 0.0f),velocity(0.0f, 0.0f),acceleration(0.0f, 0.0f),material() {}
        void AddForce(const Vector2D& force) { acceleration += force * (1.0f / mass); material.friction = 0.5; material.restitution = 0.5f; }
                

        
        
        BaseCollider2D* collider = nullptr;
      

        void Update(float deltaTime) {
            velocity += acceleration * deltaTime;
            velocity = velocity * linearDamping;
            position += velocity * deltaTime;
            acceleration = { 0.0f, 0.0f }; 

            if (collider) {collider->Center = position;}
            if (std::abs(velocity.X) < 0.0001f) velocity.X = 0.0f;
            if (std::abs(velocity.Y) < 0.0001f) velocity.Y = 0.0f;
         }
};


