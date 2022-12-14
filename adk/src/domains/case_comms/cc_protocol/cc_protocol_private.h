/*!
\copyright  Copyright (c) 2020-2022 Qualcomm Technologies International, Ltd.\n
            All Rights Reserved.\n
            Qualcomm Technologies International, Ltd. Confidential and Proprietary.
\file       cc_protocol_private.h
\ingroup    cc_protocol
\brief      Private header for case comms protocol.
*/
/*! @{ */

#ifndef CC_PROTOCOL_PRIVATE_H
#define CC_PROTOCOL_PRIVATE_H

#ifdef INCLUDE_CASE_COMMS

/*! Definitions related to the case comms packet format. */
/*! @{ */
#define CASECOMMS_HEADER_LEN            (1)
#define CASECOMMS_CID_MASK              (0x70)
#define CASECOMMS_CID_BIT_OFFSET        (4)
#define CASECOMMS_MID_MASK              (0x0f)
#define CASECOMMS_MID_BIT_OFFSET        (0)
/*! @} */

#include "cc_protocol.h"
#include <domain_message.h>

/* Include all transports, however only one will be active
   and configured by project defines to include code. */
#include "cc_protocol_trans_schemeA.h"
#include "cc_protocol_trans_schemeB.h"
#include "cc_protocol_trans_test_uart.h"

#include <message.h>
#include <sink.h>

/*! \brief Case comms protocol task state. */
typedef struct
{
    /*! Case comms protocol task. */
    TaskData task;

    /*! Case or Earbud mode in which cc_protocol is operating. */
    cc_mode_t mode;

    /*! Type of chargercomms transport cc_protocol is using. */
    cc_trans_t trans;

    /*! Registered handler callbacks for each CID
      Low number of supported CIDs so no benefit to using
      dynamic storage, may need to change. */
    cc_chan_config_t channel_cfg[CASECOMMS_CID_MAX];

    /*! Identity of last earbud which was polled, used to balance
      poll requests. */
    cc_dev_t last_earbud_polled:2;

    /*! Timeout with which to delay transmission of a poll. */
    unsigned poll_timeout;

    /*! Running total of CHARGER_COMMS_UART_TX_FAILED received in response to TX
        used to trigger seqnum reset if reahed CcProtocol_ConfigNumFailsToReset. */
    uint8 tx_fail_count;

    /*! Transport specific data, each transport defines this type. */
    transport_scheme_data scheme_data;
    
    /*! Flags to influence the sleep decision. */
    uint16 cc_allow_dormant;
} cc_protocol_t;

/*! Casecomms dormant inhibition flags. */
typedef enum
{
    cc_comms_timeout_dorm = 1,
    cc_transport_dorm     = 2,
    cc_dormant_mask       = 3  /* OR of all possible dormant inhibitors */
} cc_dormant_inhibit_t;

/*! \brief Instruct the CC layer to prevent the app from entering dormant
    \note There is no corresponding function to allow dormant from this module
          as the prevention is expected to be used as a developer function when
          debugging with charger comms.
*/
void CcProtocol_TransportPreventDormant(void);

/*! Make the case comms protocol taskdata visible throughout the component. */
extern cc_protocol_t cc_protocol;

/*! Get pointer to case comms protocol taskdata. */
#define CcProtocol_GetTaskData()   (&cc_protocol)

/*! Get pointer to case comms protocol task. */
#define CcProtocol_GetTask()       (&cc_protocol.task)

/*! Casecomms protocol internal messages. */
typedef enum
{
    /*! Timer for polling left earbud. */
    CC_PROTOCOL_INTERNAL_POLL_LEFT_TIMEOUT = INTERNAL_MESSAGE_BASE,

    /*! Timer for polling right earbud. */
    CC_PROTOCOL_INTERNAL_POLL_RIGHT_TIMEOUT,
    
    /*! Prevent sleep after comms */
    CC_PROTOCOL_INTERNAL_TIMEOUT_PROHIBIT_SLEEP,
    
    /*! This must be the final message */
    CC_PROTOCOL_INTERNAL_MESSAGE_END
} cc_protocol_internal_message_t;
ASSERT_INTERNAL_MESSAGES_NOT_OVERFLOWED(CC_PROTOCOL_INTERNAL_MESSAGE_END)


/*! Definition of Earbud polling timeout message.
    \note Both CC_PROTOCOL_INTERNAL_POLL_LEFT_TIMEOUT and
          CC_PROTOCOL_INTERNAL_POLL_RIGHT_TIMEOUT use this
          message definition, the alternate MessageId permits
          identification of timers specific to left or right.
*/
typedef struct
{
    /*! Device to send the poll to, when the timer expires. */
    cc_dev_t dest;
} CC_PROTOCOL_INTERNAL_POLL_TIMEOUT_T;

/*! \brief Route incoming packet to client.
    \param pkt Pointer to packet.
    \param len Size of the packet in bytes.
    \param cod Channel ID.
    \param mid Message ID.
    \param source_dev Type of device that sent the packet.
 */
void ccProtocol_SendRXPacketToClient(const uint8* pkt, unsigned len, cc_cid_t cid, unsigned mid, cc_dev_t source_dev);

/*! \brief Check stream based transport for pending data.
    \param src Stream source handle.
*/
void ccProtocol_ProcessStreamSource(Source src);

/*! \brief Utility function to read Channel ID from a Case Comms header. */
cc_cid_t ccProtocol_CaseCommsGetCID(uint8 ccomms_header);

/*! \brief Utility function to set Channel ID in a Case Comms header. */
void ccProtocol_CaseCommsSetCID(uint8* ccomms_header, cc_cid_t cid);

/*! \brief Utility function to read Message ID from a Case Comms header. */
unsigned ccProtocol_CaseCommsGetMID(uint8 ccomms_header);

/*! \brief Utility function to set Message ID in a Case Comms header. */
void ccProtocol_CaseCommsSetMID(uint8* ccomms_header, unsigned mid);

#endif /* INCLUDE_CASE_COMMS */
#endif  /* CC_PROTOCOL_PRIVATE_H */
/*! @} End of group documentation */
