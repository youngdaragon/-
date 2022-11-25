#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_ELEV 1
#define MAX_FLOOR 5
#define QUEUE_SIZE 200
#define ELEV_SIZE 10
#define MAX_T 20
#define GEN_RATE 0.01
#define MAX(a,b)(((a)>(b))?(a):(b))
#define UP 0
#define DOWN 1
#define NONE 2
#define CLOSE 1
#define OPEN 0
#define ON 1
#define OFF 0

typedef struct _elevator
{
    int floor_dest_flag[MAX_FLOOR];
    int num_user_to_floor[MAX_FLOOR];
    int num_user;
    int current_floor;
    int direction;
    int gate_closed;
    int next_floor;
    int remain_time_to_unload;
    int remain_time_to_ride;
    int id;
} Elevator;

typedef struct _passenger
{
    int dest_floor;
    int arrival_time;
} Passenger;

int tot_wait_time=0;
int tot_num_user=0;
int tot_num_arrival=0;

void process_elevator_stop(Elevator* elevator, int t);
void wait_in_queue(Passenger** queue, int* front, int* rear, int floor_button[][2], int s, int d, int t);
int ride_elevator(Passenger* queue, Elevator* elevator, int* front,int* rear, int floor_button[][2], int c_f,int t, int dir);
void check_floors_and_move(Elevator* elevator, int floor_button[][2], int t);
void match_moving_elevator(Elevator* elevator, int s, int d);

int main()
{
    int floor_button[MAX_FLOOR][2]; // 0 up 1 down
    Passenger** floor_upqueue, **floor_downqueue;
    floor_upqueue=(Passenger**)malloc((MAX_FLOOR)*sizeof(Passenger*));
    for (int i=0; i<MAX_FLOOR;i++)
    {
        floor_upqueue[i]=(Passenger*)malloc(QUEUE_SIZE*sizeof(Passenger));
    }
    floor_downqueue=(Passenger**)malloc((MAX_FLOOR)*sizeof(Passenger*));
    for(int i=0; i<MAX_FLOOR; i++)
    {
        floor_downqueue[i]=(Passenger*)malloc(QUEUE_SIZE*sizeof(Passenger));
    }
    int* up_front, *down_front, *up_rear,*down_rear;
    up_front=(int*)calloc(MAX_FLOOR,sizeof(int));
    up_rear=(int*)calloc(MAX_FLOOR,sizeof(int));
    down_front=(int*)calloc(MAX_FLOOR,sizeof(int));
    down_rear=(int*)calloc(MAX_FLOOR,sizeof(int));

    Elevator* elevator;
    elevator=(Elevator*)calloc(NUM_ELEV,sizeof(Elevator));
    for(int i=0;i<NUM_ELEV;i++)
    {
        elevator[i].direction=NONE;
        elevator[i].id=i;
    }
    //int* event_queue; //saves next event_time, not used in this
    srand(time(NULL));
    for (int t=0; t<MAX_T; t++)
    {
        if((rand()%10000)/10000<GEN_RATE)
        {
            int s=rand()%(MAX_FLOOR);
            int d=rand()%(MAX_FLOOR);
            if(d>s)
            {
                wait_in_queue(floor_upqueue,up_front,up_rear,floor_button,s,d,t);
            }
            else if(d<s)
            {
                wait_in_queue(floor_downqueue,down_front,down_rear,floor_button,s,d,t);
            }
            match_moving_elevator(elevator,s,d); //match to moving elevator
        }
        for (int j=0; j<NUM_ELEV; j++)
        {
            if (elevator[j].gate_closed==CLOSE)//up down
            {
                if(elevator->direction==UP && elevator->current_floor<MAX_FLOOR)
                {
                    elevator->current_floor=elevator->current_floor+1;
                }
                else if (elevator->direction==DOWN && elevator->current_floor>0)
                {
                    elevator->current_floor=elevator->current_floor-1;
                }
                process_elevator_stop(&elevator[j],t);
            }
            else if(elevator[j].direction==NONE)
            {
                check_floors_and_move(&elevator[j],floor_button,t);
            }
            if (elevator[j].gate_closed==OPEN && elevator[j].remain_time_to_unload==0)
            {
                int c_f=elevator[j].current_floor;
                int num_ride=0;
                if (elevator[j].direction!=DOWN) //UP || elevator[j].direction==NONE)
                {
                    num_ride=ride_elevator(floor_upqueue[c_f],&elevator[j],up_front,up_rear,floor_button,c_f,t,UP);
                }
                if(num_ride==0 && elevator[j].direction!=UP)
                {
                    num_ride=ride_elevator(floor_downqueue[c_f], &elevator[j],down_front,down_rear,floor_button,c_f,t,DOWN);
                }
            }
            if (elevator[j].gate_closed==OPEN && elevator[j].remain_time_to_unload<=0 && elevator[j].remain_time_to_ride<=0)
            {
                elevator[j].gate_closed=CLOSE;
            }
            if(elevator[j].remain_time_to_unload<=0)
            {
                elevator[j].remain_time_to_ride--;
            }
            elevator[j].remain_time_to_unload--;
        }
    }
    printf(" tot_wait_time = %d num_user = %d Mean wait time = %f tot arrival = %d \n", tot_wait_time, tot_num_user, (float)(tot_wait_time / tot_num_user), tot_num_arrival);
}

void wait_in_queue(Passenger** queue, int *front, int *rear, int floor_button[][2], int s, int d, int t)
{
    if((rear[s]+1)%QUEUE_SIZE == front[s])
    {
        printf("   !!! QUEUE overlow front = %d rear = %d at floor %d \n", front[s], rear[s], s);
    }
    else
    {
        tot_num_arrival++;
        queue[s][rear[s]].dest_floor=d;
        queue[s][rear[s]].arrival_time =t;
        rear[s]=++rear[s]%QUEUE_SIZE;
        if (d>s)
        {
            floor_button[s][UP]=1;
        }
        else if(d<s)
        {
            floor_button[s][DOWN]=1;
        }
        printf("    at t = %d arrival queue, front = %d rear = %d at floor %d dest %d \n", t , front[s], rear[s], s, d);
    }
}

void match_moving_elevator(Elevator* elevator, int s, int d)
{
    if (s<d)
    {
        for (int j=0; j<NUM_ELEV; j++)
        {
            if (elevator[j].direction==UP && elevator[j].current_floor<s)
            {
                elevator[j].floor_dest_flag[d]=ON;
                printf(" Add dest %d to moving elevator %d in floor %d \n", d, elevator[j].id, elevator[j].current_floor);
            }
        }
    }
    else if(s>d)
    {
        for (int j=0; j<NUM_ELEV; j++)
        {
            if (elevator[j].direction==DOWN && elevator[j].current_floor>s)
            {
                elevator[j].floor_dest_flag[d]=ON;
                printf(" Add dest %d to moving elevator %d in floor %d \n", d, elevator[j].id, elevator[j].current_floor);
            }
        }
    }
}

void process_elevator_stop(Elevator * elevator, int t)
{
    for(int f=0; f<MAX_FLOOR; f++)
    {
        int t_f=elevator->floor_dest_flag[f];
        if (t_f==ON && elevator->current_floor==f)
        {
            elevator->floor_dest_flag[f]=OFF; //just arrived
            elevator->gate_closed=OPEN;
            elevator->remain_time_to_unload=MAX(1,(int)(elevator->num_user_to_floor[f]/2));
            elevator->num_user -= elevator->num_user_to_floor[f];
            printf(" Unload at time = %d, at %d-th floor, %d users from elevator %d \n", t, f, elevator->num_user_to_floor[f],elevator->id);
            elevator->num_user_to_floor[f]=0;
            break;
        }
    }
    elevator->direction=NONE;
    for (int f=0; f<MAX_FLOOR; f++)
    {
        int t_f=elevator->floor_dest_flag[f];
        if (t_f==ON)
        {
            if (f>elevator->current_floor)
            {
                elevator->direction=UP;
            }
            else if (f< elevator ->current_floor)
            {
                elevator->direction=DOWN;
            }
            break;
        }
    }
}

int ride_elevator(Passenger *queue, Elevator *elevator, int *front, int *rear, int floor_button[][2],int c_f, int t, int dir)
{
    int num=0;
    int wait_time;

    for (int i=0; i<ELEV_SIZE; i++)
    {
        if (front[c_f]==rear[c_f])//empty
        {
            floor_button[c_f][dir]=OFF;
            break;
        }
        else
        {
            int t_f = queue[front[c_f]].dest_floor;
            elevator->num_user_to_floor[t_f]++;
            elevator->num_user++;
            elevator->floor_dest_flag[t_f]=ON;
            wait_time=t - queue[front[c_f]].arrival_time;
            printf(" Riding elevator %d , at t= %d arrival time = %d wait_time = %d \n", elevator->id, t, queue[front[c_f]], wait_time);
            tot_wait_time=tot_wait_time+wait_time;
            front[c_f]=++front[c_f] % QUEUE_SIZE; //¨ù©ª¨ù¡©
            tot_num_user++;
            num++;
        }
    }
    elevator->remain_time_to_ride=MAX(1,(int)(num/2));
    elevator->gate_closed=OPEN;
    if((elevator->num_user+=num)==0)
    {
        elevator->direction=NONE;
    }
    return num;
}

void check_floors_and_move(Elevator* elevator,int floor_button[][2], int t)
{
    int c_f = elevator->current_floor;
    if (floor_button[c_f][UP]>0 || floor_button[c_f][DOWN]>0)
    {
        elevator->remain_time_to_unload=0;
        return;
    }
    for (int f=0; f<MAX_FLOOR; f++)
    {
        if (elevator->current_floor>f && (floor_button[f][UP >0 || floor_button[f][DOWN]>0]))
        {
            elevator->direction=DOWN;
            elevator->floor_dest_flag[f]=ON;
            elevator->gate_closed=CLOSE;
            printf(" start moving elevator %d to floor %d at t= %d \n", elevator->id, f ,t);
            break;
        }
        if (elevator->current_floor <f && (floor_button[f][UP]>0 || floor_button[f][DOWN]>0))
        {
            elevator->direction=UP;
            elevator->floor_dest_flag[f]=ON;
            elevator->gate_closed=CLOSE;
            printf(" start moving elevator %d to floor %d at t= %d \n", elevator->id, f ,t);
            break;
        }
    }
}