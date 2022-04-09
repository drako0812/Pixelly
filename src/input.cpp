#include "input.hpp"
#include "machine.hpp"
#include <SFML/Window.hpp>
#include <cstring>
#include <fmt/format.h>

namespace pxly {

    bool getBit(int index, uint8 * arr, int size) {
        auto byte_index = index / 8;
        auto bit_index  = index % 8;

        pxly_assert(byte_index < size, fmt::format("byte_index ({}) out of bounds, array size {}", byte_index, size));

        auto value = arr[byte_index];
        auto bit   = 1 & (value >> bit_index);
        return bit == 1;
    }

    void setBit(int index, uint8 * arr, int size, bool value) {
        auto byte_index = index / 8;
        auto bit_index  = index % 8;

        pxly_assert(byte_index < size, fmt::format("byte_index ({}) out of bounds, array size {}", byte_index, size));

        auto ovalue     = arr[byte_index];
        auto mask       = ~(1 << bit_index);
        ovalue          = (ovalue & mask) | ((value ? 1 : 0) << bit_index);
        arr[byte_index] = ovalue;
    }

    Keyboard::Keyboard() {}

    bool Keyboard::Initialize() {
        memset(m_KeyP, 0, sizeof(m_KeyP));
        memset(m_Key, 0, sizeof(m_Key));
        return true;
    }

    void Keyboard::StartFrame() { memcpy(m_KeyP, m_Key, sizeof(m_KeyP)); }

    void Keyboard::HandleEvent(void * evt) {
        sf::Event * ev = (sf::Event *)evt;
        switch (ev->type) {
        case sf::Event::KeyPressed: setBit(ev->key.code, m_Key, sizeof(m_Key), true); break;
        case sf::Event::KeyReleased: setBit(ev->key.code, m_Key, sizeof(m_Key), false); break;
        default: break;
        }
    }

    bool Keyboard::Key(int key) { return getBit(key, m_Key, sizeof(m_Key)); }

    bool Keyboard::KeyP(int key) { return getBit(key, m_KeyP, sizeof(m_KeyP)); }

} // namespace pxly