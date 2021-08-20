#include <bits/stdc++.h>

constexpr int N=2005;

using std:: string;

std:: set<string> trans[N][N];
bool edge[N][N];

void addEdge(int x,int y,const string &s) {
	edge[x][y]=true;
	trans[x][y].insert(s);
}

void simp(int n,int st,std:: set<int> ed) {
	std:: map<int,bool> mark;
	for (int x=1;x<=n;++x) { // eliminate x
		if (mark[x]||(ed.find(x)!=ed.end())) continue;
		for (int y=1;y<=n;++y) if (edge[x][y]) {
			if (mark[y]||x==y) continue;
			for (int z=1;z<=n;++z) if (edge[z][x]) {
				if (mark[z]||x==z) continue;
				// a path: z->x->y, z may equal y
				for (auto szx: trans[z][x]) for (auto sxy: trans[x][y]) {
					trans[z][y].insert(szx+sxy);
				}
			}
		}
		mark[x]=true;
	}
	for (auto ed_point:ed) {
		for (auto str:trans[ed_point][ed_point]) {
			std::cout<<str<<"|";
		}
	}
}

char buf[N];

int main(void) {
	std:: set<int> ed;
	FILE *file=fopen("minimized_DFA.gv","r");
	int n,m,st=1;
	for (int cnt=0;fgets(buf,N,file);++cnt) {
		if (cnt==0) sscanf(buf,"//Number of Nodes: %d",&n);
		if (cnt==1) sscanf(buf,"//Number of Edges: %d",&m);
		if (cnt<=5) continue;
		if (cnt<=5+m) {
			int x,y; char ch;
			sscanf(buf,"%d -> %d [styel=bold,label=\" %c\"];",&x,&y,&ch);
			string str{ch};
			addEdge(x,y,str);
		} else {
			int id; char ch;
			sscanf(buf,"	%d:body[shape=%coublecircle, width=0.4];",&id,&ch);
			if (ch=='d'&&id!=1) ed.insert(id);
		}
	}
	if (ed.empty()) ed.insert(st);
	simp(n,st,ed);
	return 0;
}