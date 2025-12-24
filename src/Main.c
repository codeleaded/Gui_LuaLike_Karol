#include "/home/codeleaded/System/Static/Library/WindowEngine1.0.h"
#include "/home/codeleaded/System/Static/Library/LuaLike.h"
#include "/home/codeleaded/System/Static/Library/TransformedView.h"
#include "/home/codeleaded/System/Static/Library/Lib3D_Cube.h"
#include "/home/codeleaded/System/Static/Library/Lib3D_Mathlib.h"
#include "/home/codeleaded/System/Static/Library/Lib3D_Mesh.h"
#include "/home/codeleaded/System/Static/Library/Karol.h"

#define FIELD_SIZEX  10
#define FIELD_SIZEY  10
#define FIELD_SIZEZ  10


TransformedView tv;
LuaLike ll;

Camera cam;
World3D world;
int Mode = 0;
int Menu = 0;
float Speed = 4.0f;

void Menu_Set(int m){
	if(Menu==0 && m==1){
		AlxWindow_Mouse_SetInvisible(&window);
		SetMouse((Vec2){ GetWidth() / 2,GetHeight() / 2 });
	}
	if(Menu==1 && m==0){
		AlxWindow_Mouse_SetVisible(&window);
	}
	
	Menu = m;
}

void Setup(AlxWindow* w){
	ll = LuaLike_New("./code/Main.ll","./bin");
	tv = TransformedView_New((Vec2){ GetWidth(),GetHeight() });

    cam = Camera_Make(
		(Vec3D){ FIELD_SIZEX,8.0f,FIELD_SIZEZ,1.0f },
		(Vec3D){ 3.14 * 0.25f,3.14 * 0.75f,0.0f,1.0f },
		90.0f
	);

	world = World3D_Make(
		Matrix_MakeWorld((Vec3D){ 0.0f,0.0f,0.0f,1.0f },(Vec3D){ 0.0f,0.0f,0.0f,1.0f }),
		Matrix_MakePerspektive(cam.p,cam.up,cam.a),
		Matrix_MakeProjection(cam.fov,(float)GetHeight() / (float)GetWidth(),0.1f,1000.0f)
	);
	world.normal = WORLD3D_NORMAL_CAP;

    Menu_Set(1);
}

void Update(AlxWindow* w){
    if(Stroke(ALX_KEY_SPACE).PRESSED){
        const float x = 0.0f;
        const float y = 0.0f;
        const float z = 0.0f;

        Karol* pw = (Karol*)malloc(sizeof(Karol));
        *pw = Karol_New((Vec3D){ x + 0.75f * 0.75f,y + 1.75f,z + 0.75f * 0.75f },(Vec3D){ 0.75f,2.0f,0.75f });
        KarolPtr ptr = SharedPointer_Make(pw,(void*)Karol_Free);

        CStr name = Enviroment_Variablename_Next(&ll.ev,".KAROL",6);
        Scope_BuildInitVariableRange(&ll.ev.sc,name,"karol",0,&ptr);
        CStr_Free(&name);
    }
    if(Stroke(ALX_KEY_ENTER).PRESSED){
        Enviroment_ClearScript(&ll.ev);
        Enviroment_Script(&ll.ev,"./code/Main.ll");

        Variable ret = LuaLike_Function(&ll,"main",(Variable[]){ 
            VARIABLE_END
        });

        // Variable_Print(&ret);
        // printf("\n");

        Variable_Free(&ret);
    }

    if(Menu==1){
		Camera_Focus(&cam,GetMouseBefore(),GetMouse(),GetScreenRect().d);
		Camera_Update(&cam);
		SetMouse((Vec2){ GetWidth() / 2,GetHeight() / 2 });
	}
	
	if(Stroke(ALX_KEY_ESC).PRESSED)
		Menu_Set(!Menu);

	if(Stroke(ALX_KEY_Z).PRESSED)
		Mode = Mode < 2 ? Mode + 1 : 0;

	if(Stroke(ALX_KEY_W).DOWN)
		cam.p = Vec3D_Add(cam.p,Vec3D_Mul(cam.ld,Speed * w->ElapsedTime));
	if(Stroke(ALX_KEY_S).DOWN)
		cam.p = Vec3D_Sub(cam.p,Vec3D_Mul(cam.ld,Speed * w->ElapsedTime));
	if(Stroke(ALX_KEY_A).DOWN)
		cam.p = Vec3D_Add(cam.p,Vec3D_Mul(cam.sd,Speed * w->ElapsedTime));
	if(Stroke(ALX_KEY_D).DOWN)
		cam.p = Vec3D_Sub(cam.p,Vec3D_Mul(cam.sd,Speed * w->ElapsedTime));
	if(Stroke(ALX_KEY_R).DOWN)
		cam.p.y += Speed * w->ElapsedTime;
	if(Stroke(ALX_KEY_F).DOWN)
		cam.p.y -= Speed * w->ElapsedTime;


    tv.ZoomSpeed = 1.0f * w->ElapsedTime;
	TransformedView_HandlePanZoom(&tv,window.Strokes,GetMouse());

    //World3D_Set_Model(&world,Matrix_MakeWorld((Vec3D){ 0.0f,0.0f,0.0f,1.0f },(Vec3D){ 0.0f,0.0f,0.0f,1.0f }));
	World3D_Set_View(&world,Matrix_MakePerspektive(cam.p,cam.up,cam.a));
	World3D_Set_Proj(&world,Matrix_MakeProjection(cam.fov,(float)GetHeight() / (float)GetWidth(),0.1f,1000.0f));

	Clear(LIGHT_BLUE);


    Vector_Clear(&world.trisIn);
	for(int i = 0;i<FIELD_SIZEZ;i++){
		for(int j = 0;j<FIELD_SIZEX;j++){
		    Lib3D_Sides(&world.trisIn,~((short)LIB3D_CUBE_S0_TOP | (short)LIB3D_CUBE_S1_TOP),(Vec3D){ i + 0.05f,-1.0f,j + 0.05f },(Vec3D){ 0.9f,1.0f,0.9f },DARK_BLUE,DARK_BLUE);
	    }
	}
    
    for(int i = 0;i<ll.ev.sc.vars.size;i++){
        Variable* var = (Variable*)PVector_Get(&ll.ev.sc.vars,i);
        
        if(CStr_Cmp(var->typename,"karol")){
            KarolPtr* wptr = (KarolPtr*)Variable_Data(var);
            Karol* k = (Karol*)wptr->Memory;

            Karol_Add(k,&world.trisIn);
        }
    }

	World3D_update(&world,cam.p,(Vec2){ GetWidth(),GetHeight() });

	for(int i = 0;i<world.trisOut.size;i++){
		Tri3D* t = (Tri3D*)Vector_Get(&world.trisOut,i);

		if(Mode==0)
			RenderTriangle(((Vec2){ t->p[0].x, t->p[0].y }),((Vec2){ t->p[1].x, t->p[1].y }),((Vec2){ t->p[2].x, t->p[2].y }),t->c);
		if(Mode==1)
			RenderTriangleWire(((Vec2){ t->p[0].x, t->p[0].y }),((Vec2){ t->p[1].x, t->p[1].y }),((Vec2){ t->p[2].x, t->p[2].y }),t->c,1.0f);
		if(Mode==2){
			RenderTriangle(((Vec2){ t->p[0].x, t->p[0].y }),((Vec2){ t->p[1].x, t->p[1].y }),((Vec2){ t->p[2].x, t->p[2].y }),t->c);
			RenderTriangleWire(((Vec2){ t->p[0].x, t->p[0].y }),((Vec2){ t->p[1].x, t->p[1].y }),((Vec2){ t->p[2].x, t->p[2].y }),WHITE,1.0f);
		}
	}

	String str = String_Format("PX:%f, PY:%f, PZ:%f",cam.p.x,cam.p.y,cam.p.z);
	RenderCStrSize(str.Memory,str.size,0.0f,0.0f,WHITE);
	String_Free(&str);

	str = String_Format("%d(%d): AX:%f, AY:%f",world.trisIn.size,Mode,cam.a.x,cam.a.y);
	RenderCStrSize(str.Memory,str.size,0.0f,GetAlxFont()->CharSizeY,WHITE);
	String_Free(&str);
}
void Delete(AlxWindow* w){
    LuaLike_Free(&ll);
    World3D_Free(&world);
}

int main(){
    if(Create("LL - Karol",1920,1080,1,1,Setup,Update,Delete))
        Start();
    return 0;
}