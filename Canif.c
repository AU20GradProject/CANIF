/*
 * Canif.c
 *
 *  Created on: ??þ/??þ/????
 *      Author: MANSI-PC
 */

#include <Canif.h>
#include <Canif_Cbk.h>




//CanIfCtrlCanCtrlRef channel =(CanIfCtrlCanCtrlRef)Controller; //control is the input of init function

/* ----------------------------[private macro]-------------------------------*/



#if ( CANIF_DEV_ERROR_DETECT == STD_ON )
#include "Det.h"
#define VALIDATE_RV(_exp,_api,_err ) \
        if( !(_exp) ) { \
          Det_ReportError(MODULE_ID_CANIF,0,_api,_err); \
          return E_NOT_OK; \
        }

#define VALIDATE_NO_RV(_exp,_api,_err ) \
        if( !(_exp) ) { \
          Det_ReportError(MODULE_ID_CANIF,0,_api,_err); \
          return; \
        }

#define DET_REPORTERROR(_x,_y,_z,_q) Det_ReportError(_x, _y, _z, _q)
#else
#define VALIDATE_RV(_exp,_api,_err )
#define VALIDATE_NO_RV(_exp,_api,_err )
#define DET_REPORTERROR(_x,_y,_z,_q)
#endif




/* ----------------------------[private typedef]-----------------------------*/

typedef void (*RxIndicationCbType)(PduIdType , const PduInfoType*);


 typedef struct
 {
   CanIf_ControllerModeType  ControllerMode; // CanIf_ControllerModeType
   CanIf_PduGetModeType  PduMode;            // CanIf_PduGetModeType
 } CanIf_ChannelPrivateType;

typedef struct
{
  boolean initRun;
  CanIf_ChannelPrivateType channelData[];
} CanIf_GlobalType;
/*
#if 1
typedef struct  {
    PduIdType pduId;
    Can_IdType canId;
    uint8     data[8];
    uint8     dlc;
} CanIf_LPduType;
#endif
*/

typedef struct {
#if CANIF_PUBLIC_SETDYNAMICTXID_API
        Can_IdType dynCanId[CANIF_NUMBER_OF_DYNAMIC_CANTXPDUIDS];
#endif
    struct {
#if CANIF_PUBLIC_TX_BUFFERING
        PduIdType nextInQueue;
        uint8 data[8];
    // dlc set to -1 indicates empty buffer
        uint8 dlc;
#endif
#if CANIF_PUBLIC_READTXPDU_NOTIFY_STATUS_API
        boolean txConfirmed;
#endif
    } CanIF_TxLpdu[CANIF_NUM_TX_LPDU_ID];
    struct {
#if CANIF_PUBLIC_READRXPDU_DATA_API
        uint8 data[8];
        uint8 dlc;
#endif
#if CANIF_PUBLIC_READRXPDU_NOTIFY_STATUS_API
        boolean rxInd;
#endif
    } CanIF_RxLpdu[CANIF_NUM_RX_LPDU_ID];
} CanIf_LPduDataType;





#if (CANIF_PUBLIC_TX_BUFFERING == STD_ON)
/*
typedef struct {
    CanIf_LPduType lPdu;
    boolean inUse;
}CanIf_Arc_BufferEntryType;
*/

static CanIf_Arc_BufferEntryType TxPduBuffer[CANIF_ARC_MAX_NUM_LPDU_TX_BUF];
#endif




/* ----------------------------[private variables]---------------------------*/



CanIf_GlobalType CanIf_Global;

static CanIf_LPduDataType  lPduData;

/* Global configure */
//static const  *CanIf_ConfigPtr;




static const CanIf_ConfigType *CanIf_ConfigPtr;

//CanIf_ConfigPtr->InitCfg->CanIfRxPduConfigPtr.CanIfRxPduReadNotifyStatus = TRUE ;


//extern const CanIf_DispatchCfgType canIfDispatchCfg;




/* ----------------------------[private functions]---------------------------*/


static void ClearTxBuffers(void) {
    // reset all pending tx requests
    PduIdType i=0;
    for(i = 0; i < CANIF_NUM_TX_LPDU_ID; i++) {
      //  if(CanIf_ConfigPtr->txLpduCfg[i].controller == controller) //only have one controler

#if CANIF_PUBLIC_READTXPDU_NOTIFY_STATUS_API
      // clear notification status
      lPduData.CanIF_TxLpdu[i].txConfirmed = CANIF_NO_NOTIFICATION;
#endif
#if CANIF_PUBLIC_TX_BUFFERING
            // set nextInQueue to indicate empty queue
            hthData.hth[CanIf_ConfigPtr->txLpduCfg[i].hth].nextInQueue = -1;
        // set dlc to -1 to indicate empty buffer
            lPduData.txLpdu[i].dlc = -1;
#endif

    }
}

static void ClearRxBuffers(void) {
    // reset buffers
    PduIdType i=0;
    for( i = 0; i < CANIF_NUM_RX_LPDU_ID; i++) {//only have one controler
        //if(CanIf_ConfigPtr->rxLpduCfg[i].controller == controller)

#if CANIF_PUBLIC_READRXPDU_NOTIFY_STATUS_API
      // clear notification status
      lPduData.CanIF_RxLpdu[i].rxInd = CANIF_NO_NOTIFICATION;
#endif
#if CANIF_PUBLIC_READRXPDU_DATA_API
        // set dlc to -1 to indicate empty buffer
            lPduData.rxLpdu[i].dlc = -1;
#endif

    }
}




/* ----------------------------[public functions]---------------------------*/



/*****************************************************************************************/
/*
    - Synchronous, No-Reentrant, Service ID : 0x01
    - Parameters(IN)          :ConfigPtr->Pointer to configuration parameter set, used e.g. for
                               post build parameters           .
    - Parameters(OUT)         :NONE->VOID                      .
    - Parameters(INOUT)       :NONE                            .
    - Return                  :NONE                            .
    - Description             :This service Initializes internal and external interfaces of the CAN Interface
                               for the further processing      .
    -The service CanIf_Init() :is called only by the EcuM      .
    -The service CanIf_Init() :shall initialize the global variables
                               and data structures of the CanIf including
                                flags and buffers                .
*/
CanIf_PduModeType pduMode = (CanIf_PduModeType) 0;
void CanIf_Init(const CanIf_ConfigType* ConfigPtr )
{

//    VALIDATE_NO_RV((NULL != configPtr),CANIF_INIT_ID, CANIF_E_PARAM_POINTER);


    CanIf_ConfigPtr = ConfigPtr;


#if CANIF_PUBLIC_TX_BUFFERING
  for(int i  = 0; i < CANIF_NUM_TX_LPDU_ID; i++) {
    // set txpdu dlc to -1 to indicate empty tx buffer
    lPduData.txLpdu[i].dlc = -1;
  }
#endif

#if CANIF_PUBLIC_SETDYNAMICTXID_API
  // initialize all dynamic tx ids
  uint8 i;
  for( i = 0; i < CANIF_NUMBER_OF_DYNAMIC_CANTXPDUIDS; i++) {
    lPduData.dynCanId[i] = CanIf_ConfigPtr->InitCfg->CanIfTxPduConfigPtr[i]->CanIfTxPduId;
  }
#endif
  uint8 channel;
  for ( channel = 0; channel < CANIF_CHANNEL_CNT; channel++) {
    CanIf_Global.channelData[channel].ControllerMode = CANIF_CS_STOPPED;
//      ControllerOnEnterMode(channel, CANIF_CS_STOPPED);
    /* No req but still.. : PDU mode to OFFLINE */
     CanIf_Global.channelData[channel].PduMode = CANIF_GET_OFFLINE;

  }

         ClearTxBuffers();
         ClearRxBuffers();

     CanIf_Global.initRun = TRUE; // may be redundunt

}

/*            TO   DO  */

/*
 * 1.use config pointer insted of global struct
 * 2.adding clearing the notification and indication
 * 3. adding clearing buffer
 *
 * */




/*****************************************************************************************/

Std_ReturnType CanIf_SetPduMode(uint8 ControllerId, CanIf_PduModeType PduModeRequest )
{
    /*INIT of local variables */
    Std_ReturnType CanIf_U8_ErrorLoc;
    /*INIT  of the ERROR */
    CanIf_U8_ErrorLoc = E_OK;

    /*check for the available controller id */
    if(( ControllerId<CanIfCtrlId_1)||(ControllerId>CanIfCtrlId_NB))
    {
        /*Report an Error occured */
        CanIf_U8_ErrorLoc = E_NOT_OK;
    }
    /*check for the mode */
    else if(PduModeRequest >CANIF_ONLINE)
    {
        /*Report an Error occured */
        CanIf_U8_ErrorLoc = E_NOT_OK;
    }
    /*check for the state type defined in the can controller */
   /*
    * ÇÓÇá ÇãÇäí
     else if(Can_ControllerStateType != CAN_CS_STARTED)
    {
        //Report an Error occured
        CanIf_U8_ErrorLoc = E_NOT_OK;
    }
    */
    else
    {
        /*INIT  of the ERROR */
        CanIf_U8_ErrorLoc = E_OK ;
        switch(PduModeRequest)
        {
        case CANIF_OFFLINE:
            pduMode = CANIF_OFFLINE;
            break;
        case CANIF_TX_OFFLINE:
            pduMode =CANIF_TX_OFFLINE;
            break;
        case CANIF_TX_OFFLINE_ACTIVE:
            pduMode = CANIF_TX_OFFLINE_ACTIVE;
            break;
        case CANIF_ONLINE:
            pduMode = CANIF_ONLINE ;
            break;
        default :
        }
    }


return CanIf_U8_ErrorLoc ;

}
/*****************************************************************************************/
/*
     - Synchronous, Reentrant, Service ID : 0x13
    - Parameters(IN)          :CanTxPduId->This ID specifies the corresponding CAN L-PDU ID
                                           and implicitly the CAN Driver instance as well as the
                                           corresponding CAN controller device.        .
    - Parameters(OUT)         :NONE->VOID                      .
    - Parameters(INOUT)       :NONE                            .
    - Return                  :NONE                            .
    - Description             :This service confirms a previously successfully processed transmission
                               of a CAN TxPDU.     .
    -The service CanIf_TxConfirmation() :The service CanIf_TxConfirmation() is implemented in CanIf and called
                                         by the CanDrv after the CAN L-PDU has been transmitted on the CAN network.
     -The service
() :If configuration parameters CANIF_PUBLIC_READTXPDU_NOTIFY_STATUS_API
                                          and and CANIF_TXPDU_READ_NOTIFYSTATUS for the Transmitted L-PDU are set to TRUE, and if CanIf_TxConfirmation() is called, CanIf shall set
                                          the notification status for the Transmitted L-PDU .
     - If parameter CanTxPduId of CanIf_TxConfirmation() has an invalid value, CanIf shall report development error code CANIF_E_PARAM_LPDU to the Det_ReportError service of the DET module, when CanIf_TxConfirmation() is called.
     - Configuration of CanIf_TxConfirmation(): Each Tx LPDU (see ECUC_CanIf_00248) has to be configured with a corresponding transmit confirmation service of an upper layer module (see [SWS_CANIF_00011]) which is called in CanIf_TxConfirmation().
  */



/*****************************************************************************************/
/*
    - Synchronous, Reentrant, Service ID : 0x14
    - Parameters(IN)          :(1)Mailbox->Identifies the HRH and its corresponding CAN Controller .
                              :(2)PduInfoPtr->Pointer to the received L-PDU .
    - Parameters(OUT)         :NONE->VOID                      .
    - Parameters(INOUT)       :NONE                            .
    - Return                  :NONE                            .
    - Description             :This service indicates a successful reception of a received CAN Rx LPDU
                               to the CanIf after passing all filters and validation checks.

    -The service CanIf_RxIndication() :is implemented in CanIf and called by
                                       CanDrv after a CAN L-PDU has been received.                                                                .
    -Within the service CanIf_RxIndication() the CanIf routes this indication to the configured upper layer target service(s).

    -Error check for parameter Mailbox->Hoh , Mailbox->CanId for invalid value .
    -If CanIf_RxIndication() is called with invalid PduInfoPtr- >SduLength,
                             runtime error CANIF_E_INVALID_DATA_LENGTH is reported
    */
void CanIf_RxIndication( const Can_HwType* Mailbox,const PduInfoType* PduInfoPtr)
{
    /*Local Variables Init*/

    /*
     * Error Check for replacing the error detection
     * */
    uint8 CanifErrorCheck = 1;
   /*
    * Counter to get the Index for the RX PDU
    * */
    uint8 CanifLocalCount;
    /*
     * The RX PDU Index
     * */
    sint8 RxPdu_Index;
    /*
     * CHeck For THe H.W Handle
     * */
    uint8 CanIfRxPduCfg_i ;
    if((Mailbox->Hoh) != CanHwObjectCount)
    {
        CanifErrorCheck = 0;
        //Det_ReportError(CanIf_Id, CanIf_InstanceId, CanIf_RxIndication_Id, CANIF_E_PARAM_HOH);
    }

    /*
     * Check for the CAN ID
     * */
    else if((Mailbox->CanId) != 25)
    {
        CanifErrorCheck =0;
        //Det_ReportError(CanIf_Id, CanIf_InstanceId, CanIf_RxIndication_Id,CANIF_E_PARAM_CANID );
    }
    /**
     * CHeck for the DATA Length Saved ..
     */
    else if((PduInfoPtr->SduLength) != /*(CanIf_ConfigPtr->InitCfg->CanIfRxPduConfigPtr.CanIfRxPduDataLength)*/1 )
    {
        CanifErrorCheck =0;
        //Det_ReportError(CanIf_Id, CanIf_InstanceId, CanIf_RxIndication_Id,CANIF_E_INVALID_DATA_LENGTH );
    }

    //check for the initialization
    /*
     *Get the RX Index
     * */
    for(CanifLocalCount=0;CanifLocalCount< CanIf_ConfigPtr->InitCfg->CanIfMaxRxPduCfg;CanifLocalCount++)
    {
     //   if (Mailbox->id == CanIf_ConfigPtr->InitCfg->CanIfRxPduConfigPtr[CanifLocalCount].CanIfRxPduCanId)
       // {
         //   RxPdu_Index = CanifLocalCount;
           // break;
        //}
    }
    /*
     * Prepare the Data for the Upper layer Modules ..
     * */

    while (CanifErrorCheck ==1)
    {
    for (CanIfRxPduCfg_i =(uint16) 0;(  (CanifErrorCheck==(uint8)1) && (CanIfRxPduCfg_i < (uint16)CanIf_ConfigPtr->InitCfg->CanIfMaxRxPduCfg) ); CanIfRxPduCfg_i++)
       {
      //  CanIfRxPduUserRxConfirmationUL USER_NAME = CanIf_ConfigPtr->InitCfg->CanIfRxPduConfigPtr[CanIfRxPduCfg_i].CanIfRxPduUserRxIndicationUL_i;
       int USER_NAME;
        switch(USER_NAME)
        {
                case CAN_TP:
                {
                    /*
                    PduInfoType CanTpRxPdu;
                    CanTpRxPdu.SduLength = PduInfoPtr->SduLength;
                    CanTpRxPdu.SduDataPtr = (uint8 *)PduInfoPtr->SduDataPtr;
                    CanTp_RxIndication(canif_cantp_value[RxPdu_Index],&CanTpRxPdu);

                    CanifErrorCheck = 0;
                    */
                    break;
                }
                case CAN_NM:
                {

                    break;
                }
                case PDUR:
                {
                    break ;
                }
                default :
        }
       }
    }
}






/*******************************************************************************
* Service Name: CanIF_Transmit
* Service ID[hex]: 0x49
* Sync/Async: Synchronous
* Reentrancy: Reentrant for different PduIds. Non reentrant for the same PduId.
* Parameters (in): TxPduId : Identifier of the PDU to be transmitted
                   PduInfoPtr : Length of and pointer to the PDU data and pointer to MetaData.
* Parameters (inout): None
* Parameters (out): None
* Return value: Std_ReturnType E_OK: Transmit request has been accepted.
                               E_NOT_OK: Transmit request has not been accepted.
* Description: Requests transmission of a PDU.
********************************************************************************/



Std_ReturnType CanIf_Transmit(PduIdType CanTxPduId,
        const PduInfoType *PduInfoPtr) {


   // Can_PduType canPdu;
  //  Can_ReturnType writeRet;
    Std_ReturnType ret = E_OK;
     CanIfTxPduCfg *txPduPtr;
//
//    /* @req 4.0.3/CANIF661 */
//    VALIDATE_RV((TRUE == CanIf_Global.initRun), CANIF_TRANSMIT_ID, CANIF_E_UNINIT);
//    /* @req 4.0.3/CANIF320 */
//    VALIDATE_RV((NULL != PduInfoPtr), CANIF_TRANSMIT_ID, CANIF_E_PARAM_POINTER);
//    /* @req 4.0.3/CANIF319 */
//    VALIDATE_RV((CanTxPduId < CanIf_ConfigPtr->InitConfig->CanIfNumberOfCanTXPduIds), CANIF_TRANSMIT_ID, CANIF_E_INVALID_TXPDUID);
//
//    VALIDATE_RV( (CANIF_GET_OFFLINE == CanIf_Global.channelData.PduMode),CANIF_TRANSMIT_ID, CANIF_E_STOPPED )
//  //          DET_REPORT_ERROR(CANIF_TRANSMIT_ID, CANIF_E_STOPPED);
//
//    VALIDATE_RV( (CANIF_CS_STOPPED == CanIf_Global.channelData.ControllerMode) , CANIF_TRANSMIT_ID, CANIF_E_STOPPED)
//  //          DET_REPORT_ERROR(CANIF_TRANSMIT_ID, CANIF_E_STOPPED);
//
//
//        if( (CANIF_CS_STARTED != CanIf_Global.channelData[0].ControllerMode) &&
//            ((CANIF_GET_RX_ONLINE == CanIf_Global.channelData[0].PduMode) || (CANIF_GET_OFFLINE == CanIf_Global.channelData[0].PduMode)) ) {
//            return E_NOT_OK;
//        }
//
//        if( (CANIF_GET_RX_ONLINE == CanIf_Global.channelData[0].PduMode) ||
//                (CANIF_GET_OFFLINE == CanIf_Global.channelData[0].PduMode) ) {
//            return E_NOT_OK;
//        }
//
//

//        txPduPtr = CanIf_ConfigPtr->InitCfg->CanIfTxPduConfigPtr[CanTxPduId];
 //         txPduPtr = CanIf_ConfigPtr->InitCfg->CanIfTxPduConfigPtr[CanTxPduId];


        Can_HwHandleType hth =10; //txPduPtr->hth;

//        Can_IdType canId = ((CanTxPduId < CANIF_NUMBER_OF_DYNAMIC_CANTXPDUIDS)? lPduData.dynCanId[canTxPduId] : CanIf_ConfigPtr->InitCfg->CanIfTxPduConfigPtr[canTxPduId].id);
          Can_IdType canId = 25;//(Can_IdType)txPduPtr->CanIfTxPduCanId;


          Can_PduType canPdu = {
                .sdu = PduInfoPtr->SduDataPtr,
                .id = canId,
                .swPduHandle = CanTxPduId,
                .length = PduInfoPtr->SduLength
            };

          //till we adust the can errors in can driver
          Std_ReturnType canRetVal =Can_Write(hth, &canPdu);
        if(canRetVal != E_OK) {
          // if cotroller busy, just throw message, cannot do anything since no buffers
          return E_NOT_OK;
        }

        return E_OK;

/*
    Can_ReturnType canRetVal =Can_Write(hth, &canPdu);
  if(canRetVal != CAN_OK) {
    // if cotroller busy, just throw message, cannot do anything since no buffers
    return E_NOT_OK;
  }

  return E_OK;
*/

    }





/*******************************************************************************
* Service Name: CanIf_ReadTxNotifStatus
* Service ID[hex]: 0x07
* Sync/Async: Synchronous
* Reentrancy: Non Reentrant
* Parameters (in):  CanIfTxSduId  - L-SDU handle to be transmitted.
                                    This handle specifies the corresponding CAN LSDU
                                    ID and implicitly the CAN Driver instance as
                                    well as the corresponding CAN controller device.
* Parameters (inout): None
* Parameters (out): None
* Return value: CanIf_NotifStatusType    Current confirmation status of the corresponding
                                         CAN Tx L-PDU.
* Description: This service returns the confirmation status (confirmation occurred or
               not) of a specific static or dynamic CAN Tx L-PDU, requested by the
               CanIfTxSduId.
********************************************************************************/

//#if ( CANIF_PUBLIC_READTXPDU_NOTIFY_STATUS_API == STD_ON )
// service id 7
CanIf_NotifStatusType CanIf_ReadTxNotifStatus(
                                              PduIdType CanTxPduId
                                              ) {
    VALIDATE_NO_RV(CanIf_ConfigPtr != 0, 7, CANIF_E_UNINIT);
    VALIDATE_NO_RV(canTxPduId < CANIF_NUM_TX_LPDU_ID, 7, CANIF_E_INVALID_TXPDUID);
    CanIf_NotifStatusType retVal = lPduData.CanIF_TxLpdu[CanTxPduId].txConfirmed;
    if(retVal != CANIF_NO_NOTIFICATION) {
        lPduData.CanIF_TxLpdu[CanTxPduId].txConfirmed = CANIF_NO_NOTIFICATION;
    }
    return retVal;
}
//#endif






/*******************************************************************************
* Service Name: CanIf_ReadRxNotifStatus
* Service ID[hex]: 0x08
* Sync/Async: Synchronous
* Reentrancy: Non Reentrant
* Parameters (in):  CanIfRxSduId  - L-SDU handle to be transmitted.
                                    This handle specifies the corresponding CAN LSDU
                                    ID and implicitly the CAN Driver instance as
                                    well as the corresponding CAN controller device.
* Parameters (inout): None
* Parameters (out): None
* Return value: CanIf_NotifStatusType    Current confirmation status of the corresponding
                                         CAN Rx L-PDU.
* Description: This service returns the confirmation status (confirmation occurred or
               not) of a specific static or dynamic CAN Rx L-PDU, requested by the
               CanIfRxSduId.
********************************************************************************/


#if ( CANIF_PUBLIC_READRXPDU_NOTIFY_STATUS_API == STD_ON )
// service id 8
CanIf_NotifStatusType CanIf_ReadRxNotifStatus(
                                              PduIdType canRxPduId
                                              ) {
    VALIDATE_NO_RV((TRUE == CanIf_Global.initRun), CANIF_CONTROLLER_BUSOFF_ID, CANIF_E_UNINIT);
    VALIDATE_NO_RV(canRxPduId < CANIF_NUM_TX_LPDU_ID, 8, CANIF_E_INVALID_TXPDUID);
    CanIf_NotifStatusType retVal = lPduData.CanIF_RxLpdu[canRxPduId].rxInd;
    if(retVal != CANIF_NO_NOTIFICATION) {
        lPduData.CanIF_RxLpdu[canRxPduId].rxInd = CANIF_NO_NOTIFICATION;
    }
    return retVal;
}
#endif








/*****************************************************************************************/
/*
    - Synchronous, Reentrant, Service ID : 0x16
    - Parameters(IN)                          :ControllerId->Abstract CanIf ControllerId which is assigned to a
                                               CAN controller, where a BusOff occured.         .
    - Parameters(OUT)                         :NONE->VOID                      .
    - Parameters(INOUT)                       :NONE                            .
    - Return                                  :NONE                            .
    - Description                             :This service indicates a Controller BusOff event referring to the corresponding
                                               CAN Controller with the abstract CanIf ControllerId.     .
    -The callback service CanIf_ControllerBusOff() :is called by CanDrv and
                                                    implemented in CanIf. It is called in case of a mode change notification of the CanDrv.
    -The service CanIf_Init() :shall initialize the global variables
                               and data structures of the CanIf including flags and buffers                .
    -Error check for ControllerId ..
    -Configuration of CanIf_ControllerBusOff(): ID of the CAN Controller is published inside the configuration description of the CanIf
    -This service always has to be available, so there does not exist an appropriate
     configuration parameter.
*/


void CanIf_ControllerBusOff(uint8 ControllerId )
{

    VALIDATE_NO_RV((TRUE == CanIf_Global.initRun), CANIF_CONTROLLER_BUSOFF_ID, CANIF_E_UNINIT);
    VALIDATE_NO_RV((ControllerId < CAN_CONTROLLER_CNT), CANIF_CONTROLLER_BUSOFF_ID, CANIF_E_PARAM_CONTROLLERID);

    // store the new mode

    if( CANIF_CS_UNINIT != CanIf_Global.channelData[ControllerId].ControllerMode ) {

        CanIf_Global.channelData[ControllerId].ControllerMode = CANIF_CS_STOPPED;
    }
    ClearTxBuffers();
    ClearRxBuffers();

    // call ev callback
/*
        if(NULL != canIfDispatchCfg.user_ControllerBusOff) {

            (*canIfDispatchCfg.user_ControllerBusOff)(ControllerId);
    }
*/
}




/*******************************************************************************
* Service Name: CanIf_ControllerModeIndication
* Service ID[hex]: 0x08
* Sync/Async: Synchronous
* Reentrancy: Non Reentrant
* Parameters (in):  ControllerId  - Abstract CanIf ControllerId which is assigned to a
                    CAN controller, where a BusOff occured.
                    ControllerMode - Mode to which the CAN controller transitioned
* Parameters (inout): None
* Parameters (out): None
* Return value: void
* Description: This service indicates a controller state transition referring to the corresponding
               CAN controller with the abstract CanIf ControllerId.
********************************************************************************/



void CanIf_ControllerModeIndication(uint8 ControllerId, CanIf_ControllerModeType controllerMode)
{
      VALIDATE_NO_RV((TRUE == CanIf_Global.initRun), CANIF_CONTROLLER_MODE_INDICATION_ID, CANIF_E_UNINIT);

    /* @req 4.0.3/CANIF700 */
    VALIDATE_NO_RV((ControllerId < CAN_CONTROLLER_CNT), CANIF_CONTROLLER_MODE_INDICATION_ID, CANIF_E_PARAM_CONTROLLER);

    CanIf_Global.channelData[ControllerId].ControllerMode = controllerMode;
    ClearTxBuffers();
    ClearRxBuffers();
        // requested mode reached. Call ev callback
		/*
        if(NULL != canIfDispatchCfg.user_ControllerModeIndication) {

            (*canIfDispatchCfg.user_ControllerModeIndication)(ControllerId, controllerMode);
        }

*/
    }

/*******************************************************************************
* Service Name: CanIf_SetDynamicTxId
* Service ID[hex]: 0x0c
* Sync/Async: Synchronous
* Reentrancy: Non Reentrant
* Parameters (in):  CanIfTxSduId  - L-SDU handle to be transmitted.
                                    This handle specifies the corresponding CAN LSDU
                                    ID and implicitly the CAN Driver instance as
                                    well as the corresponding CAN controller device.
                            CanId - Standard/Extended CAN ID of CAN L-SDU that
                                    shall be transmitted as FD or conventional CAN
                                    frame.
* Parameters (inout): None
* Parameters (out): None
* Return value: void
* Description: This service reconfigures the corresponding CAN identifier of the requested
                CAN L-PDU.
********************************************************************************/



  #if CANIF_PUBLIC_SETDYNAMICTXID_API
// service id 12
void CanIf_SetDynamicTxId(PduIdType canTxPduId, Can_IdType canId) {
      VALIDATE_NO_RV((TRUE == CanIf_Global.initRun), CANIF_CONTROLLER_MODE_INDICATION_ID, CANIF_E_UNINIT);
    //VALIDATE_NO_RV((canId & 0x80000000) && canId < 0xA0000000 || canId < 0x800, 12, CANIF_E_PARAM_CANID);
    VALIDATE_NO_RV(canTxPduId < CANIF_NUMBER_OF_DYNAMIC_CANTXPDUIDS, 12, CANIF_E_INVALID_TXPDUID);
    // change the tx id
    lPduData.dynCanId[canTxPduId]    = canId;
}
#endif

/*******************************************ÈÏÇíå ÇãÇäí***************************************/

Std_ReturnType CanIf_SetControllerMode(uint8 ControllerId,Can_ControllerStateType ControllerMode)
{
    uint8 channel;
    CanIf_ControllerModeType oldMode;
    oldMode = CanIf_Global.channelData[channel].ControllerMode;
  //  CanController.CanId=CanIf_ConfigPtr->CanIfCtrlCanCtrlRef[channel];

    if(( ControllerId<CanIfCtrlId_1)||(ControllerId>CanIfCtrlId_NB))
    {
    //    Det_ReportError(CanIf_Id, CanIf_InstanceId, CanIf_SetControllerMode_Id, CANIF_E_PARAM_CONTROLLERID);
        return E_NOT_OK;
    }

    switch(ControllerMode)
    {
        case CAN_CS_STOPPED:
        {

            CanIf_SetPduMode(channel, CANIF_OFFLINE);

            if (Can_SetControllerMode(0x00u, CAN_CS_STOPPED) == E_NOT_OK)
            {
            return E_NOT_OK;
            }
             CanIf_Global.channelData[channel].ControllerMode = CANIF_CS_STOPPED;
            break;
        }
        case CAN_CS_STARTED:
        {
            switch(oldMode)
                {
                    case CANIF_CS_SLEEP:
                    if (Can_SetControllerMode(0x00u, CAN_CS_STOPPED) == E_NOT_OK)
                    {
                     return E_NOT_OK;
                    }
                    CanIf_Global.channelData[channel].ControllerMode = CANIF_CS_STOPPED;

                    default:
                    break;
                }
            CanIf_SetPduMode(channel, CANIF_ONLINE);
            if (Can_SetControllerMode(0x00u, CAN_CS_STARTED) == E_NOT_OK)
            {
            return E_NOT_OK;
            }
            CanIf_Global.channelData[channel].ControllerMode = CANIF_CS_STARTED;
            break;
        }

        case CAN_CS_SLEEP:
        {
            switch (oldMode)
            {
            case CANIF_CS_STARTED:
            if (Can_SetControllerMode(0x00u, CAN_CS_STOPPED) == E_NOT_OK)
            {
            return E_NOT_OK;
            }
            CanIf_Global.channelData[channel].ControllerMode = CANIF_CS_STOPPED;
            break;
            default:
            break;
            }

            if (Can_SetControllerMode(0x00u, CAN_CS_SLEEP)== E_NOT_OK)
            {
            return E_NOT_OK;
            }
            CanIf_Global.channelData[channel].ControllerMode = CANIF_CS_SLEEP;
            break;
        }
        default:
      //  Det_ReportError(CanIf_Id, CanIf_InstanceId, CanIf_SetControllerMode_Id, CANIF_E_PARAM_POINTER);
        return E_NOT_OK;
        break;
    }
    return E_OK;
}
//CanIf_ConfigType *CANIF_CFGTYPE;
CanIfTxPduCfg *txpduptr; //GLOBAL
void CanIf_TxConfirmation(PduIdType CanTxPduId)
{
//    if(CanIf_ConfigType->CanIfInitCfg->CanIfMaxTxPduCfg)
//    {
//        Det_ReportError(CanIf_Id, CanIf_InstanceId, CanIf_TxConfirmation_Id, CANIF_E_PARAM_LPDU);
//    }

    while (CanIf_Global.initRun==FALSE);
    if (txpduptr->CanIfTxPduUserTxConfirmationName!= NULL)
    {
    txpduptr->user_TxConfirmation(txpduptr->CanIfTxPduCanId);
    }
}
/*************************************äåÇíå ÇãÇäí*********************************************/
