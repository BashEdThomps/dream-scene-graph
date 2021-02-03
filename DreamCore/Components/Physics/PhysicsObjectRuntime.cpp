/*
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

#include <iostream>

#include "PhysicsObjectRuntime.h"

#include "PhysicsMotionState.h"
#include "PhysicsComponent.h"
#include "PhysicsObjectDefinition.h"
#include "PhysicsTasks.h"

#include "Common/Constants.h"
#include "Components/Graphics/Model/ModelCache.h"
#include "Components/Graphics/Model/ModelRuntime.h"
#include "Components/Graphics/Model/ModelMesh.h"
#include "Scene/SceneRuntime.h"
#include "Scene/Entity/EntityRuntime.h"
#include "Project/ProjectDefinition.h"
#include "Project/ProjectRuntime.h"

using std::unique_lock;

namespace octronic::dream
{
    PhysicsObjectRuntime::PhysicsObjectRuntime
    (
        PhysicsObjectDefinition* definition,
        PhysicsComponent* comp,
        ModelCache* modelCache,
        EntityRuntime* transform)
        : DiscreteAssetRuntime("PhysicsObjectRuntime",definition,transform),
         mCollisionShape(nullptr),
         mMotionState(nullptr),
         mRigidBody(nullptr),
         mRigidBodyConstructionInfo(nullptr),
         mInPhysicsWorld(false),
         mPhysicsComponent(comp),
         mModelCache(modelCache),
         mAddObjectTask(mPhysicsComponent,this)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        LOG_TRACE( "PhysicsObjectRuntime: Constructing" );
    }

    PhysicsObjectRuntime::~PhysicsObjectRuntime
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        LOG_TRACE( "PhysicsObjectRuntime: Destroying" );

        /***** Deletes are handled by PhysicsComponent! *****/

        if (mRigidBody != nullptr)
        {
            delete mRigidBody;
            mRigidBody = nullptr;
        }

        if (mMotionState != nullptr)
        {
            delete mMotionState;
            mMotionState = nullptr;
        }

        if (mCollisionShape != nullptr)
        {
            delete mCollisionShape;
            mCollisionShape = nullptr;
        }

        if (mRigidBodyConstructionInfo != nullptr)
        {
            delete mRigidBodyConstructionInfo;
            mRigidBodyConstructionInfo = nullptr;
        }
    }

    btCollisionShape*
    PhysicsObjectRuntime::getCollisionShape
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        return mCollisionShape;
    }

    // This has a leak when reusing Physics Object Shapes
    bool
    PhysicsObjectRuntime::useDefinition
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        auto pod = static_cast<PhysicsObjectDefinition*>(mDefinition);
        mCollisionShape = createCollisionShape(pod);
        if (!mCollisionShape)
        {
            LOG_ERROR( "PhysicsObjectRuntime: Unable to create collision shape" );
            return false;
        }
        float mass = mDefinition->getJson()[Constants::ASSET_ATTR_MASS];
        // Transform and CentreOfMass
        mMotionState = new PhysicsMotionState(&mEntityRuntime->getTransform());
        // Mass, MotionState, Shape and LocalInertia
        btVector3 inertia(0, 0, 0);
        mCollisionShape->calculateLocalInertia(mass, inertia);
        mRigidBodyConstructionInfo = new btRigidBody::btRigidBodyConstructionInfo
                (
                    btScalar(mass),
                    mMotionState,
                    mCollisionShape,
                    inertia
                    );

        mRigidBody = new btRigidBody(*mRigidBodyConstructionInfo);

        Vector3 lf, lv, af, av;
        lf = pod->getLinearFactor();
        lv = pod->getLinearVelocity();
        af = pod->getAngularFactor();
        av = pod->getAngularVelocity();

        setLinearFactor(lf.x(),lf.y(),lf.z());
        setLinearVelocity(lv.x(),lv.y(),lv.z());
        setAngularFactor(af.x(),af.y(),af.z());
        setAngularVelocity(av.x(),av.y(),av.z());

       if (pod->getControllableCharacter())
       {
           setCameraControllableCharacter();
       }

       if (pod->getKinematic())
       {
           setKinematic(true);
       }

        setRestitution(pod->getRestitution());
        setFriction(pod->getFriction());
        setCcdSweptSphereRadius(pod->getCcdSweptSphereRadius());

        mLoaded = (mRigidBody != nullptr);
        return mLoaded;
    }

     void
     PhysicsObjectRuntime::setCameraControllableCharacter
     ()
     {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mRigidBody->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
        mRigidBody->setActivationState(DISABLE_DEACTIVATION);
        mRigidBody->setAngularFactor(btVector3(0,1,0));
     }


    void
    PhysicsObjectRuntime::setKinematic
    (bool setKenematic)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        if (setKenematic)
        {
            mRigidBody->setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT);
            mRigidBody->setActivationState(DISABLE_DEACTIVATION);
        }
    }

    PhysicsAddObjectTask*
    PhysicsObjectRuntime::getAddObjectTask()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
       return &mAddObjectTask;
    }

    btCollisionShape*
    PhysicsObjectRuntime::createCollisionShape
    (PhysicsObjectDefinition* pod)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        string format = pod->getFormat();
        btCollisionShape *collisionShape = nullptr;

        if (format == Constants::COLLISION_SHAPE_SPHERE)
        {
            btScalar radius = pod->getRadius();
            collisionShape = new btSphereShape(radius);
        }
        else if (format == Constants::COLLISION_SHAPE_BOX)
        {
            btScalar boxX, boxY, boxZ;
            boxX = pod->getHalfExtentsX();
            boxY = pod->getHalfExtentsY();
            boxZ = pod->getHalfExtentsZ();
            collisionShape = new btBoxShape(btVector3(boxX,boxY,boxZ));
        }
        else if (format == Constants::COLLISION_SHAPE_CYLINDER)
        {
            btScalar boxX, boxY, boxZ;
            boxX = pod->getHalfExtentsX();
            boxY = pod->getHalfExtentsY();
            boxZ = pod->getHalfExtentsZ();
            collisionShape = new btCylinderShape(btVector3(boxX,boxY,boxZ));
        }
        else if (format == Constants::COLLISION_SHAPE_CAPSULE)
        {
            float radius = pod->getRadius();
            float height = pod->getHeight();
            collisionShape = new btCapsuleShape(radius,height);
        }
        else if (format == Constants::COLLISION_SHAPE_CONE)
        {
            //collisionShape = new btConeShape();
        }
        else if (format == Constants::COLLISION_SHAPE_MULTI_SPHERE)
        {
            //collisionShape = new btMultiSphereShape();
        }
        else if (format == Constants::COLLISION_SHAPE_CONVEX_HULL)
        {
            //collisionShape = new btConvexHullShape();
        }
        else if (format == Constants::COLLISION_SHAPE_CONVEX_TRIANGLE_MESH)
        {
            //collisionShape = new btConvexTriangleMeshShape();
        }
        else if (format == Constants::COLLISION_SHAPE_BVH_TRIANGLE_MESH)
        {
            // Load Collision Data
            auto sceneRt = mEntityRuntime->getSceneRuntime();
            if (sceneRt)
            {
                auto modelUuid = pod->getCollisionModel();
                auto pDef = static_cast<ProjectDefinition*>(sceneRt->getProjectRuntime()->getDefinition());
                if (pDef)
                {
                    auto modelDef = pDef->getAssetDefinitionByUuid(modelUuid);
                    if (modelDef)
                    {
                        auto model = static_cast<ModelRuntime*>(mModelCache->getRuntime(modelDef));
                        if (model)
                        {
                            collisionShape = createTriangleMeshShape(model);
                        }
                    }
                }
            }
        }
        else if (format == Constants::COLLISION_SHAPE_HEIGHTFIELD_TERRAIN)
        {
            // ???
        }
        else if (format == Constants::COLLISION_SHAPE_STATIC_PLANE)
        {
            float x = pod->getNormalX();
            float y = pod->getNormalY();
            float z = pod->getNormalZ();
            float constant = pod->getConstant();

            btVector3 planeNormal(x,y,z);
            auto planeConstant = btScalar(constant);

            collisionShape = new btStaticPlaneShape(planeNormal,planeConstant);
        }
        else if (format == Constants::COLLISION_SHAPE_COMPOUND)
        {
            collisionShape = new btCompoundShape();
            btCompoundShape* compound = static_cast<btCompoundShape*>(collisionShape);

            for (CompoundChildDefinition& child : pod->getCompoundChildren())
            {
                auto def = getAssetDefinitionByUuid(child.uuid);
                btCollisionShape *shape = createCollisionShape(def);
                if (shape)
                {
                    compound->addChildShape(child.transform.getBtTransform(),shape);
                }
            }
        }

        if (collisionShape)
        {
            btScalar margin = pod->getMargin();
            collisionShape->setMargin(margin);
        }

        return collisionShape;
    }

    btCollisionShape*
    PhysicsObjectRuntime::createTriangleMeshShape
    (ModelRuntime* model)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        auto* triMesh = new btTriangleMesh();
        auto  meshes = model->getMeshes();

        if (meshes.empty())
        {
            return nullptr;
        }

        for (auto mesh : meshes)
        {
            auto idx = mesh->getIndices();
            auto verts = mesh->getVertices();
            for (size_t i=0; i<idx.size()-3;)
            {
                btVector3 v1,v2,v3;

                auto i1 = verts.at(idx.at(i)).Position;
                i++;
                auto i2 = verts.at(idx.at(i)).Position;
                i++;
                auto i3 = verts.at(idx.at(i)).Position;
                i++;

               v1.setX(i1.x);
               v1.setY(i1.y);
               v1.setZ(i1.z);

               v2.setX(i2.x);
               v2.setY(i2.y);
               v2.setZ(i2.z);

               v3.setX(i3.x);
               v3.setY(i3.y);
               v3.setZ(i3.z);

               triMesh->addTriangle(v1,v2,v3);
            }
        }
        return new btBvhTriangleMeshShape(triMesh,true,true);
    }

    btRigidBody*
    PhysicsObjectRuntime::getRigidBody
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        return mRigidBody;
    }

    void
    PhysicsObjectRuntime::getWorldTransform
    (btTransform &transform)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mMotionState->getWorldTransform(transform);
    }

    btCollisionObject*
    PhysicsObjectRuntime::getCollisionObject
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        return mRigidBody;
    }

    Vector3
    PhysicsObjectRuntime::getCenterOfMassPosition
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
       return Vector3(mRigidBody->getCenterOfMassPosition());
    }

    void
    PhysicsObjectRuntime::applyCentralImpulse(const Vector3& force)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mRigidBody->applyCentralImpulse(force.toBullet());
    }

    void
    PhysicsObjectRuntime::applyTorqueImpulse(const Vector3& torque)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mRigidBody->applyTorqueImpulse(torque.toBullet());
    }

    void
    PhysicsObjectRuntime::applyForce
    (const Vector3& force)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
       mRigidBody->applyForce(force.toBullet(),btVector3(0.0f,0.0f,0.0f));
    }

    void
    PhysicsObjectRuntime::applyTorque
    (const Vector3& torque)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mRigidBody->applyTorque(torque.toBullet());
    }

    void
    PhysicsObjectRuntime::clearForces
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mRigidBody->clearForces();
    }

    void
    PhysicsObjectRuntime::setCenterOfMassTransformTx
    (Transform& tx)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
       mRigidBody->setCenterOfMassTransform(tx.getBtTransform());
    }

    void
    PhysicsObjectRuntime::setCenterOfMassTransformMat4
    (mat4 tx)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        btTransform transform;
        transform.setFromOpenGLMatrix(value_ptr(tx));
        mRigidBody->setCenterOfMassTransform(transform);
    }

    void
    PhysicsObjectRuntime::setCenterOfMassTransform3f
    (float x, float y, float z)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        btTransform transform;
        auto current = mRigidBody->getCenterOfMassTransform();
        current.setOrigin(btVector3(x,y,z));
        mRigidBody->setCenterOfMassTransform(current);
    }


    void
    PhysicsObjectRuntime::setCenterOfMassTransform3fv
    (const Vector3& tx)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        LOG_DEBUG("PhysicsObjectRuntime: Setting Center of mass {},{},{}", tx.x(), tx.y(), tx.z());
        auto mtx = mRigidBody->getCenterOfMassTransform();
        mtx.setOrigin(tx.toBullet());
        mRigidBody->setCenterOfMassTransform(mtx);
    }

    void
    PhysicsObjectRuntime::setWorldTransform
    (Transform& tx)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
       mRigidBody->setWorldTransform(tx.getBtTransform());
    }

    void
    PhysicsObjectRuntime::setLinearVelocity
    (float x, float y, float z)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mRigidBody->setLinearVelocity(btVector3(x,y,z));
    }

    Vector3
    PhysicsObjectRuntime::getLinearVelocity
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        return Vector3(mRigidBody->getLinearVelocity());
    }

    PhysicsObjectDefinition*
    PhysicsObjectRuntime::getAssetDefinitionByUuid
    (UuidType uuid)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        // TODO why from def?
        auto proj = static_cast<AssetDefinition*>(mDefinition)->getProject();
        return static_cast<PhysicsObjectDefinition*>(proj->getAssetDefinitionByUuid(uuid));
    }

    void
    PhysicsObjectRuntime::setLinearFactor
    (float x, float y, float z)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mRigidBody->setLinearFactor(btVector3(x,y,z));
    }

    void
    PhysicsObjectRuntime::setAngularFactor
    (float x, float y, float z)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mRigidBody->setAngularFactor(btVector3(x,y,z));
    }

    void
    PhysicsObjectRuntime::setAngularVelocity
    (float x, float y, float z)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mRigidBody->setAngularVelocity(btVector3(x,y,z));
    }

    void
    PhysicsObjectRuntime::setRestitution
    (float r)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
       mRigidBody->setRestitution(r);
    }

    float
    PhysicsObjectRuntime::getFriction
    () const
    {
        return mRigidBody->getFriction();
    }

    void
    PhysicsObjectRuntime::setFriction
    (float friction)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mRigidBody->setFriction(friction);
    }

    float
    PhysicsObjectRuntime::getMass
    () const
    {
       return mRigidBody->getInvMass();
    }

    void
    PhysicsObjectRuntime::setMass
    (float mass)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mPhysicsComponent->removeRigidBody(mRigidBody);
        btVector3 inertia(0.0f,0.0f,0.0f);
        mRigidBody->getCollisionShape()->calculateLocalInertia(mass,inertia);
        mRigidBody->setMassProps(mass,inertia);
        mPhysicsComponent->addRigidBody(mRigidBody);
    }

    void
    PhysicsObjectRuntime::setCcdSweptSphereRadius
    (float ccd)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        if (ccd != 0.0f)
        {
            mRigidBody->setCcdMotionThreshold(1e-7f);
            mRigidBody->setCcdSweptSphereRadius(ccd);
        }
    }

    float
    PhysicsObjectRuntime::getCcdSweptSphereRadius
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        return mRigidBody->getCcdSweptSphereRadius();
    }

    float
    PhysicsObjectRuntime::getRestitution
    ()
    const
    {
        return mRigidBody->getRestitution();
    }

    bool PhysicsObjectRuntime::isInPhysicsWorld()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        return mInPhysicsWorld;
    }

    void PhysicsObjectRuntime::setInPhysicsWorld(bool inPhysicsWorld)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if (!lg.owns_lock()) getMutex().lock();
        mInPhysicsWorld = inPhysicsWorld;
    }

}
