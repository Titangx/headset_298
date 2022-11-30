/*****************************************************************************
Copyright (c) 2018 - 2021 Qualcomm Technologies International, Ltd.
******************************************************************************/

/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: calling.proto */

/* Do not generate deprecated warnings for self */
#ifndef PROTOBUF_C__NO_DEPRECATED
#define PROTOBUF_C__NO_DEPRECATED
#endif

#include "calling.pb-c.h"
void   forward_atcommand__init
                     (ForwardATCommand         *message)
{
  static const ForwardATCommand init_value = FORWARD_ATCOMMAND__INIT;
  *message = init_value;
}
size_t forward_atcommand__get_packed_size
                     (const ForwardATCommand *message)
{
  assert(message->base.descriptor == &forward_atcommand__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t forward_atcommand__pack
                     (const ForwardATCommand *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &forward_atcommand__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t forward_atcommand__pack_to_buffer
                     (const ForwardATCommand *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &forward_atcommand__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
ForwardATCommand *
       forward_atcommand__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (ForwardATCommand *)
     protobuf_c_message_unpack (&forward_atcommand__descriptor,
                                allocator, len, data);
}
void   forward_atcommand__free_unpacked
                     (ForwardATCommand *message,
                      ProtobufCAllocator *allocator)
{
  if(!message)
    return;
  assert(message->base.descriptor == &forward_atcommand__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   incoming_call__init
                     (IncomingCall         *message)
{
  static const IncomingCall init_value = INCOMING_CALL__INIT;
  *message = init_value;
}
size_t incoming_call__get_packed_size
                     (const IncomingCall *message)
{
  assert(message->base.descriptor == &incoming_call__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t incoming_call__pack
                     (const IncomingCall *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &incoming_call__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t incoming_call__pack_to_buffer
                     (const IncomingCall *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &incoming_call__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
IncomingCall *
       incoming_call__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (IncomingCall *)
     protobuf_c_message_unpack (&incoming_call__descriptor,
                                allocator, len, data);
}
void   incoming_call__free_unpacked
                     (IncomingCall *message,
                      ProtobufCAllocator *allocator)
{
  if(!message)
    return;
  assert(message->base.descriptor == &incoming_call__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
static const ProtobufCFieldDescriptor forward_atcommand__field_descriptors[1] =
{
  {
    "command",
    1,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(ForwardATCommand, command),
    NULL,
    &protobuf_c_empty_string,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned forward_atcommand__field_indices_by_name[] = {
  0,   /* field[0] = command */
};
static const ProtobufCIntRange forward_atcommand__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 1 }
};
const ProtobufCMessageDescriptor forward_atcommand__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "ForwardATCommand",
  "ForwardATCommand",
  "ForwardATCommand",
  "",
  sizeof(ForwardATCommand),
  1,
  forward_atcommand__field_descriptors,
  forward_atcommand__field_indices_by_name,
  1,  forward_atcommand__number_ranges,
  (ProtobufCMessageInit) forward_atcommand__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCFieldDescriptor incoming_call__field_descriptors[2] =
{
  {
    "caller_number",
    1,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(IncomingCall, caller_number),
    NULL,
    &protobuf_c_empty_string,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "caller_name",
    2,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(IncomingCall, caller_name),
    NULL,
    &protobuf_c_empty_string,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned incoming_call__field_indices_by_name[] = {
  1,   /* field[1] = caller_name */
  0,   /* field[0] = caller_number */
};
static const ProtobufCIntRange incoming_call__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 2 }
};
const ProtobufCMessageDescriptor incoming_call__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "IncomingCall",
  "IncomingCall",
  "IncomingCall",
  "",
  sizeof(IncomingCall),
  2,
  incoming_call__field_descriptors,
  incoming_call__field_indices_by_name,
  1,  incoming_call__number_ranges,
  (ProtobufCMessageInit) incoming_call__init,
  NULL,NULL,NULL    /* reserved[123] */
};
