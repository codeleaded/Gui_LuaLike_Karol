#include "/home/codeleaded/System/Static/Library/AlxCallStack.h"
#include "/home/codeleaded/System/Static/Library/AlxExternFunctions.h"
#include "/home/codeleaded/System/Static/Library/LuaLikeDefines.h"

Variable math_min(Scope* sc,CStr name,Variable* args){
    Variable* a = &args[0];
    Variable* b = &args[1];
    
    Number out = I64_Min(*(Number*)a->data,*(Number*)b->data);
    Variable ret = Variable_Make("OUT","int",(Number[]){ out },sizeof(Number),0,NULL,NULL);
    return ret;
}
Variable math_max(Scope* sc,CStr name,Variable* args){
    Variable* a = &args[0];
    Variable* b = &args[1];
    
    Number out = I64_Max(*(Number*)a->data,*(Number*)b->data);
    Variable ret = Variable_Make("OUT","int",(Number[]){ out },sizeof(Number),0,NULL,NULL);
    return ret;
}

Variable math_sin(Scope* sc,CStr name,Variable* args){
    Variable* a = &args[0];
    Double out = sin(*(Double*)Variable_Data(a));
    Variable ret = Variable_Make("OUT","float",(Double[]){ out },sizeof(Double),0,NULL,NULL);
    return ret;
}
Variable math_cos(Scope* sc,CStr name,Variable* args){
    Variable* a = &args[0];
    Double out = cos(*(Double*)Variable_Data(a));
    Variable ret = Variable_Make("OUT","float",(Double[]){ out },sizeof(Double),0,NULL,NULL);
    return ret;
}
Variable math_tan(Scope* sc,CStr name,Variable* args){
    Variable* a = &args[0];
    Double out = tan(*(Double*)Variable_Data(a));
    Variable ret = Variable_Make("OUT","float",(Double[]){ out },sizeof(Double),0,NULL,NULL);
    return ret;
}

Variable math_pow(Scope* sc,CStr name,Variable* args){
    Variable* a = &args[0];
    Variable* b = &args[1];
    Double out = pow(*(Double*)Variable_Data(a),*(Double*)Variable_Data(b));
    Variable ret = Variable_Make("OUT","float",(Double[]){ out },sizeof(Double),0,NULL,NULL);
    return ret;
}

Variable math_parseInt(Scope* sc,CStr name,Variable* args){
    Variable* a = &args[0];
    
    Number out = Number_Parse(*(CStr*)a->data);
    Variable ret = Variable_Make("OUT","int",(Number[]){ out },sizeof(Number),0,NULL,NULL);
    return ret;
}
Variable math_parseFloat(Scope* sc,CStr name,Variable* args){
    Variable* a = &args[0];
    
    Double out = Double_Parse(*(CStr*)a->data,1);
    Variable ret = Variable_Make("OUT","float",(Double[]){ out },sizeof(Double),0,NULL,NULL);
    return ret;
}
Variable math_getInt(Scope* sc,CStr name,Variable* args){
    Variable* a = &args[0];
    
    CStr out = Number_Get(*(Number*)a->data);
    Variable ret = Variable_Make("OUT","str",(CStr[]){ out },sizeof(CStr),0,Scope_DestroyerOfType(sc,"str"),Scope_CpyerOfType(sc,"str"));
    return ret;
}
Variable math_getFloat(Scope* sc,CStr name,Variable* args){
    Variable* a = &args[0];
    
    CStr out = Double_Get(*(Double*)a->data,8);
    Variable ret = Variable_Make("OUT","str",(CStr[]){ out },sizeof(CStr),0,Scope_DestroyerOfType(sc,"str"),Scope_CpyerOfType(sc,"str"));
    return ret;
}

void Ex_Packer(ExternFunctionMap* Extern_Functions,Vector* funcs,Scope* s){//Vector<CStr>
    ExternFunctionMap_PushContained_C(Extern_Functions,funcs,(ExternFunction[]){
        ExternFunction_New("min","int",(Member[]){ 
            Member_New("int","a"),
            Member_New("int","b"),
            MEMBER_END
        },(void*)math_min),
        ExternFunction_New("max","int",(Member[]){ 
            Member_New("int","a"),
            Member_New("int","b"),
            MEMBER_END
        },(void*)math_max),
        ExternFunction_New("sin","float",(Member[]){ 
            Member_New("float","a"),
            MEMBER_END
        },(void*)math_sin),
        ExternFunction_New("cos","float",(Member[]){ 
            Member_New("float","a"),
            MEMBER_END
        },(void*)math_cos),
        ExternFunction_New("tan","float",(Member[]){ 
            Member_New("float","a"),
            MEMBER_END
        },(void*)math_tan),
        ExternFunction_New("pow","float",(Member[]){ 
            Member_New("float","a"),
            Member_New("float","b"),
            MEMBER_END
        },(void*)math_pow),
        ExternFunction_New("parseInt","int",(Member[]){ 
            Member_New("str","a"),
            MEMBER_END
        },(void*)math_parseInt),
        ExternFunction_New("parseFloat","float",(Member[]){ 
            Member_New("str","a"),
            MEMBER_END
        },(void*)math_parseFloat),
        ExternFunction_New("getInt","str",(Member[]){ 
            Member_New("int","a"),
            MEMBER_END
        },(void*)math_getInt),
        ExternFunction_New("getFloat","str",(Member[]){ 
            Member_New("float","a"),
            MEMBER_END
        },(void*)math_getFloat),
        ExternFunction_Null()
    });
}
