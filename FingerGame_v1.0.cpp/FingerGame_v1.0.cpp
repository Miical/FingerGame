#include <iostream>
#include <windows.h>

const int MAXN = 10000;
int f[MAXN];
bool vis[MAXN];
int change(int x) { return x % 100 * 100 + x / 100; }
void debug(int);

bool judge(int st) {
    int a = st / 1000, b = (st / 100) % 10, c = (st / 10) % 10, d = st % 10;
    return (!!a) + (!!b) == 1 && (!!c) + (!!d) == 1;
}
int num[MAXN];
bool Enter[MAXN][4];
int dfs(int st) {
    if (f[st] != 0) return f[st];
    if (judge(st) && vis[st]) return 3;
    
    int a = st / 1000, b = (st / 100) % 10, c = (st / 10) % 10, d = st % 10;
    bool tag = true, ping = true,have_ping=false;
    int x;

	if (vis[st]){
		if(!num[st]) return 3;
	} else{
		vis[st] = true; num[st]=(c&&a)+(c&&b)+(d&&a)+(d&&b);
	}
    
    x = a * 1000 + b * 100 + ((c + a) % 10) * 10 + d;
    if (c && a&&!Enter[st][0]) { Enter[st][0]=true; int t = dfs(change(x)); num[st]--; if(t!=3) tag &= (t == 1); ping &= (t == 3);have_ping|=t==3;}
    x = a * 1000 + b * 100 + ((c + b) % 10) * 10 + d;
    if (c && b&&!Enter[st][1]) { Enter[st][1]=true; int t = dfs(change(x)); num[st]--; if(t!=3) tag &= (t == 1); ping &= (t == 3);have_ping|=t==3;}
    x = a * 1000 + b * 100 + c * 10 + (d + a) % 10;
    if (d && a&&!Enter[st][2]) { Enter[st][2]=true; int t = dfs(change(x)); num[st]--; if(t!=3) tag &= (t == 1); ping &= (t == 3);have_ping|=t==3;}
    x = a * 1000 + b * 100 + c * 10 + (d + b) % 10;
    if (d && b&&!Enter[st][3]) { Enter[st][3]=true; int t = dfs(change(x)); num[st]--; if(t!=3) tag &= (t == 1); ping &= (t == 3);have_ping|=t==3;}

	if(st==2121){
		using namespace std;
		cout<<st<<' '<<"tag="<<tag<<' '<<"ping="<<ping<<' '<<"have_ping="<<have_ping<<' '<<"f[2221]="<<f[2221]<<endl;
	}
    if (ping) return f[st] = 3;
    if (have_ping&&tag) return f[st]=3;
    return f[st] = tag ? 2 : 1;
}
int solve(int st) {
    int a = st / 1000, b = (st / 100) % 10, c = (st / 10) % 10, d = st % 10, x;
    if (c && a && f[change(x = a * 1000 + b * 100 + ((c + a) % 10) * 10 + d)] == 2) return x;
    if (c && b && f[change(x = a * 1000 + b * 100 + ((c + b) % 10) * 10 + d)] == 2) return x;
    if (d && a && f[change(x = a * 1000 + b * 100 + c * 10 + (d + a) % 10)] == 2) return x;
    if (d && b && f[change(x = a * 1000 + b * 100 + c * 10 + (d + b) % 10)] == 2) return x;
    
    if (c && a && f[change(x = a * 1000 + b * 100 + ((c + a) % 10) * 10 + d)] == 3) return x;
    if (c && b && f[change(x = a * 1000 + b * 100 + ((c + b) % 10) * 10 + d)] == 3) return x;
    if (d && a && f[change(x = a * 1000 + b * 100 + c * 10 + (d + a) % 10)] == 3) return x;
    if (d && b && f[change(x = a * 1000 + b * 100 + c * 10 + (d + b) % 10)] == 3) return x;
    return x;
}
void init() {
    for (int i = 1; i <= 99; i++) {
        f[i] = 2; f[i * 100] = 1;
    }
    for (int i = 1; i < 10000; i++) if(i!=7860) dfs(i);
}

int check(int st) {
    if (st / 100 == 0) return 1;
    else if (st % 100 == 0) return 2;
    else if (f[st] == 3) return -1;
    else return 0;
}

void output(int st) {
    using std::cout;
    system("cls");
    int a = st / 1000, b = (st / 100) % 10, c = (st / 10) % 10, d = st % 10;
    cout << a << ' ' << b << '\n' << c << ' ' << d << '\n';
}
int input(int st) {
    using namespace std;
    int a = st / 1000, b = (st / 100) % 10, c = (st / 10) % 10, d = st % 10;
    int x, y; cin >> x >> y;
    if (c == x) { c = (c + y) % 10; }
    else { d = (d + y) % 10; }
    return a * 1000 + b * 100 + c * 10 + d;
}
void debug(int st){
	using namespace std;
	cout<<"\n------------------------------------\n";
	cout<<"now state:"<<st<<" "<<f[st]<<endl;
	int a = st / 1000, b = (st / 100) % 10, c = (st / 10) % 10, d = st % 10;
 	int x = a * 1000 + b * 100 + ((c + a) % 10) * 10 + d; x=change(x);
 	if(c&&a) cout<<x<<": "<<f[x]<<endl;
    x = a * 1000 + b * 100 + ((c + b) % 10) * 10 + d;  x=change(x);
    if(c&&b) cout<<x<<": "<<f[x]<<endl;
    x = a * 1000 + b * 100 + c * 10 + (d + a) % 10; x=change(x);
    if(d&&a) cout<<x<<": "<<f[x]<<endl;
    x = a * 1000 + b * 100 + c * 10 + (d + b) % 10; x=change(x);
    if(d&&b) cout<<x<<": "<<f[x]<<endl;
    cout<<"\n------------------------------------\n";
    system("pause");
}
int main() {
    using namespace std; 
    init();
    int st = 1111;
    while (check(st)!=1&&check(st)!=2) {
        output(st);
        st=input(st); output(st); Sleep(500);
        if(check(change(st))==1||check(change(st))==2) break;
		st=change(solve(change(st)));
        output(st);
    }

    if (check(st) == 1)  cout << "You lose!" << endl;
    else if (check(st) == 2) cout << "You win!" << endl;
    else cout << "Draw!" << endl;
    return 0;
}
