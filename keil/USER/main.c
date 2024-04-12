#include "delay.h"
#include "c_lcd1602.h"
#include "adc0832.h"
#include <math.h>
#include <stdio.h>

#define ABS(x) ((x > 0) ? (x) : (-x))

#define V_LV1_LIMIT  4
#define V_LV2_LIMIT  8
#define V_LV3_LIMIT  20

#define LVx_LEDs  P2
#define DATA_PICK P2

sbit BUZZER = P1^3; //����
sbit PWR    = P3^2; //��ʼ
sbit Add    = P3^0; //����������˿�
sbit Reduc  = P3^1; //����������˿�

sbit relay_A = P3^5;
sbit relay_B = P3^6;
sbit relay_C = P3^7;
void main(void)
{
	//��������
	double voltage_temp = 0, voltage_data = 0;
	int collect = 0, mode_count = 0, mode_temp = 0;
	char v_pick_temp = 0x80, data_flag = 0x00;
	float limit_data = 0;
	unsigned char v_buf[6];
	
	//��������������
	uchar bueezr_flag = 0, 
	      count = 0, 
	      buzzer_time = 9;
	
	LCD1602_init();
	Delay(200);
	LCD1602_show_str(0, 0, "V:          "); //�ڵ�һ����ʾ�����ݡ�
	LCD1602_show_str(0, 1, "MODE:       "); //�ڵڶ�����ʾ�����ݡ�	
	while(1)
	{
		/*******��ʾ��λ����********/
		switch (mode_count) {
			case 0:
				limit_data = V_LV1_LIMIT;
				LCD1602_show_str(7, 1, "0-4V ");
				LVx_LEDs = (LVx_LEDs | 0x0F) & 0xFE;
				break;
			case 1:
				limit_data = V_LV2_LIMIT;
				LCD1602_show_str(7, 1, "4-8V ");
				LVx_LEDs = (LVx_LEDs | 0x0F) & 0xFD;
				break;
			case 2:
				limit_data = V_LV3_LIMIT;
				LCD1602_show_str(7, 1, "8-20V");
				LVx_LEDs = (LVx_LEDs | 0x0F) & 0xFB;
				break;
			default:
				break;
		}
		while (!PWR) {		
			/*******��ȡ��ѹ����********/
			v_pick_temp = DATA_PICK & 0xF0;
			switch(v_pick_temp){
				case 0x80: mode_temp = 0; break;
				case 0xC0: mode_temp = 1; break;
				case 0xE0: mode_temp = 2; break;
				default: break;
			}

			if (mode_temp == 0 && mode_count == 0) {
				data_flag = 0x01;
				relay_A = 0;
			} else if (mode_temp == 1 && mode_count == 1) {
				data_flag = 0x01;
				relay_B = 0;
			} else if (mode_temp == 2 && mode_count == 2) {
				data_flag = 0x01;
				relay_C = 0;
			} else {
				data_flag = 0x00;
				voltage_data = 0;
				bueezr_flag = 1;
			}

			//��ѹ�������
			if (data_flag == 0x01) {
				bueezr_flag = 0;
				data_flag = 0x00;
				voltage_temp = read_data(0);
				voltage_data = voltage_temp / 255.0 * 5.0;
				if (!relay_B)
					voltage_data = voltage_data * 10;
				else if (!relay_C)
					voltage_data = voltage_data * 100;
				else
					;
				voltage_temp = 0;
			}
			
			//��ʾ����
			sprintf(v_buf, "%.1f V", voltage_data);
			LCD1602_show_str(5, 0, v_buf);

			/*******����������********/
			count++;
			if (count > buzzer_time * 10)
				count = buzzer_time + 1;
			if (count % buzzer_time == 0 && bueezr_flag)
				BUZZER = ~BUZZER;//������ȡ��  ����������ʾ
		}
		
		/*******�������õ�λ********/
		//��
		if (Add == 0) {
			Delay(300);
			mode_count++;
		}
		//��
		if (Reduc == 0) {
			Delay(300);
			mode_count--;
		}
		
		if (mode_count < 0)
			mode_count = 2;
		else if (mode_count > 2)
			mode_count = 0;
	}
}
