/****************************************************************************
Copyright (c) 2004 - 2022 Qualcomm Technologies International, Ltd.


FILE NAME
    init.c        

DESCRIPTION
	Connection library initialization		

NOTES

*/


/****************************************************************************
	Header files
*/
#include    "connection.h"
#include    "connection_private.h"
#include    "bluestack_handler.h"
#include    "init.h"
#include    "dm_init.h"
#include    "dm_security_init.h"
#include    "connection_tdl.h"
#include    "rfc_init.h"
#include    "l2cap_init.h"
#include    "sdp_init.h"
#include    "tcp_init.h"
#include    "udp_init.h"
#include    "vm.h"
#include    <print.h>
#include    <logging.h>


/* Make the type used for message IDs available in debug tools */
LOGGING_PRESERVE_MESSAGE_TYPE(ConnectionMessageId)
LOGGING_PRESERVE_MESSAGE_TYPE(CL_INTERNAL_T)

/*lint -e655 */

/****************************************************************************

	Local

*/

/* Enforce static linkage on the Connection Library state instance to ensure
   that the Connection Library state can only be modified from within the task
   handler.

*/
static connectionState		theCm;

const msg_filter defaultMsgFilter = {msg_group_acl};

/****************************************************************************

DESCRIPTION
    Initialize the connection library locks
*/
static void initLocks(void)
{
	/* Init the locks */
#ifndef CL_EXCLUDE_INQUIRY
	theCm.inqState.inquiryLock = 0;
#endif
	theCm.smState.setSecurityModeLock = 0;
	theCm.smState.authReqLock = 0;
	theCm.smState.encryptReqLock = 0;
	theCm.smState.deviceReqLock = 0;
	theCm.smState.sink = 0;
    theCm.smState.permanent_taddr = 0;
	theCm.infoState.stateInfoLock = 0;
    theCm.infoState.sink = 0;
#ifndef CL_EXCLUDE_SDP
	theCm.sdpState.sdpLock = 0;
	theCm.sdpState.sdpSearchLock = 0;
#endif
    theCm.l2capState.mapLock = 0;
    theCm.readTxPwrState.txPwrLock = 0;
    theCm.bleScanAdState.bleScanAdLock = 0;
    theCm.bleReadRdnAddrState.bleReadRndAddrLock = 0;
    theCm.configAddrType = 0xFFFF;
}


/****************************************************************************
NAME	
    connectionGetCmTask

DESCRIPTION
    This function returns the connection library task so that the connection
    library can post a message to itself.

RETURNS
    The connection library task.
*/
Task connectionGetCmTask(void)
{
    return &theCm.task;
}

/****************************************************************************
NAME	
    connectionGetBtVersion

DESCRIPTION
    Returns the BT Version read from BlueStack during initialization.

RETURNS
    cl_dm_bt_version
*/

cl_dm_bt_version connectionGetBtVersion(void)
{
    return theCm.infoState.version;
}

/****************************************************************************
NAME	
    connectionGetAppTask

DESCRIPTION
    This function returns the application task.

RETURNS
    The application task.
*/
Task connectionGetAppTask(void)
{
    return theCm.theAppTask;
}


/****************************************************************************
NAME	
    connectionGetMsgFilter

DESCRIPTION
    This function returns the connection library message filter.

RETURNS
    The connection library message filter.
*/
const msg_filter *connectionGetMsgFilter(void)
{
	return theCm.msgFilter;
}

/****************************************************************************
NAME
    connectionGetInitState

DESCRIPTION
    Get the initialization status of the Connection library

RETURNS
    TRUE if library initialized
*/
bool connectionGetInitState( void )
{
    return theCm.state == connectionReady;
}

/****************************************************************************
NAME
    connectionGetLockState

DESCRIPTION
    Get the system lock state of the Connection library

RETURNS
    FALSE if no locks held
*/
bool connectionGetLockState( void )
{
    return (
        theCm.inqState.inquiryLock ||
        theCm.inqState.iacLock ||
        theCm.inqState.nameLock ||
        theCm.smState.setSecurityModeLock ||
        theCm.smState.authReqLock ||
        theCm.smState.encryptReqLock ||
        theCm.smState.deviceReqLock ||
        theCm.smState.permanent_taddr ||
        theCm.infoState.stateInfoLock ||
        theCm.sdpState.sdpLock ||
        theCm.sdpState.sdpSearchLock ||
        theCm.l2capState.mapLock ||
        theCm.readTxPwrState.txPwrLock );
}

/*****************************************************************************/
uint16 connectionOutstandingWriteScanEnableReqsGet(void)
{
    return theCm.outstandingWriteScanEnableReqs;
}

/*****************************************************************************/
void connectionOutstandingWriteScanEnableReqsSetDelta(signed delta)
{
    signed reqs = (signed)theCm.outstandingWriteScanEnableReqs + delta;
    PanicFalse(reqs >= 0);
    theCm.outstandingWriteScanEnableReqs = (uint16)reqs;
}


/*****************************************************************************/
void ConnectionInit(Task theAppTask  )
{
    ConnectionInitEx3(theAppTask, &defaultMsgFilter , DEFAULT_NO_DEVICES_TO_MANAGE_CLASSIC , CONNLIB_OPTIONS_NONE );
}

/*****************************************************************************/
void ConnectionInitEx(Task theAppTask, const msg_filter *msgFilter  )
{
    ConnectionInitEx3(theAppTask, msgFilter , DEFAULT_NO_DEVICES_TO_MANAGE_CLASSIC , CONNLIB_OPTIONS_NONE );
}

/*****************************************************************************/
void ConnectionInitEx2(Task theAppTask, const msg_filter *msgFilter , uint16 TdlNumberOfDevices  )
{
    ConnectionInitEx3(theAppTask, msgFilter , TdlNumberOfDevices , CONNLIB_OPTIONS_NONE);
}

/*****************************************************************************/
void ConnectionInitEx3(Task theAppTask, const msg_filter *msgFilter , uint16 TdlNumberOfDevices  , uint16 options)
{
    theCm.msgFilter = (msgFilter == NULL) ? &defaultMsgFilter : msgFilter;
    
	/* Initialize the Connection Library Task, all upstream messages sent by
       Bluestack will be handled by this task */
    theCm.task.handler = connectionBluestackHandler;
    
	/* If a task is already registered to receive BlueStack prims then we panic! */
	if (MessageBlueStackTask(connectionGetCmTask()))
	{
		CL_DEBUG(("ERROR - task already registered\n"));
	}

	/* Init the resource locks */
	initLocks();

    /* Init the sm_init_msg types.*/
    theCm.smState.sm_init_msg = sm_init_set_none;

    /* Store the application task */
    theCm.theAppTask = theAppTask;
    
#ifndef DISABLE_CSB
    /* Init the csbTask */
    theCm.csbTask = (Task)NULL;
#endif   

    /*set the number of devices to the requested value if in the range 1 to MAX_NO_DEVICES_TO_MANAGE */
    if ((TdlNumberOfDevices >= MIN_NO_DEVICES_TO_MANAGE) && 
        (TdlNumberOfDevices <= MAX_NO_DEVICES_TO_MANAGE))
    {
        theCm.smState.TdlNumberOfDevices = TdlNumberOfDevices ;
    }
    else
    {
        /* Supplied value out of range, set default number of TDL Devices to Max */
        theCm.smState.TdlNumberOfDevices = MAX_NO_DEVICES_TO_MANAGE;
    }

    /* Process options */

    /* Enable SC */
    if (options & CONNLIB_OPTIONS_SC_ENABLE)
        theCm.flags |= CONNECTION_FLAG_SC_ENABLE;

    /* Enable SC only mode. It implies SC must be turned on as well */
    if (options & CONNLIB_OPTIONS_SCOM_ENABLE)
        theCm.flags |= (CONNECTION_FLAG_SCOM_ENABLE | CONNECTION_FLAG_SC_ENABLE);

    /* Disable Cross Transport Key Derivation during Pairing. */
    if (options & CONNLIB_OPTIONS_CTKD_DISABLE)
        theCm.flags |= CONNECTION_FLAG_CTKD_DISABLE;

    /* Enable Selective Cross Transport Key Derivatoin during Pairing. */
    if (options & CONNLIB_OPTIONS_SELECTIVE_CTKD)
        theCm.flags |= CONNECTION_FLAG_SELECTIVE_CTKD;
    
    /* Start the initialization process */
    MessageSend(connectionGetCmTask(), CL_INTERNAL_INIT_REQ, NO_PAYLOAD);
}


/****************************************************************************
NAME	
	connectionHandleInternalInit	

DESCRIPTION
	This function is called to control the initialization process.  To avoid race
	conditions at initialization, the process is serialized.

RETURNS

*/
void connectionHandleInternalInit(connectionInitState state)
{
    /* If we're ready to run, change state */    
    if(state == connectionInitComplete)
    {
        theCm.state = connectionReady;
    }
    else if (theCm.state != connectionInitialising)
    {
        theCm.state = connectionInitialising;

        /* Start a Timer to notify the Client if the initialization fails */
        MessageSendLater(&theCm.task, CL_INTERNAL_INIT_TIMEOUT_IND, NO_PAYLOAD, (uint32) INIT_TIMEOUT);
    }

    /* Check to see if all objects have been initialized */
    if(state == connectionInitComplete)
    {
		/* Initialize auth requirements to unknown */
		theCm.smState.authentication_requirements = AUTH_REQ_UNKNOWN;
			
        /* Some DM stuff can be initialized only after the DM register has happened so do it here */
        connectionDmInfoInit();

        /* Let the application we're ready to go */
        connectionSendInitCfm(theCm.theAppTask, success, theCm.infoState.version);
    }
    else
    {
        /* Depending upon the previous object initialized, initialize the next one */
        switch(state)
        {
            case connectionInit:
                connectionDmInit(); 
                break;

            case connectionInitDm:
#ifndef CL_EXCLUDE_RFCOMM
                connectionRfcInit();	
#else
                connectionL2capInit();
#endif
                break;

#ifndef CL_EXCLUDE_RFCOMM
            case connectionInitRfc:
                connectionL2capInit();
                break;
#endif

            case connectionInitL2cap:
                connectionUdpInit();
                break;

            case connectionInitUdp:
                connectionTcpInit();
                break;

            case connectionInitTcp:
                connectionSdpInit(&theCm.sdpState);
                break;

            case connectionInitSdp: 
				connectionVersionInit();
				break;
				
			case connectionInitVer:
                connectionSmInit(theCm.infoState.version,
                                 &theCm.smState,
                                 theCm.flags);
                break;

            case connectionInitSm:
                theCm.smState.noDevices = connectionAuthInit();
                break;

            case connectionInitComplete:
                /* We're ready! */                
            default:
                break;
        }
    }
}


/****************************************************************************
NAME	
	connectionSendInternalInitCfm	

DESCRIPTION
	This function is called to send a CL_INTERNAL_INIT_CFM message to the 
	Connection Library task
*/
void connectionSendInternalInitCfm(connectionInitState state)
{
    MAKE_CL_MESSAGE(CL_INTERNAL_INIT_CFM);
    message->state = state;
    MessageSend(connectionGetCmTask(), CL_INTERNAL_INIT_CFM, message);
}


/****************************************************************************
NAME	
	connectionSendInitCfm

DESCRIPTION
	This function is called from the main Connection Library task handler to 
	indicate to the Client application the result of the request to initialize
	the Connection Library
*/
void connectionSendInitCfm(Task task, connection_lib_status status, cl_dm_bt_version version)
{    
    MAKE_CL_MESSAGE(CL_INIT_CFM);
    message->status = status;
	message->version = version;
    MessageSend(task, CL_INIT_CFM, message);

    /* Cancel initialization timeout */
    if(status == success)
    {
        (void) MessageCancelFirst(connectionGetCmTask(), CL_INTERNAL_INIT_TIMEOUT_IND);
    }
}


#ifndef DISABLE_CSB
/****************************************************************************
NAME    
    ConnectionMessageCsbTask

DESCRIPTION
    Register a task to handle CSB-related messages.

RETURNS
    Previously registered task or NULL if none.
*/
Task ConnectionCsbRegisterTask(Task csbTask)
{
    Task existing_task = theCm.csbTask;

    theCm.csbTask = csbTask;
    return existing_task;
}

/****************************************************************************
NAME    
    connectionGetCsbTask

DESCRIPTION
    This function returns the application task registered for CSB-related
    messages.
*/
Task connectionGetCsbTask(void)
{
    return theCm.csbTask;
}
#endif

/****************************************************************************
NAME
    ConnectionScanEnableRegisterTask

DESCRIPTION
    Register a task to handle scan enable messages.

RETURNS
    Previously registered task or NULL if none.
*/
Task ConnectionScanEnableRegisterTask(Task scanEnableTask)
{
    Task existing_task = theCm.scanEnableTask;
    theCm.scanEnableTask = scanEnableTask;
    return existing_task;
}

/****************************************************************************
NAME
    connectionGetScanEnableTask

DESCRIPTION
    This function returns the application task registered for scan enable
    messages.
*/
Task connectionGetScanEnableTask(void)
{
    return theCm.scanEnableTask;
}

/*lint +e655 */
