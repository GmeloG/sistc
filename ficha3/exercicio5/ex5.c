/*Exercício V 
A função sigispending permite a um processo verificar se lhe foi enviado algum dos 
sinais cuja entrega bloqueou (sigprocmask). No entanto, o processo não tem forma de 
saber o número de vezes que este foi enviado. Mesmo quando o sinal não está bloqueado, 
pode dar-se o caso do mesmo sinal ser enviado várias vezes em rápida sucessão e o 
processo só detetar uma ocorrência desse sinal. Este exercício pretende ilustrar esse 
último fenómeno. 
O programa sigchld.c usa o sinal SIGCHLD para detetar a terminação de processos 
filho. O programa foi implementado de modo que estes terminem aproximadamente 5 
segundos após a sua criação. No entanto, a implementação tem uma falha.  
1) Execute o programa sigchld. Observe que, quando as impressões terminam, 
aparentemente ainda existem processos por terminar. No entanto, se abrir outro terminal 
e executar o comando “ps -e| grep sigchld” irá verificar que todos os processos 
filho já se encontram no estado zombie (defunct), isto é, já terminaram a execução do 
programa. 
Conclusão: o processo pai pode não ser capaz de detetar uma ocorrência do SIGCHLD 
para cada processo filho que termina. 
2) Termine o programa com a combinação ctrl+c. Verifique que o programa agora 
deteta a terminação de todos os processos filho. Analise o código e verifique de que forma 
esse comportamento é implementado. 
Observação: quando se prime a combinação ctrl+c num terminal, todos os processos 
do grupo do processo em foreground (i.e., tanto o processo em foreground como os seus 
processos filho que não tenham mudado de grupo) recebem o sinal SIGINT  
3) Execute novamente o programa, mas desta vez prima a combinação ctrl+c 
imediatamente após a mensagem “All processes created”. Observe que: 
• A função de atendimento do SIGINT é interrompida algumas vezes pela função 
de atendimento do SIGCHLD.  
Observação: este comportamento poderia ser evitado através da adequada 
configuração do campo sa_mask da estrutura sigaction e correspondente uso 
da função sigaction na configuração da função de atendimento do sinal 
SIGINT.  
• Os processos filho terminam devido à receção do SIGINT e não após a espera de 
5 segundos. 
4) Com base na função int_handler, altere a função cld_handler de forma que esta 
detete corretamente a terminação de todos os processos filho. */


