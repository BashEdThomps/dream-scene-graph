/*
 * SceneObjectPropertiesItem.cpp
 *
 * Created: 29 2017 by Ashley
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
#include "SceneObjectPropertiesItem.h"

#include <DreamCore.h>
#include <QVariant>

using Dream::Constants;

SceneObjectPropertiesItem::SceneObjectPropertiesItem
(
    QString name,
    SceneObjectDefinition* sodHandle,
    SceneObjectProperty property,
    QItemDelegate* delegate,
    AbstractPropertiesItem *parent
)
    : AbstractPropertiesItem(name,delegate,parent),
      mProperty(property),
      mSceneObjectDefinitionHandle(sodHandle)
{
}

SceneObjectPropertiesItem::~SceneObjectPropertiesItem
()
{

}

SceneObjectDefinition*
SceneObjectPropertiesItem::getSceneObjectDefinitionHandle
()
{
    return mSceneObjectDefinitionHandle;
}

SceneObjectProperty
SceneObjectPropertiesItem::getProperty
()
{
   return mProperty;
}

bool
SceneObjectPropertiesItem::setData
(int column,const  QVariant &value)
{
    if (column == 0)
    {
        return false;
    }

    switch(getProperty())
    {
        case SCENE_OBJECT_PROPERTY_ALWAYS_DRAW:
            mSceneObjectDefinitionHandle->setAlwaysDraw(value.toBool());
            break;
        case SCENE_OBJECT_PROPERTY_CHILD:
            break;
        case SCENE_OBJECT_PROPERTY_ASSET_DEFINITION:
            break;
        case SCENE_OBJECT_PROPERTY_TRANSLATION_CAPTURE:
            break;
        case SCENE_OBJECT_PROPERTY_ORIENTATION_CAPTURE:
            break;
        case SCENE_OBJECT_PROPERTY_SCALE_CAPTURE:
            break;
        case SCENE_OBJECT_PROPERTY_UUID:
            mSceneObjectDefinitionHandle->setUuid(value.toString().toStdString());
            break;
        case SCENE_OBJECT_PROPERTY_NAME:
            mSceneObjectDefinitionHandle->setName(value.toString().toStdString());
            break;
        case SCENE_OBJECT_PROPERTY_TRANSLATION_X:
            mSceneObjectDefinitionHandle->getTransform()->setTranslationX(value.toFloat());
            break;
        case SCENE_OBJECT_PROPERTY_TRANSLATION_Y:
            mSceneObjectDefinitionHandle->getTransform()->setTranslationY(value.toFloat());
            break;
        case SCENE_OBJECT_PROPERTY_TRANSLATION_Z:
            mSceneObjectDefinitionHandle->getTransform()->setTranslationZ(value.toFloat());
            break;
        case SCENE_OBJECT_PROPERTY_ORIENTATION_W:
            mSceneObjectDefinitionHandle->getTransform()->setOrientationW(value.toFloat());
            break;
        case SCENE_OBJECT_PROPERTY_ORIENTATION_X:
            mSceneObjectDefinitionHandle->getTransform()->setOrientationX(value.toFloat());
            break;
        case SCENE_OBJECT_PROPERTY_ORIENTATION_Y:
            mSceneObjectDefinitionHandle->getTransform()->setOrientationY(value.toFloat());
            break;
        case SCENE_OBJECT_PROPERTY_ORIENTATION_Z:
            mSceneObjectDefinitionHandle->getTransform()->setOrientationZ(value.toFloat());
            break;
        case SCENE_OBJECT_PROPERTY_SCALE_X:
            mSceneObjectDefinitionHandle->getTransform()->setScaleX(value.toFloat());
            break;
        case SCENE_OBJECT_PROPERTY_SCALE_Y:
            mSceneObjectDefinitionHandle->getTransform()->setScaleY(value.toFloat());
            break;
        case SCENE_OBJECT_PROPERTY_SCALE_Z:
            mSceneObjectDefinitionHandle->getTransform()->setScaleZ(value.toFloat());
            break;
        case SCENE_OBJECT_PROPERTY_TRANSFORM_TYPE:
            mSceneObjectDefinitionHandle->getTransform()->setTransformType(value.toString().toStdString());
            break;
        case SCENE_OBJECT_PROPERTY_HAS_FOCUS:
            mSceneObjectDefinitionHandle->setHasFocus(value.toBool());
            break;
        case SCENE_OBJECT_PROPERTY_FOLLOWS_CAMERA:
            mSceneObjectDefinitionHandle->setFollowsCamera(value.toBool());
            break;
        case SCENE_OBJECT_PROPERTY_STATIC:
            mSceneObjectDefinitionHandle->setStatic(value.toBool());
            break;
        case SCENE_OBJECT_PROPERTY_NONE:
        case SCENE_OBJECT_PROPERTY_ASSET_DEFINITION_LIST:
        case SCENE_OBJECT_PROPERTY_CHILD_LIST:
            break;
    }

    return true;
}

QVariant
SceneObjectPropertiesItem::data
(int column)
{
    if (column == 0 )
    {
        if (getProperty() == SCENE_OBJECT_PROPERTY_NONE)
        {
            return QString::fromStdString(mSceneObjectDefinitionHandle->getUuid());
        }
        return QVariant(mTitle);
    }

    switch(getProperty())
    {
        case SCENE_OBJECT_PROPERTY_ALWAYS_DRAW:
            return QVariant(mSceneObjectDefinitionHandle->getAlwaysDraw());
        case SCENE_OBJECT_PROPERTY_ASSET_DEFINITION:
            return QVariant(QString::fromStdString(Constants::getAssetTypeReadableNameFromString(getTargetAssetDefinitionHandle()->getType())));
        case SCENE_OBJECT_PROPERTY_UUID:
            return QString::fromStdString(mSceneObjectDefinitionHandle->getUuid());
        case SCENE_OBJECT_PROPERTY_NAME:
            return QString::fromStdString(mSceneObjectDefinitionHandle->getName());
        case SCENE_OBJECT_PROPERTY_TRANSLATION_X:
            return QVariant(mSceneObjectDefinitionHandle->getTransform()->getTranslationX());
        case SCENE_OBJECT_PROPERTY_TRANSLATION_Y:
            return QVariant(mSceneObjectDefinitionHandle->getTransform()->getTranslationY());
        case SCENE_OBJECT_PROPERTY_TRANSLATION_Z:
            return QVariant(mSceneObjectDefinitionHandle->getTransform()->getTranslationZ());
        case SCENE_OBJECT_PROPERTY_ORIENTATION_W:
            return QVariant(mSceneObjectDefinitionHandle->getTransform()->getOrientationW());
        case SCENE_OBJECT_PROPERTY_ORIENTATION_X:
            return QVariant(mSceneObjectDefinitionHandle->getTransform()->getOrientationX());
        case SCENE_OBJECT_PROPERTY_ORIENTATION_Y:
            return QVariant(mSceneObjectDefinitionHandle->getTransform()->getOrientationY());
        case SCENE_OBJECT_PROPERTY_ORIENTATION_Z:
            return QVariant(mSceneObjectDefinitionHandle->getTransform()->getOrientationZ());
        case SCENE_OBJECT_PROPERTY_SCALE_X:
            return QVariant(mSceneObjectDefinitionHandle->getTransform()->getScaleX());
        case SCENE_OBJECT_PROPERTY_SCALE_Y:
            return QVariant(mSceneObjectDefinitionHandle->getTransform()->getScaleY());
        case SCENE_OBJECT_PROPERTY_SCALE_Z:
            return QVariant(mSceneObjectDefinitionHandle->getTransform()->getScaleZ());
        case SCENE_OBJECT_PROPERTY_TRANSFORM_TYPE:
            return QString::fromStdString(mSceneObjectDefinitionHandle->getTransform()->getTransformType());
        case SCENE_OBJECT_PROPERTY_HAS_FOCUS:
            return QVariant(mSceneObjectDefinitionHandle->hasFocus());
        case SCENE_OBJECT_PROPERTY_FOLLOWS_CAMERA:
            return QVariant(mSceneObjectDefinitionHandle->followsCamera());
        case SCENE_OBJECT_PROPERTY_STATIC:
            return QVariant(mSceneObjectDefinitionHandle->getStatic());

        case SCENE_OBJECT_PROPERTY_CHILD:
        case SCENE_OBJECT_PROPERTY_TRANSLATION_CAPTURE:
        case SCENE_OBJECT_PROPERTY_ORIENTATION_CAPTURE:
        case SCENE_OBJECT_PROPERTY_SCALE_CAPTURE:
        case SCENE_OBJECT_PROPERTY_CHILD_LIST:
        case SCENE_OBJECT_PROPERTY_ASSET_DEFINITION_LIST:
        case SCENE_OBJECT_PROPERTY_NONE:
            return QVariant();

    }
    return QVariant();
}

SceneObjectDefinition*
SceneObjectPropertiesItem::getTargetSceneObjectDefinitionHandle
()
{
    return mTargetSceneObjectDefinitionHandle;
}

void
SceneObjectPropertiesItem::setTargetSceneObjectDefinitionHandle
(SceneObjectDefinition* targetSceneObjectDefinitionHandle)
{
    mTargetSceneObjectDefinitionHandle = targetSceneObjectDefinitionHandle;
}

IAssetDefinition*
SceneObjectPropertiesItem::getTargetAssetDefinitionHandle
()
{
    return mTargetAssetDefinitionHandle;
}

void
SceneObjectPropertiesItem::setTargetAssetDefinitionHandle
(IAssetDefinition* targetAssetDefinitionHandle)
{
    mTargetAssetDefinitionHandle = targetAssetDefinitionHandle;
}
