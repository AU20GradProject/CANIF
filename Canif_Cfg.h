/*
 * Canif_Cfg.h
 *
 *  Created on: ??þ/??þ/????
 *      Author: MANSI-PC
 */

#ifndef CANIF_CFG_H_
#define CANIF_CFG_H_


/// number of hths referenced by canif
#define CANIF_NUM_HTHS                            NUM_OF_HTHS
/// number of driver units
///todo change this name to something from specificatoin
#define CANIF_NUM_DRIVER_UNITS                    1


/// Enabled version info
#define CANIF_PUBLIC_VERSION_INFO_API             STD_OFF
/// Enables the  CanIf_ReadRxPduData function
#define CANIF_PUBLIC_READRXPDU_DATA_API     STD_OFF
/// Enables the CanIf_CancelTransmit function
#define CANIF_CANCEL_TRANSMIT_SUPPORT       STD_OFF
/// Enables the CanIf_ReadTxNotifStatus function
#define CANIF_PUBLIC_READTXPDU_NOTIFY_STATUS_API    STD_ON
/// Enables the CanIf_ReadRxNotifStatus function
#define CANIF_PUBLIC_READRXPDU_NOTIFY_STATUS_API    STD_ON
/// Enables the CanIf_GetTxConfirmationState function
#define CANIF_PUBLIC_TXCONFIRM_POLLING_SUPPORT    STD_OFF
/// Enables the CanIf_SetDynamicTxId function
#define CANIF_PUBLIC_SETDYNAMICTXID_API            STD_ON
/// Enabled the CanIf_CancelTxConfirmation function
#define CANIF_CTRLDRV_TX_CANCELLATION                     STD_OFF
/// Enables the dlc check for received frames
#define CANIF_PRIVATE_DLC_CHECK                   STD_OFF
/// Selects type of software filtering, only binary search supported
#define CANIF_PRIVATE_SOFTWARE_FILTER_TYPE            BINARY
/// Datatype for hoh ids, uint8 or uint16
#define CANIF_PUBLIC_HANDLE_TYPE_ENUM                     uint16
/// Enables the tx buffering mechanism
#define CANIF_PUBLIC_TX_BUFFERING                           STD_OFF
/// Enables use of multiple can driver units
#define CANIF_PUBLIC_MULTIPLE_DRV_SUPPORT               STD_OFF // Not supported
/// Enables DET error reports
#define CANIF_DEV_ERROR_DETECT                        STD_OFF
/// Enabled the CanIf_CheckValidation function
#define CANIF_PUBLIC_WAKEUP_CHECK_VALIDATION_API  STD_OFF // not supported
/// Defines if a can controller has wakeup support
#define CANIF_CTRL_WAKEUP_SUPPORT                 STD_OFF // not supported
/// Defines if a can tranceiver has wakeup support
#define CANIF_TRCV_WAKEUP_SUPPORT                 STD_OFF // not supported

/*Sympolic names Generation for the control ID*/
#define CanIfCtrlId_1  (1U)
#define CanIfCtrlId_2  (2U)
#define CanIfCtrlId_3  (3U)
#define CanIfCtrlId_4  (4U)
#define CanIfCtrlId_5  (5U)
#define CanIfCtrlId_6  (6U)
#define CanIfCtrlId_NB (CanIfCtrlId_6+1)
/*
typedef struct
{
    const SoftwareFilterType CanIfPrivateSoftwareFilterType;
    #if (CanIfSupportTTCAN == TRUE)
    #endif

}CanIfPrivateCfg;
*/



typedef enum {
  CANIF_PDU_ID_PDU_RX0,
  CANIF_PDU_ID_PDU_functionalRX,
  CANIF_PDU_ID_PDU_UDSphysicalRX,
  CANIF_PDU_ID_PDU_InstrumentCtrl1,
  CANIF_PDU_ID_PDU_InstrumentCtrl2,
  CANIF_PDU_ID_PDU_SERVO1_PDO1_TX,
  CANIF_PDU_ID_PDU_SERVO1_PDO2_TX,
  CANIF_PDU_ID_PDU_InstrumentCtrl3,
  CANIF_PDU_ID_PDU_SERVO1_SDO_TX,
  CANIF_PDU_ID_PDU_SERVO1_BOOT_TX,
  CANIF_PDU_ID_PDU_IMU_GYRO,
  CANIF_PDU_ID_PDU_IMU_ACCELEROMETER,
  CANIF_PDU_ID_PDU_IMU_ANGLES,
    CANIF_NUM_RX_LPDU_ID
} CanIf_Arc_RxLPduIdType;

typedef enum {
    CANIF_TX_LPDU_ID_CanDB_Message_1,
    CANIF_TX_LPDU_ID_CanDB_Message_2,
    CANIF_TX_LPDU_ID_CanDB_Message_3,
    CANIF_NUMBER_OF_DYNAMIC_CANTXPDUIDS
} CanIf_Arc_DynamicTxLPduIdType;

typedef enum {
  CANIF_PDU_ID_PDU_UDSphysicalTX = CANIF_NUMBER_OF_DYNAMIC_CANTXPDUIDS,
  CANIF_PDU_ID_PDU_functionalTX,
  CANIF_PDU_ID_PDU_IcuStatus,
  CANIF_PDU_ID_PDU_Cfg,
  CANIF_PDU_ID_PDU_SensorData,
  CANIF_PDU_ID_PDU_SERVO1_PDO1_RX,
  CANIF_PDU_ID_PDU_SERVO1_PDO2_RX,
  CANIF_PDU_ID_PDU_NM_A_TX,
  CANIF_PDU_ID_PDU_SERVO1_SDO_RX,
  CANIF_PDU_ID_PDU_SERVO1_BOOT_RX,
    CANIF_NUM_TX_LPDU_ID
} CanIf_Arc_StaticTxLPduIdType;



typedef enum {
    CANIF_CanIfCtrlCfg,
    CANIF_CHANNEL_CNT
} CanIf_Arc_ChannelIdType;





typedef enum
{
    BINARY,
    INDEX,
    LINEAR,
    TABLE,
    MASK
}SoftwareFilterType ;

typedef enum {
    DYNAMIC,
    STATIC

} CanIfTxPduType;
typedef struct{


    boolean CanIfCtrlWakeupSupport;
  //  CanController* CanIfCtrlCanCtrlRef;

}CanIfCtrlCfg;
typedef struct
{
    uint8 CanIfPublicCddHeaderFile[32];

    Can_HwHandleType CanIfPublicHandleTypeEnum;
}CanIfPublicCfg;

typedef struct{

    CanIfCtrlCfg* CanIfHthCanCtrlIdRef;
   // CanHardwareObject* CanIfHthIdSymRef;

}CanIfHthCfg;
typedef enum {
    EXTENDED_CAN = 0,
    EXTENDED_FD_CAN,
    STANDARD_CAN,
    STANDARD_FD_CAN
} CanIfTxPduCanIdType;
typedef enum {
    CAN_NM ,
    CAN_TP,
    CAN_TSYN,
    CDD,
    J1939NM,
    J1939TP,
    PDUR,
    XCP
}CanIfTxPduUserTxConfirmationUL;

typedef struct{

    CanIfHthCfg* CanIfBufferHthRef;
    uint8 CanIfBufferSize;
}CanIfBufferCfg;


typedef struct {

      /** ECU wide unique, symbolic handle for transmit CAN L-PDU. The
           *  CanIfCanTxPduId is configurable at pre-compile and post-built time.
           *  Range: 0..max. number of CantTxPduIds   PduIdType   CanTxPduId; */

        PduIdType CanIfTxPduId; // pdu id  in can if
      /// upper layer pdu id passed to callout function


      /** CAN Identifier of transmit CAN L-PDUs used by the CAN Driver for CAN L-
       *  PDU transmission. Range: 11 Bit For Standard CAN Identifier ... 29 Bit For
       *  Extended CAN identifier */
        Can_IdType CanIfTxPduCanId; // i think it is used in the trasmit function as pduidtype ... pdu id in can
    /// can id used for transmission, msb indicates extended id



    /** Data length code (in bytes) of transmit CAN L-PDUs used by the CAN
     *  Driver for CAN L-PDU transmission. The data area size of a CAN L-Pdu
     *  can have a range from 0 to 8 bytes. */
    uint8       CanIfCanTxPduIdDlc;


    /** CAN Identifier of transmit CAN L-PDUs used by the CAN Driver for CAN L-
         *  PDU transmission.
         *  EXTENDED_CAN  The CANID is of type Extended (29 bits).
         *  STANDARD_CAN  The CANID is of type Standard (11 bits). */
    CanIfTxPduCanIdType CanIfTxPduCanIdType; // mainly will be exteded


#if ( CANIF_READTXPDU_NOTIFY_STATUS_API == STD_ON )
    /** Enables and disables transmit confirmation for each transmit CAN L-PDU
     *  for reading its notification status. True: Enabled False: Disabled */
    boolean     CanIfReadTxPduNotifyStatus;
#endif



    /** Defines the type of each transmit CAN L-PDU.
         *  DYNAMIC  CAN ID is defined at runtime.
         *  STATIC  CAN ID is defined at compile-time. */
    CanIfTxPduType CanIfTxPduType_i; //manly will be static




    boolean CanIfTxPduPnFilterPdu;      // not used

    boolean CanIfTxPduTriggerTransmit ; // in orded to notify

    uint8 CanIfTxPduUserTriggerTransmitName[32]; /// pdu trigger transmit name
    uint8 CanIfTxPduUserTxConfirmationName[32]; /// confirmation name to check
    CanIfTxPduUserTxConfirmationUL CanIfTxPduUserTxConfirmationUL_i;
//    CanIfBufferCfg* CanIfTxPduBufferRef;   // may not be used but we need the hth form inside
    PduInfoType* CanIfTxPduRef; //used in transmit



    /// upper layer confirmation function, set to null if no confirmation
    //void (*CanIfTxPduUserTxConfirmationName)(uint32);
   void(*user_TxConfirmation)(PduIdType txPduId);
    /// can driver hth id to be used for transmission
      Can_HwHandleType hth;
} CanIfTxPduCfg;



/*
typedef enum {
    CAN_NM ,
    CAN_TP,
    CAN_TSYN,
    CDD,
    J1939NM,
    J1939TP,
    PDUR,
    XCP
}CanIfRxPduUserRxConfirmationUL;
*/

typedef enum
{
    EXTENDED,
    STANDARD
}CanIfHrhRangeRxPduRangeCanIdType;


typedef struct {
    uint32 CanIfHrhRangeBaseId;
    uint32 CanIfHrhRangeMask;
    uint32 CanIfHrhRangeRxPduLowerCanId;
    CanIfHrhRangeRxPduRangeCanIdType CanIfHrhRangeRxPduRangeCanIdType_i;
    uint32 CanIfHrhRangeRxPduUpperCanId;

} CanIfHrhRangeCfg;


typedef struct {

    boolean CanIfHrhSoftwareFilter;
 //   const CanHardwareObject* CanIfHrhIdSymRef;
    CanIfCtrlCfg* CanIfHrhCanCtrlIdRef;
    CanIfHrhRangeCfg* CanIfHrhRangeConfig;
} CanIfHrhCfg;

typedef struct
{
    uint32 CanIfRxPduCanIdRangeLowerCanId;
    uint32 CanIfRxPduCanIdRangeUpperCanId;

}CanIfRxPduCanIdRange;
typedef struct {

    uint32 CanIfRxPduCanId;
    uint32 CanIfRxPduCanIdMask;
    uint16 CanIfRxPduDataLength;

    CanIfTxPduCanIdType CanIfRxPduCanIdType;

    uint16 CanIfRxPduId;
    boolean CanIfRxPduReadData;
    boolean CanIfRxPduReadNotifyStatus; // SET it TRUE WITH INIT

    uint8 CanIfRxPduUserRxIndicationName[32];
    //CanIfRxPduUserRxConfirmationUL CanIfRxPduUserRxIndicationUL_i;

    CanIfHrhCfg* CanIfRxPduHrhIdRef;
    uint8* CanIfRxPduRef;
    CanIfRxPduCanIdRange* CanIfRxPduCanIdRangePtr; //
}CanIfRxPduCfg;



/*
typedef struct
{
    uint8 CanIfDispatchUserCheckTrcvWakeFlagIndicationName;
    CanIfDispatchUserCheckTrcvWakeFlagIndicationUL CanIfDispatchUserCheckTrcvWakeFlagIndicationUL_i;
    uint8 CanIfDispatchUserClearTrcvWufFlagIndicationName ;
    CanIfDispatchUserClearTrcvWufFlagIndicationUL CanIfDispatchUserClearTrcvWufFlagIndicationUL_i;
    uint8 CanIfDispatchUserConfirmPnAvailabilityName;
    CanIfDispatchUserConfirmPnAvailabilityUL CanIfDispatchUserConfirmPnAvailabilityUL_i;
    uint8 CanIfDispatchUserCtrlBusOffName[32];
    CanIfDispatchUserCtrlBusOffUL =CanIfDispatchUserCtrlBusOffUL_i;
    uint8 CanIfDispatchUserCtrlModeIndicationName[32];
    CanIfDispatchUserCtrlModeIndicationUL = CanIfDispatchUserCtrlModeIndicationUL_i;
    uint8 CanIfDispatchUserTrcvModeIndicationName[32];
    CanIfDispatchUserTrcvModeIndicationUL =CanIfDispatchUserTrcvModeIndicationUL_i;
    uint8 CanIfDispatchUserValidateWakeupEventName[32];
    CanIfDispatchUserValidateWakeupEventUL = CanIfDispatchUserValidateWakeupEventUL_i;
}CanIfDispatchCfg;

*/
/*
typedef enum
{
    CAN_SM,
    CDD
}CanIfDispatchUserCheckTrcvWakeFlagIndicationUL;
*/
/*
typedef enum
{
    CAN_SM,
    CDD
}CanIfDispatchUserClearTrcvWufFlagIndicationUL;

typedef enum
{
    CAN_SM,
      CDD
}CanIfDispatchUserConfirmPnAvailabilityUL;
typedef enum
{
    CAN_SM,
      CDD
}CanIfDispatchUserCtrlBusOffUL;
typedef enum
{
    CAN_SM,
      CDD
}CanIfDispatchUserCtrlModeIndicationUL;
typedef enum
{
    CAN_SM,
      CDD
}CanIfDispatchUserTrcvModeIndicationUL;
typedef enum
{
      CDD,
      ECUM
}CanIfDispatchUserValidateWakeupEventUL;
*/

typedef struct
{
    //CanIfInitHohCfg* CanIfCtrlDrvInitHohConfigRef;
    //CanGeneral* CanIfCtrlDrvNameRef; //in candrv

}CanIfCtrlDrvCfg;



typedef struct{

    CanIfHrhCfg* CanIfHrhCfgPtr;
    CanIfHthCfg* CanIfHthCfgPtr;

}CanIfInitHohCfg;

typedef struct{

    uint8 CanIfInitCfgSet[32]; // not used
    uint64 CanIfMaxBufferSize;// max buffer size
    uint64 CanIfMaxRxPduCfg; // in order to  know the total number of rx pdu config
    uint64 CanIfMaxTxPduCfg; // in order to  know the total number of tx pdu config
    CanIfInitHohCfg* CanIfHohConfigPtr; //not used and not understood
    CanIfRxPduCfg*  CanIfRxPduConfigPtr; // may be array
    CanIfTxPduCfg* CanIfTxPduConfigPtr[]; //may be array
}CanIfInitCfg;
typedef struct {
              // CanIfCtrlDrvCfg  *CtrlDrvCfg;
               //CanIfDispatchCfg *CanIfDispatchCfg;
               CanIfInitCfg     *InitCfg;
             //  CanIfPrivateCfg  *PrivateCfg;
               CanIfPublicCfg   *PublicCfg;
               //CanIfTrcvDrvCfg  *TrcvDrvCfg;

} CanIf_ConfigType;

#endif /* CANIF_CFG_H_ */
