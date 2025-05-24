#include "ztransform.h"

void Transform::CalculateTransform(){
	switch(a) {
		case Anchor::ANCHOR_TOP_LEFT:
			p.x = b.x;
			p.y = b.y;
			break;
		case Anchor::ANCHOR_TOP_MID:
			p.x = b.x - b.w / 2;
			p.y = b.y;
			break;
		case Anchor::ANCHOR_TOP_RIGHT:
			p.x = b.x - b.w;
			p.y = b.y;
			break;
		case Anchor::ANCHOR_RIGHT_MID:
			p.x = b.x - b.w;
			p.y = b.y - b.h / 2;
			break;
		case Anchor::ANCHOR_BOT_RIGHT:
			p.x = b.x - b.w;
			p.y = b.y - b.h;
			break;
		case Anchor::ANCHOR_BOT_MID:
			p.x = b.x - b.w / 2;
			p.y = b.y - b.h;
			break;
		case Anchor::ANCHOR_BOT_LEFT:
			p.x = b.x;
			p.y = b.y - b.h;
			break;
		case Anchor::ANCHOR_LEFT_MID:
			p.x = b.x;
			p.y = b.y - b.h / 2;
			break;
		case Anchor::ANCHOR_CENTER:
			p.x = b.x - b.w / 2;
			p.y = b.y - b.h / 2;
			break;
	}
}

Transform::Transform() : b({}), p({}), a(Anchor::ANCHOR_CENTER) {}
Transform::Transform(FBound b) : b(b), p({}), a(Anchor::ANCHOR_CENTER) {}
Transform::Transform(FBound b, Anchor a) : b(b), p({}), a(a) {}

void Transform::setBound(FBound b){
	this->b = b;
}

void Transform::setPosition(FPoint p){
	b.x = p.x;
	b.y = p.y;
}

void Transform::setAnchor(Anchor a){
	this->a = a;
}

void Transform::setScale(float& n){
	b.w *= n;
	b.h *= n;
}

void Transform::setScale(float nw, float nh){
	b.w *= nw;
	b.h *= nh;
}

const FPoint Transform::getPoint() {
	CalculateTransform();
	return p;
}

const FBound Transform::getBound(bool RealBound) {
	CalculateTransform();
	if(RealBound){
		return b;
	}else{
		FBound newBound = b;
		newBound.x = p.x;
		newBound.y = p.y;
		return newBound;
	}
}

const Anchor Transform::getAnchor() const{
	return a;
}