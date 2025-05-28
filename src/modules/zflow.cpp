// zflow.cpp
#include <utility>
#include <tuple>
#include "zflow.h"

zFlow::zFlow(
	const std::pair<std::function<bool()>, 
	std::pair<std::function<void()>, 
	std::function<void()>>>& flow) :
	condition(flow.first),
	funcYes(flow.second.first),
	funcNo(flow.second.second)
{}

void zFlow::operator()() const {
	if (!condition) return;
	
	if (condition()) {
		if (funcYes) funcYes();
	} else {
		if (funcNo) funcNo();
	}
}

void zFlow::execute() const {
	this->operator()();
}

zFlow& zFlow::setCondition(std::function<bool()> cond) {
	condition = cond;
	return *this;
}

zFlow& zFlow::setYesAction(std::function<void()> action) {
	funcYes = action;
	return *this;
}

zFlow& zFlow::setNoAction(std::function<void()> action) {
	funcNo = action;
	return *this;
}

zFlow makeFlow(
	const std::pair<std::function<bool()>, 
	std::pair<std::function<void()>, 
	std::function<void()>>>& flowPair) { 
	return zFlow(flowPair); 
}