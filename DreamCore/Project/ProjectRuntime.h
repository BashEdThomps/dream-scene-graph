/*
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 */

#pragma once

#include <string>
#include <vector>
#include <deque>

#include "Common/Runtime.h"

using std::string;
using std::vector;
using std::deque;

namespace octronic::dream
{
    // Forward Declarations
    class Project;
    class Transform;
    class AssetDefinition;
    class Time;

    class AudioComponent;
    class InputComponent;
    class GraphicsComponent;
    class WindowComponent;
    class PhysicsComponent;
    class ScriptComponent;
    class TaskManager;
    class StorageManager;

    class SceneRuntime;
    class EntityRuntime;

    class SceneDefinition;

    class AudioCache;
    class MaterialCache;
    class TextureCache;
    class ScriptCache;
    class ModelCache;
    class ShaderCache;
    class FontCache;

    // Class Declaration
    class ProjectRuntime : public Runtime
    {
    public: // Public Variables
        static int MaxFrameCount;

    public: // Public Functions

        ProjectRuntime(Project* parentProject, WindowComponent* wc, AudioComponent* ac, StorageManager* fm);
        ~ProjectRuntime() override;

        void setDone(bool);
        bool isDone() const;

        Time* getTime() const;

        AudioComponent* getAudioComponent() const;
        PhysicsComponent* getPhysicsComponent() const;
        GraphicsComponent* getGraphicsComponent() const;
        WindowComponent* getWindowComponent() const;
        ScriptComponent* getScriptComponent() const;
        InputComponent* getInputComponent() const;
        TaskManager* getTaskManager() const;
        StorageManager* getStorageManager() const;

        Project* getProject() const;

        bool initComponents();

        void collectSceneGarbage(SceneRuntime* rt);
        void collectGarbage() override;

        void updateAll();
        bool updateLogic(SceneRuntime* rt);
        void updateGraphics(SceneRuntime* rt);

        int getWindowWidth() const;
        void setWindowWidth(int);

        int getWindowHeight() const;
        void setWindowHeight(int);

        bool constructSceneRuntime(SceneRuntime* rt);
        void destructSceneRuntime(SceneRuntime* rt, bool clearCaches = false);
        void clearAllCaches();

        bool useDefinition() override;

        AudioCache* getAudioCache() const;
        ShaderCache* getShaderCache() const;
        MaterialCache* getMaterialCache() const;
        ModelCache* getModelCache() const;
        TextureCache* getTextureCache() const;
        ScriptCache* getScriptCache() const;
        FontCache* getFontCache() const;

        bool hasActiveScene() const;

        AssetDefinition* getAssetDefinitionByUuid(UuidType uuid) const;

        EntityRuntime* getEntityRuntimeByUuid(SceneRuntime* rt, UuidType uuid) const;
        SceneRuntime* getActiveSceneRuntime() const;
        SceneRuntime* getSceneRuntimeByUuid(UuidType uuid) const;

        void addSceneRuntime(SceneRuntime*);
        void removeSceneRuntime(SceneRuntime*);
        void setSceneRuntimeAsActive(UuidType uuid);
        vector<SceneRuntime*> getSceneRuntimeVector() const;
        bool hasSceneRuntime(UuidType uuid) const;
        bool hasLoadedScenes() const;

        deque<float> getFrameDurationHistory() const;
        float getAverageFramerate();

    private: // Member Functions
        bool initAudioComponent();
        bool initInputComponent();
        bool initPhysicsComponent();
        bool initGraphicsComponent();
        bool initWindowComponent();
        bool initScriptComponent();
        bool initTaskManager();
        bool initCaches();

        void deleteCaches();
        void deleteComponents();

    private: // Member Variables
        bool mDone;

        Time* mTime;
        Project* mProject;

        // Components
        AudioComponent* mAudioComponent;
        InputComponent* mInputComponent;
        GraphicsComponent* mGraphicsComponent;
        PhysicsComponent* mPhysicsComponent;
        ScriptComponent* mScriptComponent;
        WindowComponent* mWindowComponent;
        TaskManager* mTaskManager;
        StorageManager* mStorageManager;

        // Caches
        AudioCache* mAudioCache;
        TextureCache* mTextureCache;
        MaterialCache* mMaterialCache;
        ModelCache* mModelCache;
        ShaderCache* mShaderCache;
        ScriptCache* mScriptCache;
        FontCache* mFontCache;

        vector<SceneRuntime*> mSceneRuntimeVector;
        vector<SceneRuntime*> mSceneRuntimesToRemove;
        deque<float> mFrameDurationHistory;
    };
}
