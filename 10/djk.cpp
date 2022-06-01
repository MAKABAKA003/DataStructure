#include <iostream>
using namespace std;
int e[100][100];
int vis[100];
int dis[100];
int n, m, s;
int min1 = 99999999;
int u = 0;
void dijkstra()
{
    for (int i = 1; i <= n - 1; i++)
    {
        min1 = 99999999;
        // Ѱ��Ȩֵ��С�ĵ�u
        for (int j = 1; j <= n; j++)
        {
            if (vis[j] == 0 && dis[j] < min1)
            {
                min1 = dis[j];
                u = j;
            }
        }

        vis[u] = 1;

        for (int v = 1; v <= n; v++)
        {
            // ����ÿ��u�ɴ��v��˵
            if (e[u][v] < 99999999)
            {
                // �����ǰ��dis[v]�����������β���ʽ����ô�����ɳڲ���
                if (dis[v] > dis[u] + e[u][v])
                {
                    dis[v] = dis[u] + e[u][v];
                }
            }
        }
    }
}

int main()
{

    cin >> n >> m;
    // ��ʼ���ڽӱ�
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= n; j++)
        {
            if (i == j)
            {
                e[i][j] = 0;
            }
            else
            {
                e[i][j] = min1;
            }
        }
    }
    // �������
    for (int i = 1; i <= m; i++)
    {
        int a, b, c;
        cin >> a >> b >> c;
        e[a][b] = c;
    }
    for (int i = 1; i <= n; i++)
    {
        dis[i] = e[s][i];
    }
    cin >> s;
    vis[s] = 1;

    dijkstra();

    for (int i = 1; i <= n; i++)
            {   
                cout << i <<" " << dis[i] << endl;
            }
    return 0;
}