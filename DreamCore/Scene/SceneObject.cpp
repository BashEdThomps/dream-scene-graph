
#include "SceneObject.h"

#include <iostream>
#include <cmath>
#include <sstream>
#include <vector>
#include <algorithm>

#include "Scene.h"

#include "../Project/Project.h"
#include "../Utilities/String.h"
#include "../Utilities/Uuid.h"


namespace Dream
{
    SceneObject::SceneObject
    (Scene* scene)
        : mSceneHandle(scene)
    {
        mJson[Constants::SCENE_OBJECT_UUID] = Uuid::generateUuid();
        constructorInit();
    }

    SceneObject::SceneObject
    (Scene* scene, nlohmann::json soJson)
        : mSceneHandle(scene),
          mJson(soJson)
    {
        constructorInit();
        loadJsonData(soJson);
    }

    void
    SceneObject::constructorInit
    ()
    {
        mTransform.reset(new Transform3D());
        mLoaded = false;
        mDelete = false;
        mHasFocus = false;
        mParentHandle = nullptr;
        mAudioInstance = nullptr;
        mAnimationInstance = nullptr;
        mModelInstance  = nullptr;
        mShaderInstance = nullptr;
        mLightInstance = nullptr;
        mSpriteInstance = nullptr;
        mScriptInstance = nullptr;
        mPhysicsObjectInstance = nullptr;
        mFontInstance = nullptr;
    }

    void
    SceneObject::loadJsonData
    (nlohmann::json soJson)
    {
        string transformType;
        if (!soJson[Constants::SCENE_OBJECT_TRANSFORM_TYPE].is_null())
        {
            transformType = soJson[Constants::SCENE_OBJECT_TRANSFORM_TYPE];
            mTransform->setTransformType(transformType);
        }
        else
        {
            transformType = Constants::SCENE_OBJECT_TRANSFORM_TYPE_OFFSET;
            mTransform->setTransformType(transformType);
        }

        if (!soJson[Constants::SCENE_OBJECT_TRANSLATION].is_null())
        {
            nlohmann::json translation = soJson[Constants::SCENE_OBJECT_TRANSLATION];
            setTranslation(
                translation[Constants::X],
                translation[Constants::Y],
                translation[Constants::Z]
            );
        }
        else
        {
            resetTranslation();
        }

        if (!soJson[Constants::SCENE_OBJECT_ROTATION].is_null())
        {
            nlohmann::json rotation = soJson[Constants::SCENE_OBJECT_ROTATION];
            setRotation(
                rotation[Constants::X],
                rotation[Constants::Y],
                rotation[Constants::Z]
            );
        }
        else
        {
            resetRotation();
        }

        if (!soJson[Constants::SCENE_OBJECT_SCALE].is_null())
        {
            nlohmann::json scale = soJson[Constants::SCENE_OBJECT_SCALE];
            setScale(
                scale[Constants::X],
                scale[Constants::Y],
                scale[Constants::Z]
            );
        }
        else
        {
            resetScale();
        }

        if(!soJson[Constants::SCENE_OBJECT_HAS_FOCUS].is_null())
        {
            bool focus = soJson[Constants::SCENE_OBJECT_HAS_FOCUS];
            setHasFocus(focus);
        }
    }

    void
    SceneObject::resetTransform
    ()
    {
        resetTranslation();
        resetRotation();
        resetScale();
    }

    void
    SceneObject::resetTranslation
    ()
    {
        setTranslation(0.0f, 0.0f, 0.0f);
    }

    void
    SceneObject::resetRotation
    ()
    {
        setRotation(0.0f, 0.0f, 0.0f);
    }

    void
    SceneObject::resetScale
    ()
    {
        setScale(1.0f, 1.0f, 1.0f);
    }


    SceneObject::~SceneObject
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "SceneObject: Destroying Object "
                 << getNameAndUuidString() << endl;
        }
    }

    void
    SceneObject::deleteChildren
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "SceneObject: Deleting " << mChildren.size()
                 << "children of "
                 << getNameAndUuidString() << endl;
        }
        for (SceneObject* child : mChildren)
        {
            delete child;
        }
        mChildren.clear();
    }


    void
    SceneObject::deleteAssetInstances
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "SceneObject: Deleting asset instances for "
                 << getNameAndUuidString() << endl;
        }

        if (mAudioInstance != nullptr)
        {
            mAudioInstance.reset();
        }

        if (mAnimationInstance != nullptr)
        {
            mAnimationInstance.reset();
        }

        if (mModelInstance != nullptr)
        {
            mModelInstance.reset();
        }

        if (mShaderInstance != nullptr)
        {
            mShaderInstance.reset();
        }

        if (mLightInstance != nullptr)
        {
            mLightInstance.reset();
        }

        if (mSpriteInstance != nullptr)
        {
            mSpriteInstance.reset();
        }

        if (mScriptInstance != nullptr)
        {
            mScriptInstance.reset();
        }

        if (mPhysicsObjectInstance != nullptr)
        {
            mPhysicsObjectInstance.reset();
        }

        if (mFontInstance != nullptr)
        {
            mFontInstance.reset();
        }
    }

    bool
    SceneObject::hasName
    (string name)
    {
        return getName().compare(name) == 0;;
    }

    void
    SceneObject::setName
    (string name)
    {
        mJson[Constants::SCENE_OBJECT_NAME] = name;
    }

    string
    SceneObject::getName
    ()
    {
        return mJson[Constants::SCENE_OBJECT_NAME];
    }

    void
    SceneObject::setTranslation
    (glm::vec3 translation)
    {
        mTransform->setTranslation(translation);
    }

    void
    SceneObject::setRotation
    (glm::vec3 rotation)
    {
        mTransform->setRotation(rotation);
    }

    void
    SceneObject::setScale
    (glm::vec3 scale)
    {
        mTransform->setScale(scale);
    }

    void
    SceneObject::setTranslation
    (float x, float y, float z)
    {
        mTransform->setTranslation(x,y,z);
    }

    void
    SceneObject::setRotation
    (float x, float y, float z)
    {
        mTransform->setRotation(x,y,z);
    }

    void
    SceneObject::setScale
    (float x, float y, float z)
    {
        mTransform->setScale(x,y,z);
    }

    glm::vec3
    SceneObject::getRotation
    ()
    {
        return mTransform->getRotation();
    }

    glm::vec3
    SceneObject::getScale
    ()
    {
        return mTransform->getScale();
    }

    glm::vec3
    SceneObject::getTranslation
    ()
    {
        return mTransform->getTranslation();
    }

    bool
    SceneObject::hasUuid
    (string uuid)
    {
        return (getUuid().compare(uuid) == 0);
    }

    void
    SceneObject::setUuid
    (string uuid)
    {
        mJson[Constants::SCENE_OBJECT_UUID] = uuid;
    }

    string
    SceneObject::getUuid
    ()
    {
        return mJson[Constants::SCENE_OBJECT_UUID];
    }

    int
    SceneObject::countAllChildren
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "SceneObject: Count All Children, Not Implemented" << endl;
        }
        return -1;
    }

    size_t
    SceneObject::countChildren
    ()
    {
        return mChildren.size();
    }

    void
    SceneObject::addChild
    (SceneObject* child)
    {
        child->setParent(this);
        mChildren.push_back(child);
    }

    void
    SceneObject::removeChild
    (SceneObject* child)
    {
        mChildren.erase(remove(mChildren.begin(), mChildren.end(), child), mChildren.end());
    }

    bool
    SceneObject::isChildOf
    (SceneObject* parent)
    {
        return mParentHandle == parent;
    }

    bool
    SceneObject::isParentOf
    (SceneObject* sceneObject)
    {
        return sceneObject->getParent() == this;
    }

    string
    SceneObject::getNameAndUuidString
    ()
    {
        return getUuid()+" : "+getName();
    }

    void
    SceneObject::showStatus
    ()
    {
        cout << "SceneObject: " << mJson.dump(1) << endl;
        if (mParentHandle != nullptr)
        {
            cout << "SceneObject: ParentUuid: " << mParentHandle->getUuid() << endl;
        }
    }

    vector<string>
    SceneObject::getAssetDefUuidsToLoad
    ()
    {
        vector<string> toLoad;
        for (nlohmann::json uuid : mJson[Constants::SCENE_OBJECT_ASSET_INSTANCES])
        {
            toLoad.push_back(uuid);
        }
        return toLoad;
    }

    void
    SceneObject::setAnimationInstance
    (AnimationInstance* animationAsset)
    {
        mAnimationInstance.reset(animationAsset);
    }

    AnimationInstance*
    SceneObject::getAnimationInstance
    ()
    {
        return mAnimationInstance.get();
    }

    void
    SceneObject::setAudioInstance
    (AudioInstance* audioAsset)
    {
        mAudioInstance.reset(audioAsset);
    }

    AudioInstance*
    SceneObject::getAudioInstance
    ()
    {
        return mAudioInstance.get();
    }

    void
    SceneObject::setModelInstance
    (AssimpModelInstance* modelAsset)
    {
        mModelInstance.reset(modelAsset);
    }

    AssimpModelInstance*
    SceneObject::getModelInstance
    ()
    {
        return mModelInstance.get();
    }

    void
    SceneObject::setScriptInstance
    (LuaScriptInstance* scriptAsset)
    {
        mScriptInstance.reset(scriptAsset);
    }

    LuaScriptInstance*
    SceneObject::getScriptInstance
    ()
    {
        return mScriptInstance.get();
    }

    void
    SceneObject::setShaderInstance
    (ShaderInstance* shaderAsset)
    {
        mShaderInstance.reset(shaderAsset);
    }

    ShaderInstance*
    SceneObject::getShaderInstance
    ()
    {
        return mShaderInstance.get();
    }

    void
    SceneObject::setLightInstance
    (LightInstance* lightAsset)
    {
        mLightInstance.reset(lightAsset);
    }

    LightInstance*
    SceneObject::getLightInstance
    ()
    {
        return mLightInstance.get();
    }

    bool
    SceneObject::hasLightInstance
    ()
    {
        return mLightInstance != nullptr;
    }

    bool
    SceneObject::hasModelInstance
    ()
    {
        return mModelInstance != nullptr;
    }

    bool
    SceneObject::hasShaderInstance
    ()
    {
        return mShaderInstance != nullptr;
    }

    bool
    SceneObject::hasScriptInstance
    ()
    {
        return mScriptInstance != nullptr;
    }

    bool
    SceneObject::hasSpriteInstance
    ()
    {
        return mSpriteInstance != nullptr;
    }

    bool
    SceneObject::hasFontInstance
    ()
    {
        return mFontInstance != nullptr;
    }

    void
    SceneObject::setFontInstance
    (FontInstance* font)
    {
        mFontInstance.reset(font);
    }

    FontInstance*
    SceneObject::getFontInstance
    ()
    {
        return mFontInstance.get();
    }

    void
    SceneObject::setPhysicsObjectInstance
    (PhysicsObjectInstance* poi)
    {
        return mPhysicsObjectInstance.reset(poi);
    }

    PhysicsObjectInstance*
    SceneObject::getPhysicsObjectInstance
    ()
    {
        return mPhysicsObjectInstance.get();
    }

    string
    SceneObject::getTransformType
    ()
    {
        return mTransform->getTransformType();
    }

    void
    SceneObject::setTransformType
    (string transformType)
    {
        mTransform->setTransformType(transformType);
    }

    Transform3D*
    SceneObject::getTransform
    () {
        return mTransform.get();
    }

    void
    SceneObject::setTransform
    (Transform3D* transform)
    {
        mTransform.reset(transform);
    }

    void
    SceneObject::setParent
    (SceneObject* parent)
    {
        mParentHandle = parent;
    }

    SceneObject*
    SceneObject::getParent
    ()
    {
        return mParentHandle;
    }

    void SceneObject::setSpriteInstance(SpriteInstance* spriteAsset)
    {
        mSpriteInstance.reset(spriteAsset);
    }

    SpriteInstance*
    SceneObject::getSpriteInstance
    ()
    {
        return mSpriteInstance.get();
    }

    void
    SceneObject::setHasFocus
    (bool focus)
    {
        mJson[Constants::SCENE_OBJECT_HAS_FOCUS] = focus;
    }

    bool
    SceneObject::hasFocus
    ()
    {
        return mJson[Constants::SCENE_OBJECT_HAS_FOCUS];
        return mHasFocus;
    }

    void
    SceneObject::addAssetDefUuidToLoad
    (string uuid)
    {
        mJson[Constants::SCENE_OBJECT_ASSET_INSTANCES].push_back(uuid);
    }

    void
    SceneObject::setDeleteFlag
    (bool del)
    {
        mDelete = del;
    }

    bool
    SceneObject::getDeleteFlag
    ()
    {
        return mDelete;
    }

    bool
    SceneObject::getLoadedFlag
    ()
    {
        return mLoaded;
    }

    void
    SceneObject::setLoadedFlag
    (bool loaded)
    {
        mLoaded = loaded;
    }

    SceneObject*
    SceneObject::getChildByUuid
    (string childUuid)
    {
        for (SceneObject* it : mChildren)
        {
            if (it->hasUuid(childUuid))
            {
                return it;
            }
        }
        return nullptr;
    }

    bool
    SceneObject::hasEvents
    ()
    {
        return mEventQueue.size() != 0;
    }

    void
    SceneObject::sendEvent
    (Event* event)
    {
        mEventQueue.push_back(event);
    }

    vector<Event*>*
    SceneObject::getEventQueue
    ()
    {
        return &mEventQueue;
    }

    void
    SceneObject::cleanUp
    ()
    {
        cleanUpEvents();
        deleteAssetInstances();
        setLoadedFlag(false);
        setDeleteFlag(false);
    }

    void
    SceneObject::cleanUpEvents
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "SceneObject: Cleaning up events " << getNameAndUuidString()
                 << endl;
        }
        for (Event* it : mEventQueue)
        {
            delete it;
        }
        mEventQueue.clear();
    }

    bool
    SceneObject::hasPhysicsObjectInstance
    ()
    {
        return mPhysicsObjectInstance != nullptr;
    }

    vector<SceneObject*>
    SceneObject::getChildren
    ()
    {
        return mChildren;
    }

    nlohmann::json
    SceneObject::getJson
    ()
    {
        return mJson;
    }

    bool
    SceneObject::applyToAll
    (function<bool(SceneObject*)> funk)
    {
        bool retval = funk(this);

        for (SceneObject* it : mChildren)
        {
            if (it)
            {
                retval = retval || funk(it);
            }
        }
        return retval;
    }

    void*
    SceneObject::applyToAll
    (function<void*(SceneObject*)> funk)
    {
        void* retval = funk(this);
        if (retval)
        {
            return retval;
        }

        for (SceneObject* it : mChildren)
        {
            if (it)
            {
                retval = funk(it);
                if (retval)
                {
                    return retval;
                }
            }
        }
        return nullptr;
    }

    bool
    SceneObject::hasAnimationInstance
    ()
    {
        return mAnimationInstance != nullptr;
    }

    bool
    SceneObject::hasAudioInstance
    ()
    {
        return mAudioInstance != nullptr;
    }

    void
    SceneObject::createAssetInstances
    ()
    {
        for (string aDefUuid : getAssetDefUuidsToLoad())
        {
            createAssetInstanceFromAssetDefinitionByUuid(aDefUuid);
        }
    }

    void
    SceneObject::createAssetInstanceFromAssetDefinitionByUuid
    (string uuid)
    {
        AssetDefinition* assetDefinition =
            AssetDefinition::getAssetDefinitionByUuid(
                mSceneHandle->getProjectHandle()->getAssetDefinitions(),uuid
            );

        createAssetInstance(assetDefinition);
    }

    void
    SceneObject::createAssetInstance
    (AssetDefinition* definition)
    {
        if (Constants::DEBUG)
        {
            cout << "SceneObject: Creating Asset Intance of: ("
                 << definition->getType() << ") " << definition->getName()
                 << ", for  " << getNameAndUuidString()
                 << endl;
        }

        if(definition->isTypeAnimation())
        {
            createAnimationInstance(definition);
        }
        else if (definition->isTypeAudio())
        {
            createAudioInstance(definition);
        }
        else if (definition->isTypeModel())
        {
            createModelInstance(definition);
        }
        else if (definition->isTypeScript())
        {
            createScriptInstance(definition);
        }
        else if (definition->isTypeShader())
        {
            createShaderInstance(definition);
        }
        else if (definition->isTypePhysicsObject())
        {
            createPhysicsObjectInstance(definition);
        }
        else if (definition->isTypeLight())
        {
            createLightInstance(definition);
        }
        else if (definition->isTypeSprite())
        {
            createSpriteInstance(definition);
        }
        else if (definition->isTypeFont())
        {
            createFontInstance(definition);
        }
    }

    void
    SceneObject::loadAssetInstances
    ()
    {
        string projectPath = mSceneHandle->getProjectHandle()->getProjectPath();

        if (Constants::DEBUG)
        {
            cout << "SceneObject: Loading Asset Data from "
                 << projectPath << endl;
        }

        if(mAudioInstance)
        {
            mAudioInstance->load(projectPath);
        }

        if(mAnimationInstance)
        {
            mAnimationInstance->load(projectPath);
        }

        if(mModelInstance)
        {
           mModelInstance->load(projectPath);
        }

        if(mShaderInstance)
        {
            mShaderInstance->load(projectPath);
        }

        if(mLightInstance)
        {
           mLightInstance->load(projectPath);
        }

        if(mSpriteInstance)
        {
           mSpriteInstance->load(projectPath);
        }

        if(mScriptInstance)
        {
           mScriptInstance->load(projectPath);
        }

        if(mPhysicsObjectInstance)
        {
           mPhysicsObjectInstance->load(projectPath);
        }

        if(mFontInstance)
        {
           mFontInstance->load(projectPath);
        }

        setLoadedFlag(true);
    }

    void
    SceneObject::createPhysicsObjectInstance
    (AssetDefinition* definition)
    {
        if (Constants::DEBUG)
        {
            cout << "SceneObject: Creating Physics Object Asset Instance." << endl;
        }
        mPhysicsObjectInstance.reset(
            new PhysicsObjectInstance(definition, mTransform.get(),
            mSceneHandle->getProjectHandle()->getAssetDefinitions())
        );
    }

    void
    SceneObject::createAnimationInstance
    (AssetDefinition* definition)
    {
        if (Constants::DEBUG)
        {
            cout << "SceneObject: Creating Animation asset instance." << endl;
        }
        mAnimationInstance.reset(new AnimationInstance(definition,mTransform.get()));
    }

    void
    SceneObject::createAudioInstance
    (AssetDefinition* definition)
    {
        if (Constants::DEBUG)
        {
            cout << "SceneObject: Creating Audio asset instance." << endl;
        }
        // hottest trainwreck 2017!
        mAudioInstance.reset(mSceneHandle
                ->getProjectHandle()
                ->getRuntime()
                ->getAudioComponent()
                ->newAudioInstance(definition,mTransform.get())
        );
    }

    void
    SceneObject::createModelInstance
    (AssetDefinition* definition)
    {
        if (Constants::DEBUG)
        {
            cout << "SceneObject: Creating Model asset instance." << endl;
        }
        mModelInstance.reset
        (
            new AssimpModelInstance(definition,mTransform.get())
        );
    }

    void
    SceneObject::createScriptInstance
    (AssetDefinition* definition)
    {
        if (Constants::DEBUG)
        {
            cout << "SceneObject: Creating Script asset instance." << endl;
        }
        // hottest trainwreck 2017!
        mScriptInstance.reset(new LuaScriptInstance(definition, mTransform.get()));
        mSceneHandle->getProjectHandle()
              ->getRuntime()
              ->getLuaEngine()
              ->addToScriptMap(this,mScriptInstance.get());
    }

    void
    SceneObject::createShaderInstance
    (AssetDefinition* definition)
    {
        if (Constants::DEBUG)
        {
            cout << "SceneObject: Creating Shader asset instance." << endl;
        }
        mShaderInstance.reset(new ShaderInstance(definition,mTransform.get()));
    }

    void
    SceneObject::createLightInstance
    (AssetDefinition* definition)
    {
        if (Constants::DEBUG)
        {
            cout << "SceneObject: Creating Light Asset instance." << endl;
        }
        mLightInstance.reset(new LightInstance(definition,mTransform.get()));
    }

    void
    SceneObject::createSpriteInstance
    (AssetDefinition* definition)
    {
        if (Constants::DEBUG)
        {
            cout << "SceneObject: Creating Sprite Asset instance." << endl;
        }
        mSpriteInstance.reset(new SpriteInstance(definition,mTransform.get()));
    }

    void
    SceneObject::createFontInstance
    (AssetDefinition* definition)
    {
        if (Constants::DEBUG)
        {
            cout << "SceneObject: Creating Font Asset instance." << endl;
        }
        mFontInstance.reset(new FontInstance(definition,mTransform.get()));
    }

    Scene*
    SceneObject::getSceneHandle
    ()
    {
        return mSceneHandle;
    }

} // End of Dream