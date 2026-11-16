#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define TS 1
#define TN 2
#define TI 3
#define TO 4
#define TE 5
#define MT 4096
#define ML 256
#define MV 256
typedef struct{
int ty;
char tx[ML];
int nm;
int ln;
}tk;
typedef struct{
char nm[ML];
int ty;
int nv;
char sv[ML];
}vr;
typedef struct{
char *sr;
long sz;
tk ls[MT];
int tc;
int pc;
int er;
int vc;
vr vs[MV];
int br;
int dp;
}st;
st gl;
void er(int ln,const char *ms)
{
if(gl.er==0)
{
if(ln>0)
{
fprintf(stderr,"\033[31mЖанцеоды не довольны тобой, строка %d: %s!!!\033[0m\n",ln,ms);
}
else
{
fprintf(stderr,"\033[31mЖанцеоды не довольны тобой, %s!!!\033[0m\n",ms);
}
}
gl.er=1;
}
int in(void)
{
gl.sr=NULL;
gl.sz=0;
gl.tc=0;
gl.pc=0;
gl.er=0;
gl.vc=0;
gl.br=0;
gl.dp=0;
return 0;
}
int cl(void)
{
free(gl.sr);
gl.sr=NULL;
return 0;
}
int ld(const char *pa)
{
FILE *fp=NULL;
long sz=0;
fp=fopen(pa,"rb");
if(fp==NULL)
{
er(0,"такого файла нет");
return 1;
}
fseek(fp,0,SEEK_END);
sz=ftell(fp);
fseek(fp,0,SEEK_SET);
if(sz<0)
{
fclose(fp);
er(0,"не получилось прочитать файл");
return 1;
}
gl.sr=malloc((size_t)sz+1);
if(gl.sr==NULL)
{
fclose(fp);
er(0,"не хватило памяти");
return 1;
}
if(sz>0&&fread(gl.sr,1,(size_t)sz,fp)!=(size_t)sz)
{
fclose(fp);
er(0,"не получилось прочитать файл");
return 1;
}
gl.sr[sz]=0;
gl.sz=sz;
fclose(fp);
return 0;
}
int ad(int ty,const char *tx,int nm,int ln)
{
tk *tp=NULL;
if(gl.tc>=MT)
{
er(ln,"слишком много слов в файле");
return 1;
}
tp=&gl.ls[gl.tc];
tp->ty=ty;
tp->nm=nm;
tp->ln=ln;
strncpy(tp->tx,tx,ML-1);
tp->tx[ML-1]=0;
gl.tc++;
return 0;
}
int lx(void)
{
int ii=0;
int jj=0;
int ln=1;
int cc=0;
int nm=0;
char bf[ML];
while(ii<gl.sz&&gl.er==0)
{
cc=(unsigned char)gl.sr[ii];
if(cc=='\n')
{
ln++;
ii++;
continue;
}
if(isspace(cc))
{
ii++;
continue;
}
if(cc=='#')
{
while(ii<gl.sz&&gl.sr[ii]!='\n')
{
ii++;
}
continue;
}
if(cc=='/'&&ii+1<gl.sz&&gl.sr[ii+1]=='/')
{
while(ii<gl.sz&&gl.sr[ii]!='\n')
{
ii++;
}
continue;
}
if(cc=='"')
{
ii++;
jj=0;
while(ii<gl.sz&&gl.sr[ii]!='"')
{
if(gl.sr[ii]=='\n')
{
er(ln,"строка не закрыта");
return 1;
}
if(jj>=ML-1)
{
er(ln,"строка слишком длинная");
return 1;
}
if(gl.sr[ii]=='\\'&&ii+1<gl.sz)
{
ii++;
if(gl.sr[ii]=='n')
{
bf[jj++]='\n';
}
else if(gl.sr[ii]=='t')
{
bf[jj++]='\t';
}
else if(gl.sr[ii]=='\\')
{
bf[jj++]='\\';
}
else if(gl.sr[ii]=='"')
{
bf[jj++]='"';
}
else
{
bf[jj++]=gl.sr[ii];
}
ii++;
}
else
{
bf[jj++]=gl.sr[ii];
ii++;
}
}
if(ii>=gl.sz)
{
er(ln,"строка не закрыта");
return 1;
}
ii++;
bf[jj]=0;
if(ad(TS,bf,0,ln)!=0)
{
return 1;
}
continue;
}
if(isdigit(cc))
{
jj=0;
nm=0;
while(ii<gl.sz&&isdigit((unsigned char)gl.sr[ii]))
{
if(jj>=ML-1)
{
er(ln,"число слишком длинное");
return 1;
}
bf[jj++]=gl.sr[ii];
nm=nm*10+(gl.sr[ii]-'0');
ii++;
}
bf[jj]=0;
if(ad(TN,bf,nm,ln)!=0)
{
return 1;
}
continue;
}
if(isalpha(cc)||cc=='_')
{
jj=0;
while(ii<gl.sz&&(isalnum((unsigned char)gl.sr[ii])||gl.sr[ii]=='_'))
{
if(jj>=ML-1)
{
er(ln,"название слишком длинное");
return 1;
}
bf[jj++]=gl.sr[ii];
ii++;
}
bf[jj]=0;
if(ad(TI,bf,0,ln)!=0)
{
return 1;
}
continue;
}
if(strchr("=!<>",cc)!=NULL)
{
bf[0]=(char)cc;
bf[1]=0;
if(ii+1<gl.sz&&gl.sr[ii+1]=='=')
{
bf[1]='=';
bf[2]=0;
ii++;
}
if(ad(TO,bf,0,ln)!=0)
{
return 1;
}
ii++;
continue;
}
if(strchr("();{}+-*/%,",cc)!=NULL)
{
bf[0]=(char)cc;
bf[1]=0;
if(ad(TO,bf,0,ln)!=0)
{
return 1;
}
ii++;
continue;
}
er(ln,"непонятный символ");
return 1;
}
bf[0]=0;
if(ad(TE,bf,0,ln)!=0)
{
return 1;
}
return 0;
}
tk *nw(void)
{
if(gl.pc<gl.tc)
{
return &gl.ls[gl.pc];
}
return &gl.ls[gl.tc-1];
}
int fv(const char *nm)
{
int ii=0;
while(ii<gl.vc)
{
if(strcmp(gl.vs[ii].nm,nm)==0)
{
return ii;
}
ii++;
}
return -1;
}
int sv(const char *nm,int ty,int nv,const char *tx,int ln)
{
int ii=0;
ii=fv(nm);
if(ii<0)
{
if(gl.vc>=MV)
{
er(ln,"слишком много переменных");
return 1;
}
ii=gl.vc;
gl.vc++;
strncpy(gl.vs[ii].nm,nm,ML-1);
gl.vs[ii].nm[ML-1]=0;
}
gl.vs[ii].ty=ty;
gl.vs[ii].nv=nv;
strncpy(gl.vs[ii].sv,tx,ML-1);
gl.vs[ii].sv[ML-1]=0;
return 0;
}
int zn(const char *tx)
{
if(strncmp(tx,"zhan",4)!=0)
{
return 0;
}
if(tx[4]==0)
{
return 0;
}
return 1;
}
int ex(void);
int fc(void)
{
tk *tp=NULL;
int vl=0;
int ii=0;
tp=nw();
if(gl.er)
{
return 0;
}
if(tp->ty==TN)
{
vl=tp->nm;
gl.pc++;
return vl;
}
if(tp->ty==TI)
{
ii=fv(tp->tx);
if(ii<0)
{
er(tp->ln,"переменная не создана");
return 0;
}
if(gl.vs[ii].ty!=TN)
{
er(tp->ln,"строка не может быть числом");
return 0;
}
vl=gl.vs[ii].nv;
gl.pc++;
return vl;
}
if(tp->ty==TO&&strcmp(tp->tx,"-")==0)
{
gl.pc++;
return -fc();
}
if(tp->ty==TO&&strcmp(tp->tx,"(")==0)
{
gl.pc++;
vl=ex();
tp=nw();
if(tp->ty!=TO||strcmp(tp->tx,")")!=0)
{
er(tp->ln,"нет закрывающей скобки");
return 0;
}
gl.pc++;
return vl;
}
er(tp->ln,"нет числа");
return 0;
}
int tm(void)
{
tk *tp=NULL;
int vl=0;
int rh=0;
vl=fc();
while(gl.er==0)
{
tp=nw();
if(tp->ty!=TO)
{
break;
}
if(strcmp(tp->tx,"*")!=0&&strcmp(tp->tx,"/")!=0&&strcmp(tp->tx,"%")!=0)
{
break;
}
gl.pc++;
rh=fc();
if(strcmp(tp->tx,"*")==0)
{
vl=vl*rh;
}
else if(strcmp(tp->tx,"/")==0)
{
if(rh==0)
{
er(tp->ln,"делить на ноль нельзя");
return 0;
}
vl=vl/rh;
}
else
{
if(rh==0)
{
er(tp->ln,"делить на ноль нельзя");
return 0;
}
vl=vl%rh;
}
}
return vl;
}
int ex(void)
{
tk *tp=NULL;
int vl=0;
int rh=0;
vl=tm();
while(gl.er==0)
{
tp=nw();
if(tp->ty!=TO)
{
break;
}
if(strcmp(tp->tx,"+")!=0&&strcmp(tp->tx,"-")!=0)
{
break;
}
gl.pc++;
rh=tm();
if(strcmp(tp->tx,"+")==0)
{
vl=vl+rh;
}
else
{
vl=vl-rh;
}
}
return vl;
}
int ck(const char *tx)
{
tk *tp=NULL;
char bf[ML];
tp=nw();
if(tp->ty==TO&&strcmp(tp->tx,tx)==0)
{
gl.pc++;
return 1;
}
snprintf(bf,ML,"нужен знак %s",tx);
er(tp->ln,bf);
return 0;
}
int co(void)
{
tk *tp=NULL;
int lf=0;
int rh=0;
lf=ex();
if(gl.er)
{
return 0;
}
tp=nw();
if(tp->ty!=TO)
{
return lf!=0;
}
if(strcmp(tp->tx,"==")!=0&&strcmp(tp->tx,"!=")!=0&&strcmp(tp->tx,"<")!=0&&strcmp(tp->tx,"<=")!=0&&strcmp(tp->tx,">")!=0&&strcmp(tp->tx,">=")!=0)
{
return lf!=0;
}
gl.pc++;
rh=ex();
if(gl.er)
{
return 0;
}
if(strcmp(tp->tx,"==")==0)
{
return lf==rh;
}
if(strcmp(tp->tx,"!=")==0)
{
return lf!=rh;
}
if(strcmp(tp->tx,"<")==0)
{
return lf<rh;
}
if(strcmp(tp->tx,"<=")==0)
{
return lf<=rh;
}
if(strcmp(tp->tx,">")==0)
{
return lf>rh;
}
return lf>=rh;
}
int sk(void)
{
tk *tp=NULL;
int dd=1;
while(gl.er==0)
{
tp=nw();
if(tp->ty==TE)
{
er(tp->ln,"нет закрывающей фигурной скобки");
return 1;
}
if(tp->ty==TO&&strcmp(tp->tx,"{")==0)
{
dd++;
}
if(tp->ty==TO&&strcmp(tp->tx,"}")==0)
{
dd--;
gl.pc++;
if(dd==0)
{
return 0;
}
continue;
}
gl.pc++;
}
return 1;
}
int pr(void)
{
tk *tp=NULL;
int vl=0;
int ii=0;
if(ck("(")==0)
{
return 1;
}
tp=nw();
if(tp->ty==TS)
{
printf("%s\n",tp->tx);
gl.pc++;
}
else if(tp->ty==TI&&(ii=fv(tp->tx))>=0&&gl.vs[ii].ty==TS)
{
printf("%s\n",gl.vs[ii].sv);
gl.pc++;
}
else
{
vl=ex();
if(gl.er)
{
return 1;
}
printf("%d\n",vl);
}
if(ck(")")==0)
{
return 1;
}
if(ck(";")==0)
{
return 1;
}
return 0;
}
int va(void)
{
tk *tp=NULL;
char nm[ML];
int vl=0;
int ii=0;
tp=nw();
if(tp->ty!=TI)
{
er(tp->ln,"после zhanvar нужно название переменной");
return 1;
}
if(zn(tp->tx)==0)
{
er(tp->ln,"название переменной должно начинаться с zhan");
return 1;
}
strncpy(nm,tp->tx,ML-1);
nm[ML-1]=0;
gl.pc++;
if(ck("=")==0)
{
return 1;
}
tp=nw();
if(tp->ty==TS)
{
if(sv(nm,TS,0,tp->tx,tp->ln)!=0)
{
return 1;
}
gl.pc++;
}
else if(tp->ty==TI&&(ii=fv(tp->tx))>=0&&gl.vs[ii].ty==TS)
{
if(sv(nm,TS,0,gl.vs[ii].sv,tp->ln)!=0)
{
return 1;
}
gl.pc++;
}
else
{
vl=ex();
if(gl.er)
{
return 1;
}
if(sv(nm,TN,vl,"",tp->ln)!=0)
{
return 1;
}
}
if(ck(";")==0)
{
return 1;
}
return 0;
}
int nu(const char *tx)
{
int ii=0;
if(tx[0]=='-')
{
ii++;
}
if(tx[ii]==0)
{
return 0;
}
while(tx[ii]!=0)
{
if(isdigit((unsigned char)tx[ii])==0)
{
return 0;
}
ii++;
}
return 1;
}
int ip(void)
{
tk *tp=NULL;
char nm[ML];
char bf[ML];
int vl=0;
if(ck("(")==0)
{
return 1;
}
tp=nw();
if(tp->ty!=TI)
{
er(tp->ln,"в zhaninput нужно название переменной");
return 1;
}
if(zn(tp->tx)==0)
{
er(tp->ln,"название переменной должно начинаться с zhan");
return 1;
}
strncpy(nm,tp->tx,ML-1);
nm[ML-1]=0;
gl.pc++;
tp=nw();
if(tp->ty==TO&&strcmp(tp->tx,",")==0)
{
gl.pc++;
tp=nw();
if(tp->ty!=TS)
{
er(tp->ln,"после запятой нужна строка");
return 1;
}
printf("%s",tp->tx);
fflush(stdout);
gl.pc++;
}
if(ck(")")==0)
{
return 1;
}
if(ck(";")==0)
{
return 1;
}
if(fgets(bf,ML,stdin)==NULL)
{
er(0,"не получилось прочитать ввод");
return 1;
}
bf[strcspn(bf,"\n")]=0;
if(nu(bf))
{
vl=atoi(bf);
return sv(nm,TN,vl,"",tp->ln);
}
return sv(nm,TS,0,bf,tp->ln);
}
int xb(int md);
int zf(void)
{
tk *tp=NULL;
int vl=0;
if(ck("(")==0)
{
return 1;
}
vl=co();
if(gl.er)
{
return 1;
}
if(ck(")")==0)
{
return 1;
}
if(ck("{")==0)
{
return 1;
}
if(vl)
{
if(xb(1)!=0)
{
return 1;
}
if(gl.br)
{
return sk();
}
tp=nw();
if(tp->ty==TI&&strcmp(tp->tx,"zhanelse")==0)
{
gl.pc++;
if(ck("{")==0)
{
return 1;
}
return sk();
}
return 0;
}
if(sk()!=0)
{
return 1;
}
tp=nw();
if(tp->ty==TI&&strcmp(tp->tx,"zhanelse")==0)
{
gl.pc++;
if(ck("{")==0)
{
return 1;
}
return xb(1);
}
return 0;
}
int zw(void)
{
int ps=0;
int vl=0;
int rt=0;
ps=gl.pc;
gl.dp++;
while(gl.er==0)
{
gl.pc=ps;
if(ck("(")==0)
{
rt=1;
break;
}
vl=co();
if(gl.er)
{
rt=1;
break;
}
if(ck(")")==0)
{
rt=1;
break;
}
if(ck("{")==0)
{
rt=1;
break;
}
if(vl==0)
{
rt=sk();
break;
}
if(xb(1)!=0)
{
rt=1;
break;
}
if(gl.br)
{
gl.br=0;
rt=sk();
break;
}
}
gl.dp--;
return rt;
}
int zb(void)
{
tk *tp=NULL;
if(gl.dp<=0)
{
tp=nw();
er(tp->ln,"zhanbreak можно писать только внутри zhanwhile дурак дебил");
return 1;
}
if(ck(";")==0)
{
return 1;
}
gl.br=1;
return 0;
}
int xb(int md)
{
tk *tp=NULL;
while(gl.er==0)
{
tp=nw();
if(tp->ty==TE)
{
if(md)
{
er(tp->ln,"нет закрывающей фигурной скобки");
return 1;
}
return 0;
}
if(tp->ty==TO&&strcmp(tp->tx,"}")==0)
{
if(md)
{
gl.pc++;
return 0;
}
er(tp->ln,"лишняя фигурная скобка");
return 1;
}
if(tp->ty!=TI)
{
er(tp->ln,"тут нужна команда, начинающаяся с zhan");
return 1;
}
gl.pc++;
if(strcmp(tp->tx,"zhanprint")==0)
{
if(pr()!=0)
{
return 1;
}
}
else if(strcmp(tp->tx,"zhanvar")==0)
{
if(va()!=0)
{
return 1;
}
}
else if(strcmp(tp->tx,"zhaninput")==0)
{
if(ip()!=0)
{
return 1;
}
}
else if(strcmp(tp->tx,"zhanif")==0)
{
if(zf()!=0)
{
return 1;
}
}
else if(strcmp(tp->tx,"zhanwhile")==0)
{
if(zw()!=0)
{
return 1;
}
}
else if(strcmp(tp->tx,"zhanbreak")==0)
{
if(zb()!=0)
{
return 1;
}
}
else if(strcmp(tp->tx,"zhanelse")==0)
{
er(tp->ln,"zhanelse нельзя писать без zhanif");
return 1;
}
else
{
er(tp->ln,"такой команды нет, все команды начинаются с zhan");
return 1;
}
if(gl.br)
{
return 0;
}
}
return 1;
}
int rn(const char *pa)
{
if(ld(pa)!=0)
{
return 1;
}
if(lx()!=0)
{
return 1;
}
if(xb(0)!=0)
{
return 1;
}
return 0;
}
int main(int ac,char **av)
{
if(in()!=0)
{
return 1;
}
if(ac!=2)
{
er(0,"после команды zhanode нужен файл!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
cl();
return 1;
}
if(rn(av[1])!=0)
{
cl();
return 1;
}
cl();
return 0;
}
