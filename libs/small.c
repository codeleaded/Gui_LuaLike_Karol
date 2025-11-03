#include "/home/codeleaded/System/Static/Library/AlxCallStack.h"
#include "/home/codeleaded/System/Static/Library/AlxExternFunctions.h"
#include "/home/codeleaded/System/Static/Library/LuaLikeDefines.h"
//#include "/home/codeleaded/Hecke/C/Cmd_Scripter/src/LuaLike.h"


typedef int Small;

Number Implementation_SmallOf(Scope* s,Token* a){
    Number n = NUMBER_PARSE_ERROR;
    if(a->tt==TOKEN_STRING){
        Variable* a_var = Scope_FindVariable(s,a->str);
        if(a_var){
            if(CStr_Cmp(a_var->typename,"small"))   n = *(Small*)Variable_Data(a_var);
            if(CStr_Cmp(a_var->typename,"int"))     n = *(Number*)Variable_Data(a_var);
        }else{
            printf("[Small_Number]: 1. Arg: Variable %s doesn't exist!\n",a->str);
        }
    }else if(a->tt==TOKEN_NUMBER){
        n = Number_Parse(a->str);
    }else{
        printf("[Small_Number]: 1. Arg: %s is not a small type!\n",a->str);
    }
    return n;
}

Token Small_Small_Handler_Ass(Scope* s,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    printf("(small,small) ASS: %s = %s\n",a->str,b->str);

    Number n2 = Implementation_SmallOf(s,b);
    
    if(a->tt==TOKEN_STRING){
        Variable* a_var = Scope_FindVariable(s,a->str);
        if(a_var){
            Variable_PrepairFor(a_var,sizeof(Small),"small",NULL,NULL);
            Variable_SetTo(a_var,(Small[]){ n2 });
        }else{
            Scope_BuildInitVariableRange(s,a->str,"small",s->range-1,(Small[]){ n2 });
        }
    }else{
        printf("[Small_Ass]: 1. Arg: %s is not a variable type!\n",a->str);
    }

    Number res = n2;

    char* resstr = Number_Get(res);
    return Token_Move(TOKEN_NUMBER,resstr);
}
Token Small_Small_Handler_Add(Scope* s,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    printf("ADD: %s + %s\n",a->str,b->str);

    Number n1 = Implementation_SmallOf(s,a);
    Number n2 = Implementation_SmallOf(s,b);
    Number res = n1 + n2;

    char* resstr = Number_Get(res);
    return Token_Move(TOKEN_NUMBER,resstr);
}
Token Small_Small_Handler_Sub(Scope* s,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    printf("SUB: %s - %s\n",a->str,b->str);

    Number n1 = Implementation_SmallOf(s,a);
    Number n2 = Implementation_SmallOf(s,b);
    Number res = n1 - n2;

    char* resstr = Number_Get(res);
    return Token_Move(TOKEN_NUMBER,resstr);
}
Token Small_Small_Handler_Mul(Scope* s,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    printf("MUL: %s * %s\n",a->str,b->str);

    Number n1 = Implementation_SmallOf(s,a);
    Number n2 = Implementation_SmallOf(s,b);
    Number res = n1 * n2;

    char* resstr = Number_Get(res);
    return Token_Move(TOKEN_NUMBER,resstr);
}
Token Small_Small_Handler_Div(Scope* s,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    printf("DIV: %s / %s\n",a->str,b->str);

    Number n1 = Implementation_SmallOf(s,a);
    Number n2 = Implementation_SmallOf(s,b);

    Number res = 0;
    if(n2!=0) res = n1 / n2;
    else{
        printf("[Enviroment]: Error: DIV by Zero\n");
    }

    char* resstr = Number_Get(res);
    return Token_Move(TOKEN_NUMBER,resstr);
}
Token Small_Small_Handler_Neg(Scope* s,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);

    printf("NEG: -%s\n",a->str);

    Number n1 = Implementation_SmallOf(s,a);
    Number res = -n1;

    char* resstr = Number_Get(res);
    return Token_Move(TOKEN_NUMBER,resstr);
}
Token Small_Small_Handler_Equ(Scope* s,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    printf("EQU: %s == %s\n",a->str,b->str);

    Number n1 = Implementation_SmallOf(s,a);
    Number n2 = Implementation_SmallOf(s,b);
    Boolean res = n1 == n2;

    char* resstr = Boolean_Get(res);
    return Token_Move(TOKEN_BOOL,resstr);
}
Token Small_Small_Handler_Les(Scope* s,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    printf("LES: %s < %s\n",a->str,b->str);

    Number n1 = Implementation_SmallOf(s,a);
    Number n2 = Implementation_SmallOf(s,b);
    Boolean res = n1 < n2;

    char* resstr = Boolean_Get(res);
    return Token_Move(TOKEN_BOOL,resstr);
}
Token Small_Small_Handler_Grt(Scope* s,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    printf("GRT: %s > %s\n",a->str,b->str);

    Number n1 = Implementation_SmallOf(s,a);
    Number n2 = Implementation_SmallOf(s,b);
    Boolean res = n1 > n2;

    char* resstr = Boolean_Get(res);
    return Token_Move(TOKEN_BOOL,resstr);
}
Token Small_Small_Handler_Leq(Scope* s,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    printf("LEQ: %s <= %s\n",a->str,b->str);

    Number n1 = Implementation_SmallOf(s,a);
    Number n2 = Implementation_SmallOf(s,b);
    Boolean res = n1 <= n2;

    char* resstr = Boolean_Get(res);
    return Token_Move(TOKEN_BOOL,resstr);
}
Token Small_Small_Handler_Grq(Scope* s,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    printf("GRQ: %s >= %s\n",a->str,b->str);

    Number n1 = Implementation_SmallOf(s,a);
    Number n2 = Implementation_SmallOf(s,b);
    Boolean res = n1 >= n2;

    char* resstr = Boolean_Get(res);
    return Token_Move(TOKEN_BOOL,resstr);
}
Token Small_Handler_Cast(Scope* s,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);

    //printf("CAST: %s\n",a->str);

    Number res = Implementation_SmallOf(s,a);
    char* resstr = Number_Get(res);
    return Token_Move(TOKEN_CONSTSTRING_DOUBLE,resstr);
}

void Ex_Packer(ExternFunctionMap* Extern_Functions,Vector* funcs,Scope* s){//Vector<CStr>
    TypeMap_PushContained(&s->types,funcs,
        Type_New("small",8,OperatorInterationMap_Make((OperatorInterater[]){
            OperatorInterater_Make((CStr[]){ NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_LUALIKE_NEG,(Token(*)(void*,Token*,Vector*))Small_Small_Handler_Neg),
                OperatorDefiner_New(TOKEN_CAST,(Token(*)(void*,Token*,Vector*))Small_Handler_Cast),
                OperatorDefiner_New(TOKEN_INIT,NULL),
                OperatorDefiner_New(TOKEN_DESTROY,NULL),
                OPERATORDEFINER_END
            })),
            OperatorInterater_Make((CStr[]){ "small",NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_LUALIKE_ASS,(Token(*)(void*,Token*,Vector*))Small_Small_Handler_Ass),
                OperatorDefiner_New(TOKEN_LUALIKE_ADD,(Token(*)(void*,Token*,Vector*))Small_Small_Handler_Add),
                OperatorDefiner_New(TOKEN_LUALIKE_SUB,(Token(*)(void*,Token*,Vector*))Small_Small_Handler_Sub),
                OperatorDefiner_New(TOKEN_LUALIKE_MUL,(Token(*)(void*,Token*,Vector*))Small_Small_Handler_Mul),
                OperatorDefiner_New(TOKEN_LUALIKE_DIV,(Token(*)(void*,Token*,Vector*))Small_Small_Handler_Div),
                OperatorDefiner_New(TOKEN_LUALIKE_EQU,(Token(*)(void*,Token*,Vector*))Small_Small_Handler_Equ),
                OperatorDefiner_New(TOKEN_LUALIKE_LES,(Token(*)(void*,Token*,Vector*))Small_Small_Handler_Les),
                OperatorDefiner_New(TOKEN_LUALIKE_GRT,(Token(*)(void*,Token*,Vector*))Small_Small_Handler_Grt),
                OperatorDefiner_New(TOKEN_LUALIKE_LEQ,(Token(*)(void*,Token*,Vector*))Small_Small_Handler_Leq),
                OperatorDefiner_New(TOKEN_LUALIKE_GRQ,(Token(*)(void*,Token*,Vector*))Small_Small_Handler_Grq),
                OPERATORDEFINER_END
            })),
            OperatorInterater_Make((CStr[]){ "int",NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_LUALIKE_ASS,(Token(*)(void*,Token*,Vector*))Small_Small_Handler_Ass),
                OperatorDefiner_New(TOKEN_LUALIKE_ADD,(Token(*)(void*,Token*,Vector*))Small_Small_Handler_Add),
                OperatorDefiner_New(TOKEN_LUALIKE_SUB,(Token(*)(void*,Token*,Vector*))Small_Small_Handler_Sub),
                OperatorDefiner_New(TOKEN_LUALIKE_MUL,(Token(*)(void*,Token*,Vector*))Small_Small_Handler_Mul),
                OperatorDefiner_New(TOKEN_LUALIKE_DIV,(Token(*)(void*,Token*,Vector*))Small_Small_Handler_Div),
                OperatorDefiner_New(TOKEN_LUALIKE_EQU,(Token(*)(void*,Token*,Vector*))Small_Small_Handler_Equ),
                OperatorDefiner_New(TOKEN_LUALIKE_LES,(Token(*)(void*,Token*,Vector*))Small_Small_Handler_Les),
                OperatorDefiner_New(TOKEN_LUALIKE_GRT,(Token(*)(void*,Token*,Vector*))Small_Small_Handler_Grt),
                OperatorDefiner_New(TOKEN_LUALIKE_LEQ,(Token(*)(void*,Token*,Vector*))Small_Small_Handler_Leq),
                OperatorDefiner_New(TOKEN_LUALIKE_GRQ,(Token(*)(void*,Token*,Vector*))Small_Small_Handler_Grq),
                OPERATORDEFINER_END
            })),
            OPERATORINTERATER_END
        }),NULL,NULL)
    );
}