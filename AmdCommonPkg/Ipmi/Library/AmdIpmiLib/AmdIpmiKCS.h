/******************************************************************************
 * Copyright (C) 2021-2023 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/
#include <Base.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <IndustryStandard/SmBios.h>
#include <Library/MemoryAllocationLib.h>

#define IPMI_KCS_BASE_ADDRESS     PcdGet16 (PcdIpmiKCSPort)
#define IPMI_KCS_REG_DATA_IN_OUT  PcdGet16 (PcdIpmiKCSPort)
#define IPMI_KCS_REG_CMD_STATUS   IPMI_KCS_REG_DATA_IN_OUT + 1

// According to IPMI specification
#define IPMI_KCS_OBF              BIT0
#define IPMI_KCS_IBF              BIT1
#define IPMI_KCS_SMS_ATN          BIT2
#define IPMI_KCS_COMMAND_DATA     BIT3
#define IPMI_KCS_OEM1             BIT4
#define IPMI_KCS_OEM2             BIT5
#define IPMI_KCS_S0               BIT6
#define IPMI_KCS_S1               BIT7
#define IPMI_GET_STATE(s)         (s>>6)
#define IPMI_SET_STATE(s)         (s<<6)

// Control codes
#define IPMI_KCS_CC_GET_STS_ABORT 0x60
#define IPMI_KCS_CC_WRITE_START   0x61
#define IPMI_KCS_CC_WRITE_END     0x62
#define IPMI_KCS_CC_READ          0x68

// Status codes
#define IPMI_KCS_STS_NO_ERROR     0x00
#define IPMI_KCS_STS_ABORT        0x01
#define IPMI_KCS_STS_ILLEGAL      0x02
#define IPMI_KCS_STS_LENGTH_ERROR 0x06
#define IPMI_KCS_STS_UNSPECIFIED  0xFF

#define IPMI_KCS_TIMEOUT_5SEC     5000*1000  // 5sec, according to IPMI spec
#define IPMI_KCS_TIMEOUT_1MS      1000

typedef enum {
  IPMI_KCS_IDLE_STATE = 0,
  IPMI_KCS_READ_STATE,
  IPMI_KCS_WRITE_STATE,
  IPMI_KCS_ERROR_STATE
} IPMI_KCS_STATE;

typedef struct {
  UINT8 RspNetFunc;
  UINT8 RspCommand;
} IPMI_KCS_RSP_HEADER;
/**
  This service communicates with BMC using KCS protocol.

  @param[in]      NetFunction           Net function of the command.
  @param[in]      Command               IPMI Command.
  @param[in]      RequestData           Command Request Data.
  @param[in]      RequestDataSize       Size of Command Request Data.
  @param[out]     ResponseData          Command Response Data. The completion
                                        code is the first byte of response
                                        data.
  @param[in, out] ResponseDataSize      Size of Command Response Data.

  @retval         EFI_SUCCESS           The command byte stream was
                                        successfully submit to the device and a
                                        response was successfully received.
  @retval         EFI_NOT_FOUND         The command was not successfully sent
                                        to the device or a response was not
                                        successfully received from the device.
  @retval         EFI_NOT_READY         Ipmi Device is not ready for Ipmi
                                        command access.
  @retval         EFI_DEVICE_ERROR      Ipmi Device hardware error.
  @retval         EFI_TIMEOUT           The command time out.
  @retval         EFI_UNSUPPORTED       The command was not successfully sent to
                                        the device.
  @retval         EFI_OUT_OF_RESOURCES  The resource allocation is out of
                                        resource or data size error.
**/

EFI_STATUS
EFIAPI
AmdIpmiSubmitCommandKCS (
  IN     UINT8    NetFunction,
  IN     UINT8    Command,
  IN     UINT8    *RequestData,
  IN     UINT32   RequestDataSize,
     OUT UINT8    *ResponseData,
  IN OUT UINT32   *ResponseDataSize
  );
