/*
 * TextureCache.cpp
 *
 * Created: 30/11/2016 2016 by Ashley
 *
 * Copyright 2016 Octronic. All rights reserved.
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

#include "MaterialCache.h"

#include "MaterialDefinition.h"
#include "MaterialRuntime.h"
#include "Common/Logger.h"
#include "Components/Graphics/Texture/TextureDefinition.h"
#include "Components/Graphics/Texture/TextureRuntime.h"
#include "Components/Graphics/Texture/TextureCache.h"
#include "Components/Graphics/Shader/ShaderRuntime.h"
#include "Components/Graphics/Shader/ShaderCache.h"
#include "Project/Project.h"
#include "Project/ProjectDirectory.h"
#include "Project/ProjectRuntime.h"

namespace Dream
{
    MaterialCache::MaterialCache
    (ProjectRuntime* parent, ShaderCache* shaderCache, TextureCache* textureCache)
        : Cache(parent),
          mShaderCache(shaderCache),
          mTextureCache(textureCache)
    {
        LOG_TRACE( "Constructing" );
    }

    MaterialCache::~MaterialCache
    ()
    {
        LOG_TRACE( "Destructing" );
    }

    SharedAssetRuntime*
    MaterialCache::loadRuntime
    (AssetDefinition* def)
    {
        if (!def)
        {
            LOG_ERROR("Material Definition is null");
            return nullptr;
        }
        auto matDef = static_cast<MaterialDefinition*>(def);
        auto shader = static_cast<ShaderRuntime*>(mShaderCache->getRuntime(matDef->getShader()));

        if (shader == nullptr)
        {
            LOG_ERROR("Cannot create material {} with null shader", matDef->getNameAndUuidString());
            return nullptr;
        }

        auto material = new MaterialRuntime(matDef,mProjectRuntime);
        auto diffuse = static_cast<TextureRuntime*>(mTextureCache->getRuntime(matDef->getDiffuseTexture()));
        auto specular = static_cast<TextureRuntime*>(mTextureCache->getRuntime(matDef->getSpecularTexture()));
        auto normal = static_cast<TextureRuntime*>(mTextureCache->getRuntime(matDef->getNormalTexture()));
        auto displacement = static_cast<TextureRuntime*>(mTextureCache->getRuntime(matDef->getDisplacementTexture()));
        material->useDefinition();

        material->setDiffuseTexture(diffuse);
        material->setSpecularTexture(specular);
        material->setNormalTexture(normal);
        material->setDisplacementTexture(displacement);
        material->setShader(shader);
        shader->addMaterial(material);
        mRuntimes.push_back(material);
        return material;
    }
} // End of Dream
