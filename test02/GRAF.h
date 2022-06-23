#ifndef GRAF_H
#define GRAF_H
#include <iostream>
#include<queue>
#include<vector>
using namespace std;

struct ARC { //�����
    int from;     // �ߵ����
    int to;       // �ߵ��յ�
    int weight;   // �ߵ�Ȩֵ
    bool visited; // �Ƿ񱻷���
};
struct VTX { // ���嶥��
    int nodeName; // ������Ϣ
    int inDegree; // ���
    int outDegree;// ����
    bool visited; // �Ƿ񱻷���
    int num;      // ���
    int low;      // �������㷨�е��ܵ������Ͷ���ı��
    int dis;      // ��Դ���·�㷨�ж�����Դ��ľ���
    VTX* parent;  // ���ڵ�
};

class GRAF {
private:  
    int nodenum;
    int arcnum;
    int count = 1;
    ARC arc_list[401];// �߼�
    VTX vtx_list[101];// ���㼯

    // ����start, end֮���Ƿ��б�,�����򷵻ر��ڱ߼��е�����
    int findarc(int start, int end)
    {
        for (int i = 1; i <= 2 * arcnum; i++)
        {
            if (arc_list[i].from == start && arc_list[i].to == end)
                return i;
        }
        return 0;
    }

    // ��ͼ�и�����������
    void reset()
    {
        for (int i = 1; i <= nodenum; i++)
        {
            vtx_list[i].low = 0;
            vtx_list[i].num = 0;
            vtx_list[i].visited = false;
        }
        for (int i = 0; i < arcnum; i++)
        {
            arc_list[i].visited = false;
        }
    }
    // ����
    //const int & median3(int left, int right)
    //{
    //    int center = (left + right) / 2;
    //    if (arc_list[center].weight < arc_list[left].weight)
    //        std::swap(arc_list[left], arc_list[center]);
    //    if (arc_list[right].weight < arc_list[left].weight)
    //        std::swap(arc_list[left], arc_list[left]);
    //    if (arc_list[right].weight < arc_list[center].weight)
    //        std::swap(arc_list[center], arc_list[right]);
    //    std::swap(arc_list[center], arc_list[right - 1]);
    //    return arc_list[right - 1].weight;
    //}

    //void quicksort(int left, int right)
    //{
    //    if (left >= right) return;
    //    int pivot = median3(left, right);
    //    int i = left - 1, j = right + 1;
    //    int x = arc_list[left + right >> 1].weight;
    //    while (i < j)
    //    {
    //        do i++; while (arc_list[i].weight < x);
    //        do j--; while (arc_list[j].weight > x);
    //        if (i < j) swap(arc_list[i], arc_list[j]);
    //    }
    //    quicksort(left, j);
    //    quicksort(j + 1, right);
    //}

    // �����������
    int BFS(int start, int end, int pre[])
    {
#define maxn 0x7fffffff
        queue<int> myqueue;
        int flow[100];
        while (!myqueue.empty())  //�������
            myqueue.pop();
        for (int i = 1; i <= nodenum; ++i) {
            pre[i] = -1;
        }
        flow[start] = maxn;
        myqueue.push(start);
        while (!myqueue.empty()) {
            int index = myqueue.front();
            myqueue.pop();
            if (index == end)  //�ҵ�������·��
                break;
            for (int i = index + 1; i <= nodenum; ++i) {
                if (pre[i] == -1 && arc_list[findarc(index, i)].weight > 0)
                {
                    pre[i] = index;
                    flow[i] = min(arc_list[findarc(index, i)].weight, flow[index]);  //�ؼ����������ҵ�����
                    myqueue.push(i);
                }
            }
        }
        if (pre[end] == -1)  //����ͼ�в��ٴ�������·��
            return -1;
        else
            return flow[end];
    }

    // �����������(�������)
    void DFS_pre(int start, vector<int>& path)
    {
        vtx_list[start].visited = true;
        path.push_back(start);
        for (int i = 1; i <= nodenum; i++)
        {
            if (!vtx_list[i].visited && findarc(start, i))
            {
                DFS_pre(i, path);
            }
        }
    }

    // �����������(�������)
    void DFS_post(int start, vector<int>& path)
    {
        vtx_list[start].visited = true;
        for (int i = 1; i <= nodenum; i++)
        {
            if (!vtx_list[i].visited && findarc(start,i))
            {
                DFS_post(i, path);
            }
        }
        path.push_back(start);
    }

    void printPath(int v)
    {       
        if (vtx_list[v].parent != NULL)
        {
            printPath(vtx_list[v].parent->nodeName);
        }
        cout << v << " ";   
    }

    // Ѱ�Ҹ��
    void findArt(VTX& v,vector<int>& result)
    {
        v.visited = true;
        v.low = v.num = count++;
        
        for (int i = 1; i <= nodenum; i++)
        {   
            VTX* w = new VTX;
            if (findarc(v.nodeName, i))
            {
                w = &vtx_list[i];
            }
            if (w->nodeName > 0)
            {
                if (!w->visited)
                {
                    w->parent = &v;
                    findArt(*w, result);
                    if (v.nodeName!=1 && w->low >= v.num)
                        result.push_back(v.nodeName);
                    v.low = min(v.low, w->low);
                }
                else
                    if (v.parent!=NULL && v.parent->nodeName != w->nodeName)
                        v.low = min(v.low, w->num);
            }
        }
    }

    // ŷ����·
    void euler(int start, vector<int>& path)
    {
        path.push_back(start);
        {
            for (int i = 1; i <= nodenum; i++)
            {
                if (findarc(start, i) && !arc_list[findarc(start, i)].visited)
                {
                    arc_list[findarc(start, i)].visited = true;
                    arc_list[findarc(i, start)].visited = true;
                    euler(i, path);
                }
            }
        }
    }


public:
    GRAF(vector<vector<int> >& vt,int nodeNum, int arcNum) {
        nodenum = nodeNum;
        arcnum = arcNum;
        for (int i = 1; i <= nodeNum; i++) {
            vtx_list[i].nodeName = i;
            vtx_list[i].inDegree = 0;
            vtx_list[i].outDegree = 0;
            vtx_list[i].visited = 0;
            vtx_list[i].num = 0;
            vtx_list[i].low = 0;
            vtx_list[i].parent = NULL;
        }

        for (int i = 0; i< vt.size(); i++)
        {
            ARC edge;
            edge.from = vt[i][0] ;
            edge.to = vt[i][1];
            edge.weight = vt[i][2];
            edge.visited = false;
            ++vtx_list[vt[i][0]].outDegree;// ���ȼ�1
            ++vtx_list[vt[i][1]].inDegree; // ��ȼ�1
            //�������ӱ�ĵ�һ��λ��
            arc_list[i+1] = edge;
        }
    }
    ~GRAF() {}
    int vnum(){
        return nodenum;
    }
    int anum() {
        return arcnum;
    }
    bool empty() {
        return (nodenum==0);
    }
    // ��ӡ����ͼ
    void printDGraph() 
    {
        for (int i = 0; i < nodenum; i++) 
        {

            cout << "���v" << vtx_list[i].nodeName << "�����Ϊ";
            cout << vtx_list[i].inDegree <<",����Ϊ"<< vtx_list[i].outDegree<<",����Ϊ��ʼ����ı�Ϊ:";
            for(int j=0;j< nodenum;j++)
            { 
                if (arc_list[j].from == vtx_list[i].nodeName)
                {
                    cout << "(" << arc_list[j].from << "," << arc_list[j].to << "),Ȩ:" << arc_list[j].weight << "   ";
                }
            }
            cout << endl;
        }
    }

    // ��ӡ����ͼ
    void printUGraph()
    {
        for (int i = 0; i < nodenum; i++)
        {
            cout << "���v" << vtx_list[i].nodeName << "�Ķ���Ϊ";
            cout << (vtx_list[i].inDegree + vtx_list[i].outDegree)/2 << ",����Ϊ��ʼ����ı�Ϊ: ";
            for (int j = 0; j < nodenum; j++)
            {
                if (arc_list[j].from == vtx_list[i].nodeName)
                {
                    cout << "(" << arc_list[j].from << "," << arc_list[j].to << "),Ȩ:" << arc_list[j].weight << "   ";
                }
            }
            cout << endl;
        }
    }

    //  ���������㷨       - Topological sort algorithm
    void topsort()
    {
        VTX v_list[100];
        for (int i = 1; i <= nodenum; i++)
        {
            v_list[i] = vtx_list[i];
        }
        queue<VTX> q;
        int count = 0;
        while (!q.empty())
        {
            q.pop();
        }
        for (int i = 1; i <= nodenum; i++)
        {
            if (v_list[i].inDegree == 0)
                q.push(v_list[i]);
        }
        while (!q.empty())
        {
            VTX v = q.front();
            cout << v.nodeName<<" ";
            q.pop();
            vtx_list[v.nodeName].num = ++count;
            for (int j = 1; j <= nodenum; j++)
            {
                if (findarc(v.nodeName, j))
                {
                    if (--v_list[j].inDegree == 0)
                        q.push(v_list[j]);
                }
            }
        }
    }

    // ��Դ���·�㷨     - Dijkstra algorithm
    void Dijkstra(int start)
    {
        int mindis = 10000;
        for (int i = 1; i <= nodenum; i++)
        {
            vtx_list[i].dis = mindis;
        }
        vtx_list[start].dis = 0;
        for (int i = 1; i <= nodenum; i++)
        {
            if (!vtx_list[i].visited && vtx_list[i].dis < mindis)
            {
                
                vtx_list[i].visited = true;
                for(int j=1;j<=nodenum;j++)
                { 
                    if (findarc(i, j) && !vtx_list[j].visited)
                    {   
                        
                        int cvw = arc_list[findarc(i, j)].weight;
                        if (vtx_list[i].dis + cvw < vtx_list[j].dis)
                        {
                            vtx_list[j].dis = vtx_list[i].dis + cvw;
                            vtx_list[j].parent = &vtx_list[i];
                        }
                    }
                }
            }
        }
        for (int i = 1; i <= nodenum; i++)
        {
            cout << i << " " << vtx_list[i].dis << " ";
            if(i!=start)
                printPath(i);
            cout << endl;
        }
    }

    // ��С�������㷨     - Prim algorithm
    void prim()
    {
        int mindis = 10000, weight = 0;
        VTX* next = &vtx_list[1];
        for (int i = 0; i <= nodenum; i++)
        {
            vtx_list[i].dis = mindis;
        }
        vtx_list[1].dis = 0;
        while (next!=NULL)
        {
            if (!next->visited && next->dis < mindis)
            {
                next->visited = true;
                int minidx = 0;
                for (int j = 1; j <= nodenum; j++)
                {
                    int arc = findarc(next->nodeName, j);
                    if (arc && !vtx_list[j].visited)
                    {
                        int cvw = arc_list[arc].weight;
                        if (cvw < vtx_list[j].dis)
                        {
                            vtx_list[j].dis = cvw;
                            vtx_list[j].parent = next;
                        }
                    }
                    if (vtx_list[j].dis < vtx_list[minidx].dis && !vtx_list[j].visited)
                        minidx = j;
                }
                next = &vtx_list[minidx];
            }
            else
                next = NULL;
        }
        for (int i = 1; i <= nodenum; i++)
        {
            if (vtx_list[i].parent != NULL)
            {
                weight += arc_list[findarc(vtx_list[i].parent->nodeName, i)].weight;
            }
        }
        cout << weight << endl;
        for (int i = 1; i <= nodenum; i++)
        {
            if (vtx_list[i].parent != NULL)
            {
                cout << vtx_list[i].parent->nodeName << " " << vtx_list[i].nodeName << endl;
            }
        }
    }
    // ���������·���㷨 - Maximium flow (Augmented path) algorithm
    void maxflow()
    {
        int increasement = 0;
        int sumflow = 0;
        int pre[100];
        while ((increasement = BFS(1, nodenum, pre)) != -1) {
            int k = nodenum;  //����ǰ��Ѱ��·��
            while (k != 1) {
                int last = pre[k];
                arc_list[findarc(last, k)].weight -= increasement;  //�ı�����ߵ�����
                arc_list[findarc(k, last)].weight += increasement;  //�ı䷴��ߵ�����
                k = last;
            }
            sumflow += increasement;
        }
        cout << sumflow << endl;
        for (int i = 1; i <= 2 * arcnum; i++)
        {
            for(int j=i+1;j <= 2 * arcnum;j++)
                if (arc_list[findarc(j, i)].weight >= 0)
                {
                    cout << i << " " << j << " " << arc_list[findarc(j, i)].weight << endl;
                }
        }
    }

    // �������㷨       - Articulation finding algorithm
    void findArt()
    {   
        int sz, k=1;
        for (int i = 1; i <= nodenum; i++)
        {
            vector<int> branch;
            DFS_post(i, branch);
            reset();
            if (i == 1)
            {
                sz = branch.size();
                vector<int> result;
                findArt(vtx_list[i], result);
                cout << k << ": " << result.size() << " ";
                for (int i = result.size() - 1; i >= 0; i--)
                {
                    cout << result[i] << " ";
                }
            }
            else if (branch.size() != sz)
            {
                k++;
                vector<int> result;
                findArt(vtx_list[i], result);
                cout << endl<< k << ": " << result.size() << " ";
                for (int i = result.size() - 1; i >= 0; i--)
                {
                    cout << result[i] << " ";
                }
            }
            reset();
        }
    }

    // ǿ��ͨ��֧�㷨     - Strong Connected Component algorithm
    void strong_connect()
    {
        vector<int> path, result;
        for (int i = 1; i <= nodenum; i++)
        {
            if (!vtx_list[i].visited)
            {
                DFS_post(i,path);
            }
        }
        for (int i = 0; i < arcnum; i++)
        {
            swap(arc_list[i].from, arc_list[i].to);
        }
        for (int i = 1; i <= nodenum; i++)
        {
            vtx_list[i].visited = false;
        }
        for (int i = path.size(); i >= 1; i--)
        {   
            vector<int> reversepath;
            if (!vtx_list[path[i - 1]].visited)
            {
                DFS_post(path[i-1],reversepath);
                for (int j = 0; j < reversepath.size(); j++)
                {
                    cout << reversepath[j] << " ";
                }
                cout << endl;
            }
        }
        
    }
    
    // ŷ����·�㷨       - Euler circuit algorithm
    void euler()
    {
        int start = 1;
        vector<int> path;
        euler(start, path);

        for (int i = path.size() - 1; i >= 1; i--)
        {
            if (path[i] == start)
            {
                for (int j = 0; j < path.size(); j++)
                {
                    if (path[j] == path.back())
                    {
                        for (int k = 1; k < path.size() - i; k++)
                        {
                            swap(path[i + k], path[j + k]);
                        }
                        break;
                    }
                }
                if (path.back() != start)
                {
                    path.push_back(start);
                    vector<int>::iterator it = path.begin() + i;
                    path.erase(it);
                    break;
                }
            }
        }
        if (path.back() != path.front())
            cout << "������ŷ����·";
        else
        {
            for (int i = 0; i < path.size(); i++)
            {
                cout << path[i] << " ";
            }
            cout << endl;
        }
    }
};
#endif // !GRAF_H
