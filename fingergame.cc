#include<iostream>
#include<vector>
#include<string>
#include<ctime>
#include<stdlib.h>
#include<windows.h>
using namespace std;
const int MAXN = 10005;
int head[MAXN], num_edge;
struct Edge {
	int to, next;
}edge[MAXN * 4];
void add_edge(int from, int to) {
	edge[++num_edge].to = to;
	edge[num_edge].next = head[from];
	head[from] = num_edge;
}

vector<int> c[MAXN];
int dfn[MAXN], low[MAXN], stack[MAXN], col[MAXN], col_num, dfs_num, top;
bool vis[MAXN];
void Tarjan(int x) {
	dfn[x] = low[x] = ++dfs_num;
	vis[x] = 1; stack[++top] = x;
	for (int i = head[x]; i; i = edge[i].next) {
		Edge e = edge[i];
		if (!dfn[e.to]) {
			Tarjan(e.to);
			low[x] = min(low[x], low[e.to]);
		}
		else if (vis[e.to]) {
			low[x] = min(low[x], dfn[e.to]);
		}
	}
	if (dfn[x] == low[x]) {
		++col_num; vis[x] = 0;
		while (stack[top + 1] != x) {
			col[stack[top]] = col_num;
			c[col_num].push_back(stack[top]);
			vis[stack[top--]] = 0;
		}
	}
}
//---------Tarjan¡ü--------

int change(int x) { return x % 100 * 100 + x / 100; }
int P(int st, int t) {
	int a = st / 1000, b = (st / 100) % 10, c = (st / 10) % 10, d = st % 10;
	if (t == 1) if (c && a) return a * 1000 + b * 100 + ((c + a) % 10) * 10 + d;
				else return -1;
	if (t == 2) if (c && b) return a * 1000 + b * 100 + ((c + b) % 10) * 10 + d;
				else return -1;
	if (t == 3) if (d && a) return a * 1000 + b * 100 + c * 10 + (d + a) % 10;
				else return -1;
	if (t == 4) if (d && b) return a * 1000 + b * 100 + c * 10 + (d + b) % 10;
				else return -1;
}
void build() {
	for (int i = 1; i <= 9999; i++)
		for (int t = 1; t <= 4; t++)
			if (P(i, t) != -1) add_edge(i, change(P(i, t)));
}
void solve_tarjan() {
	for (int i = 1; i <= 9999; i++)
		if (!dfn[i]) Tarjan(i);
}
//-----------

int in[MAXN];
vector<int> G[MAXN];
bool con[1005][1005];
void build2() {
	for (int x = 1; x < 10000; x++) {
		for (int i = head[x]; i; i = edge[i].next) {
			Edge e = edge[i];
			if (col[x] != col[e.to] && !con[col[x]][col[e.to]]) {
				G[col[x]].push_back(col[e.to]);
				con[col[x]][col[e.to]] = true;
				in[col[e.to]]++;
			}
		}
	}
}
int dp[MAXN];//1 win£¬2 lose£¬3 draw ,4 strict draw
int f[MAXN];
void init_state() {
	for (int i = 1; i <= 99; i++)
		f[i] = dp[col[i]] = 2;
}
int calc(int x) {
	for (int i = head[x]; i; i = edge[i].next)
		if (f[edge[i].to] == 2) return 1;
	for (int i = head[x]; i; i = edge[i].next)
		if (!f[edge[i].to]) return 0;

	int sum = 0, num_stdraw = 0, num_draw = 0;
	for (int i = head[x]; i; i = edge[i].next) {
		sum++;
		if (f[edge[i].to] == 4) num_stdraw++;
		else if (f[edge[i].to] == 3) num_draw++;
	}

	if (num_draw) return 3;
	if (num_stdraw == sum) return 4;
	if (num_stdraw) return 3;
	return 2;
}
int dfs(int x) {
	if (dp[x]) return dp[x];
	int sum = 0, num[5] = {};
	for (int i = 0; i < G[x].size(); i++) {
		sum++; num[dfs(G[x][i])]++;
	}
	if (!sum || num[3] + num[4] == sum) {
		for (auto i : c[x]) f[i] = num[3] ? 3 : 4;
		return dp[x] = num[3] ? 3 : 4;
	}

	int T = c[x].size();
	while (T--) {
		for (auto i : c[x])
			if (!f[i]) f[i] = calc(i);
	}

	bool tag = true;
	for (auto i : c[x])
		for (int j = head[i]; j; j = edge[j].next)
			if (f[edge[j].to] == 3) tag = false; //check strict draw
	for (auto i : c[x]) if (!f[i]) f[i] = tag ? 4 : 3;

	return dp[x] = 1;
}
void solve_dp() {
	build2();
	init_state();
	for (int i = 1; i <= col_num; i++)
		if (!in[i]) dfs(i);
}

void init() {
	build();
	solve_tarjan();
	solve_dp();
	srand(time(0));
}
//----------------------

int check(int st) {
	if (st / 100 == 0) return 1;
	else if (st % 100 == 0) return 2;
	else if (f[st] == 4) return 4;
	else if (f[st] == 3) return 0;
	else return 0;
}
void output(int st) {
	system("cls");
	int a = st / 1000, b = (st / 100) % 10, c = (st / 10) % 10, d = st % 10;
	cout << a << ' ' << b << '\n' << c << ' ' << d << '\n';
}
int input(int st) {
	int a = st / 1000, b = (st / 100) % 10, c = (st / 10) % 10, d = st % 10;
	int x, y;

	while (1) {
		x = y = -1; cin >> x >> y;
		if ((x != 0 && (x == c || x == d)) && (y != 0 && (y == a || y == b))) break;
		else {
			cout << "Error!" << endl;
			if (cin.fail()) { cin.clear(); while (cin.get() != '\n'); }
		}
	}

	if (c == x) { c = (c + y) % 10; }
	else { d = (d + y) % 10; }
	return a * 1000 + b * 100 + c * 10 + d;
}
int solve(int st) {
	vector<int> ans;
	for (auto x : { 2,4,3,1 }) {
		ans.clear();
		for (int i = 1; i <= 4; i++)
			if (P(st, i) != -1 && f[change(P(st, i))] == x) ans.push_back(P(st, i));
		if (ans.size()) return ans[(rand() % ans.size())];
	}
}
void debug(int st) {
	cout << "\n------------------------------------\n";
	cout << "now state:" << st << " " << f[st] << endl;
	for (int i = 1; i <= 4; i++)
		if (P(st, i) != -1) cout << change(P(st, i)) << ':' << f[change(P(st, i))] << endl;
	cout << "\n------------------------------------\n";
	system("pause");
}
void work(int y) {
	int st = 1111; output(st);
	if (y == 1) { st = input(st); output(st); Sleep(200); }

	while (1) {
		st = change(solve(change(st))); output(st);
		if (check(st)) break;

		st = input(st); output(st); Sleep(200);
		if (check(change(st))) break;
	}

	if (check(st) == 1) cout << "You lose!" << endl;
	else if (check(st) == 2) cout << "You win!" << endl;
	else cout << "Draw!" << endl;
}
int main() {
	init();

	cout << "1.first  2.second: ";
	int opt;
	while (1) {
		opt = -1; cin >> opt;
		if (opt == 1 || opt == 2) break;
		else {
			cout << "Error!" << endl;
			if (cin.fail()) { cin.clear(); while (cin.get() != '\n'); }
		}
	}

	work(opt);

	system("pause");
	return 0;
}

