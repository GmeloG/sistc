/*
    
        Escreva um programa que crie um novo processo. O processo filho deverá imprimir os 
    seus PID e PPID e terminar. O processo pai deverá aguardar alguns instantes (e.g., 3 
    segundos, usando função sleep) e então executar o comando “ps -f” através da função 
    system: 
    int system(const char *command); //da norma ANSI C 
    Antes de terminar, o processo pai deverá imprimir a mensagem “Vou terminar”. Após a 
    terminação do programa volte a executar o comando “ps -f”, na shell. Analise os 
    resultados.

*/