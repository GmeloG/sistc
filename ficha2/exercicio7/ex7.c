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
            sleep(1);
            printf("Novo processo, com PID = %d (\"filho\" de %d)\n", getpid(), getppid());
        }
        exit(0); // termina o novo processo
    }

    for (int i = 0; i < 10; ++i)
    {
        sleep(2);
        printf("Processo inicial (PID = %d, \"filho\" de %d).\n", getpid(), getppid());
    }

    return (0);
}

/*
     PPID     PID    PGID     SID TTY        TPGID STAT   UID   TIME COMMAND
    376     415     415     376 pts/1        415 S+    1000   0:00 -bash
    431     432     432     432 pts/0        432 Ss+   1000   0:00 sh -c "$VSCODE_WSL_EXT_LOCATION/scripts/wslServ
    432     433     432     432 pts/0        432 S+    1000   0:00 sh /mnt/c/Users/gonca/.vscode/extensions/ms-vsc
    433     438     432     432 pts/0        432 S+    1000   0:00 sh /home/melo/.vscode-server/bin/1e790d77f81672
    438     498     432     432 pts/0        432 Sl+   1000   0:15 /home/melo/.vscode-server/bin/1e790d77f81672c49
    498     564     432     432 pts/0        432 Sl+   1000   0:01 /home/melo/.vscode-server/bin/1e790d77f81672c49
    498     595     432     432 pts/0        432 Sl+   1000   3:40 /home/melo/.vscode-server/bin/1e790d77f81672c49
    498     610     432     432 pts/0        432 Sl+   1000   0:11 /home/melo/.vscode-server/bin/1e790d77f81672c49
    610     621     621     621 pts/5      47012 Ss    1000   0:00 /bin/bash --init-file /home/melo/.vscode-server
    595     662     432     432 pts/0        432 Sl+   1000   0:00 /home/melo/.vscode-server/bin/1e790d77f81672c49
    595     688     432     432 pts/0        432 Sl+   1000   0:49 /home/melo/.vscode-server/extensions/ms-vscode.
    431    9526     432     432 pts/0        432 Sl+   1000   0:01 /home/melo/.vscode-server/extensions/ms-vscode.
  23564   23565   23565   23565 pts/2      23565 Ssl+  1000   0:01 /home/melo/.vscode-server/bin/1e790d77f81672c49
  23575   23576   23576   23576 pts/3      23576 Ssl+  1000   0:02 /home/melo/.vscode-server/bin/1e790d77f81672c49
    431   32366     432     432 pts/0        432 Sl+   1000   0:00 /home/melo/.vscode-server/extensions/ms-vscode.
    621   41705   41705     621 pts/5      47012 T     1000   0:00 ./ex11
  41705   41771   41705     621 pts/5      47012 T     1000   0:00 ./ex11
  42038   42040   42040   42040 pts/4      47284 Ss    1000   0:00 -bash
    621   43638   43638     621 pts/5      47012 T     1000   0:00 ./ex11
    431   43916   43638     621 pts/5      47012 T     1000   0:00 ./ex11
  43638   43917   43638     621 pts/5      47012 T     1000   0:00 ./ex11
    621   44240   44240     621 pts/5      47012 T     1000   0:00 ./ex11
  44240   44378   44240     621 pts/5      47012 T     1000   0:00 ./ex11
    621   47012   47012     621 pts/5      47012 S+    1000   0:00 ./ex7
  47012   47013   47012     621 pts/5      47012 Z+    1000   0:00 [ex7] <defunct>        <---------------Zombie não foi adotado pk termina primeiro que o pai
  42040   47284   47284   42040 pts/4      47284 R+    1000   0:00 ps j

*/