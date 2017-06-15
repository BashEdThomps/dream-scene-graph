/*
* AssetDefinition
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

#include <strings.h>
#include "AssetDefinition.h"

namespace Dream
{
    AssetDefinition::AssetDefinition
    (json jsonDef)
    {
        if (Constants::DEBUG)
        {
            cout << "AssetDefinition: Constructing Object" << endl;
        }
        mJson = jsonDef;
        showStatus();
    }

    AssetDefinition::~AssetDefinition
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "AssetDefinition: Destroying Object" << endl;
        }
    }

    void
    AssetDefinition::setProjectPath
    (string path)
    {
        mProjectPath = path;
    }

    string
    AssetDefinition::getProjectPath
    ()
    {
        return mProjectPath;
    }

    void
    AssetDefinition::setUuid
    (string uuid)
    {
        mJson[Constants::ASSET_UUID] = uuid;
    }

    bool
    AssetDefinition::hasUuid
    (string uuid)
    {
        string s = mJson[Constants::ASSET_UUID];
        return s.compare(uuid) == 0;
    }

    string
    AssetDefinition::getUuid
    ()
    {
        return mJson[Constants::ASSET_UUID];
    }

    void
    AssetDefinition::setName
    (string name)
    {
        mJson[Constants::ASSET_NAME] = name;
    }

    string
    AssetDefinition::getName
    ()
    {
        return mJson[Constants::ASSET_NAME];
    }

    void
    AssetDefinition::setType
    (string type)
    {
        mJson[Constants::ASSET_TYPE] = type;
    }

    string
    AssetDefinition::getType
    ()
    {
        return mJson[Constants::ASSET_TYPE];
    }

    void AssetDefinition::setFormat(string format)
    {
        mJson[Constants::ASSET_FORMAT] = format;
    }

    string
    AssetDefinition::getFormat
    ()
    {
        return mJson[Constants::ASSET_FORMAT];
    }

    bool
    AssetDefinition::isTypeLight
    ()
    {
        return getType().compare(Constants::ASSET_TYPE_LIGHT) == 0;
    }

    bool
    AssetDefinition::isTypeFont
    ()
    {
        return getType().compare(Constants::ASSET_TYPE_FONT) == 0;
    }

    bool
    AssetDefinition::isTypePhysicsObject
    ()
    {
        return getType().compare(Constants::ASSET_TYPE_PHYSICS_OBJECT) == 0;
    }

    bool
    AssetDefinition::isTypeAnimation
    ()
    {
        return getType().compare(Constants::ASSET_TYPE_ANIMATION) == 0;
    }

    bool
    AssetDefinition::isTypeAudio
    ()
    {
        return getType().compare(Constants::ASSET_TYPE_AUDIO) == 0;
    }

    bool
    AssetDefinition::isTypeModel
    ()
    {
        return getType().compare(Constants::ASSET_TYPE_MODEL) == 0;
    }

    bool
    AssetDefinition::isTypeScript
    ()
    {
        return getType().compare(Constants::ASSET_TYPE_SCRIPT) == 0;
    }

    bool
    AssetDefinition::isTypeShader
    ()
    {
        return getType().compare(Constants::ASSET_TYPE_SHADER) == 0;
    }

    bool
    AssetDefinition::isTypeSprite
    ()
    {
        return getType().compare(Constants::ASSET_TYPE_SPRITE) == 0;
    }

    bool
    AssetDefinition::isScriptFormatLua
    ()
    {
        return getFormat().compare(Constants::ASSET_FORMAT_SCRIPT_LUA) == 0;
    }

    bool
    AssetDefinition::isModelFormatAssimp
    ()
    {
        return getFormat().compare(Constants::ASSET_FORMAT_MODEL_ASSIMP) == 0;
    }

    bool
    AssetDefinition::isAudioFormatWav
    ()
    {
        return getFormat().compare(Constants::ASSET_FORMAT_AUDIO_WAV) == 0;
    }

    bool
    AssetDefinition::isAudioFormatOgg
    ()
    {
        return getFormat().compare(Constants::ASSET_FORMAT_AUDIO_OGG) == 0;
    }

    string
    AssetDefinition::getAssetTypeDirectory
    ()
    {
        if (isTypeAnimation())
        {
            return Constants::ASSET_TYPE_ANIMATION;
        }
        else if (isTypeAudio())
        {
            return Constants::ASSET_TYPE_AUDIO;
        }
        else if (isTypeModel())
        {
            return Constants::ASSET_TYPE_MODEL;
        }
        else if (isTypeScript())
        {
            return Constants::ASSET_TYPE_SCRIPT;
        }
        else if (isTypeShader())
        {
            return Constants::ASSET_TYPE_SHADER;
        }
        else if (isTypeSprite())
        {
            return Constants::ASSET_TYPE_SPRITE;
        }
        else if (isTypeFont())
        {
            return Constants::ASSET_TYPE_FONT;
        }
        else if (isTypePhysicsObject())
        {
            return Constants::ASSET_TYPE_PHYSICS_OBJECT;
        }
        else
        {
            return "Type Not Found";
        }
    }

    string
    AssetDefinition::getNameAndUuidString
    ()
    {
        return getName() + " (" + getUuid() + ")";
    }

    string
    AssetDefinition::getAssetPath
    ()
    {
        stringstream pathStream;
        pathStream << Constants::DIR_PATH_SEP
                   << Constants::ASSET_DIR << Constants::DIR_PATH_SEP
                   << getAssetTypeDirectory() << Constants::DIR_PATH_SEP
                   << getUuid() << Constants::DIR_PATH_SEP
                   << getFormat();
        return pathStream.str();
    }

    void
    AssetDefinition::showStatus
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "AssetDefinition: " << mJson.dump(1) << endl;
        }
    }

    json&
    AssetDefinition::getJson
    ()
    {
        return mJson;
    }

    AssetDefinition*
    AssetDefinition::getAssetDefinitionByUuid
    (vector<AssetDefinition*> definitions, string uuid)
    {
        for (AssetDefinition* ad : definitions)
        {
            if (ad->hasUuid(uuid))
            {
                return ad;
            }
        }
        return nullptr;
    }

} // End of Dream