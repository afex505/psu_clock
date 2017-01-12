

#include "uart.h"
#include "main.h"

void uartInit(void)
{   
    UART_TRIS = 0;
    UART_ANSEL_DN();
            
    
    RP_UART = RP_U2TX; //U2TX
    U2MODE = (1<<15)|(1<<3); //ON|BRGH
    U2STA = (0b10<<14)|(1<<10);// UTXEN
            //int generated when buffer empty
            
    
#ifdef SLOW_DEBUG
    U2BRG = 10; //baud 38400
#else
    U2BRG = 4; //baud=2000000
#endif
    //U2BRG=36;
            
    
    //check transmit U2STAbits.TRMT
}


void SendSingleByte( uint8_t data )
{
   while(U2STAbits.UTXBF)
        ;
   
   U2TXREG = data;
    //UARTSendDataByte(UART_MODULE_ID, data);
}

// *****************************************************************************
// void UARTTxBuffer(char *buffer, UINT32 size)
// *****************************************************************************
void SendString(const char *buffer)
{
    while(*buffer)
    {
        while(U2STAbits.UTXBF)
            ;

        U2TXREG = *buffer;
        //UARTSendDataByte(UART_MODULE_ID, *buffer);

        buffer++;
    }

    while(!U2STAbits.TRMT)//UARTTransmissionHasCompleted(UART_MODULE_ID))
        ;
}

void SendDataBuffer(const char *buffer, uint32_t size)
{
    while(size)
    {
        while(U2STAbits.UTXBF)
            ;
        U2TXREG = *buffer;
        //UARTSendDataByte(UART_MODULE_ID, *buffer);

        buffer++;
        size--;
    }

    while(!U2STAbits.TRMT)//!UARTTransmissionHasCompleted(UART_MODULE_ID))
        ;
}


// send a single character
void UART_putc(uint8_t data)
{
	SendSingleByte(data);
}
// send a string
void UART_print_str(const char *str)
{
    uint8_t i = 0;

    while(str[i])
        UART_putc(str[i++]);
}

void UART_raw(char *buf, int numBytes)
{
    uint8_t i = 0;

    while(i < numBytes)
        UART_putc(buf[i++]);
}

void UART_print_dec(int ln, int x)
{
	int         i;
	int         neg = 0;
	int         rem;
	int        quot;
	//uint8_t
        char s[16];

    if (ln != 0)
        {
            // binary to ascii conversion
            if(x < 0)
            {
                s[0] = '-';
                x = -x;
            }
            else
                s[0] = '+';

            for(i = 0; i < ln; i++)
            {
                quot = x / 10;
                rem = x % 10;
                s[ln - i] = (uint8_t)(rem) + '0';
                x = quot;
            }
            // send the data character one by one
            //for(i = 0; i <= ln; i++)
            //    DEBUG_uart_putc(s[i]);
            // send the full data in the buffer
            SendDataBuffer(s, ln+1);
            UART_putc(' ');
	}
        else
        {
            if (x == 0)
            {
                UART_putc('0');
            }
            else
            {
                ln = 0;
                if(x < 0)
                {
                    neg = 1;
                    x = -x;
                }
                while(x)
                {
                    rem = x % 10;
                    s[ln++] = (uint8_t)(rem) + '0';
                    x = x / 10;
                }

                if (neg == 1)
                    UART_putc('-');
                else
                    UART_putc('+');

                //revert the data position
                for(i = 0 ; i < (ln/2) ; i ++)
                {
                    int tmp;

                    tmp = s[i];
                    s[i] = s[ln - i - 1];
                    s[ln - i - 1] = tmp;

                }
                SendDataBuffer(s, ln);
            }
            UART_putc(' ');
        }
}

void UART_print_dec_pos(int ln, int x)
{
    int i;
    int rem;
    char s[16];

    if (x == 0)
    {
        UART_putc('0');
    }
    else
    {
        ln = 0;
        while (x)
        {
            rem = x % 10;
            s[ln++] = (uint8_t) (rem) + '0';
            x = x / 10;
        }

        //revert the data position
        for (i = 0; i < (ln / 2); i++)
        {
            int tmp;

            tmp = s[i];
            s[i] = s[ln - i - 1];
            s[ln - i - 1] = tmp;

        }
        SendDataBuffer(s, ln);
    }

}


const uint8_t hex_lut[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
void UART_print_hex(int format, uint32_t x)
{

    int i=16;    // fill from right to left
    uint8_t buf[i];

    while( i>1 && format>0)
    {
       i--;
       buf[i] = hex_lut[x & 15];
       format -= 4;
       x = x >> 4;
    }

    UART_putc('0');
    UART_putc('x');
    while ( i<16 )  // write from left to right
    {
        UART_putc(buf[i]);
        i++;
    }
    UART_putc(' ');
}



