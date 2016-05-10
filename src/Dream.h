#ifndef DREAM_H
#define DREAM_H

#include <iostream>
#include "Util/FileReader.h"
#include "Scene/Scene.h"
#include "ArgumentParser.h"
#include "Project.h"

#include "Interfaces/Interface.h"
#include "Interfaces/AudioInterface.h"
#include "Interfaces/PhysicsInterface.h"
#include "Interfaces/VideoInterface.h"
#include "Interfaces/ScriptingInterface.h"


namespace Dream {
	class Dream {
	protected:
		Project                       *mProject;
		Audio::AudioInterface         *mAudioInterface;
		Video::VideoInterface         *mVideoInterface;
		Physics::PhysicsInterface     *mPhysicsInterface;
		Scripting::ScriptingInterface *mScriptingInterface;

	public:
		Dream(void);
		~Dream(void);
		bool loadFromArgumentParser(ArgumentParser*);
		bool loadProjectFromFileReader(std::string projectPath, Util::FileReader*);
		Project* getProject(void);
		void setProject(Project*);
		int runProject();
		bool isProjectLoaded();
		bool createInterfaces();
	};
}

#endif // DREAM_H
