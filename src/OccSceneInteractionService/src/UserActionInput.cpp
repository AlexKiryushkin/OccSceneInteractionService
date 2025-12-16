
#include "OccSceneInteractionService/UserActionInput.h"

#include <cassert>

namespace osis
{

UserActionInput::UserActionInput(UserActionInput::ApplyOn applyOn, std::optional<std::size_t> maxInputPoints)
    : m_applyOn{applyOn}
    , m_maxInputPoints{maxInputPoints}
{
    if(maxInputPoints.has_value() && maxInputPoints == 0ULL)
    {
        throw std::runtime_error{"0 points is not permitted"};
    }
}

void UserActionInput::resetUiInput()
{
    m_actionState = ActionState::NotEntered;

    m_uiInputData.toStart = false;
    m_uiInputData.toApply = false;
    m_uiInputData.inputPoints.clear();
}

void UserActionInput::resetRenderInput()
{
    m_renderInputData.toStart = false;
    m_renderInputData.toApply = false;
    m_renderInputData.inputPoints.clear();
}

void UserActionInput::startGatherInput(const Graphic3d_Vec2i &point)
{
    if(m_actionState == ActionState::Started)
    {
        throw std::runtime_error{"Start is called twice"};
    }
    assert(m_uiInputData.inputPoints.empty());

    m_actionState = ActionState::Started;
    m_uiInputData.toStart = true;
    addPoint(point);
}

void UserActionInput::continueGatherInput(const Graphic3d_Vec2i &point)
{
    if(m_actionState != ActionState::Started)
    {
        throw std::runtime_error{"Start was not called"};
    }
    assert(!m_uiInputData.inputPoints.empty());

    addPoint(point);
    if(m_applyOn == ApplyOn::Continue)
    {
        m_uiInputData.toApply = true;
    }
}

void UserActionInput::stopGatherInput(const Graphic3d_Vec2i &point)
{
    if(m_actionState != ActionState::Started)
    {
        throw std::runtime_error{"Start was not called"};
    }
    assert(!m_uiInputData.inputPoints.empty());

    m_actionState = ActionState::Stopped;

    addPoint(point);
    if(m_applyOn == ApplyOn::Stop)
    {
        m_uiInputData.toApply = true;
    }
}

void UserActionInput::sync()
{
    if(m_actionState == ActionState::NotEntered)
    {
        return;
    }

    if(m_uiInputData.toStart)
    {
        m_renderInputData.toStart = true;
        m_uiInputData.toStart = false;
    }

    if(m_uiInputData.toApply)
    {
        m_renderInputData.toApply = true;
        m_uiInputData.toApply = false;
    }

    m_renderInputData.inputPoints = m_uiInputData.inputPoints;

    if(m_actionState == ActionState::Stopped)
    {
        m_uiInputData.inputPoints.clear();
        m_actionState = ActionState::NotEntered;
    }
}

void UserActionInput::addPoint(const Graphic3d_Vec2i &point)
{
    if(!m_maxInputPoints.has_value() || (m_uiInputData.inputPoints.size() < *m_maxInputPoints))
    {
        m_uiInputData.inputPoints.push_back(point);
    }
    else
    {
        m_uiInputData.inputPoints.back() = point;
    }
}

} // namespace osis
