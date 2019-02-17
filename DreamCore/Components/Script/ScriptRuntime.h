/*
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

#include "ScriptTasks.h"
#include "../SharedAssetRuntime.h"
#include "../Event.h"

class asIScriptModule;
class asIScriptFunction;

namespace Dream
{
    class ScriptDefinition;
    class InputComponent;
    class NanoVGComponent;
    class SceneRuntime;

    class ScriptRuntime : public SharedAssetRuntime
    {
        bool mError;
        string mSource;
        string mUuidString;
        ScriptConstructionTask mConstructionTask;
        map<uint32_t, bool> mInitialised;
        asIScriptModule* mScriptModule;

        asIScriptFunction *mInitFunction;
        asIScriptFunction *mEventFunction;
        asIScriptFunction *mUpdateFunction;
        asIScriptFunction *mDestroyFunction;
        asIScriptFunction *mInputFunction;
        asIScriptFunction *mNanoVGFunction;
    public:

        ScriptRuntime(ScriptDefinition*,ProjectRuntime*);
        ~ScriptRuntime() override;
        bool useDefinition() override;
        string getSource() const;
        void setSource(const string& source);
        bool createScript();

        bool executeOnInit(SceneObjectRuntime*);
        bool executeOnUpdate(SceneObjectRuntime*);
        bool executeOnEvent(SceneObjectRuntime*);
        bool executeOnDestroy(uint32_t destroyedSo, SceneObjectRuntime* parent);

        bool executeOnInput(InputComponent*, SceneRuntime*);
        bool executeOnNanoVG(NanoVGComponent*, SceneRuntime*);

        bool getInitialised(SceneObjectRuntime* sor);
        ScriptConstructionTask* getConstructionTask();
        void removeInitialisedFlag(uint32_t);
    };
}
