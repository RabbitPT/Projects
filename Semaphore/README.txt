-> O programa tem um pequeno bug. Nas funções de mostrar o texto ("Text_Message"), nas linhas 360 e 500, paramos de incrementar a variável "counter" quando já estivermos com o valor "17" na variável "auxcounter". Tal provoca na matriz um avanço de dois caracteres em vez de um, uma vez por passagem da frase na matriz. Caso mudemos os valor para "16" (valor mais correto visto que a matriz pode ter de uma vez 15 carateres escritos) repete uma das letras sempre na posição final da matriz até a próxima passagem do ciclo.


->Para ligar os leds verticais do lado esquerdo o pin a ser usado no arduino será o 14 (pode ser mudado caso necessário).

->Para ligar os leds verticais do lado direito o pin a ser usado no arduino será o 15 (pode ser mudado caso necessário).

->Para ligar os leds do semáforo o pin a ser usado no arduino será o 16 (pode ser mudado caso necessário).

->Caso queira-se mudar a velocidade da animação mudar a variável "refreshRate". O refresh é em milisegundos, portanto, caso o valor da variável seja de 1000, a animação terá uma velocidade de 1 segundo cada "frame".

->Na função "Text_Message" onde passamos uma array de cores, é também necessário o tamanho da array. Acreditámos que foi a melhor opção visto que o utilizador poderá escolher uma quantidade menor de cores do que a quantidade de caracteres existentes. Caso tal se verifique, o resto dos caracteres serão preenchidos com a cor verde.

