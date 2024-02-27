void teste(char p1, char p2, char *p3)
{
    p2 = p2 + p1;
    *p3 = 3 * p1;
}
int main()
{
    char a = 10;
    char b = 20;
    teste(10, a, &b);
    printf("%d, %d\n", a, b);
}
/*
    Que valores são impressos pelo programa abaixo? Justifique.

    vai imprimir os valor de 10 e 60. É passado por referencia os valores de p1 = 10, p2 = a, p3 = &b.
    & diz que o endereço de memoria que está sendo passado por referencia.
    dentro da funçao diz que p2 vai ser igual a p2 + p1 = 10. Este valor não alterado fora da função.
    Depois diz que o valor de p3 vai ser 3 * 10 = 30.
    
    a = 10; b = 30; 

*/