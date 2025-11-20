#include "/home/codeleaded/System/Static/Library/AlxCallStack.h"
#include "/home/codeleaded/System/Static/Library/AlxExternFunctions.h"
#include "/home/codeleaded/System/Static/Library/AlxEnviroment.h"
#include "/home/codeleaded/System/Static/Library/LuaLikeDefines.h"
//#include "/home/codeleaded/Hecke/C/Cmd_Scripter/src/LuaLike.h"
#include "/home/codeleaded/System/Static/Library/Pointer.h"

#include "../src/Karol.h"



void Karol_Destroyer(Variable* v){
    //printf("Karol: Destroyer -> %s!\n",v->name);
    
    SharedPointer* ptr = (SharedPointer*)v->data;
    char ret = SharedPointer_Free(ptr);
    //if(ret) printf("FULL KILL\n");
    //else    printf("SMALL KILL\n");
}
void Karol_Cpyer(Variable* src,Variable* dst){
    //printf("Karol: Cpyer!\n");
    
    SharedPointer* ptr1 = (SharedPointer*)src->data;
    SharedPointer* ptr2 = (SharedPointer*)dst->data;
    *ptr2 = SharedPointer_Share(ptr1);
}

Token Karol_Karol_Handler_Ass(Scope* s,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    //printf("ASS: %s = %s\n",a->str,b->str);

    Variable* b_var;
    if(b->tt==TOKEN_STRING){
        b_var = Scope_FindVariable(s,b->str);
        if(!b_var){
            printf("[Karol_Ass]: 1. Arg: Variable %s doesn't exist!\n",a->str);
            return Token_Null();
        }
    }else{
        printf("[Karol_Ass]: 1. Arg: %s is not a Karol type!\n",a->str);
        return Token_Null();
    }
    
    if(a->tt==TOKEN_STRING){
        Variable* a_var = Scope_FindVariable(s,a->str);
        if(!a_var){
            Scope_BuildVariableRange(s,a->str,"karol",s->range-1);
            a_var = Scope_FindVariable(s,a->str);
            if(a_var->data) free(a_var->data);
            a_var->data = NULL;
        }

        Variable_Set(a_var,b_var);
    }else{
        printf("[Karol_Ass]: 1. Arg: %s is not a variable type!\n",a->str);
        return Token_Null();
    }

    return Token_Null();
}
Token Karol_Handler_Cast(Scope* s,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);

    //printf("CAST: %s\n",a->str);

    Variable* a_var;
    if(a->tt==TOKEN_STRING){
        a_var = Scope_FindVariable(s,a->str);
        if(!a_var){
            printf("[Karol_Ass]: 1. Arg: Variable %s doesn't exist!\n",a->str);
            return Token_Null();
        }
    }else{
        printf("[Karol_Ass]: 1. Arg: %s is not a Karol type!\n",a->str);
        return Token_Null();
    }

    KarolPtr* wptr = (KarolPtr*)Variable_Data(a_var);
    Karol* k = (Karol*)wptr->Memory;

    String builder = String_Make("{ ");
    
    String_AppendDouble(&builder,k->p.x);
    String_Append(&builder,",");
    String_AppendDouble(&builder,k->p.x);
    String_Append(&builder,",");
    String_AppendDouble(&builder,k->p.x);

    String_Append(&builder," }");
    char* resstr = String_CStr(&builder);
    String_Free(&builder);
    return Token_Move(TOKEN_CONSTSTRING_DOUBLE,resstr);
}
Token Karol_Handler_Destroy(Scope* s,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);

    //printf("DESTROY: %s\n",a->str);

    Variable* a_var = Scope_FindVariable(s,a->str);
    if(a_var){
        a_var->destroy(a_var);
    }
    
    return Token_Null();
}

Variable Karol_Make(Scope* sc,CStr name,Variable* args){
    Variable* v_x = &args[0];
    Variable* v_y = &args[1];
    Variable* v_z = &args[2];
    
    Double x = *(Double*)Variable_Data(v_x);
    Double y = *(Double*)Variable_Data(v_y);
    Double z = *(Double*)Variable_Data(v_z);
    
    Karol* pw = (Karol*)malloc(sizeof(Karol));
    *pw = Karol_New((Vec3D){ x - 0.75f * 0.75f,y + 1.75f,z - 0.75f * 0.75f },(Vec3D){ 0.75f,2.0f,0.75f });
    KarolPtr ptr = SharedPointer_Make(pw,(void*)Karol_Free);

    Variable ret = Variable_Make(
        "Karoltype","karol",
        &ptr,
        sizeof(KarolPtr),
        sc->range-1,
        Scope_DestroyerOfType(sc,"karol"),
        Scope_CpyerOfType(sc,"karol")
    );
    return ret;
}
Variable Karol_Get(Scope* sc,CStr name,Variable* args){
    Variable* v_id = &args[0];
    Number id = *(Number*)Variable_Data(v_id);
    
    CStr kname = CStr_Format(".KAROL%d",id);
    Variable* karol = Scope_FindVariable(sc,kname);
    CStr_Free(&kname);

    if(karol){
        Variable ret = Variable_Make(
            "Karoltype","karol",
            (KarolPtr[]){ SharedPointer_Share(Variable_Data(karol)) },
            sizeof(KarolPtr),
            sc->range-1,
            Scope_DestroyerOfType(sc,"karol"),
            Scope_CpyerOfType(sc,"karol")
        );
        return ret;
    }
    return Variable_Null();
}

Variable Karol_SetP(Scope* sc,CStr name,Variable* args){
    Variable* a_var = &args[0];
    Variable* x_var = &args[1];
    Variable* y_var = &args[2];
    Variable* z_var = &args[3];
    
    if(!Variable_Data(a_var)){
        printf("[Karol]: Update -> %s is not init!\n",a_var->name);
    }else{
        KarolPtr* wptr = (KarolPtr*)Variable_Data(a_var);
        Karol* k = (Karol*)wptr->Memory;

        Number x = *(Number*)Variable_Data(x_var);
        Number y = *(Number*)Variable_Data(y_var);
        Number z = *(Number*)Variable_Data(z_var);

        k->p = (Vec3D){ (float)x + 0.75f * 0.75f,(float)y + 1.75f,(float)z + 0.75f * 0.75f };
    }

    return Variable_Null();
}
Variable Karol_SetA(Scope* sc,CStr name,Variable* args){
    Variable* a_var = &args[0];
    Variable* x_var = &args[1];
    Variable* y_var = &args[2];
    Variable* z_var = &args[3];
    
    if(!Variable_Data(a_var)){
        printf("[Karol]: Update -> %s is not init!\n",a_var->name);
    }else{
        KarolPtr* wptr = (KarolPtr*)Variable_Data(a_var);
        Karol* k = (Karol*)wptr->Memory;

        Number x = *(Number*)Variable_Data(x_var);
        Number y = *(Number*)Variable_Data(y_var);
        Number z = *(Number*)Variable_Data(z_var);

        k->a = (Vec3D){ (float)x * F32_PI / 180.0f,(float)y * F32_PI / 180.0f,(float)z * F32_PI / 180.0f };
    }

    return Variable_Null();
}
Variable Karol_Step(Scope* sc,CStr name,Variable* args){
    Variable* a_var = &args[0];
    Variable* b_var = &args[1];
    
    if(!Variable_Data(a_var)){
        printf("[Karol]: Update -> %s is not init!\n",a_var->name);
    }else{
        KarolPtr* wptr = (KarolPtr*)Variable_Data(a_var);
        Karol* k = (Karol*)wptr->Memory;

        Number steps = *(Number*)Variable_Data(b_var);

        M4x4D rotX = Matrix_MakeRotationX(k->a.x);
        M4x4D rotY = Matrix_MakeRotationY(k->a.y);

        k->p = Vec3D_Add(k->p,Matrix_MultiplyVector(rotX,Matrix_MultiplyVector(rotY,(Vec3D){ 0.0f,0.0f,steps,1.0f })));
    }

    return Variable_Null();
}
Variable Karol_TurnL(Scope* sc,CStr name,Variable* args){
    Variable* a_var = &args[0];
    
    if(!Variable_Data(a_var)){
        printf("[Karol]: Update -> %s is not init!\n",a_var->name);
    }else{
        KarolPtr* wptr = (KarolPtr*)Variable_Data(a_var);
        Karol* k = (Karol*)wptr->Memory;
        k->a.y -= F32_PI05;
    }

    return Variable_Null();
}
Variable Karol_TurnR(Scope* sc,CStr name,Variable* args){
    Variable* a_var = &args[0];
    
    if(!Variable_Data(a_var)){
        printf("[Karol]: Update -> %s is not init!\n",a_var->name);
    }else{
        KarolPtr* wptr = (KarolPtr*)Variable_Data(a_var);
        Karol* k = (Karol*)wptr->Memory;
        k->a.y += F32_PI05;
    }

    return Variable_Null();
}

void Ex_Packer(ExternFunctionMap* Extern_Functions,Vector* funcs,Scope* s){//Vector<CStr>
    TypeMap_PushContained(&s->types,funcs,
        Type_New("karol",sizeof(KarolPtr),OperatorInterationMap_Make((OperatorInterater[]){
            OperatorInterater_Make((CStr[]){ NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_CAST,(Token(*)(void*,Token*,Vector*))Karol_Handler_Cast),
                OperatorDefiner_New(TOKEN_INIT,NULL),
                OperatorDefiner_New(TOKEN_DESTROY,NULL),
                OPERATORDEFINER_END
            })),
            OperatorInterater_Make((CStr[]){ "karol",NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_LUALIKE_ASS,(Token(*)(void*,Token*,Vector*))Karol_Karol_Handler_Ass),
                OPERATORDEFINER_END
            })),
            OPERATORINTERATER_END
        }),Karol_Destroyer,Karol_Cpyer)
    );

    ExternFunctionMap_PushContained(Extern_Functions,funcs,ExternFunction_New("new",NULL,(Member[]){ 
        Member_New("float","x"),
        Member_New("float","y"),
        Member_New("float","z"),
        MEMBER_END
    },(void*)Karol_Make));
    ExternFunctionMap_PushContained(Extern_Functions,funcs,ExternFunction_New("get",NULL,(Member[]){ 
        Member_New("int","id"),
        MEMBER_END
    },(void*)Karol_Get));

    ExternFunctionMap_PushContained(Extern_Functions,funcs,ExternFunction_New("setp",NULL,(Member[]){ 
        Member_New("karol","w"),
        Member_New("int","px"),
        Member_New("int","py"),
        Member_New("int","pz"),
        MEMBER_END
    },(void*)Karol_SetP));
    ExternFunctionMap_PushContained(Extern_Functions,funcs,ExternFunction_New("seta",NULL,(Member[]){ 
        Member_New("karol","w"),
        Member_New("int","ax"),
        Member_New("int","ay"),
        Member_New("int","az"),
        MEMBER_END
    },(void*)Karol_SetA));
    ExternFunctionMap_PushContained(Extern_Functions,funcs,ExternFunction_New("step",NULL,(Member[]){ 
        Member_New("karol","w"),
        Member_New("int","s"),
        MEMBER_END
    },(void*)Karol_Step));
    ExternFunctionMap_PushContained(Extern_Functions,funcs,ExternFunction_New("turnl",NULL,(Member[]){ 
        Member_New("karol","w"),
        MEMBER_END
    },(void*)Karol_TurnL));
    ExternFunctionMap_PushContained(Extern_Functions,funcs,ExternFunction_New("turnr",NULL,(Member[]){ 
        Member_New("karol","w"),
        MEMBER_END
    },(void*)Karol_TurnR));
}
