#include<GL/glew.h>
#include<GLFW/glfw3.h>


#include<iostream>

#include"Vector2D.h"
#include"Rigidbody2D.h"
#include"renderer.h"






int main() {
    // GLFW başlat
    if (!glfwInit()) {
        std::cerr << "GLFW başlatılamadı!\n";
        return -1;
    }
 
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // MacOS için
#endif
    GLFWwindow* window = glfwCreateWindow(1920, 1080, "OpenGL Penceresi", nullptr, nullptr);
    if (!window) {
        std::cerr << "Pencere oluşturulamadı!\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);







   
    Rigidbody2D rigid(1.0f); // 1 kg'lık cisim
    const float gravityScale = 100.0f;
    Vector2D gravity = { 0.0f, -9.81f / gravityScale };
    float deltaTime = 0.016f;

    AABB rigidCol({ 0.0f, 0.0f },{ 0.05f, 0.05f });
    rigid.collider = &rigidCol;

    
    AABB ground({ 0.0f, -0.9f }, { 1.0f, 0.1f });

    Vector2D mtv = {0.0f,0.0f};
  
    while (!glfwWindowShouldClose(window)) {
       
        
        rigid.AddForce(gravity);
        rigid.Update(deltaTime);
       
       
        
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


       if (CollisionDispatcher::CheckCollision(rigid.collider, &ground,mtv)) {
            
            rigid.velocity = { 0.0f, 0.0f };
           
            AABB* player = static_cast<AABB*>(rigid.collider);
            AABB* groundCol = &ground;

            rigid.position.Y = groundCol->Max().Y + player->halfSize.Y;
            rigid.collider->Center = rigid.position;
        }
        
       if (CollisionDispatcher::CheckCollision(rigid.collider, &ground, mtv)) {
           rigid.velocity = {0.0f, 0.0f};
            rigid.position.Y = ground.Max().Y + rigidCol.halfSize.Y;
            rigid.collider->Center = rigid.position;
        
          
        }
          
    
          
        drawSquare(rigid.position, 0.1f);
        glColor3f(1.0f, 0.0f, 0.0f);
        drawSquare(rigid.collider->Center,rigid.collider->halfSize.X * 2.0f);
        glColor3f(1.0f, 0.0f, 0.0f);
       




       







        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
