#include "/home/codeleaded/System/Static/Library/AlxCallStack.h"
#include "/home/codeleaded/System/Static/Library/AlxExternFunctions.h"
#include "/home/codeleaded/System/Static/Library/AlxInterpreter.h"
#include "/home/codeleaded/System/Static/Library/LuaLikeDefines.h"

typedef VariableMap Object;// Vector<Variable>

void Obj_Destroyer(Variable* v){
    //printf("Obj: Destroyer!\n");
    VariableMap_Free(Variable_Data(v));
}
void Obj_Cpyer(Variable* src,Variable* dst){
    //printf("Obj: Cpyer!\n");
    Object* src_str = (Object*)Variable_Data(src);
    Object* dst_str = (Object*)Variable_Data(dst);
    *dst_str = VariableMap_Cpy(src_str);
}

Token Obj_Obj_Handler_Ass(Scope* s,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    //printf("ASS: %s = %s\n",a->str,b->str);

    Variable* b_var;
    if(b->tt==TOKEN_STRING){
        b_var = Scope_FindVariable(s,b->str);
        if(!b_var){
            printf("[Obj_Ass]: 1. Arg: Variable %s doesn't exist!\n",a->str);
            return Token_Null();
        }
    }else{
        printf("[Obj_Ass]: 1. Arg: %s is not a Obj type!\n",a->str);
        return Token_Null();
    }
    
    if(a->tt==TOKEN_STRING){
        Variable* a_var = Scope_FindVariable(s,a->str);
        if(!a_var){
            Scope_BuildVariableRange(s,a->str,"obj",s->range-1);
            a_var = Scope_FindVariable(s,a->str);
            if(a_var->data) free(a_var->data);
            a_var->data = NULL;
        }

        Variable_Set(a_var,b_var);
    }else{
        printf("[Obj_Ass]: 1. Arg: %s is not a variable type!\n",a->str);
        return Token_Null();
    }

    return Token_Null();
}
Token Obj_Any_Handler_Acs(Scope* s,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    //printf("ACS: %s.%s\n",a->str,b->str);
    
    CStr name = NULL;
    if(a->tt==TOKEN_STRING){
        Variable* a_var = Scope_FindVariable(s,a->str);
        if(a_var){
            if(!Variable_Data(a_var)){
                printf("[Obj_Acs]: 1. Arg: %s is not a init obj type!\n",a->str);
                return Token_Null();
            }else{
                VariableMap* members = Variable_Data(a_var);
                Variable* member = VariableMap_Find(members,b->str);

                if(!member){
                    VariableMap_Push(members,(Variable[]){ Variable_New(b->str,NULL,-1,-1,NULL,NULL) });
                    member = VariableMap_Find(members,b->str);
                }

                String strbuilder = String_Make(".OBJACS");
                Variable* temp = Scope_FindVariableLike(s,".OBJACS*",'*');
                
                if(temp){
                    CStr retnumberstr = temp->name + 7;// CStr_Size(".OBJACS") -> 7
                    CStr strdepth = Number_Get(Number_Parse(retnumberstr) + 1);
                    String_Append(&strbuilder,strdepth); 
                    name = String_CStr(&strbuilder);
                    String_Free(&strbuilder);
                    CStr_Free(&strdepth);
                }else{
                    name = CStr_Cpy(".OBJACS0");
                }

                Scope_BuildRefVariableRange(s,name,member->typename,s->range,member);
            }
        }else{
            printf("[Obj_Ass]: 1. Arg: %s is not a variable!\n",a->str);
            return Token_Null();
        }
    }else{
        printf("[Obj_Ass]: 1. Arg: %s is not a variable type!\n",a->str);
        return Token_Null();
    }

    return Token_Move(TOKEN_STRING,name);
}
Token Obj_Handler_Cast(Scope* s,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);

    //printf("CAST: %s\n",a->str);

    Variable* a_var;
    if(a->tt==TOKEN_STRING){
        a_var = Scope_FindVariable(s,a->str);
        if(!a_var){
            printf("[Obj_Ass]: 1. Arg: Variable %s doesn't exist!\n",a->str);
            return Token_Null();
        }
    }else{
        printf("[Obj_Ass]: 1. Arg: %s is not a Obj type!\n",a->str);
        return Token_Null();
    }

    Object* obj = (Object*)Variable_Data(a_var);

    String builder = String_Make("{ ");
    for(int i = 0;i<obj->size;i++){
        Variable* v = (Variable*)PVector_Get(obj,i);
        String_Append(&builder,v->name);
        String_Append(&builder,":");
        String_Append(&builder,v->typename);
        
        String_Append(&builder," = ");

        CStr content = Scope_VariableContentStr(s,v);
        if(content){
            String_Append(&builder,content);
            CStr_Free(&content);
        }else{
            String_Append(&builder,"None");
        }
        
        if(i<obj->size-1) String_Append(&builder,", ");
    }
    String_Append(&builder," }");
    char* resstr = String_CStr(&builder);
    String_Free(&builder);
    return Token_Move(TOKEN_CONSTSTRING_DOUBLE,resstr);
}
Token Obj_Handler_Destroy(Scope* s,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);

    //printf("DESTROY: %s\n",a->str);

    Variable* a_var = Scope_FindVariable(s,a->str);
    if(a_var){
        a_var->destroy(a_var);
    }
    
    return Token_Null();
}

Variable Obj_new(Scope* sc,CStr name,Variable* args){
    Variable* a = &args[0];
    CStr objtype = *(CStr*)a->data;
    
    Type* t = TypeMap_Find(&sc->types,objtype);
    if(t){
        Variable ret = Variable_Make(
            ".OBJECT",objtype,
            (VariableMap[]){ VariableMap_New() },
            sizeof(VariableMap),
            sc->range-1,
            Scope_DestroyerOfType(sc,objtype),
            Scope_CpyerOfType(sc,objtype)
        );
        return ret;
    }else{
        Interpreter_ErrorHandler((Enviroment*)sc,"obj::new expected a valid type name!");
        return Variable_Null();
    }
}

void Ex_Packer(ExternFunctionMap* Extern_Functions,Vector* funcs,Scope* s){//Vector<CStr>
    TypeMap_PushContained(&s->types,funcs,
        Type_New("obj",16,OperatorInterationMap_Make((OperatorInterater[]){
            OperatorInterater_Make((CStr[]){ NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_CAST,(Token(*)(void*,Token*,Vector*))Obj_Handler_Cast),
                OperatorDefiner_New(TOKEN_INIT,NULL),
                OperatorDefiner_New(TOKEN_DESTROY,NULL),
                OPERATORDEFINER_END
            })),
            OperatorInterater_Make((CStr[]){ "obj",NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_LUALIKE_ASS,(Token(*)(void*,Token*,Vector*))Obj_Obj_Handler_Ass),
                OPERATORDEFINER_END
            })),
            OperatorInterater_Make((CStr[]){ OPERATORINTERATER_DONTCARE,NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_LUALIKE_ACS,(Token(*)(void*,Token*,Vector*))Obj_Any_Handler_Acs),
                OPERATORDEFINER_END
            })),
            OPERATORINTERATER_END
        }),Obj_Destroyer,Obj_Cpyer)
    );

    ExternFunctionMap_PushContained_C(Extern_Functions,funcs,(ExternFunction[]){
        ExternFunction_New("new",NULL,(Member[]){
            Member_New("str","type"),
            MEMBER_END
        },(void*)Obj_new),
        ExternFunction_Null()
    });
}
