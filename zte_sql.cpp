#include<stdlib.h>
#include<stdio.h>
#include<algorithm>
#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<set>
#include<map>
#include<unordered_set>
using namespace std;
int get_col_min(vector<vector<int>> vec,int n)//获得某一列的最大值
{
    int tmp_min=vec[0][n];
    int len_col=vec.size();
    for(int i=1;i<len_col;i++)
    {
        if(vec[i][n]<tmp_min)
        {
            tmp_min=vec[i][n];
        }
    }
    return tmp_min;
}
int get_col_max(vector<vector<int>> vec,int n)//获得某一列的最大值
{
    int tmp_max=vec[0][n];
    int len_col=vec.size();
    for(int i=1;i<len_col;i++)
    {
        if(vec[i][n]>tmp_max)
        {
            tmp_max=vec[i][n];
        }
    }
    return tmp_max;
}
vector<vector<int>> join_on(vector<vector<int>> v1,vector<vector<int>> v2)
{
    vector<vector<int>> res;
    int v1_row_len=v1.size();
    int v2_row_len=v2.size();
    for(int i=0;i<v1_row_len;i++)
    {
        for(int j=0;j<v2_row_len;j++)
        {  
            if(v1[i][2]==v2[j][2])//此处选中第三列做比较，也可以使用参数
            {
                vector<int> tmp1(v1[i]);
                vector<int> tmp2(v2[j]);
                tmp1.insert(tmp1.end(),tmp2.begin(),tmp2.end());
                res.push_back(tmp1);
            }
            
            
        }
    }
    return res;
}

//实现group by t1.id2,t2.id2,并且是按照max(t1.id1),min(t2.id1)进行group by
vector<vector<int>> group_by(vector<vector<int>> v1)
{   
    map<int,int> t1_id2,t2_id2;
    vector<int> t1_id1,t2_id1;
    int v1_row_len=v1.size();
    vector<vector<int>> res;

    for(int i=0;i<v1_row_len;i++)
    {   

        if(t1_id2.count(v1[i][1])==0 && t2_id2.count(v1[i][4])==0)
        {
            t1_id2.insert(make_pair(v1[i][1],i));
            t2_id2.insert(make_pair(v1[i][4],i));
            vector<int> tmp1(v1[i]);
            res.push_back(tmp1);

        }
        else if(t1_id2.count(v1[i][1])>0 && t2_id2.count(v1[i][4])==0)
        {
             t2_id2.insert(make_pair(v1[i][4],i));
             vector<int> tmp1(v1[i]);
             res.push_back(tmp1);
             
        }
        
        else if (t1_id2.count(v1[i][1])==0 && t2_id2.count(v1[i][4])>0)
        {
            t1_id2.insert(make_pair(v1[i][1],i));
            vector<int> tmp1(v1[i]);
            res.push_back(tmp1);
        }  
        else if (t1_id2.count(v1[i][1])>0 && t2_id2.count(v1[i][4])>0)
        {
            int j;
            for( j=0;j<res.size();j++)
            {
                if(v1[i][1]==res[j][1] && v1[i][4]==res[j][4])
                {
                    if(v1[i][0]>res[j][0])
                    {
                        res.insert(res.begin()+j,v1[i]);
                        res.erase(res.begin()+j+1);
                    }
                    else if (v1[i][0]==res[j][0] && v1[i][3]<res[j][3])
                    {
                        res.insert(res.begin()+j,v1[i]);
                        res.erase(res.begin()+j+1);
                    }
                    
                }
            }
           
        }
    }
    return res;
}
//定义sort的cmp方法用于二维数组排序,对应实现order by max(t1.id1),t2.id2,t1.id2;
bool cmp(vector<int> v1,vector<int> v2)
{
    if(v1[0]!=v2[0]) return v1[0]<v2[0];
    if(v1[4]!=v2[4]) return v1[4]<v2[4];
    if(v1[1]!=v2[1]) return v1[1]<v2[1];
}
void order_by(vector<vector<int>> &v1)
{
    sort(v1.begin(),v1.end(),cmp); 
}

int main()
{   
    vector<vector<int>> result;
    vector<int> tmp;
    vector<vector<int>> vec_input1;
    vector<vector<int>> vec_input2;
    ifstream input1("F:\\ZTE_CSV\\input1.csv");
    ifstream input2("F:\\ZTE_CSV\\input2.csv");
    string line1;
    string line2;

    //从文件input1中读取数据至数组vec_input1
    while(getline(input1,line1))
    {   string s1; 
        istringstream s1_cin(line1);

        while(getline(s1_cin,s1,','))
        { 
           tmp.push_back(atoi(s1.c_str()));

        }
        vec_input1.push_back(tmp);
        tmp.clear();
    }
    input1.close();
    //从文件input2中读取数据至数组vec_input2
    while(getline(input2,line2))
    {   string s2; 
        istringstream s2_cin(line2);

        while(getline(s2_cin,s2,','))
        { 
           tmp.push_back(atoi(s2.c_str()));

        }
        vec_input2.push_back(tmp);
        tmp.clear();
    }
    input2.close();

    result=join_on(vec_input1,vec_input2);
    result=group_by(result);
    order_by(result);
    result=group_by(result);
    for(int i=0;i<result.size();i++)
    {
        cout<<result[i][0]<<" "<<result[i][3]<<endl;
    }
    return 0;
}