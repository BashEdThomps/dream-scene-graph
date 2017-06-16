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

namespace Dream
{
    class Project;
    class Time;
    class AnimationComponent;
    class AudioComponent;
    class Camera;
    class GraphicsComponent;
    class PhysicsComponent;
    class IWindowComponent;
    class LuaEngine;

    class ProjectRuntime
    {
    private:

        bool mDone;
        unique_ptr<Time> mTime;
        unique_ptr<Camera> mCamera;
        unique_ptr<AudioComponent> mAudioComponent;
        unique_ptr<GraphicsComponent> mGraphicsComponent;
        unique_ptr<PhysicsComponent> mPhysicsComponent;
        unique_ptr<AnimationComponent> mAnimationComponent;
        unique_ptr<LuaEngine> mLuaEngine;
        IWindowComponent *mWindowComponentHandle;
        Project* mProjectHandle;

    public:

        ProjectRuntime(Project* proj = nullptr, IWindowComponent* wcHandle = nullptr);
        ~ProjectRuntime();

        void setProjectHandle(Project*);

        void setDone(bool);
        bool isDone();

        Camera* getCamera();
        void setCamera(Camera*);

        void setTime(Time*);
        Time* getTime();

        AnimationComponent* getAnimationComponent();
        AudioComponent* getAudioComponent();
        PhysicsComponent* getPhysicsComponent();
        GraphicsComponent* getGraphicsComponent();
        IWindowComponent* getWindowComponentHandle();
        LuaEngine* getLuaEngine();

        bool initComponents();
        void cleanupComponents(Scene*);

    private:

        bool initAnimationComponent();
        void setAnimationComponent(AnimationComponent*);

        bool initAudioComponent();
        void setAudioComponent(AudioComponent*);

        bool initPhysicsComponent();
        void setPhysicsComponent(PhysicsComponent*);

        bool initGraphicsComponent();
        void setGraphicsComponent(GraphicsComponent*);

        bool initWindowComponent();
        void setWindowComponentHandle(IWindowComponent*);

        bool initLuaEngine();
        void setLuaEngine(LuaEngine*);

        void destroyComponents();
    };

} // End Dream
