#include <ctime>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <vector>
using namespace std;
const int MAXN = 10005;

/*********************
 *   游戏状态空间计算   *
 *********************/

/* 存储游戏状态关系的图结构定义，使用链式前向星进行存图 */
int head[MAXN], num_edge;
struct Edge { int to, next; } edge[MAXN * 4];
void add_edge(int from, int to) {
  edge[++num_edge].to = to;
  edge[num_edge].next = head[from];
  head[from] = num_edge;
}

/* 切换活动角色，当一方完成操作后，调用此函数，将可活动状态转移给对手 */
int changeRoles(int st) { return st % 100 * 100 + st / 100; }

/* 触碰对方手指完成一次操作
 * 在st状态下，触碰对方手指，返回触碰后的状态，若不合法则返回-1
 * 假设a, b分别表示对方的左右手，c, d分别表示我方的左右手，则操作op定义如下：
 *     1 - 用c触碰a
 *     2 - 用c触碰b
 *     1 - 用d触碰a
 *     1 - 用c触碰b
 */
int touch(int st, int op) {
  int a = st / 1000, b = (st / 100) % 10, c = (st / 10) % 10, d = st % 10;
  if (op == 1 && c && a)      { return a * 1000 + b * 100 + ((c + a) % 10) * 10 + d; }
  else if (op == 2 && c && b) { return a * 1000 + b * 100 + ((c + b) % 10) * 10 + d; }
  else if (op == 3 && d && a) { return a * 1000 + b * 100 + c * 10 + (d + a) % 10; }
  else if (op == 4 && d && b) { return a * 1000 + b * 100 + c * 10 + (d + b) % 10; }
  else { return -1; }
}

/* 对游戏状态空间建图，每一状态指向可以到达的后继状态 */
void buildGameStateGraph() {
  for (int i = 1; i <= 9999; i++)
    for (int t = 1; t <= 4; t++)
      if (touch(i, t) != -1)
        add_edge(i, changeRoles(touch(i, t)));
}

/* Tarjan 缩点，将状态空间图中的环缩成点
 * 将每个点进行染色，同一环中的点颜色相同，不同环中的点颜色不同
 * 颜色的种类数有col_num种，每一种颜色所包含的点存储在c中。
 */
vector<int> c[MAXN];
int dfn[MAXN], low[MAXN], stack[MAXN], col[MAXN], col_num, dfs_num, top;
bool vis[MAXN];

/* Tarjan 主过程 */
void Tarjan(int x) {
  dfn[x] = low[x] = ++dfs_num;
  vis[x] = 1;
  stack[++top] = x;
  for (int i = head[x]; i; i = edge[i].next) {
    Edge e = edge[i];
    if (!dfn[e.to]) {
      Tarjan(e.to);
      low[x] = min(low[x], low[e.to]);
    } else if (vis[e.to]) {
      low[x] = min(low[x], dfn[e.to]);
    }
  }
  if (dfn[x] == low[x]) {
    ++col_num;
    vis[x] = 0;
    while (stack[top + 1] != x) {
      col[stack[top]] = col_num;
      c[col_num].push_back(stack[top]);
      vis[stack[top--]] = 0;
    }
  }
}

/* 调用入口，使用此函数对整个状态空间图进行缩点 */
void solveTarjan() {
  for (int i = 1; i <= 9999; i++)
    if (!dfn[i])
      Tarjan(i);
}


/* DAG 图的存储数据结构 */
int in[MAXN];
vector<int> G[MAXN];
bool con[1005][1005];

/* 对缩点后的图进行重建，重建后的图为DAG
 * 重建后的图的邻接矩阵表示在con中，邻接表表示存储在G中，每个点的入度保存在in中
 */
void buildDAG() {
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

/* 游戏状态空间
 * 使用一个4位数表示，后2位代表当前轮需要操作的玩家的手指状态
 * ，分别表示左右手上的数字，如果为0则表示相应手被撤下，后2个
 * 数字表示对手的状态
 * 对状态空间内的每个状态进行求解，求该状态进行评价，共有四个选项：
 *   1 - 必胜态，表示若始终以最优策略进行操作，则必胜
 *   2 - 必败态，表示如果对手始终以最优策略进行操作，则必败
 *   3 - 平局，若双方都以最优策略进行操作，则平局
 *   4 - 严格平局，已经进入游戏的平局死循环，游戏平局
 *
 * colState是以颜色值为下标的状态值，gameState是以游戏状态为
 * 下标表示的状态值
 */
int colState[MAXN]; // 1 win, 2 lose, 3 draw, 4 strict draw
int gameState[MAXN];

/*
 * 初始化状态
 * 当对方双手全部撤下，我方仍有手未撤下，则为游戏终态，我方必败
 */
void initState() {
  for (int i = 1; i <= 99; i++)
    gameState[i] = colState[col[i]] = 2;
}

/* 对状态x进行一次计算（从后继状态中推演） */
int calc(int x) {
  // 如果后继状态中有必败态，则当前状态为必胜态
  for (int i = head[x]; i; i = edge[i].next)
    if (gameState[edge[i].to] == 2)
      return 1;

  // 如果后继状态中含有未计算的值，则当前状态也未确定
  for (int i = head[x]; i; i = edge[i].next)
    if (!gameState[edge[i].to])
      return 0;

  // 计算后继状态的总数，和平局/严格平局的数量
  int sum = 0, num_stdraw = 0, num_draw = 0;
  for (int i = head[x]; i; i = edge[i].next) {
    sum++;
    if (gameState[edge[i].to] == 4)
      num_stdraw++;
    else if (gameState[edge[i].to] == 3)
      num_draw++;
  }

  // 后继状态中有平局，则当前状态为平局
  if (num_draw) return 3;

  // 后继状态中只有严格平局，则当前状态为严格平局
  if (num_stdraw == sum) return 4;

  // 后继状态中有严格平局，也有必胜态，则当前状态为平局
  if (num_stdraw) return 3;

  // 后继状态中只有必胜态，则当前状态为必败态
  return 2;
}

/* 以颜色值为索引，搜索状态空间，计算状态 */
int dfs(int x) {
  if (colState[x])
    return colState[x];

  // 计算后继状态总数，和后继状态中4种状态值的数量
  int sum = 0, num[5] = {};
  for (int i = 0; i < G[x].size(); i++) {
    sum++;
    num[dfs(G[x][i])]++;
  }

  // 后继状态中只有（严格）平局，则当前状态一定是平局（若后继没有平局，则为严格平局）
  if (!sum || num[3] + num[4] == sum) {
    for (auto i : c[x])
      gameState[i] = num[3] ? 3 : 4;
    return colState[x] = num[3] ? 3 : 4;
  }

  // 对当前颜色值中的点以及进行求解，由于对后继状态的依赖，需要迭代T次
  int T = c[x].size();
  while (T--) {
    for (auto i : c[x])
      if (!gameState[i])
        gameState[i] = calc(i);
  }

  // 在上述过程中没有求出的点，特殊情况处理
  bool tag = true;
  for (auto i : c[x])
    for (int j = head[i]; j; j = edge[j].next)
      if (gameState[edge[j].to] == 3)
        tag = false; // check strict draw
  for (auto i : c[x])
    if (!gameState[i])
      gameState[i] = tag ? 4 : 3;

  return colState[x] = 1;
}

/* 对状态空间中的每一个状态求解状态值 */
void calcStateValue() {
  buildDAG();
  initState();
  for (int i = 1; i <= col_num; i++)
    if (!in[i]) dfs(i);
}

/* 初始化游戏状态，即求解gamestate数组 */
void calcGameState() {
  buildGameStateGraph();
  solveTarjan();
  calcStateValue();
  srand(time(0));
}

/*********************
 *    游戏过程处理     *
 *********************/

/* 判断当前游戏进行的状态
 *   - 0 游戏进行中
 *   - 1 游戏结束，我方胜
 *   - 2 游戏结束，我方输
 *   - 4 游戏结束，平局
 */
int check(int st) {
  if (st / 100 == 0) return 1;
  else if (st % 100 == 0) return 2;
  else if (gameState[st] == 4) return 4;
  else if (gameState[st] == 3) return 0;
  else return 0;
}

/* 游戏状态输出 */
void output(int st) {
  system("clear");
  int a = st / 1000, b = (st / 100) % 10, c = (st / 10) % 10, d = st % 10;
  cout << a << ' ' << b << '\n' << c << ' ' << d << '\n';
}

/* 游戏状态输入 */
int input(int st) {
  int a = st / 1000, b = (st / 100) % 10, c = (st / 10) % 10, d = st % 10;
  int x, y;

  while (1) {
    x = y = -1;
    cin >> x >> y;
    if ((x != 0 && (x == c || x == d)) && (y != 0 && (y == a || y == b))) {
      break;
    }
    else {
      cout << "Error!" << endl;
      if (cin.fail()) {
        cin.clear();
        while (cin.get() != '\n')
          ;
      }
    }
  }

  if (c == x) {
    c = (c + y) % 10;
  } else {
    d = (d + y) % 10;
  }
  return a * 1000 + b * 100 + c * 10 + d;
}

/* 对于当前状态st，输出最佳决策 */
int solve(int st) {
  vector<int> ans;
  int sta[4] = {2, 3, 4, 1};
  for (int s = 0; s < 4; s++) {
    int x = sta[s];
    ans.clear();
    for (int i = 1; i <= 4; i++)
      if (touch(st, i) != -1 && gameState[changeRoles(touch(st, i))] == x)
        ans.push_back(touch(st, i));
    if (ans.size())
      return ans[(rand() % ans.size())];
  }
  return -1;
}

void debug(int st) {
  cout << "\n------------------------------------\n";
  cout << "now state:" << st << " " << gameState[st] << endl;
  for (int i = 1; i <= 4; i++)
    if (touch(st, i) != -1)
      cout << changeRoles(touch(st, i)) << ':' << gameState[changeRoles(touch(st, i))] << endl;
  cout << "\n------------------------------------\n";
}

/* 游戏主循环 */
void gameMainLoop(int y) {
  int st = 1111;
  output(st);
  if (y == 1) {
    st = input(st);
    output(st);
    system("sleep 0.2");
  }

  while (1) {
    st = changeRoles(solve(changeRoles(st)));
    output(st);
    if (check(st))
      break;

    st = input(st);
    output(st);
    system("sleep 0.2");
    if (check(changeRoles(st)))
      break;
  }

  if (check(st) == 1)
    cout << "You lose!" << endl;
  else if (check(st) == 2)
    cout << "You win!" << endl;
  else
    cout << "Draw!" << endl;
}

/* 选择先手、后手 */
int selectRole() {
  cout << "1.first  2.second: ";
  int opt;
  while (1) {
    opt = -1;
    cin >> opt;
    if (opt == 1 || opt == 2) {
      break;
    }
    else {
      cout << "Error!" << endl;
      if (cin.fail()) {
        cin.clear();
        while (cin.get() != '\n');
      }
    }
  }
  return opt;
}

int main() {
  calcGameState();
  int opt = selectRole();
  gameMainLoop(opt);
  return 0;
}
