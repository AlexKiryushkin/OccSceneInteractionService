
#include "TestsOccSceneInteractionService/TestViewControllerBase.h"

#include <AIS_AttributeFilter.hxx>

namespace osis::test
{

using ::testing::_;

class TestViewControllerKeyboard : public TestViewControllerBase
{
  protected:
    double m_firstTimePress = 10.0;
    double m_secondTimePress = 10.1;
};

// Just checks that we do not crash if there is no key handler. Nothing else to check
TEST_F(TestViewControllerKeyboard, keyDown_noKeyHandler)
{
    getViewController().KeyDown(Aspect_VKey_6, m_firstTimePress, 0.0);
    getViewController().FlushViewEvents(getContext(), getView(), false);
    getViewController().HandleViewEvents(getContext(), getView());
}

// Just checks that we do not crash if there is no key handler. Nothing else to check
TEST_F(TestViewControllerKeyboard, keyUp_noKeyHandler)
{
    getViewController().KeyUp(Aspect_VKey_6, m_firstTimePress);
    getViewController().FlushViewEvents(getContext(), getView(), false);
    getViewController().HandleViewEvents(getContext(), getView());
}

TEST_F(TestViewControllerKeyboard, keyDownSimple_callbackIsCalled)
{
    getViewController().setKeyHandler(getMockKeyHandler());

    for(const auto key : {Aspect_VKey_6, Aspect_VKey_Backspace, Aspect_VKey_Escape})
    {
        getViewController().KeyDown(key, m_firstTimePress, 0.0);
        getViewController().FlushViewEvents(getContext(), getView(), false);

        EXPECT_CALL(*getMockKeyHandler(), handleKeyPressed(key));
        getViewController().HandleViewEvents(getContext(), getView());
    }
}

TEST_F(TestViewControllerKeyboard, keyUpSimple_callbackIsCalled)
{
    getViewController().setKeyHandler(getMockKeyHandler());

    for(const auto key : {Aspect_VKey_6, Aspect_VKey_Backspace, Aspect_VKey_Escape})
    {
        getViewController().KeyUp(key, m_firstTimePress);
        getViewController().FlushViewEvents(getContext(), getView(), false);

        EXPECT_CALL(*getMockKeyHandler(), handleKeyReleased(key));
        getViewController().HandleViewEvents(getContext(), getView());
    }
}

TEST_F(TestViewControllerKeyboard, keyDownSeveral_callbacksAreCalled)
{
    getViewController().setKeyHandler(getMockKeyHandler());

    const std::vector<std::vector<Aspect_VKey>> keySequences{
        std::vector<Aspect_VKey>{Aspect_VKey_6, Aspect_VKey_Backspace},
        std::vector<Aspect_VKey>{Aspect_VKey_H, Aspect_VKey_Control, Aspect_VKey_Left},
        std::vector<Aspect_VKey>{Aspect_VKey_D, Aspect_VKey_Space, Aspect_VKey_Lower, Aspect_VKey_Minus}};
    for(auto &&keys : keySequences)
    {
        for(const auto key : keys)
        {
            getViewController().KeyDown(key, m_firstTimePress, 0.0);
        }
        getViewController().FlushViewEvents(getContext(), getView(), false);

        for(const auto key : keys)
        {
            EXPECT_CALL(*getMockKeyHandler(), handleKeyPressed(key));
        }
        getViewController().HandleViewEvents(getContext(), getView());
    }
}

TEST_F(TestViewControllerKeyboard, keyUpSeveral_callbacksAreCalled)
{
    getViewController().setKeyHandler(getMockKeyHandler());

    const std::vector<std::vector<Aspect_VKey>> keySequences{
        std::vector<Aspect_VKey>{Aspect_VKey_6, Aspect_VKey_Backspace},
        std::vector<Aspect_VKey>{Aspect_VKey_H, Aspect_VKey_Control, Aspect_VKey_Left},
        std::vector<Aspect_VKey>{Aspect_VKey_D, Aspect_VKey_Space, Aspect_VKey_Lower, Aspect_VKey_Minus}};
    for(auto &&keys : keySequences)
    {
        for(const auto key : keys)
        {
            getViewController().KeyUp(key, m_firstTimePress);
        }
        getViewController().FlushViewEvents(getContext(), getView(), false);

        for(const auto key : keys)
        {
            EXPECT_CALL(*getMockKeyHandler(), handleKeyReleased(key));
        }
        getViewController().HandleViewEvents(getContext(), getView());
    }
}

TEST_F(TestViewControllerKeyboard, keyDownAndUpSameFrame_callbackIsCalled)
{
    getViewController().setKeyHandler(getMockKeyHandler());

    for(const auto key : {Aspect_VKey_6, Aspect_VKey_Backspace, Aspect_VKey_Escape})
    {
        getViewController().KeyDown(key, m_firstTimePress, 0.0);
        getViewController().KeyUp(key, m_secondTimePress);
        getViewController().FlushViewEvents(getContext(), getView(), false);

        ::testing::InSequence inSequence;
        EXPECT_CALL(*getMockKeyHandler(), handleKeyPressed(key));
        EXPECT_CALL(*getMockKeyHandler(), handleKeyReleased(key));

        getViewController().HandleViewEvents(getContext(), getView());
    }
}

TEST_F(TestViewControllerKeyboard, keyDownSimple_noCallbackSecondFrame)
{
    getViewController().setKeyHandler(getMockKeyHandler());

    for(const auto key : {Aspect_VKey_6, Aspect_VKey_Backspace, Aspect_VKey_Escape})
    {
        getViewController().KeyDown(key, m_firstTimePress, 0.0);
        getViewController().FlushViewEvents(getContext(), getView(), false);

        EXPECT_CALL(*getMockKeyHandler(), handleKeyPressed(key));
        getViewController().HandleViewEvents(getContext(), getView());

        getViewController().FlushViewEvents(getContext(), getView(), false);

        EXPECT_CALL(*getMockKeyHandler(), handleKeyPressed(_)).Times(0);
        getViewController().HandleViewEvents(getContext(), getView());
    }
}

TEST_F(TestViewControllerKeyboard, keyUpSimple_noCallbackSecondFrame)
{
    getViewController().setKeyHandler(getMockKeyHandler());

    for(const auto key : {Aspect_VKey_6, Aspect_VKey_Backspace, Aspect_VKey_Escape})
    {
        getViewController().KeyUp(key, m_firstTimePress);
        getViewController().FlushViewEvents(getContext(), getView(), false);

        EXPECT_CALL(*getMockKeyHandler(), handleKeyReleased(key));
        getViewController().HandleViewEvents(getContext(), getView());

        getViewController().FlushViewEvents(getContext(), getView(), false);

        EXPECT_CALL(*getMockKeyHandler(), handleKeyReleased(_)).Times(0);
        getViewController().HandleViewEvents(getContext(), getView());
    }
}

} // namespace osis::test
