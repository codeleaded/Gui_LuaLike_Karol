#ifndef KAROL_H
#define KAROL_H

#include "/home/codeleaded/System/Static/Library/Lib3D_Cube.h"
#include "/home/codeleaded/System/Static/Library/Lib3D_Mathlib.h"
#include "/home/codeleaded/System/Static/Library/Lib3D_Mesh.h"

typedef struct BodyPart {
    Vec3D p;
    Vec3D a;
    Vector mesh;
} BodyPart;

BodyPart BodyPart_New(Vec3D a,Vec3D p,short sides,Vec3D start,Vec3D l,unsigned int col){
    BodyPart bp;
    bp.p = p;
    bp.a = a;
    bp.mesh = Vector_New(sizeof(Tri3D));
	Lib3D_Sides(&bp.mesh,sides,start,l,col,col);
    return bp;
}
void BodyPart_Free(BodyPart* bp){
    Vector_Free(&bp->mesh);
}



typedef SharedPointer KarolPtr;

typedef struct Karol {
    Vec3D p;
    Vec3D d;
    Vec3D a;
    Vector bodyparts;
} Karol;

// Head Body Arm(1,2) Leg(1,2) Feet(1,2)
void Karol_StdFigur(Karol* k,float xl,float yl,float zl,unsigned int c1,unsigned int c2,unsigned int c3,unsigned int c4,unsigned int c5,unsigned int c6,unsigned int c7,unsigned int c8) {
	Vector_Push(&k->bodyparts,(BodyPart[]){ BodyPart_New((Vec3D){0.0f,0.0f,0.0f}, (Vec3D){ 0.0f,-yl/8.0f,0.0f                },0,                                              (Vec3D){ -xl/12.0f*4.0f,0.0f,-zl/12.0f*4.0f           },(Vec3D){ xl/12.0f*8.0f,yl/4.0f,zl/12.0f*8.0f      },c1) });
	Vector_Push(&k->bodyparts,(BodyPart[]){ BodyPart_New((Vec3D){0.0f,0.0f,0.0f}, (Vec3D){ 0.0f,-yl/8.0f*4,0.0f              },0,                                              (Vec3D){ -xl/12.0f*4.0f,0.0f,-zl/12.0f*2.0f           },(Vec3D){ xl/12.0f*8.0f,yl/8.0f*3.0f,zl/12.0f*4.0f },c2) });
	
	Vector_Push(&k->bodyparts,(BodyPart[]){ BodyPart_New((Vec3D){0.0f,0.0f,0.0f}, (Vec3D){ xl/2.0f,-yl/16.0f*3.0f,0.0f       },0,                                              (Vec3D){ -xl/12.0f*2.0f,-yl/16.0f*5.0f,-zl/12.0f*2.0f },(Vec3D){ xl/12.0f*4.0f,yl/8.0f*3.0f,zl/12.0f*4.0f },c3) });
	Vector_Push(&k->bodyparts,(BodyPart[]){ BodyPart_New((Vec3D){0.0f,0.0f,0.0f}, (Vec3D){ -xl/2.0f,-yl/16.0f*3.0f,0.0f      },0,                                              (Vec3D){ -xl/12.0f*2.0f,-yl/16.0f*5.0f,-zl/12.0f*2.0f },(Vec3D){ xl/12.0f*4.0f,yl/8.0f*3.0f,zl/12.0f*4.0f },c4) });
	
	Vector_Push(&k->bodyparts,(BodyPart[]){ BodyPart_New((Vec3D){0.0f,0.0f,0.0f}, (Vec3D){ xl/12.0f*2.0f,-yl/8.0f*4.0f,0.0f  },(LIB3D_CUBE_S0_BOTTOM | LIB3D_CUBE_S1_BOTTOM),  (Vec3D){ -xl/12.0f*2.0f,-yl/8.0f*2.0f,-zl/12.0f*2.0f  },(Vec3D){ xl/12.0f*4.0f,yl/8.0f*2.0f,zl/12.0f*4.0f },c5) });
	Vector_Push(&k->bodyparts,(BodyPart[]){ BodyPart_New((Vec3D){0.0f,0.0f,0.0f}, (Vec3D){ -xl/12.0f*2.0f,-yl/8.0f*4.0f,0.0f },(LIB3D_CUBE_S0_BOTTOM | LIB3D_CUBE_S1_BOTTOM),  (Vec3D){ -xl/12.0f*2.0f,-yl/8.0f*2.0f,-zl/12.0f*2.0f  },(Vec3D){ xl/12.0f*4.0f,yl/8.0f*2.0f,zl/12.0f*4.0f },c6) });

	Vector_Push(&k->bodyparts,(BodyPart[]){ BodyPart_New((Vec3D){0.0f,0.0f,0.0f}, (Vec3D){ xl/12.0f*2.0f,-yl/8.0f*4.0f,0.0f  },(LIB3D_CUBE_S0_TOP | LIB3D_CUBE_S1_TOP),        (Vec3D){ -xl/12.0f*2.0f,-yl/8.0f*3.0f,-zl/12.0f*2.0f  },(Vec3D){ xl/12.0f*4.0f,yl/8.0f,zl/12.0f*4.0f      },c7) });
	Vector_Push(&k->bodyparts,(BodyPart[]){ BodyPart_New((Vec3D){0.0f,0.0f,0.0f}, (Vec3D){ -xl/12.0f*2.0f,-yl/8.0f*4.0f,0.0f },(LIB3D_CUBE_S0_TOP | LIB3D_CUBE_S1_TOP),        (Vec3D){ -xl/12.0f*2.0f,-yl/8.0f*3.0f,-zl/12.0f*2.0f  },(Vec3D){ xl/12.0f*4.0f,yl/8.0f,zl/12.0f*4.0f      },c8) });
}
Karol Karol_New(Vec3D p,Vec3D d){
    Karol k;
    k.p = p;
    k.d = d;
    k.a = (Vec3D){ 0.0f,0.0f,0.0f,1.0f };
    k.bodyparts = Vector_New(sizeof(BodyPart));
    Karol_StdFigur(&k,d.x,d.y,d.z,0xFFAAAA00,0xFF000088,0xFF003399,0xFF003399,0xFF00FF00,0xFF00FF00,0xFF777777,0xFF777777);
    return k;
}
void Karol_Add(Karol* k,Vector* tris){
    M4x4D matY = Matrix_MakeRotationY(k->a.y);
    M4x4D matX = Matrix_MakeRotationX(k->a.x);
    
	for(int i = 0;i<k->bodyparts.size;i++){
        BodyPart* bp = (BodyPart*)Vector_Get(&k->bodyparts,i);
        
        for(int j = 0;j<bp->mesh.size;j++) {
            Tri3D* tri = (Tri3D*)Vector_Get(&bp->mesh,j);

			M4x4D matTurnX = Matrix_MakeRotationX(bp->a.x);
			M4x4D matTurnY = Matrix_MakeRotationY(bp->a.y);
			M4x4D matTurnZ = Matrix_MakeRotationZ(bp->a.z);
			
	        Tri3D triRotated;
            triRotated.p[0] = Vec3D_Add(bp->p,Matrix_MultiplyVector(matTurnZ,Matrix_MultiplyVector(matTurnY,Matrix_MultiplyVector(matTurnX,tri->p[0]))));
            triRotated.p[1] = Vec3D_Add(bp->p,Matrix_MultiplyVector(matTurnZ,Matrix_MultiplyVector(matTurnY,Matrix_MultiplyVector(matTurnX,tri->p[1]))));
            triRotated.p[2] = Vec3D_Add(bp->p,Matrix_MultiplyVector(matTurnZ,Matrix_MultiplyVector(matTurnY,Matrix_MultiplyVector(matTurnX,tri->p[2]))));

            triRotated.p[0] = Matrix_MultiplyVector(matY,triRotated.p[0]);
            triRotated.p[1] = Matrix_MultiplyVector(matY,triRotated.p[1]);
            triRotated.p[2] = Matrix_MultiplyVector(matY,triRotated.p[2]);

            triRotated.p[0] = Vec3D_Add(k->p,triRotated.p[0]);
            triRotated.p[1] = Vec3D_Add(k->p,triRotated.p[1]);
            triRotated.p[2] = Vec3D_Add(k->p,triRotated.p[2]);
            triRotated.c = tri->c;

            Tri3D_CalcNorm(&triRotated);
            //Tri3D_ShadeNorm(&triRotated,(Vec3D){ 0.6f,0.7f,0.8f });
	    	
            Vector_Push(tris,&triRotated);
		}
	}
}
void Karol_Free(Karol* k){
    for(int i = 0;i<k->bodyparts.size;i++){
        BodyPart* bp = (BodyPart*)Vector_Get(&k->bodyparts,i);
        BodyPart_Free(bp);
    }
    Vector_Free(&k->bodyparts);
}

#endif