#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

#define NUM_INTERVAL 7
#define MAX_TIME 30
#define MAX_DURATION 5
#define MAX_WEIGHT 5
#ifndef min
#define min(a,b)(((a)<(b))?(a):(b))
#endif
#ifndef max
#define max(a,b)(((a)>(b))?(a):(b))
#endif

typedef struct _interval
{
    int start_time;
    int duration;
    int end_time;
    int weight;
} Interval;

int check_duplicated(Interval* p, int t);
void end_time_sort(Interval* p,Interval *s);
int reculsive_schedule(Interval * p, int end_time);
void DP_schedule(Interval *p, int * opt_sum);
int find_prev_idx(Interval *p,int t);
void Select_interval(Interval *p, int *opt_sum);

int main()
{
    srand(time(NULL));
    Interval interval[NUM_INTERVAL];
    Interval sorted_interval[NUM_INTERVAL];
    int max_weight_sum; //최종결과
    int *opt_sum;
    opt_sum=(int*)calloc(NUM_INTERVAL,sizeof(int));//넘인터벌로 설정해줘야 함
    int Z;
    printf(" start_T end_T weight\n");
    for(int i=0;i<NUM_INTERVAL;i++)
    {
        while(1)
        {
            Z=rand() % MAX_TIME+1;
            if(check_duplicated(interval,Z))
            {
                break;
            }
        interval[i].end_time=Z;
        Z=rand()%MAX_DURATION+1;
        interval[i].duration=min(Z,interval[i].end_time);
        interval[i].start_time=interval[i].end_time-interval[i].duration+1;
        interval[i].weight=rand()%MAX_WEIGHT+1;
        printf(" %d-th: %d   %d   %d \n",i,interval[i].start_time,interval[i].end_time,interval[i].weight);
        }
        end_time_sort(interval,sorted_interval);

        max_weight_sum = reculsive_schedule(interval, MAX_TIME+1);
        printf(" By recursive call, max weight sum= %d \n", max_weight_sum);
        DP_schedule(interval, opt_sum);
        printf(" By DP, max weight sum= %d \n",opt_sum[NUM_INTERVAL-1]);
        Select_interval(interval,  opt_sum);

    }
}

int check_duplicated(Interval *p, int t)
{
    for(int i=0; i<NUM_INTERVAL; i++)
        if(p[i].end_time==t)
            return 0;
    return 1;
}

void end_time_sort(Interval * p, Interval *s)
{
    int tmp_min;
    int min_idx=0;
    Interval tmp;

    for(int i=0; i<NUM_INTERVAL; i++)
    {
        tmp_min=MAX_TIME+10;
        for(int j=i; j<NUM_INTERVAL; j++)
        {
            if (p[j].end_time<tmp_min)
            {
                tmp_min=p[j].end_time;
                min_idx=j;
            }
        }
        memcpy(&tmp, &p[i], sizeof(Interval));
        memcpy(&p[i], &p[min_idx], sizeof(Interval));
        memcpy(&p[min_idx], &tmp, sizeof(Interval));
    }
    printf(" \nEnd-time sorted \n");
    printf(" start_T end_T weight\n");
    for(int i=0; i<NUM_INTERVAL; i++)
        printf(" %d-th: %d     %d         %d\n",i,p[i].start_time,p[i].end_time,p[i].weight);
}

int reculsive_schedule(Interval * p, int given_end_time)
{
    int max_sum=0;
    int prev_idx2, A,B;

    for (int i=NUM_INTERVAL;i>=0;i--)
    {
        if(p[i].end_time>=given_end_time)
        {
            continue;
        }
        else
        {
            prev_idx2=find_prev_idx(p,p[i].end_time);
            A=reculsive_schedule(p,p[i].start_time)+p[i].weight;
            B=reculsive_schedule(p,p[prev_idx2].end_time+1);
            max_sum=max(A,B);
            break;
        }
    }
    return max_sum;
}

void DP_schedule(Interval* p,int *opt_sum)
{
    int prev_idx,prev_idx2;
    int prev_end_time=0;

    for(int i=0; i<NUM_INTERVAL; i++)
    {
        if(i==0)
        {
            opt_sum[0]=p[0].weight;
            prev_end_time=p[0].end_time;
            continue;
        }
        else{
            prev_idx=find_prev_idx(p,p[i].start_time);
            opt_sum[i]=max(p[i].weight+opt_sum[prev_idx],opt_sum[i-1]);
        }
    }
}

int find_prev_idx(Interval * p,int t)
{
    int idx=-1;

    for(int i=0;i<NUM_INTERVAL; i++)
    {
        if(p[i].end_time<t)
            idx=1;
        else
            return idx;
    }
}

void Select_interval(Interval * p, int *opt_sum)
{
    int prev_end_time, prev_idx, prev_idx2;

    prev_end_time=MAX_TIME+10;

    for(int i=NUM_INTERVAL-1; i>=1; i--)
    {
        if(p[i].end_time>=prev_end_time)
            continue;
        prev_idx=find_prev_idx(p,p[i].start_time);
        prev_idx2=find_prev_idx(p,p[i].end_time);

        if(p[i].weight + opt_sum[prev_idx]>opt_sum[prev_idx2])
        {
            printf(" selected idx= %d \n",i);
            prev_end_time=p[i].start_time;
        }
        else
        {
            printf(" selected idx=%d \n",prev_idx2);
            prev_end_time=p[prev_idx2].start_time;
        }
    }
}