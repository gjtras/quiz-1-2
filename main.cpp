#include <iostream>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

int main(int argc, char *argv[])
{

    const int microToSeconds = 1000000;
    const double sixSeconds = 6 * microToSeconds; 
    int option = 0; // default option: execute the command ls -l and terminate normally

    // Get flag from command line
    int opt;
    while ((opt = getopt(argc, argv, "n:")) != -1)
    {
        switch (opt)
        {
        case 'n':
            option = atoi(optarg);
            break;
        }
    }

    pid_t pid = fork();

    if ( pid < 0)
    {
        cout << "Fork failed" << endl;
        return 1;
    }
    else if (pid == 0)
    {
        cout << "Hello from the child process!" << endl;
        /* TODO: PRINT THE PARENT PID value: "The parent process ID is $ID" */
        printf("The parent process ID is %u", getppid());

        if (option % 2 == 0) // if the option number is even, execute the command ls -l and terminate normally
        {
            std::cout << "The child process will execute the command: ls -l after 6 seconds" << std::endl;
            /* TODO: SLEEP FOR 6 SECONDS*/
            usleep(sixSeconds);
            /* TODO: EXECUTE THE COMMAND ls -l USING EXECVP*/

            char* args[] = {(char*)"ls", (char*)"-l", NULL};

            execvp("ls", args);
        }
        else // if the option number is odd, terminate with a kill signal
        {
            std::cout << "The child process is exiting" << std::endl;
            kill(getpid(), SIGINT);
        }
    }
    else if (pid > 0)
    {
        int status;

        /* TODO: WAIT FOR CHILD PROCESS TO FINISH */
        while ((pid = wait(&status)) > 0);

        cout << "\nHello from the parent process!" << endl;

        /* TODO: PRINT THE CHILD PID value: "The child process ID is $ID" */
        printf("The child process ID is %u", pid);

        /* TODO: PRINT THE EXIT STATUS OF THE CHILD PROCESS BASED waitpid().
        MAKE SURE TO PASS BY REFERENCE THE STATUS VARIABLE TO THE SECOND PARAMETER OF waitpid()
        IF WIFEXITED, PRINT THE MESSAGE "The child process exited normally" WITH ENDLINE
        IF WIFSIGNALED, PRINT THE MESSAGE "The child process exited due to the kill signal" WITH ENDLINE
        */
        waitpid(pid, &status, 0);

        if ( WIFEXITED(status) )
        {
             
            printf("The child process exited normally\n");
        } else if ( WIFSIGNALED(status)){
            printf("The child process exited due to the kill signal\n");
        }
        

    }

    return 0;
}