/*****************************************************************************
Copyright (c) 2018 - 2021 Qualcomm Technologies International, Ltd.
******************************************************************************/

/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: system.proto */

#ifndef PROTOBUF_C_system_2eproto__INCLUDED
#define PROTOBUF_C_system_2eproto__INCLUDED

#include <protobuf.h>

PROTOBUF_C__BEGIN_DECLS

#if PROTOBUF_C_VERSION_NUMBER < 1003000
# error This file was generated by a newer version of protoc-c which is incompatible with your libprotobuf-c headers. Please update your headers.
#elif 1003000 < PROTOBUF_C_MIN_COMPILER_VERSION
# error This file was generated by an older version of protoc-c which is incompatible with your libprotobuf-c headers. Please regenerate this file with a newer version of protoc-c.
#endif


typedef struct _ResetConnection ResetConnection;
typedef struct _SynchronizeSettings SynchronizeSettings;
typedef struct _KeepAlive KeepAlive;
typedef struct _RemoveDevice RemoveDevice;
typedef struct _Locale Locale;
typedef struct _Locales Locales;
typedef struct _GetLocales GetLocales;
typedef struct _SetLocale SetLocale;
typedef struct _LaunchApp LaunchApp;


/* --- enums --- */

typedef enum _ResetConnection__ResetReason {
  RESET_CONNECTION__RESET_REASON__UNKNOWN = 0,
  RESET_CONNECTION__RESET_REASON__AAP_REFUSED_MAX_CONNECTIONS_REACHED = 1
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(RESET_CONNECTION__RESET_REASON)
} ResetConnection__ResetReason;

/* --- messages --- */

struct  _ResetConnection
{
  ProtobufCMessage base;
  uint32_t timeout;
  protobuf_c_boolean force_disconnect;
  ResetConnection__ResetReason reset_reason;
};
#define RESET_CONNECTION__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&reset_connection__descriptor) \
    , 0, 0, RESET_CONNECTION__RESET_REASON__UNKNOWN }


struct  _SynchronizeSettings
{
  ProtobufCMessage base;
  uint32_t timestamp_hi;
  uint32_t timestamp_lo;
};
#define SYNCHRONIZE_SETTINGS__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&synchronize_settings__descriptor) \
    , 0, 0 }


struct  _KeepAlive
{
  ProtobufCMessage base;
};
#define KEEP_ALIVE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&keep_alive__descriptor) \
     }


struct  _RemoveDevice
{
  ProtobufCMessage base;
};
#define REMOVE_DEVICE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&remove_device__descriptor) \
     }


struct  _Locale
{
  ProtobufCMessage base;
  char *name;
};
#define LOCALE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&locale__descriptor) \
    , (char *)protobuf_c_empty_string }


struct  _Locales
{
  ProtobufCMessage base;
  size_t n_supported_locales;
  Locale **supported_locales;
  Locale *current_locale;
};
#define LOCALES__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&locales__descriptor) \
    , 0,NULL, NULL }


struct  _GetLocales
{
  ProtobufCMessage base;
};
#define GET_LOCALES__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&get_locales__descriptor) \
     }


struct  _SetLocale
{
  ProtobufCMessage base;
  Locale *locale;
};
#define SET_LOCALE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&set_locale__descriptor) \
    , NULL }


struct  _LaunchApp
{
  ProtobufCMessage base;
  char *app_id;
};
#define LAUNCH_APP__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&launch_app__descriptor) \
    , (char *)protobuf_c_empty_string }


/* ResetConnection methods */
void   reset_connection__init
                     (ResetConnection         *message);
size_t reset_connection__get_packed_size
                     (const ResetConnection   *message);
size_t reset_connection__pack
                     (const ResetConnection   *message,
                      uint8_t             *out);
size_t reset_connection__pack_to_buffer
                     (const ResetConnection   *message,
                      ProtobufCBuffer     *buffer);
ResetConnection *
       reset_connection__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   reset_connection__free_unpacked
                     (ResetConnection *message,
                      ProtobufCAllocator *allocator);
/* SynchronizeSettings methods */
void   synchronize_settings__init
                     (SynchronizeSettings         *message);
size_t synchronize_settings__get_packed_size
                     (const SynchronizeSettings   *message);
size_t synchronize_settings__pack
                     (const SynchronizeSettings   *message,
                      uint8_t             *out);
size_t synchronize_settings__pack_to_buffer
                     (const SynchronizeSettings   *message,
                      ProtobufCBuffer     *buffer);
SynchronizeSettings *
       synchronize_settings__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   synchronize_settings__free_unpacked
                     (SynchronizeSettings *message,
                      ProtobufCAllocator *allocator);
/* KeepAlive methods */
void   keep_alive__init
                     (KeepAlive         *message);
size_t keep_alive__get_packed_size
                     (const KeepAlive   *message);
size_t keep_alive__pack
                     (const KeepAlive   *message,
                      uint8_t             *out);
size_t keep_alive__pack_to_buffer
                     (const KeepAlive   *message,
                      ProtobufCBuffer     *buffer);
KeepAlive *
       keep_alive__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   keep_alive__free_unpacked
                     (KeepAlive *message,
                      ProtobufCAllocator *allocator);
/* RemoveDevice methods */
void   remove_device__init
                     (RemoveDevice         *message);
size_t remove_device__get_packed_size
                     (const RemoveDevice   *message);
size_t remove_device__pack
                     (const RemoveDevice   *message,
                      uint8_t             *out);
size_t remove_device__pack_to_buffer
                     (const RemoveDevice   *message,
                      ProtobufCBuffer     *buffer);
RemoveDevice *
       remove_device__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   remove_device__free_unpacked
                     (RemoveDevice *message,
                      ProtobufCAllocator *allocator);
/* Locale methods */
void   locale__init
                     (Locale         *message);
size_t locale__get_packed_size
                     (const Locale   *message);
size_t locale__pack
                     (const Locale   *message,
                      uint8_t             *out);
size_t locale__pack_to_buffer
                     (const Locale   *message,
                      ProtobufCBuffer     *buffer);
Locale *
       locale__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   locale__free_unpacked
                     (Locale *message,
                      ProtobufCAllocator *allocator);
/* Locales methods */
void   locales__init
                     (Locales         *message);
size_t locales__get_packed_size
                     (const Locales   *message);
size_t locales__pack
                     (const Locales   *message,
                      uint8_t             *out);
size_t locales__pack_to_buffer
                     (const Locales   *message,
                      ProtobufCBuffer     *buffer);
Locales *
       locales__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   locales__free_unpacked
                     (Locales *message,
                      ProtobufCAllocator *allocator);
/* GetLocales methods */
void   get_locales__init
                     (GetLocales         *message);
size_t get_locales__get_packed_size
                     (const GetLocales   *message);
size_t get_locales__pack
                     (const GetLocales   *message,
                      uint8_t             *out);
size_t get_locales__pack_to_buffer
                     (const GetLocales   *message,
                      ProtobufCBuffer     *buffer);
GetLocales *
       get_locales__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   get_locales__free_unpacked
                     (GetLocales *message,
                      ProtobufCAllocator *allocator);
/* SetLocale methods */
void   set_locale__init
                     (SetLocale         *message);
size_t set_locale__get_packed_size
                     (const SetLocale   *message);
size_t set_locale__pack
                     (const SetLocale   *message,
                      uint8_t             *out);
size_t set_locale__pack_to_buffer
                     (const SetLocale   *message,
                      ProtobufCBuffer     *buffer);
SetLocale *
       set_locale__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   set_locale__free_unpacked
                     (SetLocale *message,
                      ProtobufCAllocator *allocator);
/* LaunchApp methods */
void   launch_app__init
                     (LaunchApp         *message);
size_t launch_app__get_packed_size
                     (const LaunchApp   *message);
size_t launch_app__pack
                     (const LaunchApp   *message,
                      uint8_t             *out);
size_t launch_app__pack_to_buffer
                     (const LaunchApp   *message,
                      ProtobufCBuffer     *buffer);
LaunchApp *
       launch_app__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   launch_app__free_unpacked
                     (LaunchApp *message,
                      ProtobufCAllocator *allocator);
/* --- per-message closures --- */

typedef void (*ResetConnection_Closure)
                 (const ResetConnection *message,
                  void *closure_data);
typedef void (*SynchronizeSettings_Closure)
                 (const SynchronizeSettings *message,
                  void *closure_data);
typedef void (*KeepAlive_Closure)
                 (const KeepAlive *message,
                  void *closure_data);
typedef void (*RemoveDevice_Closure)
                 (const RemoveDevice *message,
                  void *closure_data);
typedef void (*Locale_Closure)
                 (const Locale *message,
                  void *closure_data);
typedef void (*Locales_Closure)
                 (const Locales *message,
                  void *closure_data);
typedef void (*GetLocales_Closure)
                 (const GetLocales *message,
                  void *closure_data);
typedef void (*SetLocale_Closure)
                 (const SetLocale *message,
                  void *closure_data);
typedef void (*LaunchApp_Closure)
                 (const LaunchApp *message,
                  void *closure_data);

/* --- services --- */


/* --- descriptors --- */

extern const ProtobufCMessageDescriptor reset_connection__descriptor;
extern const ProtobufCEnumDescriptor    reset_connection__reset_reason__descriptor;
extern const ProtobufCMessageDescriptor synchronize_settings__descriptor;
extern const ProtobufCMessageDescriptor keep_alive__descriptor;
extern const ProtobufCMessageDescriptor remove_device__descriptor;
extern const ProtobufCMessageDescriptor locale__descriptor;
extern const ProtobufCMessageDescriptor locales__descriptor;
extern const ProtobufCMessageDescriptor get_locales__descriptor;
extern const ProtobufCMessageDescriptor set_locale__descriptor;
extern const ProtobufCMessageDescriptor launch_app__descriptor;

PROTOBUF_C__END_DECLS


#endif  /* PROTOBUF_C_system_2eproto__INCLUDED */
