#pragma once

#include "../IComponent.h"
#include <gainput/gainput.h>
#include <gainput/GainputInputMap.h>

using namespace gainput;
using std::vector;

namespace Dream
{
    enum InputSource
    {
        // Joystick
        JS_FaceButtonNorth,
        JS_FaceButtonEast,
        JS_FaceButtonWest,
        JS_FaceButtonSouth,
        JS_FaceHome,
        JS_FaceStart,
        JS_FaceSelect,
        JS_ShoulderLeft,
        JS_ShoulderRight,
        JS_TriggerLeft,
        JS_TriggerRight,
        JS_DPadNorth,
        JS_DPadSouth,
        JS_DPadEast,
        JS_DPadWest,
        JS_AnalogLeftStickX,
        JS_AnalogLeftStickY,
        JS_AnalogLeftButton,
        JS_AnalogRightStickX,
        JS_AnalogRightStickY,
        JS_AnalogRightButton,
        // Keyboard
        KB_UP,
        KB_DOWN,
        KB_LEFT,
        KB_RIGHT,
        KB_SPACE,
        KB_RETURN
    };

    enum MouseInputSource
    {

    };

    class InputComponent : public IComponent
    {
    public:
        InputComponent(bool useKeyboard = false, bool useMouse = false, bool useJoystick = false);
        ~InputComponent() override;

        bool init() override;
        void updateComponent() override;

        InputMap* getInputMap() const;

        void debugKeyboard() const;
        void debugMouse() const;
        void debugGamepad() const;

        bool usingKeyboard() const;
        bool usingMouse() const;
        bool usingJoystick() const;

    private:
        const static float ANALOG_DEAD_ZONE;
        bool mUseKeyboard;
        bool mUseMouse;
        bool mUseJoystick;
        InputMap* mInputMap;
        InputManager mInputManager;
        vector<DeviceId> mDevices;
    };

}
