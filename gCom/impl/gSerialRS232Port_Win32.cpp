#include "gSerialRS232PortPrivate.h"

#ifdef WIN32

void gSerialRS232PortPrivate::purgeRx(){
    DWORD commErr;
    COMSTAT	commStat;

    //PurgeComm(hPort, PURGE_RXABORT | PURGE_RXCLEAR);
    ClearCommError(hComHandle, &commErr, &commStat);
    PurgeComm(hComHandle, PURGE_RXCLEAR);
    rxErrors	= 0;
    nextPosition	= 0;
    bytesRead = 0;
    buffer.setMemTo(0);
}
gs32 gSerialRS232PortPrivate::open(gu32 _baudrate, bool bParity, gs32 retries){


    COMMTIMEOUTS commTimeOuts;
    DCB dcb;
    ros.hEvent =  CreateEvent(NULL,true,false,NULL);
    wos.hEvent =  CreateEvent(NULL,true,false,NULL);
    baudRate=_baudRate;
    maxRetries=retries;

    //! Open the port
#ifdef UNICODE
    o->hComHandle = CreateFile(o->name.unicode(),GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,NULL);
#else
    o->hComHandle = CreateFile(o->name.ascii(),GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,NULL);
#endif
    if(o->hComHandle == INVALID_HANDLE_VALUE)
        return GSERIAL_PE_OPEN;
    GetCommState(o->hComHandle,&dcb);
    dcb.fBinary = true;
    dcb.BaudRate= baudRate;
    dcb.ByteSize = 8;
    if(!bParity)
        dcb.Parity = NOPARITY;
    else
        dcb.Parity = EVENPARITY;

    dcb.StopBits = ONESTOPBIT;
    dcb.fDsrSensitivity = FALSE;
    dcb.fOutX = FALSE;
    dcb.fInX = FALSE;
    dcb.fErrorChar = FALSE;
    dcb.fRtsControl =RTS_CONTROL_ENABLE;
    dcb.fAbortOnError = TRUE;
    dcb.fOutxCtsFlow = FALSE;
    dcb.fOutxDsrFlow = FALSE;
    SetCommState(o->hComHandle, &dcb);
    DWORD mask;
    if(!GetCommMask(o->hComHandle, &mask)) //get current event mask
                    mask = 0;
    mask|= EV_RXCHAR | EV_TXEMPTY | EV_BREAK	| EV_ERR  | EV_RING	| EV_RLSD  | EV_CTS	 | EV_DSR;


    SetCommMask(o->hComHandle,mask);

    EscapeCommFunction(o->hComHandle,SETDIR);
    FlushComPort();

    return GSERIAL_PE_OK;
}

void gSerialRS232PortPrivate::receiver(){
    int			x;
    DWORD		rxCount, fdwEventMask;
    COMSTAT		curStatus;
    gu8		*recBuf;
    HANDLE		commEvnt;
    OVERLAPPED	commSync;
    DWORD		commErr;
    COMSTAT		commStat;
    BOOL		waitState;

    memset(&commSync, 0, sizeof(OVERLAPPED));
    commEvnt = CreateEvent(NULL, TRUE, FALSE, "RX_THREAD_COMM_EVENT");
    if(commEvnt == INVALID_HANDLE_VALUE)
        return;
    commSync.hEvent = commEvnt;
    ClearCommError(hComHandle, &commErr, &commStat);

    while(opened){
        waitState = WaitCommEvent(hComHandle, &fdwEventMask, &commSync);
        if (!waitState){
            if(GetLastError() == ERROR_IO_PENDING)
            {
                //MessageBox(NULL, "io pend", NULL, MB_OK);
                WaitForSingleObject(commEvnt,3000);
                GetOverlappedResult(hComHandle, &commSync, &rxCount, TRUE);
            }
            else
                ExitThread( (DWORD)0 );  // dll exiting
        }

        if (fdwEventMask & (EV_RXCHAR | EV_BREAK | EV_ERR | EV_TXEMPTY)){
            mutex.lock();
            ClearCommError(hPort, &rxErrors, &curStatus);
            rxErrors &= CE_BREAK+CE_RXOVER+CE_OVERRUN+CE_RXPARITY+CE_FRAME+CE_IOE;
            if(rxErrors){
                if (rxErrors & (CE_BREAK | CE_IOE) )
                    FlushComPort();
                else
                    purgeRx();
            }
            else{
                HANDLE		rdEvnt;
                OVERLAPPED	rdSync;
                BOOL		rdRslt;
                memset(&rdSync, 0, sizeof(OVERLAPPED));
                rdEvnt = CreateEvent(NULL, TRUE, FALSE, "RD_SYNC_EVENT");
                //memset(&rdSync, 0, sizeof(OVERLAPPED));
                rdSync.hEvent = rdEvnt;
                if (x = curStatus.cbInQue){ // There is receive data //MessageBox(NULL, "rec data", NULL, MB_OK);
                    rxReceived = 1;
                    if (x > buffer.size())
                        x = buffer.size();
                    //ReadFile(hPort, recBuf, x, &rxCount, NULL);
                    recBuf = buffer.data()[bytesRead];
                    rdRslt = ReadFile(hComHandle, recBuf, x, &rxCount, &rdSync);
                    if(!rdRslt){
                        if(GetLastError() == ERROR_IO_PENDING)
                        {
                            if(!GetOverlappedResult(hComHandle, &rdSync, &rxCount, TRUE))
                                DisplaySystemError(GetLastError());
                        }
                    }
                    if (rxCount > (buffer.size() - rxIndex))
                        rxCount = (buffer.size() - rxIndex);
                    bytesRead += rxCount;
                    parent->onReceive();
                }

            }
            mutex.unlock();
        }
    }
}

void gSerialRS232PortPrivate::close(){
    char c;
    if(hComHandle == INVALID_HANDLE_VALUE)
        return;
    EscapeCommHandle(o->hComHandle,CLRDTR);
    FlushComPort();
    send(&c, 1);
    thread.wait();

    CloseHandle(o->hComHandle);
}

bool gSerialRS232PortPrivate::send(const char *data, gu32 size){
    DWORD		bWritten;
    HANDLE		writeEvnt;
    OVERLAPPED	writeSync;
    BOOL		rslt = TRUE;

    if ( hComHandle == INVALID_HANDLE_VALUE)
        return false;
    writeEvnt = CreateEvent(NULL, TRUE, FALSE, "WRT_SYNC");
    if(writeEvnt == INVALID_HANDLE_VALUE){
      return false;
    }
    memset(&writeSync, 0, sizeof(OVERLAPPED));
    writeSync.hEvent = writeEvnt;
    rslt = WriteFile( hComHandle,data,(DWORD)size,&bWritten,&writeSync);
    if(!rslt){
        if(GetLastError() == ERROR_IO_PENDING)
        {
            if(!GetOverlappedResult(hPort, &writeSync, &bWritten, TRUE))
                rslt=false;
            else
                if(bWritten == size)
                    rslt = true;
        }
    }
    CloseHandle(writeEvnt);
    return rslt;
}

#endif
