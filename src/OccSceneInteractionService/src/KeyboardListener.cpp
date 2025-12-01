
#include "OccSceneInteractionService/KeyboardListener.h"

namespace osis
{

auto KeyboardListener::getUiKeyStates() const -> const std::vector<KeyState> &
{
    return m_uiKeyStates;
}

auto KeyboardListener::getRenderKeyStates() const -> const std::vector<KeyState> &
{
    return m_renderKeyStates;
}

void KeyboardListener::onKeyPressed(Aspect_VKey key)
{
    m_uiKeyStates.push_back(KeyState{key, KeyAction::Pressed});
}

void KeyboardListener::onKeyReleased(Aspect_VKey key)
{
    m_uiKeyStates.push_back(KeyState{key, KeyAction::Released});
}

void KeyboardListener::sync()
{
    m_renderKeyStates = m_uiKeyStates;

    // clear ui key state in order not to process it next frame
    m_uiKeyStates.clear();
}

void KeyboardListener::clearUiInput()
{
    m_uiKeyStates.clear();
}

void KeyboardListener::clearRenderInput()
{
    m_renderKeyStates.clear();
}

} // namespace osis
