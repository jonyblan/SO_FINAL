#include <standardlib.h>
#include <nanoshell.h>
#include <stddef.h>
#include <pipelib.h>
#include <videolib.h>

extern throw_zero_division();
extern throw_invalid_opcode();
#define MAX_PROCESSES 256
static char *todo[] = {
// help
"Check that they are on date",
// registers
"",
// time
"",
// echo
"Make it be able to print other things (echo test_malloc for example)",
// clear
"",
// test_zero_divition
"",
// test_invalid_opcode
"",
// test_malloc
"",
// todo
"write a lot of TODOs\n\
make it return a random todo",
// functions
"Not implemented",
// mini_process
"Make easier to understand",
// test_priority
"",

// Useful

// malloc
"",
// realloc
"Not implemented",
// calloc
"Not implemented",
// free
"",
// createProcess
"Not implemented",
// getPriority
"",
// setPriority
"",
};

static char *help_text = "Here's a list of all available commands:\n\
- help --> Help display with all commands\n\
- registers --> Displays the lastest backup of registers\n\
- time --> Displays time and date\n\
- echo [string] --> Prints the [string] argument in the display\n\
- clear --> clears the display\n\
- test_zero_division --> Test for the Zero Division exception\n\
- test_invalid_opcode --> Test for the Invalid Opcode exception\n\
- test_malloc --> starts the malloc test\n\
- todo --> displays a random thing that has to be done\n\
- functions --> displays every page inside the manual\n\
- mini_process --> creates a new process according to simpleProcess.c\n\
- test_priority --> test that the priority system is working correctly\n\
- test_semaphore --> test that the semaphore system is working correctly\n\
- test_pipe --> test that the pipe system is working correctly\n\
- mem --> shows the memory state NOT DONE\n\
";

static char *help_text2 = "- ps --> prints a list of every running process with some data from each NOT DONE\n\
- loop --> makes a process run and print its id along with a greeting every [count] of seconds\n\
- kill [pid] --> kills a process based on its [pid] NOT TESTED\n\
- nice [pid] [new priority] --> changes the [pid] process to be of [new priority] priority NOT TESTED\n\
- block [pid] --> changes the [pid] process between blocked and unblocked NOT TESTED\n\
- cat --> prints the stdin\n\
- wc --> counts the amount of lines in the input\n\
- filther --> filthers the vowels from the input \n\
- phylo --> starts running the phylosofers problem. \"a\" to add 1, \"r\" to remove one NOT DONE\n\
- use_memory [time] [bytes] --> uses [bytes] of memory for [time] seconds to make it easier to test for mem \n\
";

// add new command or useful here
static char *functions = "\
Commands: help, registers, time, echo, clear, test_zero_division\n\
test_invalid_opcode, test_malloc, todo, functions, mini_process, test_priority, test_semaphore,\n\
test_pipe, mem, loop, kill, nice, block, cat, wc, filther, phylo, use_memory\n\n\n\
Useful: malloc, realloc, calloc, free, getPriority, setPriority\n";


void bloqueadoFunc(int argc, char* argv[]){
	uint8_t sem = sem_open("test", 0);
	int j;
	for (int i = 0; i < 10; i++)
	{
		printf("(por ahi) Me bloqueo!\n");
		sem_wait(sem);
		printf("Me desbloquie :)\n");
		sleep(100);
	}
}

void liberadorFunc(int argc, char* argv[]){
	uint8_t sem = sem_open("test", 0);
	int j;
	for (int i = 0; i < 10; i++)
	{
		sem_post(sem);
		printf("Liberador sem_post value: %d\n",j );
		sleep(800);
	}
}

void hablaFunc(int argc, char* argv[]){
	uint8_t pipeId = pipe_open("hola");
	uint8_t readyToPrint= sem_open("readyToPrint", 0); // Initialize semaphore for the pipe
	uint8_t readyToWrite= sem_open("readyToWrite", 0); // Initialize semaphore for the listener
	changeProcessFd(getMyPID(), pipeId, 1); // Set write end of pipe for this process
	for (int i = 0; i < 10; i++)
	{
		printf("AAAAAA\n");
		sem_post(readyToPrint); 
		sem_wait(readyToWrite); 
		sleep(100);
	}
}

void escuchaFunc(int argc, char* argv[]){
	uint8_t pipeId = pipe_open("hola");
	uint8_t readyToPrint= sem_open("readyToPrint", 0); // Initialize semaphore for the pipe
	uint8_t readyToWrite= sem_open("readyToWrite", 0); // Initialize semaphore for the listener
	changeProcessFd(getMyPID(), pipeId, 0); // Set read end of pipe for this process
	for(int i=0; i < 10; i++){
		sem_wait(readyToPrint); 
		char buffer[7];
		scanf("%s",buffer); // Read from the pipe
		printf("%s\n", buffer);
		sem_post(readyToWrite); 
		sleep(100);
	}
}

void loopFunc(int argc, char* argv[]){
	pid_t pid;
	pid=getMyPID();
	
	int time=2;
	while(1){
		sleep(120*time); //tick frecuncy * time
		printf("Pid from loop function: %d\n", pid);
	}
}

void startProcess(int argc, char* argv[]){
	for (int i = 0; i < 10; i++)
	{
		printf("Simple process running.\nargc: %d\n", argc);
	}
}

void testFunc(int argc, char* argv[]){
	for (int i = 0; i < 10; i++)
	{
		printf("Function %d running!\n", argc);
	}	
}

void timeFunc(int argc, char* argv[]){
	printCurrentTime();
}

void echoFunc(int argc, char* argv[]){
    for (int i = 1; i < argc; i++) {  // arrancás desde 1 para ignorar "echo"
        printf("%s", argv[i]);
        if (i != argc - 1)
            printf(" "); 
    }
    printf("\n");
}

void recieveEchoFunc(int argc, char* argv[]){
	char buffer[100];
	scanf("%s",buffer);
	printf("Lo que llego: %s\n",buffer);
	return;
}


void registersFunc(int argc, char* argv[]){
	getRegisters();
}

void helpFunc(int argc, char* argv[]){
	printf("%s", help_text);
	printf("%s", help_text2);	
}

void clearFunc(int argc, char* argv[]){
	clearScreen();
}

void testZeroDivisionFunc(int argc, char* argv[]){
	throw_zero_division();
}

void testInvalidOpcodeFunc(int argc, char* argv[]){
	throw_invalid_opcode();
}

void testMallocFunc(int argc, char* argv[]){
	printf("%d\n", testMalloc());
}

void todoFunc(int argc, char* argv[]){
	TimeStamp* ts ;
	ts=getTime();
	while(todo[ts->seconds%INSTRUCTION_COUNT] == ""){
		ts->seconds++;
	}
	printf("%d\n", ts->seconds);
	printf("%s\n", todo[ts->seconds%INSTRUCTION_COUNT]);
}

void functionsFunc(int argc, char* argv[]){
	printf("%s\n", functions);
}

void testPriorityFunc(int argc, char* argv[]){
	pid_t pid1, pid2, pid3;
	pid1 = (pid_t)createProcess(&testFunc, 1, argv, "test1");
	pid2 = (pid_t)createProcess(&testFunc, 2, argv, "test2");
	pid3 = (pid_t)createProcess(&testFunc, 3, argv, "test3");
	printf("priorities: %d: %d, %d: %d, %d: %d\n\n", pid1, getPriority(pid1), pid2, getPriority(pid2), pid3, getPriority(pid3));
	setPriority(pid1, 1);
	setPriority(pid2, 7);
	setPriority(pid3, 7);
	printf("priorities: %d: %d, %d: %d, %d: %d\n\n", pid1, getPriority(pid1), pid2, getPriority(pid2), pid3, getPriority(pid3));
	wait(pid1);
	wait(pid2);
	wait(pid3);
}

void testSemaphoreFunc(int argc, char* argv[]){
	pid_t pid1, pid2;
	pid1 = (pid_t)createProcess((void*)bloqueadoFunc, 1, argv,"Bloqueado");
	pid2 = (pid_t)createProcess((void*)liberadorFunc, 2, argv,"Liberador");
	wait(pid1);
	wait(pid2);
}

void testPipeFunc(int argc, char* argv[]){
	pid_t pid1, pid2;
	pid1 = (pid_t)createProcess((void*)hablaFunc, 1, argv,"Hablador");
	pid2 = (pid_t)createProcess((void*)escuchaFunc, 2, argv,"escuchante");
	printf("Termine\n");
	/* wait(pid1);
	wait(pid2); */
}

void useMemoryFunc(int argc, char* argv[]){
	int time = 10;
	int size = 0x1000;
	if(argc == 2 || argc == 3){
		time = (int)atoi(argv[1]);
	}
	if(argc == 3){
		size = (int)atoi(argv[2]);
	}
	int* ptr = malloc(size);
	printf("Usando %d bytes en memoria y esperando por %d segundos\n", size, time);
	sleep(120*time);
	printf("Liberando %d bytes en memoria\n", size);
	free(ptr);
	
}

void memFunc(int argc, char* argv[]){
	int* memData = mem();
	printf("Total: %d\tFree: %d\tUsed: %d\n", memData[0], memData[1], memData[2]);
}
void psFunc(int argc, char* argv[]){
	processInfo array[MAX_PROCESSES];
	uint8_t count;
	count= ps(array);
	char *msg[] = {"READY", "RUNNING", "BLOCKED", "TERMINATED"};
	int index;
	printf("PID\tNombre\tEstado\tPrioridad\tStack Base\t Stack Pointer\n");
	for (uint8_t i = 0; i < count; i++)
	{
		index=0;
		switch (array[i].state) {
			case RUNNING:;
				index = 1;
				break;
			case BLOCKED:;
				index = 2;
				break;
			case TERMINATED:;
				index=3;
				break;
			default:
				break;
		}
		printf("%d\t | %s\t |  %s\t   | %d\t      |    %x\t     |    %x\n",array[i].pid,array[i].name,msg[index],array[i].priority,array[i].stackBase,array[i].stackPointer);
	}
	printf("perdon\n");
	return;
}

void killFunc(int argc, char* argv[]){
	if (argc < 2) {
		printf("Uso: kill [pid]\n");
		return;
	}
	pid_t pid = (pid_t)atoi(argv[1]);
	if (pid <= 0) {
		printf("Invalid PID: %s\n", argv[1]);
		return;
	}
	killProcess(pid);
	printf("Proceso con PID %d killed.\n", pid);
}

void niceFunc(int argc, char* argv[]){
	if (argc < 3) {
		printf("Uso: nice [pid] [new priority]\n");
		return;
	}
	pid_t pid = (pid_t)atoi(argv[1]);
	int newPriority = atoi(argv[2]);
	if (pid <= 0 || newPriority < 0) {
		printf("PID o prioridad inválidos: %s %s\n", argv[1], argv[2]);
		return;
	}
	setPriority(pid, newPriority);
	printf("Proceso con PID %d cambiado a prioridad %d.\n", pid, newPriority);
}

void blockFunc(int argc, char* argv[]){
	if (argc < 2) {
		printf("Uso: block [pid]\n");
		return;
	}
	pid_t pid = (pid_t)atoi(argv[1]);
	if (pid <= 0) {
		printf("PID inválido: %s\n", argv[1]);
		return;
	}
	int ret = blockProcess(pid);
	if(ret == 1){
		printf("Proceso con PID %d bloqueado\n", pid);
	}
	else if(ret == 2){
		printf("Proceso con PID %d desbloqueado\n", pid);
	}
	else{
		printf("PID inválido: %s\n", argv[1]);
	}
}

void catFunc(int argc, char* argv[]){
	for (int i = 1; i < argc; i++) {
        printf("%s", argv[i]);
        if (i < argc - 1)
            printf(" ");
    }
    printf("\n");
}

void wcFunc(int argc, char* argv[]){
	printf("not implemented\n");
}

void filterFunc(int argc, char* argv[]){
	/*char noVowels[CMD_MAX_CHARS] = {0};
	for (i = 0; i < CMD_MAX_CHARS && cmdBuff[i] != 0 && cmdBuff[i] != '\t'; i++)
	{
		if (!strchr("aeiouAEIOU", cmdBuff[i])){
			noVowels[j] = cmdBuff[i];
			toMinus(noVowels);
			j++;
		}   
	}
	noVowels[j] = 0;
	int k = 6;
	while (noVowels[k] != 0){
		putChar(noVowels[k]);
		k++;
	} */
	printf("not implemented\n");
}
#define CMD_MAX_CHARS 100
#define MAX_PHYLOSOFERS 20

void initPhyloPipes(uint8_t* pipes){
    for(int i = 0; i < MAX_PHYLOSOFERS; i++){
        char* pipeNum = (char*)malloc(3); 
        unsigned_num_to_str(i, 0, pipeNum);

        char* pipeName = (char*)malloc(20);
        my_strcpy(pipeName, "pipe");           // Copiamos "pipe" al buffer
        my_strcat(pipeName, pipeNum);          // Concatenamos el número

		pipes[i] = pipe_open(pipeName);

        free(pipeNum);
        free(pipeName);
    }
}

void initPhyloSemaphores(uint8_t* semaphores){
	for(int i = 0; i < MAX_PHYLOSOFERS; i++){
        char* semNum = (char*)malloc(3); 
        unsigned_num_to_str(i, 0, semNum);

        char* semName = (char*)malloc(20);
        my_strcpy(semName, "sem");           // Copiamos "pipe" al buffer
        my_strcat(semName, semNum);          // Concatenamos el número

		semaphores[i] = sem_open(semName, 1);

        free(semNum);
        free(semName);
    }
}

void phylosofer(int argc, char* argv[]){
	printf("im phylosofer number %d\n", argc);
	//int myNumber = unsigned_str_to_num(0, strlen(argv[1]), argv[1]);
	//int cantFilosofos = unsigned_str_to_num(0, strlen(argv[2]), argv[2]);
	int myNumber = argc;	
	int cantFilosofos = 5;

	char* pipeNum = (char*)malloc(3); 
    unsigned_num_to_str(myNumber, 0, pipeNum);

    char* pipeName = (char*)malloc(20);
    my_strcpy(pipeName, "pipe");           // Copiamos "pipe" al buffer
    my_strcat(pipeName, pipeNum);          // Concatenamos el número

	uint8_t masterPipe = pipe_open(pipeName);

    free(pipeNum);
    free(pipeName);


	uint8_t globalSem = sem_open("globalSem", cantFilosofos - 1);

	uint8_t semaphore[2];

	if(myNumber == 0){
		semaphore[0] = sem_open("sem5", 1);
		semaphore[1] = sem_open("sem1", 1);
	}
	else{
		char* semNum = (char*)malloc(3); 
		char* semNum2 = (char*)malloc(3); 
        unsigned_num_to_str(myNumber, 0, semNum);
        unsigned_num_to_str(myNumber + 1, 0, semNum2);

        char* semName = (char*)malloc(20);
        char* semName2 = (char*)malloc(20);
        my_strcpy(semName, "sem");          
        my_strcat(semName, semNum);
        my_strcpy(semName2, "sem");   
        my_strcat(semName2, semNum2); 

		semaphore[0] = sem_open(semName, 1);
		semaphore[1] = sem_open(semName2, 1);
		printf("I'm %d and my semaphores are %s (%d) and %s (%d)\n\n", myNumber, semName, semaphore[0], semName2, semaphore[1]);

        free(semNum);
        free(semName);
        free(semNum2);
        free(semName2);
	}
	

	while(1){
		if(myNumber == 0){
			sem_wait(globalSem);
			sem_wait(semaphore[1]);
			sem_wait(semaphore[0]);
		}
		else{
			sem_wait(globalSem);
			sem_wait(semaphore[0]);
			sem_wait(semaphore[1]);
		}
		char *text = "c";
		pipe_write(masterPipe, text, 1);
		sleep(5000);
		sem_post(globalSem);
		sem_post(semaphore[0]);
		sem_post(semaphore[1]);
		char *text2 = "d";
		pipe_write(masterPipe, text2, 1);
		sleep(5000);
	}
}

void initPhyloProcesses(int* pidFilosofos, int cantFilosofos){
	char* cant = malloc(3);
	unsigned_num_to_str(cantFilosofos, 0, cant);
	char* name = "phylosofer";
	char* argv[3] = {"phylosofer", "", cant};
	void (*fn)(uint8_t, char **) = phylosofer;

	for(int i = 0; i < cantFilosofos; i++){
		//int argc = 3; CAMBIAR Y PONER ESTO DESPUES
		int argc = i;
		char* buff = malloc(3);
		unsigned_num_to_str(i, 0, buff);
		argv[1] = buff;

		printf("creando filosofo con argv[1] = %s\n", argv[1]);
		pidFilosofos[i] = createProcess(fn, argc, argv, name);
	}
}

void phyloFunc(int argc, char* argv[]){
	//clearScreen();
	printf("Filosofos:\n\n");
	int cantFilosofos = 5;
	uint8_t pipe = pipe_open("cantPhyloPipe");
	uint8_t pipes[MAX_PHYLOSOFERS];
	uint8_t semaphores[MAX_PHYLOSOFERS];
	int estaComiendo[MAX_PHYLOSOFERS] = {0};

	pid_t pidFilosofos[MAX_PHYLOSOFERS] = {0};

	initPhyloPipes(pipes);
	initPhyloSemaphores(semaphores);
	initPhyloProcesses(pidFilosofos, cantFilosofos);


	while(1){
		for(int i = 0; i < cantFilosofos; i++){
			char *buff = malloc(1);
			char buffWrite[1] = {'n'};
			(void)pipe_write(pipe, buffWrite, 1);
			pipe_read(pipes[i], buff, 1);
			printf("buffer from %d: %s", i, buff);
			putChar(buff[0]);
			putChar('\n');
			if(buff[0] == 'c'){
				estaComiendo[i] = 1;
			}
			else if(buff[0] == 'd'){
				estaComiendo[i] = 0;
			}
			if(estaComiendo){
				printf("F\t");
			}
			else{
				printf("-\t");
			}
		}
		printf("\n");
		char buffWrite[1] = {'n'};
		char buffRead[1];
		int finished = 0;
		(void)pipe_write(pipe, buffWrite, 1);


		while(!finished){
			pipe_read(pipe, buffRead, 1);
			if(buffRead[0] == 'a'){
				cantFilosofos++;
			}
			else if(buffRead[0] == 'r'){
				cantFilosofos--;
			}
			else{	
				finished = 1;
			}
		}
		printf("\n");
		sleep(10000);
	}	
}

void phyloCounter(int argc, char* argv[]){
	int cantFilosofos = 5;
    char cmdBuff[CMD_MAX_CHARS + 1] = {0};
	uint8_t pipe = pipe_open("cantPhyloPipe");
	while(1){
        int command_length = readLine(cmdBuff, CMD_MAX_CHARS);
		for(int i = 0; i < command_length; i++){
			if(cmdBuff[i] == 'a' || cmdBuff[i] == 'r'){
				char buffWrite[1] = {cmdBuff[i]};
				pipe_write(pipe, buffWrite, 1);
			}
		}
	}
}

