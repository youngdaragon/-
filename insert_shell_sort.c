#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

#define NUM_DATA 40000
#define MAX_NUM 50000

void insert_sort(int* a, int n);
void shell_sort(int a[], int n);
void double_sort(int **a, int n);
void sub_insert_sort(int **a, int start_n, int size, int idx);
void stable_double_sort(int** a, int n);

int main()
{
    srand(time(NULL));
    float t1,t2;
    clock_t clk1, clk2;
    int* data1, *data2, **data3, **data4;
    int NUM1;

    data1=(int*)malloc(NUM_DATA*sizeof(int));
    data2=(int*)malloc(NUM_DATA*sizeof(int));

    for(int i=0; i<NUM_DATA; i++)
    {
        data1[i]=rand()%MAX_NUM;
        if(i%(NUM_DATA/10)==0)
            printf("%d : %d \n", i, data1[i]);
    }
    memcpy(data2, data1, NUM_DATA*sizeof(int));
    clk1=clock();
    insert_sort(data1,NUM_DATA);
    clk2=clock();
    t1=(float)(clk2-clk1)/CLOCKS_PER_SEC;
    printf(" \n\n data size=%d insert sort time=%lf s\n",NUM_DATA, t1);
    clk1=clock();
    shell_sort(data2,NUM_DATA);
    clk2=clock();
    t2=(float)(clk2-clk1)/CLOCKS_PER_SEC;
    printf(" \n shell sort time =%lf s, sort time ratio=%f \n\n",t2,t1/t2);
    printf(" insert sort shell sort \n");
    for (int i=0; i<NUM_DATA; i++)
        if(i%(NUM_DATA/10)==0)
            printf("%d : %d %d \n", i, data1[i],data2[i]);

    printf(" \n enter num data for 2D array sort \n");
    scanf("%d", &NUM1);
    data3=(int**)malloc(NUM1*sizeof(int*));
    data4=(int**)malloc(NUM1*sizeof(int*));
    for(int i=0; i<NUM1; i++)
    {
        data3[i]=(int*)malloc(2*sizeof(int));
        data4[i]=(int*)malloc(2*sizeof(int));
    }
    int n1=NUM1;
    int n2=2;
    for(int i=0; i<NUM1; i++)
    {
        data3[i][0]=rand()%4 +1;
        data3[i][1]=rand()%100;
    }
    memcpy(data4,data3,2+NUM1*sizeof(int));
    printf(" \n Below 2D array sort not using stability sort : \n");
    double_sort(data3,n1);

    printf("\n Below 2D array sort using stability sort : \n");
    stable_double_sort(data4, n1);
}

void insert_sort(int* a, int n)
{
    int j,t;
    for (int i=1; i<n; i++)
    {
        t=a[j];
        j=i;
        while(j>0 && a[j-1]>t)
        {
            a[j]=a[j-1];
            j--;
        }
        a[j]=t;
    }
}

void shell_sort(int a[], int n)
{
    int i,j,k,h,v;
    for(h=n/2; h>0; h/=2)
    {
        for(i=0;i<h;i++)
        {
            for(j=i+h; j<n; j+=h)
            {
                v=a[j];
                k=j;
                while(k>h-1 && a[k-h]>v)
                {
                    a[k]=a[k-h];
                    k-=h;
                }
                a[k]=v;
            }
        }
    }
}

void stable_double_sort(int** a, int n)
{
    for(int chk_idx=1; chk_idx>=0; chk_idx--)
    {
        sub_insert_sort(a,0,n,chk_idx);
    }

    printf(" \n Sorting result of 2D sort with stability \n");
    for(int i=0; i<n; i++)
    {
        printf(" %d   %d  \n",a[i][0],a[i][1]);
    }
}

void sub_insert_sort(int** a, int start_idx, int size, int chk_idx)
{
    int i,j,t[2];
    for (i=start_idx; i<start_idx +size; i++)
    {
        memcpy(t,a[i],2*sizeof(int));
        j=i;
        while(j>start_idx && a[j-1][chk_idx]>t[chk_idx])
        {
            memcpy(a[j],a[j-1],2*sizeof(int));
            j--;
        }
        memcpy(a[j],t,2*sizeof(int));
    }
}

void double_sort(int **a, int n)
{
    int i,k,start_idx,sort_size;

    sub_insert_sort(a,0,n,0);
    printf("\n Result of subsort 1st done not using stability \n");
    for(i=0; i<n; i++)
        printf(" %d   %d \n",a[i][0], a[i][1]);
    
    k=1;
    start_idx=0;
    sort_size=0;

    for (i=0; i<n; i++)
    {
        if(a[i][0]>k || i==(n-1))
        {
            if(i==(n-1) && a[i][0] ==k)
                sort_size++;
            sub_insert_sort(a,start_idx,sort_size,1);
            printf(" Sort result of k =%d \n",k);
            for(int i=start_idx; i<start_idx+sort_size; i++)
                printf(" %d   %d \n",a[i][0], a[i][1]);
            k=a[i][0];
            start_idx+=sort_size;
            sort_size=1;
        }
        else if(a[i][0]==k)
            sort_size++;
    }
    printf("\n Result of subsort 2nd done not using stability \n");
    for(int i=0; i<n; i++)
        printf(" %d    %d \n",a[i][0], a[i][1]);        
}