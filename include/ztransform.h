#pragma once
#include "zenv.h"

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

class Transform{
	private:
		FBound b;
		FPoint p;
		Anchor a;

		void CalculateTransform();

	public:
		Transform();
		Transform(FBound b);
		Transform(FBound b, Anchor a);

		void setBound(FBound b);
		void setPosition(FPoint p);
		void setAnchor(Anchor a);
		void setScale(float& s);
		void setScale(float nw, float nh);

		const FPoint& getPoint();
		const FBound getBound(bool RealBound = false);
		const Anchor& getAnchor() const;
};