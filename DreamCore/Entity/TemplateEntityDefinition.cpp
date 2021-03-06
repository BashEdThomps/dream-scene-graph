#include "TemplateEntityDefinition.h"

#include "Common/Constants.h"
#include "Common/Logger.h"
#include "Common/Uuid.h"

#include "EntityRuntime.h"
#include "Scene/SceneDefinition.h"
#include "Project/ProjectDefinition.h"

#include "Components/AssetDefinition.h"
#include "Math/Transform.h"
#include "Math/Vector.h"

#include <regex>

using std::regex;
using std::cmatch;
using std::pair;
using std::make_unique;

namespace octronic::dream
{
  TemplateEntityDefinition::TemplateEntityDefinition
  (ProjectDefinition& pDef, const json& jsonData)
    : Definition(jsonData),
      mProjectDefinition(pDef)
  {
  }

  ProjectDefinition&
  TemplateEntityDefinition::getProjectDefinition
  ()
  {
    return mProjectDefinition;
  }

  json
  TemplateEntityDefinition::getJson
  ()
  {
    return mJson;
  }

  TemplateEntityDefinition&
  TemplateEntityDefinition::duplicateDefinition
  ()
  {
    auto& newEntityDef = mProjectDefinition.get().createTemplateEntityDefinition();
    newEntityDef.setJson(mJson);
    newEntityDef.setUuid(Uuid::RandomUuid());
    string name = newEntityDef.getName();
    regex numRegex("(\\d+)$");
    cmatch match;
    string resultStr;
    auto num = -1;

    if (regex_search(name.c_str(),match,numRegex))
    {
      resultStr = match[0].str();
      num = atoi(resultStr.c_str());
    }

    if (num > -1)
    {
      num++;
      name = name.substr(0,name.length()-resultStr.length());
      name.append(std::to_string(num));
      newEntityDef.setName(name);
    }
    else
    {
      newEntityDef.setName(getName()+".1");
    }
    return newEntityDef;
    throw std::exception();
  }

  int
  TemplateEntityDefinition::getSelectedAssetDefinitionIndex
  (AssetType t)
  const
  {
    auto& pd = mProjectDefinition.get();
    auto assetDefOpt = pd.getAssetDefinitionByUuid(t, getAssetDefinition(t));
    if (assetDefOpt)
    {
    	return pd.getAssetDefinitionIndex(assetDefOpt.value().get());
    }
    return -1;
  }

  void
  TemplateEntityDefinition::setSelectedAssetDefinitionIndex
  (AssetType type, unsigned long index)
  {
    auto& pd = mProjectDefinition.get();
    auto typesVector = pd.getAssetDefinitionsVector(type);
    auto& asset = typesVector.at(index);
    setAssetDefinition(type, asset.get().getUuid());
  }

  void
  TemplateEntityDefinition::setAssetDefinition
  (AssetType type, UuidType uuid)
  {
    auto typeStr = AssetTypeHelper::GetAssetTypeStringFromTypeEnum(type);

    if (mJson.find(Constants::TEMPLATE_ENTITY_ASSET_INSTANCES) == mJson.end())
    {
      mJson[Constants::TEMPLATE_ENTITY_ASSET_INSTANCES] = json::object();
    }
    mJson[Constants::TEMPLATE_ENTITY_ASSET_INSTANCES][typeStr] = uuid;
  }

  map<AssetType, UuidType>
  TemplateEntityDefinition::getAssetDefinitionsMap
  ()
  const
  {
    map<AssetType, UuidType> assetsMap;
    for (const auto& typePair : AssetTypeHelper::TypesMap)
    {
      AssetType type = typePair.first;
      UuidType uuid = getAssetDefinition(type);
      assetsMap.insert(pair<AssetType,UuidType>(type, uuid));
    }
    return assetsMap;
  }

  UuidType
  TemplateEntityDefinition::getAssetDefinition
  (AssetType type)
  const
  {
    auto typeStr = AssetTypeHelper::GetAssetTypeStringFromTypeEnum(type);

    if (mJson.find(Constants::TEMPLATE_ENTITY_ASSET_INSTANCES) == mJson.end())
    {
      return Uuid::INVALID;
    }

    if (mJson[Constants::TEMPLATE_ENTITY_ASSET_INSTANCES].find(typeStr) ==
        mJson[Constants::TEMPLATE_ENTITY_ASSET_INSTANCES].end())
    {
      return Uuid::INVALID;
    }

    LOG_TRACE("EntityDefinition: Found {} Runtime",typeStr);

    return mJson[Constants::TEMPLATE_ENTITY_ASSET_INSTANCES][typeStr];
  }
}
