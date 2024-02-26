#if !defined(GG_KEYS_H)
#define GG_KEYS_H

#include <stdbool.h>

typedef enum {
    // KEYS FROM RAYLIB :)

    GG_KEY_NULL            = 0,        // Key: NULL, used for no key pressed
    // Alphanumeric keys
    GG_KEY_APOSTROPHE      = 39,       // Key: '
    GG_KEY_COMMA           = 44,       // Key: ,
    GG_KEY_MINUS           = 45,       // Key: -
    GG_KEY_PERIOD          = 46,       // Key: .
    GG_KEY_SLASH           = 47,       // Key: /
    GG_KEY_ZERO            = 48,       // Key: 0
    GG_KEY_ONE             = 49,       // Key: 1
    GG_KEY_TWO             = 50,       // Key: 2
    GG_KEY_THREE           = 51,       // Key: 3
    GG_KEY_FOUR            = 52,       // Key: 4
    GG_KEY_FIVE            = 53,       // Key: 5
    GG_KEY_SIX             = 54,       // Key: 6
    GG_KEY_SEVEN           = 55,       // Key: 7
    GG_KEY_EIGHT           = 56,       // Key: 8
    GG_KEY_NINE            = 57,       // Key: 9
    GG_KEY_SEMICOLON       = 59,       // Key: ;
    GG_KEY_EQUAL           = 61,       // Key: =
    GG_KEY_A               = 65,       // Key: A | a
    GG_KEY_B               = 66,       // Key: B | b
    GG_KEY_C               = 67,       // Key: C | c
    GG_KEY_D               = 68,       // Key: D | d
    GG_KEY_E               = 69,       // Key: E | e
    GG_KEY_F               = 70,       // Key: F | f
    GG_KEY_G               = 71,       // Key: G | g
    GG_KEY_H               = 72,       // Key: H | h
    GG_KEY_I               = 73,       // Key: I | i
    GG_KEY_J               = 74,       // Key: J | j
    GG_KEY_K               = 75,       // Key: K | k
    GG_KEY_L               = 76,       // Key: L | l
    GG_KEY_M               = 77,       // Key: M | m
    GG_KEY_N               = 78,       // Key: N | n
    GG_KEY_O               = 79,       // Key: O | o
    GG_KEY_P               = 80,       // Key: P | p
    GG_KEY_Q               = 81,       // Key: Q | q
    GG_KEY_R               = 82,       // Key: R | r
    GG_KEY_S               = 83,       // Key: S | s
    GG_KEY_T               = 84,       // Key: T | t
    GG_KEY_U               = 85,       // Key: U | u
    GG_KEY_V               = 86,       // Key: V | v
    GG_KEY_W               = 87,       // Key: W | w
    GG_KEY_X               = 88,       // Key: X | x
    GG_KEY_Y               = 89,       // Key: Y | y
    GG_KEY_Z               = 90,       // Key: Z | z
    GG_KEY_LEFT_BRACKET    = 91,       // Key: [
    GG_KEY_BACKSLASH       = 92,       // Key: '\'
    GG_KEY_RIGHT_BRACKET   = 93,       // Key: ]
    GG_KEY_GRAVE           = 96,       // Key: `
    // Function keys
    GG_KEY_SPACE           = 32,       // Key: Space
    GG_KEY_ESCAPE          = 256,      // Key: Esc
    GG_KEY_ENTER           = 257,      // Key: Enter
    GG_KEY_TAB             = 258,      // Key: Tab
    GG_KEY_BACKSPACE       = 259,      // Key: Backspace
    GG_KEY_INSERT          = 260,      // Key: Ins
    GG_KEY_DELETE          = 261,      // Key: Del
    GG_KEY_RIGHT           = 262,      // Key: Cursor right
    GG_KEY_LEFT            = 263,      // Key: Cursor left
    GG_KEY_DOWN            = 264,      // Key: Cursor down
    GG_KEY_UP              = 265,      // Key: Cursor up
    GG_KEY_PAGE_UP         = 266,      // Key: Page up
    GG_KEY_PAGE_DOWN       = 267,      // Key: Page down
    GG_KEY_HOME            = 268,      // Key: Home
    GG_KEY_END             = 269,      // Key: End
    GG_KEY_CAPS_LOCK       = 280,      // Key: Caps lock
    GG_KEY_SCROLL_LOCK     = 281,      // Key: Scroll down
    GG_KEY_NUM_LOCK        = 282,      // Key: Num lock
    GG_KEY_PRINT_SCREEN    = 283,      // Key: Print screen
    GG_KEY_PAUSE           = 284,      // Key: Pause
    GG_KEY_F1              = 290,      // Key: F1
    GG_KEY_F2              = 291,      // Key: F2
    GG_KEY_F3              = 292,      // Key: F3
    GG_KEY_F4              = 293,      // Key: F4
    GG_KEY_F5              = 294,      // Key: F5
    GG_KEY_F6              = 295,      // Key: F6
    GG_KEY_F7              = 296,      // Key: F7
    GG_KEY_F8              = 297,      // Key: F8
    GG_KEY_F9              = 298,      // Key: F9
    GG_KEY_F10             = 299,      // Key: F10
    GG_KEY_F11             = 300,      // Key: F11
    GG_KEY_F12             = 301,      // Key: F12
    GG_KEY_LEFT_SHIFT      = 340,      // Key: Shift left
    GG_KEY_LEFT_CONTROL    = 341,      // Key: Control left
    GG_KEY_LEFT_ALT        = 342,      // Key: Alt left
    GG_KEY_LEFT_SUPER      = 343,      // Key: Super left
    GG_KEY_RIGHT_SHIFT     = 344,      // Key: Shift right
    GG_KEY_RIGHT_CONTROL   = 345,      // Key: Control right
    GG_KEY_RIGHT_ALT       = 346,      // Key: Alt right
    GG_KEY_RIGHT_SUPER     = 347,      // Key: Super right
    GG_KEY_KB_MENU         = 348,      // Key: KB menu
    // Keypad keys
    GG_KEY_KP_0            = 320,      // Key: Keypad 0
    GG_KEY_KP_1            = 321,      // Key: Keypad 1
    GG_KEY_KP_2            = 322,      // Key: Keypad 2
    GG_KEY_KP_3            = 323,      // Key: Keypad 3
    GG_KEY_KP_4            = 324,      // Key: Keypad 4
    GG_KEY_KP_5            = 325,      // Key: Keypad 5
    GG_KEY_KP_6            = 326,      // Key: Keypad 6
    GG_KEY_KP_7            = 327,      // Key: Keypad 7
    GG_KEY_KP_8            = 328,      // Key: Keypad 8
    GG_KEY_KP_9            = 329,      // Key: Keypad 9
    GG_KEY_KP_DECIMAL      = 330,      // Key: Keypad .
    GG_KEY_KP_DIVIDE       = 331,      // Key: Keypad /
    GG_KEY_KP_MULTIPLY     = 332,      // Key: Keypad *
    GG_KEY_KP_SUBTRACT     = 333,      // Key: Keypad -
    GG_KEY_KP_ADD          = 334,      // Key: Keypad +
    GG_KEY_KP_ENTER        = 335,      // Key: Keypad Enter
    GG_KEY_KP_EQUAL        = 336,      // Key: Keypad =
    // Android key buttons
    GG_KEY_BACK            = 4,        // Key: Android back button
    GG_KEY_MENU            = 82,       // Key: Android menu button
    GG_KEY_VOLUME_UP       = 24,       // Key: Android volume up button
    GG_KEY_VOLUME_DOWN     = 25        // Key: Android volume down button
} gg_key_e;

typedef struct gg_keys {

} gg_keys_t;

void Keys_Create(gg_keys_t* keys);
void Keys_Update(gg_keys_t* keys);
bool Keys_KeyDown(gg_keys_t* keys, gg_key_e key);

#endif // GG_KEYS_H
