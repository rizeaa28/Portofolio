#include "communication.h"
#include "util_comm.h"

#include <stdio.h>


/* Task 1 - The Beginning */

void send_byte_message(void)
{
    /* TODO
     * Send the encoding of the characters: R, I, C, K
     */

    send_squanch(18); //R
    send_squanch(9); //I
    send_squanch(3); //C
    send_squanch(11); //K
}


void recv_byte_message(void)
{
    /* TODO
     * Receive 5 encoded characters, decode them and print
     * them to the standard output (as characters)
     *
     * ATTENTION!: Use fprintf(stdout, ...)
     */
    
    int a = 0, i;

    for(i=0; i<5; i++)
    {
        a = recv_squanch();

        fprintf(stdout, "%c", (a+64)); //decodarea unui caracter pentru afisare
    }


}


void comm_byte(void)
{
    /* TODO
     * Receive 10 encoded characters and send each character (the character is
     * still encoded) 2 times
     */

    int a, i;

    for(i=0; i<10; i++)
    {
        a = recv_squanch();

        send_squanch(a);
        send_squanch(a);
    }
}


/* Task 2 - Waiting for the Message */

void send_message(void)
{
    /* TODO
     * Send the message: HELLOTHERE
     * - send the encoded length
     * - send each character encoded
     */

    send_squanch(10<<2); //codarea lungimii
    send_squanch(8);
    send_squanch(5);
    send_squanch(12);
    send_squanch(12);
    send_squanch(15);
    send_squanch(20);
    send_squanch(8);
    send_squanch(5);
    send_squanch(18);
    send_squanch(5);

}


void recv_message(void)
{
    /* TODO
     * Receive a message:
     * - the first value is the encoded length
     * - length x encoded characters
     * - print each decoded character
     * 
     * ATTENTION!: Use fprintf(stdout, ...)
     */

    int l, i, a;

    l = recv_squanch();
    l = (l>>2);
    l = (l & 15); //am decodat lungimea

    fprintf(stdout, "%d", l); 

    for(i=0; i<l; i++)
    {
        a = recv_squanch();

        fprintf(stdout, "%c", (a+64)); //afisare
    }
}


void comm_message(void)
{
    /* TODO
     * Receive a message from Rick and do one of the following depending on the
     * last character from the message:
     * - 'P' - send back PICKLERICK
     * - anything else - send back VINDICATORS
     * You need to send the messages as you did at the previous tasks:
     * - encode the length and send it
     * - encode each character and send them
     */
    
    int ch, l, i;
    l = recv_squanch();
    l = (l>>2);
    l = (l & 15);

    for(i=0; i<l; i++)
    {
        ch = recv_squanch(); //am aflat ultimul caracter al mesajului primit
    }
    
    
    if(ch == 16)
    {
        send_squanch(10<<2); //PICKLERICK
        send_squanch(16);
        send_squanch(9);
        send_squanch(3);
        send_squanch(11);
        send_squanch(12);
        send_squanch(5);
        send_squanch(18);
        send_squanch(9);
        send_squanch(3);
        send_squanch(11);
    }
    else
    {
        send_squanch(11<<2); //VINDICATORS
        send_squanch(22);
        send_squanch(9);
        send_squanch(14);
        send_squanch(4);
        send_squanch(9);
        send_squanch(3);
        send_squanch(1);
        send_squanch(20);
        send_squanch(15);
        send_squanch(18);
        send_squanch(19);
    }
}


/* Task 3 - In the Zone */

void send_squanch2(uint8_t c1, uint8_t c2)
{
    /* TODO
     * Steps:
     * - "merge" the character encoded in c1 and the character encoded in c2
     * - use send_squanch to send the newly formed byte
     */

    int i, mesaj=0;

    for(i=0; i<8; i=i+2)
    {
        mesaj = (mesaj | ((c2 & 1)<<i));
        mesaj = (mesaj | ((c1 & 1)<<(i+1)));
        c2 = (c2>>1);
        c1 = (c1>>1); //am selectat fiecare bit din c1 si c2 si i-am pozitionat in cariabila "mesaj" conform codificarii din cerinta
    }

    send_squanch(mesaj); //am trimis mesajul codificat
}


uint8_t decode_squanch2(uint8_t c)
{
    /*
     * Decode the given byte:
     * - split the two characters as in the image from ocw.cs.pub.ro
     */

    uint8_t res = 0;

    /* TODO */

    int i, poz=0;

    for(i=0; i<8; i++)
    {
        if(i%2==0) //bitii de pe pozitii pare apartin primului caracter codificat
        {
            res = (res | ((c & 1) << poz));
            c = (c >> 1);
        }
        else
        {
            res = (res | ((c & 1) << (poz+4)));
            c = (c >> 1);
            poz = poz+1;
        }
    }

    return res;
}
