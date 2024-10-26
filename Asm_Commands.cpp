#include "PROGRAMM_ASM.h"


int ArgPush(ASM_t* Asm, char* buffer, int* size_of_code)
{

    char* cptr = NULL; // pointer of begin [
    char* eptr = NULL; // pointer of end ]
    char* plusptr = NULL; // pointer of +
    int32_t reg = 0;

    if((cptr = strchr(buffer, '[')) != NULL)
    {
        if((plusptr = strchr(buffer, '+')) != NULL)
        {
            Asm->code[*size_of_code] = (Code_t)RAM_REG_CONSTVAL;
            (*size_of_code)++;

            //fprintf(stderr, "## ARGTYPE = %d\n", RAM_REG_CONSTVAL);

            if((reg = Register_convert(buffer + 1)) == -1) // buffer + (cptr - buffer)
            {
                //fprintf(stderr, "SYNTAX ERROR\n");
                return SYNTAX_ERROR;
            }

            Asm->code[*size_of_code] = (Code_t)reg;
            (*size_of_code)++;

            if((eptr = strchr(buffer, ']')) == NULL)
            {
                //fprintf(stderr, "SYNTAX ERROR\n");
                return SYNTAX_ERROR;
            }


            Asm->code[*size_of_code] = (Code_t)atof(buffer + (eptr - plusptr + 1));
            (*size_of_code)++;
        }

        else if((reg = Register_convert(buffer + 1)) != -1) // buffer + (cptr - buffer)
        {
            Asm->code[*size_of_code] = (Code_t)RAM_REG;
            (*size_of_code)++;

            //fprintf(stderr, "## ARGTYPE = %d\n", RAM_REG);

            Asm->code[*size_of_code] = (Code_t)reg;
            (*size_of_code)++;
        }
        else
        {
            Asm->code[*size_of_code] = (Code_t)RAM_CONSTVAL;
            (*size_of_code)++;

            //fprintf(stderr, "## ARGTYPE = %d\n", RAM_CONSTVAL);

            Asm->code[*size_of_code] = (Code_t)atof(buffer + 1);
            (*size_of_code)++;
        }
    }
    else if((plusptr = strchr(buffer, '+')) != NULL)
    {
        Asm->code[*size_of_code] = (Code_t)REG_CONSTVAL;
        (*size_of_code)++;

        //fprintf(stderr, "## ARGTYPE = %d\n", REG_CONSTVAL);

        if((reg = Register_convert(buffer)) == -1)
        {
            //fprintf(stderr, "SYNTAX ERROR\n");
            return SYNTAX_ERROR;
        }

        Asm->code[*size_of_code] = (Code_t)reg;
        (*size_of_code)++;

        Asm->code[*size_of_code] = (Code_t)atof(buffer + (plusptr - buffer) + 1);
        (*size_of_code)++;

    }
    else if((reg = Register_convert(buffer)) != -1)
    {
        Asm->code[*size_of_code] = (Code_t)REG;
        (*size_of_code)++;

        //fprintf(stderr, "## ARGTYPER = %d\n", REG);

        Asm->code[*size_of_code] = (Code_t)reg;
        (*size_of_code)++;
    }
    else
    {
        Asm->code[*size_of_code] = (Code_t)CONSTVAL;
        (*size_of_code)++;
        Asm->code[*size_of_code] = (Code_t)atof(buffer);
        (*size_of_code)++;

        //fprintf(stderr, "## ARGTYPE = %d\n", CONSTVAL);
    }
    return 0;
}



int ArgJump(ASM_t* Asm, char* buffer, int* size_of_code)
{
    fprintf(stderr, "## JMP ARG: %s\n", buffer);
    char* lmarker = NULL;
    if((lmarker = strchr(buffer, ':')) == NULL)
    {

        uint32_t jumpIP =  atoi(buffer);
        memcpy(Asm->code + *size_of_code, &jumpIP, sizeof(jumpIP));
        *size_of_code += 1;

        fprintf(stderr, "## JMP ARG ADDED: %d\n", atoi(buffer));
        return 0;
    }

    else
    {
        uint32_t labVal = 0;
        *lmarker = '\0';

        if((labVal = FindLabel(&Asm->LTable, buffer)) == -1) // LABEL IS NOT IN THE TABLE YET
        {
            strncpy(Asm->LTable.labAr[Asm->LTable.lnum].name, buffer, COMMANDNAME_MAX);
            *size_of_code += 1;
            Asm->LTable.lnum++;
            fprintf(stderr, "## LABEL NAME ADDED: %s\n", buffer);
            LTDump(&Asm->LTable);
            return 0;
        }

        else
        {
            fprintf(stderr, "labval = %d\n", labVal);
            fprintf(stderr, "lable = %lu\n", sizeof(Asm->LTable.labAr[labVal].ipTarg));
            fprintf(stderr, "$$$ IP TARGET = %lg\n", Asm->LTable.labAr[labVal].ipTarg);
            fprintf(stderr, "$$$ code = %lg\n", *(Asm->code + *size_of_code));
            fprintf(stderr, "$$$ code ptr = %p\n", Asm->code + *size_of_code);
            memcpy(Asm->code + *size_of_code, &Asm->LTable.labAr[labVal].ipTarg, sizeof(Asm->LTable.labAr[labVal].ipTarg));
            fprintf(stderr, "$$$ code = %lg\n", *(Asm->code + *size_of_code));
            fprintf(stderr, "$$$ code ptr = %p\n", Asm->code + *size_of_code);
            (*size_of_code) += 1;
            return 0;
        }
    }
}

int ArgLabel(ASM_t* Asm, char* buffer, int size_of_code, char* lmarker)
{
        int labVal = 0;
        *lmarker = '\0';
        if((labVal = FindLabel(&Asm->LTable, buffer)) == -1)
        {
            strcpy(Asm->LTable.labAr[Asm->LTable.lnum].name, buffer);
            Asm->LTable.labAr[Asm->LTable.lnum].ipTarg = size_of_code;
            fprintf(stderr, "lable =  %lg\n", Asm->LTable.labAr[Asm->LTable.lnum].ipTarg);
            Asm->LTable.lnum++;

            fprintf(stderr, "## LABEL ADDED: %s\n", buffer);
            LTDump(&Asm->LTable);
            fprintf(stderr, "## READ: %s\n", buffer);
        }

        else if(Asm->LTable.labAr[labVal].ipTarg == -1)
        {
            Asm->LTable.labAr[labVal].ipTarg = size_of_code;

            fprintf(stderr, "## LABEL VALUE ADDED: %s = %lg\n", buffer,Asm->LTable.labAr[labVal].ipTarg);
            LTDump(&Asm->LTable);

        }

        return 0;
}


int ArgPop(ASM_t* Asm, char* buffer, int* size_of_code)
{
    char* cptr = NULL;
    char* eptr = NULL;
    char* plusptr = NULL;
    int32_t reg = 0;

    if((cptr = strchr(buffer, '[')) != NULL)
    {
        if((plusptr = strchr(buffer, '+')) != NULL)
        {
            Asm->code[*size_of_code] = (Code_t)RAM_REG_CONSTVAL;
            (*size_of_code)++;

            //ON_DEBUG(fprintf(stderr, "## ARGTYPE = %d\n", RAM_REG_CONSTVAL));

            if((reg = Register_convert(buffer + 1)) == -1) // buffer + (cptr - buffer)
            {
                fprintf(stderr, "SYNTAX ERROR\n");
                return SYNTAX_ERROR;
            }

            Asm->code[*size_of_code] = (Code_t)reg;
            (*size_of_code)++;

            if((eptr = strchr(buffer, ']')) == NULL)
            {
                fprintf(stderr, "SYNTAX ERROR\n");
                return SYNTAX_ERROR;
            }


            Asm->code[*size_of_code] = (Code_t)atof(buffer + (eptr - plusptr + 1));
            (*size_of_code)++;
        }

        else if((reg = Register_convert(buffer + 1)) != -1) // buffer + (cptr - buffer)
        {
            Asm->code[*size_of_code] = (Code_t)RAM_REG;
            (*size_of_code)++;

            //ON_DEBUG(fprintf(stderr, "## ARGTYPE = %d\n", RAM_REG));

            Asm->code[*size_of_code] = (Code_t)reg;
            (*size_of_code)++;
        }

        else
        {
            Asm->code[*size_of_code] = (Code_t)RAM_CONSTVAL;
            (*size_of_code)++;

            //ON_DEBUG(fprintf(stderr, "## ARGTYPE = %d\n", RAM_CONSTVAL));

            Asm->code[*size_of_code] = (Code_t)atof(buffer + 1);
            (*size_of_code)++;
        }
    }

    else
    {
        if((reg = Register_convert(buffer)) == -1)
        {
            return SYNTAX_ERROR;
        }

        Asm->code[*size_of_code] = (Code_t)REG;
        (*size_of_code)++;
        Asm->code[*size_of_code] = (Code_t)reg;
        (*size_of_code)++;
    }

    return 0;
}
