int iError = 0;
        Stack stack = newStack(stack);// empty list to store operators
        Element element; //struct element passed into categorize function
        categorize(&element); //struct that has token category and precedence
        pszInfix = getToken(pszInfix,element.szToken,MAX_TOKEN);
        while(pszInfix != NULL)
        {
                switch(element.iCategory)
                {
                        case CAT_OPERAND:
                                addOut(out,element);
                        case CAT_LPAREN:
                                push(stack,element);
                        case CAT_RPAREN:
                                if(isEmpty(stack))
                                {
                                        iError  = 802;
                                        break;
                                }
                                while(!isEmpty(stack))
                                {
                                pop(stack);
                                if(strcmp(element.szToken,"(") == 0)
                                        return 0;
                                        break;
                                        addOut(out,element);
                                }
                        case  CAT_OPERATOR:
                                while(!isEmpty(stack))
                                {
                                if(element.iPrecedence > topElement(stack).iPrecedence)
                                        push(stack,element);
                                }
                                 pop(stack);
                                 addOut(out,element);
                }

                pszInfix = getToken(pszInfix,element.szToken,MAX_TOKEN);
        }
        while(!isEmpty(stack))
                                                                {
                pop(stack);
                if(strcmp(element.szToken,"(") == 0)
                        iError  = 801;

        }
        freeStack(stack);
        return iError