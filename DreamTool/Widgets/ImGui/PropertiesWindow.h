#pragma once

#include "ImGuiWidget.h"
#include "PropertyType.h"

#define BigEditorBufferSize 1024*16

namespace DreamTool
{
    struct PropertiesTarget
    {
        PropertyType type;
        IDefinition* definition;
        IRuntime* runtime;
    };

    class PropertiesWindow : public ImGuiWidget
    {
    public:
        PropertiesWindow(Project*);
        ~PropertiesWindow() override;

        void draw() override;
        void pushPropertyTarget(PropertyType, IDefinition*, IRuntime*);
        void popPropertyTarget();
        void clearPropertyTargets();

    private:
        int getStringIndexInVector(string str, vector<string> vec);
        void drawDeleteSceneObjectButton();
        void drawDeleteSceneButton();
        void drawNameAndIdProperties();
        void drawProjectProperties();
        void drawSceneProperties();
        void drawSceneObjectProperties();
        void drawAssetProperties();
        void drawAudioAssetProperties();
        void drawFontAssetProperties();
        void drawLightAssetProperties();
        void drawMaterialAssetProperties();
        void drawModelAssetProperties();
        void drawPhysicsObjectAssetProperties();
        void drawScriptProperties();
        void drawShaderAssetProperties();
        void drawPathAssetProperties();
        void drawParticleEmitterAssetProperties();
        void drawTextureAssetProperties();

        void setRuntime(IRuntime* runt);
        void setDefinition(IDefinition* def);
        void setPropertyType(PropertyType t);
        void clear();
    private:
        vector<PropertiesTarget> mHistory;
        PropertyType mType;
        IDefinition* mDefinition;
        IRuntime* mRuntime;
        ImVec2 mImageSize;
        ImVec2 mBigEditorSize;
    };

}