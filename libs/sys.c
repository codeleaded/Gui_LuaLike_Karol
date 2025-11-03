#include "/home/codeleaded/System/Static/Library/AlxCallStack.h"
#include "/home/codeleaded/System/Static/Library/AlxExternFunctions.h"
#include "/home/codeleaded/System/Static/Library/LuaLikeDefines.h"
#include "/home/codeleaded/System/Static/Library/Thread.h"

Variable sys_msleep(Scope* sc,CStr name,Variable* args){
    Variable* a = &args[0];
    
    Number duration = *(Number*)a->data;
    Thread_Sleep_M(duration);
    return Variable_Null();
}
Variable sys_usleep(Scope* sc,CStr name,Variable* args){
    Variable* a = &args[0];
    
    Number duration = *(Number*)a->data;
    Thread_Sleep_U(duration);
    return Variable_Null();
}
Variable sys_nsleep(Scope* sc,CStr name,Variable* args){
    Variable* a = &args[0];
    
    Number duration = *(Number*)a->data;
    Thread_Sleep_N(duration);
    return Variable_Null();
}

void Ex_Packer(ExternFunctionMap* Extern_Functions,Vector* funcs,Scope* s){//Vector<CStr>
    ExternFunctionMap_PushContained_C(Extern_Functions,funcs,(ExternFunction[]){
        ExternFunction_New("msleep",NULL,(Member[]){ 
            Member_New("int","a"),
            MEMBER_END
        },(void*)sys_msleep),
        ExternFunction_New("usleep",NULL,(Member[]){ 
            Member_New("int","a"),
            MEMBER_END
        },(void*)sys_usleep),
        ExternFunction_New("nsleep",NULL,(Member[]){ 
            Member_New("int","a"),
            MEMBER_END
        },(void*)sys_nsleep),
        ExternFunction_Null()
    });
}
