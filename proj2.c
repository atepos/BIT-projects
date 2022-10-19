//////////////////
//
//  autor: Petr Kaska 
//  email: xkaska01@vut.fitbr.cz
//  
//  IOS-projekt 2
//
//////////////////

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <math.h>

///////////////////////////////////////////////////
//
//      definovani globalnich promnenych a struktur
//
///////////////////////////////////////////////////

#define MAXTIME 1000
#define MINTIME 0
#define DECIMAL 10

int *move_counter;
int *O_count;
int *H_count;
int *Molecul_count;
int *atoms_O_in_process;
int *atoms_H_in_process;
int *barrier_count;
int *moleculs_should_be;
int *creating_molecule;

FILE *file;

#define ALL_SEMAP "xkaska01-IOS2-all"
#define WATER_SEMAP "xkaskas01-IOS2-water"
#define HYDROGEN_SEMAP "xkaska01-IOS2-hydrogen"
#define MOLECULE_SEMAP "xkaska01-IOS2-molecule"
#define BARRIER_SEMAP "xkaska01-IOS2-barrier"
#define WAIT "xkaska01-IOS2-wait"
#define CREATED "xkaska01-IOS2-created"
#define MUTEXXXX "xkaska01-IOS2-second_mutex"

sem_t *mutex_2 = NULL;
sem_t *mutex = NULL;
sem_t *oxyQueue = NULL;
sem_t *hydrogenQueue = NULL;
sem_t *barrier = NULL;
sem_t *wait_for = NULL;
sem_t *molecule_created = NULL;


typedef struct arguments{
    int NO;
    int NH;
    int TI;
    int TB;
}args;

enum ERROR_CODE
{
    ARG_COUNT_ERR = 1,
    ARG_DATATYPE_ERR,
    TIME_ARG_DATATYPE_ERR,
    FILE_OPEN_ERR,
    SEMAPHORE_ERR,
    FORK_ERR,
};

///////////////////////////////////////////////////
//
//          vytvoreni funkci 
//
///////////////////////////////////////////////////

void clean_mem()//funkce na vycisteni sdilene pameti
{
    munmap(move_counter, sizeof(*move_counter));
    munmap(O_count, sizeof(*O_count));
    munmap(H_count, sizeof(*H_count));
    munmap(Molecul_count, sizeof(*Molecul_count));
    munmap(atoms_O_in_process, sizeof(*atoms_O_in_process));
    munmap(atoms_H_in_process, sizeof(*atoms_H_in_process));
    munmap(barrier_count, sizeof(*barrier_count));
    munmap(moleculs_should_be, sizeof(*moleculs_should_be));
    munmap(creating_molecule, sizeof(*creating_molecule));
}


void clean_sem()//funkce na zniceni semapforu
{
    sem_close(mutex);
    sem_unlink(ALL_SEMAP);

    sem_close(oxyQueue);
    sem_unlink(WATER_SEMAP);

    sem_close(hydrogenQueue);
    sem_unlink(HYDROGEN_SEMAP);

    sem_close(barrier);
    sem_unlink(MOLECULE_SEMAP);

    sem_close(wait_for);
    sem_unlink(WAIT);

    sem_close(molecule_created);
    sem_unlink(CREATED);

    sem_close(mutex_2);
    sem_unlink(MUTEXXXX);

}

void throw_err(const int errorCode)//funkce na ukonceni programu v pripade chyby a vraceni chybove hlasky
{
    switch (errorCode)
    {
    case ARG_COUNT_ERR:
            fprintf(stderr,"ERROR: nespravny pocet argumentu\n");
        break;
    
    case ARG_DATATYPE_ERR:
            fprintf(stderr,"ERROR: nespravny datovy typ\n");
        break;
    
    case TIME_ARG_DATATYPE_ERR:
            fprintf(stderr,"ERROR: nespravny datovy typ nebo moc velke nebo male cislo na vstupu\n");        
        break;

    case FILE_OPEN_ERR:
            fprintf(stderr,"ERROR: soubor nelze otevrit\n");        
        break;

    case SEMAPHORE_ERR:
            fprintf(stderr,"ERROR: semafor nebyl vytvoren\n");
        break;  
    
    case FORK_ERR:
            fprintf(stderr,"ERROR: chyba pri vytvareni child procesu\n");
        break;

    default:
        break;
    }
    clean_mem();
    clean_sem();
    fclose(file);
    exit(1);
}

void random_wait(int times)//funkce na cekani. Napriklad pri vytvareni molekuly nebo nez se molekula zaradi do fronty 
{
    int result = (rand() % times);
    result = result * 1000;
    usleep(result);
}


void semaphores_init()//inicializace semaforu
{
    mutex_2 = sem_open(MUTEXXXX, O_CREAT | O_EXCL, 0666, 1);//semafor, ktery slouzi jako pojistka aby se nedostali 3 vodiky do barriery
    if (mutex_2 == SEM_FAILED) 
    {
        throw_err(SEMAPHORE_ERR);
    }

    mutex = sem_open(ALL_SEMAP, O_CREAT | O_EXCL, 0666, 1);//semafor, ktery slouzi k tomu aby mohl jenom jeden proces v jednu chvili provadet nejakou funkci 
    if (mutex == SEM_FAILED) 
    {
        throw_err(SEMAPHORE_ERR);
    }

    oxyQueue = sem_open(WATER_SEMAP, O_CREAT | O_EXCL, 0666, 0);//semafor, ktery pousti vzdy prave 1 kyslik k tvorbe molekuly 
    if (oxyQueue == SEM_FAILED) 
    {
        throw_err(SEMAPHORE_ERR);
    }

    hydrogenQueue = sem_open(HYDROGEN_SEMAP, O_CREAT | O_EXCL, 0666, 0);//semafore, ktery pousti prave 2 vodiky k tvorbe molekuly
    if (hydrogenQueue == SEM_FAILED) 
    {
        throw_err(SEMAPHORE_ERR);
    }

    barrier = sem_open(MOLECULE_SEMAP, O_CREAT | O_EXCL, 0666, 3);//semafor, ktery ze zacatku pusti nahodne 2 atomy vodiku a 1 kysliku, ale nasledne ceka a pousti atomy prave po 3
    if (barrier == SEM_FAILED) 
    {
        throw_err(SEMAPHORE_ERR);
    }

    wait_for = sem_open(WAIT, O_CREAT | O_EXCL, 0666, 1);//semafor, ktery ceka v mainu po vytvoreni child procesu na jejich ukonceni -> pasivni cekani.
    if (wait_for == SEM_FAILED) 
    {
        throw_err(SEMAPHORE_ERR);
    }

    molecule_created = sem_open(CREATED, O_CREAT | O_EXCL, 0666, 0);//semafor, ktery zarucuje, ze nejdriv vsechny 3 atomy zacnou vytvaret molekulu a az potom ji vytvori -> zabranuje nahodnemu vytvoreni
    if (molecule_created == SEM_FAILED) 
    {
        throw_err(SEMAPHORE_ERR);
    }
}

void shared_mem_init()//inicializace sdilenych pameti
{
    move_counter = mmap(NULL,sizeof(*move_counter), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);//sdilena promenna, ktera indexuje radky
    *move_counter = 1;

    O_count = mmap(NULL,sizeof(*O_count), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);//sdilena promenna, ktera indexuje pocet kysliku
    *O_count = 0;

    H_count = mmap(NULL,sizeof(*H_count), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);//sdilena promenna, ktera indexuje pocet vodiku
    *H_count = 0;

    Molecul_count = mmap(NULL,sizeof(*Molecul_count), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);//sdilena promenna, ktera indexuje pocet molekul
    *Molecul_count = 1;

    atoms_O_in_process = mmap(NULL,sizeof(*atoms_O_in_process), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);//pomocna sdilena promenna, ktera pocita kysliky, ktere jsou pripraveny vytvorit molekulu
    *atoms_O_in_process = 0;

    atoms_H_in_process = mmap(NULL,sizeof(*atoms_H_in_process), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);//pomocna sdilena promenna, ktera pocita vodiky, ktere jsou pripraveny vytvorit molekulu
    *atoms_H_in_process = 0;

    barrier_count = mmap(NULL,sizeof(*barrier_count), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);//pomocna sdilena promenna, ktera otevira a zavira barrieru
    *barrier_count = 0;

    moleculs_should_be = mmap(NULL,sizeof(*moleculs_should_be), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);//pomocna sdilena promenna, ktera uchovava predem vypocitany ocekavany pocet molekul
    *moleculs_should_be = 0;

    creating_molecule = mmap(NULL,sizeof(*creating_molecule), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);//pomocna sdilena promenna, ktera otvira a zavira barrieru pri prechodu z creating molecule do molecule created 
    *creating_molecule = 0;
}


void process_H2O(args arguments, int number_of_process_O)//samotna funkce kysliku, ktera spolu s funkci vodiku vytvareji molekulu vody
{

if((*Molecul_count) == (*moleculs_should_be))
    {
        sem_post(hydrogenQueue);
        sem_post(hydrogenQueue);
        sem_post(oxyQueue);
    }
    (*O_count)++;

    sem_wait(mutex);
    fprintf(file, "%d: O %d: started\n",(*move_counter)++ ,number_of_process_O);
    fflush(file);
    sem_post(mutex);
    
   
    if(arguments.TI != 0)
    {
        srand(time(NULL) % getpid());
        random_wait(arguments.TI);
    }
   
    sem_wait(mutex);
    fprintf(file, "%d: O %d: going to queue\n",(*move_counter)++ ,number_of_process_O);
    fflush(file);
    sem_post(mutex);

    sem_wait(mutex_2);
    (*atoms_O_in_process)++;
    if((*atoms_H_in_process) >= 2)
    {

        sem_post(hydrogenQueue);
        sem_post(hydrogenQueue);
        sem_post(oxyQueue);

        (*atoms_H_in_process) -= 1;
        (*atoms_H_in_process) -= 1;
        (*atoms_O_in_process) -= 1;
    
    }else{
        sem_post(mutex_2);
    }

    sem_wait(oxyQueue);
    
    sem_wait(barrier);


    if((*Molecul_count) == (*moleculs_should_be))
    {    
        sem_post(barrier);
        sem_post(hydrogenQueue);
        sem_post(oxyQueue);
              
        sem_wait(mutex);
        fprintf(file,"%d: O %d: not enough H\n",(*move_counter)++, number_of_process_O);
        fflush(file);
        sem_post(mutex);

        sem_post(wait_for);

        exit(0);

    }


    sem_wait(mutex);
    fprintf(file, "%d: O %d: creating molecule %d\n",(*move_counter)++ ,number_of_process_O, (*Molecul_count));
    fflush(file);
    sem_post(mutex);

    
    (*creating_molecule)++;
    if((*creating_molecule) >= 3)
    {
        for (int i = 0; i < 3; i++)
        {
            sem_post(molecule_created);
        }
        (*creating_molecule) = 0;
    }

    

    if(arguments.TB != 0)
    {
         random_wait(arguments.TI);
    }

    sem_wait(molecule_created);

    sem_wait(mutex);
    fprintf(file, "%d: O %d: molecule %d created\n",(*move_counter)++ ,number_of_process_O, (*Molecul_count));
    fflush(file);
    sem_post(mutex);

    (*barrier_count)++;
    if(*barrier_count >= 3)
    {
        (*Molecul_count)++;
        for (int i = 0; i < 3; i++)
        {
            sem_post(barrier);
        }
        (*barrier_count) = 0;

    }


    if((*Molecul_count) == (*moleculs_should_be))
    {
        sem_post(hydrogenQueue);
        sem_post(hydrogenQueue);
        sem_post(oxyQueue);
    }
    
    sem_post(mutex_2);
    
    sem_post(wait_for);
    exit (0);
        
}

void process_H(args arguments, int number_of_process_H)//samotna funkce vodiku, ktera spolu s funkci kysliku vytvareji molekulu vody
{
	
    if((*Molecul_count) == (*moleculs_should_be))
    {
        sem_post(hydrogenQueue);
        sem_post(hydrogenQueue);
        sem_post(oxyQueue);
    }
    (*H_count)++; 

    sem_wait(mutex);
    fprintf(file, "%d: H %d: started\n",(*move_counter)++ , number_of_process_H);
    fflush(file);
    sem_post(mutex);

     if(arguments.TI != 0)
    {
        srand(time(NULL) % getpid());
        random_wait(arguments.TI);
    }

    sem_wait(mutex);
    fprintf(file, "%d: H %d: going to queue\n",(*move_counter)++ ,number_of_process_H);
    fflush(file);
    sem_post(mutex);

    sem_wait(mutex_2);
    (*atoms_H_in_process)++;
    if((*atoms_H_in_process) >= 2 && (*atoms_O_in_process) >= 1 )
    {
        sem_post(hydrogenQueue);
        sem_post(hydrogenQueue);
        sem_post(oxyQueue);

        (*atoms_H_in_process) -= 2;

        (*atoms_O_in_process) -= 1;
    
        
    }else{   

    sem_post(mutex_2);
        
    }

    sem_wait(hydrogenQueue);

    sem_wait(barrier);
   

    if((*Molecul_count) == (*moleculs_should_be))
    {
        sem_post(barrier);
        sem_post(hydrogenQueue);
        sem_post(oxyQueue);
           
        sem_wait(mutex);
        fprintf(file,"%d: H %d: not enough O or H\n",(*move_counter)++, number_of_process_H);
        fflush(file);
        sem_post(mutex);

        sem_post(wait_for);
        exit(0);
    }

    sem_wait(mutex);
    fprintf(file, "%d: H %d: creating molecule %d\n",(*move_counter)++ ,number_of_process_H, (*Molecul_count));
    fflush(file);
    sem_post(mutex);

    (*creating_molecule)++;
    if((*creating_molecule) >= 3)
    {
        for (int i = 0; i < 3; i++)
        {
            sem_post(molecule_created);
        }
        (*creating_molecule) = 0;
    }

    sem_wait(molecule_created);

    sem_wait(mutex);
    fprintf(file, "%d: H %d: molecule %d created\n",(*move_counter)++ ,number_of_process_H, (*Molecul_count));
    fflush(file);
    sem_post(mutex);

    (*barrier_count)++;
    if(*barrier_count >= 3)
    {
        (*Molecul_count)++;
        for (int i = 0; i < 3; i++)
        {
            sem_post(barrier);
        }
        (*barrier_count) = 0;

    }

    if((*Molecul_count) == (*moleculs_should_be))
    {
        sem_post(hydrogenQueue);
        sem_post(hydrogenQueue);
        sem_post(oxyQueue);
       
    }

    sem_post(wait_for);
    exit (0);
    
}



args read_arguments(int argc, char *argv[], args arguments)//funkce na zpracovani vstupu 
{
    char *err;
    if(argc == 5)
    {
        arguments.NO = strtol(argv[1],&err, DECIMAL);
        if(*err != '\0' || arguments.NO < 1)
        {
            throw_err(ARG_DATATYPE_ERR);
        }

        arguments.NH = strtol(argv[2],&err, DECIMAL);
        if(*err != '\0' || arguments.NH < 1)
        {
            throw_err(ARG_DATATYPE_ERR);
        }

        arguments.TI = strtol(argv[3],&err, DECIMAL);
        if(*err != '\0' ||  arguments.TI < MINTIME || arguments.TI > MAXTIME)
        {
            throw_err(TIME_ARG_DATATYPE_ERR);
        }

        arguments.TB = strtol(argv[4],&err, DECIMAL);
        if(*err != '\0' || arguments.TB < MINTIME || arguments.TB > MAXTIME)
        {
            throw_err(TIME_ARG_DATATYPE_ERR);
        }

        return arguments;

    }
    else
    {
        throw_err(ARG_COUNT_ERR);
    }
    return arguments;
}

///////////////////////////////////////////////////
//
//              MAIN
//
///////////////////////////////////////////////////

int main(int argc, char *argv[])
{
    args arguments;
    
 
    if((file = fopen("proj2.out","w")) == NULL)
    {
        throw_err(FILE_OPEN_ERR);
    }
    
    arguments = read_arguments(argc, argv, arguments);

    
   
    semaphores_init();
    shared_mem_init();  
   
    (*moleculs_should_be) = ((arguments.NH/2) > arguments.NO ? arguments.NO+1 : floor((arguments.NH/2))+1);//pomocna promenna, do ktere se na zacatku spocita ocekavany pocet molekul, aby program vedel kolik molekul jeste dokaze vyrobit

    for (int i = 0; i < arguments.NO; i++)//vytvareni NO procesu kysliku  
    {
        
        pid_t childred_NO[arguments.NO];
        childred_NO[arguments.NO] = fork();
        if(childred_NO[arguments.NO] == 0)
        {
            process_H2O(arguments,i+1);

        }else if(childred_NO[arguments.NO] < 0)
        {
            for (int i = 0; i < arguments.NO; i++)
            {   
                sem_post(mutex);
                sem_post(hydrogenQueue);
                sem_post(hydrogenQueue);
                sem_post(oxyQueue);
                sem_post(wait_for);
            }
            sem_post(wait_for);
            clean_mem();
            clean_sem();
        }
    }

    for (int i = 0; i < arguments.NH; i++)//vytvareni NH procesu vodiku
    {
        pid_t childred_H[arguments.NH]; 
        childred_H[arguments.NH]= fork();

        if(childred_H[arguments.NH] == 0)
        {
            process_H(arguments,i+1);

        }else if (childred_H[arguments.NH] < 0)
        {    
            for (int i = 0; i < arguments.NH; i++)
            {   
                sem_post(mutex);
                sem_post(hydrogenQueue);
                sem_post(hydrogenQueue);
                sem_post(oxyQueue);
                sem_post(wait_for);
            }
            sem_post(wait_for);
            clean_mem();
            clean_sem();
        }
    }

    for (int i = 0; i < arguments.NH+arguments.NO+1;i++)
    {
        sem_wait(wait_for);
    }
   
    clean_sem();
    clean_mem();
    fclose(file);
    
    return EXIT_SUCCESS;
}
