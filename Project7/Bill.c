// Billy Reid's Code

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

typedef void(*fptr);

enum Computers{
    PC, MAC, OSs
};
int Computer;

#define MAX_PROCESSES 10

#define INITIAL_FUEL 50
#define INITIAL_DISTANCE 0
#define INITIAL_BOMBS 6

enum tExitCodes {
    EXIT_CRASHED, EXIT_SUCCEED, EXITS
};

int PID[MAX_PROCESSES], ChildPID;
int Terminal[MAX_PROCESSES];
int Terminals = 0;
int Fuel, Distance;
int Processes, Process;
FILE *fpt[MAX_PROCESSES];
int Bombs;

void AlarmHandler (int signum){
    static int display, add_distance;

    if (--Fuel < 0) Fuel = 0;

    if (++add_distance == 2){
        add_distance = 0;
        if (Bombs == 0) Distance --;
        else Distance++;
    }

    if (++display == 3){
        display = 0;
        fprintf(fpt[Process], "T%d:P%d Bomber %d: Fuel = %d gal, Bombs = %d, Distance = %d mi.\n", Terminal[Process], ChildPID%10, Process, Fuel, Bombs, Distance);
    }

    if (Fuel == 0){
        fprintf(fpt[Process], "Bomber %d Down. Pick us up in the drink...\n", Process);
        exit (EXIT_CRASHED);
    }

    else if (Fuel < 5){
        fprintf(fpt[Process], "Mayday, Mayday. This is Bomber %d. Our fuel is almost gone.\n", Process);
    }

    if ((Bombs == 0) && (Distance == 0)){
        fprintf(fpt[Process], "Mission Complete!\n");
        if (Fuel <= 5) fprintf(fpt[Process], "Man that was close!!\n");
        exit (EXIT_SUCCEED);
    }

    alarm(1);
}

void Bomb(int signum){
    if (Bombs > 0){
        fprintf(fpt[Process], "Bomber %d dropping ordinance.\n", Process);
        if (--Bombs == 0){
            fprintf(fpt[Process], "Bomber %d returning to base.\n", Process);
        }
    }
    else fprintf(fpt[Process], "Bomber %d has no ordinance.\n", Process);
}

void Refuel(int signum){
    Fuel = 50;
    fprintf(fpt[Process], "Plane %d to base: Thank you for more fuel!!\n", Process);
}

void DoChildStuff(void){
    ChildPID = getpid();
    signal(SIGALRM, (fptr)AlarmHandler);
    signal(SIGUSR2, (fptr)Bomb);
    signal(SIGUSR1, (fptr)Refuel);
    Fuel = INITIAL_FUEL;
    Bombs = INITIAL_BOMBS;
    Distance = INITIAL_DISTANCE;
    alarm(1);
    while(1){
    }
}

void Monitor(void){
    char strIn[10];
    int p;

    Processes = 3;
    while (Processes > 0){
        scanf("%s", strIn);
        fflush(stdin);

        if (strlen(strIn) == 2){
            p = atoi(&strIn[1]);
            if ((p > 0) && (p < 4))
                switch (strIn[0]){
                    case 'k':
                        kill(PID[p], 1);
                        printf("Child %d Killed\n", p);
                        Processes--;
                        break;
                    case 'r':
                        kill(PID[p], SIGUSR1);
                        break;
                    case 'b':
                        kill(PID[p], SIGUSR2);
                        break;
                }
            else printf("Not a valid Bomber Number...\n");
        }
        else if (strlen(strIn) == 1){
            if (strIn[0] == 'q'){
                for (p = 1; p <= 3; p++){
                    kill(PID[p], 1);
                    Processes--;
                }
            }
        }
    }
}

void PrintExitStatus(int p, int status){
    char exit_byte, signal_byte, core_bit;

    exit_byte = status >> 8;
    signal_byte = status & 0x7F;
    core_bit = status & 0x80;

    if (exit_byte == EXIT_SUCCEED)
        printf("Processs %d mission a success!\n", p%10);
    else
        printf("Process %d mission a failure...\n", p%10);
}

void doParentStuff(void){
    int i;
    int child_status[MAX_PROCESSES], wait_ret[MAX_PROCESSES];

    wait_ret[1] = wait(&child_status[1]);
    wait_ret[2] = wait(&child_status[2]);
    wait_ret[3] = wait(&child_status[3]);

    for (i=1; i <= 3; i++){
        PrintExitStatus(wait_ret[i], child_status[i]);
    }
}

const char *strDir[OSs] = {"/dev/pts/", "/dev/ttys"};

void FindTerminals(void){
    int i;
    char strDev[100];

    for (i=0; i<100; i++){
        if (Computer == MAC)
            sprintf(strDev, "%s%03d", strDir[Computer], i);
        else
            sprintf(strDev, "%s%d", strDir[PC], i);
        printf("Opening %s...\n", strDev);
        fflush(stdout);

        if ((fpt[Terminals] = fopen(strDev, "w")) == NULL){
            exit(0);
        }

        else{
            Terminal[Terminals] = i;
            Terminals++;
        }
    }

    printf("Terminals Available...\n");
    for (i=0; i < Terminals; i++){
        printf("Terminal[%d] = %d\n", i, Terminal[i]);
    }
    printf("\n");
}

void CloseTerminals(void){
    int i;
    for (i=0; i<Terminals; i++){
        fclose(fpt[i]);
    }
}

int main(int argc, char *argv[]){
    int i;
    int Parent = 1;

    if (argc ==2){
        if(strcmp(argv[1], "MAC") == 0)
            Computer = MAC;
        else
            Computer = PC;
    }

    FindTerminals();

    PID[Process = 0] = getpid();

    for (i =1; i<=3; i++){
        if (Parent){
            PID[Process = i] = fork();
            if (PID[i] == 0){
                Parent = 0;
                printf("Doing Child Stuff %d\n", i);
                DoChildStuff();
            }
            else{
                printf("Child %d Created\n", i);
            }
        }
    }

    if (Parent){
        PID[4] = fork();
        if (PID[4] == 0){
            Monitor();
        }
        else{
            doParentStuff();
            kill(PID[4], 1);
            CloseTerminals();
        }
    }
return 0;
}
