#include<iostream>
#include<vector>
#include<map>
using namespace std;
class Link 
{
    public:
        int LinkID;
        int Node1;
        int Node2;
        int oldW;
        int newW;
        Link()
        {
            LinkID = 0;
            Node1 = 0;
            Node2 = 0;
            oldW = 0;
            newW = 0;
        }
};
class Node
{
    public:
        int ID;
        map<int, int> oldResult;
        map<int, int> newResult;
};

int Nodes, Dsts, Links;
vector<int> dsts;
vector<Link> links;
vector<Node> nodes;

void shortestPath(vector<int>& dists, vector<int>& visited, int min, const int dst, int times,const int flag)
{
    if(flag == 0)   //距離最短者(min)的下家 記錄到result                                   
        nodes[min].oldResult[dst] = visited[min];
    else
        nodes[min].newResult[dst] = visited[min];

    if(times <= 1)  //執行了Nodes次 結束                                  
        return;

    for(int i = 0; i < Links; i++)                  
    {
        int tmp;
        //尋找與min有關的link
        if(links[i].Node1 == min)
            tmp = links[i].Node2;

        else if(links[i].Node2 == min)
            tmp = links[i].Node1;

        else
            continue;

        //此link上的另一個node(tmp)已經找到最短路徑
        if(flag == 0 && nodes[tmp].oldResult.count(dst) == 1)       
            continue;
        else if(flag == 1 && nodes[tmp].newResult.count(dst) == 1)
            continue;

        //計算經過此link的最短路徑
        int weight = flag? links[i].newW : links[i].oldW;    
        if(dists[tmp] >= 1000000 || dists[tmp] > dists[min] + weight)       //第一次到tmp || 距離變短
        {
            dists[tmp] = dists[min] + weight;
            visited[tmp] = min;
        }
        else if(dists[tmp] == dists[min] + weight && visited[tmp] > min)    //距離相同但ID較小
        {
            visited[tmp] = min;
        }
    }

    //尋找下一個距離最短且未找到最短路徑的點(min)
    for(int i = 0; i < Nodes; i++)
    {
        if(flag == 0)
        {
            if(dists[min] >= 1000000 || nodes[min].oldResult.count(dst) == 1)       //路徑無限大 || 此點已有最短路徑
                min = i;

            else if(nodes[i].oldResult.count(dst) == 0 && dists[i] < dists[min])    //尚未找到最短路徑 && 路徑較dists[min]短
                min = i;
        }
        else
        {
            if(dists[min] >= 1000000 || nodes[min].newResult.count(dst) == 1)
                min = i;

            else if(nodes[i].newResult.count(dst) == 0 && dists[i] < dists[min])
                min = i;
            
        }
    }

    shortestPath(dists, visited, min, dst, times-1, flag);
}

void start_shortestPath(int dst,const int flag)
{
        vector<int> dists(Nodes, 1000000);      //與終點的距離
        vector<int> visited(Nodes, -1);         //初始化-1 記錄next node
        dists[dst] = 0;
        visited[dst] = dst;                     //終點下家
        shortestPath(dists, visited, dst, dst, Nodes, flag);
}

int main()
{
    cin >> Nodes >> Dsts >> Links;
    nodes.resize(Nodes);
    for(int i = 0; i < Nodes; i++)
    {
        nodes[i].ID = i;
    }
    for( int i = 0; i < Dsts; i++)  //input destinations
    {
        int tmp = 0;
        cin >> tmp;
        dsts.push_back(tmp);

    }
    for( int i = 0; i < Links; i++) //input links
    {
        Link tmp;
        cin >> tmp.LinkID 
            >> tmp.Node1 
            >> tmp.Node2 
            >> tmp.oldW 
            >> tmp.newW;
        links.push_back(tmp);
    }
    

    for(int i = 0; i < Dsts; i++)
    {
        start_shortestPath(dsts[i], 0);   //oldW
        start_shortestPath(dsts[i], 1);   //newW
    }

    for(int i = 0; i < Nodes; i++)        //output
    {
        cout<< i <<"\n";
        for(int j = 0; j < Dsts; j++)
        {
            if(i == nodes[i].oldResult[dsts[j]])    //destination
                continue;
            cout << dsts[j] << " " << nodes[i].oldResult[dsts[j]] << " \n";
        }
    }
    
    for(int i = 0; i < Nodes; i++)
    {
        int count = 0;  //與oldResult不同的次數
        for(int j = 0; j < Dsts; j++)
        {
            if(nodes[i].newResult[dsts[j]] == nodes[i].oldResult[dsts[j]])
                continue;
            if(count++ == 0)
                cout << i <<"\n";
            cout << dsts[j] << " " << nodes[i].newResult[dsts[j]] << " \n";
        }
    }
}
