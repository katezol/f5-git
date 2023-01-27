#ifndef LB20380_H_
#define LB20380_H_

typedef struct{
	unsigned char	CS_break_AK630_1	:1; // �� ���������
	unsigned char	CS_break_AK630_2	:1; // �� ���������
	unsigned char	CS_break_AU_A190	:1; // �� ���������
	unsigned char	CS_break_BM_1		:1; // �� ���������
	unsigned char	priority_PVO		:1; // ��������� ���
	unsigned char	priority_URO		:1; // ��������� ���
	unsigned char	pad0				:1;
	unsigned char	pad1				:1;
	
	unsigned char	CS_off_AK630_1		:1; // �� ���������
	unsigned char	CS_off_AK630_2		:1; // �� ���������
	unsigned char	CS_off_AU_A190		:1; // �� ���������
	unsigned char	CS_off_BM_1			:1; // �� ���������
	unsigned char	pad2					:1; // ����� ������� ���
	unsigned char	pad3			:1; // ������ ������ �� ���
	unsigned char	pad4				:1;
	unsigned char	pad5				:1;
	
	unsigned char	Work_AK630_1		:1; // �� ���������
	unsigned char	Work_AK630_2		:1; // �� ���������
	unsigned char	Work_A190		:1; // �� ���������
	unsigned char	Work_BM_1			:1; // �� ���������
	unsigned char	LAK_updown					:1; // ����� ������� ���
	unsigned char	Ls_on_VPP			:1; // ������ ������ �� ���
	unsigned char	LAK				:1;
	unsigned char	pad6				:1;
	
	unsigned char	CS;
	
}B20380_IP_t;

int B20380_Open(char* devname);

#endif /*LB20380_H_*/
