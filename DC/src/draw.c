/* Y o u r   D e s c r i p t i o n                       */
/*                            AppBuilder Photon Code Lib */
/*                                         Version 2.03  */

/* Standard headers */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/* Local headers */
#include "ablibs.h"
#include "global.h"
#include "abimport.h"
#include "proto.h"
#include "math.h"

#define DRAW_PANE_SIZE 400
double VIEW_SCALE = 150000.0;
//double VIEW_SCALE = 32000.0;

struct pointArray pointArray;
struct pointTrassa pointTrassa[200];
struct pointTrassa pointTelevik[100];
struct PointTrassaServal pointServal;
struct PointTrassaX pointTrassa_X[300];
struct pointDraw67 pointDraw67;
double courseShipView = 0.0;
int count;

extern int shipType;

int TimerRepeat = 200;

// Рисование Сетки
void drawRoundNet(PhPoint_t _center, PhPoint_t _rad, float range){
	char s[10];	
	int step = 5000;
	int i,j;
	PhPoint_t p;
	char buff[10];
	PhPoint_t rad;
	float pixonmeter = (float)_rad.x / (float)range;
    	              
	PgSetStrokeColor(Pg_BLACK);
	PgDrawEllipse(&_center, &_rad, Pg_DRAW_STROKE);
    PgSetFillColor(Pg_WHITE);    
	PgSetStrokeColor(0x444444);
    PgSetStrokeWidth( 1 );
	strcpy(s, "\1\3");
	PgSetStrokeDash(s, 2, 0x10000 );     
    PgSetTextColor(0x444444);   


    if (range > 160000) step = 50000;
    else if (range > 60000) step = 10000;
    else if (range>11000) step = 5000;
    else if (range>3000) step = 1000;
    else step=500;
	for (i = 0; i < 360; ++i) 
    {
		//if ((i % 2) != 0)
		//
		PgSetStrokeDash(NULL, 0, 0x10000 );
		if ((i % 5 == 0) & (i % 30 != 0))
		PgDrawILine(
		  _center.x + rint ((_rad.x)*sinf(i*M_PI/180)),
		  _center.y - rint ((_rad.y)*cosf(i*M_PI/180)),
		  _center.x + rint ((_rad.x-10)*sinf(i*M_PI/180)),
		  _center.y - rint ((_rad.x-10)*cosf(i*M_PI/180)));
		if ((i % 5 != 0) )
		PgDrawILine(
		 _center.x + rint ((_rad.x)*sinf(i*M_PI/180)),
		  _center.y - rint ((_rad.y)*cosf(i*M_PI/180)),
		  _center.x + rint ((_rad.x-3)*sinf(i*M_PI/180)),
		  _center.y - rint ((_rad.x-3)*cosf(i*M_PI/180)));		
		strcpy(s, "\1\3");
		PgSetStrokeDash(s, 2, 0x10000 );     
		if (i % 30 == 0)
		PgDrawILine(
		  _center.x + rint ((_rad.x)*sinf(i*M_PI/180)),
		  _center.y - rint ((_rad.y)*cosf(i*M_PI/180)),
		  _center.x,
		  _center.y);
    }	
    
    for (i = 1; i < floor (range/step)+1; ++i) 
    {
    	rad.x = rint(i*step*pixonmeter);
    	rad.y = rad.x;    	
		PgDrawEllipse(&_center, 
		              &rad,
		              Pg_DRAW_STROKE);
		sprintf(s,"%0.0f",(float)step*i/1000.0);
		if (step < 1000) sprintf(s,"%0.0f",(float)step*i);
		if ((i%2==1)&&(range/step>7))
			continue;
		if (i*step>=range-2)
			continue;
		for (j = 0; j < 4; ++j) 
		{
			p.x = _center.x + rint (i*step*pixonmeter*sinf(j*M_PI/2));
    		p.y = _center.y - rint (i*step*pixonmeter*cosf(j*M_PI/2)
    		                    -10/2);
    	  	if (j%2==1) PgDrawText( s,
						strlen(s),    	  	
                        &p,
                		Pg_TEXT_CENTER  );		              
		}             	
	}
	

	// Значение углов
	for (i = 0; i < 360; ++i){
		if (i % 10 == 0){	
			p.x = _center.x + rint((_rad.x+10)*sinf(i*M_PI/180));
			p.y = _center.y - rint ((_rad.x+10)*cosf(i*M_PI/180))+5; 
			 	        
			if (i==0) strcpy(s, "N"); else strcpy(s, itoa(i,buff,10));		 	
			PgDrawText( s,
			     			strlen(s),    	  	
			                &p,
			           		Pg_TEXT_CENTER  );
			}  
	   }	
}


// Получение координаты точки по пеленгу и дальности
int getIkoScreenFromPel( PhPoint_t _center, int rad, float pixonmeter,
									float pelengreal, float distancereal,
									PhPoint_t* screenPos){
	int x, y;
	x  =  rint(_center.x + distancereal*sinf(pelengreal)* pixonmeter);
	y  =  rint(_center.y - distancereal*cosf(pelengreal)* pixonmeter);
	// Проверка граничных условий
	if(x>_center.x+rad-20)
		return 0;
	if(x<_center.x-rad+20)
		return 0;
	if(y>_center.y+rad-20)
		return 0;
	if(y<_center.y-rad+20)
		return 0;
	
	screenPos->x = x;
	screenPos->y = y;
	return 1;	
}

// Поворот объекта 
void rotatePhPoint(PhPoint_t * p, int numPoints, float angle)
{
 	int i;
 	float xres,yres;
 	for (i = 0; i < numPoints; ++i){
 		xres = (p[i].x)*cosf(angle) - (p[i].y)*sinf(angle);
 		yres = (p[i].x)*sinf(angle) + (p[i].y)*cosf(angle);
		p[i].x = rint(xres);
		p[i].y = rint(yres);			
	}
}	

// Отрисовка значков
void drawSign(
        PhPoint_t p,
        float course,
        int objType,
      	int own
      )
{
	PgColor_t color = Pg_RED;
	PhPoint_t 	ship[5] 	= {{-6,6},	{0,0}, 	{6,6},	{6,24},	{-6,24}} ,
			  	plane[11] 	= {{- 2,4}, {0,0}, 	{2,4},	{0,4},	{0,24},	{-4,8},	{4,8},{-6,16},{-2,12},{2,16},{6,12}},
				missile[5] 	= {{- 2,4}, {0,0}, 	{2,4},	{0,4},	{0,24}},			  	
				pkr[7] 		= {{- 2,4}, {0,0}, 	{2,4},	{0,4},	{0,20},	{-4,24},{4,24},},
				prr[9] 		= {{- 2,4}, {0,0}, 	{2,4},	{0,4},	{0,24},	{-4,16},{4,16},{-4,20},{4,20}},
				hel[6] 		= {{-8,4},	{8,-4},	{-8,-4},{8,4},	{0,0},	{0,10}};
	
	switch(own){
	case 2:color = Pg_RED;
		break;
	case 1:color = Pg_BLUE;
		break;
	default:color = 0x00AA00;		
		break;
	}
	
	PgSetStrokeColor( color);
	PgSetStrokeDash(NULL, 0, 0x10000 );	
	PgSetStrokeFWidth(0x10000);	
	PgSetFillColor( color);
	//objType = otPkr; // needtocorrect
	// ТЦ - тип цели 0-неопр, 1-надв, 8-возд неопр, 9-ловушка, 10-самолет, 
	        						// 11-ПКР, 12-ПРР, 13-ВЕРТ, 15-ВПЦ;
	switch (objType) {
		case 1:
                rotatePhPoint(ship,5,course);
            	PgSetStrokeFWidth(0x20000);	
    		    PgDrawPolygon(ship, 5, &p, Pg_DRAW_STROKE | Pg_CLOSED );
    		    break;				
		case 2:
                rotatePhPoint(ship,5,course);
            	PgSetStrokeFWidth(0x20000);	
                PgDrawPolygon(ship, 5, &p, Pg_DRAW_STROKE | Pg_CLOSED );
    		    break;				
		case 10:
                rotatePhPoint(plane,7,course);
            	PgSetStrokeFWidth(0x20000);	
                PgDrawPolygon(plane, 3, &p, Pg_DRAW_FILL | Pg_CLOSED );
    		    PgDrawILine(plane[3].x+p.x,plane[3].y+p.y,plane[4].x+p.x,plane[4].y+p.y);
    		    PgDrawILine(plane[5].x+p.x,plane[5].y+p.y,plane[6].x+p.x,plane[6].y+p.y);
    		    break;
		case 11:
                rotatePhPoint(pkr,7,course);				
            	PgSetStrokeFWidth(0x20000);	
                PgDrawPolygon(pkr, 3, &p, Pg_DRAW_FILL | Pg_CLOSED );
    		    PgDrawILine(pkr[3].x+p.x,pkr[3].y+p.y,pkr[4].x+p.x,pkr[4].y+p.y);
    		    PgDrawILine(pkr[4].x+p.x,pkr[4].y+p.y,pkr[5].x+p.x,pkr[5].y+p.y);
    		    PgDrawILine(pkr[4].x+p.x,pkr[4].y+p.y,pkr[6].x+p.x,pkr[6].y+p.y);    		    
    		    break;    		        		    
		case 12:
                rotatePhPoint(prr,9,course);				
            	PgSetStrokeFWidth(0x20000);	
                PgDrawPolygon(prr, 3, &p, Pg_DRAW_FILL | Pg_CLOSED );
    		    PgDrawILine(prr[3].x+p.x,prr[3].y+p.y,prr[4].x+p.x,prr[4].y+p.y);
    		    PgDrawILine(prr[5].x+p.x,prr[5].y+p.y,prr[6].x+p.x,prr[6].y+p.y);
    		    PgDrawILine(prr[7].x+p.x,prr[7].y+p.y,prr[8].x+p.x,prr[8].y+p.y);    		    
    		    break;    		        		    
		case 13:
                rotatePhPoint(hel,6,course);
            	PgSetStrokeFWidth(0x20000);	
                PgDrawILine(hel[0].x+p.x,hel[0].y+p.y,hel[1].x+p.x,hel[1].y+p.y);
    		    PgDrawILine(hel[2].x+p.x,hel[2].y+p.y,hel[3].x+p.x,hel[3].y+p.y);
    		    PgDrawILine(hel[4].x+p.x,hel[4].y+p.y,hel[5].x+p.x,hel[5].y+p.y);    		        		    
    		    break;    		        		    	        		    
		default:{ // like Plane
            	rotatePhPoint(plane,7,course);
            	PgSetStrokeFWidth(0x20000);	
            	PgDrawPolygon(plane, 3, &p, Pg_DRAW_FILL | Pg_CLOSED );
            	PgDrawILine(plane[3].x+p.x,plane[3].y+p.y,plane[4].x+p.x,plane[4].y+p.y);
            	PgDrawILine(plane[5].x+p.x,plane[5].y+p.y,plane[6].x+p.x,plane[6].y+p.y);
    		    break;
			}
	} 	// switch
	PgSetStrokeFWidth(0x10000);
} // drawSign


// Границы отброса
void drawBound(PhPoint_t _center, int _rad){
	int i;
	static PhPoint_t center = {0,0};
	PhPoint_t pos = {0,0};
	static int rad = 0;
	static PhPoint_t bound1[95];
	static PhPoint_t bound2[95];
	static PhPoint_t bound3[95];
	static PhPoint_t bound4[95];
	
	if(center.x!=_center.x||center.y!=_center.y||_rad!=rad){
		memcpy(&center, &_center, sizeof(PhPoint_t));
		rad = _rad;
		// 1я четверть
		for(i=0; i<=91; i++){
			bound1[i].x = center.x + (float)rad*cos(i/180.0*M_PI);
			bound1[i].y = center.y - (float)rad*sin(i/180.0*M_PI);
		}
		bound1[91].x = center.x + rad;
		bound1[91].y = center.y - rad;
		      
		bound1[92].x = bound1[0].x;
		bound1[92].y = bound1[0].y;
		// 2я четверть
		for(i=0; i<=91; i++){
			bound2[i].x = center.x +(float)rad*cos((i+90)/180.0*M_PI);
			bound2[i].y = center.y - (float)rad*sin((i+90)/180.0*M_PI);
		}
		bound2[91].x = center.x - rad;
		bound2[91].y = center.y - rad;
				      
		bound2[92].x = bound2[0].x;
		bound2[92].y = bound2[0].y;
		// 3я четверть
		for(i=0; i<=91; i++){
			bound3[i].x = center.x +(float)rad*cos((i+180)/180.0*M_PI);
			bound3[i].y = center.y - (float)rad*sin((i+180)/180.0*M_PI);
		}
		bound3[91].x = center.x - rad;
		bound3[91].y = center.y + rad;
				      
		bound3[92].x = bound3[0].x;
		bound3[92].y = bound3[0].y;
		// 4я четверть
		for(i=0; i<=91; i++){
			bound4[i].x = center.x +(float)rad*cos((i+270)/180.0*M_PI);
			bound4[i].y = center.y - (float)rad*sin((i+270)/180.0*M_PI);
		}
		bound4[91].x = center.x + rad;
		bound4[91].y = center.y + rad;
				      
		bound4[92].x = bound4[0].x;
		bound4[92].y = bound4[0].y;
	}
	
	PgSetFillColor(0xd9d9d9);
	PgDrawPolygon(  bound1, 93, &pos,Pg_DRAW_FILL | Pg_CLOSED);
	PgDrawPolygon(  bound2, 93, &pos,Pg_DRAW_FILL | Pg_CLOSED);
	PgDrawPolygon(  bound3, 93, &pos,Pg_DRAW_FILL | Pg_CLOSED);
	PgDrawPolygon(  bound4, 93, &pos,Pg_DRAW_FILL | Pg_CLOSED);
}

void draw67(DocDev67Tgt* tar, PhPoint_t _center, int rad, float pixonmeter){
		int i;
		PhPoint_t p,r;
		PhPoint_t pp[11];
		char s[20];
		char buff[10];
		unsigned int ea,ba;
		float da;
		PgSetTextColor( Pg_PURPLE); 
		PgSetFillColor( Pg_PURPLE);
		PgSetStrokeColor( Pg_PURPLE);
		
	    PgSetTextColor(Pg_BLACK);   
	    PgSetFont( Helvetica18 );
		
		if(!getIkoScreenFromPel(_center, rad, pixonmeter, tar->pel/180.0*M_PI, tar->dist*1000.0, &p))
			return;
		
		r = (PhPoint_t){4,4};
		
		if (tar->line == 2)
			PgDrawEllipse(&p,&r,Pg_DRAW_FILL); // отметка эллипсом
		else 
			PgDrawEllipse(&p,&r,Pg_DRAW_STROKE); // отметка эллипсом
			
		r = (PhPoint_t){1,1};
		PgDrawEllipse(&p,&r,Pg_DRAW_FILL);
			
		if (tar->mode == 2) // опознование в 2 режиме
		{	
				r = (PhPoint_t){6,6};
				PgDrawEllipse(&p,&r,Pg_DRAW_STROKE);
		}if (tar->num > 0)//бортовой номер в  режиме
		{
		  	p.x += 5;
		  	strcpy(s, itoa(tar->num,buff,10));
		  	PgDrawText( s,strlen(s),&p,	Pg_TEXT_LEFT );
		  	p.x += 7*strlen(s);
		}
		if (tar->B){
			p.x += 5;
			strcpy(s, "Б");
			PgDrawText( s,strlen(s),&p,	Pg_TEXT_LEFT );
		}
		

}//draw67

void drawCourse(PhPoint_t center, double rad, double course)// рисуем метку курса
{
	PgDefaultStroke(PgGetGC());
	PgSetStrokeWidth(4);
    PgSetStrokeColor( Pg_RED); 
    
    PgDrawILine(
    		center.x + 0.95*rint(rad*sinf(course)),
    		center.y - 0.95*rint(rad*cosf(course)),
    		center.x + 1.0*rint(rad*sinf(course)),
    		center.y - 1.0*rint(rad*cosf(course)));
}


void drawPane( PtWidget_t *widget, PhTile_t *damage ){
	char strBuff[50];
	char s[50];
	int strL;
	int i;
	time_t dopTime;
	PhPoint_t p1, p2;
	PhPoint_t radMyTest;
	PhPoint_t targetPoint;
	float pixonmeter = 230.0 / VIEW_SCALE;
	
	PhRect_t rect;
	
	PgSetFillColor(0xd9d9d9);
	
  	PgDrawRect(&widget->extent, Pg_DRAW_FILL);
	
    PgSetFillColor(Pg_WHITE);
    p1.x = 265;
    p1.y = 260;
    p2.x = 230;
    p2.y = 230;
    PgDrawEllipse(&p1, &p2, Pg_DRAW_FILL);
    
    
    // Проверка отрисовки
    PgSetFillColor(0x71FF71);
    PgSetStrokeColor(Pg_RED);
    radMyTest.x = 2;
    radMyTest.y = 2;
    
    dopTime = time(NULL);

    for(i=0; i<300; i++)
    {
    	
    	if(pointTrassa_X[i].draw ==1)
    	{
    		if(time(NULL)-pointTrassa_X[i].timeDraw>3.0)
    		{	
    			pointTrassa_X[i].draw = 0;
    			pointTrassa_X[i].nX = 0;
    		}	
    	}
    	if((pointTrassa_X[i].draw ==0) | (pointTrassa_X[i].nX ==0)) continue;	
    	if (pointTrassa_X[i].distance >VIEW_SCALE*1.2) continue;
    	getIkoScreenFromPel(p1, 230, pixonmeter, pointTrassa_X[i].peleng,
    	   									pointTrassa_X[i].distance, &targetPoint);
    	//PgDrawEllipse(&targetPoint, &radMyTest, Pg_DRAW_FILL);
    	drawSign(targetPoint, pointTrassa_X[i].course, 
    				pointTrassa_X[i].objType,
    				pointTrassa_X[i].own);
    	sprintf(strBuff,"X%d",pointTrassa_X[i].nX);
      	//if (count%10 ==3) 
   //   	{ printf ("%s peleng=%0.1f dist=%0.0f course=%0.1f type=%d",
     // 			strBuff,pointTrassa_X[i].peleng*180/M_PI,pointTrassa_X[i].distance,pointTrassa_X[i].course*180/M_PI,pointTrassa_X[i].objType); printf ("\n");}
    	strL = strlen(strBuff);
  
    	PgSetTextColor(Pg_BLACK);   
    	PgSetFont( Helvetica18 );

    	PgDrawText( strBuff, strL, &targetPoint, Pg_TEXT_BOTTOM);
    }
    count ++;
    if (count > 1000) count =0;
   // printf ("_________\n");

    
    // Отрисовка следов трасс
    for(i=0; i<pointArray.count; i++){
    	if(getIkoScreenFromPel(p1, 230, pixonmeter, pointArray.pointCoord[i].peleng,
    										pointArray.pointCoord[i].distance, &targetPoint))
    	{
        	if(pointArray.pointCoord[i].pr_own == 2)	PgSetFillColor(Pg_RED);
        	else if(pointArray.pointCoord[i].pr_own == 1)PgSetFillColor(Pg_BLUE);
        	else PgSetFillColor(0x71FF71);

    	}
    	PgDrawEllipse(&targetPoint, &radMyTest, Pg_DRAW_FILL);
    }
    // Отрисовка трасс
    for(i=0; i<150; i++){
    	if(pointTrassa[i].draw){
    		if(time(NULL)-pointTrassa[i].timeDraw>3){
    			pointTrassa[i].draw = 0;
    			continue;
    		}
    		if(getIkoScreenFromPel(p1, 230, pixonmeter, pointTrassa[i].peleng,
    		    										pointTrassa[i].distance, &targetPoint)){
    			drawSign(targetPoint, pointTrassa[i].course, 
    					pointTrassa[i].objType,
    					pointTrassa[i].own);
    			sprintf(strBuff,"F%d",pointTrassa[i].nSource);
    			strL = strlen(strBuff);
  
    		    PgSetTextColor(Pg_BLACK);   
    		    PgSetFont( Helvetica18 );

    			PgDrawText( strBuff, strL, &targetPoint, Pg_TEXT_BOTTOM);
    			
    		}
    		
    	}
    }
    
    // Отрисовка трасс
 /*   for(i=0; i<100; i++){
    	if(pointTelevik[i].draw){
    		if(time(NULL)-pointTelevik[i].timeDraw>3){
    			pointTelevik[i].draw = 0;
    			continue;
    		}
    		if(getIkoScreenFromPel(p1, 230, pixonmeter, pointTelevik[i].peleng,
    				pointTelevik[i].distance, &targetPoint)){
    			drawSign(targetPoint, pointTelevik[i].course, 
    					pointTelevik[i].objType,
    					pointTelevik[i].own);
    			sprintf(strBuff,"T%d",pointTelevik[i].nSource);
    			strL = strlen(strBuff);
  
    		    PgSetTextColor(Pg_BLACK);   
    		    PgSetFont( Helvetica18 );

    			PgDrawText( strBuff, strL, &targetPoint, Pg_TEXT_BOTTOM);
    			
    		}
    		
    	}
    }

	if(pointServal.draw)
	{
		if(time(NULL)-pointServal.timeDraw>3)
		{
			pointServal.draw = 0;
		} else
		{	
			if(getIkoScreenFromPel(p1, 230, pixonmeter, pointServal.peleng,
					pointServal.distance, &targetPoint))
			{
				drawSign(targetPoint, pointServal.course,pointServal.objType,owOur);
				sprintf(strBuff,"F%d",pointServal.nServal);
				if (pointServal.nTelevik >0) {sprintf(s,"T%d",pointServal.nTelevik); strcat(strBuff,s);}
				strL = strlen(strBuff);
				PgSetTextColor(Pg_BLACK);   
				PgSetFont( Helvetica18 );
				PgDrawText( strBuff, strL, &targetPoint, Pg_TEXT_BOTTOM);
			}
		}
	}
*/
    
   
	// Отрисовка отметок изделия 67
    for(i=0; i<pointDraw67.count; i++){
    	if(pointDraw67.draw[i])
    		if(time(NULL) - pointDraw67.timeDraw[i] > 2){
    			pointDraw67.draw[i] = 0;
    			continue;
    		}else{
    			draw67(&pointDraw67.dev67Tgt[i], p1, p2.x, pixonmeter);
    		}
    }
    
    
    drawBound(p1, 230);
    
    PgSetStrokeColor(Pg_BLACK);
   
   drawSign( p1, courseShipView, 1, 2);

   PgSetFont( Helvetica18 );
   drawRoundNet(p1, p2, VIEW_SCALE);
    
    // Курс корабля
    drawCourse(p1, p2.x, courseShipView);
}


int
armView( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	return( Pt_CONTINUE );
}


int
clearView( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )

	{
	int i;
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	pointArray.count = 0;
	pointArray.curPos = 0;
	for(i=0; i<150; i++){
		pointTrassa[i].draw = 0;
	}
	for(i=0; i<100; i++){
		pointTelevik[i].draw = 0;
	}
	pointServal.draw = 0;
	pointDraw67.count = 0;
	pointDraw67.curPos = 0;
	
	PtDamageWidget(ABW_paneDraw);
	return( Pt_CONTINUE );

	}


int
viewScaleDown( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )

	{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	
	if ((shipType ==99)| (shipType ==98))
	{
		if(VIEW_SCALE==1000.0){
			VIEW_SCALE = 2000.0;
			PtSetResource(ABW_viewScaleEdit, Pt_ARG_TEXT_STRING, "2km", 0);
		}else if(VIEW_SCALE==2000.0){
			VIEW_SCALE = 5000.0;
			PtSetResource(ABW_viewScaleEdit, Pt_ARG_TEXT_STRING, "5km", 0);
		}else if(VIEW_SCALE==5000.0){
			VIEW_SCALE = 10000.0;
			PtSetResource(ABW_viewScaleEdit, Pt_ARG_TEXT_STRING, "10km", 0);
		}else if(VIEW_SCALE==10000.0){
			VIEW_SCALE = 30000.0;
			PtSetResource(ABW_viewScaleEdit, Pt_ARG_TEXT_STRING, "30km", 0);
		}	
	} else
	{	
		if(VIEW_SCALE==16000.0){
			VIEW_SCALE = 64000.0;
			PtSetResource(ABW_viewScaleEdit, Pt_ARG_TEXT_STRING, "64km", 0);
		}else if(VIEW_SCALE==64000.0){
			VIEW_SCALE = 100000.0;
			PtSetResource(ABW_viewScaleEdit, Pt_ARG_TEXT_STRING, "100km", 0);
		}else if(VIEW_SCALE==100000.0){
			VIEW_SCALE = 150000.0;
			PtSetResource(ABW_viewScaleEdit, Pt_ARG_TEXT_STRING, "150km", 0);
		}else if(VIEW_SCALE==150000.0){
			VIEW_SCALE = 250000.0;
			PtSetResource(ABW_viewScaleEdit, Pt_ARG_TEXT_STRING, "250km", 0);
		}	
	}
	
	PtDamageWidget(ABW_paneDraw);
	return( Pt_CONTINUE );

	}


int
viewScaleUp( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )

	{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	if ((shipType ==99) | (shipType ==98))
	{
		if(VIEW_SCALE==2000.0){
			VIEW_SCALE = 1000.0;
			PtSetResource(ABW_viewScaleEdit, Pt_ARG_TEXT_STRING, "1km", 0);
		}else if(VIEW_SCALE==5000.0){
			VIEW_SCALE = 2000.0;
			PtSetResource(ABW_viewScaleEdit, Pt_ARG_TEXT_STRING, "2km", 0);
		}else if(VIEW_SCALE==10000.0){
			VIEW_SCALE = 5000.0;
			PtSetResource(ABW_viewScaleEdit, Pt_ARG_TEXT_STRING, "5km", 0);
		}else if(VIEW_SCALE==30000.0){
			VIEW_SCALE = 10000.0;
			PtSetResource(ABW_viewScaleEdit, Pt_ARG_TEXT_STRING, "10km", 0);
		}
	} else
	{	
	
		if(VIEW_SCALE==64000.0){
			VIEW_SCALE = 16000.0;
			PtSetResource(ABW_viewScaleEdit, Pt_ARG_TEXT_STRING, "16km", 0);
		}else if(VIEW_SCALE==100000.0){
			VIEW_SCALE = 64000.0;
			PtSetResource(ABW_viewScaleEdit, Pt_ARG_TEXT_STRING, "64km", 0);
		}else if(VIEW_SCALE==150000.0){
			VIEW_SCALE = 100000.0;
			PtSetResource(ABW_viewScaleEdit, Pt_ARG_TEXT_STRING, "100km", 0);
		}else if(VIEW_SCALE==250000.0){
			VIEW_SCALE = 150000.0;
			PtSetResource(ABW_viewScaleEdit, Pt_ARG_TEXT_STRING, "150km", 0);
		}
	}	
	PtDamageWidget(ABW_paneDraw);
	return( Pt_CONTINUE );

	}


int
slower( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	
	switch(TimerRepeat){
	case 50: TimerRepeat = 100;
		PtSetResource(ABW_labelSpeed, Pt_ARG_TEXT_STRING, "x10", 0);
		break;
	case 100: TimerRepeat = 200;
		PtSetResource(ABW_labelSpeed, Pt_ARG_TEXT_STRING, "x5", 0);
		break;
	case 200: TimerRepeat = 500;
		PtSetResource(ABW_labelSpeed, Pt_ARG_TEXT_STRING, "x2", 0);
		break; 
	case 500: TimerRepeat = 1000;
		PtSetResource(ABW_labelSpeed, Pt_ARG_TEXT_STRING, "x1", 0);
		break;
	default: TimerRepeat = 1000;
		PtSetResource(ABW_labelSpeed, Pt_ARG_TEXT_STRING, "x1", 0);
		break;
		
	}
	
	PtSetResource(ABW_playTimer,Pt_ARG_TIMER_REPEAT, TimerRepeat, Pt_TRUE);
	return( Pt_CONTINUE );

	}


int
faster( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	
	switch(TimerRepeat){
	case 100: TimerRepeat = 50;
		PtSetResource(ABW_labelSpeed, Pt_ARG_TEXT_STRING, "x20", 0);
		break;
	case 200: TimerRepeat = 100;
		PtSetResource(ABW_labelSpeed, Pt_ARG_TEXT_STRING, "x10", 0);
		break;
	case 500: TimerRepeat = 200;
		PtSetResource(ABW_labelSpeed, Pt_ARG_TEXT_STRING, "x5", 0);
		break;
	case 1000: TimerRepeat = 500;
		PtSetResource(ABW_labelSpeed, Pt_ARG_TEXT_STRING, "x2", 0);
		break;
	default:	TimerRepeat = 50;
		PtSetResource(ABW_labelSpeed, Pt_ARG_TEXT_STRING, "x20", 0);
		break;
	}
	
	PtSetResource(ABW_playTimer,Pt_ARG_TIMER_REPEAT, TimerRepeat, Pt_TRUE);
	return( Pt_CONTINUE );

	}

