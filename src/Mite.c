#include<stdio.h>
#include<string.h>
#ifdef _WIN32
#include<windows.h>
#else
#include<sys/time.h>
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
int s=0,o[2][8]={0},v[2][8]={0},n[2]={0},t[2]={0},m[2]={0},pt[2]={0},nm[2]={0},W,q,i,f;
int p,a,c,co,r,P,J,x,k,u,y,d,e,U,Q,j,F,z,R,g,em,my,mb,G,np=0,df;
for(q=0;q<128;q++){if(q&0x88){q+=7;continue;}p=b[q];if(!p)continue;a=ap(p);c=p>0;
if(a==PN){o[c][q&7]++;pt[c]++;np=1;}else if(a==RO)v[c][q&7]++;else if(a==QN)n[c]=1;
if(a==KN||a==BI)nm[c]++;if(a>PN&&a<KI)m[c]+=V[a];}
wm=m[1];bx=m[0];W=m[0]+m[1];if(W>2200)s+=(nm[1]-nm[0])*85;
if(!np&&m[0]<=330&&m[1]<=330)return 0;
G=((!n[0]&&!n[1]&&W<3200)||(W<1800));
if(W>3000){int X=(b[1]==KN)+(b[6]==KN)+(b[2]==BI)+(b[5]==BI),Y=(b[113]==-KN)+(b[118]==-KN)+(b[114]==-BI)+(b[117]==-BI);
s-=(X-Y)*14;if(X>=1&&b[3]!=QN)s-=25;if(Y>=1&&b[115]!=-QN)s+=25;
if(b[51]==PN)s+=25;if(b[52]==PN)s+=25;if(b[50]==PN)s+=12;
if(b[67]==-PN)s-=25;if(b[68]==-PN)s-=25;if(b[66]==-PN)s-=12;
if(b[67]==PN)s+=28;if(b[68]==PN)s+=28;if(b[51]==-PN)s-=28;if(b[52]==-PN)s-=28;
if(b[51]==PN&&b[52]==PN)s+=20;if(b[67]==-PN&&b[68]==-PN)s-=20;
if(b[0x23]==BI&&b[0x13]==PN)s-=30;if(b[0x53]==-BI&&b[0x63]==-PN)s+=30;
if(b[2]==BI&&b[0x24]==PN&&b[51]==PN)s-=25;if(b[114]==-BI&&b[0x54]==-PN&&b[67]==-PN)s+=25;}
for(q=0;q<128;q++){if(q&0x88){q+=7;continue;}p=b[q];if(!p)continue;
c=cl(p);a=ap(p);f=q&7;co=f>3?7-f:f;r=c>0?q>>4:7-(q>>4);
P=T[a][(r<<2)|co];
if(a==KI&&G)P=(14-ap(2*(q>>4)-7)-ap(2*(q&7)-7))*(W<800?11:7);
s+=c*(V[a]+P);
if(a==PN){J=1;
for(x=-1;x<=1;x++){int l=f+x;if(l>=0&&l<8){for(k=1;k<7;k++){u=(q>>4)+c*k;if(u>0&&u<7&&b[(u<<4)|l]==-c*PN){J=0;break;}}}}
if(J){s+=c*(pb[r]+(W<2400?pb[r]>>1:0));
u=q+c*16;if(OK(u)&&b[u]&&cl(b[u])!=c)s-=c*50;
if(r==6&&OK(u)&&!b[u])s+=c*(W<2000?100:60);
if(r==5&&OK(u)&&!b[u])s+=c*(W<2000?50:30);
if(r==4&&OK(u)&&!b[u])s+=c*15;
if(v[c>0][f])s+=c*25;if(v[c<=0][f])s-=c*20;
if((OK(q-c*16-1)&&b[q-c*16-1]==c*PN)||(OK(q-c*16+1)&&b[q-c*16+1]==c*PN))s+=c*30;
if((f>0&&o[c>0][f-1])||(f<7&&o[c>0][f+1]))s+=c*35;
if(W<3200){y=fk(c);e=fk(-c);
if(y>=0){d=ap((y>>4)-(q>>4))+ap((y&7)-(q&7));s+=c*(8-d)*8;}
if(e>=0){d=ap((e>>4)-(q>>4))+ap((e&7)-(q&7));s+=c*(d<=2?-90:(d>=5?(d-3)*14:(d-3)*6));}}}
else if(!o[c<=0][f])s+=c*(15+pb[r]/3+(W<2400?20:0));
}
if(a==KN||a==BI){U=(OK(q-c*16-1)&&b[q-c*16-1]==c*PN)||(OK(q-c*16+1)&&b[q-c*16+1]==c*PN);
if(r>=3&&U)s+=c*15;mb=0;
if(a==KN){for(k=0;k<8;k++){u=q+ND[k];if(OK(u)&&(!b[u]||cl(b[u])!=c))mb++;}
if(r>=3&&r<=5&&U){int f2=1;
for(x=-1;x<=1;x+=2){int l=f+x;if(l>=0&&l<8){for(k=r+1;k<7;k++){u=c>0?k:7-k;if(b[(u<<4)|l]==-c*PN){f2=0;break;}}}}
if(f2)s+=c*25;}}
else for(k=0;k<4;k++){int v2=KD[k];for(u=q+v2;OK(u);u+=v2){if(b[u]){if(cl(b[u])!=c)mb++;break;}mb++;}}
s+=c*(mb-4)*4;}
if(a>=KN&&a<=QN){
int pd=c>0?16:-16;
if((OK(q+pd-1)&&b[q+pd-1]==-c*PN)||(OK(q+pd+1)&&b[q+pd+1]==-c*PN))s-=c*(a==QN?250:a==RO?150:60);
if(a==QN){e=fk(-c);
if(e>=0){Q=ap((q>>4)-(e>>4))+ap((q&7)-(e&7));s+=c*(14-Q)*4;}
if(v[c<=0][f])s-=c*(o[c>0][f]?40:80);}
if(a==RO){if(!o[c>0][f])s+=c*(o[c<=0][f]?15:30);
if(r>=6){s+=c*35;e=fk(-c);if(e>=0&&(c>0?(e>>4)==7:(e>>4)==0))s+=c*30;}}}
if(a==BI)t[c>0]++;}
if(t[1]>1)s+=35;if(t[0]>1)s-=35;
for(f=0;f<8;f++){int yv;if(o[1][f]>1)s-=(o[1][f]-1)*20;if(o[0][f]>1)s+=(o[0][f]-1)*20;
x=(!f||!o[1][f-1])&&(f==7||!o[1][f+1]);yv=(!f||!o[0][f-1])&&(f==7||!o[0][f+1]);
if(o[1][f]&&x)s-=15;if(o[0][f]&&yv)s+=15;}
for(i=1;i>=-1;i-=2){y=fk(i);if(y<0)continue;
j=y&7;F=0;z=n[i<=0];R=0;
for(k=0;k<8;k++)if(v[i<=0][k])R=1;
int cs=(i>0&&(y==6||y==7||y==2||y==1))||(i<0&&(y==118||y==119||y==114||y==113));
int hd=z||(R&&W>3400);
if(z){
if(cs)s+=i*((y==(i>0?6:118)||y==(i>0?7:119))?45:30);
else if(!(cr&(i>0?3:12))){s-=i*50;if(hd&&!o[1][j]&&!o[0][j])s-=i*25;}
if(!cs&&hd){if(!o[i>0][j])s-=i*25;if(!o[1][j]&&!o[0][j])s-=i*30;}
}else if(cs&&W>2400)s+=i*15;
if(cs&&hd){
for(k=j-1;k<=j+1;k++){if(k<0||k>7)continue;
int q1=y+i*16+(k-j),q2=y+i*32+(k-j);
if(OK(q1)&&b[q1]==i*PN);
else if(OK(q2)&&b[q2]==i*PN)F+=15;
else F+=z?40:22;}
s-=i*F;
if(z&&R&&OK(y+i*16-1)&&b[y+i*16-1]==i*PN&&b[y+i*16]==i*PN&&b[y+i*16+1]==i*PN&&W<2200)s-=i*15;}
if(hd){
if(!o[1][j]&&!o[0][j])s-=i*(z?35:15);
if(v[i<=0][j])s-=i*(z?45:20);
for(x=-1;x<=1;x+=2)if(j+x>=0&&j+x<8&&v[i<=0][j+x])s-=i*(z?22:10);
g=0;for(x=-1;x<=1;x++)for(k=-16;k<=16;k+=16){if(x||k){u=y+x+k;if(OK(u)&&at(u,-i))g++;}}
if(g>=2){int kp=(g-1)*(z?15:6);s-=i*kp;}}
em=m[i<=0];my=m[i>0];
if(em<1300&&!n[i<=0]&&my>em+200){e=fk(-i);
if(e>=0){int ed=ap(2*(e>>4)-7)+ap(2*(e&7)-7);d=ap((y>>4)-(e>>4))+ap((y&7)-(e&7));s+=i*(ed*8+(14-d)*8);}
else{d=ap(2*(y>>4)-7)+ap(2*(y&7)-7);s+=i*(14-d)*8;}}}
df=(m[1]-m[0])+(pt[1]-pt[0])*100;
if(df>=90){s+=(df*(4000-m[0]))/14000;if(!n[0]&&!n[1])s+=25;}else if(df<=-90){s-=((-df)*(4000-m[1]))/14000;if(!n[0]&&!n[1])s-=25;}
if(W<3200){if(s>100)s+=(3200-W)*(s-100)/4000;else if(s<-100)s-=(3200-W)*(-s-100)/4000;}
if(W<3200&&ap(df)>=90){
int ww=df>0;y=fk(ww?1:-1);e=fk(ww?-1:1);
if(y>=0&&e>=0){
int ed=ap(2*(e>>4)-7)+ap(2*(e&7)-7);
int d=ap((y>>4)-(e>>4))+ap((y&7)-(e&7));
int mu=ap(df)>=300?12:6;
s+=(ww?1:-1)*(ed*mu+(14-d)*mu);
}}
int hm=hp-(hp>0?ir[hp-1]:0);
if(hm>=60){if(s>80)s-=(hm-60)*4;else if(s<-80)s+=(hm-60)*4;}
if(!pt[1]&&m[1]<=330&&s>0)s=0;
if(!pt[0]&&m[0]<=330&&s<0)s=0;
s+=sd*2;return s*sd;}
int sr(int d,int a,int v,int y,int u,M pm);
int qs(int a,int v,int y){
int k,c,t,n,j,i,s;M mv[MM];int sc[MM];
if(ab||y>=36)return a;
if((++nc&127)==0){int el=(int)(GetTickCount()-t0);if(el>=tm)ab=1;}
k=fk(sd);c=k>=0&&at(k,-sd);
if(c)return sr(1,a,v,y,0,Z0);
t=ev();if(t>=v)return v;if(t>a)a=t;
n=gm(mv,1);for(j=0;j<n;j++)sc[j]=ms(&mv[j],0,0,0,Z0);
for(i=0;i<n;i++){PIK(i,n);
if(!mv[i].p&&t+VC(mv[i].c)+200<a)continue;
if(!mv[i].p&&sc[i]<200000)continue;
mk(&mv[i]);k=fk(sd);if(k<0||at(k,-sd)){um(&mv[i]);continue;}
sd=-sd;s=-qs(-v,-a,y+1);sd=-sd;um(&mv[i]);
if(ab)return a;if(s>a){if(s>=v)return v;a=s;}}
return a;}
#define ADJ(s) (s>INF-1000?s+y:s<-INF+1000?s-y:s)
#define RET(s) (s>INF-1000?s-y:s<-INF+1000?s+y:s)
void stt(TT*e,int sc,int fl,int f,int t,int d){
if(!ab&&(e->gn!=gn||d>=e->d||(d>=e->d-1&&fl==1))){e->k=h;e->sc=sc;e->d=d;e->fl=fl;e->f=f;e->t=t;e->gn=gn;}}
int rep(void){int i;for(i=hp-4;i>=0&&i>=hp-100;i-=2)if(hs[i]==h)return 1;return 0;}
int sr(int d,int a,int v,int y,int u,M P){
int k,c,O,hf=0,ht=0,t,n,g=0,j,i,p,G,x,s,R,o,U,bo,bf,bt,I,vs;
TT*e;M mv[MM];int sc[MM];M bm;
memset(&bm,0,sizeof bm);
if(ab||y>=36)return ev();
if(y>0){
if((hp-(hp>0?ir[hp-1]:0))>=100)return 0;
if(rep()){int vl=ev();return vl>30?-40:vl<-30?40:0;}
if(a<-INF+y)a=-INF+y;if(v>INF-y-1)v=INF-y-1;if(a>=v)return a;}
if((++nc&127)==0){int el=(int)(GetTickCount()-t0);if(el>=tm)ab=1;
if((nc&16383)==0)printf("info depth %d nodes %d time %d nps %d\n",cd,nc,el,el>0?(int)((long long)nc*1000/el):0),fflush(stdout);}
k=fk(sd);c=k>=0&&at(k,-sd);
if(c&&d<16)d++;
if(!d)return qs(a,v,y);
O=a;e=&tt[h&(TTSZ-1)];
if(e->k==h){hf=e->f;ht=e->t;
if(e->d>=d&&y>0){vs=RET(e->sc);
if(e->fl==1)return vs;
if(e->fl==2&&vs<=a)return vs;
if(e->fl==3&&vs>=v)return vs;}}
if(d>=5&&!hf&&!ht)d--;
t=ev();
if(!c&&d<=3&&t-120*d>=v&&v<INF-1000&&t<INF-1000)return v;
if(!c&&d<=3&&t+280*d<a){int rz=qs(a,a+1,y);if(rz<=a)return rz;}
{int ym=0,om=0,yp=0,qk;
for(qk=0;qk<128;qk++){if(qk&0x88){qk+=7;continue;}
int pk=b[qk],ak=ap(pk);if(!pk)continue;
if(ak>PN&&ak<KI){om+=V[ak];if(cl(pk)==sd){ym+=V[ak];yp++;}}}
if(u&&!c&&d>=3&&yp>0&&ym>600&&om>1800){
R=d>6?3:2;o=ep;ep=0;if(o)h^=Z[13][16+(o&7)];
sd=-sd;h^=Z_side;U=-sr(d-1-R,-v,-v+1,y+1,0,Z0);sd=-sd;h^=Z_side;ep=o;if(o)h^=Z[13][16+(o&7)];
if(ab)return 0;if(U>=v&&U<INF-1000)return v;if(U<=-INF+1000&&d<15)d++;}}
n=gm(mv,0);for(j=0;j<n;j++)sc[j]=ms(&mv[j],y,hf,ht,P);
for(i=0;i<n;i++){PIK(i,n);
p=ap(b[mv[i].f]);G=(p>=KN&&at(mv[i].f,-sd));
if(!c&&!G&&d<=2&&!mv[i].c&&!mv[i].p&&sc[i]<70000&&g>=(10+4*d*d))continue;
if(g&&!c&&!mv[i].c&&!mv[i].p&&d<=3&&t+90*d<a)continue;
if(g&&!c&&d<=2&&mv[i].c&&VC(mv[i].c)<=330&&sc[i]<200000&&t+VC(mv[i].c)+160<a)continue;
x=(mv[i].p||(p==PN&&((mv[i].t>>4)==(sd>0?6:1))))?1:0;
mk(&mv[i]);k=fk(sd);if(k<0||at(k,-sd)){um(&mv[i]);continue;}g++;
if(g==1){sd=-sd;s=-sr(d-1+x,-v,-a,y+1,1,mv[i]);sd=-sd;
}else{R=0;
if(!c&&!G&&g>3&&!mv[i].c&&!mv[i].p&&sc[i]<70000&&d>=3){R=1;if(d>=5&&g>6)R=2;if(d>=8&&g>12)R=3;}
sd=-sd;s=-sr(d-1-R+x,-a-1,-a,y+1,1,mv[i]);sd=-sd;
if(s>a&&R){sd=-sd;s=-sr(d-1+x,-a-1,-a,y+1,1,mv[i]);sd=-sd;}
if(s>a&&s<v){sd=-sd;s=-sr(d-1+x,-v,-a,y+1,1,mv[i]);sd=-sd;}}
um(&mv[i]);if(ab)return 0;
if(s>=v){if(!mv[i].c){bo=d*d;bf=B(mv[i].f);bt=B(mv[i].t);
if(y<32){if(K[y][0].f!=mv[i].f||K[y][0].t!=mv[i].t){K[y][1]=K[y][0];K[y][0]=mv[i];}}
if(P.f!=P.t)CM[B(P.f)][B(P.t)]=mv[i];
H[bf][bt]+=bo;if(H[bf][bt]>20000)H[bf][bt]=20000;
for(I=0;I<i;I++){if(!mv[I].c&&!mv[I].p){int kf=B(mv[I].f),kt=B(mv[I].t);H[kf][kt]-=bo;if(H[kf][kt]<-20000)H[kf][kt]=-20000;}}}
stt(e,ADJ(v),3,mv[i].f,mv[i].t,d);return v;}
if(s>a){a=s;bm=mv[i];}}
if(!g){k=fk(sd);return(k>=0&&at(k,-sd))?-INF+y:0;}
stt(e,ADJ(a),(a>O)?1:2,(a>O)?bm.f:hf,(a>O)?bm.t:ht,d);return a;}
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
int main(void){
char L[4096];setbuf(stdout,NULL);ni();
while(fgets(L,4096,stdin)){int l=strlen(L);char*m;char*p;
while(l>0&&L[l-1]<32)L[--l]=0;if(!L[0])continue;
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
else if(!strcmp(L,"stop"))ab=1;
else if(!strcmp(L,"quit"))break;}
return 0;}
