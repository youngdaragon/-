#include <stdio.h>
#include <stdlib.h>

typedef struct _maze
{
    int x,y;
    char value;
    int visited; // 1 visited, 0 unvisited
} Maze;

typedef struct _stack
{
    int top;
    int fork_x[10], fork_y[10];
} StackArray;

int *init_Maze(Maze maze[][10]);
int find_path(Maze maze[][10], StackArray *fork_stack, int *start_coordi);
int* find_next_point(Maze(*maze)[10], int* prev_coordi, StackArray *fork_stack);

int main()
{
    Maze maze[10][10];
    StackArray fork_stack;
    int *start_coordi;
    fork_stack.top=-1;
    start_coordi=init_Maze(maze);
    find_path(maze,&fork_stack,start_coordi);
}

int *init_Maze(Maze maze[][10])
{
    int i,j;
    int *start_coordi;
    start_coordi=(int*)malloc(2*sizeof(int));

    for(i=0; i<10; i++)
    {
        for(j=0; j<10; j++)
        {
            maze[i][j].value='1';
            maze[i][j].visited='0';
        }
    }
    for(j=0; j<5; j++)
        maze[1][j].value='0';
    for(j=5; j<10; j++)
        maze[3][j].value='0';
    for(i=2; i<8;i++)
    {
        maze[i][2].value='0';
        maze[i][7].value='0';
    }
    maze[1][5].value='0';
    maze[2][5].value='0';

    for(i=0; i<10; i++)
    {
        for(j=0;j<10;j++)
            printf(" %c", maze[i][j].value);
        printf("\n");
    }
    start_coordi[0]=1;
    start_coordi[1]=0;
    return start_coordi;
}

int find_path(Maze maze[][10], StackArray *fork_stack, int *start_coordi)
{
    int *next_coordi; 
    int *prev_coordi;
    next_coordi=(int*)malloc(2*sizeof(int));
    prev_coordi=(int*)malloc(2*sizeof(int));
    prev_coordi[0]=start_coordi[0];
    prev_coordi[1]=start_coordi[1];

    while(1)
    {
        printf("visit : ( %d %d )\n",prev_coordi[0],prev_coordi[1]);
        next_coordi=find_next_point(maze,prev_coordi,fork_stack);
        //push to stack if fork & select a next point
        if (next_coordi[0]>=9 || next_coordi[1]>=9)
        {
            printf(" Exit point (%d, %d) reached, \n",next_coordi[0],next_coordi[1]);
            //print_path();
            return 1;
        }
        else //proceed to the next point
        {
            prev_coordi=next_coordi;
        }
    }
    return 1;
}

int * find_next_point(Maze (*maze)[10], int *prev_coordi, StackArray *fork_stack)
{
    int x= prev_coordi[0];
    int y= prev_coordi[1];
    int next_x,next_y;
    int *coordi;
    coordi=(int*)malloc(2*sizeof(int));
    int fork_flag=0;

    for (int i=0; i<4; i++)
    {
        if (i==0)
        {
            next_x=x-1;
            next_y=y;
        }
        else if (i==1)
        {
            next_x=x;
            next_y=y+1;
        }
        else if (i==2)
        {
            next_x=x+1;
            next_y=y;
        }
        else if (i==3)
        {
            next_x=x;
            next_y=y-1;
        }
        if (next_x<0 || next_y<0)
        {
            continue; //boundary
        }
        if (next_x<0 || next_y<0)
        {
            continue; //dead end
        }
        else if(maze[next_x][next_y].value=='1')
        {
            continue; //dead end
        }
        else if(maze[next_x][next_y].visited==1)
        {
            continue; //value = 0
        }
        else if(fork_flag==0)
        {
            maze[next_x][next_y].visited=1;
            coordi[0]=next_x;
            coordi[1]=next_y;
            fork_flag=1;
        }
        else if(fork_flag==1)//value=0, visited=0
        {
            fork_stack->top++;
            fork_stack->fork_x[fork_stack->top]=x;
            fork_stack->fork_y[fork_stack->top]=y;
            printf(" stack push( %d %d ) top = %d \n", x, y, fork_stack->top);
            break;
        }
    }
    if(fork_flag==1) //found next point
    {
        return coordi;
    }
    else //dead end
    {
        int top=fork_stack->top;
        if (top>=0)
        {
            coordi[0]=fork_stack->fork_x[top];
            coordi[1]=fork_stack->fork_y[top];
            fork_stack->top--;
            printf(" pop stack (%d %d )\n",coordi[0],coordi[1]);
            return coordi;

        }
    }
}