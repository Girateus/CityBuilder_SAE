//
// Created by sebas on 03.06.2026.
//

#ifndef CITYBUILDER_BT_COMPOSITE_H
#define CITYBUILDER_BT_COMPOSITE_H
#include <vector>

#include "bt_node.h"

namespace core::ai::behaviour_tree {
    class CompositeNode : public Node{

    protected:
        std::vector<std::unique_ptr<Node>> children_;
        int currentChild_ = 0;
    public:
        void AddChild(std::unique_ptr<Node> child){
            children_.push_back(std::move(child));
        }

    };
}

#endif //CITYBUILDER_BT_COMPOSITE_H
