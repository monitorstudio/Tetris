#ifndef KEYINPUT_HPP_
#define KEYINPUT_HPP_

#include <string>
#include <deque>
#include <map>

#if defined(_WIN32)
        #define KEY_UP          VK_UP
        #define KEY_DOWN        VK_DOWN
        #define KEY_LEFT        VK_LEFT
        #define KEY_RIGHT       VK_RIGHT
        #define KEY_LEFTSHIFT   VK_LSHIFT
        #define KEY_RIGHTSHIFT  VK_RSHIFT
        #define KEY_LEFTCTRL    VK_LCONTROL
        #define KEY_RIGHTCTRL   VK_RCONTROL
        #define KEY_SPACE       VK_SPACE
        #define KEY_Q           static_cast<int>('q')
#endif

enum KeyState
{
        RELEASED, PRESSED,
        COLD1, COLD2, COLD3, COLD4, COLD5, COLD6, COLD7, COLD8, COLD9, COLD10,
        CONTINUS
};

class Input
{
public:
#if defined(__WIN32)
        Input(void);                    // For Windows
#elif defined(__linux__)
        Input(std::string kbd_path);    // For Linux
		~Input(void);
#endif

        void _update(void);
        std::deque<int> get_pressed_keys(void);
        std::deque<int> get_continus_keys(void);
private:
        std::deque<int> _get_keys(KeyState state);
        void _read_key_state(std::deque<int> &pressed, std::deque<int> &released);
        void _release(const std::deque<int> &released);
        void _press(const std::deque<int> &pressed);
        bool _key_pressed(int key);

#if defined(__linux__)
        int _kbd;
#endif
        std::map<int, KeyState> _status_table;
};

#endif  // KEYINPUT_HPP_

