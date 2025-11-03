#include "/home/codeleaded/System/Static/Library/AlxCallStack.h"
#include "/home/codeleaded/System/Static/Library/AlxExternFunctions.h"
#include "/home/codeleaded/System/Static/Library/AlxEnviroment.h"
#include "/home/codeleaded/System/Static/Library/LuaLikeDefines.h"
//#include "/home/codeleaded/Hecke/C/Cmd_Scripter/src/LuaLike.h"

#include "/home/codeleaded/System/Static/Library/Vector2.h"


void Vec2_Destroyer(Variable* v){
    //printf("Vec2: Destroyer -> %s!\n",v->name);
}
void Vec2_Cpyer(Variable* src,Variable* dst){
    //printf("Vec2: Cpyer -> %s!\n",v->name);
    Vec2* ptr1 = (Vec2*)src->data;
    Vec2* ptr2 = (Vec2*)dst->data;
    *ptr2 = *ptr1;
}

CStr Vec2_BuildAcs(Scope* s,Vec2* v,CStr type,void* data,CStr name,CStr field){
    if(CStr_Cmp(name,field)){
        CStr name = Enviroment_Variablename_Next((Enviroment*)s,LUALIKE_STACK,sizeof(LUALIKE_STACK) - 1);
        Scope_BuildRefDataVariableRange(s,name,type,s->range,data);
        return name;
    }
    return NULL;
}

#define VEC2_RETURNIF(s,v,ret,type,data,name,field)\
ret = Vec2_BuildAcs(s,v,type,data,name,field);\
if(ret) return Token_Move(TOKEN_STRING,ret);

Vec2 Implementation_Vec2Of(Scope* s,Token* a){
    Vec2 n = {0.0f,0.0f};
    
    if(a->tt==TOKEN_STRING){
        Variable* a_var = Scope_FindVariable(s,a->str);
        if(a_var){
            n = *(Vec2*)Variable_Data(a_var);
        }else{
            printf("[Vec2]: Impl -> 1. Arg: Variable %s doesn't exist!\n",a->str);
        }
    }else{
        printf("[Vec2]: Impl -> 1. Arg: %s is not a vec2 type!\n",a->str);
    }
    return n;
}
Double Implementation_FloatOf(Scope* s,Token* a){
    Double n = DOUBLE_PARSE_ERROR;
    
    if(a->tt==TOKEN_STRING){
        Variable* a_var = Scope_FindVariable(s,a->str);
        if(a_var){
            n = *(Double*)Variable_Data(a_var);
        }else{
            printf("[Float]: Impl -> 1. Arg: Variable %s doesn't exist!\n",a->str);
        }
    }else if(a->tt==TOKEN_FLOAT){
        n = Double_Parse(a->str,1);
    }else if(a->tt==TOKEN_NUMBER){
        n = (Double)Number_Parse(a->str);
    }else{
        printf("[Float]: Impl -> 1. Arg: %s is not a float type!\n",a->str);
    }
    return n;
}

Token Vec2_Vec2_Handler_Ass(Scope* s,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    //printf("ASS: %s = %s\n",a->str,b->str);

    Variable* b_var;
    if(b->tt==TOKEN_STRING){
        b_var = Scope_FindVariable(s,b->str);
        if(!b_var){
            printf("[Vec2_Ass]: 1. Arg: Variable %s doesn't exist!\n",a->str);
            return Token_Null();
        }
    }else{
        printf("[Vec2_Ass]: 1. Arg: %s is not a Vec2 type!\n",a->str);
        return Token_Null();
    }
    
    if(a->tt==TOKEN_STRING){
        Variable* a_var = Scope_FindVariable(s,a->str);
        if(!a_var){
            Scope_BuildVariableRange(s,a->str,"vec2",s->range-1);
            a_var = Scope_FindVariable(s,a->str);
            if(a_var->data) free(a_var->data);
            a_var->data = NULL;
        }

        Variable_Set(a_var,b_var);
    }else{
        printf("[Vec2_Ass]: 1. Arg: %s is not a variable type!\n",a->str);
        return Token_Null();
    }

    return Token_Null();
}
Token Vec2_Any_Handler_Acs(Scope* s,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    //printf("[Vec2]: Acs -> %s.%s\n",a->str,b->str);
    
    CStr name = NULL;
    if(a->tt==TOKEN_STRING){
        Variable* a_var = Scope_FindVariable(s,a->str);
        if(a_var){
            if(!Variable_Data(a_var)){
                printf("[Vec2_Acs]: 1. Arg: %s is not a init vec2 type!\n",a->str);
                return Token_Null();
            }else{
                Vec2* v = (Vec2*)Variable_Data(a_var);

                CStr name = NULL;
                VEC2_RETURNIF(s,v,name,"float",&v->x,b->str,"x")
                VEC2_RETURNIF(s,v,name,"float",&v->y,b->str,"y")
            }
        }else{
            printf("[Vec2_Ass]: 1. Arg: %s is not a variable!\n",a->str);
            return Token_Null();
        }
    }else{
        printf("[Vec2_Ass]: 1. Arg: %s is not a variable type!\n",a->str);
        return Token_Null();
    }

    return Token_Null();
}
Token Vec2_Vec2_Handler_Add(Scope* s,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    //printf("ADD: %s + %s\n",a->str,b->str);

    Vec2 n1 = Implementation_Vec2Of(s,a);
    Vec2 n2 = Implementation_Vec2Of(s,b);
    Vec2 res = Vec2_Add(n1,n2);

    CStr name = Enviroment_Variablename_Next((Enviroment*)s,LUALIKE_STACK,sizeof(LUALIKE_STACK) - 1);
    Scope_BuildInitVariableRange(s,name,"vec2",s->range,&res);
    return Token_Move(TOKEN_STRING,name);
}
Token Vec2_Vec2_Handler_Sub(Scope* s,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    //printf("SUB: %s - %s\n",a->str,b->str);

    Vec2 n1 = Implementation_Vec2Of(s,a);
    Vec2 n2 = Implementation_Vec2Of(s,b);
    Vec2 res = Vec2_Sub(n1,n2);

    CStr name = Enviroment_Variablename_Next((Enviroment*)s,LUALIKE_STACK,sizeof(LUALIKE_STACK) - 1);
    Scope_BuildInitVariableRange(s,name,"vec2",s->range,&res);
    return Token_Move(TOKEN_STRING,name);
}
Token Vec2_Vec2_Handler_Mul(Scope* s,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    //printf("MUL: %s * %s\n",a->str,b->str);

    Vec2 n1 = Implementation_Vec2Of(s,a);
    Vec2 n2 = Implementation_Vec2Of(s,b);
    Vec2 res = Vec2_Mul(n1,n2);

    CStr name = Enviroment_Variablename_Next((Enviroment*)s,LUALIKE_STACK,sizeof(LUALIKE_STACK) - 1);
    Scope_BuildInitVariableRange(s,name,"vec2",s->range,&res);
    return Token_Move(TOKEN_STRING,name);
}
Token Vec2_Vec2_Handler_Div(Scope* s,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    //printf("DIV: %s / %s\n",a->str,b->str);

    Vec2 n1 = Implementation_Vec2Of(s,a);
    Vec2 n2 = Implementation_Vec2Of(s,b);
    Vec2 res = Vec2_Div(n1,n2);

    CStr name = Enviroment_Variablename_Next((Enviroment*)s,LUALIKE_STACK,sizeof(LUALIKE_STACK) - 1);
    Scope_BuildInitVariableRange(s,name,"vec2",s->range,&res);
    return Token_Move(TOKEN_STRING,name);
}

Token Vec2_Float_Handler_Mul(Scope* s,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    //printf("MUL: %s * %s\n",a->str,b->str);

    Vec2 n1 = Implementation_Vec2Of(s,a);
    Double n2 = Implementation_FloatOf(s,b);
    Vec2 res = Vec2_Mulf(n1,n2);

    CStr name = Enviroment_Variablename_Next((Enviroment*)s,LUALIKE_STACK,sizeof(LUALIKE_STACK) - 1);
    Scope_BuildInitVariableRange(s,name,"vec2",s->range,&res);
    return Token_Move(TOKEN_STRING,name);
}
Token Vec2_Float_Handler_Div(Scope* s,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    //printf("DIV: %s / %s\n",a->str,b->str);

    Vec2 n1 = Implementation_Vec2Of(s,a);
    Double n2 = Implementation_FloatOf(s,b);
    Vec2 res = Vec2_Divf(n1,n2);

    CStr name = Enviroment_Variablename_Next((Enviroment*)s,LUALIKE_STACK,sizeof(LUALIKE_STACK) - 1);
    Scope_BuildInitVariableRange(s,name,"vec2",s->range,&res);
    return Token_Move(TOKEN_STRING,name);
}

Token Vec2_Vec2_Handler_Neg(Scope* s,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);

    //printf("NEG: -%s\n",a->str);

    Vec2 n1 = Implementation_Vec2Of(s,a);
    Vec2 res = Vec2_Neg(n1);

    CStr name = Enviroment_Variablename_Next((Enviroment*)s,LUALIKE_STACK,sizeof(LUALIKE_STACK) - 1);
    Scope_BuildInitVariableRange(s,name,"vec2",s->range,&res);
    return Token_Move(TOKEN_STRING,name);
}
Token Vec2_Vec2_Handler_Equ(Scope* s,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    //printf("EQU: %s == %s\n",a->str,b->str);

    Vec2 n1 = Implementation_Vec2Of(s,a);
    Vec2 n2 = Implementation_Vec2Of(s,b);
    Boolean res = n1.x == n2.x && n1.y == n2.y;

    char* resstr = Boolean_Get(res);
    return Token_Move(TOKEN_BOOL,resstr);
}
Token Vec2_Handler_Cast(Scope* s,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);

    //printf("CAST: %s\n",a->str);

    Variable* a_var;
    if(a->tt==TOKEN_STRING){
        a_var = Scope_FindVariable(s,a->str);
        if(!a_var){
            printf("[Vec2_Ass]: 1. Arg: Variable %s doesn't exist!\n",a->str);
            return Token_Null();
        }
    }else{
        printf("[Vec2_Ass]: 1. Arg: %s is not a Vec2 type!\n",a->str);
        return Token_Null();
    }

    Vec2* v = (Vec2*)Variable_Data(a_var);

    String builder = String_Make("{ ");
    
    String_AppendDouble(&builder,v->x);
    String_Append(&builder,",");
    String_AppendDouble(&builder,v->y);

    String_Append(&builder," }");
    char* resstr = String_CStr(&builder);
    String_Free(&builder);
    return Token_Move(TOKEN_CONSTSTRING_DOUBLE,resstr);
}
Token Vec2_Handler_Destroy(Scope* s,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);

    //printf("DESTROY: %s\n",a->str);

    Variable* a_var = Scope_FindVariable(s,a->str);
    if(a_var){
        a_var->destroy(a_var);
    }
    
    return Token_Null();
}

Variable Vec2_Function_Make(Scope* sc,CStr name,Variable* args){
    Variable* v_x = &args[0];
    Variable* v_y = &args[1];
    
    Double x = *(Double*)v_x->data;
    Double y = *(Double*)v_y->data;
    
    Variable ret = Variable_Make(
        "Vec2type","vec2",
        (Vec2[]){ {.x = x,.y = y} },
        sizeof(Vec2),
        sc->range-1,
        Scope_DestroyerOfType(sc,"vec2"),
        Scope_CpyerOfType(sc,"vec2")
    );
    return ret;
}
Variable Vec2_Function_Dot(Scope* sc,CStr name,Variable* args){
    Variable* v1_var = &args[0];
    Variable* v2_var = &args[1];
    
    Double b = DOUBLE_PARSE_ERROR;
    if(!Variable_Data(v1_var)){
        printf("[Vec2]: Dot -> %s is not init!\n",v1_var->name);
    }else{
        Vec2* v1 = (Vec2*)Variable_Data(v1_var);
        Vec2* v2 = (Vec2*)Variable_Data(v2_var);
        b = Vec2_Dot(*v1,*v2);
    }

    return Variable_Make(
        ".DOT",
        "float",
        (Double[]){ b },
        sizeof(Double),
        sc->range,
        Scope_DestroyerOfType(sc,"float"),
        Scope_CpyerOfType(sc,"float")
    );
}
Variable Vec2_Function_Mag(Scope* sc,CStr name,Variable* args){
    Variable* v1_var = &args[0];
    Variable* v2_var = &args[1];
    
    Double b = DOUBLE_PARSE_ERROR;
    if(!Variable_Data(v1_var)){
        printf("[Vec2]: Mag -> %s is not init!\n",v1_var->name);
    }else{
        Vec2* v1 = (Vec2*)Variable_Data(v1_var);
        Vec2* v2 = (Vec2*)Variable_Data(v2_var);
        b = Vec2_Dot(*v1,*v2);
    }

    return Variable_Make(
        ".MAG",
        "float",
        (Double[]){ b },
        sizeof(Double),
        sc->range,
        Scope_DestroyerOfType(sc,"float"),
        Scope_CpyerOfType(sc,"float")
    );
}
Variable Vec2_Function_Perp(Scope* sc,CStr name,Variable* args){
    Variable* v1_var = &args[0];
    
    Vec2 v = {0.0f,0.0f};
    if(!Variable_Data(v1_var)){
        printf("[Vec2]: Perp -> %s is not init!\n",v1_var->name);
    }else{
        Vec2* v1 = (Vec2*)Variable_Data(v1_var);
        v = Vec2_Perp(*v1);
    }

    return Variable_Make(
        ".PERP",
        "vec2",
        (Vec2[]){ v },
        sizeof(Vec2),
        sc->range,
        Scope_DestroyerOfType(sc,"vec2"),
        Scope_CpyerOfType(sc,"vec2")
    );
}
Variable Vec2_Function_Norm(Scope* sc,CStr name,Variable* args){
    Variable* v1_var = &args[0];
    
    Vec2 v = {0.0f,0.0f};
    if(!Variable_Data(v1_var)){
        printf("[Vec2]: Norm -> %s is not init!\n",v1_var->name);
    }else{
        Vec2* v1 = (Vec2*)Variable_Data(v1_var);
        v = Vec2_Norm(*v1);
    }

    return Variable_Make(
        ".NORM",
        "vec2",
        (Vec2[]){ v },
        sizeof(Vec2),
        sc->range,
        Scope_DestroyerOfType(sc,"vec2"),
        Scope_CpyerOfType(sc,"vec2")
    );
}

void Ex_Packer(ExternFunctionMap* Extern_Functions,Vector* funcs,Scope* s){//Vector<CStr>
    TypeMap_PushContained(&s->types,funcs,
        Type_New("vec2",sizeof(Vec2),OperatorInterationMap_Make((OperatorInterater[]){
            OperatorInterater_Make((CStr[]){ NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_CAST,(Token(*)(void*,Token*,Vector*))Vec2_Handler_Cast),
                OperatorDefiner_New(TOKEN_INIT,NULL),
                OperatorDefiner_New(TOKEN_DESTROY,NULL),
                OPERATORDEFINER_END
            })),
            OperatorInterater_Make((CStr[]){ "vec2",NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_LUALIKE_ASS,(Token(*)(void*,Token*,Vector*))Vec2_Vec2_Handler_Ass),
                OperatorDefiner_New(TOKEN_LUALIKE_ADD,(Token(*)(void*,Token*,Vector*))Vec2_Vec2_Handler_Add),
                OperatorDefiner_New(TOKEN_LUALIKE_SUB,(Token(*)(void*,Token*,Vector*))Vec2_Vec2_Handler_Sub),
                OperatorDefiner_New(TOKEN_LUALIKE_MUL,(Token(*)(void*,Token*,Vector*))Vec2_Vec2_Handler_Mul),
                OperatorDefiner_New(TOKEN_LUALIKE_DIV,(Token(*)(void*,Token*,Vector*))Vec2_Vec2_Handler_Div),
                OperatorDefiner_New(TOKEN_LUALIKE_EQU,(Token(*)(void*,Token*,Vector*))Vec2_Vec2_Handler_Equ),
                OPERATORDEFINER_END
            })),
            OperatorInterater_Make((CStr[]){ "float",NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_LUALIKE_MUL,(Token(*)(void*,Token*,Vector*))Vec2_Float_Handler_Mul),
                OperatorDefiner_New(TOKEN_LUALIKE_DIV,(Token(*)(void*,Token*,Vector*))Vec2_Float_Handler_Div),
                OPERATORDEFINER_END
            })),
            OperatorInterater_Make((CStr[]){ OPERATORINTERATER_DONTCARE,NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_LUALIKE_ACS,(Token(*)(void*,Token*,Vector*))Vec2_Any_Handler_Acs),
                OPERATORDEFINER_END
            })),
            OPERATORINTERATER_END
        }),Vec2_Destroyer,Vec2_Cpyer)
    );

    ExternFunctionMap_PushContained(Extern_Functions,funcs,ExternFunction_New("new",NULL,(Member[]){ 
        Member_New("str","name"),
        Member_New("int","width"),
        Member_New("int","height"),
        MEMBER_END
    },(void*)Vec2_Function_Make));
    ExternFunctionMap_PushContained(Extern_Functions,funcs,ExternFunction_New("dot","float",(Member[]){ 
        Member_New("vec2","v1"),
        Member_New("vec2","v2"),
        MEMBER_END
    },(void*)Vec2_Function_Dot));
    ExternFunctionMap_PushContained(Extern_Functions,funcs,ExternFunction_New("mag","float",(Member[]){ 
        Member_New("vec2","v"),
        MEMBER_END
    },(void*)Vec2_Function_Mag));
    ExternFunctionMap_PushContained(Extern_Functions,funcs,ExternFunction_New("perp","vec2",(Member[]){ 
        Member_New("vec2","v"),
        MEMBER_END
    },(void*)Vec2_Function_Perp));
    ExternFunctionMap_PushContained(Extern_Functions,funcs,ExternFunction_New("norm","vec2",(Member[]){ 
        Member_New("vec2","v"),
        MEMBER_END
    },(void*)Vec2_Function_Norm));
}
