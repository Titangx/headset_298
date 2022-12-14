#ifndef __VM_H__
#define __VM_H__
#include <app/vm/vm_if.h>
#include <bdaddr_.h>
#include <vm_.h>

/*! file  @brief VM access to low-level firmware and hardware */

#if TRAPSET_PROFILE

/**
 *  \brief Request a profile to apply.
 *             On supported products, a profile configures the hardware to allow
 *  for enhanced performance at the 
 *             expense of power consumption. The improvement seen in performance
 *  and the power consumption impact 
 *             is product specific. \c vm_runtime_profile defines the available
 *  profiles.
 *             For example, on QCC512x, \c VM_PERFORMANCE profile will configure
 *  the application CPU to 80 MHz 
 *             offering additional code performance.
 *             Profile changes can be requested by the application at any time.
 *  However profile changes 
 *             cannot always be accepted if it is determined that it affects
 *  functionality. The request will still be noted 
 *             and will be applied when possible. The higher performance profiles
 *  are usually applied immediately.
 *         
 *  \param profile             Runtime profile selection. The profiles available are chip
 *  specific. \sa vm_runtime_profile, VmGetRunTimeProfile()
 *             
 *  \return             \return TRUE if the profile could be applied immediately. 
 *             FALSE if the profile change could not be accepted. The profile will
 *  be applied when possible.
 *           
 * 
 * \ingroup trapset_profile
 */
bool VmRequestRunTimeProfile(vm_runtime_profile profile);

/**
 *  \brief Retrieve the current profile applied.
 *         The current profile being used is returned.
 *         
 *  \return             Runtime profile in use. The profiles available are chip specific.
 *  \sa vm_runtime_profile, VmRequestRunTimeProfile()
 *             
 * 
 * \ingroup trapset_profile
 */
vm_runtime_profile VmGetRunTimeProfile(void );
#endif /* TRAPSET_PROFILE */
#if TRAPSET_CORE

/**
 *  \brief Output a character on the given channel. 
 *  \param channel The character to output. 
 *  \param c The channel to output to.
 * 
 * \ingroup trapset_core
 * 
 * WARNING: This trap is UNIMPLEMENTED
 */
void VmPutChar(uint16 channel, uint16 c);

/**
 *  \brief Counts the maximum number of additional memory blocks which can be allocated 
 *  \return  Note that this does not correspond to physical memory; it's possible for
 *  malloc() and similar functions to fail even when this function returns
 *  non-zero; the value returned by this function is only an upper bound on the
 *  number of possible allocations. 
 * 
 * \ingroup trapset_core
 */
uint16 VmGetAvailableAllocations(void );

/**
 *  \brief Read the current value of a 32-bit millisecond timer.
 *   Don't poll this; using MessageSendLater is much more efficient.
 * 
 * \ingroup trapset_core
 */
uint32 VmGetClock(void );

/**
 *  \brief Enables or disables deep sleep. 
 *  \param en FALSE will prevent the chip going into deep sleep. TRUE will permit the chip to
 *  go into deep sleep. It can still be blocked from doing so by other firmware
 *  components. 
 *  \return The previous status.
 * 
 * \ingroup trapset_core
 */
bool VmDeepSleepEnable(bool en);

/**
 *  \brief Request that the radio transmitter be enabled or disabled.
 *   
 *   Equivalent to using ENABLE_TX and DISABLE_TX over BCCMD from
 *   off-chip.
 *  \return TRUE if the request was satisfied, FALSE if it was not possible.
 * 
 * \ingroup trapset_core
 */
bool VmTransmitEnable(bool enabled);

/**
 *  \brief Allows the application to override the default specified by
 *  PSKEY_LC_DEFAULT_TX_POWER. 
 *    The default transmit power is used for paging, inquiry, and their responses,
 *    and as the initial power for new acl links. The value passed is rounded down
 *  to
 *    the next available value when set, so the value returned by a call to 
 *    VmTransmitPowerGetDefault may be less than that previously passed to 
 *    VmTransmitPowerSetDefault.
 *    
 *  \param power int16 The new default power to use in dBm.
 *  \return TRUE on success, else FALSE.
 * 
 * \ingroup trapset_core
 */
bool VmTransmitPowerSetDefault(int16 power);

/**
 *  \brief Allows the application to override the maximum specified by
 *  PSKEY_LC_MAX_TX_POWER. 
 *    The maximum transmit power is only referenced when increasing the transmit
 *  power,
 *    so if the transmit power on a link is already above this level the new value
 *    will not take effect until an attempt is made to increase the power.
 *    The value passed is rounded down to the next available value when set, so
 *  the value 
 *    returned by a call to VmTransmitPowerGetMaximum may be less than that
 *  previously passed 
 *    to VmTransmitPowerSetMaximum.
 *    
 *  \param power The new maximum power to use in dBm.
 *  \return TRUE on success, else FALSE.
 * 
 * \ingroup trapset_core
 */
bool VmTransmitPowerSetMaximum(int16 power);

/**
 *  \brief Returns the current default power setting.
 * 
 * \ingroup trapset_core
 */
int16 VmTransmitPowerGetDefault(void );

/**
 *  \brief Returns the current maximum power setting.
 * 
 * \ingroup trapset_core
 */
int16 VmTransmitPowerGetMaximum(void );

/**
 *  \brief Reads the internal temperature of BlueCore
 *   
 *  \return Approximate temperature in degrees Celsius, or INVALID_TEMPERATURE if it could
 *  not be read.
 * 
 * \ingroup trapset_core
 */
int16 VmGetTemperature(void );

/**
 *  \brief Enables the VM to set the PCM_CLK frequency when using the 4 mHz internal
 *  clock. 
 *  \param frequency The frequency PCM_CLK will output at as a vm_pcm_clock_setting type. Valid
 *  values are PCM_CLK_OFF, PCM_CLK_128, PCM_CLK_256 and PCM_CLK_512 kHz. 
 *  \return TRUE if the change was accepted, else FALSE.
 * 
 * \ingroup trapset_core
 * 
 * WARNING: This trap is UNIMPLEMENTED
 */
bool VmSetPcmClock(vm_pcm_clock_setting frequency);

/**
 *  \brief Enables the VM to check the current PCM_CLK frequency, when it is being
 *  generated from the 4MHz internal clock.
 *     
 *  \return A vm_pcm_clock_setting type indicating the current PCM clock frequency.
 * 
 * \ingroup trapset_core
 * 
 * WARNING: This trap is UNIMPLEMENTED
 */
vm_pcm_clock_setting VmGetPcmClock(void );

/**
 *  \brief Get the absolute value for Vref on this version of BlueCore
 *     This is a constant value for a given version of BlueCore. See adc.h for
 *     the intended use of this function.
 *     In ADK6 firmware, AdcReadRequest() will return reading for Vref
 *  (\#adcsel_vref_hq_buff)
 *     without scaling it to a nominal voltage. This function
 *     provides the information that the battery library needs to adjust its
 *     readings appropriately.
 *     
 *  \return The value of Vref (in mV) for the version of BlueCore the application is
 *  running on.
 * 
 * \ingroup trapset_core
 */
uint16 VmReadVrefConstant(void );

/**
 *  \brief Read the current value of the 32-bit micro-second timer. The 32-bit value
 *  returned will wrap from 0xFFFFFFFF to 0x00000000 every 2**32 / 1e6 / 60 = 71.6
 *  minutes.
 *   Don't poll this; using MessageSendLater is much more efficient.
 *  \return The current value of the 32-bit microsecond timer.
 * 
 * \ingroup trapset_core
 */
uint32 VmGetTimerTime(void );

/**
 *  \brief Reads the temperature sensors on BlueCore BC7+ chips
 *   
 *        Sensor       Action
 *     TSENSOR_MAIN    Reads cached temp from main sensor
 *     TSENSOR_PMU     Reads temp from PMU sensor
 *         xxx         All other sensor numbers return INVALID_SENSOR
 *  \return Temperature in degrees Celsius, or INVALID_TEMPERATURE if it could not be read.
 * 
 * \ingroup trapset_core
 */
int16 VmGetTemperatureBySensor(vm_temp_sensor sensor);

/**
 *  \brief Allows one \#MESSAGE_TX_POWER_CHANGE_EVENT to be sent to the system task. 
 *   This is a one-shot enable, i.e., it allows exactly one power change
 *   message to be sent to the task registered with MessageSystemTask().
 *   Once that message has been sent, VmTransmitPowerMessagesEnable()
 *   must be called again to allow the next power change message to be
 *   sent. This gives the application the opportunity to limit the rate
 *   at which power change messages arrive, which can be quite frequent
 *   if unchecked.
 *   Calling VmTransmitPowerMessagesEnable(TRUE) again before a power
 *   change message has been received has no effect.
 *  \param enable TRUE to permit power change messages to be sent, FALSE to stop them.
 * 
 * \ingroup trapset_core
 */
void VmTransmitPowerMessagesEnable(bool enable);

/**
 *  \brief Returns an enum value relating to the cause of the last reset.
 *            When a system reset occurs on BC7 chips a value is stored which 
 *            relates to the cause of the reset.  This value can be retrieved by a 
 *            VM app and takes the enumerated values defined in the
 *  \#vm_reset_source 
 *            type. Any value not covered by this definition cannot be determined 
 *            and is deemed an unexpected reset.
 *            See the type definition for more information.
 *      
 *  \return A \#vm_reset_source type indicating the source of the last reset.
 * 
 * \ingroup trapset_core
 */
vm_reset_source VmGetResetSource(void );

/**
 *  \brief By default, the VM software watchdog is disabled. Calling this trap with a
 *  valid timeout value will initiate/reset the VM software watchdog timer. If the
 *  VM software watchdog timer expires, then a VM Panic will be raised with Panic
 *  code PANIC_VM_SW_WD_EXPIRED and it will reset the chip.
 *    
 *     \note
 *     The purpose of the 3-stage disable sequence is to ensure that rogue
 *     applications do not randomly disable the watchdog by kicking it with some
 *     single disable codes. The return value of the this trap is designed to
 *     indicate to the user that either the watchdog kick has succeeded due to a
 *     valid timeout within the specified range or that the entire 3-stage disable
 *     sequence has succeeded. Returning TRUE for a call on the first disable
 *  sequence
 *     alone would actually go against the notion that the operation of disabling
 *  the
 *     watchdog has succeeded. In-fact at that moment (after having kicked with the
 *     first disable code), there is no guarantee of whether the disabling is
 *  either
 *     being done deliberately (in a valid manner) or the kick has been called by
 *     some rouge code. Hence, unless the entire disable operation does not
 *  succeed,
 *     the VM must not return TRUE.
 *  \param timeout The timeout period in seconds, in the range 1 to 300 or the specific disable
 *  codes VM_SW_WATCHDOG_DISABLE_CODE1, VM_SW_WATCHDOG_DISABLE_CODE2,
 *  VM_SW_WATCHDOG_DISABLE_CODE3.
 *  \return returns TRUE, 1.Whenever the VM software watchdog is kicked i.e., valid range
 *  (1-300 seconds). Eg: VmsoftwareWDKick(20); - Returns TRUE 2.Whenever the
 *  3-stage disable sequence is followed as per the requirement. Eg:
 *  VmsoftwareWDKick(20); - Returns TRUE
 *  VmSoftwareWDKick(VM_SW_WATCHDOG_DISABLE_CODE1); - Returns FALSE
 *  VmSoftwareWDKick(VM_SW_WATCHDOG_DISABLE_CODE2); - Returns FALSE
 *  VmSoftwareWDKick(VM_SW_WATCHDOG_DISABLE_CODE3); - Returns TRUE returns FALSE,
 *  1.Whenever the VM software watchdog timeout doesn't fall under 1-300 seconds
 *  range. 2.Whenever the VM software watchdog doesn't follow the 3-stage sequence
 *  or disabling the VM software watchdog fails. Eg: VmsoftwareWDKick(20); -
 *  Returns TRUE VmSoftwareWDKick(VM_SW_WATCHDOG_DISABLE_CODE2); - Returns FALSE
 *  VmSoftwareWDKick(VM_SW_WATCHDOG_DISABLE_CODE1); - Returns FALSE
 *  VmSoftwareWDKick(VM_SW_WATCHDOG_DISABLE_CODE3); - Returns FALSE
 * 
 * \ingroup trapset_core
 */
bool VmSoftwareWdKick(uint16 timeout);

/**
 *  \brief Enables or disables some deep sleep wake sources. 
 *  \param wake_source Wake source ID to enable or disable.
 *  \param en TRUE if wake source should be enabled, FALSE otherwise. 
 *  \return TRUE if the operation was successful, FALSE otherwise.
 * 
 * \ingroup trapset_core
 */
bool VmDeepSleepWakeSourcesEnable(vm_wake_source_type wake_source, bool en);

/**
 *  \brief Read the firmware or application version.
 *  \param id Enumeration to select the component whose version is being read
 *  \return Return the version or 0 if the id key is wrong.
 * 
 * \ingroup trapset_core
 */
uint32 VmGetFwVersion(component_id id);

/**
 *  \brief Allows the application to read the Security features from OTP memory. 
 *   This trap takes a bitfield of security features to read
 *   and returns the settings read from the OTP (One Time Program) memory in the
 *   same bitfield format.
 *   
 *   Defines for the bits are provided in the header vm_if.h
 *         
 *  \param read_mask   Bitmask indicating bit to read:
 *   Bit 0     SECURITY_ENABLE
 *   Bit 1     SECURITY_DEBUG_TRANSPORT
 *   Bit 2     SECURITY_USBDBG_DISABLED
 *   Bit 3     SECURITY_DEBUG_ALLOWED
 *   Value of 5 (0b0101) means SECURITY_ENABLE and SECURITY_USBDBG_DISABLED will be
 *   read.
 *             
 *  \return   Return bits read. 
 *   Bit 0     SECURITY_ENABLE
 *   Bit 1     SECURITY_DEBUG_TRANSPORT
 *   Bit 2     SECURITY_USBDBG_DISABLED
 *   Bit 3     SECURITY_DEBUG_ALLOWED
 *             
 *   Example: Return value of 3 (0b0011) with the mask set to 15 (0b1111) means
 *   SECURITY_ENABLE and SECURITY_DEBUG_TRANSPORT bits are set while
 *   SECURITY_USBDBG_DISABLED and SECURITY_DEBUG_ALLOWED bits are not set.
 *           
 * 
 * \ingroup trapset_core
 */
uint16 VmReadSecurity(security_bits read_mask);

/**
 *  \brief Set chip wide security features in OTP memory.
 *   This trap takes a bitfield of security features to write
 *   to the OTP (One Time Program) memory in the bitfield format.
 *   
 *   Defines for the bits are provided in the header vm_if.h 
 *   
 *   A chip reset is required for the new OTP settings to become effective.
 *   VmReadSecurity() will not return the new value until the chip is reset.
 *   
 *   Once the USB Debug disabled is written, USB Debug will be permanently
 *   disabled, with no means to re-enable.
 *   
 *   To write the Security bits in OTP the chip needs to lower the voltage on the
 *   output of the 'Bypass LDO' to 2.65V. This LDO normally outputs 3.3V (when
 *   external power source is connected to VCHG or when BypassLdoForce3v3 System
 *   Manager MIB key is set) or 2.9V. Reducing this voltage may affect
 *   functionality of USB, audio hardware and/or any external circuitry using this
 *   supply rail.
 *   
 *   This trap will return FALSE if invalid bits are selected or if
 *   SECURITY_DEBUG_TRANSPORT is being set without SECURITY_ENABLE bit set. In
 *   these cases the contents of the OTP is not altered.
 *   
 *   The SECURITY_ENABLE bit can be set using the BlueFlash component in BlueSuite.
 *         
 *  \param enable   Security feature bits:
 *   Bit 0     Reserved
 *   Bit 1     SECURITY_DEBUG_TRANSPORT
 *   Bit 2     SECURITY_USBDBG_DISABLED
 *   Example: Enable value of 6 (0b110) requests SECURITY_DEBUG_TRANSPORT and
 *   SECURITY_USBDBG_DISABLED to be set
 *             
 *  \return TRUE if successful, else FALSE. 
 * 
 * \ingroup trapset_core
 */
bool VmEnableSecurity(security_bits enable);

/**
 *  \brief Allows the application to override the power for LE Adverts. 
 *     If the override parameter is TRUE request that LE adverts are 
 *     transmitted at the requested power level. Otherwise revert to
 *     the default for the Bluetooth controller.
 *     In most cases the controller will select the closest dBm
 *     value at or below the requested value, to ensure that max
 *     power limits are not exceeded. However, if the requested
 *     value is lower than the lowest supported transmit power
 *     then the Controller will instead use the lowest supported
 *     value.
 *     
 *     This Traps will return FALSE on CSRA68105, QCC302x and QCC512x.
 *    
 *  \param override Set TRUE to override the system default or FALSE to revert.
 *  \param power The new power to use for LE Adverts in dBm.
 *  \return TRUE on success, else FALSE.
 * 
 * \ingroup trapset_core
 */
bool VmAdvertisePowerSetMaximum(bool override, int16 power);
#endif /* TRAPSET_CORE */
#if TRAPSET_BLUESTACK

/**
 *  \brief Sends an L2CAP Bluestack primitive. 
 *  \param prim A pointer to the primitive to send. The memory must have been dynamically
 *  allocated.
 * 
 * \ingroup trapset_bluestack
 */
void VmSendL2capPrim(void * prim);

/**
 *  \brief Sends a DM Bluestack primitive. 
 *  \param prim A pointer to the primitive to send. The memory must have been dynamically
 *  allocated.
 * 
 * \ingroup trapset_bluestack
 */
void VmSendDmPrim(void * prim);

/**
 *  \brief Sends an SDP Bluestack primitive.
 *  \param prim A pointer to the primitive to send. The memory must have been dynamically
 *  allocated.
 * 
 * \ingroup trapset_bluestack
 */
void VmSendSdpPrim(void * prim);

/**
 *  \brief Converts a VM address space pointer to a handle 
 *     When assembling a primitive which includes indirect blocks, the application
 *     must:
 *     - Allocate a block
 *     - Convert it to a handle
 *     - Store the handle in the primitive rather than storing the pointer itself
 *  \param pointer The pointer to convert.
 * 
 * \ingroup trapset_bluestack
 */
void * VmGetHandleFromPointer(void * pointer);

/**
 *  \brief Converts a handle to a VM address space pointer. 
 *     If a Bluestack primitive refers to indirect blocks of memory, those are
 *     presented in the primitive as handles rather than real pointers. To access
 *     the data an application must pass the handle to VmGetPointerFromHandle()
 *     which will make the indirect block visible to the application. The
 *  application
 *     must call this exactly once for each such indirect block, and the resulting
 *     pointers must all be passed to free. Failure to perform this procedure will 
 *     result in a resource leak.
 *     Note - while VmGetHandleFromPointer() will successfully produce a handle
 *  from
 *     a pointer to a constant, VmGetPointerFromHandle() will not produce a pointer
 *     from such a handle. It will instead panic with
 *  VM_PANIC_READ_FROM_ILLEGAL_ADDRESS.
 *  \param handle The handle to convert.
 * 
 * \ingroup trapset_bluestack
 */
void * VmGetPointerFromHandle(void * handle);

/**
 *  \brief Retrives the tp_bdaddr value from the given CID value.
 *  \param cid The connection identifier to fetch the Bluetooth address from. 
 *  \param tpaddr If the address is found it will be returned to the location pointed at by this
 *  value. 
 *  \return TRUE if an address was found for a given CID, FALSE otherwise. 
 * 
 * \ingroup trapset_bluestack
 */
bool VmGetBdAddrtFromCid(uint16 cid, tp_bdaddr * tpaddr);

/**
 *  \brief Retrieves public address for a given random address.
 *                      Note: To be used only on an established link.
 *  \param random_addr Random address. 
 *  \param public_addr If the public address is found it will be returned to the location pointed at
 *  by this value. 
 *  \return TRUE if public address was found for a given resolvable random address, FALSE
 *  otherwise.
 * 
 * \ingroup trapset_bluestack
 */
bool VmGetPublicAddress(const tp_bdaddr * random_addr, tp_bdaddr * public_addr);

/**
 *  \brief Retrieves local IRK (Identity Resolving Key) of the device.
 *        
 *  The IR (Identity Root) is a root key of the device for the LE Transport and is 
 *  used to generate the local IRK. The local IRK can be requested only after the 
 *  initialization of the IR value using "DM_SM_INIT_REQ_T" primitive.
 *         
 *  \param irk If IRK information could be read, it will be returned to the location pointed
 *  at by this value. 
 *  \return TRUE if local IRK was retrieved, FALSE otherwise.
 * 
 * \ingroup trapset_bluestack
 */
bool VmGetLocalIrk(packed_irk * irk);

/**
 *  \brief Updates ER (Encryption Root) and IR (Identity Root) root key values of the
 *  device. The local IRK/CSRK/LTK are regenerated as a result of calling this
 *  trap.
 *        
 *  The root keys comprise IR and ER keys. These are used to generate IRK, 
 *  CSRK and Legacy LTK. This trap is normally used if a device wants to 
 *  synchronize its root keys with a device with which it would share a common 
 *  Bluetooth address. This trap doesn't update the root keys in the persistent 
 *  store, so the change is not persistent.
 *  Note: The application needs to make sure that it should not call this trap 
 *  when the device is advertising or scanning using a resolvable private address.
 *  Moreover, if the device has a pre-existing LE SC (secure connections) pairing 
 *  with other remote devices and this trap is called then the new ER value will 
 *  not affect the bonding state for those devices. If the device has a legacy 
 *  pairing with remote devices, then overwriting the ER value will invalidate the 
 *  bonding state for those remote devices.
 *         
 *  \param root_keys  New ER and IR values. 
 *  \return TRUE if IR and ER values were set successfully, FALSE otherwise.
 * 
 * \ingroup trapset_bluestack
 */
bool VmUpdateRootKeys(packed_root_keys * root_keys);

/**
 *  \brief Override L2CAP connecton context.
 *        
 * This trap allows application to override the L2CAP connection context for a 
 * given cid. In general, this trap will be used where the L2CAP connection 
 * structures in Bluestack is populated as a result of unmarshalling and not 
 * because of a L2CAP connection initiation from the application. In such a case 
 * connection context in the L2CAP connection structures won't be initialized.
 * So, after unmarshalling, the application needs to explicitly call this trap to 
 * initialize the connection context for all the relevant L2CAP connection ids.
 *         
 *  \param cid  The L2CAP connection id 
 *  \param context  Connection context 
 *  \return TRUE if connection context value is set successfully, FALSE otherwise.
 * 
 * \ingroup trapset_bluestack
 */
bool VmOverrideL2capConnContext(uint16 cid, conn_context_t context);

/**
 *  \brief Override DM Sync connecton context.
 *        
 * This trap allows application to override the DM Sync connection context for a 
 * given eSCO connection handle. In general, this trap will be used where the DM 
 * Sync connection structures in Bluestack is populated as a result of
 *  unmarshalling 
 * and not because of a DM Sync connection initiation from the application. In
 *  such 
 * a case connection context in the DM Sync connection structures won't be
 *  initialized.
 * So, after unmarshalling, the application needs to explicitly call this trap to 
 * initialize the connection context for all the relevant eSCO connection handles.
 *         
 *  \param handle  The eSCO connection handle 
 *  \param context  Connection context 
 *  \return TRUE if connection context value is set successfully, FALSE otherwise.
 * 
 * \ingroup trapset_bluestack
 */
bool VmOverrideSyncConnContext(uint16 handle, conn_context_t context);

/**
 *  \brief Override RFCOMM connecton context.
 *        
 * This trap allows application to override the RFCOMM connection context for a 
 * given RFCOMM connection id. In general, this trap will be used where the RFCOMM 
 * connection structures in Bluestack is populated as a result of unmarshalling 
 * and not because of a RFCOMM client connection initiation from the application. 
 * In such a case connection context in the RFCOMM connection structures won't be 
 * initialized. So, after unmarshalling, the application needs to explicitly call 
 * this trap to initialize the connection context for all the relevant RFCOMM 
 * connection ids.
 *         
 *  \param conn_id  The RFCOMM connection id 
 *  \param context  Connection context 
 *  \return TRUE if connection context value is set successfully, FALSE otherwise.
 * 
 * \ingroup trapset_bluestack
 */
bool VmOverrideRfcommConnContext(uint16 conn_id, conn_context_t context);

/**
 *  \brief Get the mode of the defined ACL connection.
 *        
 * This trap allows the application to read the mode of a ACL connection.
 *         
 *  \param acl_address The remote Bluetooth device address.
 *  \return HCI_BT_MODE_ACTIVE, HCI_BT_MODE_HOLD or HCI_BT_MODE_SNIFF will be returned if
 *  the ACL exists.
 * HCI_BT_MODE_MAX will be returned if the ACL does not exist.
 * These values are defined in the file hci.h.
 * 
 * \ingroup trapset_bluestack
 */
uint8 VmGetAclMode(const tp_bdaddr * acl_address);

/**
 *  \brief Get the role of the defined ACL connection.
 *        
 * This trap allows the application to read the local role of a ACL connection.
 *         
 *  \param acl_address The remote Bluetooth device address.
 *  \return HCI_MASTER or HCI_SLAVE will be returned if the ACL exists.
 * HCI_MASTER_SLAVE_UNKNOWN will be returned if the ACL does not exist.
 * These values are defined in the file hci.h.
 * 
 * \ingroup trapset_bluestack
 */
uint8 VmGetAclRole(const tp_bdaddr * acl_address);

/**
 *  \brief Get the RSSI for the ACL with given remote device.
 *  \param tpaddr The remote Bluetooth device address.
 *  \param rssi If a valid ACL is found, the RSSI in dBm will be written to this location.
 *  \return TRUE if the RSSI has been obtained successfully, FALSE otherwise. 
 * 
 * \ingroup trapset_bluestack
 */
bool VmBdAddrGetRssi(const tp_bdaddr * tpaddr, int16 * rssi);

/**
 *  \brief Get the RSSI for the CIS handle with the remote device.
 *  \param tpbdaddr The remote Bluetooth device address.
 *  \param handle The remote Bluetooth device CIS connection handle.
 *  \param rssi If a valid CIS connection is found, the RSSI in dBm will be written to the
 *  memory pointed to.
 *  \return TRUE if the RSSI has been obtained successfully, FALSE otherwise. This trap
 *  will also return FALSE on CSRA68105, QCC302x, 
 *           QCC512x, QCC514x, and QCC515x devices
 * 
 * \ingroup trapset_bluestack
 */
bool VmCisConnGetRssi(const tp_bdaddr * tpbdaddr, uint16  handle, int16 * rssi);

/**
 *  \brief  Read SCO statistics for the given SCO connection. 
 *  This trap will return FALSE on CSRA68105, QCC302x and
 *           QCC512x devices 
 *  \param handle The handle of SCO connection. 
 *  \param sco_stats Pointer to the memory to write statistics if successful. 
 *  \return TRUE on success, else FALSE.
 * 
 * \ingroup trapset_bluestack
 */
bool VmGetScoStatistics(uint16 handle, sco_statistics_t * sco_stats);

/**
 *  \brief  Obtain the ACL rx link quality.
 *  This trap is intended to support 'wireless' debugging of link issues for 
 *         example as a 'quantitative' metric to compare the quality of the
 *  primary's and secondary's
 *         receptions from the remote Bluetooth device. Only EDR ACL links are
 *  supported and not LE links.
 *         This trap will return FALSE on CSRA68105, QCC302x and QCC512x devices
 *  as their radios 
 *         don't implement this feature. 
 *  \param acl_address The remote Bluetooth device address.
 *  \param link_quality Pointer to the memory to write the link quality if successful.
 *               The return value is a measure of the quality of this link for the
 *  reception of packets.
 *               0 represents the lowest (worst) quality and 65535 represents the
 *  highest (best) quality).
 *             
 *  \return TRUE on success, else FALSE.
 * 
 * \ingroup trapset_bluestack
 */
bool VmGetAclLinkQuality(const tp_bdaddr * acl_address, uint16 * link_quality);

/**
 *  \brief           Return the system clock values for the next Sniff instant and the
 *  next Sniff Subrating instant on the connection for the given bluetooth address.
 *           The next_sniff_clock values give the time in microseconds for the
 *  next sniff instant
 *         
 *  \param tpaddr The Bluetooth address to use. 
 *  \param next_sniff Pointer to the memory to write next sniff clock information if successful. 
 *  \return TRUE on success, else FALSE.
 * 
 * \ingroup trapset_bluestack
 */
bool VmReadNextSniffClock(const tp_bdaddr * tpaddr, next_sniff_clock_t * next_sniff);
#endif /* TRAPSET_BLUESTACK */
#if TRAPSET_RFCOMM

/**
 *  \brief Sends an RFCOMM Bluestack primitive. 
 *  \param prim A pointer to the primitive to send. The memory must have been dynamically
 *  allocated.
 * 
 * \ingroup trapset_rfcomm
 */
void VmSendRfcommPrim(void * prim);
#endif /* TRAPSET_RFCOMM */
#if TRAPSET_VSDM

/**
 *  \brief Sends a BlueStack Vendor Specific Device Manager(VSDM) primitive.
 *  \param prim A pointer to the primitive to send. The memory must have been dynamically
 *  allocated.
 * 
 * \ingroup trapset_vsdm
 */
void VmSendVsdmPrim(void * prim);
#endif /* TRAPSET_VSDM */
#if TRAPSET_MIRRORING

/**
 *  \brief Sends a BlueStack Mirroring Manager(MDM) primitive.
 *  \param prim A pointer to the primitive to send. The memory must have been dynamically
 *  allocated.
 * 
 * \ingroup trapset_mirroring
 */
void VmSendMdmPrim(void * prim);
#endif /* TRAPSET_MIRRORING */
#if TRAPSET_AMUXCLOCK

/**
 *  \brief Enables the amux clock output on AIO0.
 *     Note that this is only available on BC4 parts, excluding BC4-external.
 *     Note that PSKEY_AMUX_CLOCK must be have been set correctly, and
 *     PSKEY_AMUX_AIO0 must be set to ANA_AMUX_A_SEL_DIG_OUT_CLK_AMUX.
 *  \param enabled Whether the clock is enabled.
 * 
 * \ingroup trapset_amuxclock
 * 
 * WARNING: This trap is UNIMPLEMENTED
 */
void VmAmuxClockEnable(bool enabled);
#endif /* TRAPSET_AMUXCLOCK */
#if TRAPSET_BDADDR

/**
 *  \brief Override local Bluetooth device address.
 * This function allows the application to override local Bluetooth device
 *  address. 
 * Bluetooth controller will use the new address instead of the one programmed 
 * on the device until a reset operation or override request with a different 
 * address. This function will return failure if any of below conditions are met:
 * 1) Device is in BR/EDR or LE link.
 * 2) Device is doing paging, page scan, inquiry, inquiry scan, LE scanning,
 *  advertising or initiating.  
 * 3) Device is a CSB broadcaster, listener or receiving synchronisation train.
 * 4) Device is in test, remote or local loopback mode. 
 *         
 *  \param bd_addr Pointer to new Bluetooth device address.
 *  \return TRUE if the request succeeded, else FALSE.
 * 
 * \ingroup trapset_bdaddr
 */
bool VmOverrideBdaddr(const bdaddr * bd_addr);
#endif /* TRAPSET_BDADDR */
#if TRAPSET_ATT

/**
 *  \brief Sends an ATT Bluestack primitive. 
 *  \param prim A pointer to the primitive to send. The memory must have been dynamically
 *  allocated.
 * 
 * \ingroup trapset_att
 */
void VmSendAttPrim(void * prim);

/**
 *  \brief Remvoes all the advertising filters, allowing all advert reports through to the
 *  host.
 * 
 * \ingroup trapset_att
 */
bool VmClearAdvertisingReportFilter(void );

/**
 *  \brief This helps in controlling how BlueCore filters advertising report events by BLE
 *  advertising data content. 
 *         Filtering is based only on the contents of advertising
 *         data. Since directed connectable adverts do not contain advertising
 *         data, and they are expressly intended for the receiving device,
 *         they are always passed to the host and are unaffected by this
 *         filter. The event type, Bluetooth device address of the sender and 
 *         other properties of the advertising reports are ignored by the 
 *         filter.
 *         With no filter present, all advertising packets received during 
 *         scanning are passed to the host in LE Advertising Report Events, 
 *         subject to advert flood protection. The filter is used to select 
 *         advertising reports based on the contents of the advertising data
 *         (AD) and send to the host only the matching reports, thus saving
 *         the host being woken up unnecessarily.
 *         
 *  \param operation  Describes the relationship between multiple filters. Currently the only valid
 *  operation is OR (0x00), meaning that adverts will be sent to the host if they
 *  are matched by any of the filters.
 *  \param ad_type The AD type of the AD structure to match. The filter will only match adverts
 *  containing an AD structure of this type. Enum as defined in Bluetooth Assigned
 *  Numbers.
 *  \param interval The interval for repeated attempts to match the pattern in the data portion of
 *  the AD structure. For example, if the interval is 4 then we attempt to match
 *  at offsets, 0, 4, 8, 12, etc in the data portion of the AD structure. If
 *  interval is 0 then we only attempt to match at offset 0. If interval is
 *  0xffff, then exact match is required.
 *  \param pattern_length The length of the pattern data, i.e. number of uint8s pattern data present in
 *  location pointed by @a pattern_addr.
 *  \param pattern_addr A pointer to the malloc'd space where pattern data is present. Each pattern
 *  data is 8bit data and no two pattern data should be packed inside uint16.
 *  BlueCore will only consider lower 8 bit of each pattern data for pattern
 *  matching. If pattern_addr is NULL, then no pattern matching. The allocated
 *  memory for pattern data will be freed by the firmware.
 *  \return TRUE if the advertising filter pattern is added successfully, otherwise FALSE. 
 * 
 * \ingroup trapset_att
 */
bool VmAddAdvertisingReportFilter(uint16 operation, uint16 ad_type, uint16 interval, uint16 pattern_length, uint8 * pattern_addr);
#endif /* TRAPSET_ATT */
#if TRAPSET_PSU

/**
 *  \brief Returns a bit pattern representing the source of the voltage regulator enable
 *  signal on (re)boot.
 *            When the voltage regulators of a CSR8670 or CSR8670-like chip are 
 *            enabled as part of the turn-on sequence or during reset, a value is 
 *            stored indicating the source of the signals responsible for enabling 
 *            the chip's power supplies.  This value can be retrieved by a VM app
 *  and 
 *            is a bit-pattern composed of the enumerated values defined in the 
 *            \#vm_power_enabler type.
 *            See the type definition for more information.
 *      
 *  \return A bit pattern representing regulator enabling signals. See the
 *  \#vm_power_enabler type definition.
 * 
 * \ingroup trapset_psu
 */
uint16 VmGetPowerSource(void );
#endif /* TRAPSET_PSU */
#endif
