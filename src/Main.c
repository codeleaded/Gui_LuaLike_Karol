#include "/home/codeleaded/System/Static/Library/LuaLike.h"

int main(int argc,char** argv){
    
    LuaLike ll = LuaLike_New("./code/Main.ll","./bin");
    Variable ret = LuaLike_Function(&ll,"main",(Variable[]){ 
        VARIABLE_END
    });
    LuaLike_PrintVariable(&ll,&ret);
    Variable_Free(&ret);
    
    LuaLike_Free(&ll);
    
    return 0;
}