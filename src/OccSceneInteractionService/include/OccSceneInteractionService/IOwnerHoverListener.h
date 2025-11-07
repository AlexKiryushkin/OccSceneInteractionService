
#ifndef OCC_SCENE_INTERACTION_SERVICE_I_OWNER_HOVER_LISTENER_H
#define OCC_SCENE_INTERACTION_SERVICE_I_OWNER_HOVER_LISTENER_H

#include <SelectMgr_EntityOwner.hxx>
#include <Standard_Handle.hxx>
#include <Standard_Transient.hxx>
#include <Standard_Type.hxx>

namespace osis
{

/**
 * @brief An interface that allow to perform additional actions when an owner starts/stops to be hovered (dynamically
 * highlighted).
 */
class IOwnerHoverListener : public Standard_Transient
{
  public:
    /**
     * @brief Macro for OCC RTTI
     */
    DEFINE_STANDARD_RTTI_INLINE(IOwnerHoverListener, Standard_Transient)

  public:
    IOwnerHoverListener() = default;

    IOwnerHoverListener(const IOwnerHoverListener &) = default;
    IOwnerHoverListener(IOwnerHoverListener &&) = default;

    IOwnerHoverListener &operator=(const IOwnerHoverListener &) = default;
    IOwnerHoverListener &operator=(IOwnerHoverListener &&) = default;

    ~IOwnerHoverListener() override = default;

    /**
     * @brief Performs an action when owner starts to be hovered. Is called from Render thread.
     * @param pSelectedOwner Owner that starts to be hovered.
     */
    virtual void handleOwnerStartHovered(Handle(SelectMgr_EntityOwner) pSelectedOwner) = 0;

    /**
     * @brief Performs an action when owner stops to be hovered. Is called from Render thread.
     * @param pSelectedOwner Owner that stops to be hovered.
     */
    virtual void handleOwnerStopHovered(Handle(SelectMgr_EntityOwner) pSelectedOwner) = 0;
};

} // namespace osis

#endif // OCC_SCENE_INTERACTION_SERVICE_I_OWNER_HOVER_LISTENER_H
