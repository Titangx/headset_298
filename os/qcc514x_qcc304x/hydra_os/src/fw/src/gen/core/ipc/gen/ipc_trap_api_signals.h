/*
 * This file is autogenerated from api.xml by api_codegen.py
 *
 * Copyright (c) 2020 Qualcomm Technologies International, Ltd.
 */
IPC_SIGNAL_ID_UTIL_RANDOM = 0x1c000e,
IPC_SIGNAL_ID_UTIL_RANDOM_RSP = 0x1c800e,
IPC_SIGNAL_ID_PS_STORE = 0x1c0017,
IPC_SIGNAL_ID_PS_STORE_RSP = 0x1c8017,
IPC_SIGNAL_ID_PS_RETRIEVE = 0x1c0018,
IPC_SIGNAL_ID_PS_RETRIEVE_RSP = 0x1c8018,
IPC_SIGNAL_ID_PS_DEFRAG = 0x1c0019,
IPC_SIGNAL_ID_PS_FREE_COUNT = 0x1c001a,
IPC_SIGNAL_ID_PS_FREE_COUNT_RSP = 0x1c801a,
IPC_SIGNAL_ID_PS_FULL_RETRIEVE = 0x1c001b,
IPC_SIGNAL_ID_PS_FULL_RETRIEVE_RSP = 0x1c801b,
IPC_SIGNAL_ID_PS_DROP = 0x1c005c,
IPC_SIGNAL_ID_PS_DROP_RSP = 0x1c805c,
IPC_SIGNAL_ID_BOOT_GET_MODE = 0x1c001c,
IPC_SIGNAL_ID_BOOT_GET_MODE_RSP = 0x1c801c,
IPC_SIGNAL_ID_BOOT_SET_MODE = 0x1c001d,
IPC_SIGNAL_ID_VM_DEEP_SLEEP_ENABLE = 0x1c0023,
IPC_SIGNAL_ID_VM_DEEP_SLEEP_ENABLE_RSP = 0x1c8023,
IPC_SIGNAL_ID_VM_TRANSMIT_ENABLE = 0x1c0024,
IPC_SIGNAL_ID_VM_TRANSMIT_ENABLE_RSP = 0x1c8024,
IPC_SIGNAL_ID_VM_TRANSMIT_POWER_SET_DEFAULT = 0x1c0025,
IPC_SIGNAL_ID_VM_TRANSMIT_POWER_SET_DEFAULT_RSP = 0x1c8025,
IPC_SIGNAL_ID_VM_TRANSMIT_POWER_SET_MAXIMUM = 0x1c0026,
IPC_SIGNAL_ID_VM_TRANSMIT_POWER_SET_MAXIMUM_RSP = 0x1c8026,
IPC_SIGNAL_ID_VM_TRANSMIT_POWER_GET_DEFAULT = 0x1c0027,
IPC_SIGNAL_ID_VM_TRANSMIT_POWER_GET_DEFAULT_RSP = 0x1c8027,
IPC_SIGNAL_ID_VM_TRANSMIT_POWER_GET_MAXIMUM = 0x1c0028,
IPC_SIGNAL_ID_VM_TRANSMIT_POWER_GET_MAXIMUM_RSP = 0x1c8028,
IPC_SIGNAL_ID_VM_GET_TEMPERATURE = 0x1c0029,
IPC_SIGNAL_ID_VM_GET_TEMPERATURE_RSP = 0x1c8029,
IPC_SIGNAL_ID_VM_AMUX_CLOCK_ENABLE = 0x1f0000,
IPC_SIGNAL_ID_VM_READ_VREF_CONSTANT = 0x1c002c,
IPC_SIGNAL_ID_VM_READ_VREF_CONSTANT_RSP = 0x1c802c,
IPC_SIGNAL_ID_ADC_READ_REQUEST = 0x1c002d,
IPC_SIGNAL_ID_ADC_READ_REQUEST_RSP = 0x1c802d,
IPC_SIGNAL_ID_PS_UPDATE_AUDIO_KEY = 0x1c002f,
IPC_SIGNAL_ID_PS_UPDATE_AUDIO_KEY_RSP = 0x1c802f,
IPC_SIGNAL_ID_PS_READ_AUDIO_KEY = 0x1c0030,
IPC_SIGNAL_ID_PS_READ_AUDIO_KEY_RSP = 0x1c8030,
IPC_SIGNAL_ID_NATIVE_PAUSE_FOR_DEBUG = 0x1a0000,
IPC_SIGNAL_ID_SINK_SLACK = 0x160002,
IPC_SIGNAL_ID_SINK_SLACK_RSP = 0x168002,
IPC_SIGNAL_ID_SINK_CLAIM = 0x160003,
IPC_SIGNAL_ID_SINK_CLAIM_RSP = 0x168003,
IPC_SIGNAL_ID_SINK_MAP = 0x160004,
IPC_SIGNAL_ID_SINK_MAP_RSP = 0x168004,
IPC_SIGNAL_ID_SINK_FLUSH = 0x160005,
IPC_SIGNAL_ID_SINK_FLUSH_RSP = 0x168005,
IPC_SIGNAL_ID_SINK_FLUSH_HEADER = 0x160006,
IPC_SIGNAL_ID_SINK_FLUSH_HEADER_RSP = 0x168006,
IPC_SIGNAL_ID_SOURCE_SIZE = 0x160007,
IPC_SIGNAL_ID_SOURCE_SIZE_RSP = 0x168007,
IPC_SIGNAL_ID_SOURCE_SIZE_HEADER = 0x160008,
IPC_SIGNAL_ID_SOURCE_SIZE_HEADER_RSP = 0x168008,
IPC_SIGNAL_ID_SOURCE_MAP = 0x160009,
IPC_SIGNAL_ID_SOURCE_MAP_RSP = 0x168009,
IPC_SIGNAL_ID_SOURCE_MAP_HEADER = 0x16000a,
IPC_SIGNAL_ID_SOURCE_MAP_HEADER_RSP = 0x16800a,
IPC_SIGNAL_ID_SOURCE_DROP = 0x16000b,
IPC_SIGNAL_ID_SOURCE_BOUNDARY = 0x16000c,
IPC_SIGNAL_ID_SOURCE_BOUNDARY_RSP = 0x16800c,
IPC_SIGNAL_ID_SOURCE_SIZE_BLOCKING = 0x160035,
IPC_SIGNAL_ID_SOURCE_SIZE_BLOCKING_RSP = 0x168035,
IPC_SIGNAL_ID_STREAM_MOVE = 0x16000d,
IPC_SIGNAL_ID_STREAM_MOVE_RSP = 0x16800d,
IPC_SIGNAL_ID_STREAM_CONNECT = 0x16000e,
IPC_SIGNAL_ID_STREAM_CONNECT_RSP = 0x16800e,
IPC_SIGNAL_ID_STREAM_CONNECT_DISPOSE = 0x16000f,
IPC_SIGNAL_ID_STREAM_CONNECT_DISPOSE_RSP = 0x16800f,
IPC_SIGNAL_ID_STREAM_DISCONNECT = 0x160010,
IPC_SIGNAL_ID_STREAM_DISCONNECT_RSP = 0x168010,
IPC_SIGNAL_ID_SOURCE_IS_VALID = 0x160033,
IPC_SIGNAL_ID_SOURCE_IS_VALID_RSP = 0x168033,
IPC_SIGNAL_ID_SINK_IS_VALID = 0x160034,
IPC_SIGNAL_ID_SINK_IS_VALID_RSP = 0x168034,
IPC_SIGNAL_ID_STREAM_CONFIGURE = 0x160011,
IPC_SIGNAL_ID_STREAM_CONFIGURE_RSP = 0x168011,
IPC_SIGNAL_ID_SOURCE_CONFIGURE = 0x160012,
IPC_SIGNAL_ID_SOURCE_CONFIGURE_RSP = 0x168012,
IPC_SIGNAL_ID_SINK_CONFIGURE = 0x160013,
IPC_SIGNAL_ID_SINK_CONFIGURE_RSP = 0x168013,
IPC_SIGNAL_ID_STREAM_SOURCE_FROM_SINK = 0x160014,
IPC_SIGNAL_ID_STREAM_SOURCE_FROM_SINK_RSP = 0x168014,
IPC_SIGNAL_ID_STREAM_SINK_FROM_SOURCE = 0x160015,
IPC_SIGNAL_ID_STREAM_SINK_FROM_SOURCE_RSP = 0x168015,
IPC_SIGNAL_ID_SOURCE_CLOSE = 0x160016,
IPC_SIGNAL_ID_SOURCE_CLOSE_RSP = 0x168016,
IPC_SIGNAL_ID_SINK_CLOSE = 0x160017,
IPC_SIGNAL_ID_SINK_CLOSE_RSP = 0x168017,
IPC_SIGNAL_ID_SINK_ALIAS = 0x160018,
IPC_SIGNAL_ID_SINK_ALIAS_RSP = 0x168018,
IPC_SIGNAL_ID_SINK_SYNCHRONISE = 0x160019,
IPC_SIGNAL_ID_SINK_SYNCHRONISE_RSP = 0x168019,
IPC_SIGNAL_ID_SOURCE_SYNCHRONISE = 0x16001a,
IPC_SIGNAL_ID_SOURCE_SYNCHRONISE_RSP = 0x16801a,
IPC_SIGNAL_ID_TRANSFORM_START = 0x16001b,
IPC_SIGNAL_ID_TRANSFORM_START_RSP = 0x16801b,
IPC_SIGNAL_ID_TRANSFORM_STOP = 0x16001c,
IPC_SIGNAL_ID_TRANSFORM_STOP_RSP = 0x16801c,
IPC_SIGNAL_ID_TRANSFORM_DISCONNECT = 0x16001d,
IPC_SIGNAL_ID_TRANSFORM_DISCONNECT_RSP = 0x16801d,
IPC_SIGNAL_ID_TRANSFORM_POLL_TRAFFIC = 0x16001e,
IPC_SIGNAL_ID_TRANSFORM_POLL_TRAFFIC_RSP = 0x16801e,
IPC_SIGNAL_ID_TRANSFORM_FROM_SOURCE = 0x16001f,
IPC_SIGNAL_ID_TRANSFORM_FROM_SOURCE_RSP = 0x16801f,
IPC_SIGNAL_ID_TRANSFORM_FROM_SINK = 0x160020,
IPC_SIGNAL_ID_TRANSFORM_FROM_SINK_RSP = 0x168020,
IPC_SIGNAL_ID_TRANSFORM_CONFIGURE = 0x160021,
IPC_SIGNAL_ID_TRANSFORM_CONFIGURE_RSP = 0x168021,
IPC_SIGNAL_ID_TRANSFORM_QUERY = 0x16003a,
IPC_SIGNAL_ID_TRANSFORM_QUERY_RSP = 0x16803a,
IPC_SIGNAL_ID_STREAM_REGION_SOURCE = 0x160022,
IPC_SIGNAL_ID_STREAM_REGION_SOURCE_RSP = 0x168022,
IPC_SIGNAL_ID_TRANSFORM_CHUNK = 0x160023,
IPC_SIGNAL_ID_TRANSFORM_CHUNK_RSP = 0x168023,
IPC_SIGNAL_ID_TRANSFORM_SLICE = 0x160024,
IPC_SIGNAL_ID_TRANSFORM_SLICE_RSP = 0x168024,
IPC_SIGNAL_ID_TRANSFORM_ADPCM_DECODE = 0x160025,
IPC_SIGNAL_ID_TRANSFORM_ADPCM_DECODE_RSP = 0x168025,
IPC_SIGNAL_ID_SINK_MAP_INIT = 0x16002e,
IPC_SIGNAL_ID_SINK_MAP_INIT_RSP = 0x16802e,
IPC_SIGNAL_ID_SOURCE_MAP_INIT = 0x16002f,
IPC_SIGNAL_ID_SOURCE_MAP_INIT_RSP = 0x16802f,
IPC_SIGNAL_ID_SINK_UNMAP = 0x160030,
IPC_SIGNAL_ID_SINK_UNMAP_RSP = 0x168030,
IPC_SIGNAL_ID_SOURCE_UNMAP = 0x160031,
IPC_SIGNAL_ID_SOURCE_UNMAP_RSP = 0x168031,
IPC_SIGNAL_ID_SINK_FLUSH_BLOCKING = 0x160032,
IPC_SIGNAL_ID_SINK_FLUSH_BLOCKING_RSP = 0x168032,
IPC_SIGNAL_ID_TRANSFORM_PACKETISE = 0x160036,
IPC_SIGNAL_ID_TRANSFORM_PACKETISE_RSP = 0x168036,
IPC_SIGNAL_ID_STATUS_QUERY = 0x2e0001,
IPC_SIGNAL_ID_HOST_SEND_MESSAGE = 0x200000,
IPC_SIGNAL_ID_HOST_SEND_MESSAGE_RSP = 0x208000,
IPC_SIGNAL_ID_MESSAGE_HOST_COMMS_TASK = 0x200001,
IPC_SIGNAL_ID_MESSAGE_HOST_COMMS_TASK_RSP = 0x208001,
IPC_SIGNAL_ID_STREAM_HOST_SINK = 0x60000,
IPC_SIGNAL_ID_STREAM_HOST_SINK_RSP = 0x68000,
IPC_SIGNAL_ID_STREAM_CHARGER_COMMS_UART_SINK = 0x480002,
IPC_SIGNAL_ID_STREAM_CHARGER_COMMS_UART_SINK_RSP = 0x488002,
IPC_SIGNAL_ID_CHARGER_COMMS_UART_CONFIGURE = 0x480003,
IPC_SIGNAL_ID_CHARGER_COMMS_UART_CONFIGURE_RSP = 0x488003,
IPC_SIGNAL_ID_STREAM_UART_CONFIGURE = 0x50001,
IPC_SIGNAL_ID_STREAM_UART_CONFIGURE_RSP = 0x58001,
IPC_SIGNAL_ID_STREAM_RINGTONE_SOURCE = 0xa0000,
IPC_SIGNAL_ID_STREAM_RINGTONE_SOURCE_RSP = 0xa8000,
IPC_SIGNAL_ID_STREAM_AUDIO_SOURCE = 0xa0001,
IPC_SIGNAL_ID_STREAM_AUDIO_SOURCE_RSP = 0xa8001,
IPC_SIGNAL_ID_STREAM_AUDIO_SINK = 0xa0002,
IPC_SIGNAL_ID_STREAM_AUDIO_SINK_RSP = 0xa8002,
IPC_SIGNAL_ID_CODEC_SET_IIR_FILTER = 0x2b0000,
IPC_SIGNAL_ID_CODEC_SET_IIR_FILTER_RSP = 0x2b8000,
IPC_SIGNAL_ID_STREAM_SCO_SINK = 0xa0003,
IPC_SIGNAL_ID_STREAM_SCO_SINK_RSP = 0xa8003,
IPC_SIGNAL_ID_STREAM_SCO_SOURCE = 0xa0006,
IPC_SIGNAL_ID_STREAM_SCO_SOURCE_RSP = 0xa8006,
IPC_SIGNAL_ID_SOURCE_GET_SCO_HANDLE = 0xa0007,
IPC_SIGNAL_ID_SOURCE_GET_SCO_HANDLE_RSP = 0xa8007,
IPC_SIGNAL_ID_SINK_GET_SCO_HANDLE = 0xa0004,
IPC_SIGNAL_ID_SINK_GET_SCO_HANDLE_RSP = 0xa8004,
IPC_SIGNAL_ID_STREAM_RFCOMM_SINK = 0xd0001,
IPC_SIGNAL_ID_STREAM_RFCOMM_SINK_RSP = 0xd8001,
IPC_SIGNAL_ID_SINK_GET_RFCOMM_CONN_ID = 0xd0002,
IPC_SIGNAL_ID_SINK_GET_RFCOMM_CONN_ID_RSP = 0xd8002,
IPC_SIGNAL_ID_SINK_GET_RFCOMM_SERVER_CHANNEL = 0xd0003,
IPC_SIGNAL_ID_SINK_GET_RFCOMM_SERVER_CHANNEL_RSP = 0xd8003,
IPC_SIGNAL_ID_STREAM_RFCOMM_SINK_FROM_SERVER_CHANNEL = 0xd0004,
IPC_SIGNAL_ID_STREAM_RFCOMM_SINK_FROM_SERVER_CHANNEL_RSP = 0xd8004,
IPC_SIGNAL_ID_SINK_GET_RFCOMM_DLCI = 0xd0005,
IPC_SIGNAL_ID_SINK_GET_RFCOMM_DLCI_RSP = 0xd8005,
IPC_SIGNAL_ID_STREAM_RFCOMM_SINK_FROM_DLCI = 0xd0006,
IPC_SIGNAL_ID_STREAM_RFCOMM_SINK_FROM_DLCI_RSP = 0xd8006,
IPC_SIGNAL_ID_STREAM_L2CAP_SINK = 0xf0006,
IPC_SIGNAL_ID_STREAM_L2CAP_SINK_RSP = 0xf8006,
IPC_SIGNAL_ID_SINK_GET_L2CAP_CID = 0xf0007,
IPC_SIGNAL_ID_SINK_GET_L2CAP_CID_RSP = 0xf8007,
IPC_SIGNAL_ID_SINK_GET_BD_ADDR = 0x160026,
IPC_SIGNAL_ID_SINK_GET_BD_ADDR_RSP = 0x168026,
IPC_SIGNAL_ID_STREAM_SINKS_FROM_BD_ADDR = 0x160027,
IPC_SIGNAL_ID_STREAM_SINKS_FROM_BD_ADDR_RSP = 0x168027,
IPC_SIGNAL_ID_SINK_GET_RSSI = 0x160028,
IPC_SIGNAL_ID_SINK_GET_RSSI_RSP = 0x168028,
IPC_SIGNAL_ID_SINK_POLL_AWAY_TIME = 0xf0008,
IPC_SIGNAL_ID_SINK_POLL_AWAY_TIME_RSP = 0xf8008,
IPC_SIGNAL_ID_INQUIRY_SET_PRIORITY = 0xf0009,
IPC_SIGNAL_ID_INQUIRY_SET_PRIORITY_RSP = 0xf8009,
IPC_SIGNAL_ID_INQUIRY_GET_PRIORITY = 0xf000a,
IPC_SIGNAL_ID_INQUIRY_GET_PRIORITY_RSP = 0xf800a,
IPC_SIGNAL_ID_FILE_FIND = 0x30000,
IPC_SIGNAL_ID_FILE_FIND_RSP = 0x38000,
IPC_SIGNAL_ID_FILE_TYPE = 0x30001,
IPC_SIGNAL_ID_FILE_TYPE_RSP = 0x38001,
IPC_SIGNAL_ID_FILE_PARENT = 0x30002,
IPC_SIGNAL_ID_FILE_PARENT_RSP = 0x38002,
IPC_SIGNAL_ID_STREAM_FILE_SOURCE = 0x30003,
IPC_SIGNAL_ID_STREAM_FILE_SOURCE_RSP = 0x38003,
IPC_SIGNAL_ID_STREAM_FILESYSTEM_SOURCE = 0x30008,
IPC_SIGNAL_ID_STREAM_FILESYSTEM_SOURCE_RSP = 0x38008,
IPC_SIGNAL_ID_STREAM_FILESYSTEM_SINK = 0x30009,
IPC_SIGNAL_ID_STREAM_FILESYSTEM_SINK_RSP = 0x38009,
IPC_SIGNAL_ID_FILE_SYSTEM_UNMOUNT = 0x3000a,
IPC_SIGNAL_ID_FILE_SYSTEM_UNMOUNT_RSP = 0x3800a,
IPC_SIGNAL_ID_FILE_MAP = 0x3000b,
IPC_SIGNAL_ID_FILE_MAP_RSP = 0x3800b,
IPC_SIGNAL_ID_STREAM_SD_MMC_SOURCE = 0x340000,
IPC_SIGNAL_ID_STREAM_SD_MMC_SOURCE_RSP = 0x348000,
IPC_SIGNAL_ID_STREAM_SD_MMC_SINK = 0x340001,
IPC_SIGNAL_ID_STREAM_SD_MMC_SINK_RSP = 0x348001,
IPC_SIGNAL_ID_SD_MMC_DEVICE_INFO = 0x340004,
IPC_SIGNAL_ID_SD_MMC_DEVICE_INFO_RSP = 0x348004,
IPC_SIGNAL_ID_PIO_GET_KALIMBA_CONTROL32 = 0xc0006,
IPC_SIGNAL_ID_PIO_GET_KALIMBA_CONTROL32_RSP = 0xc8006,
IPC_SIGNAL_ID_PIO_SET_KALIMBA_CONTROL32 = 0xc0007,
IPC_SIGNAL_ID_PIO_SET_KALIMBA_CONTROL32_RSP = 0xc8007,
IPC_SIGNAL_ID_I2C_TRANSFER = 0x2d004b,
IPC_SIGNAL_ID_I2C_TRANSFER_RSP = 0x2d804b,
IPC_SIGNAL_ID_STREAM_I2C_SOURCE = 0x2a004c,
IPC_SIGNAL_ID_STREAM_I2C_SOURCE_RSP = 0x2a804c,
IPC_SIGNAL_ID_TEST_PERFORM_ = 0x300000,
IPC_SIGNAL_ID_TEST_PERFORM__RSP = 0x308000,
IPC_SIGNAL_ID_ENERGY_ESTIMATION_SET_BOUNDS = 0x70000,
IPC_SIGNAL_ID_ENERGY_ESTIMATION_SET_BOUNDS_RSP = 0x78000,
IPC_SIGNAL_ID_ENERGY_ESTIMATION_OFF = 0x70001,
IPC_SIGNAL_ID_ENERGY_ESTIMATION_OFF_RSP = 0x78001,
IPC_SIGNAL_ID_USB_ATTACHED_STATUS = 0x250000,
IPC_SIGNAL_ID_USB_ATTACHED_STATUS_RSP = 0x258000,
IPC_SIGNAL_ID_USB_HUB_ATTACH = 0x260000,
IPC_SIGNAL_ID_USB_HUB_ATTACH_RSP = 0x268000,
IPC_SIGNAL_ID_USB_HUB_DETACH = 0x260001,
IPC_SIGNAL_ID_USB_HUB_DETACH_RSP = 0x268001,
IPC_SIGNAL_ID_USB_HUB_CONFIGURE = 0x260002,
IPC_SIGNAL_ID_USB_HUB_CONFIGURE_RSP = 0x268002,
IPC_SIGNAL_ID_USB_HUB_CONFIG_KEY = 0x260003,
IPC_SIGNAL_ID_USB_HUB_CONFIG_KEY_RSP = 0x268003,
IPC_SIGNAL_ID_USB_ADD_INTERFACE = 0x250001,
IPC_SIGNAL_ID_USB_ADD_INTERFACE_RSP = 0x258001,
IPC_SIGNAL_ID_USB_ADD_END_POINTS = 0x250002,
IPC_SIGNAL_ID_USB_ADD_END_POINTS_RSP = 0x258002,
IPC_SIGNAL_ID_USB_ADD_DESCRIPTOR = 0x250003,
IPC_SIGNAL_ID_USB_ADD_DESCRIPTOR_RSP = 0x258003,
IPC_SIGNAL_ID_USB_ADD_STRING_DESCRIPTOR = 0x250004,
IPC_SIGNAL_ID_USB_ADD_STRING_DESCRIPTOR_RSP = 0x258004,
IPC_SIGNAL_ID_USB_ADD_INTERFACE_ASSOCIATION_DESCRIPTOR = 0x250005,
IPC_SIGNAL_ID_USB_ADD_INTERFACE_ASSOCIATION_DESCRIPTOR_RSP = 0x258005,
IPC_SIGNAL_ID_STREAM_USB_CLASS_SINK = 0x250007,
IPC_SIGNAL_ID_STREAM_USB_CLASS_SINK_RSP = 0x258007,
IPC_SIGNAL_ID_STREAM_USB_END_POINT_SINK = 0x250008,
IPC_SIGNAL_ID_STREAM_USB_END_POINT_SINK_RSP = 0x258008,
IPC_SIGNAL_ID_STREAM_USB_VENDOR_SINK = 0x250009,
IPC_SIGNAL_ID_STREAM_USB_VENDOR_SINK_RSP = 0x258009,
IPC_SIGNAL_ID_PSU_CONFIGURE = 0x140001,
IPC_SIGNAL_ID_PSU_CONFIGURE_RSP = 0x148001,
IPC_SIGNAL_ID_VM_GET_POWER_SOURCE = 0x140002,
IPC_SIGNAL_ID_VM_GET_POWER_SOURCE_RSP = 0x148002,
IPC_SIGNAL_ID_CHARGER_CONFIGURE = 0x20000,
IPC_SIGNAL_ID_CHARGER_CONFIGURE_RSP = 0x28000,
IPC_SIGNAL_ID_CHARGER_STATUS = 0x20001,
IPC_SIGNAL_ID_CHARGER_STATUS_RSP = 0x28001,
IPC_SIGNAL_ID_CHARGER_DEBOUNCE = 0x130001,
IPC_SIGNAL_ID_CHARGER_DEBOUNCE_RSP = 0x138001,
IPC_SIGNAL_ID_CHARGER_MESSAGE_REQUEST = 0x130003,
IPC_SIGNAL_ID_CHARGER_MESSAGE_REQUEST_RSP = 0x138003,
IPC_SIGNAL_ID_MICBIAS_CONFIGURE = 0x150000,
IPC_SIGNAL_ID_MICBIAS_CONFIGURE_RSP = 0x158000,
IPC_SIGNAL_ID_FONT_INIT = 0xe0000,
IPC_SIGNAL_ID_FONT_INIT_RSP = 0xe8000,
IPC_SIGNAL_ID_FONT_GET_GLYPH = 0xe0001,
IPC_SIGNAL_ID_FONT_GET_GLYPH_RSP = 0xe8001,
IPC_SIGNAL_ID_STREAM_FAST_PIPE_SINK = 0x10000,
IPC_SIGNAL_ID_STREAM_FAST_PIPE_SINK_RSP = 0x18000,
IPC_SIGNAL_ID_STREAM_SHUNT_SINK = 0x290000,
IPC_SIGNAL_ID_STREAM_SHUNT_SINK_RSP = 0x298000,
IPC_SIGNAL_ID_SRAM_MAP = 0x270000,
IPC_SIGNAL_ID_SRAM_MAP_RSP = 0x278000,
IPC_SIGNAL_ID_VM_GET_TEMPERATURE_BY_SENSOR = 0x1c0041,
IPC_SIGNAL_ID_VM_GET_TEMPERATURE_BY_SENSOR_RSP = 0x1c8041,
IPC_SIGNAL_ID_VM_TRANSMIT_POWER_MESSAGES_ENABLE = 0x1c0042,
IPC_SIGNAL_ID_VM_GET_RESET_SOURCE = 0x1c0000,
IPC_SIGNAL_ID_VM_GET_RESET_SOURCE_RSP = 0x1c8000,
IPC_SIGNAL_ID_STREAM_ATT_SOURCE = 0x180001,
IPC_SIGNAL_ID_STREAM_ATT_SOURCE_RSP = 0x188001,
IPC_SIGNAL_ID_VM_CLEAR_ADVERTISING_REPORT_FILTER = 0x180002,
IPC_SIGNAL_ID_VM_CLEAR_ADVERTISING_REPORT_FILTER_RSP = 0x188002,
IPC_SIGNAL_ID_VM_ADD_ADVERTISING_REPORT_FILTER = 0x180003,
IPC_SIGNAL_ID_VM_ADD_ADVERTISING_REPORT_FILTER_RSP = 0x188003,
IPC_SIGNAL_ID_CAPACITIVE_SENSOR_PAD_QUERY = 0x80002,
IPC_SIGNAL_ID_CAPACITIVE_SENSOR_PAD_QUERY_RSP = 0x88002,
IPC_SIGNAL_ID_CAPACITIVE_SENSOR_CONFIGURE_MULTI_PAD = 0x80004,
IPC_SIGNAL_ID_CAPACITIVE_SENSOR_CONFIGURE_MULTI_PAD_RSP = 0x88004,
IPC_SIGNAL_ID_CAPACITIVE_SENSOR_ENABLE_PADS = 0x80005,
IPC_SIGNAL_ID_CAPACITIVE_SENSOR_ENABLE_PADS_RSP = 0x88005,
IPC_SIGNAL_ID_CAPACITIVE_SENSOR_CONFIGURE_TOUCH_OFFSET = 0x80006,
IPC_SIGNAL_ID_CAPACITIVE_SENSOR_CONFIGURE_TOUCH_OFFSET_RSP = 0x88006,
IPC_SIGNAL_ID_CAPACITIVE_SENSOR_SHIELD_ENABLE = 0x80007,
IPC_SIGNAL_ID_CAPACITIVE_SENSOR_SHIELD_ENABLE_RSP = 0x88007,
IPC_SIGNAL_ID_USB_ADD_CONFIGURATION = 0x25000a,
IPC_SIGNAL_ID_USB_ADD_CONFIGURATION_RSP = 0x25800a,
IPC_SIGNAL_ID_OTP_READ_DATA = 0x2c0000,
IPC_SIGNAL_ID_OTP_READ_DATA_RSP = 0x2c8000,
IPC_SIGNAL_ID_OTP_WRITE_DATA = 0x2c0001,
IPC_SIGNAL_ID_OTP_WRITE_DATA_RSP = 0x2c8001,
IPC_SIGNAL_ID_LCD_CONFIGURE = 0x120000,
IPC_SIGNAL_ID_LCD_CONFIGURE_RSP = 0x128000,
IPC_SIGNAL_ID_LCD_SET = 0x120001,
IPC_SIGNAL_ID_LCD_SET_RSP = 0x128001,
IPC_SIGNAL_ID_CHARGER_GET_BATTERY_STATUS_AT_BOOT = 0x280000,
IPC_SIGNAL_ID_CHARGER_GET_BATTERY_STATUS_AT_BOOT_RSP = 0x288000,
IPC_SIGNAL_ID_USB_DEVICE_STATE = 0x25000c,
IPC_SIGNAL_ID_USB_DEVICE_STATE_RSP = 0x25800c,
IPC_SIGNAL_ID_USB_ADD_ALT_INTERFACE = 0x25000d,
IPC_SIGNAL_ID_USB_ADD_ALT_INTERFACE_RSP = 0x25800d,
IPC_SIGNAL_ID_VM_GET_BD_ADDRT_FROM_CID = 0xf000b,
IPC_SIGNAL_ID_VM_GET_BD_ADDRT_FROM_CID_RSP = 0xf800b,
IPC_SIGNAL_ID_STREAM_ATT_SOURCE_ADD_HANDLE = 0x180005,
IPC_SIGNAL_ID_STREAM_ATT_SOURCE_ADD_HANDLE_RSP = 0x188005,
IPC_SIGNAL_ID_TRANSFORM_HID = 0x190000,
IPC_SIGNAL_ID_TRANSFORM_HID_RSP = 0x198000,
IPC_SIGNAL_ID_IMAGE_UPGRADE_GET_INFO = 0x350000,
IPC_SIGNAL_ID_IMAGE_UPGRADE_GET_INFO_RSP = 0x358000,
IPC_SIGNAL_ID_IMAGE_UPGRADE_STREAM_GET_SINK = 0x350001,
IPC_SIGNAL_ID_IMAGE_UPGRADE_STREAM_GET_SINK_RSP = 0x358001,
IPC_SIGNAL_ID_IMAGE_UPGRADE_SINK_GET_POSITION = 0x350002,
IPC_SIGNAL_ID_IMAGE_UPGRADE_SINK_GET_POSITION_RSP = 0x358002,
IPC_SIGNAL_ID_IMAGE_UPGRADE_ERASE = 0x350003,
IPC_SIGNAL_ID_IMAGE_UPGRADE_SWAP_TRY = 0x350005,
IPC_SIGNAL_ID_IMAGE_UPGRADE_SWAP_TRY_RSP = 0x358005,
IPC_SIGNAL_ID_IMAGE_UPGRADE_SWAP_TRY_STATUS = 0x350006,
IPC_SIGNAL_ID_IMAGE_UPGRADE_SWAP_TRY_STATUS_RSP = 0x358006,
IPC_SIGNAL_ID_IMAGE_UPGRADE_SWAP_COMMIT = 0x350007,
IPC_SIGNAL_ID_IMAGE_UPGRADE_SWAP_COMMIT_RSP = 0x358007,
IPC_SIGNAL_ID_IMAGE_UPGRADE_HASH_INITIALISE = 0x350008,
IPC_SIGNAL_ID_IMAGE_UPGRADE_HASH_INITIALISE_RSP = 0x358008,
IPC_SIGNAL_ID_IMAGE_UPGRADE_HASH_SECTION_UPDATE = 0x350009,
IPC_SIGNAL_ID_IMAGE_UPGRADE_HASH_SECTION_UPDATE_RSP = 0x358009,
IPC_SIGNAL_ID_IMAGE_UPGRADE_HASH_MSG_UPDATE = 0x35000a,
IPC_SIGNAL_ID_IMAGE_UPGRADE_HASH_MSG_UPDATE_RSP = 0x35800a,
IPC_SIGNAL_ID_IMAGE_UPGRADE_HASH_FINALISE = 0x35000b,
IPC_SIGNAL_ID_IMAGE_UPGRADE_HASH_FINALISE_RSP = 0x35800b,
IPC_SIGNAL_ID_IMAGE_UPGRADE_COPY = 0x35000c,
IPC_SIGNAL_ID_IMAGE_UPGRADE_ABORT_COMMAND = 0x35000d,
IPC_SIGNAL_ID_IMAGE_UPGRADE_ABORT_COMMAND_RSP = 0x35800d,
IPC_SIGNAL_ID_IMAGE_UPGRADE_AUDIO = 0x35000e,
IPC_SIGNAL_ID_IMAGE_UPGRADE_HASH_ALL_SECTIONS_UPDATE = 0x35000f,
IPC_SIGNAL_ID_IMAGE_UPGRADE_STREAM_GET_SOURCE = 0x350010,
IPC_SIGNAL_ID_IMAGE_UPGRADE_STREAM_GET_SOURCE_RSP = 0x358010,
IPC_SIGNAL_ID_IMAGE_UPGRADE_STREAM_GET_SOURCE_FROM_OFFSET = 0x350011,
IPC_SIGNAL_ID_IMAGE_UPGRADE_STREAM_GET_SOURCE_FROM_OFFSET_RSP = 0x358011,
IPC_SIGNAL_ID_STREAM_PARTITION_OVERWRITE_SINK = 0x1e0000,
IPC_SIGNAL_ID_STREAM_PARTITION_OVERWRITE_SINK_RSP = 0x1e8000,
IPC_SIGNAL_ID_PARTITION_MOUNT_FILESYSTEM = 0x1e0001,
IPC_SIGNAL_ID_PARTITION_MOUNT_FILESYSTEM_RSP = 0x1e8001,
IPC_SIGNAL_ID_STREAM_ATT_SOURCE_REMOVE_ALL_HANDLES = 0x180006,
IPC_SIGNAL_ID_STREAM_ATT_SOURCE_REMOVE_ALL_HANDLES_RSP = 0x188006,
IPC_SIGNAL_ID_PARTITION_GET_INFO = 0x1e0002,
IPC_SIGNAL_ID_PARTITION_GET_INFO_RSP = 0x1e8002,
IPC_SIGNAL_ID_PARTITION_SET_MESSAGE_DIGEST = 0x1e0003,
IPC_SIGNAL_ID_PARTITION_SET_MESSAGE_DIGEST_RSP = 0x1e8003,
IPC_SIGNAL_ID_TRANSFORM_RTP_ENCODE = 0xc0008,
IPC_SIGNAL_ID_TRANSFORM_RTP_ENCODE_RSP = 0xc8008,
IPC_SIGNAL_ID_TRANSFORM_RTP_DECODE = 0xc0009,
IPC_SIGNAL_ID_TRANSFORM_RTP_DECODE_RSP = 0xc8009,
IPC_SIGNAL_ID_PARTITION_GET_RAW_SERIAL_SOURCE = 0x1e0004,
IPC_SIGNAL_ID_PARTITION_GET_RAW_SERIAL_SOURCE_RSP = 0x1e8004,
IPC_SIGNAL_ID_VM_SOFTWARE_WD_KICK = 0x1c0046,
IPC_SIGNAL_ID_VM_SOFTWARE_WD_KICK_RSP = 0x1c8046,
IPC_SIGNAL_ID_STREAM_REFORMAT_SERIAL_FLASH_SINK = 0x100000,
IPC_SIGNAL_ID_STREAM_REFORMAT_SERIAL_FLASH_SINK_RSP = 0x108000,
IPC_SIGNAL_ID_INFRARED_CONFIGURE = 0x170001,
IPC_SIGNAL_ID_INFRARED_CONFIGURE_RSP = 0x178001,
IPC_SIGNAL_ID_CODEC_SET_IIR_FILTER16_BIT = 0x320000,
IPC_SIGNAL_ID_CODEC_SET_IIR_FILTER16_BIT_RSP = 0x328000,
IPC_SIGNAL_ID_STREAM_PARTITION_RESUME_SINK = 0x1e0005,
IPC_SIGNAL_ID_STREAM_PARTITION_RESUME_SINK_RSP = 0x1e8005,
IPC_SIGNAL_ID_PARTITION_SINK_GET_POSITION = 0x1e0006,
IPC_SIGNAL_ID_PARTITION_SINK_GET_POSITION_RSP = 0x1e8006,
IPC_SIGNAL_ID_FEATURE_VERIFY_LICENSE = 0x1c0048,
IPC_SIGNAL_ID_FEATURE_VERIFY_LICENSE_RSP = 0x1c8048,
IPC_SIGNAL_ID_PS_STORE_FS_TAB = 0x1e0007,
IPC_SIGNAL_ID_PS_STORE_FS_TAB_RSP = 0x1e8007,
IPC_SIGNAL_ID_PS_SET_STORE = 0x1c0049,
IPC_SIGNAL_ID_PS_GET_STORE = 0x1c004a,
IPC_SIGNAL_ID_PS_GET_STORE_RSP = 0x1c804a,
IPC_SIGNAL_ID_OPERATOR_CREATE = 0x110000,
IPC_SIGNAL_ID_OPERATOR_CREATE_RSP = 0x118000,
IPC_SIGNAL_ID_OPERATOR_DESTROY_MULTIPLE = 0x110001,
IPC_SIGNAL_ID_OPERATOR_DESTROY_MULTIPLE_RSP = 0x118001,
IPC_SIGNAL_ID_OPERATOR_START_MULTIPLE = 0x110002,
IPC_SIGNAL_ID_OPERATOR_START_MULTIPLE_RSP = 0x118002,
IPC_SIGNAL_ID_OPERATOR_STOP_MULTIPLE = 0x110003,
IPC_SIGNAL_ID_OPERATOR_STOP_MULTIPLE_RSP = 0x118003,
IPC_SIGNAL_ID_OPERATOR_RESET_MULTIPLE = 0x110004,
IPC_SIGNAL_ID_OPERATOR_RESET_MULTIPLE_RSP = 0x118004,
IPC_SIGNAL_ID_OPERATOR_MESSAGE = 0x110005,
IPC_SIGNAL_ID_OPERATOR_MESSAGE_RSP = 0x118005,
IPC_SIGNAL_ID_OPERATOR_BUNDLE_LOAD = 0x110009,
IPC_SIGNAL_ID_OPERATOR_BUNDLE_LOAD_RSP = 0x118009,
IPC_SIGNAL_ID_OPERATOR_BUNDLE_UNLOAD = 0x11000a,
IPC_SIGNAL_ID_OPERATOR_BUNDLE_UNLOAD_RSP = 0x11800a,
IPC_SIGNAL_ID_OPERATOR_FRAMEWORK_CONFIGURATION_SET = 0x11000b,
IPC_SIGNAL_ID_OPERATOR_FRAMEWORK_CONFIGURATION_SET_RSP = 0x11800b,
IPC_SIGNAL_ID_OPERATOR_FRAMEWORK_CONFIGURATION_GET = 0x11000c,
IPC_SIGNAL_ID_OPERATOR_FRAMEWORK_CONFIGURATION_GET_RSP = 0x11800c,
IPC_SIGNAL_ID_OPERATOR_FRAMEWORK_ENABLE = 0x11000d,
IPC_SIGNAL_ID_OPERATOR_FRAMEWORK_ENABLE_RSP = 0x11800d,
IPC_SIGNAL_ID_OPERATOR_FRAMEWORK_ENABLE_ASYNC = 0x110014,
IPC_SIGNAL_ID_OPERATOR_FRAMEWORK_ENABLE_ASYNC_RSP = 0x118014,
IPC_SIGNAL_ID_OPERATOR_DELEGATE_MULTIPLE = 0x11000f,
IPC_SIGNAL_ID_OPERATOR_DELEGATE_MULTIPLE_RSP = 0x11800f,
IPC_SIGNAL_ID_OPERATOR_DATA_LOAD_EX = 0x110010,
IPC_SIGNAL_ID_OPERATOR_DATA_LOAD_EX_RSP = 0x118010,
IPC_SIGNAL_ID_OPERATOR_DATA_UNLOAD_EX = 0x110011,
IPC_SIGNAL_ID_OPERATOR_DATA_UNLOAD_EX_RSP = 0x118011,
IPC_SIGNAL_ID_OPERATOR_DATA_LOAD_RA = 0x110012,
IPC_SIGNAL_ID_OPERATOR_DATA_LOAD_RA_RSP = 0x118012,
IPC_SIGNAL_ID_OPERATOR_DATA_UNLOAD_RA = 0x110013,
IPC_SIGNAL_ID_OPERATOR_DATA_UNLOAD_RA_RSP = 0x118013,
IPC_SIGNAL_ID_NFC_SEND_PRIM = 0x330000,
IPC_SIGNAL_ID_STREAM_NFC_SINK = 0x330001,
IPC_SIGNAL_ID_STREAM_NFC_SINK_RSP = 0x338001,
IPC_SIGNAL_ID_XIO_MEASURE_VOLTAGE = 0x3a000a,
IPC_SIGNAL_ID_XIO_MEASURE_VOLTAGE_RSP = 0x3a800a,
IPC_SIGNAL_ID_DAC_ENABLE = 0x3a000b,
IPC_SIGNAL_ID_DAC_ENABLE_RSP = 0x3a800b,
IPC_SIGNAL_ID_DAC_SET_GAIN = 0x3a000c,
IPC_SIGNAL_ID_DAC_SET_GAIN_RSP = 0x3a800c,
IPC_SIGNAL_ID_DAC_SET_LEVEL = 0x3a000d,
IPC_SIGNAL_ID_DAC_SET_LEVEL_RSP = 0x3a800d,
IPC_SIGNAL_ID_PIO_SET_WAKEUP_STATE_BANK = 0x1c004b,
IPC_SIGNAL_ID_PIO_SET_DEEP_SLEEP_WAKE_BANK = 0x1c004c,
IPC_SIGNAL_ID_VM_DEEP_SLEEP_WAKE_SOURCES_ENABLE = 0x1c004d,
IPC_SIGNAL_ID_VM_DEEP_SLEEP_WAKE_SOURCES_ENABLE_RSP = 0x1c804d,
IPC_SIGNAL_ID_STREAM_CSB_SINK = 0x360000,
IPC_SIGNAL_ID_STREAM_CSB_SINK_RSP = 0x368000,
IPC_SIGNAL_ID_STREAM_CSB_SOURCE = 0x360001,
IPC_SIGNAL_ID_STREAM_CSB_SOURCE_RSP = 0x368001,
IPC_SIGNAL_ID_CSB_RECEIVER_SET_AFH_MAP = 0x360002,
IPC_SIGNAL_ID_CSB_RECEIVER_SET_AFH_MAP_RSP = 0x368002,
IPC_SIGNAL_ID_CSB_TRANSMITTER_SET_AFH_MAP = 0x360003,
IPC_SIGNAL_ID_CSB_TRANSMITTER_SET_AFH_MAP_RSP = 0x368003,
IPC_SIGNAL_ID_CSB_ENABLE_NEW_AFH_MAP_EVENT = 0x360004,
IPC_SIGNAL_ID_SINK_GET_WALLCLOCK = 0x16002d,
IPC_SIGNAL_ID_SINK_GET_WALLCLOCK_RSP = 0x16802d,
IPC_SIGNAL_ID_STREAM_PIPE_PAIR = 0x160037,
IPC_SIGNAL_ID_STREAM_PIPE_PAIR_RSP = 0x168037,
IPC_SIGNAL_ID_TRANSFORM_CONVERT_CLOCK = 0x160038,
IPC_SIGNAL_ID_TRANSFORM_CONVERT_CLOCK_RSP = 0x168038,
IPC_SIGNAL_ID_TRANSFORM_HASH = 0x160039,
IPC_SIGNAL_ID_TRANSFORM_HASH_RSP = 0x168039,
IPC_SIGNAL_ID_STREAM_ISP_SOURCE = 0x16003b,
IPC_SIGNAL_ID_STREAM_ISP_SOURCE_RSP = 0x16803b,
IPC_SIGNAL_ID_STREAM_ISP_SINK = 0x16003c,
IPC_SIGNAL_ID_STREAM_ISP_SINK_RSP = 0x16803c,
IPC_SIGNAL_ID_DORMANT_CONFIGURE = 0x1c002e,
IPC_SIGNAL_ID_DORMANT_CONFIGURE_RSP = 0x1c802e,
IPC_SIGNAL_ID_VM_GET_PUBLIC_ADDRESS = 0xf000c,
IPC_SIGNAL_ID_VM_GET_PUBLIC_ADDRESS_RSP = 0xf800c,
IPC_SIGNAL_ID_VM_GET_LOCAL_IRK = 0xf000d,
IPC_SIGNAL_ID_VM_GET_LOCAL_IRK_RSP = 0xf800d,
IPC_SIGNAL_ID_VM_UPDATE_ROOT_KEYS = 0xf000e,
IPC_SIGNAL_ID_VM_UPDATE_ROOT_KEYS_RSP = 0xf800e,
IPC_SIGNAL_ID_VM_OVERRIDE_L2CAP_CONN_CONTEXT = 0xf000f,
IPC_SIGNAL_ID_VM_OVERRIDE_L2CAP_CONN_CONTEXT_RSP = 0xf800f,
IPC_SIGNAL_ID_VM_OVERRIDE_SYNC_CONN_CONTEXT = 0xf0010,
IPC_SIGNAL_ID_VM_OVERRIDE_SYNC_CONN_CONTEXT_RSP = 0xf8010,
IPC_SIGNAL_ID_VM_OVERRIDE_RFCOMM_CONN_CONTEXT = 0xf0011,
IPC_SIGNAL_ID_VM_OVERRIDE_RFCOMM_CONN_CONTEXT_RSP = 0xf8011,
IPC_SIGNAL_ID_VM_GET_ACL_MODE = 0xf0012,
IPC_SIGNAL_ID_VM_GET_ACL_MODE_RSP = 0xf8012,
IPC_SIGNAL_ID_VM_GET_ACL_ROLE = 0xf0013,
IPC_SIGNAL_ID_VM_GET_ACL_ROLE_RSP = 0xf8013,
IPC_SIGNAL_ID_AUDIO_PWM_POWER_ENABLE = 0x390000,
IPC_SIGNAL_ID_AUDIO_PWM_POWER_ENABLE_RSP = 0x398000,
IPC_SIGNAL_ID_AUDIO_PWM_MUTE = 0x390001,
IPC_SIGNAL_ID_CRYPTO_AES128_CTR = 0x380000,
IPC_SIGNAL_ID_CRYPTO_AES128_CTR_RSP = 0x388000,
IPC_SIGNAL_ID_CRYPTO_AES128_CBC = 0x380001,
IPC_SIGNAL_ID_CRYPTO_AES128_CBC_RSP = 0x388001,
IPC_SIGNAL_ID_AUDIO_MASTER_CLOCK_CONFIGURE = 0x370000,
IPC_SIGNAL_ID_AUDIO_MASTER_CLOCK_CONFIGURE_RSP = 0x378000,
IPC_SIGNAL_ID_SOURCE_MASTER_CLOCK_ENABLE = 0x370001,
IPC_SIGNAL_ID_SOURCE_MASTER_CLOCK_ENABLE_RSP = 0x378001,
IPC_SIGNAL_ID_SINK_MASTER_CLOCK_ENABLE = 0x370002,
IPC_SIGNAL_ID_SINK_MASTER_CLOCK_ENABLE_RSP = 0x378002,
IPC_SIGNAL_ID_GENERATE_MASTER_CLOCK_OUTPUT = 0x370003,
IPC_SIGNAL_ID_GENERATE_MASTER_CLOCK_OUTPUT_RSP = 0x378003,
IPC_SIGNAL_ID_AUDIO_ANC_STREAM_ENABLE = 0x3c0000,
IPC_SIGNAL_ID_AUDIO_ANC_STREAM_ENABLE_RSP = 0x3c8000,
IPC_SIGNAL_ID_AUDIO_ANC_FILTER_LPF_SET = 0x3c0001,
IPC_SIGNAL_ID_AUDIO_ANC_FILTER_LPF_SET_RSP = 0x3c8001,
IPC_SIGNAL_ID_AUDIO_ANC_FILTER_IIR_SET = 0x3c0002,
IPC_SIGNAL_ID_AUDIO_ANC_FILTER_IIR_SET_RSP = 0x3c8002,
IPC_SIGNAL_ID_AUDIO_ANC_CONFIG = 0x3c0003,
IPC_SIGNAL_ID_AUDIO_ANC_CONFIG_RSP = 0x3c8003,
IPC_SIGNAL_ID_VM_REQUEST_RUN_TIME_PROFILE = 0x3e0000,
IPC_SIGNAL_ID_VM_REQUEST_RUN_TIME_PROFILE_RSP = 0x3e8000,
IPC_SIGNAL_ID_VM_GET_RUN_TIME_PROFILE = 0x3e0001,
IPC_SIGNAL_ID_VM_GET_RUN_TIME_PROFILE_RSP = 0x3e8001,
IPC_SIGNAL_ID_OPERATOR_FRAMEWORK_TRIGGER_NOTIFICATION_START = 0x3d0000,
IPC_SIGNAL_ID_OPERATOR_FRAMEWORK_TRIGGER_NOTIFICATION_START_RSP = 0x3d8000,
IPC_SIGNAL_ID_OPERATOR_FRAMEWORK_TRIGGER_NOTIFICATION_STOP = 0x3d0001,
IPC_SIGNAL_ID_OPERATOR_FRAMEWORK_TRIGGER_NOTIFICATION_STOP_RSP = 0x3d8001,
IPC_SIGNAL_ID_OPERATOR_DATA_LOAD = 0x3d0002,
IPC_SIGNAL_ID_OPERATOR_DATA_LOAD_RSP = 0x3d8002,
IPC_SIGNAL_ID_OPERATOR_DATA_UNLOAD = 0x3d0003,
IPC_SIGNAL_ID_OPERATOR_DATA_UNLOAD_RSP = 0x3d8003,
IPC_SIGNAL_ID_AUDIO_DSP_CLOCK_CONFIGURE = 0x3d0004,
IPC_SIGNAL_ID_AUDIO_DSP_CLOCK_CONFIGURE_RSP = 0x3d8004,
IPC_SIGNAL_ID_AUDIO_DSP_GET_CLOCK = 0x3d0005,
IPC_SIGNAL_ID_AUDIO_DSP_GET_CLOCK_RSP = 0x3d8005,
IPC_SIGNAL_ID_AUDIO_POWER_SAVE_MODE_SET = 0x3d0006,
IPC_SIGNAL_ID_AUDIO_POWER_SAVE_MODE_SET_RSP = 0x3d8006,
IPC_SIGNAL_ID_AUDIO_POWER_SAVE_MODE_GET = 0x3d0007,
IPC_SIGNAL_ID_AUDIO_POWER_SAVE_MODE_GET_RSP = 0x3d8007,
IPC_SIGNAL_ID_OPERATOR_FRAMEWORK_PRESERVE = 0x3d0008,
IPC_SIGNAL_ID_OPERATOR_FRAMEWORK_PRESERVE_RSP = 0x3d8008,
IPC_SIGNAL_ID_OPERATOR_FRAMEWORK_RELEASE = 0x3d0009,
IPC_SIGNAL_ID_OPERATOR_FRAMEWORK_RELEASE_RSP = 0x3d8009,
IPC_SIGNAL_ID_STREAM_ATT_SINK = 0x180007,
IPC_SIGNAL_ID_STREAM_ATT_SINK_RSP = 0x188007,
IPC_SIGNAL_ID_STREAM_ATT_SERVER_SOURCE = 0x180008,
IPC_SIGNAL_ID_STREAM_ATT_SERVER_SOURCE_RSP = 0x188008,
IPC_SIGNAL_ID_STREAM_ATT_CLIENT_SOURCE = 0x180009,
IPC_SIGNAL_ID_STREAM_ATT_CLIENT_SOURCE_RSP = 0x188009,
IPC_SIGNAL_ID_STREAM_ATT_ADD_HANDLE = 0x18000a,
IPC_SIGNAL_ID_STREAM_ATT_ADD_HANDLE_RSP = 0x18800a,
IPC_SIGNAL_ID_STREAM_ATT_REMOVE_ALL_HANDLES = 0x18000b,
IPC_SIGNAL_ID_STREAM_ATT_REMOVE_ALL_HANDLES_RSP = 0x18800b,
IPC_SIGNAL_ID_STREAM_ATT_CLIENT_SINK = 0x18000c,
IPC_SIGNAL_ID_STREAM_ATT_CLIENT_SINK_RSP = 0x18800c,
IPC_SIGNAL_ID_STREAM_ATT_SERVER_SINK = 0x18000d,
IPC_SIGNAL_ID_STREAM_ATT_SERVER_SINK_RSP = 0x18800d,
IPC_SIGNAL_ID_USB_DEBUG_ALLOW = 0x25000e,
IPC_SIGNAL_ID_USB_DEBUG_ALLOW_RSP = 0x25800e,
IPC_SIGNAL_ID_USB_CONNECT = 0x25000f,
IPC_SIGNAL_ID_USB_CONNECT_RSP = 0x25800f,
IPC_SIGNAL_ID_USB_DESCRIPTORS_HASH = 0x250010,
IPC_SIGNAL_ID_USB_DESCRIPTORS_HASH_RSP = 0x258010,
IPC_SIGNAL_ID_ACL_RECEIVE_ENABLE = 0x420000,
IPC_SIGNAL_ID_ACL_RECEIVE_ENABLE_RSP = 0x428000,
IPC_SIGNAL_ID_ACL_RECEIVED_DATA_PROCESSED = 0x420001,
IPC_SIGNAL_ID_ACL_RECEIVED_DATA_PROCESSED_RSP = 0x428001,
IPC_SIGNAL_ID_ACL_TRANSMIT_DATA_PENDING = 0x420002,
IPC_SIGNAL_ID_ACL_TRANSMIT_DATA_PENDING_RSP = 0x428002,
IPC_SIGNAL_ID_VM_OVERRIDE_BDADDR = 0x3f0000,
IPC_SIGNAL_ID_VM_OVERRIDE_BDADDR_RSP = 0x3f8000,
IPC_SIGNAL_ID_ACL_HANDOVER_PREPARE = 0x430002,
IPC_SIGNAL_ID_ACL_HANDOVER_PREPARE_RSP = 0x438002,
IPC_SIGNAL_ID_ACL_HANDOVER_PREPARED = 0x430003,
IPC_SIGNAL_ID_ACL_HANDOVER_PREPARED_RSP = 0x438003,
IPC_SIGNAL_ID_STREAM_ACL_MARSHAL_SOURCE = 0x430004,
IPC_SIGNAL_ID_STREAM_ACL_MARSHAL_SOURCE_RSP = 0x438004,
IPC_SIGNAL_ID_STREAM_ACL_MARSHAL_SINK = 0x430005,
IPC_SIGNAL_ID_STREAM_ACL_MARSHAL_SINK_RSP = 0x438005,
IPC_SIGNAL_ID_ACL_HANDOVER_COMMIT = 0x430006,
IPC_SIGNAL_ID_ACL_HANDOVER_COMMIT_RSP = 0x438006,
IPC_SIGNAL_ID_ACL_HANDOVER_CANCEL = 0x430007,
IPC_SIGNAL_ID_ACL_HANDOVER_CANCEL_RSP = 0x438007,
IPC_SIGNAL_ID_STREAM_AUDIO_SYNC_SOURCE = 0x430008,
IPC_SIGNAL_ID_STREAM_AUDIO_SYNC_SOURCE_RSP = 0x438008,
IPC_SIGNAL_ID_AUDIO_MAP_CPU_SPEED = 0x3d000a,
IPC_SIGNAL_ID_AUDIO_MAP_CPU_SPEED_RSP = 0x3d800a,
IPC_SIGNAL_ID_RA_PARTITION_OPEN = 0x410000,
IPC_SIGNAL_ID_RA_PARTITION_OPEN_RSP = 0x418000,
IPC_SIGNAL_ID_RA_PARTITION_CLOSE = 0x410001,
IPC_SIGNAL_ID_RA_PARTITION_CLOSE_RSP = 0x418001,
IPC_SIGNAL_ID_RA_PARTITION_ERASE = 0x410002,
IPC_SIGNAL_ID_RA_PARTITION_ERASE_RSP = 0x418002,
IPC_SIGNAL_ID_RA_PARTITION_WRITE = 0x410003,
IPC_SIGNAL_ID_RA_PARTITION_WRITE_RSP = 0x418003,
IPC_SIGNAL_ID_RA_PARTITION_READ = 0x410004,
IPC_SIGNAL_ID_RA_PARTITION_READ_RSP = 0x418004,
IPC_SIGNAL_ID_RA_PARTITION_BG_ERASE = 0x410005,
IPC_SIGNAL_ID_RA_PARTITION_NAMED_OPEN = 0x410006,
IPC_SIGNAL_ID_RA_PARTITION_NAMED_OPEN_RSP = 0x418006,
IPC_SIGNAL_ID_DEBUG_PARTITION_CONFIG = 0x460000,
IPC_SIGNAL_ID_DEBUG_PARTITION_CONFIG_RSP = 0x468000,
IPC_SIGNAL_ID_DEBUG_PARTITION_INFO = 0x460001,
IPC_SIGNAL_ID_DEBUG_PARTITION_INFO_RSP = 0x468001,
IPC_SIGNAL_ID_DEBUG_PARTITION_ERASE = 0x460002,
IPC_SIGNAL_ID_DEBUG_PARTITION_ERASE_RSP = 0x468002,
IPC_SIGNAL_ID_STREAM_DEBUG_PARTITION_SOURCE = 0x460003,
IPC_SIGNAL_ID_STREAM_DEBUG_PARTITION_SOURCE_RSP = 0x468003,
IPC_SIGNAL_ID_TEST2_CW_TRANSMIT = 0x440000,
IPC_SIGNAL_ID_TEST2_CW_TRANSMIT_RSP = 0x448000,
IPC_SIGNAL_ID_TEST2_RF_STOP = 0x440001,
IPC_SIGNAL_ID_TEST2_RF_STOP_RSP = 0x448001,
IPC_SIGNAL_ID_TEST2_TX_DATA = 0x440002,
IPC_SIGNAL_ID_TEST2_TX_DATA_RSP = 0x448002,
IPC_SIGNAL_ID_TEST2_RX_START = 0x440003,
IPC_SIGNAL_ID_TEST2_RX_START_RSP = 0x448003,
IPC_SIGNAL_ID_VM_GET_B_T_RADIO_CALIBRATION_QUALITY = 0x440015,
IPC_SIGNAL_ID_VM_GET_B_T_RADIO_CALIBRATION_QUALITY_RSP = 0x448015,
IPC_SIGNAL_ID_TEST2_TX_ENHANCED = 0x440005,
IPC_SIGNAL_ID_TEST2_TX_ENHANCED_RSP = 0x448005,
IPC_SIGNAL_ID_TEST2_RX_ENHANCED = 0x440006,
IPC_SIGNAL_ID_TEST2_RX_ENHANCED_RSP = 0x448006,
IPC_SIGNAL_ID_TEST2_RX_STAT_ENHANCED = 0x440007,
IPC_SIGNAL_ID_TEST2_RX_STAT_ENHANCED_RSP = 0x448007,
IPC_SIGNAL_ID_PS_NEXT_KEY = 0x1c0056,
IPC_SIGNAL_ID_PS_NEXT_KEY_RSP = 0x1c8056,
IPC_SIGNAL_ID_PS_NEXT_AUDIO_KEY = 0x1c0057,
IPC_SIGNAL_ID_PS_NEXT_AUDIO_KEY_RSP = 0x1c8057,
IPC_SIGNAL_ID_PS_DEFRAG_BLOCKING = 0x1c0058,
IPC_SIGNAL_ID_PS_DEFRAG_BLOCKING_RSP = 0x1c8058,
IPC_SIGNAL_ID_VM_ADVERTISE_POWER_SET_MAXIMUM = 0x1c005b,
IPC_SIGNAL_ID_VM_ADVERTISE_POWER_SET_MAXIMUM_RSP = 0x1c805b,
IPC_SIGNAL_ID_QSPI_IDENTITY_REQUEST = 0x450000,
IPC_SIGNAL_ID_QSPI_IDENTITY_REQUEST_RSP = 0x458000,
IPC_SIGNAL_ID_QSPI_U_I_D_REQUEST = 0x450001,
IPC_SIGNAL_ID_QSPI_U_I_D_REQUEST_RSP = 0x458001,
IPC_SIGNAL_ID_QSPI_O_T_P_REQUEST = 0x450002,
IPC_SIGNAL_ID_QSPI_O_T_P_REQUEST_RSP = 0x458002,
IPC_SIGNAL_ID_VM_BD_ADDR_GET_RSSI = 0xf0014,
IPC_SIGNAL_ID_VM_BD_ADDR_GET_RSSI_RSP = 0xf8014,
IPC_SIGNAL_ID_VM_CIS_CONN_GET_RSSI = 0xf0017,
IPC_SIGNAL_ID_VM_CIS_CONN_GET_RSSI_RSP = 0xf8017,
IPC_SIGNAL_ID_CHARGER_COMMS_TRANSMIT = 0x480001,
IPC_SIGNAL_ID_CHARGER_COMMS_TRANSMIT_RSP = 0x488001,
IPC_SIGNAL_ID_CHARGER_COMMS_UART_RESET_SEQ_NUM = 0x480004,
IPC_SIGNAL_ID_CHARGER_COMMS_UART_RESET_SEQ_NUM_RSP = 0x488004,
IPC_SIGNAL_ID_CHARGER_COMMS_UART_PRIORITY_TRANSMIT = 0x480005,
IPC_SIGNAL_ID_CHARGER_COMMS_UART_PRIORITY_TRANSMIT_RSP = 0x488005,
IPC_SIGNAL_ID_ACL_RELIABLE_MIRROR_DEBUG_STATISTICS = 0x430009,
IPC_SIGNAL_ID_ACL_RELIABLE_MIRROR_DEBUG_STATISTICS_RSP = 0x438009,
IPC_SIGNAL_ID_SINK_L2CAP_FILTER_PACKETS = 0x43000a,
IPC_SIGNAL_ID_SINK_L2CAP_FILTER_PACKETS_RSP = 0x43800a,
IPC_SIGNAL_ID_VM_GET_SCO_STATISTICS = 0xf0015,
IPC_SIGNAL_ID_VM_GET_SCO_STATISTICS_RSP = 0xf8015,
IPC_SIGNAL_ID_VM_GET_ACL_LINK_QUALITY = 0xf0016,
IPC_SIGNAL_ID_VM_GET_ACL_LINK_QUALITY_RSP = 0xf8016,
IPC_SIGNAL_ID_STREAM_ACL_ESTABLISH_SINK = 0x43000b,
IPC_SIGNAL_ID_STREAM_ACL_ESTABLISH_SINK_RSP = 0x43800b,
IPC_SIGNAL_ID_ACL_ESTABLISH_COMMIT = 0x43000c,
IPC_SIGNAL_ID_ACL_ESTABLISH_COMMIT_RSP = 0x43800c,
IPC_SIGNAL_ID_STREAM_EXT_SCAN_SOURCE = 0x4b0000,
IPC_SIGNAL_ID_STREAM_EXT_SCAN_SOURCE_RSP = 0x4b8000,
IPC_SIGNAL_ID_STREAM_PERIODIC_SCAN_SOURCE = 0x4b0001,
IPC_SIGNAL_ID_STREAM_PERIODIC_SCAN_SOURCE_RSP = 0x4b8001,
IPC_SIGNAL_ID_STREAM_ISO_SOURCE = 0x4a0000,
IPC_SIGNAL_ID_STREAM_ISO_SOURCE_RSP = 0x4a8000,
IPC_SIGNAL_ID_STREAM_ISO_SINK = 0x4a0001,
IPC_SIGNAL_ID_STREAM_ISO_SINK_RSP = 0x4a8001,
IPC_SIGNAL_ID_ACL_HANDOVER_ROLE_COMMIT = 0x43000d,
IPC_SIGNAL_ID_ACL_HANDOVER_ROLE_COMMIT_RSP = 0x43800d,
IPC_SIGNAL_ID_FEATURE_CHALLENGE_RESPONSE = 0x1c0065,
IPC_SIGNAL_ID_FEATURE_CHALLENGE_RESPONSE_RSP = 0x1c8065,
IPC_SIGNAL_ID_VM_READ_NEXT_SNIFF_CLOCK = 0xf0018,
IPC_SIGNAL_ID_VM_READ_NEXT_SNIFF_CLOCK_RSP = 0xf8018,
IPC_SIGNAL_ID_SINK_ENABLE_LINK_STATUS_TRACKING = 0xf001a,
IPC_SIGNAL_ID_SINK_ENABLE_LINK_STATUS_TRACKING_RSP = 0xf801a,
IPC_SIGNAL_ID_DEBUG_PARTITION_CONFIG32 = 0x460004,
IPC_SIGNAL_ID_DEBUG_PARTITION_CONFIG32_RSP = 0x468004,
IPC_SIGNAL_ID_XTAL_CLOCK_OUT_ENABLE = 0x4d0000,
IPC_SIGNAL_ID_XTAL_CLOCK_OUT_ENABLE_RSP = 0x4d8000,
IPC_SIGNAL_ID__SENTINEL__
