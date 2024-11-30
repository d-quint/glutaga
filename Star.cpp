#define GLEW_STATIC

#include "Star.h"

Star::Star() {	
    x = static_cast<float>(rand()) / RAND_MAX * 
        (SCREEN_RIGHT_GAMEPLAY - SCREEN_LEFT_GAMEPLAY) + SCREEN_LEFT_GAMEPLAY;
    y = static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f;

    // Base opacity between 0.3 to 0.5
    baseAlpha = 0.3f + (static_cast<float>(rand()) / RAND_MAX * 0.2f);
    currentAlpha = baseAlpha;
    size = 15.0f * baseAlpha * baseAlpha;
    
    // Speed directly proportional to base opacity
    speedY = -0.2f - (baseAlpha * 2.0f);  // Range from -0.6 to -1.8

    glGenBuffers(1, &vboVertices);
    glGenBuffers(1, &vboColors);

	GLfloat vertices[2] = {x, y};
    glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    GLfloat colors[4] = {1.0f, 1.0f, 1.0f, currentAlpha};
    glBindBuffer(GL_ARRAY_BUFFER, vboColors);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Star::~Star() {
    glDeleteBuffers(1, &vboVertices);
    glDeleteBuffers(1, &vboColors);
}

void Star::update(float deltaTime) {
    // Move based on constant speed
    y += speedY * deltaTime;
    
    // Add random twinkle effect to current alpha
    float twinkleOffset = (static_cast<float>(rand()) / RAND_MAX * 0.4f) - 0.2f;  // -0.2 to +0.2
    currentAlpha = baseAlpha + twinkleOffset;

    // Update color buffer with new alpha
    float colors[4] = {1.0f, 1.0f, 1.0f, currentAlpha};
    glBindBuffer(GL_ARRAY_BUFFER, vboColors);
    
    GLfloat* ptr = static_cast<GLfloat*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));

    if (ptr) {
        memcpy(ptr, colors, sizeof(colors));
        glUnmapBuffer(GL_ARRAY_BUFFER);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Reset position if off screen, using the full gameplay width
    if (y < -1.0f) {
        y = 1.0f;
        x = static_cast<float>(rand()) / RAND_MAX * 
            (SCREEN_RIGHT_GAMEPLAY - SCREEN_LEFT_GAMEPLAY) + SCREEN_LEFT_GAMEPLAY;
    }
}

void Star::render() {    
    glPointSize(size);

    glPushMatrix();
    
    glTranslatef(x, y, 0.0f);

    glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, vboColors);
    glColorPointer(4, GL_FLOAT, 0, 0);

    glDrawArrays(GL_POINTS, 0, 1);
    
    glPopMatrix();
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
} 
