/*
* Dream::Plugnis::Audio::Open::AudioComponent
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

#ifndef AUDIOCOMPONENT_H
#define AUDIOCOMPONENT_H

#include <iostream>
#include <vector>

#ifdef __APPLE__
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
#include </al.h>
#include </alc.h>
#endif

#include "../IComponent.h"
#include "AudioInstance.h"

namespace Dream
{

    class AudioComponent : public IComponent
    {
    private:
        std::vector<AudioInstance*> mPlayQueue;
        std::vector<AudioInstance*> mPauseQueue;
        std::vector<AudioInstance*> mStopQueue;
        std::vector<ALuint> mSources;
        std::vector<ALuint> mBuffers;
        ALCdevice*  mDevice;
        ALCcontext* mContext;
    public:
        AudioComponent();
        ~AudioComponent();
        bool init() override;
        void updateComponent(Scene*) override;
        void cleanUp(Scene*) override;

        void setSourcePosision(ALuint, std::vector<float>);
        void setListenerPosition(std::vector<float>);

        void pushToPlayQueue(AudioInstance*);
        void pushToPauseQueue(AudioInstance*);
        void pushToStopQueue(AudioInstance*);

        void playAudioAsset(AudioInstance*);
        void pauseAudioAsset(AudioInstance*);
        void stopAudioAsset(AudioInstance*);
        AudioStatus getAudioStatus(AudioInstance*);

        float getSampleOffset(ALuint);
        float getSampleOffset(AudioInstance*);
        std::vector<char> getAudioBuffer(AudioInstance*, ALint, ALint);
        AudioInstance* newAudioInstance(AssetDefinition*,Transform3D*);

    protected:
        ALuint generateBuffers(size_t);
        ALuint generateSources(size_t);

        void deleteBuffers(int, ALuint);
        void deleteSources(int, ALuint);

        void playSource(ALuint);
        void stopSource(ALuint);
        void pauseSource(ALuint);

        void updatePlayQueue();
        void updatePauseQueue();
        void updateStopQueue();
        void cleanUpBuffersAndSources();

        void deleteAllSources();
        void deleteAllBuffers();

    }; // End of AudioComponent
} // End of Dream

#endif // End of AUDIOCOMPONENT_H
