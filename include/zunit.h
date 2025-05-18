#pragma once
#include "zenv.h"
#include <unordered_map>
#include <variant>

enum class Anchor {
    ANCHOR_TOP_LEFT,
    ANCHOR_TOP_MID,
    ANCHOR_TOP_RIGHT,
    ANCHOR_LEFT_MID,
    ANCHOR_BOT_RIGHT,
    ANCHOR_BOT_MID,
    ANCHOR_BOT_LEFT,
    ANCHOR_RIGHT_MID,
    ANCHOR_CENTER
};

enum class objType{
	LINE, RECT, IMAGE, TEXT, POLYGON, BUTTON, INPUT
};

using Propvals = std::variant<float, std::string, std::vector<int>, std::vector<Vertex>, FBound, FPoint, Font>;

struct objProp {
    objType type;
    std::unordered_map<std::string, Propvals> props;

    objProp(objType t);
    const std::unordered_map<std::string, Propvals>& getTemplateForType(objType t);
	template<typename T>
	bool getProp(const std::string& key, T& outVal) const;
};

class Transform{
	private:
		FBound b;
		FPoint p;
		Anchor a;

		void CalculateTransform();

	public:
		Transform() = default;
		Transform(FBound b);
		Transform(FBound b, Anchor a);

		void setBound(FBound b);
		void setPosition(FPoint p);
		void setAnchor(Anchor a);
		void setScale(float& s);
		void setScale(float& nw, float& nh);

		const FPoint& getPoint();
		const FBound& getBound(bool RealBound = false);
		const Anchor& getAnchor() const;
};