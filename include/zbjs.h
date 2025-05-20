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

public:
    zbjs(Renderer& renderer, objProp& props);
    ~zbjs();

    void addItem(Renderer& renderer, zbj&& item);
    void insertItem(Renderer& renderer, std::unique_ptr<zbj> item, size_t id);
    void removeItem(size_t id);
    void show(Renderer& renderer);

    const objProp& getProps() const;
    const std::vector<std::unique_ptr<zbj>>& getItems() const;
};