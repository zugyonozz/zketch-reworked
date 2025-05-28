// zbj.h
#pragma once
#include "zfont.h"
#include "ztransform.h"

class zbj {
private:
    Transform transform;
    Color color;
    Texture texture;

public:
// Constructer of zbj
	zbj() = default;
// Constructer of zbj
// `Parameter`
// - `tranform`
// Transform is an object containing { Found, Point, Anchor }.
// - `color`
// Color is an object containing { r, g, b, a }.
    zbj(const Transform& transform, const Color& color);

// Move constructor
	zbj(zbj&& other) noexcept;

// Move assignment operator
	zbj& operator=(zbj&& other) noexcept;

// Destructor of zbj
    ~zbj();
    
// Common methods

// Draw line object.
// `Parameters`
// - `renderer`
// renderer is member of zmain object.
// - `p1`
// initial drawing position.
// - `p2`
// final drawing position
//
// `Return`
// - `bool`
// If success create object return value is True, if not return value is False.
    bool draw(Renderer& renderer, const FPoint& p1, const FPoint& p2);

// Draw rectangle object.
// `Parameters`
// - `renderer`
// renderer is member of zmain object.
// - `radiusScale`
// roundness value of the rectangle, default value is 0.
//
// `Return`
// - `bool`
// If success create object return value is True, if not return value is False.
    bool draw(Renderer& renderer, float radiusScale = 0.0f);

// Draw text object.
//
// `Parameters`
// - `renderer`
// renderer is member of zmain object.
// - `font`
// font is data of Font object.
// - `text`
// text to be drawn.
// - `pos`
// position of text.
//
// `Return`
// - `bool`
// If success create object return value is True, if not return value is False.
    bool draw(Renderer& renderer, Font font, const char* text, const FPoint& pos);

// Draw image object.
//
// `Parameters`
// - `renderer`
// renderer is member of zmain object.
// - `path`
// path of file image will be shown.
//
// `Return`
// - `bool`
// If success create object return value is True, if not return value is False.
    bool draw(Renderer& renderer, const char* path);

// Draw polygon object.
//
// `Parameters`
// - `renderer`
// renderer is member of zmain object.
// - `indices`
// index of vertices, to control the order of vertices.
// - `vertices`
// list of vertex, contain positions, colors, and positions texture data.
//
// `Return`
// - `bool`
// If success create object return value is True, if not return value is False.
	bool draw(Renderer& renderer, std::vector<int>& indices, std::vector<Vertex>& vertices);

// Function to display objects that have been drawn.
//
// `Parameter`
// - `renderer`
// renderer is member of zmain object.
//
// `Return`
// - `bool`
// If success to show object return value is True, if not return value is False.
	bool show(Renderer& renderer);

// Function to display with a certain scope of the drawn object.
//
// `Parameters`
// - `renderer`
// renderer is a member of the zmain object.
// - `srcRect`
// scope of the drawn object
//
// `Return`
// - `bool`
// If success to show object return value is True, if not return value is False.
	bool show(Renderer& renderer, FBound& srcRect);

// Funtion to reset all object properties.
//
// `Return`
// - `bool`
// If success to show object return value is True, if not return value is False.
	bool reset();

// Function to clear one of object from zbj
// `Parameter`
// - `property`
// Existing property formats are "Texture", "Color", "Transform".
//
// `Return`
// - `bool`
// If success to show object return value is True, if not return value is False.
	bool clear(std::string_view property);
    
// Setter methods

// Function to set the opacity level of an object.
//
// `Parameter`
// - `a`
// a is alpha of object
// `Return`
// - `bool`
// If success return value is True, if not return value is False.
	bool setOpacity(Uint8 a);

// Function to set boundary.
//
// `Parameter`
// - `newBound`
// New boundary will replace the current boundary.
    void setTransform(const FBound& newBound);

// Function to set position.
//
// `Parameter`
// - `newPosition`
// New position will replace the current position.
	void setTransform(const FPoint& newPosition);

// Function to set color.
//
// `Parameter`
// - `newColor`
// New color will replace the current color.
    void setColor(const Color& newColor);

// Function to set anchor point.
//
// `Parameter`
// - `newAnchor`
// New anchor point will replace the current anchor point.
    void setAnchor(Anchor newAnchor);
    
// Getter methods

// Funtion to get data of object transform.
//
// `Return`
// - `Transform`
// Object to control tranform object will be shown.
	Transform& getTransform();

// Funtion to get data of object texture.
//
// `Return`
// - `Texture`
// Object to used for storing object will be shown.
    Texture& getTexture();

// Funtion to get data of object Color.
//
// `Return`
// - `Color`
// Object to used for storing color of object will be shown.
	Color& getColor();
};