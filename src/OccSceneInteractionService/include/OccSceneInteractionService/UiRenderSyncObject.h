
#ifndef OCC_SCENE_INTERACTION_SERVICE_UI_RENDER_SYNC_OBJECT_H
#define OCC_SCENE_INTERACTION_SERVICE_UI_RENDER_SYNC_OBJECT_H

namespace osis
{

/**
 * @brief This is a general class for data should be shared between ui and render threads and should always be in sync.
 * This class provides methods to set the value in UI thread and sync this value in synchronization time when UI and
 * Render threads are synced.
 * Also there is a possibility to reset the render data. It is needed in case you want to use the render data only once.
 * @tparam T Type of data. Should be default constructible, copy or move constructible and assignable, equality comparable.
 */
template <class T>
class UiRenderSyncObject
{
  public:
    UiRenderSyncObject() = default;

    /**
     * @brief Sets UI data. Should be called in UI thread.
     * @param data data to be set.
     */
    void setUiData(T data)
    {
        uiData = std::move(data);
        wasChanged = true;
    }

    /**
     * @brief Synchronizes UI and Render data. Should be called at sync stage between UI and Render threads.
     * @return true if render data was changed.
     */
    bool sync()
    {
        if(wasChanged)
        {
            wasChanged = false;

            if(renderData == uiData)
            {
                return false;
            }

            renderData = uiData;
            return true;
        }

        return false;
    }

    /**
     * @brief Returns UI data. Should be called in UI thread.
     * @return UI data.
     */
    const T &getUiData() const { return uiData; }

    /**
     * @brief Returns Render data. Should be called in Render thread.
     * @return Render data.
     */
    const T &getRenderData() const { return renderData; }

    /**
     * @brief Resets render data. Should be called in Render thread.
     * @note It is usually used when render data should be used only once.
     */
    void resetRenderData() { renderData = {}; }

  private:
    T uiData = {};
    T renderData = {};
    bool wasChanged = false;
};

} // namespace osis

#endif // OCC_SCENE_INTERACTION_SERVICE_UI_RENDER_SYNC_OBJECT_H
