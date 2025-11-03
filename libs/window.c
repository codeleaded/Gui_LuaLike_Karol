#include "/home/codeleaded/System/Static/Library/AlxCallStack.h"
#include "/home/codeleaded/System/Static/Library/AlxExternFunctions.h"
#include "/home/codeleaded/System/Static/Library/AlxEnviroment.h"
#include "/home/codeleaded/System/Static/Library/LuaLikeDefines.h"
//#include "/home/codeleaded/Hecke/C/Cmd_Scripter/src/LuaLike.h"

#include "/home/codeleaded/System/Static/Library/WindowEngine1.0.h"
#include "/home/codeleaded/System/Static/Library/Pointer.h"


typedef SharedPointer WindowPtr;

void Window_Destroyer(Variable* v){
    //printf("Window: Destroyer -> %s!\n",v->name);
    
    SharedPointer* ptr = (SharedPointer*)v->data;
    char ret = SharedPointer_Free(ptr);
    //if(ret) printf("FULL KILL\n");
    //else    printf("SMALL KILL\n");
}
void Window_Cpyer(Variable* src,Variable* dst){
    //printf("Window: Cpyer!\n");
    
    SharedPointer* ptr1 = (SharedPointer*)src->data;
    SharedPointer* ptr2 = (SharedPointer*)dst->data;
    *ptr2 = SharedPointer_Share(ptr1);
}

#define WINDOW_TESTKEY(key,test) if(CStr_Cmp(key,#test)) keystates = alxw->Strokes[ALX_KEY_##test];

Boolean Window_GetKey(AlxWindow* alxw,CStr key,CStr action){
    States keystates;
    WINDOW_TESTKEY(key,W)
    WINDOW_TESTKEY(key,S)
    WINDOW_TESTKEY(key,A)
    WINDOW_TESTKEY(key,D)
    WINDOW_TESTKEY(key,UP)
    WINDOW_TESTKEY(key,DOWN)
    WINDOW_TESTKEY(key,LEFT)
    WINDOW_TESTKEY(key,RIGHT)

    Boolean state = 0;
    if(CStr_Cmp(action,"pressed"))  state = keystates.PRESSED;
    if(CStr_Cmp(action,"released")) state = keystates.RELEASED;
    if(CStr_Cmp(action,"down"))     state = keystates.DOWN;
    if(CStr_Cmp(action,"repeat"))   state = keystates.REPEAT;

    return state;
}
CStr Window_BuildAcs(Scope* s,WindowPtr* wptr,CStr type,void* data,CStr name,CStr field){
    if(CStr_Cmp(name,field)){
        CStr name = Enviroment_Variablename_Next((Enviroment*)s,LUALIKE_STACK,sizeof(LUALIKE_STACK) - 1);
        Scope_BuildRefDataVariableRange(s,name,type,s->range,data);
        return name;
    }
    return NULL;
}

#define WINDOW_RETURNIF(s,wptr,ret,type,data,name,field)\
ret = Window_BuildAcs(s,wptr,type,data,name,field);\
if(ret) return Token_Move(TOKEN_STRING,ret);

Token Window_Window_Handler_Ass(Scope* s,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    //printf("ASS: %s = %s\n",a->str,b->str);

    Variable* b_var;
    if(b->tt==TOKEN_STRING){
        b_var = Scope_FindVariable(s,b->str);
        if(!b_var){
            printf("[Window_Ass]: 1. Arg: Variable %s doesn't exist!\n",a->str);
            return Token_Null();
        }
    }else{
        printf("[Window_Ass]: 1. Arg: %s is not a Window type!\n",a->str);
        return Token_Null();
    }
    
    if(a->tt==TOKEN_STRING){
        Variable* a_var = Scope_FindVariable(s,a->str);
        if(!a_var){
            Scope_BuildVariableRange(s,a->str,"window",s->range-1);
            a_var = Scope_FindVariable(s,a->str);
            if(a_var->data) free(a_var->data);
            a_var->data = NULL;
        }

        Variable_Set(a_var,b_var);
    }else{
        printf("[Window_Ass]: 1. Arg: %s is not a variable type!\n",a->str);
        return Token_Null();
    }

    return Token_Null();
}
Token Window_Any_Handler_Acs(Scope* s,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);
    Token* b = (Token*)Vector_Get(args,1);

    //printf("[Window]: Acs -> %s.%s\n",a->str,b->str);
    
    CStr name = NULL;
    if(a->tt==TOKEN_STRING){
        Variable* a_var = Scope_FindVariable(s,a->str);
        if(a_var){
            if(!Variable_Data(a_var)){
                printf("[Window_Acs]: 1. Arg: %s is not a init window type!\n",a->str);
                return Token_Null();
            }else{
                WindowPtr* wptr = (WindowPtr*)Variable_Data(a_var);
                AlxWindow* alxw = (AlxWindow*)wptr->Memory;

                CStr name = NULL;
                WINDOW_RETURNIF(s,wptr,name,"str",&alxw->Name,b->str,"name")
                WINDOW_RETURNIF(s,wptr,name,"small",&alxw->Width,b->str,"width")
                WINDOW_RETURNIF(s,wptr,name,"small",&alxw->Height,b->str,"height")
                WINDOW_RETURNIF(s,wptr,name,"small",&alxw->Running,b->str,"running")
            }
        }else{
            printf("[Window_Ass]: 1. Arg: %s is not a variable!\n",a->str);
            return Token_Null();
        }
    }else{
        printf("[Window_Ass]: 1. Arg: %s is not a variable type!\n",a->str);
        return Token_Null();
    }

    return Token_Null();
}
Token Window_Handler_Cast(Scope* s,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);

    //printf("CAST: %s\n",a->str);

    Variable* a_var;
    if(a->tt==TOKEN_STRING){
        a_var = Scope_FindVariable(s,a->str);
        if(!a_var){
            printf("[Window_Ass]: 1. Arg: Variable %s doesn't exist!\n",a->str);
            return Token_Null();
        }
    }else{
        printf("[Window_Ass]: 1. Arg: %s is not a Window type!\n",a->str);
        return Token_Null();
    }

    WindowPtr* wptr = (WindowPtr*)Variable_Data(a_var);
    AlxWindow* alxw = (AlxWindow*)wptr->Memory;

    String builder = String_Make("{ ");
    
    String_Append(&builder,alxw->Name);
    String_Append(&builder,",");
    String_AppendNumber(&builder,alxw->Width);
    String_Append(&builder,",");
    String_AppendNumber(&builder,alxw->Height);

    String_Append(&builder," }");
    char* resstr = String_CStr(&builder);
    String_Free(&builder);
    return Token_Move(TOKEN_CONSTSTRING_DOUBLE,resstr);
}
Token Window_Handler_Destroy(Scope* s,Token* op,Vector* args){
    Token* a = (Token*)Vector_Get(args,0);

    //printf("DESTROY: %s\n",a->str);

    Variable* a_var = Scope_FindVariable(s,a->str);
    if(a_var){
        a_var->destroy(a_var);
    }
    
    return Token_Null();
}

Variable Window_Make(Scope* sc,CStr name,Variable* args){
    Variable* v_name = &args[0];
    Variable* v_width = &args[1];
    Variable* v_height = &args[2];
    
    CStr tname = *(CStr*)v_name->data;
    Number width = *(Number*)v_width->data;
    Number height = *(Number*)v_height->data;
    
    AlxWindow* pw = (AlxWindow*)malloc(sizeof(AlxWindow));
    *pw = AlxWindow_New(tname,width,height,1,1,NULL,NULL,NULL);
    WindowPtr ptr = SharedPointer_Make(pw,(void*)AlxWindow_OnlyExit);

    Variable ret = Variable_Make(
        "Windowtype","window",
        &ptr,
        sizeof(WindowPtr),
        sc->range-1,
        Scope_DestroyerOfType(sc,"window"),
        Scope_CpyerOfType(sc,"window")
    );
    return ret;
}
Variable Window_Init(Scope* sc,CStr name,Variable* args){
    Variable* a_var = &args[0];
    
    if(!Variable_Data(a_var)){
        printf("[Window]: Update -> %s is not init!\n",a_var->name);
    }else{
        WindowPtr* wptr = (WindowPtr*)Variable_Data(a_var);
        AlxWindow* alxw = (AlxWindow*)wptr->Memory;
        AlxWindow_GameInit(alxw);
    }

    return Variable_Null();
}
Variable Window_Update(Scope* sc,CStr name,Variable* args){
    Variable* a_var = &args[0];
    
    if(!Variable_Data(a_var)){
        printf("[Window]: Update -> %s is not init!\n",a_var->name);
    }else{
        WindowPtr* wptr = (WindowPtr*)Variable_Data(a_var);
        AlxWindow* alxw = (AlxWindow*)wptr->Memory;
        AlxWindow_GameUpdate(alxw);
    }

    return Variable_Null();
}
Variable Window_Render(Scope* sc,CStr name,Variable* args){
    Variable* a_var = &args[0];
    
    if(!Variable_Data(a_var)){
        printf("[Window]: Render -> %s is not init!\n",a_var->name);
    }else{
        WindowPtr* wptr = (WindowPtr*)Variable_Data(a_var);
        AlxWindow* alxw = (AlxWindow*)wptr->Memory;
        AlxWindow_Render(alxw);
    }

    return Variable_Null();
}
Variable Window_Running(Scope* sc,CStr name,Variable* args){
    Variable* a_var = &args[0];
    
    Boolean b = 0;
    if(!Variable_Data(a_var)){
        printf("[Window]: Running -> %s is not init!\n",a_var->name);
    }else{
        WindowPtr* wptr = (WindowPtr*)Variable_Data(a_var);
        AlxWindow* alxw = (AlxWindow*)wptr->Memory;
        b = (Boolean)alxw->Running;
    }

    return Variable_Make(
        ".RUNNING",
        "bool",
        (Boolean[]){ b },
        sizeof(Boolean),
        sc->range,
        Scope_DestroyerOfType(sc,"bool"),
        Scope_CpyerOfType(sc,"bool")
    );
}
Variable Window_Width(Scope* sc,CStr name,Variable* args){
    Variable* a_var = &args[0];
    
    Double b = 0;
    if(!Variable_Data(a_var)){
        printf("[Window]: Width -> %s is not init!\n",a_var->name);
    }else{
        WindowPtr* wptr = (WindowPtr*)Variable_Data(a_var);
        AlxWindow* alxw = (AlxWindow*)wptr->Memory;
        b = (Double)alxw->Width;
    }

    return Variable_Make(
        ".WIDTH",
        "float",
        (Double[]){ b },
        sizeof(Double),
        sc->range,
        Scope_DestroyerOfType(sc,"float"),
        Scope_CpyerOfType(sc,"float")
    );
}
Variable Window_Height(Scope* sc,CStr name,Variable* args){
    Variable* a_var = &args[0];
    
    Double b = 0;
    if(!Variable_Data(a_var)){
        printf("[Window]: Height -> %s is not init!\n",a_var->name);
    }else{
        WindowPtr* wptr = (WindowPtr*)Variable_Data(a_var);
        AlxWindow* alxw = (AlxWindow*)wptr->Memory;
        b = (Double)alxw->Height;
    }

    return Variable_Make(
        ".HEIGHT",
        "float",
        (Double[]){ b },
        sizeof(Double),
        sc->range,
        Scope_DestroyerOfType(sc,"float"),
        Scope_CpyerOfType(sc,"float")
    );
}

Variable Window_Elapsed(Scope* sc,CStr name,Variable* args){
    Variable* a_var = &args[0];
    Variable* key_var = &args[1];
    Variable* action_var = &args[2];
    
    Double b = 0.0;
    if(!Variable_Data(a_var)){
        printf("[Window]: Elapsed -> %s is not init!\n",a_var->name);
    }else{
        WindowPtr* wptr = (WindowPtr*)Variable_Data(a_var);
        AlxWindow* alxw = (AlxWindow*)wptr->Memory;
        b = (Double)alxw->ElapsedTime;
    }

    return Variable_Make(
        ".KEYSTATE",
        "float",
        (Double[]){ b },
        sizeof(Double),
        sc->range,
        Scope_DestroyerOfType(sc,"float"),
        Scope_CpyerOfType(sc,"float")
    );
}
Variable Window_Key(Scope* sc,CStr name,Variable* args){
    Variable* a_var = &args[0];
    Variable* key_var = &args[1];
    Variable* action_var = &args[2];
    
    Boolean b = 0;
    if(!Variable_Data(a_var)){
        printf("[Window]: Render -> %s is not init!\n",a_var->name);
    }else{
        WindowPtr* wptr = (WindowPtr*)Variable_Data(a_var);
        AlxWindow* alxw = (AlxWindow*)wptr->Memory;

        CStr key = *(CStr*)Variable_Data(key_var);
        CStr action = *(CStr*)Variable_Data(action_var);

        b = Window_GetKey(alxw,key,action);
    }

    return Variable_Make(
        ".KEYSTATE",
        "bool",
        (Boolean[]){ b },
        sizeof(Boolean),
        sc->range,
        Scope_DestroyerOfType(sc,"bool"),
        Scope_CpyerOfType(sc,"bool")
    );
}

Variable Window_Clear(Scope* sc,CStr name,Variable* args){
    Variable* a_var = &args[0];
    Variable* p_var = &args[1];
    
    if(!Variable_Data(a_var)){
        printf("[Window]: Clear -> %s is not init!\n",a_var->name);
    }else{
        WindowPtr* wptr = (WindowPtr*)Variable_Data(a_var);
        AlxWindow* alxw = (AlxWindow*)wptr->Memory;

        Pixel p = (Pixel)(*(Number*)Variable_Data(p_var));
        Graphics_Clear(alxw->Buffer,alxw->Width,alxw->Height,p);
    }

    return Variable_Null();
}
Variable Window_Rect(Scope* sc,CStr name,Variable* args){
    Variable* a_var = &args[0];
    Variable* x_var = &args[1];
    Variable* y_var = &args[2];
    Variable* w_var = &args[3];
    Variable* h_var = &args[4];
    Variable* p_var = &args[5];
    
    if(!Variable_Data(a_var)){
        printf("[Window]: Rect -> %s is not init!\n",a_var->name);
    }else{
        WindowPtr* wptr = (WindowPtr*)Variable_Data(a_var);
        AlxWindow* alxw = (AlxWindow*)wptr->Memory;

        Double x = *(Double*)Variable_Data(x_var);
        Double y = *(Double*)Variable_Data(y_var);
        Double w = *(Double*)Variable_Data(w_var);
        Double h = *(Double*)Variable_Data(h_var);
        Pixel p = (Pixel)(*(Number*)Variable_Data(p_var));
        Rect_RenderXX(alxw->Buffer,alxw->Width,alxw->Height,x,y,w,h,p);
    }

    return Variable_Null();
}
Variable Window_String(Scope* sc,CStr name,Variable* args){
    Variable* a_var = &args[0];
    Variable* s_var = &args[1];
    Variable* x_var = &args[2];
    Variable* y_var = &args[3];
    Variable* p_var = &args[4];
    
    if(!Variable_Data(a_var)){
        printf("[Window]: String -> %s is not init!\n",a_var->name);
    }else{
        WindowPtr* wptr = (WindowPtr*)Variable_Data(a_var);
        AlxWindow* alxw = (AlxWindow*)wptr->Memory;

        CStr s = *(CStr*)Variable_Data(s_var);
        Double x = *(Double*)Variable_Data(x_var);
        Double y = *(Double*)Variable_Data(y_var);
        Pixel p = (Pixel)(*(Number*)Variable_Data(p_var));
        CStr_RenderAlxFont(alxw->Buffer,alxw->Width,alxw->Height,&alxw->AlxFont,s,x,y,p);
    }

    return Variable_Null();
}

void Ex_Packer(ExternFunctionMap* Extern_Functions,Vector* funcs,Scope* s){//Vector<CStr>
    TypeMap_PushContained(&s->types,funcs,
        Type_New("window",sizeof(AlxWindow),OperatorInterationMap_Make((OperatorInterater[]){
            OperatorInterater_Make((CStr[]){ NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_CAST,(Token(*)(void*,Token*,Vector*))Window_Handler_Cast),
                OperatorDefiner_New(TOKEN_INIT,NULL),
                OperatorDefiner_New(TOKEN_DESTROY,NULL),
                OPERATORDEFINER_END
            })),
            OperatorInterater_Make((CStr[]){ "window",NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_LUALIKE_ASS,(Token(*)(void*,Token*,Vector*))Window_Window_Handler_Ass),
                OPERATORDEFINER_END
            })),
            OperatorInterater_Make((CStr[]){ OPERATORINTERATER_DONTCARE,NULL },OperatorDefineMap_Make((OperatorDefiner[]){
                OperatorDefiner_New(TOKEN_LUALIKE_ACS,(Token(*)(void*,Token*,Vector*))Window_Any_Handler_Acs),
                OPERATORDEFINER_END
            })),
            OPERATORINTERATER_END
        }),Window_Destroyer,Window_Cpyer)
    );

    ExternFunctionMap_PushContained(Extern_Functions,funcs,ExternFunction_New("new",NULL,(Member[]){ 
        Member_New("str","name"),
        Member_New("int","width"),
        Member_New("int","height"),
        MEMBER_END
    },(void*)Window_Make));
    ExternFunctionMap_PushContained(Extern_Functions,funcs,ExternFunction_New("init",NULL,(Member[]){ 
        Member_New("window","w"),
        MEMBER_END
    },(void*)Window_Init));
    ExternFunctionMap_PushContained(Extern_Functions,funcs,ExternFunction_New("update",NULL,(Member[]){ 
        Member_New("window","w"),
        MEMBER_END
    },(void*)Window_Update));
    ExternFunctionMap_PushContained(Extern_Functions,funcs,ExternFunction_New("render",NULL,(Member[]){ 
        Member_New("window","w"),
        MEMBER_END
    },(void*)Window_Render));
    ExternFunctionMap_PushContained(Extern_Functions,funcs,ExternFunction_New("running","bool",(Member[]){ 
        Member_New("window","w"),
        MEMBER_END
    },(void*)Window_Running));
    ExternFunctionMap_PushContained(Extern_Functions,funcs,ExternFunction_New("width","float",(Member[]){ 
        Member_New("window","w"),
        MEMBER_END
    },(void*)Window_Width));
    ExternFunctionMap_PushContained(Extern_Functions,funcs,ExternFunction_New("height","float",(Member[]){ 
        Member_New("window","w"),
        MEMBER_END
    },(void*)Window_Height));
    
    ExternFunctionMap_PushContained(Extern_Functions,funcs,ExternFunction_New("elapsed","float",(Member[]){ 
        Member_New("window","w"),
        MEMBER_END
    },(void*)Window_Elapsed));
    ExternFunctionMap_PushContained(Extern_Functions,funcs,ExternFunction_New("key","bool",(Member[]){ 
        Member_New("window","w"),
        Member_New("str","key"),
        Member_New("str","action"),
        MEMBER_END
    },(void*)Window_Key));

    ExternFunctionMap_PushContained(Extern_Functions,funcs,ExternFunction_New("clear",NULL,(Member[]){ 
        Member_New("window","w"),
        Member_New("int","p"),
        MEMBER_END
    },(void*)Window_Clear));
    ExternFunctionMap_PushContained(Extern_Functions,funcs,ExternFunction_New("rect",NULL,(Member[]){ 
        Member_New("window","w"),
        Member_New("float","x"),
        Member_New("float","y"),
        Member_New("float","width"),
        Member_New("float","height"),
        Member_New("int","p"),
        MEMBER_END
    },(void*)Window_Rect));
    ExternFunctionMap_PushContained(Extern_Functions,funcs,ExternFunction_New("string",NULL,(Member[]){ 
        Member_New("window","w"),
        Member_New("str","s"),
        Member_New("float","x"),
        Member_New("float","y"),
        Member_New("int","p"),
        MEMBER_END
    },(void*)Window_String));

}
