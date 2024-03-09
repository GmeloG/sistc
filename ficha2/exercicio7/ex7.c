/*
        Exercício 7 (Processos “zombie”)  
    Por omissão, um processo não termina imediatamente quando termina a execução do 
    respetivo programa. O processo só termina completamente quando o seu processo pai 
    “lê”1 o respetivo valor de retorno usando uma das funções wait, waitpid ou waitid. 
    Durante esse período – desde o fim do programa até à leitura do valor de retorno do 
    processo – diz-se que o processo está no estado zombie (também assinalado como defunct 
    nas listagens do comando ps)  
    Na prática, quando um processo termina a execução do respetivo programa, todos os seus 
    processos filho no estado zombie são eliminados do sistema. Esta terminação 
    “automática” de processos zombie deve-se ao mecanismo de adoção de processos órfãos. 
    Nesses casos, a leitura do valor de retorno do processo zombie é feita pelo processo 
    adotivo (init, systemd, etc.) imediatamente após a adoção do processo. Em rigor, logo 
    que um processo passa ao estado zombie, todos os seus processos filho são imediatamente 
    adotados pelo processo adotivo do sistema. 
    a) Volte a executar o programa do exercício 1 e, noutro terminal, execute o comando 
    “ps j” na altura em que o processo filho já terminou e o processo inicial continua as 
    suas impressões. Identifique o processo no estado zombie. 

     449     14879   14878   10318 pts/10     10318 S     1000   0:00 ./ex7

    b) Confirme, novamente com o comando ps j, que após a terminação do processo 
    inicial, o processo no estado zombie é também eliminado. 
    
    
    PERGUNTAR AO PROF
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{

    pid_t r;

    printf("\nProcesso %d a executar %s\n\n", getpid(), argv[0]);

    r = fork(); // retorna 0 no novo processo

    printf("Valor de retorno da função fork para o processo %d: %d\n\n", getpid(), r);

    if (r == 0)
    {
        for (int i = 0; i < 10; ++i)
        {
            sleep(2);
            printf("Novo processo, com PID = %d (\"filho\" de %d)\n",
                   getpid(), getppid());
        }
        exit(0); // termina o novo processo
    }

    for (int i = 0; i < 10; ++i)
    {
        sleep(1);
        printf("Processo inicial (PID = %d, \"filho\" de %d).\n", getpid(), getppid());
    }

    return (0);
}

/*
     ps j
    PPID    PID     PGID    SID TTY        TPGID STAT   UID   TIME COMMAND
    396     431     431     396 pts/1        431 S+    1000   0:00 -bash
    449     450     450     450 pts/0        450 Ss+   1000   0:00 sh -c "$VSCODE_WSL_EXT_LOCATION/scripts/wslSer
    450     451     450     450 pts/0        450 S+    1000   0:00 sh /mnt/c/Users/gonca/.vscode/extensions/ms-vs
    451     456     450     450 pts/0        450 S+    1000   0:00 sh /home/melo/.vscode-server/bin/019f4d1419fbc
    456     516     450     450 pts/0        450 Sl+   1000   0:37 /home/melo/.vscode-server/bin/019f4d1419fbc821
    516     540     450     450 pts/0        450 Sl+   1000   0:14 /home/melo/.vscode-server/bin/019f4d1419fbc821
    10220   10221   10221   10221 pts/2      10221 Ssl+  1000   0:01 /home/melo/.vscode-server/bin/019f4d1419fbc821
    10229   10230   10230   10230 pts/3      10230 Ssl+  1000   0:02 /home/melo/.vscode-server/bin/019f4d1419fbc821
    516     10237   450     450 pts/0        450 Sl+   1000   0:00 /home/melo/.vscode-server/bin/019f4d1419fbc821
    516     10250   450     450 pts/0        450 Sl+   1000   1:06 /home/melo/.vscode-server/bin/019f4d1419fbc821
    540     10318   10318   10318 pts/10     10318 Ss+   1000   0:00 /bin/bash --init-file /home/melo/.vscode-serve
    10250   10380   450     450 pts/0        450 Sl+   1000   0:09 /home/melo/.vscode-server/extensions/ms-vscode
    10250   10405   450     450 pts/0        450 Sl+   1000   0:00 /home/melo/.vscode-server/bin/019f4d1419fbc821
    13848   13852   13852   13852 pts/4      14974 Ss    1000   0:00 -bash
    449     14855   450     450 pts/0        450 Sl+   1000   0:00 /home/melo/.vscode-server/extensions/ms-vscode
    449     14879   14878   10318 pts/10     10318 S     1000   0:00 ./ex7  <---------------------------------------------------
    13852   14974   14974   13852 pts/4      14974 R+    1000   0:00 ps j


    PPID     PID    PGID     SID TTY        TPGID STAT   UID   TIME COMMAND
    396     431     431     396 pts/1        431 S+    1000   0:00 -bash
    449     450     450     450 pts/0        450 Ss+   1000   0:00 sh -c "$VSCODE_WSL_EXT_LOCATION/scripts/wslSer
    450     451     450     450 pts/0        450 S+    1000   0:00 sh /mnt/c/Users/gonca/.vscode/extensions/ms-vs
    451     456     450     450 pts/0        450 S+    1000   0:00 sh /home/melo/.vscode-server/bin/019f4d1419fbc
    456     516     450     450 pts/0        450 Sl+   1000   0:38 /home/melo/.vscode-server/bin/019f4d1419fbc821
    516     540     450     450 pts/0        450 Sl+   1000   0:15 /home/melo/.vscode-server/bin/019f4d1419fbc821
  10220   10221   10221   10221 pts/2      10221 Ssl+  1000   0:02 /home/melo/.vscode-server/bin/019f4d1419fbc821
  10229   10230   10230   10230 pts/3      10230 Ssl+  1000   0:03 /home/melo/.vscode-server/bin/019f4d1419fbc821
    516   10237     450     450 pts/0        450 Sl+   1000   0:00 /home/melo/.vscode-server/bin/019f4d1419fbc821
    516   10250     450     450 pts/0        450 Sl+   1000   1:32 /home/melo/.vscode-server/bin/019f4d1419fbc821
    540   10318   10318   10318 pts/10     10318 Ss+   1000   0:00 /bin/bash --init-file /home/melo/.vscode-serve
  10250   10380     450     450 pts/0        450 Sl+   1000   0:18 /home/melo/.vscode-server/extensions/ms-vscode
  10250   10405     450     450 pts/0        450 Sl+   1000   0:00 /home/melo/.vscode-server/bin/019f4d1419fbc821
  13848   13852   13852   13852 pts/4      17027 Ss    1000   0:00 -bash
    449   14855     450     450 pts/0        450 Sl+   1000   0:00 /home/melo/.vscode-server/extensions/ms-vscode
  13852   17027   17027   13852 pts/4      17027 R+    1000   0:00 ps j

*/