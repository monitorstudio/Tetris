#include <cstdio>
#include <cstring>
#include <cassert>
#include <cstddef>

#if defined(__linux__)
        #include <fcntl.h>
        #include <linux/input.h>
#elif defined(_WIN32)
        #include <windows.h>
#endif

#include "KeyInput.hpp"

static int keys[] =
{
        KEY_UP,         KEY_DOWN,
        KEY_LEFT,       KEY_RIGHT,
        KEY_LEFTSHIFT,  KEY_RIGHTSHIFT,
        KEY_LEFTCTRL,   KEY_RIGHTCTRL,
        KEY_SPACE,      KEY_Q
};

#if defined(_WIN32)
Input::Input(void)
{
        for(std::size_t i = 0; i < sizeof(keys) / sizeof(int); i++)
                _status_table[keys[i]] = RELEASED;
}
#elif defined(__linux__)
Input::Input(std::string kbd_path)
{
        assert((_kbd = open(kbd_path.c_str(), O_RDONLY)) != -1);

        for(std::size_t i = 0; i < sizeof(keys) / sizeof(int); i++)
                _status_table[keys[i]] = RELEASED;
}
#endif

std::deque<int> Input::get_pressed_keys(void)
{
        return _get_keys(PRESSED);
}

std::deque<int> Input::get_continus_keys(void)
{
        return _get_keys(CONTINUS);
}

std::deque<int> Input::_get_keys(KeyState state)
{
        std::deque<int> array;

        for(std::size_t i = 0; i < sizeof(keys) / sizeof(int); i++)
        {
                if(_status_table[keys[i]] == state)
                        array.push_back(keys[i]);
        }

        return array;
}

void Input::_update(void)
{
        std::deque<int> pressed, released;

        _read_key_state(pressed, released);

        _release(released);
        _press(pressed);
}

void Input::_read_key_state(std::deque<int> &pressed, std::deque<int> &released)
{
        for(std::size_t i = 0; i < sizeof(keys) / sizeof(int); i++)
        {
                _key_pressed(keys[i]) ? pressed.push_back(keys[i]) : released.push_back(keys[i]);
        }
}

void Input::_release(const std::deque<int> &released)
{
        for(std::size_t i = 0, size = released.size(); i < size; i++)
                _status_table[released[i]] = RELEASED;
}

void Input::_press(const std::deque<int> &pressed)
{
        for(std::size_t i = 0, size = pressed.size(); i < size; i++)
        {
                switch(_status_table[pressed[i]])
                {
                case RELEASED:
                        _status_table[pressed[i]] = PRESSED;
                        break;
                case PRESSED:
                        _status_table[pressed[i]] = COLD1;
                        break;
                case COLD1:
                        _status_table[pressed[i]] = COLD2;
                        break;
                case COLD2:
                        _status_table[pressed[i]] = COLD3;
                        break;
                case COLD3:
                        _status_table[pressed[i]] = COLD4;
                        break;
                case COLD4:
                        _status_table[pressed[i]] = COLD5;
                        break;
                case COLD5:
                        _status_table[pressed[i]] = COLD6;
                        break;
                case COLD6:
                        _status_table[pressed[i]] = COLD7;
                        break;
                case COLD7:
                        _status_table[pressed[i]] = COLD8;
                        break;
                case COLD8:
                        _status_table[pressed[i]] = COLD9;
                        break;
                case COLD9:
                        _status_table[pressed[i]] = COLD10;
                        break;
                case COLD10:
                        _status_table[pressed[i]] = CONTINUS;
                        break;
                case CONTINUS:
                        break;
                }
        }
}

bool Input::_key_pressed(int key)
{
#if defined(__linux__)
        char key_b[(KEY_MAX + 7) / 8];

	memset(key_b, 0, sizeof(key_b));
	ioctl(_kbd, EVIOCGKEY(sizeof(key_b)), key_b);

	return !!(key_b[key / 8] & (1 << (key % 8)));
#elif defined(_WIN32)
        BYTE state[256];

        GetKeyboardState(state);

        return (bool)((unsigned short)GetKeyState(key) >> ((sizeof(SHORT) * 8) - 1));
#endif
}

