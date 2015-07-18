#include "instructionset.h"

InstructionSet::InstructionSet()
{
        unsigned int* REGSPTR = (unsigned int*)&CPUREGS;
        CPUREGS.EAX = 0x0;
        __get_cpuid(0x0, &CPUREGS.EAX, &CPUREGS.EBX, &CPUREGS.ECX, &CPUREGS.EDX);
        maxleaf = 0x80000000 + CPUREGS.EAX;
    //if(maxleaf < 0x80000004)
        //return; //Unsupported brand string

    for(int x = 0x80000002; x <= maxleaf; x++)
    {

        __get_cpuid(x, &CPUREGS.EAX, &CPUREGS.EBX, &CPUREGS.ECX, &CPUREGS.EDX);
        //memcpy((char*)(cpusignature + (x - 0x80000002)*16), (char*)&CPUREGS, sizeof(unsigned int)*4);

    }

    __get_cpuid(0x80000001, &CPUREGS.EAX, &CPUREGS.EBX, &CPUREGS.ECX, &CPUREGS.EDX);

    std::bitset<sizeof(unsigned int)*8> EDXBITS(CPUREGS.EDX);

        PFLAGS.PYSICAL_ADDRESS_E = EDXBITS.test(6);
        PFLAGS.MULTIPROC_CAPABLE = EDXBITS.test(19);
        PFLAGS.EXT_MMX = EDXBITS.test(22);
        PFLAGS.MMX_INSTRUCTIONS = EDXBITS.test(23);
        PFLAGS.LONGMODE = EDXBITS.test(29);
        PFLAGS.EXT_3DNOW = EDXBITS.test(30);
        PFLAGS.B3DNOW = EDXBITS.test(31);
        PFLAGS.MULTIPROCESSOR_CAPABLE = EDXBITS.test(19);


}
