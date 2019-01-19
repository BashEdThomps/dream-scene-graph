/*
* ShaderRuntime
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

#ifdef WIN32
#include <windows.h>
#endif

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#include <GL/glu.h>
#endif

#include <vector>
#include <map>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/matrix.hpp>

#include "ShaderUniform.h"

#include "../../SharedAssetRuntime.h"
#include "../Light/LightRuntime.h"
#include "../../../Common/DreamObject.h"

using namespace std;
using namespace nlohmann;
using namespace glm;

namespace Dream
{
    class ShaderCache;
    class ShaderDefinition;
    class MaterialRuntime;
    class LightRuntime;
    class Camera;

    class ShaderRuntime
            : public SharedAssetRuntime
    {
    private:
        const static unsigned int MAX_LIGHTS;
        const static GLint UNIFORM_NOT_FOUND;
        const static char* UNIFORM_POINT_LIGHT_COUNT;
        const static char* UNIFORM_SPOT_LIGHT_COUNT;
        const static char* UNIFORM_DIRECTIONAL_LIGHT_COUNT;
        const static size_t MAX_RuntimeS;

        unsigned int mPointLightCount;
        GLint mPointLightCountLocation;

        unsigned int mSpotLightCount;
        GLint mSpotLightCountLocation;

        unsigned int mDirectionalLightCount;
        GLint mDirectionalLightCountLocation;

        GLuint mShaderProgram;

        bool mNeedsRebind;
        GLint mMaterialLocation;

        vector<shared_ptr<ShaderUniform>> mUniformVector;
        vector<MaterialRuntime*> mMaterials;
        vector<mat4> mRuntimeMatricies;
        map<string,GLint> mUinformCache;
        GLuint mVertexShader;
        GLuint mFragmentShader;
        bool mRecompile;
        string mVertexSource;
        string mFragmentSource;

    public:
        ShaderRuntime(ShaderDefinition*, ProjectRuntime*);
        ~ShaderRuntime() override;

        static GLuint CurrentTexture0;
        static GLuint CurrentTexture1;
        static GLuint CurrentTexture2;
        static GLuint CurrentTexture3;
        static GLuint CurrentTexture4;
        static GLuint CurrentShaderProgram;
        static GLuint CurrentVAO;
        static GLuint CurrentVBO;
        static void InvalidateState();

        bool useDefinition() override;

        void use();
        void unbind();
        GLuint getShaderProgram();
        int countMaterials();

        // MVP
        bool setModelMatrix(mat4,string name = "model");
        bool setViewMatrix(mat4,string name = "view");
        bool setProjectionMatrix(mat4,string name = "projection");

        // Viewer
        bool setViewerPosition(vec3, string name = "viewPos");

        void bindVertexArray(GLuint);
        void unbindVertexArray();

        GLint getUniformLocation(string name);

        void addUniform(UniformType type, string name, int count, void* data);
        void bindLight(LightRuntime* light);

        void syncUniforms();

        void bindMaterial(MaterialRuntime* material);
        void bindLightQueue(vector<LightRuntime*> lightQueue);
        void bindRuntimes(vector<SceneObjectRuntime*> Runtimes);

        void addMaterial(MaterialRuntime* material);

        void logMaterials();

        void drawGeometryPass(Camera*);
        void drawShadowPass(ShaderRuntime* shadowPassShader);

        bool getRecompile() const;
        void setRecompile(bool recompile);

        string getVertexSource() const;
        void setVertexSource(string vertexSource);

        string getFragmentSource() const;
        void setFragmentSource(string fragmentSource);

    protected:
        bool compileVertex();
        bool compileFragment();
        bool linkProgram();
    };
}