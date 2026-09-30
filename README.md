# Dino na LCD

Jogo do dino desenvolvido em C para o microcontrolador AT89S52 (família 8051). Este é um jogo do dinossauro em display LCD 16x2 e um jogo de corrida em display gráfico (GLCD) 128x64. É utilizado teclado matricial como controle e foram feitos com leitura de datasheets e manipulação direta das portas do microcontrolador.

<img width="306" height="324" alt="image" src="https://github.com/user-attachments/assets/e3864bfb-131e-496e-8d90-1dd6dfc8b908" />

- **Controle:** qualquer tecla do teclado matricial faz o dinossauro pular.
- **Obstáculos:** cactos e, depois de 50 pontos, pássaros que vêm em outra linha do display.
- **Dificuldade:** a velocidade aumenta conforme a pontuação sobe.
- **Modo noite:** a partir de 100 pontos, a tela alterna entre modo dia e modo noite a cada 50 pontos.
- **Fim de jogo:** ao bater em um obstáculo, aparece "GAME OVER" com a pontuação final.
- **Como foi feito:** os desenhos do dinossauro, do cacto e do pássaro são caracteres customizados gravados na memória do LCD (CGRAM). O LCD funciona em modo de 4 bits e o teclado é lido por varredura de linhas e colunas, com debounce.

## Arquivos:

- **JogoDino.c:** contém o código fonte do jogo.
- **VideoDino.mp4:** vídeo de demonstração do funcionamento do jogo.

## Como rodar

O código usa a sintaxe do compilador C51 (Keil). Compile cada arquivo em um projeto para o AT89S52 e grave o `.hex` gerado no microcontrolador.

Ligações usadas nos códigos:

- **Teclado matricial:** porta P0
- **LCD:** LCD 16x2 na porta P1
