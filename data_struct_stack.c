#include<stdio.h>
#include<stdlib.h>

typedef struct _maze
{
    int x,y;
    char value;
    int visited; // 1 visited, 0 unvisited
} Maze; //�̷� �⺻Ʋ ����

typedef struct _stack
{
    int top;
    int fork_x[100],fork_y[100];
} StackArray; // �̷θ� ���������� ���� ���� �⺻����

typedef struct _stack1
{
    int top1;
    int path_x[100],path_y[100];
} StackPath; // ��θ� ã������ ���� �⺻����

int *init_Maze(Maze maze[][10]);
int find_path(Maze maze[][10],StackArray *fork_stack,int *start_coordi,StackPath *path_stack);
int* find_next_point(Maze(*maze)[10],int* prev_coordi, StackArray *fork_stack,StackPath *path_stack);
void print_stack(StackPath *path_stack);

int main()
{
    Maze maze[10][10]; //�̷� ũ�� ����
    StackArray fork_stack;
    StackPath path_stack;
    int* start_coordi;
    fork_stack.top =-1; //fork_stack top -1 ����
    path_stack.top1=-1; //path_stack top -1 ����

    start_coordi=init_Maze(maze); // �̷θ���� �Լ��� ����
    find_path(maze,&fork_stack,start_coordi,&path_stack); //���ã�� �Լ��� ����
}

void print_stack(StackPath *path_stack)
{
    int i;
    int top1=path_stack->top1;
    for (i=0;i<=top1;i++)
    {
        printf("(%d, %d) \n",path_stack->path_x[i],path_stack->path_y[i]);
    }
    //stack�� �ִ� ���� print ���ִ� �Լ�
}

int *init_Maze(Maze maze[][10])
{
    int i,j;
    int* start_coordi;
    start_coordi=(int*)malloc(2*sizeof(int)); //�޸� ���� �Ҵ�

    for(i=0;i<10;i++)
        for(j=0;j<10;j++)
        {
            maze[i][j].value='1';
            maze[i][j].visited='0'; //�̷��� ù visited�� 0���� ����
        }
    for(j=0;j<5;j++)
            maze[1][j].value='0';
    for(j=5;j<10;j++)
            maze[3][j].value='0';
    for(i=2;i<8;i++)
        {
            maze[i][2].value='0';
            maze[i][7].value='0';
        }
    maze[1][5].value='0';
    maze[2][5].value='0';

    for(i=0;i<10;i++)
        {
            for(j=0;j<10;j++)
            {
                printf(" %c ",maze[i][j].value);
            }
            printf(" \n");
        }
    start_coordi[0]=1;
    start_coordi[1]=0;
    maze[start_coordi[0]][start_coordi[1]].visited=1; //�������� visited�� 1�� �ǵ��� ����
    return start_coordi;
}

int find_path(Maze maze[][10], StackArray *fork_stack, int *start_coordi,StackPath *path_stack)
{
    int *next_coordi; //���� ��ǥ��
    int *prev_coordi; //���� ��ǥ��
    next_coordi=(int*)malloc(2*sizeof(int));
    prev_coordi=(int*)malloc(2*sizeof(int));
    prev_coordi[0]=start_coordi[0]; //�ʱ�x�� ����
    prev_coordi[1]=start_coordi[1]; //�ʱ�y�� ����

    while(1)
    {
        printf("visit : ( %d %d)\n", prev_coordi[0],prev_coordi[1]); //�湮�� ����Ʈ ���
        next_coordi=find_next_point(maze,prev_coordi,fork_stack,path_stack); //������ ���� ��ǥ ���ϱ�
        int top1=path_stack->top1; //���ã�� stack top ����
        //push to stack if fork & select a next point
        if (next_coordi[0]>=9 || next_coordi[1]>=9) //������ ��,���� �����ϸ� Ż��
        {
            printf("Exit point (%d %d) reached, \n",next_coordi[0],next_coordi[1]);//������ ��ǥ�� ����
            printf("\n path point is \n");
            printf("(%d, %d)\n",start_coordi[0],start_coordi[1]);
            print_stack(path_stack);
            return 1;
        }
        else // proceed to the next point
        {
            prev_coordi=next_coordi; //prev�� next�� �ٲٴ� �ڵ�
        }
    }
    return 1;
}

int * find_next_point(Maze(*maze)[10],int *prev_coordi, StackArray *fork_stack,StackPath *path_stack)
{
    int x=prev_coordi[0];//�Էµ� x
    int y=prev_coordi[1];//�Էµ� y
    int next_x,next_y;//���� ��ǥ��
    int *coordi;//��ǥ��
    coordi=(int*)malloc(2*sizeof(int));
    int fork_flag=0;//������ ǥ�� ����

    for(int i=0;i<=4;i++)
    {
        if (i==0)
        {
            next_x=x-1;
            next_y=y;
        } // ��ĭ ���� ������
        else if(i==1)
        {
            next_x=x;
            next_y=y+1;
        } //��ĭ ���������� ������
        else if(i==2)
        {
            next_x=x+1;
            next_y=y;
        }//��ĭ �Ʒ��� ������
        else if(i==3)
        {
            next_x=x;
            next_y=y-1;
        }//��ĭ �������� ������
        if (next_x<0 || next_y<0)
        {
            continue; //boundary
        }//0���� ���� ��ǥ�� ������ for loop�� ���ư�
        else if(next_x>=10 || next_y>=10)
        {
            continue; //boundary
        }//10���� ũ�� �ݺ������� ���ư�
        else if(maze[next_x][next_y].value=='1')
        {
            continue; //dead end
        }//�������̸� ��ǥ�� ���ư�
        else if(maze[next_x][next_y].visited==1)
        {
            continue; //dead end
        }//�湮�ߴ� ��ǥ�� ���ư�
        else if(fork_flag==0)
        {
            maze[next_x][next_y].visited=1;
            coordi[0]=next_x;
            coordi[1]=next_y;
            fork_flag=1;
        }//���� ���������� �� �ش������ ������ fork_flag�� 1�� �����ϰ� ��ǥ�� ���������� ����
        else if(fork_flag==1) //value=0,visited=0
        {
            fork_stack->top++;
            fork_stack->fork_x[fork_stack->top]=x;
            fork_stack->fork_y[fork_stack->top]=y;
            printf("fork stack push (%d %d) top= %d \n", x, y, fork_stack->top);
            break;
        }//���� �ռ��� ��� �̿ܿ��� 0�� �ִٸ�(�������̶��) fork_stack�� ��ǥ�� �׾Ƶ� 
    }
    if (fork_flag==1) //found next point
    {
        path_stack->top1++;
        path_stack->path_x[path_stack->top1]=coordi[0];
        path_stack->path_y[path_stack->top1]=coordi[1];
        printf("path stack push (%d %d) top= %d \n", coordi[0], coordi[1], path_stack->top1);
        return coordi;
        //�������� ��ε��� stack_path�� �׾Ƶ�
    }
    else //dead end
    { //���� ���� ������ �� pop�� �̿��� ���ư��� �ڵ�
        int top =fork_stack->top;
        int top1=path_stack->top1; // �� stack�鿡 top�� ����
        if(top>=0) //top�� 0���� Ŭ���� ����
        {
            coordi[0]=fork_stack->fork_x[top];
            coordi[1]=fork_stack->fork_y[top];
            fork_stack->top--; //��ǥ���� top�� ��ǥ���� �־��ְ� stack���� top�� ���� ���� �����ش�. 
            printf("pop fork_stack (%d %d) \n", coordi[0], coordi[1]);
            path_stack->top1--; //��ǥ���� top�� ��ǥ���� �־��ְ� stack���� top�� ���� ���� �����ش�. 
            printf("pop path_stack (%d %d) \n", coordi[0], coordi[1]);
            return coordi;
        }
    }
}
