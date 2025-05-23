#pragma once
#include <memory>
#include "zenv.h"
#include "zbj.h"
#include "utils.h"
#include "zprop.h"

class zbjs {
private:
    objProp props;
    std::vector<std::unique_ptr<zbj>> items;

	void handleLine(Renderer& renderer);
	void handleRect(Renderer& renderer);
	void handleImage(Renderer& renderer);
	void handleText(Renderer& renderer);
	void handlePolygon(Renderer& renderer);
	void handleTextBox(Renderer& renderer);

public:
    zbjs(Renderer& renderer, objProp& props);
    ~zbjs();

    void addItem(Renderer& renderer, zbj&& item);
    void insertItem(Renderer& renderer, std::unique_ptr<zbj> item, size_t id);
    void removeItem(size_t id);
    void show(Renderer& renderer);
	void showExclude(Renderer& renderer, size_t id);
	void showExclude(Renderer& renderer, std::vector<size_t> id);

    const objProp& getProps() const;
    const std::vector<std::unique_ptr<zbj>>& getItems() const;
};