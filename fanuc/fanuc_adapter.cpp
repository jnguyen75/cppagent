/*
* Copyright (c) 2008, AMT – The Association For Manufacturing Technology (“AMT”)
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the name of the AMT nor the
*       names of its contributors may be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* DISCLAIMER OF WARRANTY. ALL MTCONNECT MATERIALS AND SPECIFICATIONS PROVIDED
* BY AMT, MTCONNECT OR ANY PARTICIPANT TO YOU OR ANY PARTY ARE PROVIDED "AS IS"
* AND WITHOUT ANY WARRANTY OF ANY KIND. AMT, MTCONNECT, AND EACH OF THEIR
* RESPECTIVE MEMBERS, OFFICERS, DIRECTORS, AFFILIATES, SPONSORS, AND AGENTS
* (COLLECTIVELY, THE "AMT PARTIES") AND PARTICIPANTS MAKE NO REPRESENTATION OR
* WARRANTY OF ANY KIND WHATSOEVER RELATING TO THESE MATERIALS, INCLUDING, WITHOUT
* LIMITATION, ANY EXPRESS OR IMPLIED WARRANTY OF NONINFRINGEMENT,
* MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. 

* LIMITATION OF LIABILITY. IN NO EVENT SHALL AMT, MTCONNECT, ANY OTHER AMT
* PARTY, OR ANY PARTICIPANT BE LIABLE FOR THE COST OF PROCURING SUBSTITUTE GOODS
* OR SERVICES, LOST PROFITS, LOSS OF USE, LOSS OF DATA OR ANY INCIDENTAL,
* CONSEQUENTIAL, INDIRECT, SPECIAL OR PUNITIVE DAMAGES OR OTHER DIRECT DAMAGES,
* WHETHER UNDER CONTRACT, TORT, WARRANTY OR OTHERWISE, ARISING IN ANY WAY OUT OF
* THIS AGREEMENT, USE OR INABILITY TO USE MTCONNECT MATERIALS, WHETHER OR NOT
* SUCH PARTY HAD ADVANCE NOTICE OF THE POSSIBILITY OF SUCH DAMAGES.
*/

#include "internal.hpp"
#include "Fwlib32.h"
#include "fanuc_adapter.hpp"

FanucAdapter::FanucAdapter(int aPort, const char *aDeviceIP, int aDevicePort) : 
  Adapter(aPort), 
  mAlarm("alarm"), mPower("power"), mExecution("execution"), mLine("line"),
  mXact("Xact"), mYact("Yact"), mZact("Zact"), 
  mXcom("Xcom"), mYcom("Ycom"), mZcom("Zcom"),
  mSpindleSpeed("spindle_speed"), mPathFeedrate("path_feedrate"), 
  mMode("mode")
{
  addDatum(mAlarm);
  addDatum(mPower);
  addDatum(mExecution);
  addDatum(mLine);
  addDatum(mXact);
  addDatum(mYact);
  addDatum(mZact);
  addDatum(mXcom);
  addDatum(mYcom);
  addDatum(mZcom);
  addDatum(mSpindleSpeed);
  addDatum(mPathFeedrate);
  addDatum(mMode);

  mDevicePort = aDevicePort;
  mDeviceIP = aDeviceIP;
  mConnected = false;
}

FanucAdapter::~FanucAdapter()
{
  disconnect();
}

void FanucAdapter::gatherDeviceData()
{
  if (!mConnected)
    connect();
  else
  {
    getPositions();
    getSpeeds();
    getLine();
    getStatus();
  }
}

void FanucAdapter::disconnect()
{
  if (!mConnected)
  {
    mPower.setValue(Power::eOFF);
    cnc_freelibhndl(mFlibhndl);  
    mConnected = false;
  }
}

void FanucAdapter::connect()
{
  short ret = ::cnc_allclibhndl3(mDeviceIP, mDevicePort, 10, &mFlibhndl);
  printf("Result: %d", ret);
  if (ret == EW_OK) 
  {
      mConnected = true;
      mPower.setValue(Power::eON);
  }
  else
  {
    mPower.setValue(Power::eOFF);
    mConnected = false;
    Sleep(5000);
  }
}

void FanucAdapter::getPositions()
{
  if (!mConnected)
    return;

  ODBPOS PosData[MAX_AXIS]; //Position Data
  short data_num = 3;
  short ret = cnc_rdposition(mFlibhndl, -1, &data_num, &PosData[0]);
  if (ret == EW_OK)
  {
	  mXact.setValue(PosData[0].abs.data / pow( (long double)10.0,( long double) PosData[0].abs.dec));
    mYact.setValue(PosData[1].abs.data / pow( (long double)10.0,(long double) PosData[0].abs.dec  ));
    mZact.setValue(PosData[2].abs.data / pow( (long double)10.0,(long double) PosData[0].abs.dec  ));
  }
  else
  {
    disconnect();
  }
}

void FanucAdapter::getSpeeds()
{
  if (!mConnected)
    return;
    
  ODBSPEED speed;
  short ret = cnc_rdspeed(mFlibhndl, -1, &speed);
  if (ret == EW_OK)
  {
    mPathFeedrate.setValue(speed.actf.data);
    mSpindleSpeed.setValue(speed.acts.data);
  }
  else
  {
    disconnect();
  }
}

void FanucAdapter::getLine()
{
  if (!mConnected)
    return;
  
  ODBSEQ block;
  int ret = cnc_rdseqnum(mFlibhndl, &block );
  if (ret == EW_OK)
  {
      mLine.setValue(block.data);
  }
  else
  {
    disconnect();
  }
}

void FanucAdapter::getStatus()
{
  if (!mConnected)
    return;

  ODBST status;
  int ret = cnc_statinfo(mFlibhndl, &status);
  if (ret == EW_OK)
  {
    if (status.manual != 0) // other than no selection
      mMode.setValue(ControllerMode::eMANUAL);
    else if (status.aut == 1) // MDI for aut
      mMode.setValue(ControllerMode::eMANUAL_DATA_INPUT);
    else if (status.aut != 0) // Other than no selection or MDI
      mMode.setValue(ControllerMode::eAUTOMATIC);
      
    if (status.run == 0) // STOP
      mExecution.setValue(Execution::eIDLE);
    else if (status.run == 1 || status.motion == 3) // HOLD or motion is Wait
      mExecution.setValue(Execution::ePAUSED);
    else if (status.run == 2) // STaRT
      mExecution.setValue(Execution::eEXECUTING);
      
    if (status.emergency == 1)
      mAlarm.setValue(Alarm::eESTOP, "EMerGency", Alarm::eCRITICAL, Alarm::eACTIVE, "EMerGency Status Set");
    else if (status.alarm == 1)
      mAlarm.setValue(Alarm::eOTHER, "ALarM", Alarm::eERROR, Alarm::eACTIVE, "ALarM Status Set");
    else if (status.warning == 1)
      mAlarm.setValue(Alarm::eOTHER, "WaRNing", Alarm::eWARNING, Alarm::eACTIVE, "WaRNing Status Set");
  }
  else
  {
    disconnect();
  }
}