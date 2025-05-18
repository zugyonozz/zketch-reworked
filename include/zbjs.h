#pragma once
#include "zenv.h"
#include "zbj.h"
#include "utils.h"

class zbjs {
private:
    objProp props;
    std::vector<zbj*> items;

public:
    zbjs(Renderer& renderer, objProp& props);
    ~zbjs();

    void addItem(Renderer& renderer, zbj& item);
    void insertItem(Renderer& renderer, zbj& item, size_t& id);
    void removeItem(size_t& id);
    void show(Renderer& renderer);

    const objProp& getProps() const;
    const std::vector<zbj*>& getItems() const;
};