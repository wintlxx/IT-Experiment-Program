#include <cstdio>
#include <cstring>
#include <map>
#include <stack>
#include <algorithm>
using namespace std;
struct node
{
    char s1[10], s2[20] ;
} g[100];

char s[100] ;
int num ;
map<char,int> Map1;//对终结符映射
map<char,int> Map2;//对非终结符映射
int Map1_num, Map2_num;//记录终结符个数，非终结符个数
char s1[100] , s2[100] ;//存储终结符和非终结符
int first[20][20] ;
int follow[20][20] ;
int vis[100];//禁止重复递归
int acc[20] ;//使用以求得的集合
int select1[100][20] ;
int c[20][20] ;
char sta[100] ;
int cnt ;
void dfs1(char ch,int dis[])
{
    if( acc[ Map1[ch] ] )
    {
        for(int i = 1 ; i <= Map2_num; i++)
            dis[i] = first[ Map1[ch] ][i] ;
        return ;
    }
    int value[20] ;
    memset(value,0,sizeof(value));
    for(int i = 1 ; i <= num ; i++)
    {
        if( vis[i] ) continue ;
        if( g[i].s1[0] != ch ) continue ;
        int j , k , len = strlen(g[i].s2) ;
        for(j = 0 ; j < len ; j++)
        {
            if( Map1[ g[i].s2[j] ] )
            {
                vis[i] = 1 ;
                dfs1(g[i].s2[j],value);
                for(k = 2 ; k <= Map2_num ; k++)
                    if( value[k] ) dis[k] = 1 ;
                if( value[1] == 0 ) return ;
            }
            else
            {
                dis[ Map2[ g[i].s2[j] ] ] = 1 ;
                break;
            }
        }
        if( j == len )
            dis[1] = 1 ;
    }
    return ;
}
void dfs2(char ch,int dis[])
{
    int i , j , k , len , flag = 0 ;
    if( acc[ Map1[ch] ] )
    {
        for(int i = 1 ; i <= Map2_num; i++)
            dis[i] = follow[ Map1[ch] ][i] ;
        return ;
    }
    for(i = 1 ; i <= num ; i++)
    {
        len = strlen(g[i].s2) ;
        for(j = 0 ; j < len ; j++)
        {
            if( g[i].s2[j] == ch )
            {
                flag = 1 ;
                continue ;
            }
            if( !flag ) continue ;
            if( Map1[ g[i].s2[j] ] )
            {
                for(k = 2 ; k <= Map2_num ; k++)
                    if( first[ Map1[ g[i].s2[j] ] ][k] ) dis[k] = 1 ;
                if( !first[ Map1[ g[i].s2[j] ] ][1] )
                    flag = 0 ;
            }
            else
            {
                dis[ Map2[ g[i].s2[j] ] ] = 1 ;
                flag = 0 ;
            }
        }
        if( !flag || vis[i] ) continue ;
        int value[20] ;
        memset(value,0,sizeof(value));
        vis[i] = 1 ;
        dfs2(g[i].s1[0],value);
        for(i = 1 ; i <= Map2_num; i++)
            if( value[i] ) dis[i] = 1 ;

    }
}
int main()
{
    int i , j , k , len ;
    //输入
    num = Map1_num = Map2_num = 0 ;
    Map1.clear();
    Map2.clear();
    Map2[ '@' ] = ++Map2_num;
    s2[ Map2_num ] = '@';
    int m;
    scanf("%d", &m);
    while( m-- )
    {
        scanf("%s", s);
        len = strlen(s) ;
        num++ ;
        for(i = 0 ; i < 1 ; i++)
        {
            g[num].s1[i] = s[i] ;
            if( !Map1[ s[i] ] )
            {
                Map1[ s[i] ] = ++Map1_num ;
                s1[Map1_num] = s[i];
            }
        }
        g[num].s1[i] = '\0' ;
        for(i = 3 ; i < len ; i++)
        {
            g[num].s2[i-3] = s[i] ;
            if( s[i] >= 'A' && s[i] <= 'Z' )
            {
                if( !Map1[ s[i] ] )
                {
                    Map1[ s[i] ] = ++Map1_num ;
                    s1[Map1_num] = s[i];
                }
            }
            else
            {
                if( !Map2[ s[i] ] )
                {
                    Map2[ s[i] ] = ++Map2_num;
                    s2[Map2_num] = s[i];
                }
            }
        }
        g[num].s2[i-3] = '\0';
    }
    //求first
    memset(first,0,sizeof(first));
    memset(acc,0,sizeof(acc));
    for(i = 1 ; i <= Map1_num ; i++)
    {
        int value[20];
        memset(vis,0,sizeof(vis));
        memset(value,0,sizeof(value));
        dfs1(s1[i],value);
        for(j = 1 ; j <= Map2_num; j++)
            first[i][j] = value[j] ;
        acc[i] = 1 ;
    }
    //求follow
    Map2['#'] = ++Map2_num ;
    s2[ Map2_num ] = '#' ;
    memset(follow,0,sizeof(follow));
    memset(acc,0,sizeof(acc));
    for(i = 1 ; i <= Map1_num ; i++)
    {
        int value[20] ;
        memset(vis,0,sizeof(vis)) ;
        memset(value,0,sizeof(value));
        if( s1[i] == 'S' ) value[ Map2['#'] ] = 1 ;
        dfs2(s1[i],value);
        for(j = 1 ; j <= Map2_num; j++)
            follow[i][j] = value[j] ;
        acc[i] = 1 ;
    }
    //求select1
    memset(select1,0,sizeof(select1));
    for(i = 1 ; i <= num ; i++)
    {
        for(j = 0 ; j < strlen(g[i].s2) ; j++)
        {
            if( Map2[ g[i].s2[j] ] && g[i].s2[j] != '@' )
            {
                select1[i][ Map2[ g[i].s2[j] ] ] = 1 ;
                break ;
            }
            for(k = 2 ; k <= Map2_num ; k++)
            {
                if( first[ Map1[ g[i].s2[j] ] ][k] ) select1[i][k] = 1 ;
            }
            if( first[ Map1[ g[i].s1[j] ] ][1] == 0 ) break ;
        }
        if( j == strlen( g[i].s2 ) )
        {
            for(k = 2 ;  k <= Map2_num ; k++)
                if( follow[ Map1[ g[i].s1[0] ] ][k] ) select1[i][k] = 1 ;
        }
    }
    //预测分析表
    memset(c,0,sizeof(c)) ;
    for(i = 1 ; i <= num ; i++)
    {
        for(j = 1 ; j <= Map2_num ; j++)
        {
            if( select1[i][j] )
                c[ Map1[ g[i].s1[0] ] ][j] = i ;
        }
    }
    for(i = 1 ; i <= Map2_num ; i++)
    {
        if( i == 1 ) printf("\t") ;
        else
            printf("%c\t", s2[i]) ;
    }
    printf("\n") ;
    for(i = 1 ; i <= Map1_num ; i++)
    {
        printf("%c\t", s1[i]) ;
        for(j = 2 ; j <= Map2_num ; j++)
        {
            if( c[i][j] == 0 ) printf("\t") ;
            else printf("%s\t", g[ c[i][j] ].s2) ;
        }
        printf("\n") ;
    }
    return 0;
}
