#include<stdio.h>
#include <stdlib.h>

void multiplyMatrix(double **mA, double ** mB, double **mC, int a_row_size, int a_col_size, int b_col_size);
void input_matrix(double mat[][2], int row_size, int col_size);
void input_matrix2(double **mat, int row_size, int col_size);

int main()
{
    int source[10]={1,2,1,3,4,2,1};
    int dist[10];
    int a,c,i,j,num;

    for (a=0; a<10; a++)
        dist[a]=0;

    for(i=0; i<10; i++)
    {
        num=source[i];
        dist[num]++;
    }
    for (a=1; a<10; a++)
        printf(" %d", dist[a]);

    int max_size=10;
    int max_num=10;
    int tmp;
    int *source2;
    int *dist2;
    source2=(int*)calloc(max_size, sizeof(int));
    dist2=(int*)calloc(max_num,sizeof(int));
    printf(" \n enter integer numbrs no larger than ");
    printf(" %d, otherwise entering ends \n", max_num+1);
    for(i=0; i<max_size; i++)
    {
        printf(" enter %d-th number : ",i);
        scanf("%d",&tmp);
        if(tmp>0 && tmp <=max_num)
        {
            source2[i]=tmp;
            printf(" %d is saved \n",tmp);
        }
        else{
            printf(" Outof range, loop ends \n");
            break;
        }
    }
    for ( i=0; i<max_size; i++)
    {
        num=source2[i];
        printf(" %d-th data= %d \n",i,num);
        dist2[num]++;
    }
    printf(" distribution is \n");
    for (a=1; a<max_num; a++)
        printf(" %d ",dist2[a]);
    
    int r, row_size=3;
    int col_size=2;
    double **matA, **matB, **matC;

    matA=(double**)calloc(row_size,sizeof(double*));
    for(int r=0; r<row_size; r++)
        matA[r]=(double*)calloc(row_size,sizeof(double));
    
    matB=(double**)calloc(col_size,sizeof(double*));
    for(r=0; r<col_size; r++)
        matB[r]=(double*)calloc(col_size,sizeof(double));

    matC=(double**)calloc(row_size, sizeof(double*));
    for(r=0; r<row_size; r++)
        matC[r]=(double*)calloc(row_size,sizeof(double));
    
    input_matrix2(matA, row_size,col_size);
    input_matrix2(matB,col_size,row_size);
    multiplyMatrix(matA,matB,matC,row_size,col_size,row_size);

    double matD[3][2];
    double (*matE)[2] =matD;

    input_matrix(matD, row_size, col_size);
    printf(" matE[0][0] is %lf \n",matE[0][0]);
}

void input_matrix(double mat[][2], int row_size, int col_size)
{
    int r,c;
    printf(" Enter numbers for %d by %d by %d matrix \n",row_size,col_size);
    for(r=0; r<row_size; r++)
        for(c=0; c<col_size; c++)
            scanf("%lf", &mat[r][c]);
}

void input_matrix2(double **mat, int row_size, int col_size)
{
    int r,c;
    printf(" Enter numbers for %d by %d matrix \n",row_size,col_size);
    for (r=0; r<row_size; r++)
        for(c=0; c<col_size; c++)
            scanf("%lf", &mat[r][c]);
}

void multiplyMatrix(double** mA, double** mB, double** mC,int a_row_size, int a_col_size, int b_col_size)
{
    int r,c,k;
    printf(" multiplication of the two matrices \n");
    for(r=0; r<a_row_size; r++)
    {
        printf("\n");
        for(c=0;c<b_col_size; c++)
        {
            mC[r][c]=0;
            for (k=0; k<a_col_size; k++)
                mC[r][c]+=mA[r][k]*mB[k][c];
            printf(" %lf ", mC[r][c]);
        }
    }
}