
//========================================================================================

int32 remo_SYS_BASE_LENGTH;

uint8 BASE[C_remo_SYS_BASE_LENGTH];

//ϵͳ��ʼ��
void remo_SYS_Init()
{
	remo_SYS_BASE_LENGTH = C_remo_SYS_BASE_LENGTH;
}
//����ȫ���ж�
void remo_SYS_interrupt_enable()
{
	//🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧
	// __enable_irq();
}
//�ر�ȫ���ж�
void remo_SYS_interrupt_disable()
{
	//🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧🧧
	//__disable_irq();
}

//========================================================================================

#define UART_DEFAULT_ID 0

#include "UART_List.c"

uint8_t uart_send_buff[10];

//��ʼ������0
void remo_USART0_Init(uint32 BaudRate)
{
    hi_uart_attribute uart_attr = {
        .baud_rate = BaudRate,
        //data_bits: 8bits
        .data_bits = 8,
        .stop_bits = 1,
        .parity = 0,
    };
	IoTGpioInit(HI_IO_NAME_GPIO_3);
    IoTGpioSetDir(HI_IO_NAME_GPIO_3, IOT_GPIO_DIR_OUT);
    IoTGpioInit(HI_IO_NAME_GPIO_4);
    IoTGpioSetDir(HI_IO_NAME_GPIO_4, IOT_GPIO_DIR_IN);
    
    hi_io_set_func(HI_IO_NAME_GPIO_3, HI_IO_FUNC_GPIO_3_UART0_TXD); /* uart0 tx */ 
    hi_io_set_func(HI_IO_NAME_GPIO_4, HI_IO_FUNC_GPIO_4_UART0_RXD); /* uart0 rx */ 

	uint32_t ret = hi_uart_init( HI_UART_IDX_0, &uart_attr, NULL );
    if ( ret != HI_ERR_SUCCESS ) {
        printf("Failed to init uart! Err code = %d\n", ret);
        return;
    }
}
void remo_USART1_Init(uint32 BaudRate)
{
    hi_uart_attribute uart_attr = {
        .baud_rate = BaudRate,
        //data_bits: 8bits
        .data_bits = 8,
        .stop_bits = 1,
        .parity = 0,
    };
	IoTGpioInit(HI_IO_NAME_GPIO_6);
    IoTGpioSetDir(HI_IO_NAME_GPIO_6, IOT_GPIO_DIR_OUT);
    IoTGpioInit(HI_IO_NAME_GPIO_5);
    IoTGpioSetDir(HI_IO_NAME_GPIO_5, IOT_GPIO_DIR_IN);
    
    hi_io_set_func(HI_IO_NAME_GPIO_6, HI_IO_FUNC_GPIO_6_UART1_TXD); /* uart0 tx */ 
    hi_io_set_func(HI_IO_NAME_GPIO_5, HI_IO_FUNC_GPIO_5_UART1_RXD); /* uart0 rx */ 

	uint32_t ret = hi_uart_init( HI_UART_IDX_1, &uart_attr, NULL );
    if ( ret != HI_ERR_SUCCESS ) {
        printf("Failed to init uart! Err code = %d\n", ret);
        return;
    }
}
void remo_USART2_Init(uint32 BaudRate)
{
    hi_uart_attribute uart_attr = {
        .baud_rate = BaudRate,
        //data_bits: 8bits
        .data_bits = 8,
        .stop_bits = 1,
        .parity = 0,
    };
	IoTGpioInit(HI_IO_NAME_GPIO_11);
    IoTGpioSetDir(HI_IO_NAME_GPIO_11, IOT_GPIO_DIR_OUT);
    IoTGpioInit(HI_IO_NAME_GPIO_12);
    IoTGpioSetDir(HI_IO_NAME_GPIO_12, IOT_GPIO_DIR_IN);

    //Initialize uart driver
    hi_io_set_func(HI_IO_NAME_GPIO_11, HI_IO_FUNC_GPIO_11_UART2_TXD); /* uart2 tx */ 
    hi_io_set_func(HI_IO_NAME_GPIO_12, HI_IO_FUNC_GPIO_12_UART2_RXD); /* uart2 rx */ 

	uint32_t ret = hi_uart_init( HI_UART_IDX_2, &uart_attr, NULL );
    if ( ret != HI_ERR_SUCCESS ) {
        printf("Failed to init uart! Err code = %d\n", ret);
        return;
    }
}
//��ʼ������0
void remo_USART_Init( uint32 BaudRate )
{ 
	//ͨ�ó�ʼ��
	UART_List_Init();

	//用于下载程序
	remo_USART0_Init( BaudRate );
	U0_isOpen = true;
}
//--------------------------------------------------
//������������
void remo_USART_set( int8 ID, uint32 addr, int16 max, int32 len_addr )
{
	if( ID == 0 ) {
		UART_List_SetAddr0( addr, max, len_addr );
		remo_USART0_Init( 115200 );
	}
	if( ID == 1 ) {
		UART_List_SetAddr1( addr, max, len_addr );
		remo_USART1_Init( 115200 );
	}
	if( ID == 2 ) {
		UART_List_SetAddr2( addr, max, len_addr );
		remo_USART2_Init( 115200 );
	}
}
//���ڲ�����
void remo_USART_Baudrate( int8 ID, uint32 b )
{
	if( ID == 0 ) {
		remo_USART0_Init( b );
	}
	if( ID == 1 ) {
		remo_USART1_Init( b );
	}
	if( ID == 2 ) {
		remo_USART2_Init( b );
	}
}
//�����ֽ�
void remo_USART_write_byte( int8 ID, uint8 tempc )
{
	if( ID == 0 ) {
		uart_send_buff[0] = tempc;
		hi_uart_write( HI_UART_IDX_0, uart_send_buff, 1 );
	}
	if( ID == 1 ) {
		uart_send_buff[0] = tempc;
		hi_uart_write( HI_UART_IDX_1, uart_send_buff, 1 );
	}
	if( ID == 2 ) {
		uart_send_buff[0] = tempc;
		hi_uart_write( HI_UART_IDX_2, uart_send_buff, 1 );
	}
}

//====================================================================================
//系统定时器调度参数
volatile uint32 start;
volatile uint32 ntime;
volatile uint32 Timer_1msC;
volatile uint32 Timer_1usC;

//计数器参数
static uint32 c_start;

void Timer_Init()
{
	ntime = 0;
	Timer_1msC = 1000;
	Timer_1usC = 1;
}

void Timer_SetTimeMs( uint16 t )
{
	ntime = t;
	start = (uint32)hi_get_us();
}

bool Timer_isTimeOut()
{
	uint32 delta_mtime = (uint32)hi_get_us() - start;
	return delta_mtime >= (Timer_1msC * ntime);
}

//这里增加读取计数值的函数和读取每微秒的计数值函数, 相除即可得经过的微秒数
void Timer_start_count()
{
	c_start = (uint32)hi_get_us();
}
uint32 Timer_get_count()
{
	return (uint32)hi_get_us() - c_start;
}
uint32 Timer_get_1us_count()
{
	return Timer_1usC;
}

//========================================================================================

#define NUMBER HI_GPIO_IDX_MAX

uint32 GPIO_MODE_List[NUMBER] = {
	HI_IO_FUNC_GPIO_0_GPIO,
	HI_IO_FUNC_GPIO_1_GPIO,
	HI_IO_FUNC_GPIO_2_GPIO,
	HI_IO_FUNC_GPIO_3_GPIO,
	HI_IO_FUNC_GPIO_4_GPIO,
	HI_IO_FUNC_GPIO_5_GPIO,
	HI_IO_FUNC_GPIO_6_GPIO,
	HI_IO_FUNC_GPIO_7_GPIO,
	HI_IO_FUNC_GPIO_8_GPIO,
	HI_IO_FUNC_GPIO_9_GPIO,
	HI_IO_FUNC_GPIO_10_GPIO,
	HI_IO_FUNC_GPIO_11_GPIO,
	HI_IO_FUNC_GPIO_12_GPIO,
	HI_IO_FUNC_GPIO_13_GPIO,
	HI_IO_FUNC_GPIO_14_GPIO,
};

void IO_Init()
{
	hi_gpio_init();

	for( uint8 i = 0; i < NUMBER; ++i ) {
		hi_gpio_set_dir( i, IOT_GPIO_DIR_IN );
		hi_io_set_pull( i, HI_IO_PULL_UP );
		hi_io_set_func( i, GPIO_MODE_List[i] );
	}
}

void IO_Close()
{
	
}
void IO_DirWrite( uint8 i, uint8 d )
{
	hi_io_set_func( i, GPIO_MODE_List[i] );

	if( d == 0 ) { // GPIO_MODE_IPD ���ɵ�·ģʽ
		hi_gpio_set_dir( i, IOT_GPIO_DIR_IN );
	}
	else {
		hi_gpio_set_dir( i, IOT_GPIO_DIR_OUT );
	}
}
void IO_PullWrite( uint8 i, uint8 d )
{
	hi_io_set_func( i, GPIO_MODE_List[i] );
	hi_io_set_pull( i, HI_IO_PULL_UP );
}

void IO_OutWrite( uint8 i, uint8 d )
{
	//A3 A4需要加上这两行指令, 研究下为什么
	//hi_io_set_func( i, GPIO_MODE_List[i] );

	if( d == 0 ) {
		hi_gpio_set_ouput_val( i, 0 );
	}
	else {
		hi_gpio_set_ouput_val( i, 1 );
	}

}
void IO_OutWrite_0( uint8 i )
{
	hi_gpio_set_ouput_val( i, 0 );
}
void IO_OutWrite_1( uint8 i )
{
	hi_gpio_set_ouput_val( i, 1 );
}
uint8 IO_OutRead( uint8 i )
{
	hi_gpio_value hv;
	hi_gpio_get_output_val( i, &hv );
	return hv;
}
uint8 IO_InRead( uint8 i )
{
	hi_gpio_value hv;
	hi_gpio_get_input_val( i, &hv );
	return hv;
}
//-------------------------------------------------
int8 IO_AnalogGetID( uint8 i )
{
	/*
	IO4  -> ADC1
	IO5  -> ADC2
	IO7  -> ADC3
	IO9  -> ADC4
	IO11 -> ADC5
	IO12 -> ADC0
	IO13 -> ADC6
	*/
	switch( i ) {
		case 4: return 1;
		case 5: return 2;
		case 7: return 3;
		case 9: return 4;
		case 11: return 5;
		case 12: return 0;
		case 13: return 6;
		default: return -1;
	}
}
void IO_AnalogOpen( uint8 i, uint8 d )
{
	int8 j = IO_AnalogGetID( i );
	if( j == -1 ) {
		return;
	}
	hi_gpio_set_dir( (uint8)j, IOT_GPIO_DIR_IN );
	hi_io_set_pull( (uint8)j, HI_IO_PULL_NONE );
	hi_io_set_func( (uint8)j, GPIO_MODE_List[i] );
}
int32 IO_AnalogRead( uint8 i )
{
	int8 j = IO_AnalogGetID( i );
	if( j == -1 ) {
		return;
	}
	uint16 data = 0;
	uint32 ret = hi_adc_read( (hi_adc_channel_index)j, &data, HI_ADC_EQU_MODEL_1, HI_ADC_CUR_BAIS_3P3V, 0 ); //HI_ADC_CUR_BAIS_DEFAULT
	if( ret != HI_ERR_SUCCESS ) {
		return 0;
	}
	uint32 ddd = data;
	return (int)ddd / 2;
}

//========================================================================================

//参考示例里边设定为常量 #define TEST_FLASH_OFFSET 0x001FF000

//读取到的地址是 0x001F0000, SIZE是 0x00005000
hi_u32 usr_partion_addr;
hi_u32 usr_partion_size;

#define CODE_LEN 0x8000
uint8 CodeList[CODE_LEN] __attribute__((aligned(32)));

uint8 temp[100] __attribute__((aligned(32)));

void Flash_Init()
{
	Write4Byte = true; //F303������false, ��ΪҪ����E103��Ϊ��true

	hi_flash_init();

	hi_flash_partition_init();

	//获取用户分区
    hi_get_usr_partition_table( &usr_partion_addr, &usr_partion_size );

	//usr_partion_size += 0x2000;
	usr_partion_size += 0x1800;

	//获取文件系统分区
	//hi_get_fs_partition_table( &usr_partion_addr, &usr_partion_size );
/*
	remo_USART_write( 0xAA );remo_USART_write( 0xAB );remo_USART_write( 0xAC );remo_USART_write( 0xAD );

	remo_USART_write( usr_partion_addr%256 );usr_partion_addr >>= 8;
	remo_USART_write( usr_partion_addr%256 );usr_partion_addr >>= 8;
	remo_USART_write( usr_partion_addr%256 );usr_partion_addr >>= 8;
	remo_USART_write( usr_partion_addr%256 );usr_partion_addr >>= 8;

	remo_USART_write( usr_partion_size%256 );usr_partion_size >>= 8;
	remo_USART_write( usr_partion_size%256 );usr_partion_size >>= 8;
	remo_USART_write( usr_partion_size%256 );usr_partion_size >>= 8;
	remo_USART_write( usr_partion_size%256 );usr_partion_size >>= 8;
*/
}

void Flash_Load()
{
	hi_flash_read( usr_partion_addr, usr_partion_size, CodeList );
}

void Flash_Save()
{
	hi_flash_write( usr_partion_addr, usr_partion_size, CodeList, true );
	//hi_flash_write( usr_partion_addr + 0x5000, 0x2000, CodeList + 0x5000, true );
}

//��ȡFlash�ߴ�, ��λ��KB
int32 Flash_GetSize()
{
	return usr_partion_size / 1024;
}

void Flash_Unlock()
{
	
}

void Flash_Lock()
{
	
}

void Flash_Clear()
{
	
}

void Flash_Write2Byte( uint32 addr, uint16 b )
{
	
}

void Flash_Write4Byte( uint32 addr, uint32 b )
{
	CodeList[addr] = (uint8)b; addr++; b >>= 8;
	CodeList[addr] = (uint8)b; addr++; b >>= 8;
	CodeList[addr] = (uint8)b; addr++; b >>= 8;
	CodeList[addr] = (uint8)b;
}

uint8 Flash_ReadByte( uint32 addr )
{
	return CodeList[addr];
}

uint32 Flash_ReadUint32( uint32 addr )
{
	return *(uint32*)(CodeList + addr);
}
uint8* Flash_GetAddr( uint32 addr )
{
	return (uint8*)(CodeList + addr);
}
//========================================================================================

hi_u32 drv_timer;
bool remo_DrvTimer_open;

uint32 remo_DrvTimer_temp;
uint8 remo_DrvTimer_tick;

bool remo_DrvTimer_running;

//��ʼ����ʱ��
void remo_DrvTimer_Init()
{
	// create timer handle
    hi_u32 ret = hi_hrtimer_create( &drv_timer );
	if( ret != HI_ERR_SUCCESS ) {
		printf("=====ERROR===== hrtimer handle create ret is: %d !!\r\n", ret);
		return;
	}
	remo_DrvTimer_open = false;
	remo_DrvTimer_running = false;

	remo_DrvTimer_tick = 0;
}
static hi_void hrtimer_callback( hi_u32 data )
{
	if( remo_DrvTimer_running ) {
		return;
	}
	//必须要放到最前边, 才能保证时间精度
	if( remo_DrvTimer_open ) {
		int ret = hi_hrtimer_start( drv_timer, 100, hrtimer_callback, 0 );
	}
	remo_DrvTimer_running = true;

	//100KHz 部分模块可能有误差
	remo_tick_run10us();

	remo_tick_run100us();

	remo_DrvTimer_tick++;
	if( remo_DrvTimer_tick >= 10 ) {
		remo_DrvTimer_tick = 0;
		remo_tick_run1000us();
	}
	remo_DrvTimer_running = false;


	remo_DrvTimer_temp++;
	if( remo_DrvTimer_temp == 10000 ) {
		remo_DrvTimer_temp = 0;
		//remo_USART_write( 'A' );
	}

}
//������ʱ��
void remo_DrvTimer_Start()
{
	remo_DrvTimer_open = true;
	int ret = hi_hrtimer_start( drv_timer, 100, hrtimer_callback, 0 );
}
//�رն�ʱ��
void remo_DrvTimer_Stop()
{
	remo_DrvTimer_open = false;
	hi_hrtimer_stop( drv_timer );
}






