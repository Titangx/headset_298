# Copyright (c) 2018-2020 Qualcomm Technologies International, Ltd.
#   %%version
BUILDOUTPUT_PATH?=..
ifeq ($(UBUILD_PYTHON_VER),3)
	PYTHON=python
else
	ifeq ($(DEVKIT_ROOT),)
		PYTHON=python
	else
		PYTHON=$(DEVKIT_ROOT)/tools/python27/python.exe
	endif
endif
KCC_DIR?=$(DEVKIT_ROOT)/tools/kcc
CC=$(KCC_DIR)/bin/kcc
AR=$(KCC_DIR)/bin/kar
ifeq ($(OS),Windows_NT)
	RMDIR=rmdir /S /Q
	FIXPATH=$(subst /,\,$1)
	MKDIR=$(DEVKIT_ROOT)/tools/bin/createpath $1
else
	RMDIR=rm -rf
	FIXPATH=$1
	MKDIR=mkdir -p $1
endif
MAKE_DIR=$(call MKDIR,${@D})
C_SOURCE=core/appcmd/appcmd.c core/bitserial/bitserial_config.c core/bitserial/bitserial_transfer.c core/bitserial/bitserial_trap_api.c core/buffer/buf_init_handle.c core/buffer/buf_raw_read_map_16bit.c core/buffer/buf_raw_read_map_16bit_be.c core/buffer/buf_raw_read_map_16bit_save_state.c core/buffer/buf_raw_read_map_8bit.c core/buffer/buf_raw_read_map_8bit_save_state.c core/buffer/buf_raw_read_unmap.c core/buffer/buf_raw_read_update_restore_state.c core/buffer/buf_raw_read_write_map_16bit.c core/buffer/buf_raw_read_write_map_16bit_be.c core/buffer/buf_raw_read_write_map_16bit_save_state.c core/buffer/buf_raw_read_write_map_8bit.c core/buffer/buf_raw_read_write_map_8bit_save_state.c core/buffer/buf_raw_read_write_unmap.c core/buffer/buf_raw_update_tail_free.c core/buffer/buf_raw_write_map_16bit.c core/buffer/buf_raw_write_map_16bit_be.c core/buffer/buf_raw_write_map_16bit_save_state.c core/buffer/buf_raw_write_map_8bit.c core/buffer/buf_raw_write_map_8bit_save_state.c core/buffer/buf_raw_write_only_map_16bit.c core/buffer/buf_raw_write_only_map_16bit_be.c core/buffer/buf_raw_write_only_map_8bit.c core/buffer/buf_raw_write_only_map_8bit_save_state.c core/buffer/buf_raw_write_only_unmap.c core/buffer/buf_raw_write_unmap.c core/buffer/buf_raw_write_update_restore_state.c core/buffer/buffer_msg.c core/buffer/buffer_msg_ptr_access.c core/bus_interrupt/bus_interrupt.c core/cache/cache.c core/dorm/dorm_config.c core/dorm/dorm_get_kip_flags.c core/dorm/dorm_kalimba.c core/excep/excep.c core/excep/excep_test.c core/fault/fault.c core/fault/fault_appcmd.c core/fault/fault_comms.c core/fault/fault_db.c core/hal/hal_bitserial.c core/hal/hal_data_conv.c core/hal/hal_data_conv_access.c core/hal/hal_data_conv_cal.c core/hal/hal_delay_us.c core/hydra_log/hydra_log_firm.c core/hydra_log/hydra_log_soft.c core/id/id.c core/int/configure_interrupt.c core/int/configure_sw_interrupt.c core/int/configure_sw_interrupt_raw.c core/int/generate_sw_interrupt.c core/int/init_int.c core/int/swint_demux.c core/int/unconfigure_sw_interrupt.c core/ipc/ipc_bluestack.c core/ipc/ipc_deep_sleep.c core/ipc/ipc_fault_panic.c core/ipc/ipc_init.c core/ipc/ipc_malloc.c core/ipc/ipc_memory_access.c core/ipc/ipc_mmu.c core/ipc/ipc_pio.c core/ipc/ipc_recv.c core/ipc/ipc_recv_common.c core/ipc/ipc_sched.c core/ipc/ipc_sd_mmc.c core/ipc/ipc_send.c core/ipc/ipc_send_common.c core/ipc/ipc_stream.c core/ipc/ipc_test.c core/ipc/ipc_test_traps.c core/ipc/ipc_test_tunnel.c core/ipc/ipc_trap_api.c core/ipc/ipc_uart.c core/ipc/ipc_vm.c core/itime_kal/itime_kal.c core/kal_utils/kal_utils.c core/ledctrl/ledctrl.c core/longtimer/get_deci_time.c core/longtimer/get_milli_time.c core/longtimer/get_second_time.c core/longtimer/longtimer.c core/marshal/marshal.c core/marshal/marshal_base.c core/marshal/marshal_object_set.c core/marshal/unmarshal.c core/memprot/memprot.c core/optim/mempack.c core/panic/panic.c core/panic/panic_comms.c core/panic/panic_on_assert.c core/pio/init_pio.c core/pio/pio_get_levels_mask.c core/pio/pio_set_directions_mask.c core/pio/pio_set_internal_owners_mask.c core/pio/pio_set_levels_mask.c core/piodebounce/piodebounce.c core/pioint/pioint_configure.c core/pioint/pioint_init.c core/pl_timers/pl_timers.c core/pmalloc/init_pmalloc.c core/pmalloc/pcopy.c core/pmalloc/pfree.c core/pmalloc/pfree_set_free_list_ptr.c core/pmalloc/pmalloc.c core/pmalloc/pmalloc_available.c core/pmalloc/pmalloc_config.c core/pmalloc/pmalloc_debug_check_block.c core/pmalloc/pmalloc_debug_validate_free_list.c core/pmalloc/pmalloc_debug_validate_pool_control.c core/pmalloc/pmalloc_set_monitor_limits.c core/pmalloc/pmalloc_unit.c core/pmalloc/prealloc.c core/pmalloc/prightsize.c core/pmalloc/psizeof.c core/pmalloc/xpcopy.c core/pmalloc/xpmalloc.c core/pmalloc/xpmalloc_buffer.c core/pmalloc/xprealloc.c core/pmalloc/xzpmalloc.c core/pmalloc/zpmalloc.c core/sched_oxygen/sched_oxygen.c core/sched_oxygen/sched_oxygen_cancel.c core/slt/slt_entry.c core/utils/utils_sll.c customer/core/init/init.c customer/core/trap_api/trap_api_acl.c customer/core/trap_api/trap_api_audio.c customer/core/trap_api/trap_api_bdaddr.c customer/core/trap_api/trap_api_bitserial.c customer/core/trap_api/trap_api_bluestack.c customer/core/trap_api/trap_api_capacitive_sensor.c customer/core/trap_api/trap_api_charger.c customer/core/trap_api/trap_api_chargercomms.c customer/core/trap_api/trap_api_core.c customer/core/trap_api/trap_api_core_pio.c customer/core/trap_api/trap_api_core_util.c customer/core/trap_api/trap_api_csb.c customer/core/trap_api/trap_api_extra.c customer/core/trap_api/trap_api_file.c customer/core/trap_api/trap_api_led.c customer/core/trap_api/trap_api_link_status.c customer/core/trap_api/trap_api_marshal.c customer/core/trap_api/trap_api_message.c customer/core/trap_api/trap_api_message_log.c customer/core/trap_api/trap_api_operator.c customer/core/trap_api/trap_api_psu.c customer/core/trap_api/trap_api_sd_mmc.c customer/core/trap_api/trap_api_sensor.c customer/core/trap_api/trap_api_stream.c customer/core/trap_api/trap_api_test2.c customer/core/trap_api/trap_api_test_support.c customer/core/trap_api/trap_api_uart.c gen/core/trap_version/trap_version_supported.c gen/customer/core/trap_api/gen/trap_api_ipc_glue.c
ASM_SOURCE=core/appcmd/appcmd_call_function.asm core/crt/crt0.asm core/crt/crt0_rst_maxim.asm core/int/interrupt.asm core/int/interrupt_inc.asm core/io/auraplus/d00/io/io_defs.asm core/io/auraplus/d00/io/io_map.asm core/io/io_defs.asm core/kal_utils/kal_utils_asm.asm core/optim/udiv3216_opt.asm core/optim/uint64_divmod31_opt.asm core/pmalloc/pmalloc_trace_pc.asm core/slt/slt_header.asm
CHIP_TYPE=qcc514x_qcc304x
INSTALL_DIR=$(BUILDOUTPUT_PATH)/../installed_libs
LIB_DIR=$(INSTALL_DIR)/lib/os/$(CHIP_TYPE)
ADK_INTERFACE=$(INSTALL_DIR)/include
ADK_INTERFACE_FW=$(ADK_INTERFACE)/firmware_$(CHIP_TYPE)
ADK_INTERFACE_STD=$(ADK_INTERFACE)/standard
TRAPSETS_HEADER=$(ADK_INTERFACE_FW)/trapsets.h
MAKE_TOOLS_DIR=../tools/make
BUILD_DIR=$(BUILDOUTPUT_PATH)/build/auraplus_p1_d00_lib
INCPATHS=core/include_fw ../../common/interface ../../common/interface/app bt/qbluestack/adapter bt/qbluestack/port customer/core customer/apps customer/libs gen/customer/core customer/core customer/apps customer/libs gen/customer/core core core/include gen/core bt customer/core/trap_api gen/customer/core/trap_api customer/core gen/customer/core customer/core/trap_api gen/customer/core/trap_api customer/core gen/customer/core .
INCLUDE_PATHS=$(foreach inc,$(INCPATHS),-I$(inc))
ASM_INCLUDE_PATHS=$(foreach inc,$(INCPATHS),-A$(inc))
PREINCPATHS=gen/build_defs.h
PREINCLUDES=$(foreach pre,$(PREINCPATHS),-preinclude $(pre))
ASM_PREINCLUDES=$(foreach pre,$(PREINCPATHS),-Xa -include -Xa $(pre))
C_OBJECTS=$(patsubst %.c, $(BUILD_DIR)/%.o,$(C_SOURCE))
ASM_OBJECTS=$(patsubst %.asm, $(BUILD_DIR)/%.o,$(ASM_SOURCE))
OBJECTS:=$(C_OBJECTS) $(ASM_OBJECTS)
CHIP_NAME_OPT=-kqcc514x_apps
WARNING_OPTS=-Wall -WAccuracyLoss -WnoPointerUnaligned -WnoConstantTest -WnoDeclarationMadeGlobal -WnoExplicitQualifier
CFLAGS=-g -minim $(CHIP_NAME_OPT) -Xc -o2 -Xa -apply-b179745-workaround $(WARNING_OPTS)
CFLAGS+=$(PREINCLUDES) $(INCLUDE_PATHS)
CFLAGS+=$(ASM_PREINCLUDES) $(ASM_INCLUDE_PATHS)
C_DEPGEN_FLAGS=-MD -MF ${<:%.c=$(BUILD_DIR)/%.d} -MT ${<:%.c=$(BUILD_DIR)/%.o}
ASM_DEPGEN_FLAGS=-Xa -MD -Xa -MF -Xa ${<:%.asm=$(BUILD_DIR)/%.d} -Xa -MT -Xa ${<:%.asm=$(BUILD_DIR)/%.o}
DEPFILES=$(OBJECTS:.o=.d) $(BUILD_DIR)/customer/core/init/init_usb_early_init.d
all: $(ADK_INTERFACE) $(LIB_DIR)/lib$(CHIP_TYPE).a $(LIB_DIR)/libusb_early_init.a
$(ADK_INTERFACE): $(TRAPSETS_HEADER) $(ADK_INTERFACE_FW)/app/acl/acl_if.h $(ADK_INTERFACE_FW)/app/adc/adc_if.h $(ADK_INTERFACE_FW)/app/audio/audio_if.h $(ADK_INTERFACE_FW)/app/bitserial/bitserial_if.h $(ADK_INTERFACE_FW)/app/bluestack/att_prim.h $(ADK_INTERFACE_FW)/app/bluestack/bluetooth.h $(ADK_INTERFACE_FW)/app/bluestack/dm_prim.h $(ADK_INTERFACE_FW)/app/bluestack/hci.h $(ADK_INTERFACE_FW)/app/bluestack/l2cap_prim.h $(ADK_INTERFACE_FW)/app/bluestack/mdm_prim.h $(ADK_INTERFACE_FW)/app/bluestack/rfcomm_prim.h $(ADK_INTERFACE_FW)/app/bluestack/sdc_prim.h $(ADK_INTERFACE_FW)/app/bluestack/sds_prim.h $(ADK_INTERFACE_FW)/app/bluestack/types.h $(ADK_INTERFACE_FW)/app/bluestack/vendor_specific_hci.h $(ADK_INTERFACE_FW)/app/bluestack/vsdm_prim.h $(ADK_INTERFACE_FW)/app/capacitive_sensor/capacitive_sensor_if.h $(ADK_INTERFACE_FW)/app/charger/charger_if.h $(ADK_INTERFACE_FW)/app/charger_comms/charger_comms_if.h $(ADK_INTERFACE_FW)/app/debug_partition/debug_partition_data_if.h $(ADK_INTERFACE_FW)/app/debug_partition/debug_partition_if.h $(ADK_INTERFACE_FW)/app/dormant/dormant_if.h $(ADK_INTERFACE_FW)/app/feature/feature_if.h $(ADK_INTERFACE_FW)/app/file/file_if.h $(ADK_INTERFACE_FW)/app/flash_ops/flash_ops_if.h $(ADK_INTERFACE_FW)/app/image_upgrade/image_upgrade_if.h $(ADK_INTERFACE_FW)/app/infrared/infrared_if.h $(ADK_INTERFACE_FW)/app/lcd/lcd_if.h $(ADK_INTERFACE_FW)/app/led/led_if.h $(ADK_INTERFACE_FW)/app/marshal/marshal_if.h $(ADK_INTERFACE_FW)/app/message/link_status_if.h $(ADK_INTERFACE_FW)/app/message/subsystem_if.h $(ADK_INTERFACE_FW)/app/message/system_message.h $(ADK_INTERFACE_FW)/app/mic_bias/mic_bias_if.h $(ADK_INTERFACE_FW)/app/operator/operator_if.h $(ADK_INTERFACE_FW)/app/partition/partition_if.h $(ADK_INTERFACE_FW)/app/pio/pio_if.h $(ADK_INTERFACE_FW)/app/ps/ps_if.h $(ADK_INTERFACE_FW)/app/psu/psu_if.h $(ADK_INTERFACE_FW)/app/ra_partition/ra_partition_if.h $(ADK_INTERFACE_FW)/app/ringtone/ringtone_if.h $(ADK_INTERFACE_FW)/app/ringtone/ringtone_notes.h $(ADK_INTERFACE_FW)/app/sd_mmc/sd_mmc_if.h $(ADK_INTERFACE_FW)/app/sensor/sensor_if.h $(ADK_INTERFACE_FW)/app/status/status_if.h $(ADK_INTERFACE_FW)/app/stream/stream_if.h $(ADK_INTERFACE_FW)/app/uart/uart_if.h $(ADK_INTERFACE_FW)/app/usb/usb_common_if.h $(ADK_INTERFACE_FW)/app/usb/usb_hub_if.h $(ADK_INTERFACE_FW)/app/usb/usb_if.h $(ADK_INTERFACE_FW)/app/vm/vm_if.h $(ADK_INTERFACE_FW)/app/voltsense/voltsense_if.h $(ADK_INTERFACE_FW)/app/bluestack/qbl_adapter_types.h $(ADK_INTERFACE_FW)/app/bluestack/qbl_types.h $(ADK_INTERFACE_FW)/hydra_macros.h $(ADK_INTERFACE_STD)/hydra_types.h $(ADK_INTERFACE_FW)/hydra_log.h $(ADK_INTERFACE_FW)/hydra_log_disabled.h $(ADK_INTERFACE_FW)/hydra_log_firm.h $(ADK_INTERFACE_FW)/hydra_log_firm_modules.h $(ADK_INTERFACE_FW)/hydra_log_soft.h $(ADK_INTERFACE_STD)/macros.h $(ADK_INTERFACE_STD)/types.h $(ADK_INTERFACE_FW)/pmalloc.h $(ADK_INTERFACE_FW)/pmalloc_trace.h $(ADK_INTERFACE_STD)/csrtypes.h $(ADK_INTERFACE_FW)/hydra_log_subsystems.h $(ADK_INTERFACE_FW)/acl.h $(ADK_INTERFACE_FW)/adc.h $(ADK_INTERFACE_FW)/api.h $(ADK_INTERFACE_FW)/audio_anc.h $(ADK_INTERFACE_FW)/audio_clock.h $(ADK_INTERFACE_FW)/audio_mclk.h $(ADK_INTERFACE_FW)/audio_power.h $(ADK_INTERFACE_FW)/audio_pwm.h $(ADK_INTERFACE_FW)/bdaddr_.h $(ADK_INTERFACE_FW)/bitserial_api.h $(ADK_INTERFACE_FW)/boot.h $(ADK_INTERFACE_FW)/capacitivesensor.h $(ADK_INTERFACE_FW)/charger.h $(ADK_INTERFACE_FW)/chargercomms.h $(ADK_INTERFACE_FW)/clock_out.h $(ADK_INTERFACE_FW)/codec_.h $(ADK_INTERFACE_FW)/crypto.h $(ADK_INTERFACE_FW)/csb.h $(ADK_INTERFACE_FW)/csb_.h $(ADK_INTERFACE_FW)/debug_partition_api.h $(ADK_INTERFACE_FW)/dormant.h $(ADK_INTERFACE_FW)/energy.h $(ADK_INTERFACE_FW)/feature.h $(ADK_INTERFACE_FW)/file.h $(ADK_INTERFACE_FW)/font.h $(ADK_INTERFACE_FW)/host.h $(ADK_INTERFACE_FW)/i2c.h $(ADK_INTERFACE_FW)/imageupgrade.h $(ADK_INTERFACE_FW)/infrared.h $(ADK_INTERFACE_FW)/inquiry.h $(ADK_INTERFACE_FW)/kalimba.h $(ADK_INTERFACE_FW)/lcd.h $(ADK_INTERFACE_FW)/led.h $(ADK_INTERFACE_FW)/loader.h $(ADK_INTERFACE_FW)/marshal.h $(ADK_INTERFACE_FW)/message.h $(ADK_INTERFACE_FW)/message_.h $(ADK_INTERFACE_FW)/micbias.h $(ADK_INTERFACE_FW)/native.h $(ADK_INTERFACE_FW)/nfc.h $(ADK_INTERFACE_FW)/operator.h $(ADK_INTERFACE_FW)/operator_.h $(ADK_INTERFACE_FW)/os.h $(ADK_INTERFACE_FW)/otp.h $(ADK_INTERFACE_FW)/panic.h $(ADK_INTERFACE_FW)/partition.h $(ADK_INTERFACE_FW)/pio.h $(ADK_INTERFACE_FW)/ps.h $(ADK_INTERFACE_FW)/psu.h $(ADK_INTERFACE_FW)/qspi.h $(ADK_INTERFACE_FW)/ra_partition_api.h $(ADK_INTERFACE_FW)/sdmmc.h $(ADK_INTERFACE_FW)/sensor.h $(ADK_INTERFACE_FW)/sink.h $(ADK_INTERFACE_FW)/sink_.h $(ADK_INTERFACE_FW)/source.h $(ADK_INTERFACE_FW)/source_.h $(ADK_INTERFACE_FW)/sram.h $(ADK_INTERFACE_FW)/status.h $(ADK_INTERFACE_FW)/stream.h $(ADK_INTERFACE_FW)/test.h $(ADK_INTERFACE_FW)/test2.h $(ADK_INTERFACE_FW)/test2_.h $(ADK_INTERFACE_FW)/transform.h $(ADK_INTERFACE_FW)/transform_.h $(ADK_INTERFACE_FW)/usb.h $(ADK_INTERFACE_FW)/usb_hub.h $(ADK_INTERFACE_FW)/util.h $(ADK_INTERFACE_FW)/vm.h $(ADK_INTERFACE_FW)/vm_.h $(ADK_INTERFACE_FW)/voltsense.h
$(TRAPSETS_HEADER): gen/build_defs.h
ifeq ($(OS),Windows_NT)
	set PYTHONPATH=$(PYTHONPATH);$(MAKE_TOOLS_DIR) && $(PYTHON) $(MAKE_TOOLS_DIR)/create_trapsets_header.py $< $@
else
	$(PYTHON) $(MAKE_TOOLS_DIR)/create_trapsets_header.py $< $@
endif
$(ADK_INTERFACE_FW)/app/acl/acl_if.h: ../../common/interface/app/acl/acl_if.h
$(ADK_INTERFACE_FW)/app/adc/adc_if.h: ../../common/interface/app/adc/adc_if.h
$(ADK_INTERFACE_FW)/app/audio/audio_if.h: ../../common/interface/app/audio/audio_if.h
$(ADK_INTERFACE_FW)/app/bitserial/bitserial_if.h: ../../common/interface/app/bitserial/bitserial_if.h
$(ADK_INTERFACE_FW)/app/bluestack/att_prim.h: ../../common/interface/app/bluestack/att_prim.h
$(ADK_INTERFACE_FW)/app/bluestack/bluetooth.h: ../../common/interface/app/bluestack/bluetooth.h
$(ADK_INTERFACE_FW)/app/bluestack/dm_prim.h: ../../common/interface/app/bluestack/dm_prim.h
$(ADK_INTERFACE_FW)/app/bluestack/hci.h: ../../common/interface/app/bluestack/hci.h
$(ADK_INTERFACE_FW)/app/bluestack/l2cap_prim.h: ../../common/interface/app/bluestack/l2cap_prim.h
$(ADK_INTERFACE_FW)/app/bluestack/mdm_prim.h: ../../common/interface/app/bluestack/mdm_prim.h
$(ADK_INTERFACE_FW)/app/bluestack/rfcomm_prim.h: ../../common/interface/app/bluestack/rfcomm_prim.h
$(ADK_INTERFACE_FW)/app/bluestack/sdc_prim.h: ../../common/interface/app/bluestack/sdc_prim.h
$(ADK_INTERFACE_FW)/app/bluestack/sds_prim.h: ../../common/interface/app/bluestack/sds_prim.h
$(ADK_INTERFACE_FW)/app/bluestack/types.h: ../../common/interface/app/bluestack/types.h
$(ADK_INTERFACE_FW)/app/bluestack/vendor_specific_hci.h: ../../common/interface/app/bluestack/vendor_specific_hci.h
$(ADK_INTERFACE_FW)/app/bluestack/vsdm_prim.h: ../../common/interface/app/bluestack/vsdm_prim.h
$(ADK_INTERFACE_FW)/app/capacitive_sensor/capacitive_sensor_if.h: ../../common/interface/app/capacitive_sensor/capacitive_sensor_if.h
$(ADK_INTERFACE_FW)/app/charger/charger_if.h: ../../common/interface/app/charger/charger_if.h
$(ADK_INTERFACE_FW)/app/charger_comms/charger_comms_if.h: ../../common/interface/app/charger_comms/charger_comms_if.h
$(ADK_INTERFACE_FW)/app/debug_partition/debug_partition_data_if.h: ../../common/interface/app/debug_partition/debug_partition_data_if.h
$(ADK_INTERFACE_FW)/app/debug_partition/debug_partition_if.h: ../../common/interface/app/debug_partition/debug_partition_if.h
$(ADK_INTERFACE_FW)/app/dormant/dormant_if.h: ../../common/interface/app/dormant/dormant_if.h
$(ADK_INTERFACE_FW)/app/feature/feature_if.h: ../../common/interface/app/feature/feature_if.h
$(ADK_INTERFACE_FW)/app/file/file_if.h: ../../common/interface/app/file/file_if.h
$(ADK_INTERFACE_FW)/app/flash_ops/flash_ops_if.h: ../../common/interface/app/flash_ops/flash_ops_if.h
$(ADK_INTERFACE_FW)/app/image_upgrade/image_upgrade_if.h: ../../common/interface/app/image_upgrade/image_upgrade_if.h
$(ADK_INTERFACE_FW)/app/infrared/infrared_if.h: ../../common/interface/app/infrared/infrared_if.h
$(ADK_INTERFACE_FW)/app/lcd/lcd_if.h: ../../common/interface/app/lcd/lcd_if.h
$(ADK_INTERFACE_FW)/app/led/led_if.h: ../../common/interface/app/led/led_if.h
$(ADK_INTERFACE_FW)/app/marshal/marshal_if.h: ../../common/interface/app/marshal/marshal_if.h
$(ADK_INTERFACE_FW)/app/message/link_status_if.h: ../../common/interface/app/message/link_status_if.h
$(ADK_INTERFACE_FW)/app/message/subsystem_if.h: ../../common/interface/app/message/subsystem_if.h
$(ADK_INTERFACE_FW)/app/message/system_message.h: ../../common/interface/app/message/system_message.h
$(ADK_INTERFACE_FW)/app/mic_bias/mic_bias_if.h: ../../common/interface/app/mic_bias/mic_bias_if.h
$(ADK_INTERFACE_FW)/app/operator/operator_if.h: ../../common/interface/app/operator/operator_if.h
$(ADK_INTERFACE_FW)/app/partition/partition_if.h: ../../common/interface/app/partition/partition_if.h
$(ADK_INTERFACE_FW)/app/pio/pio_if.h: ../../common/interface/app/pio/pio_if.h
$(ADK_INTERFACE_FW)/app/ps/ps_if.h: ../../common/interface/app/ps/ps_if.h
$(ADK_INTERFACE_FW)/app/psu/psu_if.h: ../../common/interface/app/psu/psu_if.h
$(ADK_INTERFACE_FW)/app/ra_partition/ra_partition_if.h: ../../common/interface/app/ra_partition/ra_partition_if.h
$(ADK_INTERFACE_FW)/app/ringtone/ringtone_if.h: ../../common/interface/app/ringtone/ringtone_if.h
$(ADK_INTERFACE_FW)/app/ringtone/ringtone_notes.h: ../../common/interface/app/ringtone/ringtone_notes.h
$(ADK_INTERFACE_FW)/app/sd_mmc/sd_mmc_if.h: ../../common/interface/app/sd_mmc/sd_mmc_if.h
$(ADK_INTERFACE_FW)/app/sensor/sensor_if.h: ../../common/interface/app/sensor/sensor_if.h
$(ADK_INTERFACE_FW)/app/status/status_if.h: ../../common/interface/app/status/status_if.h
$(ADK_INTERFACE_FW)/app/stream/stream_if.h: ../../common/interface/app/stream/stream_if.h
$(ADK_INTERFACE_FW)/app/uart/uart_if.h: ../../common/interface/app/uart/uart_if.h
$(ADK_INTERFACE_FW)/app/usb/usb_common_if.h: ../../common/interface/app/usb/usb_common_if.h
$(ADK_INTERFACE_FW)/app/usb/usb_hub_if.h: ../../common/interface/app/usb/usb_hub_if.h
$(ADK_INTERFACE_FW)/app/usb/usb_if.h: ../../common/interface/app/usb/usb_if.h
$(ADK_INTERFACE_FW)/app/vm/vm_if.h: ../../common/interface/app/vm/vm_if.h
$(ADK_INTERFACE_FW)/app/voltsense/voltsense_if.h: ../../common/interface/app/voltsense/voltsense_if.h
$(ADK_INTERFACE_FW)/app/bluestack/qbl_adapter_types.h: bt/qbluestack/adapter/qbl_adapter_types.h
$(ADK_INTERFACE_FW)/app/bluestack/qbl_types.h: bt/qbluestack/port/qbl_types.h
$(ADK_INTERFACE_FW)/hydra_macros.h: core/hydra/hydra_macros.h
$(ADK_INTERFACE_STD)/hydra_types.h: core/hydra/hydra_types.h
$(ADK_INTERFACE_FW)/hydra_log.h: core/hydra_log/hydra_log.h
$(ADK_INTERFACE_FW)/hydra_log_disabled.h: core/hydra_log/hydra_log_disabled.h
$(ADK_INTERFACE_FW)/hydra_log_firm.h: core/hydra_log/hydra_log_firm.h
$(ADK_INTERFACE_FW)/hydra_log_firm_modules.h: core/hydra_log/hydra_log_firm_modules.h
$(ADK_INTERFACE_FW)/hydra_log_soft.h: core/hydra_log/hydra_log_soft.h
$(ADK_INTERFACE_STD)/macros.h: core/include/macros.h
$(ADK_INTERFACE_STD)/types.h: core/include/types.h
$(ADK_INTERFACE_FW)/pmalloc.h: core/pmalloc/pmalloc.h
$(ADK_INTERFACE_FW)/pmalloc_trace.h: core/pmalloc/pmalloc_trace.h
$(ADK_INTERFACE_STD)/csrtypes.h: customer/core/trap_api/csrtypes.h
$(ADK_INTERFACE_FW)/hydra_log_subsystems.h: gen/core/hydra_log/hydra_log_subsystems.h
$(ADK_INTERFACE_FW)/acl.h: gen/customer/core/trap_api/acl.h
$(ADK_INTERFACE_FW)/adc.h: gen/customer/core/trap_api/adc.h
$(ADK_INTERFACE_FW)/api.h: gen/customer/core/trap_api/api.h
$(ADK_INTERFACE_FW)/audio_anc.h: gen/customer/core/trap_api/audio_anc.h
$(ADK_INTERFACE_FW)/audio_clock.h: gen/customer/core/trap_api/audio_clock.h
$(ADK_INTERFACE_FW)/audio_mclk.h: gen/customer/core/trap_api/audio_mclk.h
$(ADK_INTERFACE_FW)/audio_power.h: gen/customer/core/trap_api/audio_power.h
$(ADK_INTERFACE_FW)/audio_pwm.h: gen/customer/core/trap_api/audio_pwm.h
$(ADK_INTERFACE_FW)/bdaddr_.h: gen/customer/core/trap_api/bdaddr_.h
$(ADK_INTERFACE_FW)/bitserial_api.h: gen/customer/core/trap_api/bitserial_api.h
$(ADK_INTERFACE_FW)/boot.h: gen/customer/core/trap_api/boot.h
$(ADK_INTERFACE_FW)/capacitivesensor.h: gen/customer/core/trap_api/capacitivesensor.h
$(ADK_INTERFACE_FW)/charger.h: gen/customer/core/trap_api/charger.h
$(ADK_INTERFACE_FW)/chargercomms.h: gen/customer/core/trap_api/chargercomms.h
$(ADK_INTERFACE_FW)/clock_out.h: gen/customer/core/trap_api/clock_out.h
$(ADK_INTERFACE_FW)/codec_.h: gen/customer/core/trap_api/codec_.h
$(ADK_INTERFACE_FW)/crypto.h: gen/customer/core/trap_api/crypto.h
$(ADK_INTERFACE_FW)/csb.h: gen/customer/core/trap_api/csb.h
$(ADK_INTERFACE_FW)/csb_.h: gen/customer/core/trap_api/csb_.h
$(ADK_INTERFACE_FW)/debug_partition_api.h: gen/customer/core/trap_api/debug_partition_api.h
$(ADK_INTERFACE_FW)/dormant.h: gen/customer/core/trap_api/dormant.h
$(ADK_INTERFACE_FW)/energy.h: gen/customer/core/trap_api/energy.h
$(ADK_INTERFACE_FW)/feature.h: gen/customer/core/trap_api/feature.h
$(ADK_INTERFACE_FW)/file.h: gen/customer/core/trap_api/file.h
$(ADK_INTERFACE_FW)/font.h: gen/customer/core/trap_api/font.h
$(ADK_INTERFACE_FW)/host.h: gen/customer/core/trap_api/host.h
$(ADK_INTERFACE_FW)/i2c.h: gen/customer/core/trap_api/i2c.h
$(ADK_INTERFACE_FW)/imageupgrade.h: gen/customer/core/trap_api/imageupgrade.h
$(ADK_INTERFACE_FW)/infrared.h: gen/customer/core/trap_api/infrared.h
$(ADK_INTERFACE_FW)/inquiry.h: gen/customer/core/trap_api/inquiry.h
$(ADK_INTERFACE_FW)/kalimba.h: gen/customer/core/trap_api/kalimba.h
$(ADK_INTERFACE_FW)/lcd.h: gen/customer/core/trap_api/lcd.h
$(ADK_INTERFACE_FW)/led.h: gen/customer/core/trap_api/led.h
$(ADK_INTERFACE_FW)/loader.h: gen/customer/core/trap_api/loader.h
$(ADK_INTERFACE_FW)/marshal.h: gen/customer/core/trap_api/marshal.h
$(ADK_INTERFACE_FW)/message.h: gen/customer/core/trap_api/message.h
$(ADK_INTERFACE_FW)/message_.h: gen/customer/core/trap_api/message_.h
$(ADK_INTERFACE_FW)/micbias.h: gen/customer/core/trap_api/micbias.h
$(ADK_INTERFACE_FW)/native.h: gen/customer/core/trap_api/native.h
$(ADK_INTERFACE_FW)/nfc.h: gen/customer/core/trap_api/nfc.h
$(ADK_INTERFACE_FW)/operator.h: gen/customer/core/trap_api/operator.h
$(ADK_INTERFACE_FW)/operator_.h: gen/customer/core/trap_api/operator_.h
$(ADK_INTERFACE_FW)/os.h: gen/customer/core/trap_api/os.h
$(ADK_INTERFACE_FW)/otp.h: gen/customer/core/trap_api/otp.h
$(ADK_INTERFACE_FW)/panic.h: gen/customer/core/trap_api/panic.h
$(ADK_INTERFACE_FW)/partition.h: gen/customer/core/trap_api/partition.h
$(ADK_INTERFACE_FW)/pio.h: gen/customer/core/trap_api/pio.h
$(ADK_INTERFACE_FW)/ps.h: gen/customer/core/trap_api/ps.h
$(ADK_INTERFACE_FW)/psu.h: gen/customer/core/trap_api/psu.h
$(ADK_INTERFACE_FW)/qspi.h: gen/customer/core/trap_api/qspi.h
$(ADK_INTERFACE_FW)/ra_partition_api.h: gen/customer/core/trap_api/ra_partition_api.h
$(ADK_INTERFACE_FW)/sdmmc.h: gen/customer/core/trap_api/sdmmc.h
$(ADK_INTERFACE_FW)/sensor.h: gen/customer/core/trap_api/sensor.h
$(ADK_INTERFACE_FW)/sink.h: gen/customer/core/trap_api/sink.h
$(ADK_INTERFACE_FW)/sink_.h: gen/customer/core/trap_api/sink_.h
$(ADK_INTERFACE_FW)/source.h: gen/customer/core/trap_api/source.h
$(ADK_INTERFACE_FW)/source_.h: gen/customer/core/trap_api/source_.h
$(ADK_INTERFACE_FW)/sram.h: gen/customer/core/trap_api/sram.h
$(ADK_INTERFACE_FW)/status.h: gen/customer/core/trap_api/status.h
$(ADK_INTERFACE_FW)/stream.h: gen/customer/core/trap_api/stream.h
$(ADK_INTERFACE_FW)/test.h: gen/customer/core/trap_api/test.h
$(ADK_INTERFACE_FW)/test2.h: gen/customer/core/trap_api/test2.h
$(ADK_INTERFACE_FW)/test2_.h: gen/customer/core/trap_api/test2_.h
$(ADK_INTERFACE_FW)/transform.h: gen/customer/core/trap_api/transform.h
$(ADK_INTERFACE_FW)/transform_.h: gen/customer/core/trap_api/transform_.h
$(ADK_INTERFACE_FW)/usb.h: gen/customer/core/trap_api/usb.h
$(ADK_INTERFACE_FW)/usb_hub.h: gen/customer/core/trap_api/usb_hub.h
$(ADK_INTERFACE_FW)/util.h: gen/customer/core/trap_api/util.h
$(ADK_INTERFACE_FW)/vm.h: gen/customer/core/trap_api/vm.h
$(ADK_INTERFACE_FW)/vm_.h: gen/customer/core/trap_api/vm_.h
$(ADK_INTERFACE_FW)/voltsense.h: gen/customer/core/trap_api/voltsense.h
$(ADK_INTERFACE_FW)/app/acl/acl_if.h $(ADK_INTERFACE_FW)/app/adc/adc_if.h $(ADK_INTERFACE_FW)/app/audio/audio_if.h $(ADK_INTERFACE_FW)/app/bitserial/bitserial_if.h $(ADK_INTERFACE_FW)/app/bluestack/att_prim.h $(ADK_INTERFACE_FW)/app/bluestack/bluetooth.h $(ADK_INTERFACE_FW)/app/bluestack/dm_prim.h $(ADK_INTERFACE_FW)/app/bluestack/hci.h $(ADK_INTERFACE_FW)/app/bluestack/l2cap_prim.h $(ADK_INTERFACE_FW)/app/bluestack/mdm_prim.h $(ADK_INTERFACE_FW)/app/bluestack/rfcomm_prim.h $(ADK_INTERFACE_FW)/app/bluestack/sdc_prim.h $(ADK_INTERFACE_FW)/app/bluestack/sds_prim.h $(ADK_INTERFACE_FW)/app/bluestack/types.h $(ADK_INTERFACE_FW)/app/bluestack/vendor_specific_hci.h $(ADK_INTERFACE_FW)/app/bluestack/vsdm_prim.h $(ADK_INTERFACE_FW)/app/capacitive_sensor/capacitive_sensor_if.h $(ADK_INTERFACE_FW)/app/charger/charger_if.h $(ADK_INTERFACE_FW)/app/charger_comms/charger_comms_if.h $(ADK_INTERFACE_FW)/app/debug_partition/debug_partition_data_if.h $(ADK_INTERFACE_FW)/app/debug_partition/debug_partition_if.h $(ADK_INTERFACE_FW)/app/dormant/dormant_if.h $(ADK_INTERFACE_FW)/app/feature/feature_if.h $(ADK_INTERFACE_FW)/app/file/file_if.h $(ADK_INTERFACE_FW)/app/flash_ops/flash_ops_if.h $(ADK_INTERFACE_FW)/app/image_upgrade/image_upgrade_if.h $(ADK_INTERFACE_FW)/app/infrared/infrared_if.h $(ADK_INTERFACE_FW)/app/lcd/lcd_if.h $(ADK_INTERFACE_FW)/app/led/led_if.h $(ADK_INTERFACE_FW)/app/marshal/marshal_if.h $(ADK_INTERFACE_FW)/app/message/link_status_if.h $(ADK_INTERFACE_FW)/app/message/subsystem_if.h $(ADK_INTERFACE_FW)/app/message/system_message.h $(ADK_INTERFACE_FW)/app/mic_bias/mic_bias_if.h $(ADK_INTERFACE_FW)/app/operator/operator_if.h $(ADK_INTERFACE_FW)/app/partition/partition_if.h $(ADK_INTERFACE_FW)/app/pio/pio_if.h $(ADK_INTERFACE_FW)/app/ps/ps_if.h $(ADK_INTERFACE_FW)/app/psu/psu_if.h $(ADK_INTERFACE_FW)/app/ra_partition/ra_partition_if.h $(ADK_INTERFACE_FW)/app/ringtone/ringtone_if.h $(ADK_INTERFACE_FW)/app/ringtone/ringtone_notes.h $(ADK_INTERFACE_FW)/app/sd_mmc/sd_mmc_if.h $(ADK_INTERFACE_FW)/app/sensor/sensor_if.h $(ADK_INTERFACE_FW)/app/status/status_if.h $(ADK_INTERFACE_FW)/app/stream/stream_if.h $(ADK_INTERFACE_FW)/app/uart/uart_if.h $(ADK_INTERFACE_FW)/app/usb/usb_common_if.h $(ADK_INTERFACE_FW)/app/usb/usb_hub_if.h $(ADK_INTERFACE_FW)/app/usb/usb_if.h $(ADK_INTERFACE_FW)/app/vm/vm_if.h $(ADK_INTERFACE_FW)/app/voltsense/voltsense_if.h $(ADK_INTERFACE_FW)/app/bluestack/qbl_adapter_types.h $(ADK_INTERFACE_FW)/app/bluestack/qbl_types.h $(ADK_INTERFACE_FW)/hydra_macros.h $(ADK_INTERFACE_STD)/hydra_types.h $(ADK_INTERFACE_FW)/hydra_log.h $(ADK_INTERFACE_FW)/hydra_log_disabled.h $(ADK_INTERFACE_FW)/hydra_log_firm.h $(ADK_INTERFACE_FW)/hydra_log_firm_modules.h $(ADK_INTERFACE_FW)/hydra_log_soft.h $(ADK_INTERFACE_STD)/macros.h $(ADK_INTERFACE_STD)/types.h $(ADK_INTERFACE_FW)/pmalloc.h $(ADK_INTERFACE_FW)/pmalloc_trace.h $(ADK_INTERFACE_STD)/csrtypes.h $(ADK_INTERFACE_FW)/hydra_log_subsystems.h $(ADK_INTERFACE_FW)/acl.h $(ADK_INTERFACE_FW)/adc.h $(ADK_INTERFACE_FW)/api.h $(ADK_INTERFACE_FW)/audio_anc.h $(ADK_INTERFACE_FW)/audio_clock.h $(ADK_INTERFACE_FW)/audio_mclk.h $(ADK_INTERFACE_FW)/audio_power.h $(ADK_INTERFACE_FW)/audio_pwm.h $(ADK_INTERFACE_FW)/bdaddr_.h $(ADK_INTERFACE_FW)/bitserial_api.h $(ADK_INTERFACE_FW)/boot.h $(ADK_INTERFACE_FW)/capacitivesensor.h $(ADK_INTERFACE_FW)/charger.h $(ADK_INTERFACE_FW)/chargercomms.h $(ADK_INTERFACE_FW)/clock_out.h $(ADK_INTERFACE_FW)/codec_.h $(ADK_INTERFACE_FW)/crypto.h $(ADK_INTERFACE_FW)/csb.h $(ADK_INTERFACE_FW)/csb_.h $(ADK_INTERFACE_FW)/debug_partition_api.h $(ADK_INTERFACE_FW)/dormant.h $(ADK_INTERFACE_FW)/energy.h $(ADK_INTERFACE_FW)/feature.h $(ADK_INTERFACE_FW)/file.h $(ADK_INTERFACE_FW)/font.h $(ADK_INTERFACE_FW)/host.h $(ADK_INTERFACE_FW)/i2c.h $(ADK_INTERFACE_FW)/imageupgrade.h $(ADK_INTERFACE_FW)/infrared.h $(ADK_INTERFACE_FW)/inquiry.h $(ADK_INTERFACE_FW)/kalimba.h $(ADK_INTERFACE_FW)/lcd.h $(ADK_INTERFACE_FW)/led.h $(ADK_INTERFACE_FW)/loader.h $(ADK_INTERFACE_FW)/marshal.h $(ADK_INTERFACE_FW)/message.h $(ADK_INTERFACE_FW)/message_.h $(ADK_INTERFACE_FW)/micbias.h $(ADK_INTERFACE_FW)/native.h $(ADK_INTERFACE_FW)/nfc.h $(ADK_INTERFACE_FW)/operator.h $(ADK_INTERFACE_FW)/operator_.h $(ADK_INTERFACE_FW)/os.h $(ADK_INTERFACE_FW)/otp.h $(ADK_INTERFACE_FW)/panic.h $(ADK_INTERFACE_FW)/partition.h $(ADK_INTERFACE_FW)/pio.h $(ADK_INTERFACE_FW)/ps.h $(ADK_INTERFACE_FW)/psu.h $(ADK_INTERFACE_FW)/qspi.h $(ADK_INTERFACE_FW)/ra_partition_api.h $(ADK_INTERFACE_FW)/sdmmc.h $(ADK_INTERFACE_FW)/sensor.h $(ADK_INTERFACE_FW)/sink.h $(ADK_INTERFACE_FW)/sink_.h $(ADK_INTERFACE_FW)/source.h $(ADK_INTERFACE_FW)/source_.h $(ADK_INTERFACE_FW)/sram.h $(ADK_INTERFACE_FW)/status.h $(ADK_INTERFACE_FW)/stream.h $(ADK_INTERFACE_FW)/test.h $(ADK_INTERFACE_FW)/test2.h $(ADK_INTERFACE_FW)/test2_.h $(ADK_INTERFACE_FW)/transform.h $(ADK_INTERFACE_FW)/transform_.h $(ADK_INTERFACE_FW)/usb.h $(ADK_INTERFACE_FW)/usb_hub.h $(ADK_INTERFACE_FW)/util.h $(ADK_INTERFACE_FW)/vm.h $(ADK_INTERFACE_FW)/vm_.h $(ADK_INTERFACE_FW)/voltsense.h:
	$(PYTHON) $(MAKE_TOOLS_DIR)/install_adk_header.py $< $@
$(LIB_DIR)/lib$(CHIP_TYPE).a: $(OBJECTS)
	$(MAKE_DIR)
	$(AR) ru $@ $^
$(LIB_DIR)/libusb_early_init.a: $(BUILD_DIR)/customer/core/init/init_usb_early_init.o
	$(MAKE_DIR)
	$(AR) ru $@ $^
$(BUILD_DIR)/%.o: %.c $(BUILD_DIR)/%.d
	$(MAKE_DIR)
	$(info Compiling $<)
	$(CC) $(CFLAGS) $(C_DEPGEN_FLAGS) -o $@ -c $<
$(BUILD_DIR)/%.o: %.asm $(BUILD_DIR)/%.d
	$(MAKE_DIR)
	$(info Compiling $<)
	$(CC) $(CFLAGS) $(ASM_DEPGEN_FLAGS) -o $@ -c $<
.PHONY: clean
clean:
	-$(RMDIR) $(call FIXPATH,$(BUILDOUTPUT_PATH)/build)
	-$(RMDIR) $(call FIXPATH,$(LIB_DIR))
.PHONY: make_os_hdrs
make_os_hdrs: $(ADK_INTERFACE)
$(DEPFILES):
include $(wildcard $(DEPFILES))
