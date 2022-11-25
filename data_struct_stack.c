#include<stdio.h>
#include<stdlib.h>

typedef struct _maze
{
    int x,y;
    char value;
    int visited; // 1 visited, 0 unvisited
} Maze; //미로 기본틀 설정

typedef struct _stack
{
    int top;
    int fork_x[100],fork_y[100];
} StackArray; // 미로를 빠져나가기 위한 스택 기본설정

typedef struct _stack1
{
    int top1;
    int path_x[100],path_y[100];
} StackPath; // 경로를 찾기위한 스택 기본설정

int *init_Maze(Maze maze[][10]);
int find_path(Maze maze[][10],StackArray *fork_stack,int *start_coordi,StackPath *path_stack);
int* find_next_point(Maze(*maze)[10],int* prev_coordi, StackArray *fork_stack,StackPath *path_stack);
void print_stack(StackPath *path_stack);

int main()
{
    Maze maze[10][10]; //미로 크기 설정
    StackArray fork_stack;
    StackPath path_stack;
    int* start_coordi;
    fork_stack.top =-1; //fork_stack top -1 설정
    path_stack.top1=-1; //path_stack top -1 설정

    start_coordi=init_Maze(maze); // 미로만드는 함수에 대입
    find_path(maze,&fork_stack,start_coordi,&path_stack); //경로찾는 함수에 대입
}

void print_stack(StackPath *path_stack)
{
    int i;
    int top1=path_stack->top1;
    for (i=0;i<=top1;i++)
    {
        printf("(%d, %d) \n",path_stack->path_x[i],path_stack->path_y[i]);
    }
    //stack에 있는 것을 print 해주는 함수
}

int *init_Maze(Maze maze[][10])
{
    int i,j;
    int* start_coordi;
    start_coordi=(int*)malloc(2*sizeof(int)); //메모리 동적 할당

    for(i=0;i<10;i++)
        for(j=0;j<10;j++)
        {
            maze[i][j].value='1';
            maze[i][j].visited='0'; //미로의 첫 visited는 0으로 설정
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
    maze[start_coordi[0]][start_coordi[1]].visited=1; //시작점의 visited가 1이 되도록 설정
    return start_coordi;
}

int find_path(Maze maze[][10], StackArray *fork_stack, int *start_coordi,StackPath *path_stack)
{
    int *next_coordi; //다음 좌표값
    int *prev_coordi; //이전 좌표값
    next_coordi=(int*)malloc(2*sizeof(int));
    prev_coordi=(int*)malloc(2*sizeof(int));
    prev_coordi[0]=start_coordi[0]; //초기x값 대입
    prev_coordi[1]=start_coordi[1]; //초기y값 대입

    while(1)
    {
        printf("visit : ( %d %d)\n", prev_coordi[0],prev_coordi[1]); //방문한 포인트 찍기
        next_coordi=find_next_point(maze,prev_coordi,fork_stack,path_stack); //다음에 가는 좌표 구하기
        int top1=path_stack->top1; //경로찾는 stack top 추출
        //push to stack if fork & select a next point
        if (next_coordi[0]>=9 || next_coordi[1]>=9) //마지막 행,열에 도달하면 탈출
        {
            printf("Exit point (%d %d) reached, \n",next_coordi[0],next_coordi[1]);//마지막 좌표값 추출
            printf("\n path point is \n");
            printf("(%d, %d)\n",start_coordi[0],start_coordi[1]);
            print_stack(path_stack);
            return 1;
        }
        else // proceed to the next point
        {
            prev_coordi=next_coordi; //prev를 next로 바꾸는 코드
        }
    }
    return 1;
}

int * find_next_point(Maze(*maze)[10],int *prev_coordi, StackArray *fork_stack,StackPath *path_stack)
{
    int x=prev_coordi[0];//입력된 x
    int y=prev_coordi[1];//입력된 y
    int next_x,next_y;//다음 좌표값
    int *coordi;//좌표값
    coordi=(int*)malloc(2*sizeof(int));
    int fork_flag=0;//갈림길 표시 숫자

    for(int i=0;i<=4;i++)
    {
        if (i==0)
        {
            next_x=x-1;
            next_y=y;
        } // 한칸 위로 움직임
        else if(i==1)
        {
            next_x=x;
            next_y=y+1;
        } //한칸 오른쪽으로 움직임
        else if(i==2)
        {
            next_x=x+1;
            next_y=y;
        }//한칸 아래로 움직임
        else if(i==3)
        {
            next_x=x;
            next_y=y-1;
        }//한칸 왼쪽으로 움직임
        if (next_x<0 || next_y<0)
        {
            continue; //boundary
        }//0보다 작은 좌표가 나오면 for loop로 돌아감
        else if(next_x>=10 || next_y>=10)
        {
            continue; //boundary
        }//10보다 크면 반복문으로 돌아감
        else if(maze[next_x][next_y].value=='1')
        {
            continue; //dead end
        }//막힌벽이면 좌표면 돌아감
        else if(maze[next_x][next_y].visited==1)
        {
            continue; //dead end
        }//방문했던 좌표면 돌아감
        else if(fork_flag==0)
        {
            maze[next_x][next_y].visited=1;
            coordi[0]=next_x;
            coordi[1]=next_y;
            fork_flag=1;
        }//만약 전진했을때 위 해당사항이 없으면 fork_flag를 1로 설정하고 좌표는 다음값으로 설정
        else if(fork_flag==1) //value=0,visited=0
        {
            fork_stack->top++;
            fork_stack->fork_x[fork_stack->top]=x;
            fork_stack->fork_y[fork_stack->top]=y;
            printf("fork stack push (%d %d) top= %d \n", x, y, fork_stack->top);
            break;
        }//만약 앞서간 경로 이외에도 0이 있다면(갈림길이라면) fork_stack에 좌표를 쌓아둠 
    }
    if (fork_flag==1) //found next point
    {
        path_stack->top1++;
        path_stack->path_x[path_stack->top1]=coordi[0];
        path_stack->path_y[path_stack->top1]=coordi[1];
        printf("path stack push (%d %d) top= %d \n", coordi[0], coordi[1], path_stack->top1);
        return coordi;
        //지나갔던 경로들을 stack_path에 쌓아둠
    }
    else //dead end
    { //만약 벽을 만났을 때 pop을 이용한 돌아가는 코드
        int top =fork_stack->top;
        int top1=path_stack->top1; // 각 stack들에 top을 추출
        if(top>=0) //top이 0보다 클때만 적용
        {
            coordi[0]=fork_stack->fork_x[top];
            coordi[1]=fork_stack->fork_y[top];
            fork_stack->top--; //좌표값에 top의 좌표값을 넣어주고 stack에서 top의 수를 낯춰 없애준다. 
            printf("pop fork_stack (%d %d) \n", coordi[0], coordi[1]);
            path_stack->top1--; //좌표값에 top의 좌표값을 넣어주고 stack에서 top의 수를 낯춰 없애준다. 
            printf("pop path_stack (%d %d) \n", coordi[0], coordi[1]);
            return coordi;
        }
    }
}
