/*
 * ProjectRuntime.h
 *
 * Created: 05 2017 by Ashley
 *
 * Copyright 2017 Octronic. All rights reserved.
 *
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

#include <memory>
#include <string>
#include <vector>

#include "../Common/IRuntime.h"
#include "../Common/ILoggable.h"

using std::string;
using std::unique_ptr;
using std::vector;

namespace Dream
{
    // Forward Declarations

    class Project;
    class PathComponent;
    class AudioComponent;
    class InputComponent;
    class GraphicsComponent;
    class NanoVGComponent;
    class IWindowComponent;
    class PhysicsComponent;
    class LuaComponent;
    class Transform3D;
    class Camera;
    class SceneRuntime;
    class SceneDefinition;
    class TextureCache;
    class LuaScriptCache;
    class AssimpCache;
    class FontCache;
    class ShaderCache;
    class ComponentThread;

    class Time;

    // Class Declaration
    class ProjectRuntime : public Runtime, ILoggable
    {

    private: // Member Variables
        bool mDone;
        bool mParallel;

        unique_ptr<Time> mTime;
        unique_ptr<Camera> mCamera;

        unique_ptr<AudioComponent> mAudioComponent;
        unique_ptr<ComponentThread> mAudioComponentThread;

        unique_ptr<InputComponent> mInputComponent;
        unique_ptr<ComponentThread> mInputComponentThread;

        unique_ptr<GraphicsComponent> mGraphicsComponent;
        unique_ptr<ComponentThread> mGraphicsComponentThread;

        unique_ptr<PhysicsComponent> mPhysicsComponent;
        unique_ptr<ComponentThread> mPhysicsComponentThread;

        unique_ptr<PathComponent> mPathComponent;
        unique_ptr<ComponentThread> mPathComponentThread;

        unique_ptr<LuaComponent> mLuaComponent;
        unique_ptr<ComponentThread> mLuaComponentThread;

        unique_ptr<NanoVGComponent> mNanoVGComponent;

        // Caches

        unique_ptr<TextureCache> mTextureCache;
        unique_ptr<AssimpCache> mModelCache;
        unique_ptr<FontCache> mFontCache;
        unique_ptr<ShaderCache> mShaderCache;
        unique_ptr<LuaScriptCache> mScriptCache;

        IWindowComponent* mWindowComponentHandle;
        unique_ptr<SceneRuntime> mActiveSceneRuntime;
        Project* mProjectHandle;

    public: // Public Functions
        ProjectRuntime(Project* parentProject, IWindowComponent* wcHandle = nullptr);
        ~ProjectRuntime() override;

        void setDone(bool);
        bool isDone();

        Camera* getCameraHandle();
        Time* getTimeHandle();

        PathComponent* getPathComponentHandle();
        AudioComponent* getAudioComponentHandle();
        PhysicsComponent* getPhysicsComponentHandle();
        GraphicsComponent* getGraphicsComponentHandle();
        NanoVGComponent* getNanoVGComponentHandle();
        IWindowComponent* getWindowComponentHandle();
        LuaComponent* getLuaComponentHandle();
        Project* getProjectHandle();
        InputComponent* getInputComponentHandle();

        bool initComponents();

        void collectGarbage() override;

        void updateAll();
        bool updateLogic();
        void updateGraphics();
        void updateFlush();
        bool allThreadsHaveUpdated();

        int getWindowWidth();
        void setWindowWidth(int);

        int getWindowHeight();
        void setWindowHeight(int);

        SceneRuntime* constructActiveSceneRuntime(SceneDefinition* sceneDefinitionHandle);

        bool hasActiveSceneRuntime();
        SceneRuntime* getActiveSceneRuntimeHandle();
        void resetActiveSceneRuntime();

        void useDefinition(IDefinition*) override;

        FontCache* getFontCacheHandle();
        ShaderCache* getShaderCacheHandle();
        TextureCache* getTextureCacheHandle();
        AssimpCache* getModelCacheHandle();
        bool mGraphicsUpdating;
        bool mLogicUpdating;

        bool getLogicUpdating() const;
        bool getGraphicsUpdating() const;

        void cleanUpThreads();
        bool getParallel() const;
        void setParallel(bool parallel);

    private: // Member Functions
        bool initPathComponent();
        bool initAudioComponent();
        bool initInputComponent();
        bool initPhysicsComponent();
        bool initGraphicsComponent();
        bool initWindowComponent();
        bool initLuaComponent();
        bool initCaches();

        void cleanUpPathComponentThread();
        void cleanUpAudioComponentThread();
        void cleanUpInputComponentThread();
        void cleanUpGraphicsComponentThread();
        void cleanUpLuaComponentThread();
        void cleanUpPhysicsComponentThread();
    };

} // End Dream
