/*
 * Canif.h
 *
 *  Created on: ??þ/??þ/????
 *      Author: MANSI-PC
 */

#ifndef CANIF_H_
#define CANIF_H_

//#include <Can_GeneralTypes.h>
#include <Canif_Types.h>
#include <ComStack_Types.h>

#include <Canif_Cfg.h>

//#include <ComStack_Types.h>


/************************** Published information *********************/

/* Module Version 1.0.0 */
#define CANIF_TYPES_SW_MAJOR_VERSION           (1U)
#define CANIF_TYPES_SW_MINOR_VERSION           (0U)
#define CANIF_TYPES_SW_PATCH_VERSION           (0U)

/* AUTOSAR Version 4.3.1 */
#define CANIF_TYPES_AR_RELEASE_MAJOR_VERSION   (4U)
#define CANIF_TYPES_AR_RELEASE_MINOR_VERSION   (3U)
#define CANIF_TYPES_AR_RELEASE_PATCH_VERSION   (1U)







// Service IDs
#define CANIF_INIT_ID                0x01
#define CANIF_INIT_CONTROLLER_ID     0x02
#define CANIF_SET_CONTROLLER_MODE_ID 0x03
#define CANIF_GET_CONTROLLER_MODE_ID 0x04
#define CANIF_TRANSMIT_ID            0x05
#define CANIF_READTXPDUDATA_ID       0x06
#define CANIF_READTXNOTIFSTATUS_ID   0x07
#define CANIF_READRXNOTIFSTATUS_ID   0x08
#define CANIF_SETPDUMODE_ID          0x09
#define CANIF_GETPDUMODE_ID          0x0A
#define CANIF_SETDYNAMICTX_ID        0x0C
#define CANIF_SET_TRANSCEIVERMODE_ID     0x0D
#define CANIF_GET_TRANSCEIVERMODE_ID     0x0E
#define CANIF_GET_TRCVMODEREASON_ID      0x0F
#define CANIF_SET_TRANSCEIVERWAKEMODE_ID 0x10
#define CANIF_CHECKWAKEUP_ID          0x11
#define CANIF_CHECKVALIDATION_ID      0x12
#define CANIF_TXCONFIRMATION_ID       0x13
#define CANIF_RXINDICATION_ID         0x14
#define CANIF_CANCELTXCONFIRMATION_ID 0x15
#define CANIF_CONTROLLER_BUSOFF_ID    0x16

#define CANIF_CONTROLLER_MODE_INDICATION_ID 0x17

#define CANIF_SETWAKEUPEVENT_ID       0x40
#define CANIF_ARCERROR_ID           0x41













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
void CanIf_Init(const CanIf_ConfigType* ConfigPtr ) ;

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
     -The service CanIf_TxConfirmation() :If configuration parameters CANIF_PUBLIC_READTXPDU_NOTIFY_STATUS_API
                                          and and CANIF_TXPDU_READ_NOTIFYSTATUS for the Transmitted L-PDU are set to TRUE, and if CanIf_TxConfirmation() is called, CanIf shall set
                                          the notification status for the Transmitted L-PDU .
     - If parameter CanTxPduId of CanIf_TxConfirmation() has an invalid value, CanIf shall report development error code CANIF_E_PARAM_LPDU to the Det_ReportError service of the DET module, when CanIf_TxConfirmation() is called.
     - Configuration of CanIf_TxConfirmation(): Each Tx LPDU (see ECUC_CanIf_00248) has to be configured with a corresponding transmit confirmation service of an upper layer module (see [SWS_CANIF_00011]) which is called in CanIf_TxConfirmation().
  */
void CanIf_TxConfirmation( PduIdType CanTxPduId) ;













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



Std_ReturnType CanIf_Transmit(PduIdType CanTxPduId,const PduInfoType *PduInfoPtr) ;

/*****************************************************************************************/
/*
    - Synchronous, Reentrant, Service ID : 0x17
    - Parameters(IN)          :(1)ControllerId->Abstract CanIf ControllerId which is assigned to a
                                  CAN controller, which state has been transitioned.           .
                              :(2)ControllerMode->Mode to which the CAN controller transitioned .

    - Parameters(OUT)         :NONE->VOID                      .
    - Parameters(INOUT)       :NONE                            .
    - Return                  :NONE                            .
    - Description             :This service indicates a controller state transition referring to the corresponding
                               CAN controller with the abstract CanIf ControllerId.    .

    -The callback service CanIf_ControllerModeIndication() :is called by CanDrv and implemented in CanIf.
                                                            It is called in case of a state transition notification of the CanDrv.
                                                                .
    -Has the same specification of above function & check also for controller id .
*/
void CanIf_ControllerModeIndication( uint8 ControllerId,Can_ControllerStateType ControllerMode);

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
void CanIf_RxIndication( const Can_HwType* Mailbox,const PduInfoType* PduInfoPtr);

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
void CanIf_ControllerBusOff(uint8 ControllerId );

/*****************************************************************************************/

Std_ReturnType CanIf_SetPduMode(uint8 ControllerId, CanIf_PduModeType PduModeRequest );
Std_ReturnType CanIf_SetControllerMode(uint8 ControllerId,Can_ControllerStateType ControllerMode);

#endif /* CANIF_H_ */
