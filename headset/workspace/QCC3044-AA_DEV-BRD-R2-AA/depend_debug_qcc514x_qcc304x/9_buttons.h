#ifndef BUTTON_CONFIG_H
#define BUTTON_CONFIG_H

#include "domain_message.h"
#include "input_event_manager.h"
extern const InputEventConfig_t input_event_config;

extern const InputActionMessage_t default_message_group[38];

#define MFB                  (1UL <<  1)
#define VOICE                (1UL <<  7)
#define SW8                  (1UL <<  8)
#define BACK                 (1UL <<  5)
#define SYS_CTRL             (1UL <<  0)
#define VOL_PLUS             (1UL <<  4)
#define MUSIC                (1UL <<  2)
#define FORWARD              (1UL <<  6)
#define VOL_MINUS            (1UL <<  3)

#define MIN_INPUT_ACTION_MESSAGE_ID (LOGICAL_INPUT_MESSAGE_BASE-2)
#define MAX_INPUT_ACTION_MESSAGE_ID (LOGICAL_INPUT_MESSAGE_BASE+37)

#define LI_MFB_BUTTON_RELEASE_FACTORY_RESET_DS   (LOGICAL_INPUT_MESSAGE_BASE- 0x2)
#define LI_MFB_BUTTON_HELD_FACTORY_RESET_DS      (LOGICAL_INPUT_MESSAGE_BASE- 0x1)
#define LI_MFB_BUTTON_RELEASE_8SEC               (LOGICAL_INPUT_MESSAGE_BASE+ 0x0)
#define APP_BUTTON_PLAY_PAUSE_TOGGLE             (LOGICAL_INPUT_MESSAGE_BASE+ 0x1)
#define LI_MFB_BUTTON_RELEASE_1SEC               (LOGICAL_INPUT_MESSAGE_BASE+ 0x2)
#define APP_VA_BUTTON_RELEASE                    (LOGICAL_INPUT_MESSAGE_BASE+ 0x3)
#define APP_LEAKTHROUGH_DISABLE                  (LOGICAL_INPUT_MESSAGE_BASE+ 0x4)
#define APP_VA_BUTTON_HELD_1SEC                  (LOGICAL_INPUT_MESSAGE_BASE+ 0x5)
#define LI_MFB_BUTTON_HELD_4SEC                  (LOGICAL_INPUT_MESSAGE_BASE+ 0x6)
#define APP_BUTTON_BACKWARD_HELD_RELEASE         (LOGICAL_INPUT_MESSAGE_BASE+ 0x7)
#define APP_BUTTON_VOLUME_UP_RELEASE             (LOGICAL_INPUT_MESSAGE_BASE+ 0x8)
#define LI_MFB_BUTTON_HELD_3SEC                  (LOGICAL_INPUT_MESSAGE_BASE+ 0x9)
#define LI_MFB_BUTTON_HELD_6SEC                  (LOGICAL_INPUT_MESSAGE_BASE+ 0xa)
#define LI_MFB_BUTTON_HELD_DFU                   (LOGICAL_INPUT_MESSAGE_BASE+ 0xb)
#define APP_BUTTON_VOLUME_UP                     (LOGICAL_INPUT_MESSAGE_BASE+ 0xc)
#define APP_BUTTON_BACKWARD_HELD                 (LOGICAL_INPUT_MESSAGE_BASE+ 0xd)
#define APP_VA_BUTTON_DOWN                       (LOGICAL_INPUT_MESSAGE_BASE+ 0xe)
#define APP_LEAKTHROUGH_ENABLE                   (LOGICAL_INPUT_MESSAGE_BASE+ 0xf)
#define LI_MFB_BUTTON_HELD_1SEC                  (LOGICAL_INPUT_MESSAGE_BASE+0x10)
#define LI_MFB_BUTTON_RELEASE_4SEC               (LOGICAL_INPUT_MESSAGE_BASE+0x11)
#define APP_BUTTON_FORWARD_HELD_RELEASE          (LOGICAL_INPUT_MESSAGE_BASE+0x12)
#define APP_BUTTON_BACKWARD                      (LOGICAL_INPUT_MESSAGE_BASE+0x13)
#define LI_MFB_BUTTON_SINGLE_PRESS               (LOGICAL_INPUT_MESSAGE_BASE+0x14)
#define APP_ANC_ENABLE                           (LOGICAL_INPUT_MESSAGE_BASE+0x15)
#define LI_MFB_BUTTON_RELEASE_DFU                (LOGICAL_INPUT_MESSAGE_BASE+0x16)
#define LI_MFB_BUTTON_RELEASE_6SEC               (LOGICAL_INPUT_MESSAGE_BASE+0x17)
#define LI_MFB_BUTTON_RELEASE_3SEC               (LOGICAL_INPUT_MESSAGE_BASE+0x18)
#define APP_BUTTON_VOLUME_DOWN                   (LOGICAL_INPUT_MESSAGE_BASE+0x19)
#define APP_BUTTON_FORWARD_HELD                  (LOGICAL_INPUT_MESSAGE_BASE+0x1a)
#define APP_ANC_SET_NEXT_MODE                    (LOGICAL_INPUT_MESSAGE_BASE+0x1b)
#define APP_ANC_TOGGLE_ON_OFF                    (LOGICAL_INPUT_MESSAGE_BASE+0x1c)
#define APP_ANC_DISABLE                          (LOGICAL_INPUT_MESSAGE_BASE+0x1d)
#define APP_BUTTON_FORWARD                       (LOGICAL_INPUT_MESSAGE_BASE+0x1e)
#define APP_LEAKTHROUGH_SET_NEXT_MODE            (LOGICAL_INPUT_MESSAGE_BASE+0x1f)
#define LI_MFB_BUTTON_HELD_8SEC                  (LOGICAL_INPUT_MESSAGE_BASE+0x20)
#define APP_VA_BUTTON_SINGLE_CLICK               (LOGICAL_INPUT_MESSAGE_BASE+0x21)
#define APP_BUTTON_VOLUME_DOWN_RELEASE           (LOGICAL_INPUT_MESSAGE_BASE+0x22)
#define APP_VA_BUTTON_DOUBLE_CLICK               (LOGICAL_INPUT_MESSAGE_BASE+0x23)
#define APP_LEAKTHROUGH_TOGGLE_ON_OFF            (LOGICAL_INPUT_MESSAGE_BASE+0x24)
#define APP_VA_BUTTON_HELD_RELEASE               (LOGICAL_INPUT_MESSAGE_BASE+0x25)

#endif
