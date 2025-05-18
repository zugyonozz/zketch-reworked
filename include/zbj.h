// zbj.h
#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include "zenv.h"
#include "zunit.h"

class zbj {
private:
    Transform transform;
    Color color;
    Texture texture;

public:
	zbj() = default;
    zbj(Transform& transform, Color& color);
    ~zbj();
    
    // commons
    bool draw(Renderer& renderer, FPoint& p1, FPoint& p2);
    bool draw(Renderer& renderer, float radiusScale = 0.0f, bool fill = true);
    bool draw(Renderer& renderer, Font font, const char* text, FPoint& pos);
    bool draw(Renderer& renderer, const char* path);
	bool draw(Renderer& renderer, std::vector<int>& indices, std::vector<Vertex>& vertices);
	bool show(Renderer& renderer);
	bool clear();
    
    // setters
    void setTransform(const FBound& newBound);
	void setTransform(const FPoint& newPosition);
    void setColor(const Color& newColor);
    void setAnchor(Anchor newAnchor);
    
    // Getters
	Transform& getTransform();
    const Texture& getTexture() const;
	const Color& getColor() const;
};