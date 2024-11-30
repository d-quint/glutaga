#define GLEW_STATIC

#include "Enemy.h"
#include "Projectile.h"

#include <cmath> // For sine wave calculations
#include <cstdlib> // For random spawn

Enemy::Enemy() : x(0), y(0), size(0.1f), 
                 speedY(0.0001f + static_cast<float>(rand()) / RAND_MAX * 0.0003f), 
                 swayAmplitude(0.001f + static_cast<float>(rand()) / RAND_MAX * 0.003f), 
                 swayFrequency(4.0f), active(false), 
                 vertices(NULL), colors(NULL), vertexCount(0),
                 deathParticles(100.0f), isDying(false), deathTimer(0.0f) {
    loadSpriteData("enemy.txt");
}

Enemy::~Enemy() {
    delete[] vertices;
    delete[] colors;
    glDeleteBuffers(1, &vboVertices);
    glDeleteBuffers(1, &vboColors);
}

void Enemy::loadSpriteData(const char* filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open sprite file: " << filename << std::endl;
        return;
    }

    std::vector<float> tempVertices;
    std::vector<float> tempColors;

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        float x, y, r, g, b;
        if (ss >> x >> y >> r >> g >> b) {
            tempVertices.push_back(x);
            tempVertices.push_back(y);
            tempColors.push_back(r);
            tempColors.push_back(g);
            tempColors.push_back(b);
        }
    }

    // Calculate vertex count (number of xy pairs)
    vertexCount = tempVertices.size() / 2;

    if (vertexCount * 2 != tempVertices.size() || vertexCount * 3 != tempColors.size()) {
        std::cerr << "Mismatch between vertex count and color count in sprite file" << std::endl;
        return;
    }

    // Allocate arrays
    vertices = new float[vertexCount * 2];  // xy pairs
    colors = new float[vertexCount * 3];    // rgb triplets

    // Copy data into the arrays
    std::copy(tempVertices.begin(), tempVertices.end(), vertices);
    std::copy(tempColors.begin(), tempColors.end(), colors);

    // Initialize the buffers
    initializeBuffers();

    file.close();
}

void Enemy::initializeBuffers() {
    if (!vertices || !colors || vertexCount == 0) {
        std::cerr << "Error: Sprite data is not loaded properly!" << std::endl;
        return;
    }
  
    glGenBuffers(1, &vboVertices);
    glGenBuffers(1, &vboColors);

    glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * 2 * sizeof(float), vertices, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vboColors);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 * sizeof(float), colors, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Enemy::updateVertices() {
    glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
    GLfloat* ptr = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    
    if (ptr) {
        for (size_t i = 0; i < vertexCount; i++) {
            // Update x coordinate
            ptr[i * 2] = vertices[i * 2] * size + x;
            // Update y coordinate
            ptr[i * 2 + 1] = vertices[i * 2 + 1] * size + y;
        }
        
        glUnmapBuffer(GL_ARRAY_BUFFER);
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Enemy::spawn() {
    x = static_cast<float>(rand()) / RAND_MAX * 
        (SCREEN_RIGHT_GAMEPLAY - SCREEN_LEFT_GAMEPLAY) + SCREEN_LEFT_GAMEPLAY;
    y = SCREEN_TOP + size;
    active = true;
    updateVertices();
}

void Enemy::shoot(std::vector<Projectile*>& projectileList) {
    // Don't shoot if inactive or dying
    if (!active || isDying) return;

    for (int i = 0; i < projectileList.size(); i++) {
        if (!projectileList[i]->isActive()) {
            projectileList[i]->spawn(x, y - size);
            break;
        }
    }
}

void Enemy::update(float deltaTime) {
    if (!active && !isDying) return;

    if (isDying) {
        deathTimer -= deltaTime*0.001f;
        deathParticles.update(deltaTime*0.001f);
        
        if (deathTimer <= 0) {
            isDying = false;
            active = false;
        }
        return;
    }

    // Update vertical position
    y -= speedY * deltaTime;

    // Update horizontal position with sine wave motion
    x += swayAmplitude * std::sin(swayFrequency * y);

    // Update vertices based on new position
    updateVertices();

    // Deactivate if it goes below the screen
    if (y < SCREEN_BOTTOM - size) {
        active = false;
    }
}

void Enemy::render() {
    if (!active && !isDying) return;

    // Render death particles first
    if (isDying) {
        deathParticles.render();
    }

    // Only render the enemy if it's not dying
    if (!isDying) {
        glEnableVertexAttribArray(0);
        
        glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, vboColors);
        glColorPointer(3, GL_FLOAT, 0, 0);

        glDrawArrays(GL_QUADS, 0, vertexCount);

        glDisableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

void Enemy::deactivate() {
    active = false;
}

bool Enemy::isActive() const {
    return active;
}

bool Enemy::checkCollision(float projectileX, float projectileY, float projectileWidth, float projectileHeight) {
    if (!active || isDying) return false;

    bool collisionX = x + size > projectileX - projectileWidth/2 && 
                     x - size < projectileX + projectileWidth/2;
    bool collisionY = y + size > projectileY - projectileHeight/2 && 
                     y - size < projectileY + projectileHeight/2;

    if (collisionX && collisionY) {
        startDeathAnimation();
        return true;
    }
    return false;
}

void Enemy::setSize(float newSize) {
    size = newSize;
    updateVertices();
}

void Enemy::startDeathAnimation() {
    if (!isDying) {
        isDying = true;
        deathTimer = DEATH_ANIMATION_TIME;
        deathParticles.emitExplosion(x, y);
    }
}
