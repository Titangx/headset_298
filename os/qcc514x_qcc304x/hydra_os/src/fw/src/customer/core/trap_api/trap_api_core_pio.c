/* Copyright (c) 2016 - 2021 Qualcomm Technologies International, Ltd. */
/*   %%version */
/**
 * \file
 * PIO trap file. This implements the high level functions for the PIO traps.
 */

#include "trap_api/trap_api_private.h"
#include "pioint/pioint.h"
#include "pio_cfg/pio_cfg.h"
#include "ledctrl/ledctrl.h"
#include "panic/panic.h"
#include "fault/fault.h"
#include <pio.h>

#ifndef DESKTOP_TEST_BUILD

enum
{
    PIO_TRAP_PANIC_GET_32_BANK = 0,
    PIO_TRAP_PANIC_GET_DIR_32_BANK,
    PIO_TRAP_PANIC_GET_STRONG_BIAS_32_BANK,
    PIO_TRAP_PANIC_GET_MAP_PINS_32_BANK,
    PIO_TRAP_PANIC_GET_DRIVE_STRENGTH,
    PIO_TRAP_PANIC_GET_UNUSED_PINS_32_BANK,
    PIO_TRAP_PANIC_GET_PULL_ENABLE_32_BANK,
    PIO_TRAP_PANIC_GET_PULL_DIRECTION_32_BANK,
    PIO_TRAP_PANIC_GET_PULL_STICKY_32_BANK
};

/** Constant definition for an unused parameter. */
#define UNUSED_PARAM 0

static uint32 pio_pull_drive_dir[NUMBER_OF_PIO_BANKS];

/**
 * Stores the PIO mapping.
 */
static uint32 pio_map[NUMBER_OF_PIO_BANKS];

/**
 * Stores the PIO ownership.
 */
static uint32 pio_owned[NUMBER_OF_PIO_BANKS];

/**
 * Helper function for getting IO mapped PIOs.
 * \param bank Bank index.
 * \param mask PIO mask to check.
 * \return mask of PIOs that are IO mapped.
 */
static uint32 pio_get_io_mapped(uint16 bank, uint32 mask);

/**
 * Helper function for getting HW mapped PIOs.
 * \param bank Bank index.
 * \param mask PIO mask to check.
 * \return mask of PIOs that are HW mapped.
 */
static uint32 pio_get_hw_mapped(uint16 bank, uint32 mask);

/**
 * Helper function for getting not owned PIOs.
 * \param bank Bank index.
 * \param mask PIO mask to check.
 * \return mask of PIOs that are not owned.
 */
static uint32 pio_get_not_owned(uint16 bank, uint32 mask);

/**
 * Helper function for checking PIO parameters.
 * \param bank Bank index.
 * \param mask PIO mask to check. If 0, this parameter is ignored
 * \return 0 if parameters are valid, -1 if bank is invalid
 */
static uint32 pio_check_params(uint16 bank, uint32 mask);

/**
 * Helper function for combined checking PIO parameters and if they are not owned
 * \param bank Bank index
 * \param mask PIO mask to check
 * \return \c pio_check_params if not zero, else \c pio_get_not_owned
 */
static uint32 pio_check_params_and_not_owned(uint16 bank, uint32 mask);

/**
 * Helper function for simple common PIO IPC information retrieval
 * \param bank Bank index
 * \param mask PIO mask to query
 * \param op PIO IPC Operation to do
 * \param diatribe Diatribe to use in the case of a panic
 * \param log_name Function name (as a HYDRA_LOG_STRING)
 * \return Return from \c pio_ipc
 */
static uint32 pio_get_pio_ipc_common(uint16 bank, uint32 mask, pio_ipc_op op, DIATRIBE_TYPE diatribe, const char *log_name);

/**
 * Helper function for simple common PIO IPC settings change
 * \param bank Bank index
 * \param mask PIO mask for which bits to modify
 * \param bits New setting bits for each PIO in mask
 * \param op PIO IPC Operation to do
 * \param log_name Function name (as a HYDRA_LOG_STRING)
 * \return Return from \c pio_ipc
 */
static uint32 pio_set_pio_ipc_common(uint16 bank, uint32 mask, uint32 bits, pio_ipc_op op, const char *log_name);

/**
 * Helper function for catching a PIO event (registered to piodebounce). This
 * sends a message to the customer task. We require this to be run in the
 * background.
 * \param group      PIO debounce group ID.
 * \param bank       PIO bank index.
 * \param state      PIO levels.
 * \param timestamp  PIO debounce timestamp (in microseconds).
 */
static void pio_handler(uint16 group, uint16 bank, pio_size_bits state,
                        TIME timestamp);

uint32 PioGet32Bank(uint16 bank)
{
    uint32 status;

    L4_DBG_MSG1("PIO trap: PioGet32Bank(%d)", bank);

    status = pio_check_params(bank, 0);
    if (status)
    {
        panic_diatribe(PANIC_PIO_TRAP_BAD_PARAMETERS,
                       PIO_TRAP_PANIC_GET_32_BANK);
    }

    /* pio_get_levels_mask returns driven value for outputs and status for
       inputs. Just what we need. */
    return pio_get_levels_mask(bank, ALL_PIOS);
}

uint32 PioSet32Bank(uint16 bank, uint32 mask, uint32 bits)
{
    uint32 status, input_mask, output_mask;

    L4_DBG_MSG3("PIO trap: PioSet32Bank(%d, 0x%x, 0x%x)", bank, mask, bits);

    status = pio_check_params_and_not_owned(bank, mask);
    if (status)
    {
        return status;
    }

    /* update pull direction and drive record */
    pio_pull_drive_dir[bank] = pio_pull_drive_dir[bank] & (~mask) | (bits & mask);
    /* Set levels of output PIOs. */
    output_mask = pio_get_directions_mask(bank, mask);
    pio_set_levels_mask(bank, output_mask, pio_pull_drive_dir[bank]);
    /* Set pull directions for input PIOs. */
    input_mask = mask & (~output_mask);
    if (input_mask)
    {
        status = pio_ipc(SET_PULLDIR, bank, input_mask, bits & input_mask);
        if (status)
        {
            return status;
        }
        status = pio_ipc(SET_PULLEN, bank, input_mask, input_mask);
        if (status)
        {
            return status;
        }
    }
    return 0;
}

uint32 PioGetDir32Bank(uint16 bank)
{
    uint32 status;

    L4_DBG_MSG1("PIO trap: PioGetDir32Bank(%d)", bank);

    status = pio_check_params(bank, 0);
    if (status)
    {
        panic_diatribe(PANIC_PIO_TRAP_BAD_PARAMETERS,
                       PIO_TRAP_PANIC_GET_DIR_32_BANK);
    }

    return pio_get_directions_mask(bank, ALL_PIOS);
}

uint32 PioSetDir32Bank(uint16 bank, uint32 mask, uint32 dir)
{
    uint32 status, input_mask, output_mask;

    L4_DBG_MSG3("PIO trap: PioSetDir32Bank(%d, 0x%x, 0x%x)", bank, mask, dir);

    status = pio_check_params_and_not_owned(bank, mask);
    if (status)
    {
        return status;
    }

    output_mask = mask & dir;
    if (output_mask)
    {
        /* Set drive enable and level for output PIOs */
        pio_set_levels_mask(bank, output_mask, pio_pull_drive_dir[bank]);
        pio_set_directions_mask(bank, output_mask, output_mask);
        /* Disable pulls for output PIOs */
        status = pio_ipc(SET_PULLEN, bank, output_mask, 0);
        if (status)
        {
            return status;
        }
    }

    input_mask = mask & (~output_mask);
    if (input_mask)
    {
        /* Set and enable pulls for input PIOs */
        status = pio_ipc(SET_PULLDIR, bank, input_mask, pio_pull_drive_dir[bank] & input_mask);
        if (status)
        {
            return status;
        }
        status = pio_ipc(SET_PULLEN, bank, input_mask, input_mask);
        if (status)
        {
            return status;
        }
        /* Set drive disable for input PIOs */
        pio_set_directions_mask(bank, input_mask, 0);
    }
    return 0;
}

HYDRA_LOG_STRING(pio_log_string_PioGetStrongBias32Bank, "PioGetStrongBias32Bank");
uint32 PioGetStrongBias32Bank(uint16 bank)
{
    return pio_get_pio_ipc_common(bank, ALL_PIOS, GET_PULLSTR, PIO_TRAP_PANIC_GET_STRONG_BIAS_32_BANK, pio_log_string_PioGetStrongBias32Bank);
}

HYDRA_LOG_STRING(pio_log_string_PioSetStrongBias32Bank, "PioSetStrongBias32Bank");
uint32 PioSetStrongBias32Bank(uint16 bank, uint32 mask, uint32 bits)
{
    return pio_set_pio_ipc_common(bank, mask, bits, SET_PULLSTR, pio_log_string_PioSetStrongBias32Bank);
}

uint32 PioGetMapPins32Bank(uint16 bank)
{
    uint32 status;

    L4_DBG_MSG1("PIO trap: PioGetMapPins32Bank(%d)", bank);

    status = pio_check_params(bank, 0);
    if (status)
    {
        panic_diatribe(PANIC_PIO_TRAP_BAD_PARAMETERS,
                       PIO_TRAP_PANIC_GET_MAP_PINS_32_BANK);
    }

    return pio_map[bank];
}

uint32 PioSetMapPins32Bank(uint16 bank, uint32 mask, uint32 bits)
{
    uint32 status;
    uint32 to_own;
    uint32 to_mux_as_pio;
    uint32 input_mask;

    L4_DBG_MSG3("PIO trap: PioSetMapPins32Bank(%d, 0x%x, 0x%x)",
                bank, mask, bits);

    status = pio_check_params(bank, mask);
    if (status)
    {
        return status;
    }

    to_own = pio_get_not_owned(bank, mask);
    if (to_own)
    {
        status = pio_ipc(ACQUIRE, bank, to_own, to_own);
        if (status)
        {
            return status;
        }
    }

    to_mux_as_pio = mask & bits;
    if (to_mux_as_pio)
    {
        status = pio_ipc(SET_OWNER, bank, to_mux_as_pio, to_mux_as_pio);
        if (status)
        {
            return status;
        }
        status = pio_ipc(SET_PAD_MUX, bank, to_mux_as_pio,
                         PIO_CFG_PAD_MUX_CORE_PIO);
        if (status)
        {
            return status;
        }
        status = pio_ipc(SET_PIO_MUX, bank, to_mux_as_pio,
                         IPC_PIO_MUX_APPS);
        if (status)
        {
            return status;
        }
        /* it's OK if this fails, not all chips and not all PIOs support this */
        (void)pio_ipc(SET_XIO_MODE, bank, mask, 0);
        input_mask = (~pio_get_directions_mask(bank, mask)) & mask;
        if (input_mask)
        {
            status = pio_ipc(SET_PULLEN, bank, input_mask, input_mask);
            if (status)
            {
                return status;
            }
        }
        ledctrl_set_pio_mask(bank, mask, FALSE);
    }

    pio_owned[bank] = BIT_WRITE_MASK(pio_owned[bank], mask, mask);
    pio_map[bank] = BIT_WRITE_MASK(pio_map[bank], mask, bits);
    return 0;
}

bool PioSetFunction(uint16 pin, pin_function_id function)
{
    uint32 status;
    uint16 bank = PBANK(pin);
    uint32 mask = POFFM(pin);

    status = pio_get_io_mapped(bank, mask);
    if (status)
    {
        return FALSE;
    }

    switch(function)
    {
        case LED:
        case ANALOGUE:
        case BITSERIAL_0_CLOCK_IN:
        case BITSERIAL_0_CLOCK_OUT:
        case BITSERIAL_0_DATA_IN:
        case BITSERIAL_0_DATA_OUT:
        case BITSERIAL_0_SEL_IN:
        case BITSERIAL_0_SEL_OUT:
        case BITSERIAL_1_CLOCK_IN:
        case BITSERIAL_1_CLOCK_OUT:
        case BITSERIAL_1_DATA_IN:
        case BITSERIAL_1_DATA_OUT:
        case BITSERIAL_1_SEL_IN:
        case BITSERIAL_1_SEL_OUT:
#if HAVE_NUMBER_OF_BITSERIALS > 2
        case BITSERIAL_2_CLOCK_IN:
        case BITSERIAL_2_CLOCK_OUT:
        case BITSERIAL_2_DATA_IN:
        case BITSERIAL_2_DATA_OUT:
        case BITSERIAL_2_SEL_IN:
        case BITSERIAL_2_SEL_OUT:
#endif /* HAVE_NUMBER_OF_BITSERIALS > 2 */
        case UART_RX:
        case UART_TX:
        case UART_CTS:
        case UART_RTS:
        case CHARGER_COMMS_UART_TX_RX:
        case CHARGER_COMMS_PULL:
        case CHARGER_COMMS_VCHG_REG:
        case CHARGER_COMMS_ISOLATE:
        case OTHER:
            status = pio_ipc(SET_PULLEN, bank, mask, 0);
            if (status)
            {
                return FALSE;
            }
            status = pio_ipc(SET_PAD_MUX, bank, mask, PIO_CFG_PAD_MUX_CORE_PIO);
            if (status)
            {
                return FALSE;
            }
            if ((function == ANALOGUE) || (function == OTHER))
            {
                status = pio_ipc(SET_PIO_MUX, bank, mask, IPC_PIO_MUX_APPS);
                if (status)
                {
                    return FALSE;
                }

            }
            ledctrl_set_pio_mask(bank, mask, FALSE);
            break;
        case RESETB:
            status = pio_ipc(SET_PULLEN, bank, mask, mask);
            if (status)
            {
                return FALSE;
            }
            status = pio_ipc(SET_PULLDIR, bank, mask, mask);
            if (status)
            {
                return FALSE;
            }
            status = pio_ipc(SET_PULLSTR, bank, mask, mask);
            if (status)
            {
                return FALSE;
            }
            status = pio_ipc(SET_PAD_MUX, bank, mask, PIO_CFG_PAD_MUX_RESETB);
            if (status)
            {
                return FALSE;
            }
            return TRUE;
        default:
            return FALSE;
    }

    switch(function)
    {
        case LED:
            status = pio_ipc(SET_PIO_MUX, bank, mask, IPC_PIO_MUX_LED_CTRL);
            if (status)
            {
                return FALSE;
            }
            ledctrl_set_pio_mask(bank, mask, TRUE);
            break;
        case ANALOGUE:
            /*lint -e774 */
            if (((bank == PIO_CFG_XIO_BANK) && (mask & PIO_CFG_XIO_MASK)) ||
                ((bank == PIO_CFG_LED_BANK) && (mask & PIO_CFG_LED_MASK)))
            {
                /* set drive enable to 0 for analogue mode
                   (XIOs needs this thanks to HW signal overloading)
                   (for LEDs we make sure they are not driving) */
                status = pio_ipc(SET_DRIVE_ENABLE, bank, mask, 0);
                if (status)
                {
                    return FALSE;
                }
                /* set drive level to 0 for analogue mode
                   (XIOs needs this thanks to HW signal overloading) */
                status = pio_ipc(SET_DRIVE, bank, mask, 0);
                if (status)
                {
                    return FALSE;
                }
                /* set PIO mux to P0 to activate drive enable and drive level */
                status = pio_ipc(SET_OWNER, bank, mask, 0);
                if (status)
                {
                    return FALSE;
                }
                /* XIOs need more stuff set up
                   LEDs are set up by the Curator from this point */
                /*lint -e774 */
                if ((bank == PIO_CFG_XIO_BANK) && (mask & PIO_CFG_XIO_MASK))
                {
                    /* set drive strength for analogue mode
                       (thanks to HW signal overloading) */
                    status = pio_ipc(SET_DRIVE_STRENGTH, bank, mask, 2);
                    if (status)
                    {
                        return FALSE;
                    }
                    /* set mode for analogue */
                    status = pio_ipc(SET_XIO_MODE, bank, mask, mask);
                    if (status)
                    {
                        return FALSE;
                    }
                }
            }
            else
            {
                /* Analogue mode not supported */
                return FALSE;
            }
            break;
        case BITSERIAL_0_CLOCK_IN:
        case BITSERIAL_0_CLOCK_OUT:
        case BITSERIAL_0_DATA_IN:
        case BITSERIAL_0_DATA_OUT:
        case BITSERIAL_0_SEL_IN:
        case BITSERIAL_0_SEL_OUT:
        case BITSERIAL_1_CLOCK_IN:
        case BITSERIAL_1_CLOCK_OUT:
        case BITSERIAL_1_DATA_IN:
        case BITSERIAL_1_DATA_OUT:
        case BITSERIAL_1_SEL_IN:
        case BITSERIAL_1_SEL_OUT:
#if HAVE_NUMBER_OF_BITSERIALS > 2
        case BITSERIAL_2_CLOCK_IN:
        case BITSERIAL_2_CLOCK_OUT:
        case BITSERIAL_2_DATA_IN:
        case BITSERIAL_2_DATA_OUT:
        case BITSERIAL_2_SEL_IN:
        case BITSERIAL_2_SEL_OUT:
#endif /* HAVE_NUMBER_OF_BITSERIALS > 2 */
            status = pio_ipc(SET_PIO_MUX, bank, mask, IPC_PIO_MUX_HOST);
            if (status)
            {
                return FALSE;
            }
            status = pio_ipc(SET_FUNC_BITSERIAL, bank, mask, function);
            if (status)
            {
                return FALSE;
            }
            break;
        case UART_RX:
        case UART_TX:
        case UART_CTS:
        case UART_RTS:
            status = pio_ipc(SET_PIO_MUX, bank, mask, IPC_PIO_MUX_HOST);
            if (status)
            {
                return FALSE;
            }
            status = pio_ipc(SET_FUNC_UART, bank, mask, function);
            if (status)
            {
                return FALSE;
            }
            break;

        case CHARGER_COMMS_UART_TX_RX:
        case CHARGER_COMMS_PULL:
        case CHARGER_COMMS_VCHG_REG:
        case CHARGER_COMMS_ISOLATE:
            status = pio_ipc(SET_PIO_MUX, bank, mask,
                             function == CHARGER_COMMS_UART_TX_RX ? IPC_PIO_MUX_HOST : IPC_PIO_MUX_APPS);
            if (status)
            {
                return FALSE;
            }
            status = pio_ipc(SET_FUNC_CHARGER_COMMS, bank, mask, function);
            if (status)
            {
                return FALSE;
            }
            break;

        case OTHER:
            status = pio_ipc(RELEASE, bank, mask, 0);
            if (status)
            {
                return FALSE;
            }
            pio_owned[bank] = BIT_WRITE_MASK(pio_owned[bank], mask, 0);
            break;
        default:
            return FALSE;
    }
    return TRUE;
}

HYDRA_LOG_STRING(pio_log_string_PioGetDriveStrength, "PioGetDriveStrength");
pin_drive_strength_id PioGetDriveStrength(uint16 pin)
{
    uint16 bank = PBANK(pin);
    uint32 mask = POFFM(pin);

    return (pin_drive_strength_id) pio_get_pio_ipc_common(
        bank,
        mask,
        GET_DRIVE_STRENGTH,
        PIO_TRAP_PANIC_GET_DRIVE_STRENGTH,
        pio_log_string_PioGetDriveStrength
    );
}

HYDRA_LOG_STRING(pio_log_string_PioSetDriveStrength32Bank, "PioSetDriveStrength32Bank");
uint32 PioSetDriveStrength32Bank(uint16 bank,
                                 uint32 mask,
                                 pin_drive_strength_id drive_strength)
{
    return pio_set_pio_ipc_common(bank, mask, (uint32)drive_strength, SET_DRIVE_STRENGTH, pio_log_string_PioSetDriveStrength32Bank);
}

uint32 PioDebounce32Bank(uint16 bank, uint32 mask, uint16 count, uint16 period)
{
    return PioDebounceGroup32Bank(0, bank, mask, count, period);
}

uint32 PioDebounceGroup32Bank(uint16 group, uint16 bank, uint32 mask,
                              uint16 count, uint16 period)
{
    uint32 status;

    L4_DBG_MSG3("PIO trap: PioDebounceGroup32Bank(%d, %d, 0x%x)",
                group, bank, mask);

    if (group >= PIODEBOUNCE_NUMBER_OF_GROUPS)
    {
        return mask;
    }
    status = pio_check_params_and_not_owned(bank, mask);
    if (status)
    {
        return status;
    }
    status = pio_get_hw_mapped(bank, mask);
    if (status)
    {
        return status;
    }
    piodebounce_config(group, bank, mask, count, period, PRIORITY_LOW, pio_handler);
    PioSetDeepSleepWakeBank(bank, mask, mask);
    return 0;
}

Task MessagePioTask(Task task)
{
    L4_DBG_MSG1("PIO trap: MessagePioTask(0x%x)", task);
    return trap_api_register_message_task(task, IPC_MSG_TYPE_PIO);
}

uint32 PioDebounceGroupBankPriority(uint16 group, uint16 bank, uint32 mask,
                              uint16 count, uint16 period, pio_group_priority priority)
{
    UNUSED(group);
    UNUSED(bank);
    UNUSED(mask);
    UNUSED(count);
    UNUSED(period);
    UNUSED(priority);
    return FALSE;
}

Task MessagePioGroupTask(Task task, uint16 group)
{
    UNUSED(task);
    UNUSED(group);
    return NULL;
}


HYDRA_LOG_STRING(pio_log_string_PioGetUnusedPins32Bank, "PioGetUnusedPins32Bank");
uint32 PioGetUnusedPins32Bank(uint16 bank)
{
    return pio_get_pio_ipc_common(bank, UNUSED_PARAM, GET_UNUSED, PIO_TRAP_PANIC_GET_UNUSED_PINS_32_BANK, pio_log_string_PioGetUnusedPins32Bank);
}

static uint32 pio_get_pio_ipc_common(uint16 bank, uint32 mask, pio_ipc_op op, DIATRIBE_TYPE diatribe, const char *log_name)
{
    uint32 status;

    L4_DBG_MSG2("PIO trap: %s(%u)", log_name, bank);

    status = pio_check_params(bank, 0);
    if (status)
    {
        panic_diatribe(PANIC_PIO_TRAP_BAD_PARAMETERS,
                       diatribe);
    }

    return pio_ipc(op, bank, mask, UNUSED_PARAM);
}

static uint32 pio_set_pio_ipc_common(uint16 bank, uint32 mask, uint32 bits, pio_ipc_op op, const char *log_name)
{
    uint32 status;

    L4_DBG_MSG4("PIO trap: %s(%u, 0x%X, 0x%X)", log_name, bank, mask, bits);

    status = pio_check_params_and_not_owned(bank, mask);
    if (status)
    {
        return status;
    }
    return pio_ipc(op, bank, mask, bits);
}

HYDRA_LOG_STRING(pio_log_string_PioGetPullEnable32Bank, "PioGetPullEnable32Bank");
uint32 PioGetPullEnable32Bank(uint16 bank)
{
    return pio_get_pio_ipc_common(bank, ALL_PIOS, GET_PULLEN, PIO_TRAP_PANIC_GET_PULL_ENABLE_32_BANK, pio_log_string_PioGetPullEnable32Bank);
}

HYDRA_LOG_STRING(pio_log_string_PioSetPullEnable32Bank, "PioSetPullEnable32Bank");
uint32 PioSetPullEnable32Bank(uint16 bank, uint32 mask, uint32 bits)
{
    return pio_set_pio_ipc_common(bank, mask, bits, SET_PULLEN, pio_log_string_PioSetPullEnable32Bank);
}

HYDRA_LOG_STRING(pio_log_string_PioGetPullDirection32Bank, "PioGetPullDirection32Bank");
uint32 PioGetPullDirection32Bank(uint16 bank)
{
    return pio_get_pio_ipc_common(bank, ALL_PIOS, GET_PULLDIR, PIO_TRAP_PANIC_GET_PULL_DIRECTION_32_BANK, pio_log_string_PioGetPullDirection32Bank);
}

HYDRA_LOG_STRING(pio_log_string_PioSetPullDirection32Bank, "PioSetPullDirection32Bank");
uint32 PioSetPullDirection32Bank(uint16 bank, uint32 mask, uint32 bits)
{
    return pio_set_pio_ipc_common(bank, mask, bits, SET_PULLDIR, pio_log_string_PioSetPullDirection32Bank);
}

HYDRA_LOG_STRING(pio_log_string_PioGetPullSticky32Bank, "PioGetPullSticky32Bank");
uint32 PioGetPullSticky32Bank(uint16 bank)
{
    return pio_get_pio_ipc_common(bank, ALL_PIOS, GET_STICKY, PIO_TRAP_PANIC_GET_PULL_STICKY_32_BANK, pio_log_string_PioGetPullSticky32Bank);
}

HYDRA_LOG_STRING(pio_log_string_PioSetPullSticky32Bank, "PioSetPullSticky32Bank");
uint32 PioSetPullSticky32Bank(uint16 bank, uint32 mask, uint32 bits)
{
    return pio_set_pio_ipc_common(bank, mask, bits, SET_STICKY, pio_log_string_PioSetPullSticky32Bank);
}

static void pio_handler(uint16 group, uint16 bank, pio_size_bits state, TIME timestamp)
{
    MessagePioChanged *message;

    L4_DBG_MSG4("PIO trap: message to task, group %d bank %d state 0x%x "
                "timestamp 0x%x", group, bank, state, timestamp);

    message = pnew(MessagePioChanged);
    message->state = (uint16) (state & 0xffff);
    message->time = (uint32) (timestamp / MILLISECOND);
    message->state16to31 = (uint16) (state >> 16);
    message->bank = bank;

    trap_api_send_pio_message_filtered(group, MESSAGE_PIO_CHANGED, message, TRUE);
}

static uint32 pio_get_io_mapped(uint16 bank, uint32 mask)
{
    return pio_map[bank] & mask;
}

static uint32 pio_get_hw_mapped(uint16 bank, uint32 mask)
{
    return (~pio_map[bank]) & mask;
}

static uint32 pio_get_not_owned(uint16 bank, uint32 mask)
{
    return (~pio_owned[bank]) & mask;
}

static uint32 pio_check_params(uint16 bank, uint32 mask)
{
    if ( bank >= NUMBER_OF_PIO_BANKS )
    {
        L4_DBG_MSG1("PIO trap: Invalid Bank(%d)", bank);
        return ~(0u);
    }

#if POFF(NUMBER_OF_PIOS) != 0
    /* mask checks only required for the last bank */
    if ( bank == (NUMBER_OF_PIO_BANKS-1) &&
         mask & ~(POFFM(NUMBER_OF_PIOS)-1))
    {
        L4_DBG_MSG2("PIO trap: Invalid mask(0x%x) for bank (%d)", mask, bank);
        return (mask & ~(POFFM(NUMBER_OF_PIOS)-1));
    }
#else
    /* all masks are valid */
    UNUSED(mask);
#endif

    return 0;

}

static uint32 pio_check_params_and_not_owned(uint16 bank, uint32 mask)
{
    uint32 status;
    status = pio_check_params(bank, mask);
    if (status)
    {
        return status;
    }
    status = pio_get_not_owned(bank, mask);
    return status;
}

uint32 pio_ipc(pio_ipc_op op, uint16 bank, uint32 mask, uint32 value)
{
    IPC_PIO msg;

    if ((op != GET_UNUSED) && (mask == 0))
    {
        fault(FAULT_IPC_PIO_NO_EFFECT);
        return 0;
    }

    msg.bank = bank;
    msg.mask = mask;
    msg.value = value;
    switch(op)
    {
        case ACQUIRE:
            ipc_send_pio_acquire(&msg);
            break;
        case RELEASE:
            ipc_send_pio_release(&msg);
            break;
        case SET_OWNER:
            ipc_send_pio_set_owner(&msg);
            break;
        case GET_OWNER:
            ipc_send_pio_get_owner(&msg);
            break;
        case SET_PULLEN:
            ipc_send_pio_set_pull_en(&msg);
            break;
        case GET_PULLEN:
            ipc_send_pio_get_pull_en(&msg);
            break;
        case SET_PULLDIR:
            ipc_send_pio_set_pull_dir(&msg);
            break;
        case GET_PULLDIR:
            ipc_send_pio_get_pull_dir(&msg);
            break;
        case SET_PULLSTR:
            ipc_send_pio_set_pull_str(&msg);
            break;
        case GET_PULLSTR:
            ipc_send_pio_get_pull_str(&msg);
            break;
        case GET_UNUSED:
            ipc_send_pio_get_unused(&msg);
            break;
        case SET_PIO_MUX:
            ipc_send_pio_set_pio_mux(&msg);
            break;
        case GET_PIO_MUX:
            ipc_send_pio_get_pio_mux(&msg);
            break;
        case SET_PAD_MUX:
            ipc_send_pio_set_pad_mux(&msg);
            break;
        case GET_PAD_MUX:
            ipc_send_pio_get_pad_mux(&msg);
            break;
        case SET_DRIVE_STRENGTH:
            ipc_send_pio_set_drive_strength(&msg);
            break;
        case GET_DRIVE_STRENGTH:
            ipc_send_pio_get_drive_strength(&msg);
            break;
        case SET_STICKY:
            ipc_send_pio_set_sticky(&msg);
            break;
        case GET_STICKY:
            ipc_send_pio_get_sticky(&msg);
            break;
        case SET_SLEW:
            ipc_send_pio_set_slew(&msg);
            break;
        case GET_SLEW:
            ipc_send_pio_get_slew(&msg);
            break;
        case SET_XIO_MODE:
            ipc_send_pio_set_xio_mode(&msg);
            break;
        case GET_XIO_MODE:
            ipc_send_pio_get_xio_mode(&msg);
            break;
        case SET_DRIVE_ENABLE:
            ipc_send_pio_set_drive_enable(&msg);
            break;
        case GET_DRIVE_ENABLE:
            ipc_send_pio_get_drive_enable(&msg);
            break;
        case SET_DRIVE:
            ipc_send_pio_set_drive(&msg);
            break;
        case GET_DRIVE:
            ipc_send_pio_get_drive(&msg);
            break;
        case SET_FUNC_BITSERIAL:
            ipc_send_pio_set_func_bitserial(&msg);
            break;
        case SET_FUNC_UART:
            ipc_send_pio_set_func_uart(&msg);
            break;
        case SET_FUNC_CHARGER_COMMS:
            ipc_send_pio_set_func_charger_comms(&msg);
            break;
        default:
            panic_diatribe(PANIC_PIO_UNHANDLED_IPC_OP, op);
            /* no break */
    }
    return msg.value;
}


#endif /* DESKTOP_TEST_BUILD */

