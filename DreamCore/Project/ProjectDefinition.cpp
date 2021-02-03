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
 *
 */

#include "ProjectDefinition.h"
#include "Project.h"
#include "Scene/SceneDefinition.h"
#include "Components/AssetDefinition.h"
#include "Components/Audio/AudioDefinition.h"
#include "Components/Animation/AnimationDefinition.h"
#include "Components/Path/PathDefinition.h"
#include "Components/Graphics/Font/FontDefinition.h"
#include "Components/Graphics/Light/LightDefinition.h"
#include "Components/Graphics/Material/MaterialDefinition.h"
#include "Components/Graphics/Model/ModelDefinition.h"
#include "Components/Graphics/Shader/ShaderDefinition.h"
#include "Components/Graphics/Texture/TextureDefinition.h"
#include "Components/Physics/PhysicsObjectDefinition.h"
#include "Components/Script/ScriptDefinition.h"
#include "Common/Uuid.h"
#include "Common/Logger.h"
#include "Common/Constants.h"

using std::pair;
using std::unique_lock;

namespace octronic::dream
{
    ProjectDefinition::ProjectDefinition
    (const json& data)
        : Definition("ProjectDefinition",data)

    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        LOG_TRACE("ProjectDefinition: Constructing {}", getNameAndUuidString());
    }


    ProjectDefinition::~ProjectDefinition
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        LOG_TRACE("ProjectDefinition: Destructing {}", getNameAndUuidString());
        deleteSceneDefinitions();
        deleteAssetDefinitions();
    }

    void
    ProjectDefinition::setStartupSceneUuid
    (UuidType sceneUuid)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        mJson[Constants::PROJECT_STARTUP_SCENE] = sceneUuid;
    }

    UuidType ProjectDefinition::getStartupSceneUuid()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        if (!mJson[Constants::PROJECT_STARTUP_SCENE].is_number())
        {
            mJson[Constants::PROJECT_STARTUP_SCENE] = 0;
        }
        return mJson[Constants::PROJECT_STARTUP_SCENE];
    }

    void
    ProjectDefinition::setDescription
    (const string &description)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        mJson[Constants::PROJECT_DESCRIPTION] = description;
    }

    string
    ProjectDefinition::getDescription
    (void)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        return mJson[Constants::PROJECT_DESCRIPTION];
    }

    void
    ProjectDefinition::setAuthor
    (const string &author)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        mJson[Constants::PROJECT_AUTHOR] = author;
    }

    string
    ProjectDefinition::getAuthor
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        return mJson[Constants::PROJECT_AUTHOR];
    }

    void
    ProjectDefinition::loadChildDefinitions
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        loadAssetDefinitions();
        loadSceneDefinitions();
    }

    void
    ProjectDefinition::loadAssetDefinitions
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        LOG_DEBUG("ProjectDefinition: Loading AssetDefinitions from JSON");

        for (const json& it : mJson[Constants::PROJECT_ASSET_ARRAY])
        {
            loadAssetDefinition(it);
        }
        regroupAssetDefinitions();
    }

    void
    ProjectDefinition::loadSceneDefinitions
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        LOG_DEBUG("ProjectDefinition: Loading ScenesDefinitions from JSON");

        for (const json& it : mJson[Constants::PROJECT_SCENE_ARRAY])
        {
            loadSceneDefinition(it);
        }
    }


    AssetDefinition*
    ProjectDefinition::createAssetDefinition
    (const json &assetDefinitionJs)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        AssetType type = Constants::getAssetTypeEnumFromString(assetDefinitionJs[Constants::ASSET_TYPE]);

        switch (type)
        {
            case ASSET_TYPE_ENUM_ANIMATION:
                return new AnimationDefinition(this, assetDefinitionJs);
            case ASSET_TYPE_ENUM_AUDIO:
                return new AudioDefinition(this, assetDefinitionJs);
            case ASSET_TYPE_ENUM_FONT:
                return new FontDefinition(this, assetDefinitionJs);
            case ASSET_TYPE_ENUM_LIGHT:
                return new LightDefinition(this, assetDefinitionJs);
            case ASSET_TYPE_ENUM_MATERIAL:
                return new MaterialDefinition(this, assetDefinitionJs);
            case ASSET_TYPE_ENUM_MODEL:
                return new ModelDefinition(this, assetDefinitionJs);
            case ASSET_TYPE_ENUM_PATH:
                return new PathDefinition(this, assetDefinitionJs);
            case ASSET_TYPE_ENUM_PHYSICS_OBJECT:
                return new PhysicsObjectDefinition(this, assetDefinitionJs);
            case ASSET_TYPE_ENUM_SCRIPT:
                return new ScriptDefinition(this, assetDefinitionJs);
            case ASSET_TYPE_ENUM_SHADER:
                return new ShaderDefinition(this, assetDefinitionJs);
            case ASSET_TYPE_ENUM_TEXTURE:
                return new TextureDefinition(this, assetDefinitionJs);
            case ASSET_TYPE_ENUM_NONE:
                LOG_ERROR("ProjectDefinition: Unable to create Asset Definition. Unknown Type");
                break;
        }
        return nullptr;
    }

    map<AssetType, vector<string> > &
    ProjectDefinition::getAssetDefinitionGroups
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        return mAssetDefinitionGroups;
    }

    void
    ProjectDefinition::loadAssetDefinition
    (const json &assetDefinitionJs)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        AssetDefinition* newDef = createAssetDefinition(assetDefinitionJs);
        if (newDef != nullptr)
        {
            mAssetDefinitions.push_back(newDef);
        }
    }

    void
    ProjectDefinition::removeAssetDefinition
    (AssetDefinition* assetDefinition)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        LOG_DEBUG(
                    "ProjectDefinition: Removing AssetDefinition {} from {}",
                    assetDefinition->getNameAndUuidString(),
                    getNameAndUuidString()
                    );
        auto iter = begin(mAssetDefinitions);
        auto endPos = end(mAssetDefinitions);
        while (iter != endPos)
        {
            if ((*iter) == assetDefinition)
            {
                LOG_DEBUG(
                            "ProjectDefinition: Found AssetDefinition to {} remove from {}",
                            assetDefinition->getNameAndUuidString(),
                            getNameAndUuidString()
                            );
                mAssetDefinitions.erase(iter);
                return;
            }
            iter++;
        }
    }

    size_t
    ProjectDefinition::countAssetDefinitions
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        return mAssetDefinitions.size();
    }

    AssetDefinition*
    ProjectDefinition::getAssetDefinitionByUuid
    (UuidType uuid)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        for (auto it = begin(mAssetDefinitions); it != end(mAssetDefinitions); it++)
        {
            if ((*it)->hasUuid(uuid))
            {
                return (*it);
            }
        }
        return nullptr;
    }

    AssetDefinition*
    ProjectDefinition::getAssetDefinitionByName
    (const string& name)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        for (auto it = begin(mAssetDefinitions); it != end(mAssetDefinitions); it++)
        {
            if ((*it)->hasName(name))
            {
                return (*it);
            }
        }
        return nullptr;
    }

    void
    ProjectDefinition::addAssetDefinition
    (AssetDefinition* def)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

       mAssetDefinitions.push_back(def);
       regroupAssetDefinitions();
    }

    void
    ProjectDefinition::loadSceneDefinition
    (const json &scene)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        auto so = new SceneDefinition(this, scene);
        so->loadRootEntityDefinition();
        mSceneDefinitions.push_back(so);
    }

    size_t
    ProjectDefinition::countScenesDefinitions
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        return mSceneDefinitions.size();
    }

    SceneDefinition*
    ProjectDefinition::getSceneDefinitionByName
    (const string &name)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        for (auto it = begin(mSceneDefinitions); it != end(mSceneDefinitions); it++)
        {
            if ((*it)->hasName(name))
            {
                return (*it);
            }
        }
        return nullptr;
    }

    SceneDefinition*
    ProjectDefinition::getSceneDefinitionByUuid
    (UuidType uuid)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        for (auto it = begin(mSceneDefinitions); it != end(mSceneDefinitions); it++)
        {
            if ((*it)->hasUuid(uuid))
            {
                return (*it);
            }
        }
        return nullptr;
    }

    vector<SceneDefinition*>
    ProjectDefinition::getSceneDefinitionsVector
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        vector<SceneDefinition*> list;
        for (auto it = begin(mSceneDefinitions); it != end(mSceneDefinitions); it++)
        {
            list.push_back((*it));
        }
        return list;
    }

    void
    ProjectDefinition::removeSceneDefinition
    (SceneDefinition* sceneDef)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        LOG_DEBUG(
                    "ProjectDefinition: Removing SceneDefinition {} from {}",
                    sceneDef->getNameAndUuidString(),
                    getNameAndUuidString()
                    );

        auto iter = begin(mSceneDefinitions);
        auto endPos = end(mSceneDefinitions);
        while (iter != endPos)
        {
            if ((*iter) == sceneDef)
            {
                LOG_DEBUG(
                            "ProjectDefinition: Found scene to {} remove from {}",
                            sceneDef->getNameAndUuidString(),
                            getNameAndUuidString()
                            );
                delete (*iter);
                mSceneDefinitions.erase(iter);
                return;
            }
            iter++;
        }
    }

    vector<AssetDefinition*>
    ProjectDefinition::getAssetDefinitionsVector
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        vector<AssetDefinition*> definitionsList;
        for (auto it = begin(mAssetDefinitions); it != end(mAssetDefinitions); it++)
        {
            definitionsList.push_back((*it));
        }
        return definitionsList;
    }

    vector<AssetDefinition*>
    ProjectDefinition::getAssetDefinitionsVector
    (AssetType type)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        vector<AssetDefinition*> definitionsList;
        for (auto it = begin(mAssetDefinitions); it != end(mAssetDefinitions); it++)
        {
            auto thisAssetType = Constants::getAssetTypeEnumFromString((*it)->getType());
            if (thisAssetType == type)
            {
                definitionsList.push_back((*it));
            }
        }
        return definitionsList;
    }


    SceneDefinition*
    ProjectDefinition::createNewSceneDefinition
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        json scene;
        scene[Constants::UUID] = Uuid::generateUuid();
        scene[Constants::NAME] = Constants::SCENE_DEFAULT_NAME;
        Transform camTransform;
        scene[Constants::SCENE_CAMERA_TRANSFORM] = camTransform.getJson();
        auto sd = new SceneDefinition(this, scene);
        sd->createNewRootEntityDefinition();
        mSceneDefinitions.push_back(sd);
        return sd;
    }

    AssetDefinition*
    ProjectDefinition::createNewAssetDefinition
    (AssetType type)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        json assetDefinitionJson;

        string defaultFormat = (*Constants::DREAM_ASSET_FORMATS_MAP.at(type).begin());
        LOG_DEBUG("ProjectDefinition: Creating new AssetDefinition with default Format {}", defaultFormat);

        assetDefinitionJson[Constants::NAME] = Constants::ASSET_DEFINITION_DEFAULT_NAME;
        assetDefinitionJson[Constants::UUID] = Uuid::generateUuid();
        assetDefinitionJson[Constants::ASSET_TYPE] = Constants::getAssetTypeStringFromTypeEnum(type);
        assetDefinitionJson[Constants::ASSET_FORMAT] = defaultFormat;
        AssetDefinition* ad = createAssetDefinition(assetDefinitionJson);
        mAssetDefinitions.push_back(ad);
        return ad;
    }

    SceneDefinition*
    ProjectDefinition::getStartupSceneDefinition
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        UuidType startupScene = getStartupSceneUuid();
        LOG_DEBUG("ProjectDefinition: Finding startup scene {}", startupScene);
        return getSceneDefinitionByUuid(startupScene);
    }

    json
    ProjectDefinition::getJson
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        mJson[Constants::PROJECT_ASSET_ARRAY] = json::array();
        for (AssetDefinition* ad : getAssetDefinitionsVector())
        {
            mJson[Constants::PROJECT_ASSET_ARRAY].push_back(ad->getJson());
        }

        mJson[Constants::PROJECT_SCENE_ARRAY] = json::array();
        for (SceneDefinition* sd : getSceneDefinitionsVector())
        {
            mJson[Constants::PROJECT_SCENE_ARRAY].push_back(sd->getJson());
        }

        return mJson;
    }

    map<AssetType, vector<AssetDefinition*> >
    ProjectDefinition::getAssetDefinitionsMap
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        vector<AssetDefinition*> ads = getAssetDefinitionsVector();
        map<AssetType, vector<AssetDefinition*> > handlesMap;

        auto begins = begin(ads);
        auto ends = end(ads);
        auto current = begins;

        auto endMap = end(handlesMap);

        // Iterate over AD s
        while (current != ends)
        {
            // Current AD type
            AssetType currentType = Constants::getAssetTypeEnumFromString((*current)->getType());
            // Find type vector in map
            auto typeVector = handlesMap.find(currentType);
            // Vector does not exist
            if (typeVector == endMap)
            {
                // Create it
                vector<AssetDefinition*> typeVector;
                handlesMap.insert(
                            std::pair<AssetType, vector<AssetDefinition*>>
                            (currentType, typeVector)
                            );
            }
            handlesMap.at(currentType).push_back((*current));
            current++;
        }
        return handlesMap;
    }

    void ProjectDefinition::deleteAssetDefinitions()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        for (auto ad : mAssetDefinitions)
        {
            delete ad;
        }
        mAssetDefinitions.clear();
    }

    void ProjectDefinition::deleteSceneDefinitions()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        for (auto sd : mSceneDefinitions)
        {
            delete sd;
        }
        mSceneDefinitions.clear();
    }

    long
    ProjectDefinition::getSceneDefinitionIndex
    (SceneDefinition* sDef)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        vector<SceneDefinition*> defs = getSceneDefinitionsVector();
        auto it = std::find(defs.begin(), defs.end(), sDef);
        if (it == defs.end())
        {
            return -1;
        }
        else
        {
            return std::distance(defs.begin(), it);
        }
    }

    SceneDefinition*
    ProjectDefinition::getSceneDefinitionAtIndex
    (int index)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        return getSceneDefinitionsVector().at(index);
    }

    long
    ProjectDefinition::getAssetDefinitionIndex
    (AssetType type, AssetDefinition* sDef)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        vector<AssetDefinition*> defs = getAssetDefinitionsVector(type);
        auto it = std::find(defs.begin(), defs.end(), sDef);
        if (it == defs.end())
        {
            return -1;
        }
        else
        {
            return std::distance(defs.begin(), it);
        }
    }

    long
    ProjectDefinition::getAssetDefinitionIndex
    (AssetType type, UuidType uuid)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        vector<AssetDefinition*> defs = getAssetDefinitionsVector(type);
        for (int i = 0; i < defs.size(); i++)
        {
            if (defs.at(i)->getUuid() == uuid) return i;
        }
        return -1;
    }

    AssetDefinition*
    ProjectDefinition::getAssetDefinitionAtIndex
    (AssetType type, int idx)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        return getAssetDefinitionsVector(type).at(idx);
    }

    vector<string>
    ProjectDefinition::getAssetNamesVector
    (AssetType type)
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        vector<string> retval;
        auto assets = getAssetDefinitionsVector(type);
        for (auto asset : assets)
        {
            retval.push_back(asset->getName());
        }
        return retval;
    }

    void
    ProjectDefinition::regroupAssetDefinitions
    ()
    {
        const unique_lock<mutex> lg(getMutex(), std::adopt_lock);
        if(!lg.owns_lock()) getMutex().lock();

        mAssetDefinitionGroups.clear();
        for (const auto& typePair : Constants::DREAM_ASSET_TYPES_MAP)
        {
            mAssetDefinitionGroups.insert(
                pair<AssetType,vector<string> >(
                    typePair.first,
                    vector<string>()
                )
            );
        }

        for (auto ad : mAssetDefinitions)
        {
            auto group = ad->getGroup();
            auto& groups = mAssetDefinitionGroups[Constants::getAssetTypeEnumFromString(ad->getType())];
            if (find(groups.begin(),groups.end(), group) == groups.end())
            {
                groups.push_back(group);
            }
        }
    }

    // Static ==================================================================

    ProjectDefinition*
    ProjectDefinition::createNewProjectDefinition
    (const string &name)
    {
        json j = json::object();
        j[Constants::NAME] = name;
        j[Constants::UUID] = Uuid::generateUuid();
        j[Constants::PROJECT_AUTHOR] = "";
        j[Constants::PROJECT_DESCRIPTION] = "";
        j[Constants::PROJECT_STARTUP_SCENE] = "";
        j[Constants::PROJECT_ASSET_ARRAY] = json::array();
        j[Constants::PROJECT_SCENE_ARRAY] = json::array();
        return new ProjectDefinition(j);
    }
}
