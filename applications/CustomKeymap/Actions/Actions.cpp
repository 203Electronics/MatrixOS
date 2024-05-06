#include "../UAD.h"

// Actions
#include "midi/MidiAction.h"
#include "keyboard/KeyboardAction.h"
#include "layer/LayerAction.h"
#include "wrap/WrapAction.h"

// Effects
#include "color/ColorEffect.h"


#define TAG "UAD Actions"

bool UAD::ExecuteAction(ActionInfo* actionInfo, cb0r_t actionData, ActionEvent* actionEvent)
{
    if(actionInfo->depth > 5)
    {
        MLOGE(TAG, "Action depth exceeded");
        return false;
    }

    cb0r_s action_index;

    if(!cb0r_get(actionData, 0, &action_index) || action_index.type != CB0R_INT)
    {
        MLOGE(TAG, "Failed to get action index");
        return false;
    }

    uint32_t action_signature = 0;

    if(actionInfo->actionType == ActionType::ACTION)
    {
        action_signature = actionList[action_index.value];
        MLOGV(TAG, "Executing action - %d", action_signature);
    }
    else if(actionInfo->actionType == ActionType::EFFECT)
    {
        action_signature = effectList[action_index.value];
        MLOGV(TAG, "Executing effect - %d", action_signature);
    }


    if(actionEvent->type == ActionEventType::KEYEVENT)
    {
        switch (action_signature)
        {
            case MidiAction::signature:
                return MidiAction::KeyEvent(this, actionInfo, actionData, actionEvent->keyInfo);
            case KeyboardAction::signature:
                return KeyboardAction::KeyEvent(this, actionInfo, actionData, actionEvent->keyInfo);
            case LayerAction::signature:
                return LayerAction::KeyEvent(this, actionInfo, actionData, actionEvent->keyInfo);
            case WrapAction::signature:
                return WrapAction::KeyEvent(this, actionInfo, actionData, actionEvent->keyInfo);
            case ColorEffect::signature:
                return ColorEffect::KeyEvent(this, actionInfo, actionData, actionEvent->keyInfo);
        }
    }
    else if(actionEvent->type == ActionEventType::INITIALIZATION)
    {
        switch (action_signature)
        {
            case ColorEffect::signature:
                ColorEffect::Initialization(this, actionInfo, actionData);
                return true;
        }
    }
    return false;
}