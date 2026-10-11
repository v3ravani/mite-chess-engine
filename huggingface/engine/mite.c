#include<stdio.h>
#include<string.h>

#ifdef __EMSCRIPTEN__
#include<emscripten.h>
#define EXPORT EMSCRIPTEN_KEEPALIVE
static int GetTickCount(void){return (int)emscripten_get_now();}
#elif defined(_WIN32)
#include<windows.h>
#define EXPORT
#else
#include<sys/time.h>
#define EXPORT
static int GetTickCount(void){struct timeval tv;gettimeofday(&tv,NULL);return tv.tv_sec*1000+tv.tv_usec/1000;}
#endif

enum{PN=1,KN,BI,RO,QN,KI};
#define INF 30000
#define MD 24
#define MM 256
#define ap(p) ((p)>0?(p):-(p))
#define cl(p) ((p)>0?1:-1)
#define B(s) (((s)+((s)&7))>>1)
#define OK(s) (!((s)&~0x77))
#define VC(c) ((c)==99?100:V[ap(c)])
#define SW(i,j) {M tm=mv[i];mv[i]=mv[j];mv[j]=tm;int ts=sc[i];sc[i]=sc[j];sc[j]=ts;}
#define PIK(i,n) {int be=i,pj;for(pj=i+1;pj<n;pj++)if(sc[pj]>sc[be])be=pj;if(be!=i)SW(i,be);}
#define AM(f,t,c,p) mv[n++]=(M){f,t,c,p,0,0,0ULL}
#define Z0 (M){0,0,0,0,0,0,0ULL}
typedef unsigned long long ULL;
typedef struct{int f,t,c,p,r,e;ULL h;}M;
typedef struct{ULL k;short sc;char f,t,d,fl,gn,pd;}TT;
#define TTSZ (1<<20)
TT tt[TTSZ];
ULL Z[14][128],h,hs[1024];
#define Z_side Z[13][0]
int hp=0,b[128],sd,mn,cr,ab,ep,t0,tm=2800,nc,cd,ko[2],wm,bx,ir[1024];
char gn,T[7][32],KD[]={-17,-15,15,17,-16,-1,1,16},ND[]={-33,-31,-18,-14,14,18,31,33};
#define fk(c) ko[(c)>0]
int V[]={0,100,320,330,500,900,20000},pb[]={0,10,20,35,70,140,280,0};
M K[32][2],CM[64][64];short H[64][64];

void s2(int s,char*o){o[0]='a'+(s&7);o[1]='1'+(s>>4);o[2]=0;}
void ms2(M*m,char*s){s2(m->f,s);s2(m->t,s+2);s[4]=m->p?"  nbrq"[ap(m->p)]:0;s[5]=0;}
ULL gh(void){int s;ULL v=(sd<0?Z_side:0)^Z[13][cr];if(ep)v^=Z[13][16+(ep&7)];for(s=0;s<128;s++){if(s&0x88){s+=7;continue;}if(b[s])v^=Z[b[s]+6][s];}return v;}

void mk(M*m){
int d=m->t-m->f,p=b[m->f],a=ap(p),np;
m->r=cr;m->e=ep;m->h=h;hs[hp]=h;ir[hp]=(m->c||a==PN)?hp:(hp>0?ir[hp-1]:0);hp++;
if(m->f==4)cr&=12;if(m->f==116)cr&=3;
if(m->f==0||m->t==0)cr&=13;if(m->f==7||m->t==7)cr&=14;
if(m->f==112||m->t==112)cr&=7;if(m->f==119||m->t==119)cr&=11;
if(a==KI)ko[sd>0]=m->t;
h^=Z[p+6][m->f];
if(m->c==99){int es=m->t-sd*16;b[es]=0;h^=Z[-sd*PN+6][es];}
else if(m->c)h^=Z[m->c+6][m->t];
if(a==KI&&(d==2||d==-2)){int rf=m->f+(d>0?3:-4),rt=m->f+(d>0?1:-1);b[rt]=b[rf];b[rf]=0;h^=Z[sd*RO+6][rf]^Z[sd*RO+6][rt];}
if(a==PN&&(d==32||d==-32))ep=m->f+d/2;else ep=0;
h^=Z[13][m->r]^Z[13][cr];if(m->e)h^=Z[13][16+(m->e&7)];if(ep)h^=Z[13][16+(ep&7)];
np=m->p?m->p:p;b[m->t]=np;b[m->f]=0;h^=Z[np+6][m->t]^Z_side;}

void um(M*m){
int d=m->t-m->f,p=ap(b[m->t]);
if(hp>0)hp--;
if(p==KI){ko[sd>0]=m->f;if(d==2||d==-2){int rf=m->f+(d>0?3:-4),rt=m->f+(d>0?1:-1);b[rf]=b[rt];b[rt]=0;}}
if(m->c==99){b[m->f]=sd*PN;b[m->t]=0;b[m->t-sd*16]=-sd*PN;}
else{b[m->f]=m->p?sd*PN:b[m->t];b[m->t]=m->c;}
cr=m->r;ep=m->e;h=m->h;}

void ni(void){
int i,f,k;ULL se=987654321ULL,*z=(ULL*)Z;
memset(b,0,sizeof b);sd=1;mn=1;cr=15;ep=0;ko[1]=4;ko[0]=116;wm=bx=0;hp=0;memset(ir,0,sizeof ir);
memset(K,0,sizeof K);memset(H,0,sizeof H);memset(CM,0,sizeof CM);memset(tt,0,sizeof tt);memset(T,0,sizeof T);
for(i=0;i<166;i++)((char*)T)[32+i]=("KKKKLMMGLJIKKKKOLLMPMMOQUUUUKKKKACEECGKLELMNEKNOELNOEKMNCGKKACEEGIIIILKKIMMMIKMMILLMIKLMIKKKGIIIKKKLJKKKJKKKJKKKJKKKJKKKLMMMKKKKGIIJIKLKILLLKKLLJKLLIKLLIKKKGIIJOQMKOO"[i]-'K')*5;
for(i=8;i<32;i++)T[6][i]=i<12?-15:i<16?-30:-45;
for(f=0;f<8;f++){k="42356324"[f]-'0';b[f]=k;b[0x70+f]=-k;b[0x10+f]=PN;b[0x60+f]=-PN;}
for(i=0;i<14*128;i++)z[i]=(se=se*6364136223846793005ULL+1);
h=gh();}

void sf(char*f){
int r=7,c=0;char*P=" PNBRQKpnbrqk";
memset(b,0,sizeof b);cr=0;ep=0;hp=0;memset(ir,0,sizeof ir);
while(*f&&*f!=' '){if(*f=='/'){r--;c=0;}else if(*f>='1'&&*f<='8')c+=*f-'0';
else{char*p=strchr(P,*f);if(p){int v=p-P;b[(r<<4)|c]=(v<=6?v:6-v);if(ap(b[(r<<4)|c])==KI)ko[b[(r<<4)|c]>0]=(r<<4)|c;c++;}}f++;}
if(*f==' ')f++;sd=(*f=='b')?-1:1;
if(*f)f++;if(*f==' ')f++;
while(*f&&*f!=' '){char*k=strchr("KQkq",*f);if(k)cr|=1<<(k-"KQkq");f++;}
if(*f==' ')f++;if(*f>='a'&&*f<='h')ep=((f[1]-'1')<<4)|(f[0]-'a');h=gh();}

int at(int s,int y){
int x,i,v,t,p;
for(x=-1;x<=1;x+=2){int a=s-(y<<4)+x;if(OK(a)&&b[a]==y)return 1;}
for(i=0;i<8;i++){int a=s+ND[i],k=s+KD[i];if(OK(a)&&b[a]==y*KN)return 1;if(OK(k)&&b[k]==y*KI)return 1;}
for(i=0;i<8;i++){v=KD[i];for(t=s+v;OK(t);t+=v){p=b[t];if(!p)continue;if(p==y*(i<4?BI:RO)||p==y*QN)return 1;break;}}
return 0;}

int lg(M*m){int k;mk(m);k=fk(sd);k=k>=0&&!at(k,-sd);um(m);return k;}

int lva(int sq,int si,char*u){
int i,v,t,p,a,k;
for(i=-1;i<=1;i+=2){a=sq-(si<<4)+i;if(OK(a)&&!u[a]&&b[a]==si*PN)return a;}
for(i=0;i<8;i++){a=sq+ND[i];if(OK(a)&&!u[a]&&b[a]==si*KN)return a;}
for(k=BI;k<=QN;k++)for(i=(k==RO?4:0);i<(k==BI?4:8);i++){v=KD[i];for(t=sq+v;OK(t);t+=v){p=b[t];if(!p)continue;if(!u[t]&&p==si*k)return t;if(!u[t])break;}}
for(i=0;i<8;i++){a=sq+KD[i];if(OK(a)&&!u[a]&&b[a]==si*KI)return a;}
return -1;}

int see(M*m){
int sq=m->t,g[32],d[32],ng=0,si,a,i,v;char u[128]={0};
if(!m->c&&!m->p)return 1;
v=VC(m->c);u[m->f]=1;g[ng++]=V[ap(b[m->f])];si=-sd;
for(;;){a=lva(sq,si,u);if(a<0)break;u[a]=1;g[ng++]=V[ap(b[a])];si=-si;if(ng>=30)break;}
if(ng==1)return 1;
d[ng-1]=g[ng-2];
for(i=ng-2;i>0;i--){int vl=g[i-1]-d[i+1];d[i]=vl>0?vl:0;}
return v-d[1]>=0;}

int chk(M*m,int e){
int f=m->f,t=m->t,p=ap(b[f]),d=e-t,s=0,i,a=ap(d);
if(p==KN){for(i=0;i<8;i++)if(d==ND[i])return 1;return 0;}
if(p==PN)return(d==sd*16-1||d==sd*16+1);
if(p==BI||p==QN){
if(a%15==0&&(a/15)==ap((e>>4)-(t>>4)))s=d>0?15:-15;
else if(a%17==0&&(a/17)==ap((e>>4)-(t>>4)))s=d>0?17:-17;}
if((p==RO||p==QN)&&!s){
if((t&7)==(e&7))s=d>0?16:-16;
else if((t>>4)==(e>>4))s=d>0?1:-1;}
if(s){int q=t+s;while(q!=e&&OK(q)&&(!b[q]||q==f))q+=s;return q==e;}
return 0;}

int gm(M*mv,int cp){
int n=0,s,p,a,i,f,tf,d,r,to,x,t2,t,g;
for(s=0;s<128;s++){if(s&0x88){s+=7;continue;}
p=b[s];if(!p||cl(p)!=sd)continue;a=ap(p);
if(a==PN){d=sd*16;r=sd>0?7:0;to=s+d;
if(OK(to)&&!b[to]){if(!cp){AM(s,to,0,(to>>4)==r?sd*QN:0);x=s+2*d;if((s&0x70)==(sd>0?0x10:0x60)&&OK(x)&&!b[x])AM(s,x,0,0);}else if((to>>4)==r)AM(s,to,0,sd*QN);}
for(x=-1;x<=1;x+=2){int tc=s+d+x;if(tc&0x88)continue;if(b[tc]&&cl(b[tc])!=sd&&ap(b[tc])!=KI)AM(s,tc,b[tc],(tc>>4)==r?sd*QN:0);else if(ep&&tc==ep)AM(s,tc,99,0);}
}else if(a==KN||a==KI){
for(i=0;i<8;i++){t2=s+(a==KN?ND[i]:KD[i]);if(t2&0x88)continue;t=b[t2];if((cp&&!t)||(t&&(cl(t)==sd||ap(t)==KI)))continue;AM(s,t2,t,0);}
if(!cp&&a==KI&&s==(sd>0?4:116)&&!at(s,-sd)){
if((cr&(sd>0?1:4))&&!b[s+1]&&!b[s+2]&&b[s+3]==sd*RO&&!at(s+1,-sd)&&!at(s+2,-sd))AM(s,s+2,0,0);
if((cr&(sd>0?2:8))&&!b[s-1]&&!b[s-2]&&!b[s-3]&&b[s-4]==sd*RO&&!at(s-1,-sd)&&!at(s-2,-sd))AM(s,s-2,0,0);}
}else{f=a==RO?4:0;tf=a==BI?4:8;
for(i=f;i<tf;i++)for(t=s+KD[i];OK(t);t+=KD[i]){g=b[t];if(g){if(cl(g)!=sd&&ap(g)!=KI)AM(s,t,g,0);break;}if(!cp)AM(s,t,0,0);}}}
return n;}

int ms(M*m,int y,int hf,int ht,M P){
int p=ap(b[m->f]),v,ek;
if(m->f==hf&&m->t==ht)return 300000;
if(m->c){
int c=VC(m->c),o=(p==KI?0:V[p]);
if(c>=o)return 250000+c*10-o;
M tm=*m;if(see(&tm)){ek=fk(-sd);int ic=(ek>=0&&chk(m,ek));return(ic?240000:220000)+c*10-o;}
return 10000+c*10-o;}
if(m->p)return 200000+VC(m->c);
if(p==PN&&((m->t>>4)==(sd>0?6:1)))return 175000;
if(p==PN&&((m->t>>4)==(sd>0?5:2)))return 140000;
ek=fk(-sd);if(ek>=0&&chk(m,ek))return 160000;
if(y<32){if(K[y][0].f==m->f&&K[y][0].t==m->t)return 90000;if(K[y][1].f==m->f&&K[y][1].t==m->t)return 70000;}
if(P.f!=P.t&&CM[B(P.f)][B(P.t)].f==m->f&&CM[B(P.f)][B(P.t)].t==m->t)return 80000;
v=H[B(m->f)][B(m->t)];
if(p==PN){if(sd>0&&(m->t==51||m->t==52||m->t==50))v+=2000;if(sd<0&&(m->t==67||m->t==68||m->t==66))v+=2000;if(sd<0&&((m->t==68&&b[52]==PN)||(m->t==67&&b[51]==PN)))v+=1000;if(sd>0&&((m->t==52&&b[68]==-PN)||(m->t==51&&b[67]==-PN)))v+=1000;}
if(p>PN){
int d=sd*16,t=m->t;
if((OK(t+d-1)&&b[t+d-1]==-sd*PN)||(OK(t+d+1)&&b[t+d+1]==-sd*PN))v-=15000;
int u=(sd>0?m->f>>4:7-(m->f>>4)),x=m->f&7;if(x>3)x=7-x;
int w=(sd>0?m->t>>4:7-(m->t>>4)),z=m->t&7;if(z>3)z=7-z;
v+=(T[p][(w<<2)|z]-T[p][(u<<2)|x])*6;
}
return v>40000?40000:(v<-40000?-40000:v);}

int ev(void){
int v=0,s,p,a,c,m=0,eg=0,k=0,w=0,wk=fk(1),bk=fk(-1),wt=0,bt=0;
int wp=0,bp=0,wpc=0,bpc=0,wr=0,br=0,wb=0,bb=0,pc,f,r;
for(s=0;s<128;s++){if(s&0x88){s+=7;continue;}
p=b[s];if(!p)continue;a=ap(p);c=cl(p);f=s&7;r=s>>4;
v+=c*V[a];m+=a!=PN&&a!=KI;if(a>PN&&a<KI)eg+=V[a];
if(a==PN){if(c>0){wp|=1<<f;wpc++;}else{bp|=1<<f;bpc++;}}
else if(a==RO){if(c>0)wr|=1<<f;else br|=1<<f;}
else if(a==BI){if(c>0)wb++;else bb++;}
if(a!=KI){pc=(c>0?r:7-r);if(f>3)f=7-f;v+=c*T[a][(pc<<2)|f];}
else{if(c>0){int z=s&7;if(z>3)z=7-z;v+=T[KI][(r<<2)|z];}
else{int z=s&7;if(z>3)z=7-z;v-=T[KI][((7-r)<<2)|z];}}}
if(wb>=2)v+=35;if(bb>=2)v-=35;
for(f=0;f<8;f++){
if(wr&(1<<f)){if(!(wp&(1<<f)))v+=((bp&(1<<f))?15:30);}
if(br&(1<<f)){if(!(bp&(1<<f)))v-=((wp&(1<<f))?15:30);}}
for(f=0;f<8;f++){
int msk=1<<f;
if(wp&msk){if((wp&(msk>>1))||(wp&(msk<<1)))v+=12;if(wpc>0&&!(wp&~msk))v-=10;}
if(bp&msk){if((bp&(msk>>1))||(bp&(msk<<1)))v-=12;if(bpc>0&&!(bp&~msk))v+=10;}}
if(wk>=0){int r=wk>>4,f=wk&7,kd=0;for(s=-1;s<=1;s+=2){int ts=wk+s;if(OK(ts)&&b[ts]==PN)kd++;}if(r<=1&&f>=5&&b[0x16]==PN&&b[0x17]==PN)kd+=2;if(r<=1&&f<=2&&b[0x11]==PN&&b[0x12]==PN)kd+=2;v+=kd*12;}
if(bk>=0){int r=bk>>4,f=bk&7,kd=0;for(s=-1;s<=1;s+=2){int ts=bk+s;if(OK(ts)&&b[ts]==-PN)kd++;}if(r>=6&&f>=5&&b[0x66]==-PN&&b[0x67]==-PN)kd+=2;if(r>=6&&f<=2&&b[0x61]==-PN&&b[0x62]==-PN)kd+=2;v-=kd*12;}
if(wk>=0&&bk>=0&&m<=3){int d=ap((wk>>4)-(bk>>4))+ap((wk&7)-(bk&7));
if(wpc>bpc){int r=7-(bk>>4),f=bk&7;if(f>3)f=7-f;v+=(r+f)*10-d*5;}
else if(bpc>wpc){int r=wk>>4,f=wk&7;if(f>3)f=7-f;v-=(r+f)*10-d*5;}}
if(eg<1400){int ph=eg*256/1400,egv=v;
if(wpc==0&&bpc==0&&ap(v)<400)egv/=4;
if(wpc>0&&bpc==0&&v>0){int psq=0;for(s=0;s<128;s++){if(s&0x88){s+=7;continue;}if(b[s]==PN)psq=s;}if(psq){int pr=psq>>4,pf=psq&7;egv+=pr*20;if(wk>=0){int kd=ap((wk>>4)-pr)+ap((wk&7)-pf);egv-=kd*8;}}}
if(bpc>0&&wpc==0&&v<0){int psq=0;for(s=0;s<128;s++){if(s&0x88){s+=7;continue;}if(b[s]==-PN)psq=s;}if(psq){int pr=7-(psq>>4),pf=psq&7;egv-=pr*20;if(bk>=0){int kd=ap((bk>>4)-(psq>>4))+ap((bk&7)-pf);egv+=kd*8;}}}
v=(v*ph+egv*(256-ph))/256;}
return sd*v;}

int rp(void){int i,c=0,st=hp-1,e=hp>100?hp-100:0;if(hp>0&&ir[hp-1]>e)e=ir[hp-1];for(i=st;i>=e;i-=2)if(hs[i]==h&&++c>=2)return 1;return 0;}

#define ADJ(s) ((s)>INF-100?(s)+y:(s)<-INF+100?(s)-y:(s))
#define UNADJ(s) ((s)>INF-100?(s)-y:(s)<-INF+100?(s)+y:(s))

void stt(TT*re,int s,char fl,char f,char t,char d){
if(re->k!=h||d>=re->d||fl==1){re->k=h;re->sc=s;re->fl=fl;re->d=d;re->f=f;re->t=t;re->gn=gn;}}

int qs(int a,int w,int y){
int v,n,i,s,k,sc[MM];M mv[MM];nc++;
if((nc&2047)==0&&tm<10000000&&(int)(GetTickCount()-t0)>=tm)ab=1;
if(ab)return a;
if(y>=MD)return ev();
k=fk(sd);if(k>=0&&at(k,-sd))return-INF+y;
v=ev();if(v>=w)return w;if(v>a)a=v;
n=gm(mv,1);for(i=0;i<n;i++)sc[i]=VC(mv[i].c)*10-(ap(b[mv[i].f])==KI?0:V[ap(b[mv[i].f])]);
for(i=0;i<n;i++){PIK(i,n);
if(mv[i].c!=99&&!mv[i].p&&v+VC(mv[i].c)+200<a)continue;
if(!see(&mv[i]))continue;
mk(&mv[i]);k=fk(sd);if(k<0||at(k,-sd)){um(&mv[i]);continue;}
sd=-sd;s=-qs(-w,-a,y+1);sd=-sd;um(&mv[i]);
if(ab)return a;if(s>=w)return w;if(s>a)a=s;}
return a;}

int sr(int d,int a,int w,int y,int nu,M P){
int n,i,s,O=a,v=-INF-1,g=0,sc[MM],k,hf=-1,ht=-1,R,e,I,bo,bf,bt,se=0,ek,ttm=0;
M mv[MM],bm=Z0;TT*re;nc++;
if((nc&2047)==0&&tm<10000000&&(int)(GetTickCount()-t0)>=tm)ab=1;
if(ab)return a;
if(y>0){if(rp())return 0;if(mn>=100)return 0;}
k=fk(sd);e=(k>=0&&at(k,-sd));if(e)d++;
if(d<=0)return qs(a,w,y);
if(y>=MD)return ev();
re=&tt[h&(TTSZ-1)];
if(re->k==h){
ttm=1;hf=re->f;ht=re->t;
if(re->d>=d&&!nu){int sc_adj=UNADJ(re->sc);
if(re->fl==1)return sc_adj;
if(re->fl==2&&sc_adj<=a)return a;
if(re->fl==3&&sc_adj>=w)return w;}}
se=ev();
if(!e&&d<=3&&!nu&&w<INF-1000&&se-150*d>=w)return w;
if(!e&&d>=3&&nu&&w<INF-1000&&se>=w){
int mcnt=0;for(i=0;i<128;i++){if(i&0x88){i+=7;continue;}if(b[i]&&cl(b[i])==sd&&ap(b[i])!=PN&&ap(b[i])!=KI)mcnt++;}
if(mcnt){sd=-sd;ep=0;h^=Z_side;s=-sr(d-1-2-d/4,-w,-w+1,y+1,0,Z0);sd=-sd;h^=Z_side;if(s>=w)return w;}}
n=gm(mv,0);
for(i=0;i<n;i++)sc[i]=ms(&mv[i],y,hf,ht,P);
for(i=0;i<n;i++){PIK(i,n);
mk(&mv[i]);k=fk(sd);if(k<0||at(k,-sd)){um(&mv[i]);continue;}
g++;sd=-sd;
if(!mv[i].c&&!mv[i].p&&!e&&d>=3&&g>3){
R=1+(g>8)+(d>5)+(!nu);
if(mv[i].f==hf&&mv[i].t==ht)R=0;
if(R>d-1)R=d-1;
s=-sr(d-1-R,-a-1,-a,y+1,1,mv[i]);
if(s>a)s=-sr(d-1,-w,-a,y+1,1,mv[i]);}
else if(!nu&&g>1){
s=-sr(d-1,-a-1,-a,y+1,1,mv[i]);
if(s>a&&s<w)s=-sr(d-1,-w,-a,y+1,1,mv[i]);}
else s=-sr(d-1,-w,-a,y+1,1,mv[i]);
sd=-sd;um(&mv[i]);
if(ab)return a;
if(s>v){v=s;if(s>a){a=s;bm=mv[i];}}
if(s>=w){if(!mv[i].c){bo=d*d;bf=B(mv[i].f);bt=B(mv[i].t);
if(y<32){if(K[y][0].f!=mv[i].f||K[y][0].t!=mv[i].t){K[y][1]=K[y][0];K[y][0]=mv[i];}}
if(P.f!=P.t)CM[B(P.f)][B(P.t)]=mv[i];
H[bf][bt]+=bo;if(H[bf][bt]>20000)H[bf][bt]=20000;
for(I=0;I<i;I++){if(!mv[I].c&&!mv[I].p){int kf=B(mv[I].f),kt=B(mv[I].t);H[kf][kt]-=bo;if(H[kf][kt]<-20000)H[kf][kt]=-20000;}}}
stt(re,ADJ(v),3,mv[i].f,mv[i].t,d);return v;}}
if(!g){k=fk(sd);return(k>=0&&at(k,-sd))?-INF+y:0;}
stt(re,ADJ(a),(a>O)?1:2,(a>O)?bm.f:hf,(a>O)?bm.t:ht,d);return a;}

int bt(M*o,int md){
int n,O=0,v=-INF-1,f,t,e,i,j,r,u,a,w,x,y,sc[MM],k,s,l,ns,p,q,le=0;
M mv[MM],z;TT*re;
t0=GetTickCount();ab=0;nc=0;gn++;
for(i=0;i<64;i++)for(j=0;j<64;j++)H[i][j]>>=1;memset(K,0,sizeof K);
n=gm(mv,0);re=&tt[h&(TTSZ-1)];
f=(re->k==h)?re->f:-1;t=(re->k==h)?re->t:-1;
for(i=0;i<n;i++)if(lg(&mv[i])){if(!O){*o=mv[i];O=1;}if(mv[i].f==f&&mv[i].t==t){*o=mv[i];break;}}
for(e=1;e<=md&&e<=MD;e++){if(ab)break;
l=(int)(GetTickCount()-t0);if(l>=tm)break;
if(e>=3&&tm<10000000){int d=l-le,fa=(O&&(z.f!=o->f||z.t!=o->t||ap(r-v)>40))?1:2;
if(l+d*fa>tm||l>tm*55/100)break;}
le=l;cd=e;
x=(O&&e>=4)?v-65:-INF;y=(O&&e>=4)?v+65:INF;
for(;;){a=x;w=y;r=-INF-1;u=0;
for(j=0;j<n;j++)sc[j]=ms(&mv[j],0,o->f,o->t,Z0);
for(i=0;i<n;i++){PIK(i,n);
mk(&mv[i]);k=fk(sd);if(k<0||at(k,-sd)){um(&mv[i]);continue;}
sd=-sd;
if(!u)s=-sr(e-1,-w,-a,1,1,mv[i]);
else{s=-sr(e-1,-a-1,-a,1,1,mv[i]);if(s>a&&s<w)s=-sr(e-1,-w,-a,1,1,mv[i]);}
sd=-sd;um(&mv[i]);
if(ab)break;
if(!u||s>a){r=s;z=mv[i];u=1;if(s>a)a=s;}
}
if(ab)break;
if(r<=x&&x>-INF){x=-INF;continue;}
if(r>=y&&y<INF){y=INF;continue;}
break;}
if(!ab&&u){char pv[16];v=r;*o=z;O=1;l=(int)(GetTickCount()-t0);
ms2(o,pv);ns=l>0?(int)((long long)nc*1000/l):0;
p=v>INF-1000?1:v<-INF+1000?-1:0;q=p?(INF-ap(v)+1)/2*p:v;
printf("info depth %d score %s %d time %d nodes %d nps %d pv %s\n",e,p?"mate":"cp",q,l,nc,ns,pv);
fflush(stdout);stt(re,v,1,o->f,o->t,e);}
if(v>INF-1000||v<-INF+1000)break;}
if(O&&!lg(o))for(i=0;i<n;i++)if(lg(&mv[i])){*o=mv[i];break;}
return O;}

int pl(char*s){
int f=((s[1]-'1')<<4)|(s[0]-'a'),to=((s[3]-'1')<<4)|(s[2]-'a'),i,n,k;
char p=s[4]|32;M mv[MM];n=gm(mv,0);
for(i=0;i<n;i++){if(mv[i].f!=f||mv[i].t!=to)continue;
if(s[4]&&ap(b[f])==PN)mv[i].p=sd*(p=='n'?KN:p=='b'?BI:p=='r'?RO:QN);
mk(&mv[i]);k=fk(sd);if(k>=0&&at(k,-sd)){um(&mv[i]);return 0;}sd=-sd;mn++;return 1;}
return 0;}

EXPORT void process_command(const char* cmd){
char L[4096];int l;char*m;char*p;
if(!cmd)return;
strncpy(L,cmd,4095);L[4095]=0;
l=strlen(L);
while(l>0&&L[l-1]<32)L[--l]=0;
if(!L[0])return;
if(!strcmp(L,"uci")){puts("id name Mite\nid author Viraj Ravani\nuciok");fflush(stdout);}
else if(!strcmp(L,"isready")){puts("readyok");fflush(stdout);}
else if(!strcmp(L,"ucinewgame"))ni();
else if(!strncmp(L,"pos",3)){m=strstr(L,"moves");
if(strstr(L,"start"))ni();else{char*f=strstr(L,"fen");if(f)sf(f+4);}
if(m){m+=5;while(*m){char v[8];int i=0;while(*m==' ')m++;if(!*m)break;
while(*m&&*m!=' '&&i<7)v[i++]=*m++;v[i]=0;if(i>=4)pl(v);}}}
else if(!strncmp(L,"go",2)){int d=MD;tm=2600;
if((p=strstr(L,"depth"))){sscanf(p+5,"%d",&d);tm=2000000000;}
if((p=strstr(L,"movetime"))){sscanf(p+8,"%d",&tm);tm=tm*92/100;}
else if((p=strstr(L,sd>0?"wtime":"btime"))){
int t=0,c=0,g=0;char*q;sscanf(p+5,"%d",&t);
if((q=strstr(L,sd>0?"winc":"binc")))sscanf(q+4,"%d",&c);
if((q=strstr(L,"movestogo")))sscanf(q+9,"%d",&g);
if(g>0)tm=t/(g+2)+c*3/4;
else if(t>30000)tm=t/36+c*3/4;
else if(t>10000)tm=t/32+c*6/10;
else if(t>4000)tm=t/30+c/2;
else if(t>1500)tm=t/25+c*4/10;
else if(t>600)tm=t/20+c*3/10;
else tm=c>50?c*3/10:15;
if(mn<=10&&tm>50)tm=tm*70/100;
if(tm>t*25/100)tm=t*25/100;
if(t>300&&tm>t-250)tm=t-250;
if(t<=300)tm=c>50?c*25/100:10;
if(tm<1)tm=1;}
if(tm<1)tm=1;
{M o;if(bt(&o,d)){char s[6];ms2(&o,s);printf("bestmove %s\n",s);}
else puts("bestmove (none)");fflush(stdout);}}
else if(!strcmp(L,"stop"))ab=1;}

int main(void){
setbuf(stdout,NULL);ni();
#ifndef __EMSCRIPTEN__
char L[4096];
while(fgets(L,4096,stdin)){
if(!strcmp(L,"quit\n")||!strcmp(L,"quit"))break;
process_command(L);}
#endif
return 0;}
