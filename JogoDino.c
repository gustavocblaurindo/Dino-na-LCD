#include <reg51.h>

// Definição e declaração dos pinos em lcd
sfr lcd_port = 0x90;   
sbit rs = P1^3;
sbit en = P1^2;

sbit p0_0 = P0^0;
sbit p0_1 = P0^1;
sbit p0_2 = P0^2;
sbit p0_3 = P0^3;
sbit p0_4 = P0^4;
sbit p0_5 = P0^5;
sbit p0_6 = P0^6;

char pressed_char = '\0';
char keys[13] = "123456789*0#";
unsigned int score = 0;

// Realiza um delay
void delay(unsigned int count)
{
    unsigned int i, j;
    for(i = 0; i < count; i++)
        for(j = 0; j < 112; j++);
}

//Envia um comando ao LCD
void lcd_command(char cmnd)
{
    lcd_port = (lcd_port & 0x0F) | (cmnd & 0xF0);
    rs = 0;
    en = 1; delay(1); en = 0;
    lcd_port = (lcd_port & 0x0F) | (cmnd << 4);
    en = 1; delay(1); en = 0;
    delay(2);
}

//Envia um caractere ao LCD
void lcd_char(char char_data)
{
    lcd_port = (lcd_port & 0x0F) | (char_data & 0xF0);
    rs = 1;
    en = 1; delay(1); 
		en = 0;
    lcd_port = (lcd_port & 0x0F) | (char_data << 4);
    en = 1; 
		delay(1); 
		en = 0;
    delay(2);
}

//Envia uma string ao LCD com a posição corrente do cursor
void lcd_string(char *str)
{
    int i;
    for(i = 0; str[i] != 0; i++)
        lcd_char(str[i]);
}

//Define a posição do cursor para o envio da string do LCD
void lcd_string_xy(char row, char pos, char *str)
{
    if(row == 0)
        lcd_command(0x80 + pos);
    else
        lcd_command(0xC0 + pos);
    lcd_string(str);
}

// Escreve um espaço numa posição — apaga o caractere sem dar clear na tela
void lcd_clear_pos(char row, char pos)
{
    if(row == 0)
        lcd_command(0x80 + pos);
    else
        lcd_command(0xC0 + pos);
    lcd_char(' ');
}

//Realiza a inicialização do display LCD
void lcd_init(void)
{
    delay(20);
    lcd_command(0x02);
    lcd_command(0x28);
    lcd_command(0x0C);
    lcd_command(0x06);
    lcd_command(0x01);
    p0_4 = 1;
    p0_5 = 1;
    p0_6 = 1;
}

//Cria o caractere e armazena na CGRAM
void lcd_create_char(unsigned char localizacao, unsigned char *msg)
{
    int i;
    lcd_command(0x40 + (localizacao * 8));
    for(i = 0; i < 8; i++){
        lcd_char(msg[i]);
		}
    lcd_command(0x80);
}

// Verifica qual coluna do teclado foi pressionada com debounce
int check_column()
{
    if (!p0_4){
        delay(50); 
        if(!p0_4);
        return 4;
    }
    else if (!p0_5){
        delay(50);
        if(!p0_5);
        return 5;
    }
    else if (!p0_6){
        delay(50);
        if(!p0_6);
        return 6;
    }
    return 0;
}

// Identifica o caractere com base na linha e coluna
int check_column_and_line(int linha)
{
    int coluna = check_column();
    if(coluna){
        pressed_char = keys[linha * 3 + 2 - (coluna - 4)];
        return 1;
    }
    return 0;
}

// Retorna 1 apenas na borda de descida (novo pressionamento)
int scan_key(void)
{
    int result = 0;
    p0_3 = 0;
    if(check_column_and_line(0)){ 
			p0_3 = 1; 
			return 1; 
		}
    p0_3 = 1;
    p0_2 = 0;
    if(check_column_and_line(1)){ 
			p0_2 = 1; 
			return 1; 
		}
    p0_2 = 1;
    p0_1 = 0;
    if(check_column_and_line(2)){ 
			p0_1 = 1; 
			return 1; 
		}
    p0_1 = 1;
    p0_0 = 0;
    if(check_column_and_line(3)){ 
			p0_0 = 1; 
			return 1; 
		}
    p0_0 = 1;
    return result;
}

//Imprime a pontuação do joguinho do dinossauro
void print_score(int x, int y)
{
    char pr[5];
    int i = 0;

    unsigned int milhar = score / 1000;
    unsigned int centena = (score / 100) % 10;
    unsigned int dezena  = (score / 10) % 10;
    unsigned int unidade = score % 10;

    if(milhar > 0){
        pr[i] = milhar + '0';
				i++;
    }
    if(centena > 0 || i > 0){
        pr[i] = centena + '0';
				i++;
    }
    if(dezena > 0 || i > 0){
        pr[i] = dezena + '0';
				i++;
    }
    pr[i++] = unidade + '0';
    pr[i] = '\0';
    lcd_string_xy(x, y, pr);
}

// Armazenamento na CGRAM
//Sprites do modo dia
// Slot 0: Dino
unsigned char dino[8] = { 0x00, 0x07, 0x05, 0x17, 0x1C, 0x1F, 0x0D, 0x0C };
// Slot 1: Cacto
unsigned char cactus[8] = {0x00, 0x03, 0x03, 0x1B, 0x1B, 0x1F, 0x06, 0x06};
// Slot 2: Pássaro
unsigned char bird[8] = {0x00, 0x0C, 0x12, 0x11, 0x1F, 0x00, 0x00, 0x00};

// Sprites do modo noite
// Slot 3: Dino (Noite)
unsigned char dino_n[8] = { 0xFF, 0xF8, 0xFA, 0xE8, 0xE3, 0xE0, 0xF2, 0xF3 };
// Slot 4: Cacto (Noite)
unsigned char cactus_n[8] = { 0xFF, 0xFC, 0xFC, 0xE4, 0xE4, 0xE0, 0xF9, 0xF9 };
// Slot 5: Pássaro (Noite)
unsigned char bird_n[8] = { 0xFF, 0xF3, 0xED, 0xEE, 0xE0, 0xFF, 0xFF, 0xFF };

int cactus_pos = 15;
int bird_pos = -1;
int jumping = 0;
int jump_time = 0;
int gamespeed = 200;
		
void main()
{
    int col, i, wait_steps;
    int modes = 0;
    int key_pressed = 0;
    int just_landed = 0;
    char spr_dino = 0;
    char spr_cactus = 1;
    char spr_bird = 2;

    lcd_init();
    lcd_create_char(0, dino);
    lcd_create_char(1, cactus);
		lcd_create_char(2, bird);
    lcd_create_char(3, dino_n);
    lcd_create_char(4, cactus_n);
    lcd_create_char(5, bird_n);

    lcd_string_xy(0,0,"Jogo do T-rex"); 
    delay(2000);
    lcd_command(0x01);
    while(1)
    {
        // Detecção do botão para pulo 
        if(key_pressed)
        {
            if(!jumping && !just_landed)
            {
                jumping = 1;
                jump_time = 3;
            }
        }
        
        // Verifica modo dia ou noite a cada 50 pontos após 100 pontos (sintaxe corrigida)
				if(score < 100){
					modes = 0;
				}
				else if (((score/50) % 2) == 0){
					modes = 1;
				}
				else{
					modes = 0;
				}
        
        if(modes == 1)
        {
            spr_dino = 3;
            spr_cactus = 4;
            spr_bird = 5;
            // Preenche a tela de preto 
            lcd_command(0x80);
            for(col = 0; col < 16; col++){
							lcd_char(0xFF);
						}
            lcd_command(0xC0);
            for(col = 0; col < 16; col++){
							lcd_char(0xFF);
						}
        }
        else
        {
            spr_dino = 0;
            spr_cactus = 1;
            spr_bird = 2;
            lcd_command(0x01);
        }

        // Imprime a pontuação
        print_score(0,12);
        // Inicializa o cacto
        lcd_command(0xC0 + cactus_pos);
        lcd_char(spr_cactus);
				//Inicializa o pássaro
				if(bird_pos >= 0){
					lcd_command(0x80 + bird_pos);
					lcd_char(spr_bird);
				}
        // O cenário prossegue normalmente se o dinossauro estiver pulando
        if(jumping)
        {
            lcd_command(0x80 + 1);
            lcd_char(spr_dino);
            jump_time--;
            if(jump_time == 0){
                jumping = 0;
                just_landed = 1;
						}
        }
        else
        {
            lcd_command(0xC0 + 1);
            lcd_char(spr_dino);
            just_landed = 0; 
        }
        // Detecção de colisão com o cacto ou pássaro
        if((cactus_pos == 1 && !jumping) || (bird_pos == 1 && jumping))
        {
            lcd_command(0x01);
						delay(20);
            lcd_string_xy(0,3,"GAME OVER");
            lcd_string_xy(1,0,"Score:");
            print_score(1,7);
            while(1);
        }
				// Movimento do pássaro
        bird_pos--;
				if((score > 50) && (cactus_pos < 9) && (bird_pos < 0)){
					bird_pos = 15;
				}
				// Movimento do cacto
        cactus_pos--;
        if(cactus_pos < 0)
        {
            cactus_pos = 15;
        }
				
        if(score > 50){
					gamespeed = 200 >> (score / 50);
				}
				
        //Leitura contínua durante o delay do jogo para detectar melhor possível clique do botao
        key_pressed = 0;
        wait_steps = gamespeed / 5;
        if(wait_steps <= 0){
					wait_steps = 1;
				}
        
        for(i = 0; i < wait_steps; i++) {
            if(!key_pressed && scan_key()) {
                key_pressed = 1;
            }
            delay(5);
        }
        
        // Lógica de pontuação
        if(key_pressed && jumping){
            delay(5);
            if(scan_key()){ // Se continuou segurando 
                if(score > 0){
									score--;
								}
            }
            else{
                score++;
            }
        }
        else{
            score++;
        }
		}
}