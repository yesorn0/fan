#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#define FND	PORTA


// 인터럽트 4 ~ 6을 입력핀으로 받을거임
// PE4 ~ 6

uint8_t flag =0;

uint8_t segment[3]= // ISR이 flag를 통해 동작해서 int main()안 지역변수로 들어가도 무방.
{
	0x3F, 0x06, 0x5B
};

ISR(INT4_vect) // sw_stop
{
	flag =0; // 버튼 상태 설정
}

ISR(INT5_vect) //sw_weak
{
	flag =1; // 버튼 상태 설정
}


ISR(INT6_vect) //sw_stong
{
	flag =2; // 버튼 상태 설정
}



int main(void)
{
	DDRB |=(1<<DDRB4);
	TCCR0 |= (1<<WGM00) | (1<<COM01) | (1<<WGM01) | (1<<CS02);
	//Fast PWM 모드 설정
	//분주 N 64 설정 // non 인버팅 모드
	
	
	sei();
	EICRB |= (1<<ISC41) | (1<<ISC51) | (1<<ISC61); // 하강엣지 사용
	EIMSK |= (1<<INT4) | (1<<INT5) | (1<< INT6); // 인터럽트 4, 5, 6 사용
	DDRA = 0xff; // PORTA 를 출력으로 설정
	DDRE = 0x00; // PORTE를 입력으로 설정
	PORTA = ~(segment[0]); // 초기화면 0
	
	while (1)
	{
		if(flag == 0)
		{
			FND = ~(segment[0]);
			OCR0 = 0;
		}
		else if(flag == 1)
		{
			if(!(PINE & (1<<PINE5)))
			{
				_delay_ms(10);
				if(!(PINE & (1<<PINE5)))
				{
					FND	= ~(segment[1]);
					OCR0 = 127;
				}
			}
			
		}
		else if(flag == 2)
		{
			if(!(PINE & (1<<PINE6)))
			{
				_delay_ms(10);
				if(!(PINE & (1<<PINE6)))
				{
					FND = ~(segment[2]);
					OCR0 = 254;
				}
			}
			
		}
		
	}
}

