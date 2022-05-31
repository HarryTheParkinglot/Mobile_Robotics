/************************************************************************************************************/
/*                                                                                                          */
/*                                                Move.h                                                    */
/*                                                                                                          */
/*                                                                                       2020. 1. 1.        */
/************************************************************************************************************/
#ifndef __MOVE_H
#define __MOVE_H

#define SW1 (~PINB&0x10)
#define SW2 (~PINB&0x20)
#define SW3 (~PINB&0x40)
#define SW4 (~PINB&0x80)

#define LED_ON(x) (PORTB |= 1<<x)
#define LED_OFF(x) (PORTB &=~ 1<<x)

#define lcd(a,b,...) sprintf(buf,__VA_ARGS__); LCD(a,b,buf);
#define cha (M_PI / 180)

volatile extern unsigned int gtc; 
volatile extern double sgx,sgy,sga,msp[],gsp[];
volatile extern int co[20],code,IR_1,IR_2,IR_3,IR_4,IR_5,IND_1,IND_2,pro[9][5],tot;
volatile extern char buf[20];
volatile int pc;

int ba1,bar_1;
int ba2,bar_2;
int ba3,bar_3;
int ba4,bar_4;
int ba5,bar_5;

int sba1,sbar_1;
int sba2,sbar_2;

double acf,cacf,zacf;
int oc,wa,psdTimer;

int Omni(int x, int y, int z, int ang, int sp, int ac);
void OmniSet(int tot, int ac);

void Axis(int x, int y, int z, int sp, int ac);
void Drift(int x, int y, int z, int sp, int ac);
void Ot(int r, int z, int ang, int sp, int ac);
void MovePsd(int check, int ps, int x, int y, int z,int pc, int sp, int ac);
void MoveLine(int check, int sel, int cm , int sp, int ac);
int MoveLineCheck(int check, int x, int y, int z, int sp, int ac);
void Avoid(int ps, int pm, int pm2, int dis, int y, int sp, int ac);
int CPsd(int check, int ps, int cm, int v, int pm, int pc,int sp, int ac);
void At(double z, int sp);
void Ad(int x, int y, int z, int sp, int ac, int zsp);

void TM(unsigned char wm);
void TB(int col, int zp, int xp, int yp);
void TB2(int col, int xp, int yp);
void AB(int yp, int ym, int xp, int xm, int zp, int zm, int xsp, int ysp);

void IR();
void SS();
int LS();
int LC(int lines);
double LF(double max, double min, double v);

int V1();

int SensorIn(int n, int dis);
#endif      // __MOVE_H
