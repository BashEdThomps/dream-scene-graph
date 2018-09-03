/*
 * PhysicsComponent
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <vector>
#include <memory>

#include "../IComponent.h"
#include <LinearMath/btVector3.h>
#include <glm/matrix.hpp>

using glm::mat4;
using std::vector;
using std::unique_ptr;

class btDynamicsWorld;
class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btBroadphaseInterface;
class btSequentialImpulseConstraintSolver;
class btVector3;
class btRigidBody;
class btCollisionObject;

namespace Dream
{
    class PhysicsDebugDrawer;
    class PhysicsObjectInstance;
    class SceneRuntime;
    class SceneObjectRuntime;

    class PhysicsComponent : public IComponent
    {
    protected:
        shared_ptr<PhysicsDebugDrawer> mDebugDrawer;
        btDynamicsWorld *mDynamicsWorld;
        btBroadphaseInterface *mBroadphase;
        btDefaultCollisionConfiguration *mCollisionConfiguration;
        btCollisionDispatcher *mDispatcher;
        btSequentialImpulseConstraintSolver *mSolver;
        mat4 mProjectionMatrix;
        btVector3 mGravity;
        bool mDebug;
    public:
        PhysicsComponent();
        ~PhysicsComponent();
        void populatePhysicsWorld(shared_ptr<SceneRuntime> scene);
        void setGravity(vector<float>);
        void setDebug(bool);
        bool init() override;
        void updateComponent() override;
        void addPhysicsObjectInstance(shared_ptr<PhysicsObjectInstance>);
        void addRigidBody(btRigidBody*);
        void removeRigidBody(btRigidBody*);
        void removePhysicsObjectInstance(shared_ptr<PhysicsObjectInstance>);
        void setViewProjectionMatrix(mat4, mat4);
        void checkContactManifolds(shared_ptr<SceneRuntime> scene);
        shared_ptr<SceneObjectRuntime> getSceneObjectRuntime(shared_ptr<SceneRuntime> scene, const btCollisionObject*);
        void drawDebug();
    };// End of PhysicsComponent

} // End of Dream
