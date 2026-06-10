//
// Created by sebas on 04.06.2026.
//

#ifndef CITYBUILDER_BT_NODE_FACTORY_H
#define CITYBUILDER_BT_NODE_FACTORY_H
#include <memory>

#include "bt_action.h"
#include "bt_selector.h"
#include "bt_sequence.h"

namespace core::ai::behaviour_tree::node_factory {

    template<std::invocable Taction>
    std::unique_ptr<ActionNode<Taction>> MakeAction(Taction callback_){
        return std::make_unique<ActionNode<Taction>>(callback_);
    }

    inline std::unique_ptr<SequenceNode> MakeSequence(){
        return std::make_unique<SequenceNode>();
    }

    inline std::unique_ptr<SelectorNode> MakeSelector(){
        return std::make_unique<SelectorNode>();
    }
}

#endif //CITYBUILDER_BT_NODE_FACTORY_H
