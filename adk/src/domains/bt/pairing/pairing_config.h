/*!
\copyright  Copyright (c) 2019 Qualcomm Technologies International, Ltd.\n
            All Rights Reserved.\n
            Qualcomm Technologies International, Ltd. Confidential and Proprietary.
\file       pairing_config.h
\brief      Configuration related definitions for the pairing task.
*/

#ifndef PAIRING_CONFIG_H_
#define PAIRING_CONFIG_H_


#ifdef CF133_BATT
/*! Minimum RSSI to pair with a device */
#define appConfigPeerPairingMinRssi() (-60)
#else
/*! Minimum RSSI to pair with a device */
#define appConfigPeerPairingMinRssi() (-50)
#endif

/*! Minimum difference in RSSI between devices discovered
    with the highest RSSI in order to pair */
#define appConfigPeerPairingMinRssiDelta() (10)

/*! Timeout in seconds for user initiated handset pairing */
#define appConfigHandsetPairingTimeout()    (120)

/*! Timeout in seconds for authentication 

    \note This can include user interaction */
#define appConfigAuthenticationTimeout()       (45)

/*! Timeout in seconds for automatic handset pairing */
#define appConfigAutoHandsetPairingTimeout()    (300)

/*! Timeout in seconds to disable le pairing after br/edr pairing */
#define appConfigLePairingDisableTimeout()      (60)

/*! Key ID peer Earbud link-key derivation */
#define appConfigTwsKeyId()       (0x74777332UL)

/*! Timeout in milliseconds to wait for CTKD over BR/EDR-SM, after BR/EDR pairing has completed */
#define appConfigSmpCtkdTimeout()               (500)

#endif /* PAIRING_CONFIG_H_ */
