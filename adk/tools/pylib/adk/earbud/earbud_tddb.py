############################################################################
# CONFIDENTIAL
#
# Copyright (c) 2021-22 Qualcomm Technologies International, Ltd.
#   %%version
#
############################################################################

from csr.dev.fw.firmware_component import FirmwareComponent
from csr.dev.model import interface
from csr.dev.fw.psflash import Psflash

class EarbudTrustedDeviceList(FirmwareComponent):
    ''' Class providing trusted device list summary information about the Earbud application.
    '''
    TDDB_LIST_MAX = 8
    # For Extended TD DB layout
    EXT_TDDB_LIST_MAX = 12

    TDDB_KEYCOUNT_SC = 2
    TDDB_KEYCOUNT_GATT = 1
    TDDB_KEYCOUNT_APP = 1

    PSKEY_CONNLIB_BASE = 0x260e
    PSKEY_TDDB_SYSTEM = PSKEY_CONNLIB_BASE
    PSKEY_TDDB_INDEX = PSKEY_TDDB_SYSTEM + 1
    PSKEY_TDDB_BASE = PSKEY_TDDB_INDEX + 1

    PSKEY_TDDB_BASE_SC = PSKEY_TDDB_BASE + (2 * TDDB_LIST_MAX)
    PSKEY_TDDB_BASE_GATT = PSKEY_TDDB_BASE_SC + (TDDB_KEYCOUNT_SC * TDDB_LIST_MAX)
    PSKEY_TDDB_BASE_APP = PSKEY_TDDB_BASE_GATT + (TDDB_KEYCOUNT_GATT * TDDB_LIST_MAX)
    # For Extended TD DB layout
    PSKEY_EXT_TDDB_BASE_SC = PSKEY_TDDB_BASE
    PSKEY_EXT_TDDB_BASE_GATT = PSKEY_EXT_TDDB_BASE_SC + (TDDB_KEYCOUNT_SC * EXT_TDDB_LIST_MAX)
    PSKEY_EXT_TDDB_BASE_APP = PSKEY_EXT_TDDB_BASE_GATT + (TDDB_KEYCOUNT_GATT * EXT_TDDB_LIST_MAX)

    TDDB_FILE_BASE = 0xFFFF0000
    TDDB_VER_BIT_LE  = (1 << 0)
    TDDB_VER_BIT_SIGN = (1 << 1)
    TDDB_VER_BIT_PRIVACY = (1 << 2)
    TDDB_VER_BIT_EXTENDED_TD_DB = (1 << 3)
    TDDB_VER_BIT_CACHING = (1 << 4)

    PSKEY_TRUSTED_DEVICE_LIST = TRUSTED_DEVICE_INDEX = PSKEY_CONNLIB_BASE + 1
    TRUSTED_DEVICE_LIST = TRUSTED_DEVICE_INDEX

    TDDB_SYSTEM_OFFSET_VERSION = 0
    TDDB_SYSTEM_OFFSET_ER = 4
    TDDB_SYSTEM_OFFSET_IR = 20
    TDDB_SYSTEM_OFFSET_DIV = 36
    TDDB_SYSTEM_OFFSET_HASH = 40

    TDDB_INDEX_OFFSET_COUNT = 0
    TDDB_INDEX_OFFSET_ELEM = 4
    TDDB_INDEX_ELEM_OFFSET_ADDR = 0
    TDDB_INDEX_ELEM_OFFSET_ADDR_TYPE = 8
    TDDB_INDEX_ELEM_OFFSET_RANK = 9
    TDDB_INDEX_ELEM_OFFSET_PRIORITY = 10

    TDDB_INDEX_ELEM_ADDR_OFFSET_LAP = 0
    TDDB_INDEX_ELEM_ADDR_OFFSET_UAP = 4
    TDDB_INDEX_ELEM_ADDR_OFFSET_NAP = 6
    TDDB_INDEX_ELEM_SIZE = 12
    # For Extended TD DB layout
    EXT_TDDB_INDEX_OFFSET_ELEM = 2
    EXT_TDDB_INDEX_ELEM_ADDR_OFFSET_LAP = 4
    EXT_TDDB_INDEX_ELEM_ADDR_OFFSET_UAP = 2
    EXT_TDDB_INDEX_ELEM_ADDR_OFFSET_NAP = 0
    EXT_TDDB_INDEX_ELEM_ADDR_OFFSET_ATTR = 6
    EXT_TDDB_INDEX_ELEM_SIZE = 8

    TDDB_BREDRKEY_OFFSET_TYPE = 0
    TDDB_BREDRKEY_OFFSET_LEN = 1
    TDDB_BREDRKEY_OFFSET_LINKKEY = 2
    TDDB_BREDRKEY_OFFSET_AUTHORISED = 18

    TDDB_LEKEY_OFFSET_SECREQ = 2
    TDDB_LEKEY_OFFSET_ENC = 4
    TDDB_LEKEY_OFFSET_DIV = 30
    TDDB_LEKEY_OFFSET_ID = 32
    TDDB_LEKEY_OFFSET_SIGN = 48

    TDDB_LEKEY_ENCCENTRAL_OFFSET_EDIV = 0
    TDDB_LEKEY_ENCCENTRAL_OFFSET_RAND = 2
    TDDB_LEKEY_ENCCENTRAL_OFFSET_LTK = 10

    KEY_TYPE_AUTH_P192 = 5
    KEY_TYPE_UNAUTH_P256 = 7
    KEY_TYPE_AUTH_P256 = 8

    def __init__(self, env, core, parent=None):
        FirmwareComponent.__init__(self, env, core, parent=parent)

        self._psflash = Psflash(env, core)
        try:
            # Check for Synergy ADK build
            env.vars["synergy_service_inst"]
        except KeyError:
            self._is_synergyadk = False
        else:
            self._is_synergyadk = True


    def _psread(self, ps_key):
        """ Read PS Keys value from PsFlash based on ps_key."""
        key_value = str(self._psflash.keys(show_data=False, report=True ,keys=ps_key))
        # Parse the return string for PS Keys value
        key_value = key_value[key_value.find(':') + 1:]
        key_value = key_value.split()
        output = [int(x,16) for x in key_value]
        return output

    @property
    def tddb_list_max(self):
        try:
            self._tddb_list_max
        except AttributeError:
            self._parse_system_info()
        return self._tddb_list_max

    @property
    def extended_td_db(self):
        try:
            self._extended_td_db
        except AttributeError:
            self._parse_system_info()
        return self._extended_td_db

    @property
    def pskey_tddb_base_sc(self):
        try:
            self._pskey_tddb_base_sc
        except AttributeError:
            self._parse_system_info()
        return self._pskey_tddb_base_sc

    def _parse_system_info(self):
        """
        Get the CONNLIB0 data from PS and check for TDDB_VER_BIT_EXTENDED_TD_DB bitmask value in file version
        to set self._extended_td_db flag and self._tddb_list_max value accordingly.
        :return: None
        """
        system = self._psread(self.PSKEY_TDDB_SYSTEM)
        # Match the database version
        version = system[self.TDDB_SYSTEM_OFFSET_VERSION] + (system[self.TDDB_SYSTEM_OFFSET_VERSION + 1] << 16)
        if version & self.TDDB_VER_BIT_EXTENDED_TD_DB:
            self._extended_td_db = True
            self._tddb_list_max = self.EXT_TDDB_LIST_MAX
            self._pskey_tddb_base_sc = self.PSKEY_EXT_TDDB_BASE_SC
        else:
            self._extended_td_db = False
            self._tddb_list_max = self.TDDB_LIST_MAX
            self._pskey_tddb_base_sc = self.PSKEY_TDDB_BASE_SC

        # Extract system keys
        er = system[self.TDDB_SYSTEM_OFFSET_ER : self.TDDB_SYSTEM_OFFSET_ER + 8]
        ir = system[self.TDDB_SYSTEM_OFFSET_IR : self.TDDB_SYSTEM_OFFSET_IR + 8]
        div = system[self.TDDB_SYSTEM_OFFSET_DIV]
        hash = system[self.TDDB_SYSTEM_OFFSET_HASH : self.TDDB_SYSTEM_OFFSET_HASH + 8]

    def parse_tddb_list(self):
        """
        Get the CONNLIB1 data from PS that describes the number of devices in trusted device
        database and device info [address,rank,priority] of each devices.
        :return: device count, list of devices with device info
        """
        devices = [None] * self.tddb_list_max
        index = self._psread(self.PSKEY_TDDB_INDEX)
        # Number of devices present in Trusted Device database in PS
        device_count = index[self.TDDB_INDEX_OFFSET_COUNT]
        # Retrieve device address, rank and priority for the Trusted devices
        indexElemBase = 0
        if self.extended_td_db is True:
            indexElemBase = self.EXT_TDDB_INDEX_OFFSET_ELEM
        else:
            indexElemBase = self.TDDB_INDEX_OFFSET_ELEM

        for i in range(self.tddb_list_max):
            if self.extended_td_db is True:
                indexElem = index[indexElemBase : indexElemBase + self.EXT_TDDB_INDEX_ELEM_SIZE]
                rank = (indexElem[self.EXT_TDDB_INDEX_ELEM_ADDR_OFFSET_ATTR] & 0x3c) >> 2 # bit mask value: bit 2 - bit 5 represents the rank of device 
            else:
                indexElem = index[indexElemBase : indexElemBase + self.TDDB_INDEX_ELEM_SIZE]
                rank = indexElem[self.TDDB_INDEX_ELEM_OFFSET_RANK]

            if rank < device_count and rank >= 0:
                device = {}
                addr = {}

                if self.extended_td_db is True:
                    addr["lap"] = (indexElem[self.EXT_TDDB_INDEX_ELEM_ADDR_OFFSET_LAP] +
                              (indexElem[self.EXT_TDDB_INDEX_ELEM_ADDR_OFFSET_LAP + 1] << 8) +
                              (indexElem[self.EXT_TDDB_INDEX_ELEM_ADDR_OFFSET_UAP] << 16))
                    addr["uap"] = indexElem[self.EXT_TDDB_INDEX_ELEM_ADDR_OFFSET_UAP + 1]
                    addr["nap"] = (indexElem[self.EXT_TDDB_INDEX_ELEM_ADDR_OFFSET_NAP] +
                              (indexElem[self.EXT_TDDB_INDEX_ELEM_ADDR_OFFSET_NAP + 1] << 8))
                    addr["type"] = indexElem[self.EXT_TDDB_INDEX_ELEM_ADDR_OFFSET_ATTR] & 0x03    # bit mask value: bit 0 - bit 1 represents the address type of device
                    priority = (indexElem[self.EXT_TDDB_INDEX_ELEM_ADDR_OFFSET_ATTR] & 0x40) >> 6 # bit mask value: bit 6 represents the priority of device
                else:
                    addr["lap"] = (indexElem[self.TDDB_INDEX_ELEM_ADDR_OFFSET_LAP] +
                              (indexElem[self.TDDB_INDEX_ELEM_ADDR_OFFSET_LAP + 1] << 8) +
                              (indexElem[self.TDDB_INDEX_ELEM_ADDR_OFFSET_LAP + 2] << 16))
                    addr["uap"] = indexElem[self.TDDB_INDEX_ELEM_ADDR_OFFSET_UAP]
                    addr["nap"] = (indexElem[self.TDDB_INDEX_ELEM_ADDR_OFFSET_NAP] +
                              (indexElem[self.TDDB_INDEX_ELEM_ADDR_OFFSET_NAP + 1] << 8))
                    addr["type"] = indexElem[self.TDDB_INDEX_ELEM_OFFSET_ADDR_TYPE]
                    priority = indexElem[self.TDDB_INDEX_ELEM_OFFSET_PRIORITY]

                device["addr"] = addr
                device["rank"] = rank
                device["priority"] = priority
                device["index"] = i  # record this item's actual position in list, so item's correct bredr_key & le_key can be referenced

                devices[i] = device

            if self.extended_td_db is True:
                indexElemBase = indexElemBase + self.EXT_TDDB_INDEX_ELEM_SIZE
            else:
                indexElemBase = indexElemBase + self.TDDB_INDEX_ELEM_SIZE

        devices = list(filter(None, devices))

        return device_count, devices


    def _swap_adjacent_elem(self, link_key):
        """Method to swap the adjacent elements present in the list."""
        link_key[::2], link_key[1::2] = link_key[1::2], link_key[::2]
        return link_key

    def _extract_bredr_key(self, devices):
        """Method to extract the bredr information for each of the devices in the list."""
        for device in devices:
            bredr = {}
            ps_key = self.pskey_tddb_base_sc + device["index"]
            key_val = self._psread(ps_key)

            if key_val is not None and len(key_val) > 0:
                linkKeyType = key_val[self.TDDB_BREDRKEY_OFFSET_TYPE] & 0xFF
                if linkKeyType != 0:
                    linkKeyLen = key_val[self.TDDB_BREDRKEY_OFFSET_LEN]
                    linkKey = key_val[self.TDDB_BREDRKEY_OFFSET_LINKKEY + 15 :
                        self.TDDB_BREDRKEY_OFFSET_LINKKEY -1 : -1]
                    authorised = key_val[self.TDDB_BREDRKEY_OFFSET_AUTHORISED]

                    bredr["type"] = linkKeyType
                    bredr["len"] = linkKeyLen
                    bredr["linkkey"] = linkKey
                    bredr["authorised"] = authorised
                    device["bredr"] = bredr


    def _extract_le_key(self, devices):
        """Method to extract the Le key information for each of the devices in the list."""
        for device in devices:
            le = {}
            ps_key = self.pskey_tddb_base_sc + self.tddb_list_max + device["index"]
            key_val = self._psread(ps_key)

            if key_val is not None and len(key_val) > 0:
                keyValid = key_val[0] & 0xFF

                if keyValid != 0:
                    le["secReq"] = key_val[self.TDDB_LEKEY_OFFSET_SECREQ]

                    if keyValid & 0x01:
                        encCentral = key_val[self.TDDB_LEKEY_OFFSET_ENC:self.TDDB_LEKEY_OFFSET_DIV]

                        le["ediv"] = (encCentral[self.TDDB_LEKEY_ENCCENTRAL_OFFSET_EDIV] +
                            (encCentral[self.TDDB_LEKEY_ENCCENTRAL_OFFSET_EDIV + 1] << 8))
                        le["rand"] = encCentral[self.TDDB_LEKEY_ENCCENTRAL_OFFSET_RAND + 7:
                            self.TDDB_LEKEY_ENCCENTRAL_OFFSET_RAND -1 : -1]
                        le["ltk"] = encCentral[self.TDDB_LEKEY_ENCCENTRAL_OFFSET_LTK + 15 :
                            self.TDDB_LEKEY_ENCCENTRAL_OFFSET_LTK -1 : -1]
                        le["keySize"] = key_val[1]

                    if keyValid & 0x02:
                        le["id"] = key_val[self.TDDB_LEKEY_OFFSET_SIGN -1 :
                            self.TDDB_LEKEY_OFFSET_ID -1 : -1]

                    if keyValid & 0x04:
                        sign = key_val[self.TDDB_LEKEY_OFFSET_SIGN : -1]
                        le["csrk"] = self._swap_adjacent_elem(sign[ : 16])
                        le["counter"] = (sign[8] + (sign[9] << 8) +
                            (sign[10] << 16) + (sign[11] << 24))

                    if keyValid & 0x08:
                        le["div"] = key_val[self.TDDB_LEKEY_OFFSET_DIV]

                    if keyValid & 0x10:
                        le["rpaOnly"] = True
                    else:
                        le["rpaOnly"] = False
                    device["le"] = le

    def _getDeviceStrData(self, device):
        """Method that returns a string with all the printable values for the device."""
        if "bredr" in device:
            bredr = device["bredr"]
            key_type = bredr["type"]

            mitm = key_type in (self.KEY_TYPE_AUTH_P192,self.KEY_TYPE_AUTH_P256)
            sc = key_type in (self.KEY_TYPE_UNAUTH_P256,self.KEY_TYPE_AUTH_P256)

            result = "".join(["Trusted device : ", "Yes" if bredr["authorised"] else "No","\n"])
            result = "".join([result, "Encryption key size : ", str(bredr["len"]),"\n"])
            result = "".join([result, "MITM protection : ", "Yes" if mitm else "No","\n"])
            result = "".join([result, "Secure connections : ", "Yes" if sc else "No","\n"])
            result = "".join([result, "LE Bonding : ", "Yes" if "le" in device else "No","\n"])
            result = "".join([result, "Priority device : ", "Yes" if device["priority"] else "No","\n"])

            if key_type:
                header = "BR/EDR key ({})".format(bredr["type"])
                value = "".join(["{:02X}".format(n) for n in bredr["linkkey"]])
                result = "".join([result, header, " : ", value,"\n"])

        if "le" in device:
            le = device["le"]

            if "ltk" in le:
                header = "LE central keys"

                result = "".join([result, header, "\n"])
                result = "".join([result, "\tEDIV : ", "".join(["{:04X}".format(le["ediv"]), "\n"])])
                result = "".join([result, "\tRAND : ", "".join(["{:02X}".format(n) for n in le["rand"]]), "\n"])
                result = "".join([result, "\tLTK : ", "".join(["{:02X}".format(n) for n in le["ltk"]]), "\n"])

            if "div" in le:
                result = "".join([result, "Diversifier : ", "{:02X}".format(le["div"]), "\n"])

            if "id" in le:
                result = "".join([result, "ID IRK : ", "".join(["{:02X}".format(n) for n in le["id"]]), "\n"])

        return result


    def _generate_report_body_elements(self):
        if self._is_synergyadk:
            grp = interface.Group("Summary")
            # Parse System Info in PS to check the file version to know the TD DB layout present
            self._parse_system_info()

            device_count, devices = self.parse_tddb_list()

            if device_count != len(devices):
                tbl = interface.Error("Trusted device database is corrupted : Device count is {} "
                    "and number of devices in database is {}.\n".format(device_count,len(devices)))
                grp.append(tbl)

            keys_in_summary = ['bdaddr', 'value']
            tbl = interface.Table(keys_in_summary)

            self._extract_bredr_key(devices)
            self._extract_le_key(devices)
            for rank in range(self.tddb_list_max):
                for device in devices:
                    if device["rank"] == rank:
                        bdaddr = "{:04X}:{:02X}:{:06X}".format(device["addr"]["nap"],
                            device["addr"]["uap"], device["addr"]["lap"])
                        tbl.add_row([bdaddr,self._getDeviceStrData(device)])

            grp.append(tbl)
        else:
            grp = interface.Group("Note :- Supported only for Synergy ADK build")

        return [grp]
