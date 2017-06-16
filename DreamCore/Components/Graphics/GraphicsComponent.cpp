/*
* GraphicsComponent
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

#include "GraphicsComponent.h"

#include <functional>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Camera.h"

#include "Font/FontInstance.h"
#include "Font/FontCharacter.h"

#include "Light/LightInstance.h"

#include "Model/AssimpModelInstance.h"
#include "Model/TextureCache.h"

#include "Shader/ShaderInstance.h"
#include "Shader/ShaderCache.h"

#include "Sprite/SpriteInstance.h"

#include "../Transform3D.h"

#include "../Window/IWindowComponent.h"

#include "../../Common/Constants.h"

#include "../../Scene/Scene.h"
#include "../../Scene/SceneRuntime.h"
#include "../../Scene/SceneJsonData.h"
#include "../../Scene/SceneObject/SceneObject.h"
#include "../../Scene/SceneObject/SceneObjectRuntime.h"
#include "../../Scene/SceneObject/SceneObjectJsonData.h"

using glm::vec3;
using glm::mat4;
using glm::rotate;
using glm::translate;
using glm::scale;

namespace Dream
{

    GraphicsComponent::GraphicsComponent
    (Camera* camera, IWindowComponent* windowComponent)
        : IComponent(),
          mCamera(camera),
          mWindowComponentHandle(windowComponent),
          mActiveSceneHandle(nullptr)
    {

    }

    GraphicsComponent::~GraphicsComponent
    (void)
    {
        if (Constants::DEBUG)
        {
            cout << "GraphicsComponent: Destroying Object" << endl;
        }
    }

    bool
    GraphicsComponent::init
    (void)
    {
        if (Constants::DEBUG)
        {
            cout << "GraphicsComponent: Initialising" << endl;
            cout << "GraphicsComponent: Initialising GLEW" << endl;
        }

        glewExperimental = GL_TRUE;
        GLenum glewInitResult = glewInit();

        if (glewInitResult != GLEW_OK)
        {
            cerr << "GraphicsComponent: GLEW failed to initialise." << endl;
            return false;
        }

        if (Constants::DEBUG)
        {
            cout << "GraphicsComponent: OpenGL Version " << glGetString(GL_VERSION) << endl;
            cout << "GraphicsComponent: Shader Version " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
        }

        onWindowDimensionsChanged();

        glEnable(GL_DEPTH_TEST);
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

        create2DVertexObjects();
        createFontVertexObjects();

        if (Constants::DEBUG)
        {
            cout << "GraphicsComponent: Initialisation Done." << endl;
        }
        return true;
    }

    void
    GraphicsComponent::onWindowDimensionsChanged
    ()
    {

        // Define the viewport dimensions
        int windowWidth  = mWindowComponentHandle->getWidth();
        int windowHeight = mWindowComponentHandle->getHeight();

        if (Constants::DEBUG)
        {
            cout << "GraphicsComponwnt: Window Dimensions Changed! "
                 << windowWidth << "," << windowHeight << endl;
        }

        glViewport(0, 0, windowWidth, windowHeight);

        // Ortho projection for 2D
        mOrthoProjection = ortho
        (
            0.0f,
            static_cast<float>(windowWidth),
            static_cast<float>(windowHeight),
            0.0f,
            -1.0f, 1.0f
        );
        // Perspective Projection Matrix
        mProjectionMatrix = perspective(
            mCamera->getZoom(),
            static_cast<float>(windowWidth)/static_cast<float>(windowHeight),
            mMinimumDraw,
            mMaximumDraw
        );

        if (Constants::VERBOSE)
        {
            cout << "GraphicsComponent: Window dimensions changed" << endl
                 << "\tWindowWidth:" << mWindowComponentHandle->getWidth() << endl
                 << "\tWindowHeight:" << mWindowComponentHandle->getHeight() << endl
                 << "\tMinDraw: " << mMinimumDraw << endl
                 << "\tMaxDraw: " << mMaximumDraw << endl;
        }
    }

    void
    GraphicsComponent::preRender
    ()
    {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        // Clear the colorbuffer
        if (mActiveSceneHandle)
        {
            glClearColor(
                mActiveSceneHandle->getRuntime()->getClearColour()[Constants::RED_INDEX],
                mActiveSceneHandle->getRuntime()->getClearColour()[Constants::GREEN_INDEX],
                mActiveSceneHandle->getRuntime()->getClearColour()[Constants::BLUE_INDEX],
                mActiveSceneHandle->getRuntime()->getClearColour()[Constants::ALPHA_INDEX]
            );
        }
        else
        {
            glClearColor(0.0f,0.0f,0.0f,0.0f);
        }
    }

    void
    GraphicsComponent::postRender
    ()
    {
        glDisable(GL_CULL_FACE);
        glDisable (GL_BLEND);
    }

    void
    GraphicsComponent::create2DVertexObjects
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "GraphicsComponent: Creating 2D VAO/VBO" << endl;
        }
        glGenVertexArrays(1, &mSpriteQuadVAO);
        glGenBuffers(1, &mSpriteVBO);
        glBindBuffer(GL_ARRAY_BUFFER, mSpriteVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(mSpriteVertices), mSpriteVertices, GL_STATIC_DRAW);
        glBindVertexArray(mSpriteQuadVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), static_cast<GLvoid*>(0));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }


    void
    GraphicsComponent::createFontVertexObjects
    ()
    {
        if (Constants::DEBUG)
        {
            cout << "GraphicsComponent: Creating Font VAO/VBO" << endl;
        }
        glGenVertexArrays(1, &mFontVAO);
        glGenBuffers(1, &mFontVBO);
        glBindVertexArray(mFontVAO);
        glBindBuffer(GL_ARRAY_BUFFER, mFontVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void
    GraphicsComponent::updateComponent
    (Scene* scene)
    {
        if (Constants::VERBOSE)
        {
            cout << "GraphicsComponrnt: updateComponent(Scene*) Called" << endl;
        }

        if (mWindowComponentHandle->sizeHasChanged())
        {
            onWindowDimensionsChanged();
        }

        // View transform
        mViewMatrix = mCamera->getViewMatrix();

        if (!mWindowComponentHandle->shouldClose())
        {
            // Clear existing Queues
            clear2DQueue();
            clear3DQueue();
            clearLightQueue();

            scene->getRootSceneObject()->applyToAll
            (
                function < void*(SceneObject*) >
                (
                    [&](SceneObject* object)
                    {
                        // Models
                        if (object->getRuntime()->hasModelInstance())
                        {
                            if (object->getRuntime()->hasShaderInstance())
                            {
                                addTo3DQueue(object);
                            }
                            else
                            {
                                cerr << "GraphicsComponent: Object " << object->getJsonData()->getUuid()
                                     << " has model, but no shader assigned." << endl;
                            }
                        }

                        // Sprites
                        if (object->getRuntime()->hasSpriteInstance())
                        {
                            if (object->getRuntime()->hasShaderInstance())
                            {
                                addTo2DQueue(object);
                            }
                            else
                            {
                                cerr << "GraphicsComponent: Object " << object->getJsonData()->getUuid()
                                     << " has sprite, but no shader assigned." << endl;
                            }
                        }

                        // Fonts
                        if (object->getRuntime()->hasFontInstance())
                        {
                            if (object->getRuntime()->hasShaderInstance())
                            {
                                addTo2DQueue(object);
                            }
                            else
                            {
                                cerr << "GraphicsComponent: Object " << object->getJsonData()->getUuid()
                                     << " has font, but no shader assigned." << endl;
                            }
                        }

                        // Lights
                        if (object->getRuntime()->hasLightInstance())
                        {
                            addToLightQueue(object->getRuntime()->getLightInstance());
                        }

                        return nullptr;
                    }
                )
            );
        }
    }

    void
    GraphicsComponent::clear2DQueue
    ()
    {
        m2DQueue.clear();
    }

    void
    GraphicsComponent::addTo2DQueue
    (SceneObject* object)
    {
        m2DQueue.push_back(object);
    }

    void
    GraphicsComponent::draw2DQueue
    ()
    {
        for (SceneObject* sceneObj : m2DQueue)
        {
            if (sceneObj->getRuntime()->hasSpriteInstance())
            {
                drawSprite(sceneObj);
            }
            else if (sceneObj->getRuntime()->hasFontInstance())
            {
                drawFont(sceneObj);
            }
        }
    }

    void
    GraphicsComponent::clear3DQueue
    ()
    {
        m3DQueue.clear();
    }

    void
    GraphicsComponent::addTo3DQueue
    (SceneObject* object)
    {
        m3DQueue.push_back(object);
    }

    void
    GraphicsComponent::draw3DQueue
    ()
    {
        for (SceneObject* it : m3DQueue)
        {
            drawModel(it);
        }
    }

    mat4
    GraphicsComponent::getViewMatrix
    ()
    {
        return mViewMatrix;
    }

    mat4
    GraphicsComponent::getProjectionMatrix
    ()
    {
        return mProjectionMatrix;
    }

    void
    GraphicsComponent::drawSprite
    (SceneObject* sceneObject)
    {
        // Get Assets
        SpriteInstance* sprite = sceneObject->getRuntime()->getSpriteInstance();
        ShaderInstance* shader = sceneObject->getRuntime()->getShaderInstance();
        // Get arguments
        vec2 size = vec2(sprite->getWidth(),sprite->getHeight());
        GLfloat rotateValue = sceneObject->getRuntime()->getTransform()->getRotationZ();
        GLfloat scaleValue = sceneObject->getRuntime()->getTransform()->getScaleZ();
        vec3 color = vec3(1.0f);
        // Setup Shader
        shader->use();
        float tX = sprite->getTransform()->getTranslationX();
        float tY = sprite->getTransform()->getTranslationY();
        vec2 position = vec2(tX,tY);
        // Offset origin to middle of sprite
        mat4 model;
        model = translate(model, vec3(position, 0.0f));
        model = translate(model, vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
        model = rotate(model, rotateValue, vec3(0.0f, 0.0f, 1.0f));
        model = translate(model, vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
        model = scale(model, vec3(size.x*scaleValue,size.y*scaleValue, 1.0f));
        // Pass uniform arguments to shader
        glUniformMatrix4fv(glGetUniformLocation(
                               shader->getShaderProgram(), "model"),
                           1, GL_FALSE, value_ptr(model)
                           );
        glUniform3fv(glGetUniformLocation(
                         shader->getShaderProgram(), "spriteColor"),
                     1, value_ptr(color)
                     );
        glUniform1i(glGetUniformLocation(shader->getShaderProgram(),"image"),0);
        glUniformMatrix4fv(glGetUniformLocation(
                               shader->getShaderProgram(), "projection"),
                           1, GL_FALSE, value_ptr(mOrthoProjection)
                           );
        // Bind texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,sprite->getTexture());
        // Bind VAO
        glBindVertexArray(mSpriteQuadVAO);
        // Draw
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // Cleanup
        glBindVertexArray(0);
        glUseProgram(0);
    }

    void
    GraphicsComponent::drawFont
    (SceneObject* sceneObject)
    {
        // Get Assets
        FontInstance* font = sceneObject->getRuntime()->getFontInstance();
        float tX = font->getTransform()->getTranslationX();
        float tY = font->getTransform()->getTranslationY();

        // Setup Shader
        ShaderInstance* shader = sceneObject->getRuntime()->getShaderInstance();
        vec2 size = vec2(font->getWidth(),font->getHeight());
        GLfloat rotateValue = sceneObject->getRuntime()->getTransform()->getRotationZ();
        GLfloat scaleValue = sceneObject->getRuntime()->getTransform()->getScaleZ();

        shader->use();

        vec2 position = vec2(tX,tY);
        // Offset origin to middle of sprite
        mat4 model;
        model = translate(model, vec3(position, 0.0f));
        model = translate(model, vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
        model = rotate(model, rotateValue, vec3(0.0f, 0.0f, 1.0f));
        model = translate(model, vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
        model = scale(model, vec3(size.x*scaleValue,size.y*scaleValue, 1.0f));

        // Activate corresponding render state
        glUniform3f(
                    glGetUniformLocation(shader->getShaderProgram(), "textColor"),
                    font->getColour()[0], font->getColour()[1], font->getColour()[2]
        );

        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(mFontVAO);

        // Pass uniform arguments to shader
        glUniformMatrix4fv(glGetUniformLocation(
                               shader->getShaderProgram(), "model"),
                           1, GL_FALSE, value_ptr(model)
                           );

        glUniformMatrix4fv(glGetUniformLocation(
                               shader->getShaderProgram(), "projection"),
                           1, GL_FALSE, value_ptr(mOrthoProjection)
                           );

        // Iterate through all characters
        string text = font->getText();
        map<GLchar,FontCharacter> charMap = font->getCharMap();
        for (string::const_iterator c = text.begin(); c != text.end(); c++)
        {
            FontCharacter ch = charMap[*c];

            GLfloat xpos = tX + ch.Bearing.x;// * scale;
            //GLfloat ypos = tY - (ch.Size.y - ch.Bearing.y) * scale;
            GLfloat ypos = tY - (ch.Bearing.y);// * scale);

            GLfloat w = ch.Size.x;// * scale;
            GLfloat h = ch.Size.y;// * scale;
            // Update VBO for each character
            GLfloat vertices[6][4] = {
                { xpos,     ypos + h,   0.0, 1.0 },
                { xpos,     ypos,       0.0, 0.0 },
                { xpos + w, ypos,       1.0, 0.0 },

                { xpos,     ypos + h,   0.0, 1.0 },
                { xpos + w, ypos,       1.0, 0.0 },
                { xpos + w, ypos + h,   1.0, 1.0 }
            };
            // Render glyph texture over quad
            glBindTexture(GL_TEXTURE_2D, ch.TextureID);
            // Update content of VBO memory
            glBindBuffer(GL_ARRAY_BUFFER, mFontVBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            // Render quad
            glDrawArrays(GL_TRIANGLES, 0, 6);
            // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            tX += (ch.Advance >> 6);// * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void
    GraphicsComponent::drawModel
    (SceneObject* sceneObject)
    {
        if (Constants::VERBOSE)
        {
           cout << "GraphicsComponent: Drawing Model " << sceneObject->getNameAndUuidString() << endl;
        }

        // Get Assets
        AssimpModelInstance* model = sceneObject->getRuntime()->getModelInstance();
        ShaderInstance* shader = sceneObject->getRuntime()->getShaderInstance();
        shader->use();
        // Set Ambient Light Values
        GLint uAmbientStrength = glGetUniformLocation(shader->getShaderProgram(),"ambientStrength");
        GLint uAmbientColor    = glGetUniformLocation(shader->getShaderProgram(),"ambientColor");
        GLfloat strength = 0;
        if (uAmbientColor > 0 && uAmbientStrength > 0)
        {
            vec3 ambientColour;
            if (mActiveSceneHandle)
            {
                ambientColour = vec3(
                    mActiveSceneHandle->getJson()[Constants::SCENE_AMBIENT_LIGHT_COLOUR][Constants::RED],
                    mActiveSceneHandle->getJson()[Constants::SCENE_AMBIENT_LIGHT_COLOUR][Constants::GREEN],
                    mActiveSceneHandle->getJson()[Constants::SCENE_AMBIENT_LIGHT_COLOUR][Constants::BLUE]
                );
                strength = mActiveSceneHandle->getJson()[Constants::SCENE_AMBIENT_LIGHT_COLOUR][Constants::ALPHA];
            }
            else
            {
            }

            glUniform1f(uAmbientStrength,strength);
            glUniform3fv(uAmbientColor,1,value_ptr(ambientColour));

        }
        // Set Diffuse Light Values
        vector<LightInstance*>::
                iterator lights;
        int i;
        for (i=1, lights = mLightQueue.begin(); lights != mLightQueue.end(); lights++, i++)
        {
            stringstream uPosStr;
            uPosStr << "diffusePos_" << i;
            stringstream uColorStr;
            uColorStr << "diffuseColor_" << i;

            GLint uLightPos   = glGetUniformLocation(shader->getShaderProgram(),uPosStr.str().c_str());
            GLint uLightColor = glGetUniformLocation(shader->getShaderProgram(),uColorStr.str().c_str());

            if (uLightPos > 0)
            {
                vec3 lightPos = (*lights)->getTransform()->getTranslation();
                glUniform3fv(uLightPos  ,1, value_ptr(lightPos));
            }
            else
            {
                if (Constants::VERBOSE)
                {
                    cout << "GraphicsComponent: cannot find uniform for " << uPosStr.str() << endl;
                }
            }

            if (uLightColor > 0)
            {
                vec3 lightColor = (*lights)->getColor();
                glUniform3fv(uLightColor,1, value_ptr(lightColor));
            }
            else
            {
                if (Constants::VERBOSE)
                {
                    cout << "GraphicsComponent: cannot find uniform for " << uColorStr.str() << endl;
                }
            }
        }

        // Pass view/projection transform to shader
        glUniformMatrix4fv
        (
            glGetUniformLocation(shader->getShaderProgram(), "projection"),
            1, GL_FALSE, value_ptr(mProjectionMatrix)
        );

        glUniformMatrix4fv
        (
            glGetUniformLocation(shader->getShaderProgram(), "view"),
            1, GL_FALSE, value_ptr(mViewMatrix)
        );
        // calculate the model matrix
        mat4 modelMatrix;
        // Get raw data
        vec3 translation = sceneObject->getRuntime()->getTranslation();
        quat rot = sceneObject->getRuntime()->getTransform()->getOrientation();
        vec3 scaleValue = sceneObject->getRuntime()->getScale();
        // Translate
        modelMatrix = translate(modelMatrix,translation);
        // Rotate
        mat4 rotMat = mat4_cast(rot);
        modelMatrix = modelMatrix * rotMat;
        // Scale
        modelMatrix = scale(modelMatrix, scaleValue);
        model->setModelMatrix(modelMatrix);

        // Pass model matrix to shader
        glUniformMatrix4fv(
                    glGetUniformLocation(shader->getShaderProgram(), "model"),
                    1, GL_FALSE, value_ptr(modelMatrix)
                    );
        // Draw using shader
        model->draw(shader);
        // Unbind shader
        glBindVertexArray(0);
        glUseProgram(0);
        Constants::checkGLError("After 3D Draw");
    }



    void
    GraphicsComponent::addToLightQueue
    (LightInstance* lightInstance)
    {
        mLightQueue.push_back(lightInstance);
    }


    void
    GraphicsComponent::clearLightQueue
    ()
    {
        mLightQueue.clear();
    }

    Camera*
    GraphicsComponent::getCamera
    ()
    {
        return mCamera;
    }

    void
    GraphicsComponent::cleanUp
    (Scene* scene)
    {
        clear2DQueue();
        clear3DQueue();
        clearLightQueue();
        // Clean up caches
        TextureCache::cleanUp();
        ShaderCache::cleanUp();
        AssimpModelInstance::cleanUpCache();
        mActiveSceneHandle = nullptr;
    }

    void
    GraphicsComponent::setActiveScene
    (Scene* scene)
    {
        mActiveSceneHandle = scene;
    }

} // End of Dream
