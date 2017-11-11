#ifndef GSERIALRS232PORT_H
#define GSERIALRS232PORT_H
#include <gString/gString.h>
#define GSERIALPORT_DEFAULT_BUFFER_SIZE 512
namespace gfw{
namespace gcom{
    enum GSERIAL_PORT_ERROR{
        //!No error
        GSERIAL_PE_OK,
        //! Port is empty due to calling clear
        GSERIAL_PE_EMPTY,
        //! Could not open port
        GSERIAL_PE_OPEN,

    };

    class gSerialRS232;
    //! Defines a single serial port
    class SHARED_GFW gSerialRS232Port:public gBaseShared{
    public:
        //! Default constructor
        gSerialRS232Port();
        //! Specialized constructor
        gSerialRS232Port(const gString &name, gu32 bufferSize,gSerialRS232 *parent);
        //! Default copy constructor
        gSerialRS232Port(const gSerialRS232Port &other);
        //!Destructor
        ~gSerialRS232Port();
        //! Assign
        gSerialRS232Port &operator = (const gSerialRS232Port &other);
        //! Manually creates the port
        void create(const gString &name, gu32 bufferSize, gSerialRS232 *parent);
        //! Opens this serial port at a given baudrate
        gs32 open(gu32 baudRate, bool bParity, gs32 retries);
        //! Closes the serial port
        void close();
        //! Gets the string name of this serial port "COM1" for windows or TTY for Linux
        const gString &getName() const;
        //! Sends byte array to the port
        bool send(const gByteArray &array);
        //! Sends a string through the port
        bool send(const gString &value);
        //! Gets the parent or holder of this port
        gSerialRS232 *parent() const;
        //! Returns whether the port is open or not
        bool isOpened() const;
        //! Clears the internal buffer
        void clearBuffer();
        //! Gets a copy of total bytes from buffer
        gByteArray getBytesRead() const;
        //! Gets the count of bytes read
        gu32 getBytesReadCount() const;
    };
}
}
#endif // GSERIALRS232PORT_H
