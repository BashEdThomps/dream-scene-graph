/*
 * Transform3D.cpp
 *
 * Created: 22/11/2016 2016 by Ashley
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
#include "Transform3D.h"

#include "../Common/Constants.h"

namespace Dream
{

    Transform3D::Transform3D
    ()
    {
        mTransformType = Constants::TRANSFORM_TYPE_ABSOLUTE;
        mTranslation   = vec3(0.0f);
        mScale         = vec3(0.0f);
        mOrientation   = quat();
    }

    Transform3D::Transform3D
    (Transform3D* obj)
    {
        mTransformType = obj->getTransformType();
        mTranslation = obj->getTranslation();
        mOrientation = obj->getOrientation();
        mScale =obj->getScale();
    }

    // Translation ===================================================================

    vec3
    Transform3D::getTranslation
    ()
    {
        return mTranslation;
    }

    void
    Transform3D::setTranslation
    (vec3 translation)
    {
        mTranslation = translation;
    }

    void
    Transform3D::setTranslation
    (float x, float y, float z)
    {
        mTranslation.x = x;
        mTranslation.y = y;
        mTranslation.z = z;
    }

    void
    Transform3D::setTranslationX
    (float x)
    {
        mTranslation.x = x;
    }

    void
    Transform3D::setTranslationY
    (float y)
    {
        mTranslation.y = y;
    }

    void
    Transform3D::setTranslationZ
    (float z)
    {
        mTranslation.z = z;
    }

    float
    Transform3D::getTranslationX
    ()
    {
        return mTranslation.x;
    }

    float
    Transform3D::getTranslationY
    ()
    {
        return mTranslation.y;
    }

    float
    Transform3D::getTranslationZ
    ()
    {
        return mTranslation.z;
    }

    // Rotation ======================================================================

    vec3
    Transform3D::getRotation
    ()
    {
        vec3 rotation(3);
        vec3 euler = eulerAngles(mOrientation);
        rotation[0] = euler.x;
        rotation[1] = euler.y;
        rotation[2] = euler.z;
        return rotation;
    }

    void
    Transform3D::setRotation
    (vec3 rot)
    {
        mOrientation = quat(rot);
    }

    void
    Transform3D::setRotation
    (float x, float y, float z)
    {
        mOrientation = quat(vec3(x,y,z));
    }

    float
    Transform3D::getRotationX
    ()
    {
        return eulerAngles(mOrientation).x;
    }

    float
    Transform3D::getRotationY
    ()
    {
        return eulerAngles(mOrientation).y;
    }

    float
    Transform3D::getRotationZ
    ()
    {
        return eulerAngles(mOrientation).z;
    }

    void
    Transform3D::setRotationX
    (float x)
    {
        vec3 euler = eulerAngles(mOrientation);
        mOrientation = quat(vec3(x,euler.y,euler.z));
    }

    void
    Transform3D::setRotationY
    (float y)
    {
        vec3 euler = eulerAngles(mOrientation);
        mOrientation = quat(vec3(euler.x,y,euler.z));
    }

    void
    Transform3D::setRotationZ
    (float z)
    {
        vec3 euler = eulerAngles(mOrientation);
        mOrientation = quat(vec3(euler.x,euler.y,z));
    }

    // Scale =========================================================================

    vec3
    Transform3D::getScale
    ()
    {
        return mScale;
    }

    void
    Transform3D::setScale
    (vec3 scale)
    {
        mScale = scale;
    }

    void
    Transform3D::setScale
    (float x, float y, float z)
    {
        mScale = vec3(x,y,z);
    }

    float
    Transform3D::getScaleX
    ()
    {
        return mScale.x;
    }

    float
    Transform3D::getScaleY
    ()
    {
        return mScale.y;
    }

    float
    Transform3D::getScaleZ
    ()
    {
        return mScale.z;
    }

    void
    Transform3D::setScaleX
    (float x)
    {
        mScale.x = x;
    }

    void
    Transform3D::setScaleY
    (float y)
    {
        mScale.y = y;
    }

    void
    Transform3D::setScaleZ
    (float z)
    {
        mScale.z = z;
    }

    void
    Transform3D::translateByX
    (float delta)
    {
        mTranslation.x += delta;
    }

    void
    Transform3D::translateByY
    (float delta)
    {
        mTranslation.y += delta;
    }

    void
    Transform3D::translateByZ
    (float delta)
    {
        mTranslation.z += delta;
    }

    void
    Transform3D::rotateByX
    (float delta)
    {
        vec3 euler = eulerAngles(mOrientation);
        mOrientation = quat(vec3(euler.x+delta,euler.y,euler.z));
    }

    void
    Transform3D::rotateByY
    (float delta)
    {
        vec3 euler = eulerAngles(mOrientation);
        mOrientation = quat(vec3(euler.x,euler.y+delta,euler.z));
    }

    void
    Transform3D::rotateByZ
    (float delta)
    {
        vec3 euler = eulerAngles(mOrientation);
        mOrientation = quat(vec3(euler.x,euler.y,euler.z+delta));
    }

    void
    Transform3D::scaleByX
    (float delta)
    {
        mScale.x += delta;
    }

    void
    Transform3D::scaleByY
    (float delta)
    {
        mScale.y += delta;
    }

    void
    Transform3D::scaleByZ
    (float delta)
    {
        mScale.z += delta;
    }

    // Transform Type ================================================================

    void
    Transform3D::setTransformType
    (string type)
    {
        mTransformType = type;
    }

    string
    Transform3D::getTransformType
    ()
    {
        return mTransformType;
    }

    btVector3
    Transform3D::getTranslationAsBtVector3
    ()
    {
        return btVector3(getTranslationX(),getTranslationY(),getTranslationZ());
    }

    btVector3
    Transform3D::getRotationAsBtVector3
    ()
    {
        return btVector3(getRotationX(),getRotationY(),getRotationZ());
    }

    quat
    Transform3D::getOrientation
    ()
    {
        return mOrientation;
    }

    void
    Transform3D::setOrientation
    (float w, float x, float y, float z)
    {
        mOrientation = quat(w,x,y,z);
    }

    json
    Transform3D::getJson
    ()
    {
        return mJson;
    }

} // End of Dream