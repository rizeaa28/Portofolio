#include "gates.h"

#include <stdio.h>
#include <assert.h>

/* Task 1 - Bit by Bit */

uint8_t get_bit(uint64_t nr, uint8_t i)
{
    assert(i <= 8 * sizeof nr);

    uint8_t res = -1;

    /* TODO
     *
     * "res" should be 1 if the bit is active, else 0
     */

    if((nr & (1<<i)) != 0)
        res = 1;
    else
        res = 0;

    return res;
}


uint64_t flip_bit(uint64_t nr, uint8_t i)
{
    assert(i <= 8 * sizeof nr);

    uint64_t res = -1;

    /* TODO
     *
     * Return the "nr" with the ith bit flipped
     */

    nr = (nr ^ (1ULL<<i));

    res = nr;

    return res;
}


uint64_t activate_bit(uint64_t nr, uint8_t i)
{
    assert(i <= 8 * sizeof nr);

    uint64_t res = 0xFF;

    /* TODO
     *
     * Return the "nr" with the ith bit "1"
     */

    nr = (nr | (1ULL<<i));

    res = nr;

    return res;
}


uint64_t clear_bit(uint64_t nr, uint8_t i)
{
    assert(i <= 8 * sizeof nr);

    uint64_t res = -1;

    /* TODO
     *
     * Return the "nr" with the ith bit "0"
     */

    res = (nr & (~(1ULL<<i)));

    return res;
}


/* Task 2 - One Gate to Rule Them All */

uint8_t nand_gate(uint8_t a, uint8_t b)
{
    assert (a == 0 || a == 1);
    assert (b == 0 || b == 1);

    return !(a & b);
}


uint8_t and_gate(uint8_t a, uint8_t b)
{
    assert (a == 0 || a == 1);
    assert (b == 0 || b == 1);

    uint8_t res = -1;

    /* TODO - Use the nand gate to implement the and gate */

    res=nand_gate(nand_gate(a,b),nand_gate(a,b));

    return res;
}


uint8_t not_gate(uint8_t a)
{
    assert (a == 0 || a == 1);

    uint8_t res = -1;

    /* TODO - Use the nand gate to implement the not gate */

    res=nand_gate(a,a);

    return res;
}


uint8_t or_gate(uint8_t a, uint8_t b)
{
    assert (a == 0 || a == 1);
    assert (b == 0 || b == 1);

    uint8_t res = -1;

    /* TODO - Use the previously defined gates to implement the or gate */

    res=nand_gate(nand_gate(a,a),nand_gate(b,b));

    return res;
}


uint8_t xor_gate(uint8_t a, uint8_t b)
{
    assert (a == 0 || a == 1);
    assert (b == 0 || b == 1);

    uint8_t res = -1;

    /* TODO - Use the previously defined gates to implement the xor gate */

    res=nand_gate(nand_gate(a,nand_gate(a,b)),nand_gate(b,nand_gate(a,b)));

    return res;
}


/* Task 3 - Just Carry the Bit */

uint8_t full_adder(uint8_t a, uint8_t b, uint8_t c)
{
    assert (a == 0 || a == 1);
    assert (b == 0 || b == 1);
    assert (c == 0 || c == 1);

    uint8_t res = -1;

    /* TODO - implement the full_adder using the previous gates
     * Since the full_adder needs to provide 2 results, you should
     * encode the sum bit and the carry bit in one byte - you can encode
     * it in whatever way you like
     */

    res = 0;

    res = or_gate(and_gate(a,b),and_gate(xor_gate(a,b),c)); //carry-ul
    
    res = (res<<1);

    if(xor_gate(c,xor_gate(a,b))==1) //suma
        res = (res | 1);

    return res;
}


uint64_t ripple_carry_adder(uint64_t a, uint64_t b)
{
    
    uint64_t res = 0;

    /* TODO
     * Use the full_adder to implement the ripple carry adder
     * If there is ANY overflow while adding "a" and "b" then the
     * result should be 0
     */

    int i, carry=0, add=0, bit_a, bit_b;

    for(i=0; i<64; i++)
    {
        bit_a = (a & 1);
        a = (a>>1);
        bit_b = (b&1);
        b = (b>>1); //selectam bitii care trebuie adunati
        
        add = full_adder(bit_a,bit_b,carry); 

        if(add % 2 !=0) 
            res = (res | (1<<i)); //punem rezultatul sum in res

        add = (add>>1);

        carry = add; //retinem noul carry pentru urmatoare adunare
    }

    if(carry!=0)
        res = 0; //overflow

    return res;
}
