#ifndef KEYINPUT_HPP_
#define KEYINPUT_HPP_

#include <string>
#include <deque>
#include <map>

enum KeyState { RELEASED, PRESSED,
                COLD1, COLD2, COLD3, COLD4, COLD5, COLD6, COLD7, COLD8, COLD9, COLD10,
                CONTINUS };

class Input
{
public:
        Input(void);

        void _update(void);
        std::deque<int> get_pressed_keys(void);
        std::deque<int> get_continus_keys(void);
private:
        std::deque<int> _get_keys(KeyState state);
        void _read_key_state(std::deque<int> &pressed, std::deque<int> &released);
        void _release(const std::deque<int> &released);
        void _press(const std::deque<int> &pressed);
        bool _key_pressed(int key);

        int _kbd;
        std::map<int, KeyState> _status_table;
};

#endif  // KEYINPUT_HPP_

