#include<iostream>
#include<cstdio>
#include<queue>
#include<cstring>
#include<vector>
#include<windows.h>
using namespace std;
const int MAXN=10005;
int head[MAXN],num_edge;
struct Edge{
    int to,next;
}edge[MAXN*4];
void add_edge(int from,int to){
    edge[++num_edge].to=to;
    edge[num_edge].next=head[from];
    head[from]=num_edge;
}

vector<int> c[MAXN];
int dfn[MAXN],low[MAXN],stack[MAXN],col[MAXN],col_num,dfs_num,top;
bool vis[MAXN];
void Tarjan(int x){
    dfn[x]=low[x]=++dfs_num;
    vis[x]=1; stack[++top]=x;
    for(int i=head[x];i;i=edge[i].next){
        Edge e=edge[i];
        if(!dfn[e.to]){
            Tarjan(e.to);
            low[x]=min(low[x],low[e.to]);
        }else if(vis[e.to]){
            low[x]=min(low[x],dfn[e.to]);
        }
    }
    if(dfn[x]==low[x]){
        ++col_num; vis[x]=0;
        while(stack[top+1]!=x){
            col[stack[top]]=col_num;
			c[col_num].push_back(stack[top]);
            vis[stack[top--]]=0;
        }
    }
}
//---------Tarjan¡ü--------

int change(int x){return x%100*100+x/100;}
int P(int st,int t){
	int a=st/1000,b=(st/100)%10,c=(st/10)%10,d=st%10;
	if(t==1) if(c&&a) return a*1000+b*100+((c+a)%10)*10+d; else return -1;
	if(t==2) if(c&&b) return a*1000+b*100+((c+b)%10)*10+d; else return -1;
	if(t==3) if(d&&a) return a*1000+b*100+c*10+(d+a)%10; else return -1;
	if(t==4) if(d&&b) return a*1000+b*100+c*10+(d+b)%10; else return -1;
}
void build(){
	for(int i=1;i<=9999;i++)
		for(int t=1;t<=4;t++)
			if(P(i,t)!=-1) add_edge(i,change(P(i,t)));
}
void solve_tarjan(){
	for(int i=1;i<=9999;i++)
        if(!dfn[i]) Tarjan(i);
}
//-----------

int in[MAXN];
vector<int> G[MAXN];
bool con[1005][1005];
void build2(){
	for(int x=1;x<10000;x++){
		for(int i=head[x];i;i=edge[i].next){
			Edge e=edge[i];
			if(col[x]!=col[e.to]&&!con[col[x]][col[e.to]]){
				G[col[x]].push_back(col[e.to]);
				con[col[x]][col[e.to]]=true;
				//cout<<col[x]<<' '<<col[e.to]<<endl;
				in[col[e.to]]++;
			}
		}
	}
}
int dp[MAXN];//1 win£¬2 lose£¬3 draw
int f[MAXN];
void init_state(){
	for(int i=1;i<=99;i++)
		f[i]=dp[col[i]]=2;
}
int calc(int x){
	for(int i=head[x];i;i=edge[i].next)
		if(f[edge[i].to]==2) return 1;
	for(int i=head[x];i;i=edge[i].next)
		if(!f[edge[i].to]) return 0;
	for(int i=head[x];i;i=edge[i].next)
		if(f[edge[i].to]==3) return 3;
	return 2;
}
int dfs(int x){
	if(dp[x]) return dp[x];
	int sum=0,num[4]={};
	for(int i=0;i<G[x].size();i++){
		sum++; num[dfs(G[x][i])]++;}
	if(!sum||num[3]==sum){
		for(auto i:c[x]) f[i]=3; 
		return dp[x]=3;
	}


	int T=c[x].size();
	while(T--){
		for(auto i:c[x])
			if(!f[i]) f[i]=calc(i);
	}

	for(auto i:c[x]) if(!f[i]) f[i]=3;
	return dp[x]=1;
}
void solve_dp(){
	build2();
	init_state();
	for(int i=1;i<=col_num;i++)
		if(!in[i]) dfs(i);
}

void init(){
	build();
	solve_tarjan();
	solve_dp();
}
//----------------------

int check(int st){
	if(st/100==0) return 1;
	else if(st%100==0) return 2;
	else if(f[st]==3) return -1;
	else return 0;
}
void output(int st){
	system("cls");
	int a=st/1000,b=(st/100) %10,c=(st/10) %10,d=st%10;
	cout<<a<<' '<<b<<'\n'<<c<<' '<<d<<'\n';
}
int input(int st){
	int a=st/1000,b=(st/100) %10,c=(st/10) %10,d=st%10;
	int x,y;cin>>x>>y;
	if(c==x){c=(c+y) %10;}
	else{d=(d+y)%10;}
	return a*1000+b*100+c*10+d;
}
int solve(int st){
	for(auto x:{2,3,1})
		for(int i=1;i<=4;i++)
			if(P(st,i)!=-1&&f[change(P(st,i))]==x) return P(st,i);
}
void debug(int st){
	cout<<"\n------------------------------------\n";
	cout<<"now state:"<<st<<" "<<f[st]<<endl;
	for(int i=1;i<=4;i++)
		if(P(st,i)!=-1) cout<<change(P(st,i))<<':'<<f[change(P(st,i))]<<endl;
	cout<<"\n------------------------------------\n";
	system("pause");
}
int main(){
	init();
	
	int st=1111; output(st);
	while(check(st)!=1&&check(st)!=2){
		st=input(st); output(st);
  		if(check(change(st))==1||check(change(st))==2) break;// debug(change(st));
		Sleep(500);
	
  		st=change(solve(change(st)));
		output(st);
	}
	if(check(st) ==1) cout<<"You lose!"<<endl;
	else if(check(st) ==2) cout<<"You win!"<<endl;
	else cout<<"Draw!"<<endl;
	system("pause");
	return 0;
}

