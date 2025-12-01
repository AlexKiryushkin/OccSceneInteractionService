
#include <OccSceneInteractionService/KeyboardListener.h>

#include <gtest/gtest.h>

namespace osis::test
{

class TestKeyBoardListener : public ::testing::Test
{
};

TEST_F(TestKeyBoardListener, keyPressedSimple)
{
    for(auto key : {Aspect_VKey_Space, Aspect_VKey_M, Aspect_VKey_F})
    {
        KeyboardListener keyboardListener;
        keyboardListener.onKeyPressed(key);

        ASSERT_EQ(keyboardListener.getUiKeyStates().size(), 1ULL);
        EXPECT_EQ(keyboardListener.getUiKeyStates().front().key, key);
        EXPECT_EQ(keyboardListener.getUiKeyStates().front().keyAction, KeyboardListener::KeyAction::Pressed);
    }
}

TEST_F(TestKeyBoardListener, keyPressedMultiple)
{
    std::vector<std::vector<Aspect_VKeyBasic>> pressedKeys{
        std::vector{Aspect_VKey_Space, Aspect_VKey_M}, std::vector{Aspect_VKey_Alt, Aspect_VKey_F, Aspect_VKey_Slash},
        std::vector{Aspect_VKey_Control, Aspect_VKey_R, Aspect_VKey_K, Aspect_VKey_H}};

    for(auto &&currPressedKeys : pressedKeys)
    {
        KeyboardListener keyboardListener;
        for(const auto pressedKey : currPressedKeys)
        {
            keyboardListener.onKeyPressed(pressedKey);
        }

        ASSERT_EQ(keyboardListener.getUiKeyStates().size(), currPressedKeys.size());
        for(std::size_t i{}; i < keyboardListener.getUiKeyStates().size(); ++i)
        {
            EXPECT_EQ(keyboardListener.getUiKeyStates().at(i).key, currPressedKeys.at(i));
            EXPECT_EQ(keyboardListener.getUiKeyStates().at(i).keyAction, KeyboardListener::KeyAction::Pressed);
        }
    }
}

TEST_F(TestKeyBoardListener, keyReleasedSimple)
{
    for(auto key : {Aspect_VKey_Space, Aspect_VKey_M, Aspect_VKey_F})
    {
        KeyboardListener keyboardListener;
        keyboardListener.onKeyReleased(key);

        ASSERT_EQ(keyboardListener.getUiKeyStates().size(), 1ULL);
        EXPECT_EQ(keyboardListener.getUiKeyStates().front().key, key);
        EXPECT_EQ(keyboardListener.getUiKeyStates().front().keyAction, KeyboardListener::KeyAction::Released);
    }
}

TEST_F(TestKeyBoardListener, keyReleasedMultiple)
{
    std::vector<std::vector<Aspect_VKeyBasic>> releasedKeys{
        std::vector{Aspect_VKey_Space, Aspect_VKey_M}, std::vector{Aspect_VKey_Alt, Aspect_VKey_F, Aspect_VKey_Slash},
        std::vector{Aspect_VKey_Control, Aspect_VKey_R, Aspect_VKey_K, Aspect_VKey_H}};

    for(auto &&currReleasedKeys : releasedKeys)
    {
        KeyboardListener keyboardListener;
        for(const auto releasedKey : currReleasedKeys)
        {
            keyboardListener.onKeyReleased(releasedKey);
        }

        ASSERT_EQ(keyboardListener.getUiKeyStates().size(), currReleasedKeys.size());
        for(std::size_t i{}; i < keyboardListener.getUiKeyStates().size(); ++i)
        {
            EXPECT_EQ(keyboardListener.getUiKeyStates().at(i).key, currReleasedKeys.at(i));
            EXPECT_EQ(keyboardListener.getUiKeyStates().at(i).keyAction, KeyboardListener::KeyAction::Released);
        }
    }
}

TEST_F(TestKeyBoardListener, keyPressedAndReleased)
{
    for(auto key : {Aspect_VKey_Space, Aspect_VKey_M, Aspect_VKey_F})
    {
        KeyboardListener keyboardListener;
        keyboardListener.onKeyPressed(key);
        keyboardListener.onKeyReleased(key);

        ASSERT_EQ(keyboardListener.getUiKeyStates().size(), 2ULL);

        EXPECT_EQ(keyboardListener.getUiKeyStates().front().key, key);
        EXPECT_EQ(keyboardListener.getUiKeyStates().front().keyAction, KeyboardListener::KeyAction::Pressed);

        EXPECT_EQ(keyboardListener.getUiKeyStates().back().key, key);
        EXPECT_EQ(keyboardListener.getUiKeyStates().back().keyAction, KeyboardListener::KeyAction::Released);
    }
}

TEST_F(TestKeyBoardListener, syncSimple)
{
    KeyboardListener keyboardListener;
    keyboardListener.onKeyPressed(Aspect_VKey_Space);
    keyboardListener.sync();
    EXPECT_TRUE(keyboardListener.getUiKeyStates().empty());

    ASSERT_EQ(keyboardListener.getRenderKeyStates().size(), 1ULL);
    EXPECT_EQ(keyboardListener.getRenderKeyStates().front().key, Aspect_VKey_Space);
    EXPECT_EQ(keyboardListener.getRenderKeyStates().front().keyAction, KeyboardListener::KeyAction::Pressed);

    keyboardListener.onKeyReleased(Aspect_VKey_M);
    keyboardListener.onKeyPressed(Aspect_VKey_Space);
    keyboardListener.sync();
    EXPECT_TRUE(keyboardListener.getUiKeyStates().empty());

    ASSERT_EQ(keyboardListener.getRenderKeyStates().size(), 2ULL);

    EXPECT_EQ(keyboardListener.getRenderKeyStates().front().key, Aspect_VKey_M);
    EXPECT_EQ(keyboardListener.getRenderKeyStates().front().keyAction, KeyboardListener::KeyAction::Released);

    EXPECT_EQ(keyboardListener.getRenderKeyStates().back().key, Aspect_VKey_Space);
    EXPECT_EQ(keyboardListener.getRenderKeyStates().back().keyAction, KeyboardListener::KeyAction::Pressed);
}

TEST_F(TestKeyBoardListener, clearUiInput)
{
    KeyboardListener keyboardListener;
    keyboardListener.onKeyPressed(Aspect_VKey_Space);
    keyboardListener.clearUiInput();
    ASSERT_TRUE(keyboardListener.getUiKeyStates().empty());

    keyboardListener.onKeyReleased(Aspect_VKey_M);
    keyboardListener.onKeyPressed(Aspect_VKey_Space);
    keyboardListener.clearUiInput();
    ASSERT_TRUE(keyboardListener.getUiKeyStates().empty());

    keyboardListener.onKeyPressed(Aspect_VKey_Control);
    keyboardListener.onKeyPressed(Aspect_VKey_M);
    keyboardListener.onKeyPressed(Aspect_VKey_Space);
    keyboardListener.clearUiInput();
    ASSERT_TRUE(keyboardListener.getUiKeyStates().empty());
}

TEST_F(TestKeyBoardListener, clearRenderInput)
{
    KeyboardListener keyboardListener;
    keyboardListener.onKeyPressed(Aspect_VKey_Space);
    keyboardListener.sync();
    keyboardListener.clearRenderInput();
    ASSERT_TRUE(keyboardListener.getRenderKeyStates().empty());

    keyboardListener.onKeyReleased(Aspect_VKey_M);
    keyboardListener.onKeyPressed(Aspect_VKey_Space);
    keyboardListener.sync();
    keyboardListener.clearRenderInput();
    ASSERT_TRUE(keyboardListener.getRenderKeyStates().empty());

    keyboardListener.onKeyPressed(Aspect_VKey_Control);
    keyboardListener.onKeyPressed(Aspect_VKey_M);
    keyboardListener.onKeyPressed(Aspect_VKey_Space);
    keyboardListener.sync();
    keyboardListener.clearRenderInput();
    ASSERT_TRUE(keyboardListener.getRenderKeyStates().empty());
}

} // namespace osis::test
