#include <cstdio>
#include <cstring>
#include <cassert>
#include <cstddef>
#include <unistd.h>

#if defined(__linux__)
        #include <fcntl.h>
        #include <linux/input.h>
        #include <sys/types.h>
        #include <sys/stat.h>
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

Input::Input(void)
        : _cold_during(70)
{
        for(std::size_t i = 0; i < sizeof(keys) / sizeof(int); i++)
        {
                _status_table[keys[i]] = RELEASED;
                _cold_count[keys[i]] = 0;
        }
}
#if defined(__linux__)
Input::Input(std::string kbd_path)
        : Input()
{
        assert((_kbd = ::open(kbd_path.c_str(), O_RDONLY)) != -1);
}

Input::~Input(void)
{
        ::close(_kbd);
}
#endif

void Input::set_cold_during(std::size_t during)
{
        _cold_during = during;
}

std::deque<int> Input::get_pressed_keys(void)
{
        return _get_keys(PRESSED);
}

std::deque<int> Input::get_repeated_keys(void)
{
        return _get_keys(REPEATED);
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
                        _status_table[pressed[i]] = COLD;
                        break;
                case COLD:
                        if(_cold_count[pressed[i]] != _cold_during)
                        {
                                _cold_count[pressed[i]] += 1;
                        }
                        else
                        {
                                _status_table[pressed[i]] = REPEATED;
                                _cold_count[pressed[i]] = 0;
                        }
                        break;
                case REPEATED:
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

        // GetKeyState returns SHORT
        // If key is being pressed, the MSB is HIGH
        return (static_cast<signed short>(GetKeyState(key)) < 0);
#endif
}

