//
// Created by sebas on 03.06.2026.
//

#ifndef CITYBUILDER_BT_SELECTOR_H
#define CITYBUILDER_BT_SELECTOR_H
#include <vector>

#include "bt_composite.h"

namespace core::ai::behaviour_tree {
    class SelectorNode : public CompositeNode {
    public:
        ~SelectorNode() override = default;
        void Reset() override{ currentChild_ = 0; };

        Status Tick() override{
            Status status = children_[currentChild_]->Tick();
            if (status == Status::kFailure) {
                currentChild_++;
                if (currentChild_ >= children_.size()) {
                    Reset();
                    return Status::kFailure;
                }
                return Tick();
            }

            return status;
        }
    };



}

#endif //CITYBUILDER_BT_SELECTOR_H
