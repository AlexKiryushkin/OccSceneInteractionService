
#include "TestsOccSceneInteractionService/MockCustomMouseAction.h"
#include "TestsOccSceneInteractionService/MockInputHandler.h"
#include "TestsOccSceneInteractionService/TestViewControllerBase.h"

#include <OccSceneInteractionService/SceneInteractionEnvironment.h>

#include <AIS_AttributeFilter.hxx>

#include <gmock/gmock.h>

namespace osis::test
{

using ::testing::_;
using ::testing::StrictMock;

class TestViewControllerSceneInteractionEnvironment : public TestViewControllerBase
{
  protected:
    static SelectionEnvironment makeSelectionEnvironment(bool allowRubberBand, bool isNullFilter = false)
    {
        auto filter = isNullFilter ? Handle(SelectMgr_Filter){} : Handle(SelectMgr_Filter){new AIS_AttributeFilter()};

        return SelectionEnvironment{
            allowRubberBand, std::move(filter),
            Handle(StrictMock<MockOwnerHoverListener>){new StrictMock<MockOwnerHoverListener>()}};
    }

    static CameraEnvironment makeCameraEnvironment(bool allowPan, bool allowRotation, bool allowZoom)
    {
        return CameraEnvironment{allowPan, allowRotation, allowZoom,
                                 Handle(StrictMock<MockCameraListener>){new StrictMock<MockCameraListener>()}};
    }

    static Handle(CustomMouseAction) makeCustomAction(unsigned int mouseCombination)
    {
        return {new MockCustomMouseAction(UserActionInput{UserActionInput::ApplyOn::Stop, 3},
                                          std::vector{mouseCombination},
                                          Handle(StrictMock<MockInputHandler>){new StrictMock<MockInputHandler>()})};
    }

    static Handle(IKeyHandler) makeKeyHandler()
    {
        return {new StrictMock<MockKeyHandler>()};
    }

    static Handle(IMouseClickHandler) makeMouseClickHandler()
    {
        return {new StrictMock<MockMouseClickHandler>()};
    }

    static Handle(IMouseHoverListener) makeMouseHoverListener()
    {
        return {new StrictMock<MockMouseHoverListener>()};
    }

    static std::vector<Handle(CustomMouseAction)> makeCustomActions(std::size_t actionsCount)
    {
        std::vector<Handle(CustomMouseAction)> customActions;
        customActions.reserve(actionsCount);

        for(std::size_t i{}; i < actionsCount; ++i)
        {
            customActions.push_back(makeCustomAction(static_cast<unsigned>(i + 1ULL)));
        }

        return customActions;
    }
};

TEST_F(TestViewControllerSceneInteractionEnvironment, setSelectionEnvironment)
{
    const std::vector<SelectionEnvironment> selectionEnvironments{
        makeSelectionEnvironment(true), makeSelectionEnvironment(false), makeSelectionEnvironment(false, true)};
    for(auto &&selectionEnvironment : selectionEnvironments)
    {
        const auto oldSize = getContext()->Filters().Size();
        const auto pOldOwnerHoverListener = getViewController().getOwnerHoverListener();
        getViewController().setSelectionEnvironment(selectionEnvironment);

        EXPECT_EQ(getViewController().toAllowRubberBandSelection(), selectionEnvironment.allowRubberBandSelection);

        // Filters, selections handler and hover listener will be set at sync time
        EXPECT_EQ(getContext()->Filters().Size(), oldSize);
        EXPECT_EQ(getViewController().getOwnerHoverListener(), pOldOwnerHoverListener);

        getViewController().FlushViewEvents(getContext(), getView(), false);

        const auto expectedNewFilterSize = selectionEnvironment.pSelectionFilter ? 1 : 0;
        ASSERT_EQ(getContext()->Filters().Size(), expectedNewFilterSize);
        if(expectedNewFilterSize == 1)
        {
            EXPECT_EQ(getContext()->Filters().First(), selectionEnvironment.pSelectionFilter);
        }

        EXPECT_EQ(getViewController().getOwnerHoverListener(), selectionEnvironment.pOwnerHoverListener);
    }
}

TEST_F(TestViewControllerSceneInteractionEnvironment, setCameraEnvironment)
{
    const std::vector<CameraEnvironment> cameraEnvironments{makeCameraEnvironment(true, false, true),
                                                            makeCameraEnvironment(false, true, false),
                                                            makeCameraEnvironment(false, true, true)};
    for(auto &&cameraEnvironment : cameraEnvironments)
    {
        const auto pOldCameraListener = getViewController().getCameraListener();
        getViewController().setCameraEnvironment(cameraEnvironment);

        EXPECT_EQ(getViewController().ToAllowPanning(), cameraEnvironment.allowPan);
        EXPECT_EQ(getViewController().ToAllowRotation(), cameraEnvironment.allowRotation);
        EXPECT_EQ(getViewController().ToAllowZooming(), cameraEnvironment.allowZoom);

        // Camera listener will be set at sync time
        EXPECT_EQ(getViewController().getCameraListener(), pOldCameraListener);

        getViewController().FlushViewEvents(getContext(), getView(), false);
        EXPECT_EQ(getViewController().getCameraListener(), cameraEnvironment.pCameraListener);
    }
}

TEST_F(TestViewControllerSceneInteractionEnvironment, setKeyHandler)
{
    const auto keyHandlers = {makeKeyHandler(), makeKeyHandler(), makeKeyHandler()};

    for(auto &&pKeyHandler : keyHandlers)
    {
        const auto oldKeyHandler = getViewController().getKeyHandler();
        getViewController().setKeyHandler(pKeyHandler);

        // Key handler will be set at sync time
        EXPECT_EQ(getViewController().getKeyHandler(), oldKeyHandler);

        getViewController().FlushViewEvents(getContext(), getView(), false);
        EXPECT_EQ(getViewController().getKeyHandler(), pKeyHandler);
    }
}

TEST_F(TestViewControllerSceneInteractionEnvironment, setMouseClickHandler)
{
    const auto mouseClickHandlers = {makeMouseClickHandler(), makeMouseClickHandler(), makeMouseClickHandler()};

    for(auto &&pMouseClickHandler : mouseClickHandlers)
    {
        const auto oldMouseClickHandler = getViewController().getMouseClickHandler();
        getViewController().setMouseClickHandler(pMouseClickHandler);

        // Mouse click handler will be set at sync time
        EXPECT_EQ(getViewController().getMouseClickHandler(), oldMouseClickHandler);

        getViewController().FlushViewEvents(getContext(), getView(), false);
        EXPECT_EQ(getViewController().getMouseClickHandler(), pMouseClickHandler);
    }
}

TEST_F(TestViewControllerSceneInteractionEnvironment, setMouseHoverListener)
{
    const auto mouseHoverListeners = {makeMouseHoverListener(), makeMouseHoverListener(), makeMouseHoverListener()};

    for(auto &&pMouseHoverListener : mouseHoverListeners)
    {
        const auto oldMouseHoverListener = getViewController().getMouseHoverListener();
        getViewController().setMouseHoverListener(pMouseHoverListener);

        // Mouse hover listener will be set at sync time
        EXPECT_EQ(getViewController().getMouseHoverListener(), oldMouseHoverListener);

        getViewController().FlushViewEvents(getContext(), getView(), false);
        EXPECT_EQ(getViewController().getMouseHoverListener(), pMouseHoverListener);
    }
}

TEST_F(TestViewControllerSceneInteractionEnvironment, setCustomActions)
{
    const std::vector<std::vector<Handle(CustomMouseAction)>> customActions{
        makeCustomActions(1ULL), makeCustomActions(2ULL), makeCustomActions(3ULL), makeCustomActions(1ULL)};
    for(auto &&currCustomActions : customActions)
    {
        const auto oldCustomActions = getViewController().getCustomActions();
        const auto oldGesturesCount = getViewController().MouseGestureMap().Size();

        getViewController().setCustomMouseActions(currCustomActions);

        // Actions will be set at sync time
        EXPECT_EQ(getViewController().getCustomActions(), oldCustomActions);
        EXPECT_EQ(getViewController().MouseGestureMap().Size(), oldGesturesCount);

        ::testing::InSequence seq;
        for(auto &&pOldCustomAction : oldCustomActions)
        {
            EXPECT_CALL(dynamic_cast<MockCustomMouseAction &>(*pOldCustomAction), tearDown());
        }

        for(auto &&pOldCustomAction : currCustomActions)
        {
            EXPECT_CALL(dynamic_cast<MockCustomMouseAction &>(*pOldCustomAction), setUp());
        }

        getViewController().FlushViewEvents(getContext(), getView(), false);
        EXPECT_EQ(getViewController().getCustomActions(), currCustomActions);

        const auto expectedGestureCount = oldGesturesCount + currCustomActions.size() - oldCustomActions.size();
        EXPECT_EQ(getViewController().MouseGestureMap().Size(), expectedGestureCount);
    }
}

TEST_F(TestViewControllerSceneInteractionEnvironment, setSceneInteractionEnvironment)
{
    auto sceneInteractionEnvironments = {
        SceneInteractionEnvironment{makeSelectionEnvironment(true), makeCameraEnvironment(true, false, true),
                                    makeKeyHandler(), makeMouseClickHandler(), makeMouseHoverListener(),
                                    makeCustomActions(2)},
        SceneInteractionEnvironment{makeSelectionEnvironment(false), makeCameraEnvironment(false, true, false),
                                    makeKeyHandler(), makeMouseClickHandler(), makeMouseHoverListener(),
                                    makeCustomActions(3)},
        SceneInteractionEnvironment{makeSelectionEnvironment(true), makeCameraEnvironment(true, true, false),
                                    makeKeyHandler(), makeMouseClickHandler(), makeMouseHoverListener(),
                                    makeCustomActions(4)}};

    for(auto &&sceneInteractionEnvironment : sceneInteractionEnvironments)
    {
        auto &&[selectionEnvironment, cameraEnvironment, pKeyHandler, pMouseClickHandler, pMouseHoverListener,
                customActions] = sceneInteractionEnvironment;

        const auto oldSize = getContext()->Filters().Size();
        const auto pOldOwnerHoverListener = getViewController().getOwnerHoverListener();
        const auto pOldCameraListener = getViewController().getCameraListener();
        const auto pOldKeyHandler = getViewController().getKeyHandler();
        const auto pOldMouseClickHandler = getViewController().getMouseClickHandler();
        const auto pOldMouseHoverListener = getViewController().getMouseHoverListener();
        const auto oldCustomActions = getViewController().getCustomActions();

        getViewController().setSceneInteractionEnvironment(sceneInteractionEnvironment);

        EXPECT_EQ(getViewController().ToAllowPanning(), cameraEnvironment.allowPan);
        EXPECT_EQ(getViewController().ToAllowRotation(), cameraEnvironment.allowRotation);
        EXPECT_EQ(getViewController().ToAllowZooming(), cameraEnvironment.allowZoom);
        EXPECT_EQ(getViewController().toAllowRubberBandSelection(), selectionEnvironment.allowRubberBandSelection);

        // Render environment will be set at sync time
        EXPECT_EQ(getContext()->Filters().Size(), oldSize);
        EXPECT_EQ(getViewController().getOwnerHoverListener(), pOldOwnerHoverListener);
        EXPECT_EQ(getViewController().getCameraListener(), pOldCameraListener);
        EXPECT_EQ(getViewController().getKeyHandler(), pOldKeyHandler);
        EXPECT_EQ(getViewController().getMouseClickHandler(), pOldMouseClickHandler);
        EXPECT_EQ(getViewController().getMouseHoverListener(), pOldMouseHoverListener);
        EXPECT_EQ(getViewController().getCustomActions(), oldCustomActions);

        ::testing::InSequence seq;
        for(auto &&pOldCustomAction : oldCustomActions)
        {
            EXPECT_CALL(dynamic_cast<MockCustomMouseAction &>(*pOldCustomAction), tearDown());
        }

        for(auto &&pOldCustomAction : customActions)
        {
            EXPECT_CALL(dynamic_cast<MockCustomMouseAction &>(*pOldCustomAction), setUp());
        }

        getViewController().FlushViewEvents(getContext(), getView(), false);
        ASSERT_EQ(getContext()->Filters().Size(), 1);
        EXPECT_EQ(getContext()->Filters().First(), selectionEnvironment.pSelectionFilter);
        EXPECT_EQ(getViewController().getOwnerHoverListener(), selectionEnvironment.pOwnerHoverListener);
        EXPECT_EQ(getViewController().getCameraListener(), cameraEnvironment.pCameraListener);
        EXPECT_EQ(getViewController().getKeyHandler(), pKeyHandler);
        EXPECT_EQ(getViewController().getMouseClickHandler(), pMouseClickHandler);
        EXPECT_EQ(getViewController().getMouseHoverListener(), pMouseHoverListener);
        EXPECT_EQ(getViewController().getCustomActions(), customActions);
    }
}

} // namespace osis::test
