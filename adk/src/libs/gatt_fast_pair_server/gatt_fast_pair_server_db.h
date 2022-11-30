/*
 * THIS FILE IS AUTOGENERATED, DO NOT EDIT!
 *
 * generated by gattdbgen from gatt_fast_pair_server/gatt_fast_pair_server_db.dbi_
 */
#ifndef __GATT_FAST_PAIR_SERVER_DB_H
#define __GATT_FAST_PAIR_SERVER_DB_H

#include <csrtypes.h>

#define HANDLE_FAST_PAIR_SERVICE        (0x0001)
#define HANDLE_FAST_PAIR_SERVICE_END    (0xffff)
#define HANDLE_MODEL_ID                 (0x0003)
#define HANDLE_KEYBASED_PAIRING         (0x0005)
#define HANDLE_KEYBASED_PAIRING_CLIENT_CONFIG (0x0006)
#define HANDLE_PASSKEY                  (0x0008)
#define HANDLE_PASSKEY_CLIENT_CONFIG    (0x0009)
#define HANDLE_ACCOUNT_KEY              (0x000b)
#define HANDLE_DATA                     (0x000d)
#define HANDLE_DATA_CLIENT_CONFIG       (0x000e)

uint16 *GattGetDatabase(uint16 *len);
uint16 GattGetDatabaseSize(void);
const uint16 *GattGetConstDatabase(uint16 *len);

#endif

/* End-of-File */
