/**
 * 5 semestre - Eng. da Computa��o - Insper
 * Rafael Corsi - rafael.corsi@insper.edu.br
 *
 * Projeto 1 para a placa SAME70-XPLD
 *
 * Objetivo :
 *  - Tocar música com buzzer externo
 *
 * Material :
 *  - Kit: ATMEL SAME70-XPLD - ARM CORTEX M7
 *
 * Integrantes: Raphael Azevedo e Guilherme Aliperti
 *	
 */


	

/************************************************************************/
/* includes                                                             */
/************************************************************************/

#include "asf.h"

/************************************************************************/
/* defines                                                              */
/************************************************************************/

//configs LED
#define LED_PIO           PIOC
#define LED_PIO_ID        12
#define LED_PIO_IDX       8u
#define LED_PIO_IDX_MASK  (1u << LED_PIO_IDX)

//Configs button 1, play/stop
#define BUT1_PIO           PIOA
#define BUT1_PIO_ID        ID_PIOA
#define BUT1_PIO_IDX       2u
#define BUT1_PIO_IDX_MASK  (1u << BUT1_PIO_IDX)

//Configs button 2, change song 
#define BUT2_PIO           PIOA
#define BUT2_PIO_ID        ID_PIOA
#define BUT2_PIO_IDX       3u
#define BUT2_PIO_IDX_MASK  (1u << BUT2_PIO_IDX)


//Configs Buzzer
#define BUZZ_PIO			PIOA
#define BUZZ_PIO_ID			10
#define BUZZ_PIO_IDX		19u
#define BUZZ_PIO_IDX_MASK	(1u << BUZZ_PIO_IDX)

/************************************************************************/
/* defines notas músicas                                                */
/************************************************************************/
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978

/************************************************************************/
/* constants                                                            */
/************************************************************************/

/* https://www.princetronics.com/supermariothemesong/ */
int melodymario[] = {
	NOTE_E7, NOTE_E7, 0, NOTE_E7,
	0, NOTE_C7, NOTE_E7, 0,
	NOTE_G7, 0, 0,  0,
	NOTE_G6, 0, 0, 0,
	
	NOTE_C7, 0, 0, NOTE_G6,
	0, 0, NOTE_E6, 0,
	0, NOTE_A6, 0, NOTE_B6,
	0, NOTE_AS6, NOTE_A6, 0,
	
	NOTE_G6, NOTE_E7, NOTE_G7,
	NOTE_A7, 0, NOTE_F7, NOTE_G7,
	0, NOTE_E7, 0, NOTE_C7,
	NOTE_D7, NOTE_B6, 0, 0,
	
	NOTE_C7, 0, 0, NOTE_G6,
	0, 0, NOTE_E6, 0,
	0, NOTE_A6, 0, NOTE_B6,
	0, NOTE_AS6, NOTE_A6, 0,
	
	NOTE_G6, NOTE_E7, NOTE_G7,
	NOTE_A7, 0, NOTE_F7, NOTE_G7,
	0, NOTE_E7, 0, NOTE_C7,
	NOTE_D7, NOTE_B6, 0, 0
};

int tempomario[] = {
	12, 12, 12, 12,
	12, 12, 12, 12,
	12, 12, 12, 12,
	12, 12, 12, 12,
	
	12, 12, 12, 12,
	12, 12, 12, 12,
	12, 12, 12, 12,
	12, 12, 12, 12,
	
	9, 9, 9,
	12, 12, 12, 12,
	12, 12, 12, 12,
	12, 12, 12, 12,
	
	12, 12, 12, 12,
	12, 12, 12, 12,
	12, 12, 12, 12,
	12, 12, 12, 12,
	
	9, 9, 9,
	12, 12, 12, 12,
	12, 12, 12, 12,
	12, 12, 12, 12,
};


/* https://www.hackster.io/joshi/piezo-christmas-songs-fd1ae9 */
int melody2[] = {
	NOTE_E5, NOTE_E5, NOTE_E5,
	NOTE_E5, NOTE_E5, NOTE_E5,
	NOTE_E5, NOTE_G5, NOTE_C5, NOTE_D5,
	NOTE_E5,
	NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5,
	NOTE_F5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5,
	NOTE_E5, NOTE_D5, NOTE_D5, NOTE_E5,
	NOTE_D5, NOTE_G5
	};

int tempomelody2[] = {
	 8, 8, 4,
	 8, 8, 4,
	 8, 8, 8, 8,
	 2,
	 8, 8, 8, 8,
	 8, 8, 8, 16, 16,
	 8, 8, 8, 8,
	 4, 4
};

/* https://github.com/deathbearbrown/arduino-tunes/blob/master/zelda/zelda.ino */
int melodyzelda[] = {
	NOTE_C4, NOTE_A4, NOTE_G4, NOTE_F4, NOTE_G4,  NOTE_E4,
	NOTE_C4, NOTE_A4, NOTE_G4, NOTE_F4, NOTE_E4,  NOTE_F4,
	NOTE_C4, NOTE_A4, NOTE_G4, NOTE_F4, NOTE_G4,  NOTE_CS4,
	NOTE_C4, NOTE_A4, NOTE_G4, NOTE_F4, NOTE_CS4, NOTE_C4,
	NOTE_C4, NOTE_A4, NOTE_G4, NOTE_F4, NOTE_DS4, NOTE_CS4,
	NOTE_A3, NOTE_G3, NOTE_G4, NOTE_F4, NOTE_FS4, NOTE_F4,
	NOTE_G4, NOTE_F4, NOTE_G4, NOTE_CS4, NOTE_F4, NOTE_G4,
	NOTE_F4, NOTE_CS4, NOTE_C4
};

int tempozelda[] = {
	3, 4, 4, 6, 6, 3,
	4, 6, 6, 4, 4, 3,
	2, 6, 6, 4, 6, 6,
	3, 4, 4, 6, 6, 3,
	3, 4, 4, 6, 6, 3,
	4, 6, 6, 4, 4, 3,
	4, 6, 6, 4, 6, 6,
	3, 4, 4
};

//*****************************************

/************************************************************************/
/* variaveis globais                                                    */
/************************************************************************/

/************************************************************************/
/* prototypes                                                           */
/************************************************************************/

void init(void);

/************************************************************************/
/* interrupcoes                                                         */
/************************************************************************/

/************************************************************************/
/* funcoes                                                              */
/************************************************************************/

void buzz(long frequency, long duration) {
	pio_clear(PIOC, LED_PIO_IDX_MASK); //Liga LED
	long delayValue = 1000000 / frequency / 2;
	long Cycles = frequency * duration / 1000;
	for (long i = 0; i < Cycles; i++) {
		pio_set(BUZZ_PIO, BUZZ_PIO_IDX_MASK);	// 1 no BUZZ
		delay_us(delayValue);						//delay de acordo com a frequencia e duracao do toque
		pio_clear(BUZZ_PIO, BUZZ_PIO_IDX_MASK); // 0 no BUZZ
		delay_us(delayValue);
	}
	pio_set(PIOC, LED_PIO_IDX_MASK); //desliga LED
}

int play_music(int melody[], int tempo[], int size, int Note){
	for (int thisNote = Note; thisNote < size; thisNote++) {
		int noteDuration = 1000 / tempo[thisNote];   //50000 era 1000
		buzz(melody[thisNote], noteDuration);
		if(!(pio_get(BUT1_PIO, PIO_INPUT, BUT1_PIO_IDX_MASK))){ //se o botao de play for pressionado para a musica
			buzz(0, noteDuration);
			delay_s(2);
			return thisNote;
		}
		/*while((pio_get(BUT2_PIO, PIO_INPUT, BUT2_PIO_IDX_MASK))) { //enquanto o botão de play não for pressionado denovo
			if(!(pio_get(BUT2_PIO, PIO_INPUT, BUT2_PIO_IDX_MASK))) {
				break;
			}
		}*/
		
		int pauseBetweenNotes = noteDuration * 1.30;
		delay_ms(pauseBetweenNotes); //tempo entre os toques
		
		buzz(0, noteDuration);
		
		}
	return 0;
	}

// Fun��o de inicializa��o do uC
void init(void)
{
	sysclk_init();

	//desativa watchdog timer
	WDT->WDT_MR = WDT_MR_WDDIS;

	// enable buzz
	pmc_enable_periph_clk(BUZZ_PIO_ID);
	pio_set_output(BUZZ_PIO, BUZZ_PIO_IDX_MASK, 0, 0, 0);

	//enable LED
	pmc_enable_periph_clk(LED_PIO_ID);
	pio_set_output(LED_PIO, LED_PIO_IDX_MASK, 0, 0, 0);

	//enable button 1
	pmc_enable_periph_clk(BUT1_PIO_ID);
	pio_set_input(BUT1_PIO, BUT1_PIO_IDX_MASK, PIO_DEFAULT);
	pio_pull_up(BUT1_PIO, BUT1_PIO_IDX_MASK, 1);

	//enable button 2
	pmc_enable_periph_clk(BUT2_PIO_ID);
	pio_set_input(BUT2_PIO, BUT2_PIO_IDX_MASK, PIO_DEFAULT);
	pio_pull_up(BUT2_PIO, BUT2_PIO_IDX_MASK, 1);
	
}
	
	

/************************************************************************/
/* Main                                                                 */
/************************************************************************/

// Funcao principal chamada na inicalizacao do uC.
int main(void)
{
  init();
  int sizemario = sizeof(melodymario) / sizeof(int);
  int sizemelody2 = sizeof(melody2) / sizeof(int);
  int sizezelda = sizeof(melodyzelda) / sizeof(int);
  int Note = 0;
  int nmusic = 1;

  // super loop
  // aplicacoes embarcadas n�o devem sair do while(1).
  while (1)
  {
	if (!pio_get(BUT2_PIO, PIO_INPUT, BUT2_PIO_IDX_MASK)) { 
		delay_ms(500);
		Note = 0; 
		if (nmusic == 3) {
			nmusic = 1;
		} else {
			nmusic++;
		}
	}
	  
	if ((!pio_get(BUT1_PIO, PIO_INPUT, BUT1_PIO_IDX_MASK)) && (nmusic == 1)) {
		delay_s(1);
		Note = play_music(melodymario, tempomario, sizemario, Note);
		
	} else if ((!pio_get(BUT1_PIO, PIO_INPUT, BUT1_PIO_IDX_MASK)) && (nmusic == 2)) {
		delay_s(1);
		Note = play_music(melody2, tempomelody2, sizemelody2, Note);	
	} else if ((!pio_get(BUT1_PIO, PIO_INPUT, BUT1_PIO_IDX_MASK)) && (nmusic == 3)) {
		delay_s(1);
		Note = play_music(melodyzelda, tempozelda, sizezelda, Note);	
	}
	pio_set(PIOC, LED_PIO_IDX_MASK);      // Coloca 1 no pino LED
  }
  return 0;
}
