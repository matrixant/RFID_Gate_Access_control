/*
 * UART_FIFO.c
 *
 *  Created on: 2013-3-18
 *      Author: Administrator
 */


#include <msp430.h>
#include "UART_Global.h"
/******************************************************************************************************
 * ��       �ƣ�Rx_FIFO_WriteChar()
 * ��       �ܣ���Rx����FIFO��д1�ֽ�
 * ��ڲ�����Data����д��FIFO������
 * ���ڲ�����1��д�����ݳɹ���0��д������ʧ��
 * ˵       ��������FIFOʱ��Ҫ�ر����ж�
 * ��       ������
 ******************************************************************************************************/
char Rx_FIFO_WriteChar(unsigned char data)
{
	if(Rx_FIFO_DataNum==RX_FIFO_SIZE) return(0);            //�ж�FIFO�Ƿ���װ��δ�����ݣ����װ������0
	__disable_interrupt();                      			//����FIFOǰһ��Ҫ�����ж�
	Rx_FIFO_DataNum++;                          			//δ��ȡ���ݸ�����һ
	Rx_FIFO[Rx_FIFO_IndexW]=data;               			//������д��д��ָ��λ�õ�FIFO����
	Rx_FIFO_IndexW++;                           			//дָ����λ
	if (Rx_FIFO_IndexW>=RX_FIFO_SIZE)           			//�ж�ָ���Ƿ�Խ��
		Rx_FIFO_IndexW=0;               	    			//дָ��ѭ������
	__enable_interrupt();                       			//�ָ����ж�ʹ��
	return(1);												//���سɹ�
}
/******************************************************************************************************
 * ��       �ƣ�Rx_FIFO_ReadChar()
 * ��       �ܣ���Rx����FIFO�ж�1�ֽ�
 * ��ڲ�����*Chr��������ֽڱ�����ָ��
 * ���ڲ�������1����ȡ���ݳɹ�����0����ȡ����ʧ��
 * ˵       ��������FIFOʱ��Ҫ�ر����ж�
 * ��       ������
 ******************************************************************************************************/
unsigned char Rx_FIFO_ReadChar()
{
	if(Rx_FIFO_DataNum==0) return(0);	        //�ж�FIFO���Ƿ���δ�����ݣ����û�з���0
	unsigned char temp;
    __disable_interrupt();						//����FIFOǰһ��Ҫ�����ж�
	Rx_FIFO_DataNum--;							//����ȡ���ݸ�����һ
	temp=Rx_FIFO[Rx_FIFO_IndexR];				//����ָ��λ�õ�FIFO���ݸ���ָ����ָ����
	Rx_FIFO_IndexR++;							//��ָ����λ
	if (Rx_FIFO_IndexR>=RX_FIFO_SIZE) 		    //�ж�ָ���Ƿ�Խ��
		Rx_FIFO_IndexR=0;						//��ָ��ѭ������
	__enable_interrupt();						//�ָ����ж�ʹ��
	return temp;
}

/******************************************************************************************************
 * ��       �ƣ�Rx_FIFO_Clear()
 * ��       �ܣ����Rx����FIFO��
 * ��ڲ�������
 * ���ڲ�������
 * ˵       ������ղ�����Ҫ���ȥ��FIFOÿһ���ֽڵ�����д0��
 * 					ֻ���дָ������Ϳ����������㼴�ɡ�
 * ��       ������
 ******************************************************************************************************/
void Rx_FIFO_Clear()
{
	__disable_interrupt();				    //����FIFOǰһ��Ҫ�����ж�
	Rx_FIFO_DataNum=0;						//FIFO��δ��ȡ������Ŀ����
	Rx_FIFO_IndexR=0;						//FIFO��ģ���ָ������
	Rx_FIFO_IndexW=0;						//FIFO��ģ��дָ������
	__enable_interrupt();					//�ָ����ж�ʹ��
}
/******************************************************************************************************
 * ��       �ƣ�Tx_FIFO_WriteChar()
 * ��       �ܣ���Tx����FIFO��д1�ֽ�
 * ��ڲ�����Data����д��FIFO������
 * ���ڲ�����1��д�����ݳɹ���0��д������ʧ��
 * ˵       ������ȫ�¡�һ�η������ݱ����ֶ�����Tx�жϣ�����ȫ�¡�����һ�������ֶ�����Tx�жϡ�
 * 						ȫ�·��͵��оݱ���ͬʱ����FIFO�����ݺ�Tx��Busy����������
 * ��       ������
 ******************************************************************************************************/
char Tx_FIFO_WriteChar(unsigned char data)
{
    if(Tx_FIFO_DataNum==TX_FIFO_SIZE) return(0);		    //�ж�FIFO�Ƿ���װ��δ�����ݣ����װ������0
    __disable_interrupt();       	            			//����FIFOǰһ��Ҫ�����ж�
	//-----��ȫ�¡�һ�η������ݱ����ֶ�����Tx�ж�-----
	if((Tx_FIFO_DataNum==0) &&(!(UCA0STATW & UCBUSY)))		//�ж��Ƿ�Ϊһ�Ρ�ȫ�¡�����
        UCA0IFG |= UCTXIFG;									// �ֶ�����һ��
        Tx_FIFO_DataNum++;                             		//δ��ȡ���ݸ�����һ
        Tx_FIFO[Tx_FIFO_IndexW]=data;                  		//������д��д��ָ��λ�õ�FIFO����
        Tx_FIFO_IndexW++;                               	//дָ����λ
	if (Tx_FIFO_IndexW >= TX_FIFO_SIZE)                		//�ж�ָ���Ƿ�Խ��
        Tx_FIFO_IndexW=0;                              		//дָ��ѭ������
    __enable_interrupt();                              		//�ָ����ж�ʹ��
	return(1);												//���سɹ�
}
/******************************************************************************************************
 * ��       �ƣ�Tx_FIFO_ReadChar()
 * ��       �ܣ���Tx����FIFO�ж�1�ֽ�
 * ��ڲ�����*Chr��������ֽڱ�����ָ��
 * ���ڲ�������1����ȡ���ݳɹ�����0����ȡ����ʧ��
 * ˵       ��������FIFOʱ��Ҫ�ر����ж�
 * ��       ������
 ******************************************************************************************************/
unsigned char Tx_FIFO_ReadChar()
{
    if(Tx_FIFO_DataNum==0) return(0);		                //�ж�FIFO���Ƿ���δ�����ݣ����û�з���0
    unsigned char tmp;
    __disable_interrupt();                 			        //����FIFOǰһ��Ҫ�����ж�
    Tx_FIFO_DataNum--;                     		        	//����ȡ���ݸ�����һ
    tmp=Tx_FIFO[Tx_FIFO_IndexR];                          	//����ָ��λ�õ�FIFO���ݸ���ָ����ָ����
    Tx_FIFO_IndexR++;                      			        //��ָ����λ
    if (Tx_FIFO_IndexR>=TX_FIFO_SIZE)                       //�ж�ָ���Ƿ�Խ��
	Tx_FIFO_IndexR=0;                  						//��ָ��ѭ������
    __enable_interrupt();                  	        		//�ָ����ж�ʹ��
    return tmp;												//���سɹ�
}

/******************************************************************************************************
 * ��       �ƣ�Tx_FIFO_Clear()
 * ��       �ܣ����Tx����FIFO��
 * ��ڲ�������
 * ���ڲ�������
 * ˵       ������ղ�����Ҫ���ȥ��FIFOÿһ���ֽڵ�����д0��
 * 					ֻ���дָ������Ϳ����������㼴�ɡ�
 * ��       ������
 ******************************************************************************************************/
void Tx_FIFO_Clear()
{
    __disable_interrupt();        	                                //����FIFOǰһ��Ҫ�����ж�
    Tx_FIFO_DataNum=0;                                              //FIFO��δ��ȡ������Ŀ����
    Tx_FIFO_IndexR=0;             		                        //FIFO��ģ���ָ������
    Tx_FIFO_IndexW=0;            	                                //FIFO��ģ��дָ������
    __enable_interrupt();         	                                //�ָ����ж�ʹ��
}