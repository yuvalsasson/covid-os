#include "serial.h"

void serial_configure_baud_rate(unsigned short com, unsigned short divisor)
{
    outb(SERIAL_LINE_CNTROL(com), 1 << 7); // Enable DLAB
    outb(SERIAL_DIVISOR_LSB(com), divisor & 0xFF); 
    outb(SERIAL_DIVISOR_MSB(com), (divisor >> 8) & 0xFF);
    outb(SERIAL_LINE_CNTROL(com), 0); // Clear DLAB flag
}

void serial_init_port(unsigned short com)
{
    /*
        ---------------------------------
        | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
        | d | b |     p     | s |   l   |
        | 0 | 0 | 0 | 0 | 0 | 0 |   3   | = 3
        ---------------------------------
        d - dlab bit
        p - pairity
        l - length of the data (0: 5, 1: 6,2: 7,3: 8)
        b - break signal
        s - number of stop bits (0: 1, 1: 2)
    */
    /* 8bit no parity */
    outb(SERIAL_LINE_CNTROL(com), 0x3);

    /* 
        Configure FIFO control with 0xC7
        - Enable FIFO
        - Clear buffers
        - Use 14 bytes as size of queue
     */ 
    outb(SERIAL_FIFO_CONTROL(com), 0xC7);

    /* 
        Configure Modem to send data only
    */
   outb(SERIAL_MODEM_CONTROL(com), 0x3);

}

int serial_is_fifo_empty(unsigned short com)
{
    // 0x20 = 0001 0000
    return inb(SERIAL_LINE_STATUS(com)) & 0x20;
}

void serial_write_ch(unsigned short com, unsigned char data)
{
    while (serial_is_fifo_empty(com) == 0)
    {
        ;
    }
    outb(SERIAL_DATA_PORT(com), data); 
}

void serial_write(unsigned short com, unsigned char * data, unsigned int length)
{
    for (unsigned int i = 0; i < length; i++)
    {
        serial_write_ch(com, data[i]);
    }
    
}