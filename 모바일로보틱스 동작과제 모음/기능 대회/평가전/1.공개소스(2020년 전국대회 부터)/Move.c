/************************************************************************************************************/
/*                                                                                                          */
/*                                                Move.c                                                    */
/*                                                                                                          */
/*                                                                                       2020. 1. 1.        */
/************************************************************************************************************/
#include "Interface.h"

#define ginit(sname) oc = tot = gtc = psdTimer = 0; for(int i = 0 ; i < 4 ; i++) msp[i] = gsp[i] = 0; lcd(0,7,"%13s",sname);
#define LE oc = psdTimer = sgx = sgy = sga = 0;


#define O(x,y,z,ang) Omni(x,y,z,ang,sp,ac);
#define OS OmniSet(tot,ac);


ISR(TIMER1_OVF_vect){
	TCNT1H=0xFF; TCNT1L=0x70; //0.01초

	acf += 0.02;
	acf = (acf >= 1)? 1 : acf;

	cacf += 0.02;
	cacf = (cacf >= 1)? 1 : cacf;

	zacf += 0.05;
	zacf = (zacf >= 1)? 1 : zacf;

	IR();

	if(IR_1 && !ba1) ba1 = 1,bar_1++;
	else if(!IR_1 && ba1) ba1 = 0;

	if(IR_2 && !ba2) ba2 = 2,bar_2++,co[code++] = 1;
	else if(!IR_2 && ba2) ba2 = 0;

	if(IR_3 && !ba3) ba3 = 3,bar_3++;
	else if(!IR_3 && ba3) ba3 = 0;

	if(IR_4 && !ba4) ba4 = 4,bar_4++;
	else if(!IR_4 && ba4) ba4 = 0;

	if(IR_5 && !ba5) ba5 = 5,bar_5++;
	else if(!IR_5 && ba5) ba5 = 0;

	if(IND_1 && !sba1) sba1 = 1,sbar_1++,co[code++] = 2;
	else if(!IND_1 && sba1) sba1 = 0;

	if(IND_2 && !sba2) sba2 = 2,sbar_2++;
	else if(!IND_2 && sba2) sba2 = 0;

	if(pc)pc++;
	if(oc)oc++;
	if(gtc)gtc--;
	if(psdTimer)psdTimer++;
	if(code > 19) code = 0;

	wrcm(sgy,sgx,sga);
}


volatile unsigned int gtc; 
volatile double sgx,sgy,sga,msp[4],gsp[4];
volatile int co[20],code,IR_1,IR_2,IR_3,IR_4,IR_5,IND_1,IND_2,pro[9][5],tot;
volatile char buf[20];




int Omni(int x, int y, int z, int ang, int sp, int ac){
	int xv[4] = {x,y,-z,-ang},d =1;
	double mtp=  0;

	if(ac < 100 && ac) d = 2;
	if(ac > 100) ac-=100;

	for(int i = 0 ;i < 4 ; i++) if(mtp < fabs(xv[i]))mtp = fabs(xv[i]);
	for(int i = 0 ;i < 4 ; i++) msp[i] = xv[i]/mtp * sp;

	return ceil(mtp / sp * 100) + (abs(ac)/d - (2-d));
}
void OmniSet(int tot, int ac){
	int m = (ac < 0 || ac > 100)? 1 : 0,ac2;
	
	if(ac >100) ac-=100;
	ac2 =abs(ac);


	for(int i = 0 ;i < 4 ; i++) {
		if(oc >= 0 && !(ac < 0)) gsp[i] = (double)msp[i] * (oc + 1)/ac2;
		if(oc >= ac2 || (ac < 0)) gsp[i] = (double)msp[i];
		if(oc >= tot - ac2 + 1 && m) gsp[i] = (double)msp[i] * (tot - oc)/ac2;
	}
}

void Axis(int x, int y, int z, int sp, int ac){
	double e,s = gyro;

	ginit("Axis");

	O(x,y,0,0)
	sp *= fabs(sp / hypot(msp[0],msp[1]));
	tot = O(x,y,z,0);
	
	while(1){ OS
		if(!gtc){
			gtc = 1;

			if(!z){
				e = s - gyro;
				e *= (sp + 10)/10;
				if(fabs(e) >30) e = 0;
				gsp[2] = e;
			}

			if(!oc) oc = 1;
			if(oc >tot) {LE return;}
			speed(gsp[0],gsp[1],gsp[2],wa);
		}
	}
}
void Drift(int x, int y, int z, int sp, int ac){
	double e,s = gyro,na = 0;

	ginit("Drift");

	O(x,y,z,0)
	sp *= fabs(sp / hypot(msp[0],msp[1]));
	tot = O(x,y,z,0);
	
	while(1){ 
		if(!gtc){
			OS
			gtc = 1;
			

			na += -gsp[2]/100;

			if(!oc) oc = 1;
			if(oc > tot) {LE return;}
			speed(gsp[0],gsp[1],gsp[2],wa + na);
		}
	}
}
void Ot(int r, int z, int ang, int sp, int ac){
	double e,s = gyro,na = 0,na2 = 0;
	double y = r * abs(z) / 180 * M_PI;


	ginit("Ot");
	
	O(0,y,z,ang)
	sp *= fabs(sp / msp[1]);
	tot = O(0,y,z,ang);
	
	while(1){ OS
		if(!gtc){
			gtc = 1;

			na += gsp[2]/100;
			na2 += -gsp[3]/100;

			if(!oc) oc = 1;
			if(oc >tot) {LE return;}
			speed(0,gsp[1],gsp[3],wa + na + na2);
		}
	}
}
void MovePsd(int check, int ps, int x, int y, int z, int pc, int sp, int ac){
	double e,s = gyro;

	ginit("Axis");

	O(x,y,0,0)
	sp *= fabs(sp / hypot(msp[0],msp[1]));
	tot = O(x,y,z,0);
	
	while(1){ OS
		if(!gtc){
			gtc = 1;

			if(!z){
				e = s - gyro;
				e *= (sp + 10)/10;
				if(fabs(e) >30) e = 0;
				gsp[2] =e;
			}

			if(!oc) oc = 1;
			if(oc >tot) {LE return;}
			if(check == 0 && SensorIn(ps,pc)) {LE return;}
			if(check == 1 && g_psd[ps] >= pc) {LE return;}
			speed(gsp[0],gsp[1],gsp[2],wa);
		}
	}
}
void MoveLine(int check, int sel, int cm , int sp, int ac){
	double e,s = gyro;

	ginit("MoveLine");

	
	tot = O(0,cm,0,0);
	
	while(1){ OS
		if(!gtc){
			gtc = 1;

			int lines,c;

			lines = LS();
			if(lines != 11) c=  LC(lines);
			else c= 0;

			if(lines == 10 && check == 1) {LE return;}
			if(lines == 0 && check == 0) {LE return;}

			if(!oc) oc = 1;
			if(oc >tot) {LE return;}
			speed(0,gsp[1],-c,wa);
		}
	}
}
int MoveLineCheck(int check, int x, int y, int z, int sp, int ac){
	double e,s = gyro;

	ginit("MoveLineCheck");

	O(x,y,0,0)
	sp *= fabs(sp / hypot(msp[0],msp[1]));
	tot = O(x,y,z,0);
	
	while(1){ OS
		if(!gtc){
			gtc = 1;

			if(!z){
				e = s - gyro;
				e *= (sp + 10)/10;
				if(fabs(e) >30) e = 0;
				gsp[2] =e;
			}

			if(!oc) oc = 1;
			if(oc >tot) {LE return;}
			if(check == 0 && IR_2) {LE return 1;}
			if(check == 1 && IND_1) {LE return 2;}
			if(check == 2 && IR_2) {LE return 1;}
			if(check == 2 && IND_1) {LE return 2;}
			speed(gsp[0],gsp[1],gsp[2],wa);
		}
	}

}
void Avoid(int ps, int pm, int pm2, int dis, int y, int sp, int ac){
	double e,s = gyro;

	ginit("Avoid");
	
	int l = (ps + 1) %9, r = (ps + 8) % 9, zig =0, d = 1;
	if(y < 0) d = -1;



	tot = O(0,y,0,0);
	
	while(1){ OS
		if(!gtc){
			gtc = 1;

		
			e = s - gyro;
			e *= (sp + 10)/10;
			if(fabs(e) >30) e = 0;
	
			zig =0;
			if(g_psd[l] < pm) zig += (g_psd[l] - pm ) * 3.5;
			if(g_psd[r] < pm2) zig += (g_psd[r] - pm2 ) * -3.5;
			
			if(dis && SensorIn(ps,pc)) {LE return;}


			if(!oc) oc = 1;
			if(oc >tot) {LE return;}
			speed(-zig*d,gsp[1],e,wa);
		}
	}
}
int CPsd(int check, int ps, int cm, int v, int pm, int pc,int sp, int ac){
	double e,s = gyro;

	ginit("CPsd");

	tot = O(0,cm,0,0);
	int x,y,z,d=((ps * 40 + wa) % 360 < 180)? 1 : -1;
	while(1){ OS
		if(!gtc){
			gtc = 1;

			
			if(abs(check) < 9){
				if(g_psd[abs(check)] != 9){
					if(check <= 0 && SensorIn(abs(check),pc)) {LE return;}
					if(check > 0 && g_psd[abs(check)] >= pc) {LE return;}
				}
			}

			x = z = g_psd[ps] - pm;
			if(abs(z) >= 5 )z = LF(v*5,-v*5,z);

			if(cm < 0)z = -z;
			if(check == 12) z= 0, x*= 4;
			
			x/=2;
			x = LF(sp,-sp,x);

			if(check== 13 && IR_2) {LE return 1;}
			if(check== 13 && IND_1) {LE return 2;}
			if(!oc) oc = 1;
			if(oc >tot) {LE return;}
			speed(-x*d,gsp[1],-z*d*v,wa);
		}
	}
}
void At(double z, int sp){
	double e,s = gyro;

	ginit("At");

	zacf=  0.1;
	while(1){ 
		if(!gtc){
			gtc = 1;

			e =s - gyro -z;
			e = LF(sp,-sp,e*5);



			if(!oc) oc = 1;
			if(fabs(e) <= 0,1) {LE return;}
			speed(0,0,e*zacf,0);
		}
	}
}
void Ad(int x, int y, int z, int sp, int ac, int zsp){
	double e,s = gyro,na = 0;

	ginit("Ad");
	
	if(zsp) zacf = 0.1;
	if(zsp > 1){ 
		O(x,y,0,0)
		sp *= fabs(sp / hypot(msp[0],msp[1]));
		tot = O(x,y,0,0);
	}
	else{
		O(x,y,z,0)
		sp *= fabs(sp / hypot(msp[0],msp[1]));
		tot = O(x,y,z,0);
		zsp = abs((int)msp[2]);
	}
	
	while(1){ OS
		if(!gtc){
			gtc = 1;

			e =s - gyro -z;
			e = LF(zsp,-zsp,e*5);
	
			gsp[2] = e * zacf;
			na += -gsp[2]/100;


			if(!oc) oc = 1;
			if(oc >tot) {LE return;}
			speed(gsp[0],gsp[1],gsp[2],wa + na);
		}
	}
}

void TM(unsigned char wm){
	unsigned char buff=0;
	while(1){	//우선순위 큰것부터
		putchar1(17);
		putchar1(wm);
		while(!rx1_flg); 
		buff=getchar1();
		if(buff){
			buff=0;
			break;
		}
	}	
}
void TB(int col, int zp, int xp, int yp){
	double e,s = gyro;

	ginit("TB");

	int xpp=  0, ypp=  0,zpp= 0;
	int x =  0, y =  0,z = 0;

	pc = 0;


	while(1){ 
		if(!gtc){
			gtc = 1;

			xpp = zpp = Cmd(col,102);
			ypp = Cmd(col,103);
			

			while(!xpp && !ypp){
				SS();

				xpp = Cmd(col,102);
				ypp = Cmd(col,103);	

				if(!pc) pc = 1;
				if(pc > 350) {SS(); return;}
				if(xpp) acf = cacf = 0;
			}
			
			if(xp) x=  (xpp - xp) * 0.8;
			if(yp) y=  (yp - ypp);
			if(zp) z=  (zpp - zp) * 0.6;

			x = LF(30,-30,x);
			y = LF(55,-55,y);
			z = LF(50,-50,z);


			if(!oc) oc = 1;
			if(oc >350) {LE return;}
			if(abs(x) <= 0 && abs(y) <= 1 && abs(z) <= 0) {LE return;}
			speed(x*acf*0.8,y*cacf*0.8,z*acf,wa);
		}
	}
}
void TB2(int col, int xp, int yp){
	double e,s = gyro;

	ginit("TB2");

	int xpp=  0, ypp=  0,zpp= 0;
	int x =  0, y =  0,z = 0;
	

	xpp = Cmd(col,102);
	xp += pos[2];
	xp += (xp - xpp) * ((xp - xpp) / 600 + 0.24);


	pc = 0;


	while(1){ 
		if(!gtc){
			gtc = 1;

			xpp = Cmd(col,102);
			ypp = Cmd(col,103);
			

			while(!xpp && !ypp){
				SS();

				xpp = Cmd(col,102);
				ypp = Cmd(col,103);	

				if(!pc) pc = 1;
				if(pc > 350) {SS(); return;}
				if(xpp) acf = cacf = 0;
			}
			
			if(xp) x=  (xpp - xp) * 0.8;
			if(yp) y=  (yp - ypp);
			

			x = LF(50,-50,x);
			y = LF(60,-60,y);
			


			if(!oc) oc = 1;
			if(oc >350) {LE return;}
			if(abs(x) <= 0 && abs(y) <= 1) {LE return;}
			speed(0,y*cacf*0.8,x*acf,wa + pos[2]);
		}
	}
}
void AB(int yp, int ym, int xp, int xm, int zp, int zm, int xsp, int ysp){
	double e,s = gyro;

	ginit("AB");

	int xbo=  1, ybo=  1,zbo= 1;
	int x =  0, y =  0,z = 0;

	double xra =(xm - 10)/xsp;
	double yra =(ym - 10)/ysp;

	int ap = (zp + xp); if(xp && zp) ap/=2;

	int ya = (abs(yp * 40 -360) < abs(yp*40))? yp * 40 - 360 : yp * 40;

	if(ap * 40 + wa > 180) xbo *= -1;
	if(abs(ya + wa ) < 90) ybo *= -1;
	if(ap * 40 + wa > 180) zbo=  (xbo < 0)? 1 : -1;
	else zbo=  (xbo < 0)? -1 : 1;
	pc = 0;


	while(1){ 
		if(!gtc){
			gtc = 1;

			if(xm){
				x = (psd[xp] - xm) * xbo;
				x/= xra;
			}
			if(zm){
				z = (psd[xp] - psd[zp] - xm + zm) * zbo * 1.2;
			}
			else{
				z = s - gyro;
				if(ysp)z *= (ysp + 10)/10;
				else z *= (xsp + 10)/10;
				if(fabs(z) >30) z = 0;				

			}
			if(ym){
				y = (psd[yp] - ym) * ybo;
				y/= yra;
			}

			x = LF(xsp,-xsp,x);
			y = LF(ysp,-ysp,y);
			z = LF(xsp,-xsp,z);


			if(!oc) oc = 1;
			if(oc >500) {LE return;}
			if(abs(x) <= 0 && abs(y) <= 0 && abs(z) <= 0) {LE return;}
			speed(x*acf,y*cacf,z*acf,wa);
		}
	}
}

void IR(){
	IR_1 = (PINE&4)? 0 : 1;
	IR_2 = (PINE&8)? 0 : 1;
	IR_3 = (PINE&16)? 0 : 1;
	IR_4= (PIND&16)? 0 : 1;
	IR_5 = (PIND&64)? 0 : 1;

	IND_1 = (~PING&8)? 1 : 0;
	IND_2 = (~PING&16)? 1 : 0;
}
void SS(){
	speed(0,0,0,0);
}
int LS(){
	if(IR_1 == 0 && IR_2 == 0 && IR_3 == 0) return 0;
	if(IR_1 == 1 && IR_2 == 0 && IR_3 == 0) return 1;
	if(IR_1 == 1 && IR_2 == 1 && IR_3 == 0) return 2;
	if(IR_1 == 0 && IR_2 == 1 && IR_3 == 0) return 3;
	if(IR_1 == 0 && IR_2 == 1 && IR_3 == 1) return 4;
	if(IR_1 == 0 && IR_2 == 0 && IR_3 == 1) return 5;
	if(IR_1 == 1 && IR_2 == 1 && IR_3 == 1) return 10;
}
int LC(int lines){
	int hd[11] = {0,100,50,0,-50,-100};
	return (int)(hd[lines] * 0.25);
}
double LF(double max, double min, double v){
	if(max < v) return max;
	if(min > v) return min;
	return v;
}
int V1(){
	for(int i = 0 ; i < 9 ; i++)
		for(int j = 0 ; j  < 5 ; j++)
			pro[i][j] = 0;

	int am,str[200];

	rx1_flg= 0;

	while(1){
		gtc =50;
		rx1_flg=  0;

		while(!rx1_flg) if(!gtc) {return 0;}

		if(getchar1() == 'V'){
			rx1_flg = 0;
			while(!rx1_flg);
			am = getchar1() -'0';

			for(int i = 0 ; i < am * 15 ; i++){
				rx1_flg = 0;
				while(!rx1_flg);
				str[i] = getchar1() -'0';
			}
			break;
		}
	}

	for(int i = 0 ; i < am ; i++){
		pro[i][0]=  str[i * 15 + 1];
		pro[i][1]=  str[i * 15 + 3] * 100 + str[i * 15 + 4] * 10 + str[i * 15 + 5];
		pro[i][2]=  str[i * 15 + 6] * 100 + str[i * 15 + 7] * 10 + str[i * 15 + 8];
		pro[i][3]=  str[i * 15 + 9] * 100 + str[i * 15 + 10] * 10 + str[i * 15 + 11];
		pro[i][4]=  str[i * 15 + 12] * 100 + str[i * 15 + 13] * 10 + str[i * 15 + 14];
	}
	LE return 1;
}



int SensorIn(int n, int dis){
	if(g_psd[n] <= dis) {if(!psdTimer) psdTimer =1;}
	else psdTimer =0;
	return psdTimer >= 3;
}


int T(int data, int ci){
	int puck[5],col = 0,col2 = 0, min = 255,max = 0;

	for(int i = 1 ; i <= ci ; i++){
		puck[i] = Cmd(i,abs(data));

		if(puck[i] > max) max = puck[i], col = i;
		if(puck[i] < min && puck[i]) min = puck[i], col2 = i;
	}

	return (data > 0)?  col : col2;
}

