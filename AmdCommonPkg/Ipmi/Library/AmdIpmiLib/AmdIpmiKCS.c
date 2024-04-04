/******************************************************************************
 * Copyright (C) 2021-2024 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/
#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Uefi/UefiSpec.h>
#include <Library/TimerLib.h>
#include <Library/BaseMemoryLib.h>

#include "AmdIpmiKCS.h"

/**
  This function waits for parameter Flag to set.
  Checks status flag in every 1ms internal till 5 seconds elapses.

  @param[in]  Flag        KCS Flag to test.
  @retval     EFI_SUCCESS The KCS flag under test is set.
  @retval     EFI_TIMEOUT The KCS flag didnt set in 5 second windows.
**/
EFI_STATUS
WaitStatusSet (
  IN  UINT8 Flag
  )
{
  UINT64 Timeout = 0;

  while (!(IoRead8 (IPMI_KCS_REG_CMD_STATUS) & Flag)) {
    MicroSecondDelay (IPMI_KCS_TIMEOUT_1MS);
    Timeout = Timeout + IPMI_KCS_TIMEOUT_1MS;
    if (Timeout >= IPMI_KCS_TIMEOUT_5SEC) {
      return EFI_TIMEOUT;
    }
  }
  return EFI_SUCCESS;
}

/**
  This function waits for parameter Flag to get cleared.
  Checks status flag in every 1ms internal till 5 seconds elapses.

  @param[in]  Flag        KCS Flag to test.

  @retval     EFI_SUCCESS The KCS flag under test is clear.
  @retval     EFI_TIMEOUT The KCS flag didnt cleared in 5 second windows.
**/
EFI_STATUS
WaitStatusClear (
  IN  UINT8 Flag
  )
{
  UINT64 Timeout = 0;

  while (IoRead8 (IPMI_KCS_REG_CMD_STATUS) & Flag) {
    MicroSecondDelay (IPMI_KCS_TIMEOUT_1MS);
    Timeout = Timeout + IPMI_KCS_TIMEOUT_1MS;
    if (Timeout >= IPMI_KCS_TIMEOUT_5SEC) {
      return EFI_TIMEOUT;
    }
  }
  return EFI_SUCCESS;
}

/**
  This function validates KCS OBF bit.
  Checks whether OBF bit is set or not.

  @retval EFI_SUCCESS OBF bit is set.
  @retval EFI_TIMEOUT OBF bit is not set.
**/
EFI_STATUS
ClearOBF (
  IN  VOID
  )
{
  if (IoRead8 (IPMI_KCS_REG_CMD_STATUS) & IPMI_KCS_OBF) {
    IoRead8 (IPMI_KCS_REG_DATA_IN_OUT); // read the data to clear the OBF
    if (IoRead8 (IPMI_KCS_REG_CMD_STATUS) & IPMI_KCS_OBF) {
      return EFI_NOT_READY;
    }
  }
  return EFI_SUCCESS;
}

/**
  This function writes/sends data to the KCS port.
  Algorithm is based on flow chart provided in IPMI spec 2.0
  Figure 9-6, KCS Interface BMC to SMS Write Transfer Flow Chart

  @param[in]  NetFunction           Net function of the command.
  @param[in]  Command               IPMI Command.
  @param[in]  RequestData           Command Request Data.
  @param[in]  RequestDataSize       Size of Command Request Data.

  @retval     EFI_SUCCESS           The command byte stream was successfully
                                    submit to the device and a response was
                                    successfully received.
  @retval     EFI_NOT_FOUND         The command was not successfully sent to the
                                    device or a response was not successfully
                                    received from the device.
  @retval     EFI_NOT_READY         Ipmi Device is not ready for Ipmi command
                                    access.
  @retval     EFI_DEVICE_ERROR      Ipmi Device hardware error.
  @retval     EFI_TIMEOUT           The command time out.
  @retval     EFI_UNSUPPORTED       The command was not successfully sent to
                                    the device.
  @retval     EFI_OUT_OF_RESOURCES  The resource allocation is out of resource or
                                    data size error.
**/

EFI_STATUS
IpmiKcsWrite (
  IN  UINT8   NetFunction,
  IN  UINT8   Command,
  IN  UINT8   *RequestData,
  IN  UINT32  RequestDataSize)
{
  EFI_STATUS  Status;
  UINT32      Length;
  UINT8       *Buffer;
  UINT8       *BufferPtr;

  Length = sizeof (NetFunction) + sizeof (Command);
  if (RequestData != NULL) {
    Length = Length + RequestDataSize;
  }
  Buffer = AllocateZeroPool (Length);
  if (Buffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  /*
   * Buffer[0] = NetFunction
   * Buffer[1] = Command
   * Buffer [2..RequestDataSize] = RequestData
   */
  BufferPtr = Buffer;
  CopyMem (BufferPtr, &NetFunction, sizeof (NetFunction));
  BufferPtr += sizeof (NetFunction);
  CopyMem (BufferPtr, &Command, sizeof (Command));
  BufferPtr += sizeof (Command);
  if (Length > (sizeof(NetFunction) + sizeof (Command))) {
    CopyMem (BufferPtr, RequestData, RequestDataSize);
  }
  BufferPtr = Buffer;

  // Step 1. wait for IBF to get clear
  Status = WaitStatusClear (IPMI_KCS_IBF);
  if (EFI_ERROR (Status)) {
    FreePool (Buffer);
    return Status;
  }

  // Step 2. clear OBF
  if (EFI_ERROR (ClearOBF ())) {
    FreePool (Buffer);
    return EFI_NOT_READY;
  }

  // Step 3. WR_START to CMD, phase=wr_start
  IoWrite8 (IPMI_KCS_REG_CMD_STATUS, IPMI_KCS_CC_WRITE_START);

  // Step 4. wait for IBF to get clear
  Status = WaitStatusClear (IPMI_KCS_IBF);
  if (EFI_ERROR (Status)) {
    FreePool (Buffer);
    return Status;
  }

  // Step 5. check state it should be WRITE_STATE, else exit with error
  if (IPMI_GET_STATE (IoRead8 (IPMI_KCS_REG_CMD_STATUS))
      != IPMI_KCS_WRITE_STATE) {
    FreePool (Buffer);
    return EFI_NOT_READY;
  }

  // Step 6, Clear OBF
  if (EFI_ERROR (ClearOBF ())) {
    FreePool (Buffer);
    return EFI_NOT_READY;
  }

  while (Length > 1) {
    // Step 7, phase wr_data, write one byte of Data
    IoWrite8 (IPMI_KCS_REG_DATA_IN_OUT, *BufferPtr);
    Length--;
    BufferPtr++;

    // step 8. wait for IBF clear
    Status = WaitStatusClear (IPMI_KCS_IBF);
    if (EFI_ERROR (Status)) {
      FreePool (Buffer);
      return Status;
    }

    // Step 9. check state it should be WRITE_STATE, else exit with error
    if (IPMI_GET_STATE (IoRead8 (IPMI_KCS_REG_CMD_STATUS))
        != IPMI_KCS_WRITE_STATE) {
      FreePool (Buffer);
      return EFI_NOT_READY;
    }

    // step 10
    if (EFI_ERROR (ClearOBF ())) {
      FreePool (Buffer);
      return EFI_NOT_READY;
    }
    /*
     * step 11, check for DATA completion if more than one byte;
     * if still need to be transered then go to step 7 and repeat
     */
  }

  // Step 12, WR_END  to CMD
  IoWrite8 (IPMI_KCS_REG_CMD_STATUS, IPMI_KCS_CC_WRITE_END);

    // Step 13. wait for IBF to get clear
  Status = WaitStatusClear (IPMI_KCS_IBF);
  if (EFI_ERROR (Status)) {
    FreePool (Buffer);
    return Status;
  }

  // Step 14. check state it should be WRITE_STATE, else exit with error
  if (IPMI_GET_STATE (IoRead8 (IPMI_KCS_REG_CMD_STATUS))
     != IPMI_KCS_WRITE_STATE) {
    FreePool (Buffer);
    return EFI_NOT_READY;
  }

  // STep 15
  if (EFI_ERROR (ClearOBF ())) {
    FreePool (Buffer);
    return EFI_NOT_READY;
  }

  // Step 16, write the last byte
  IoWrite8 (IPMI_KCS_REG_DATA_IN_OUT, *BufferPtr);
  FreePool (Buffer);
  return EFI_SUCCESS;
}

/**
  This function reads/recieves data from KCS port.
  Algorithm is based on flow chart provided in IPMI spec 2.0
  Figure 9-7, KCS Interface BMC to SMS Read Transfer Flow Chart

  @param[in]        DataBytes             Buffer to hold the read Data.
  @param[in, out]   Length                Number of Bytes read from KCS port.
  @retval           EFI_SUCCESS           The command byte stream was
                                          successfully submit to the device and
                                          a response was successfully received.
  @retval           EFI_NOT_FOUND         The command was not successfully sent
                                          to the device or a response was not
                                          successfully received from the
                                          device.
  @retval           EFI_NOT_READY         Ipmi Device is not ready for Ipmi
                                          command access.
  @retval           EFI_DEVICE_ERROR      Ipmi Device hardware error.
  @retval           EFI_TIMEOUT           The command time out.
  @retval           EFI_UNSUPPORTED       The command was not successfully set
                                          to the device.
  @retval           EFI_OUT_OF_RESOURCES  The resource allocation is out of
                                          resource or data size error.
**/

EFI_STATUS
IpmiKcsRead (
      OUT UINT8   *DataByte,
  IN  OUT UINT32  *Length
  )
{
  EFI_STATUS  Status;
  UINT32      ReadLength;

  ReadLength = 0;
  while (ReadLength < *Length) {
    // Step 1. wait for IBF to get clear
    Status = WaitStatusClear (IPMI_KCS_IBF);
    if (EFI_ERROR (Status)) {
      *Length = ReadLength;
      return Status;
    }
    // Step 2. check state it should be READ_STATE, else exit with error
    if (IPMI_GET_STATE (IoRead8 (IPMI_KCS_REG_CMD_STATUS))
       == IPMI_KCS_READ_STATE) {
      // Step 2.1.1 check of OBF to get clear
      Status = WaitStatusSet (IPMI_KCS_OBF);
      if (EFI_ERROR (Status)) {
        *Length = ReadLength;
        return Status;
      }
      // Step 2.1.2 read data from data out
      DataByte[ReadLength++] = IoRead8 (IPMI_KCS_REG_DATA_IN_OUT);
      Status = WaitStatusClear (IPMI_KCS_IBF);
      if (EFI_ERROR (Status)) {
        *Length = ReadLength;
        return Status;
      }
      // Step 2.1.3 Write READ byte to data in reg
      IoWrite8 (IPMI_KCS_REG_DATA_IN_OUT, IPMI_KCS_CC_READ);
    } else if (IPMI_GET_STATE (IoRead8 (IPMI_KCS_REG_CMD_STATUS))
               == IPMI_KCS_IDLE_STATE) {
      // Step 2.2.1
      Status = WaitStatusSet (IPMI_KCS_OBF);
      if (EFI_ERROR (Status)) {
        *Length = ReadLength;
        return Status;
      }
      // Step 2.2.2 read dummy data
      IoRead8 (IPMI_KCS_REG_DATA_IN_OUT); // Dummy read as per IPMI spec
      *Length = ReadLength;
      return EFI_SUCCESS;
    } else {
      *Length = ReadLength;
      return EFI_DEVICE_ERROR;
    }
  }
  *Length = ReadLength;
  return EFI_SUCCESS;
}

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
  )
{
  EFI_STATUS          Status;
  IPMI_KCS_RSP_HEADER RspHeader;
  UINT32              RspHeaderSize;

  if ((RequestData != NULL) &&
      (RequestDataSize <= 0)) {
    return EFI_INVALID_PARAMETER;
  }
  Status = IpmiKcsWrite (
    (NetFunction << 2),
    Command,
    RequestData,
    RequestDataSize
    );
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "IPMI KCS Write Failed with Status(%r) for NetFunction(0x%x),"\
      " Command(0x%x).\n",
      Status,
      NetFunction,
      Command
      ));
    return Status;
  }
  /*
   * Read the response header
   */
  RspHeaderSize = sizeof (IPMI_KCS_RSP_HEADER);
  Status = IpmiKcsRead ((UINT8 *)&RspHeader, &RspHeaderSize);
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "IPMI KCS read response header failed Status(%r), "\
      "RspNetFunction = 0x%x, "\
      "RspCommand = 0x%x \n",
      Status,
      RspHeader.RspNetFunc,
      RspHeader.RspCommand
      ));
    return (Status);
  }
  Status = IpmiKcsRead ((UINT8 *)ResponseData, ResponseDataSize);
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "IPMI KCS response read Failed with Status(%r) for NetFunction(0x%x),"\
      " Command(0x%x).\n",
      Status,
      NetFunction,
      Command
      ));
  }
  return Status;
}
