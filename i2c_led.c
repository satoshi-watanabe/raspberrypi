#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <linux/i2c-dev.h>	//I2C用
#include <wiringPi.h>		//GPIO操作用

//define
#define I2C_FILE_NAME		"/dev/i2c-1"
#define	I2C_ADDRESS		(0x18)
#define PRESSURE_ADDRESS	(0x28)

#define	LED_PORT0		7	//GPIO7  led 0bit
#define	LED_PORT1		8	//GPIO8  led 1bit
#define LED_PORT2		25	//GPIO25 led 2bit
#define LED_PORT3		24	//GPIO24 led 3bit
#define	LED_ON			1
#define LED_OFF			0

#define	PRESSURE_LEVEL_FULL	(256*4)
#define	PRESSURE_LEVEL_3	(256*3)
#define PRESSURE_LEVEL_2	(256*2)
#define PRESSURE_LEVEL_1	(0xff*1)
#define PRESSURE_LEVEL_EMP	0
#define PRESSURE_EMP_THRESH	(0x01)

//private function
int getPressureValue(int);
void offLED_All(void);
void onLED_firstLevel(void);
void onLED_secondLevel(void);
void onLED_thirdLevel(void);
void onLED_maxLevel(void);

int main(int argc, char **argv)
{
	int i2c_fd;	//discripter for device file
	int pres=0;	//pressure value

	//device file open

	if ( (i2c_fd = open(I2C_FILE_NAME, O_RDWR)) < 0 ){
		printf("Failed to open i2c port\n");
		exit(1);
	}

	//address set
	if ( ioctl(i2c_fd, I2C_SLAVE, I2C_ADDRESS) < 0 ){
		printf("Unable to get bus access to talk to slave\n");
		exit(1);
	}

	//device initialize
	//必要ならば

	//wirinPi initialize
	if ( wiringPiSetupGpio() == -1 ){
		printf("WiringPi initilize file\n");
		exit(1);
	}
	pinMode(LED_PORT0, OUTPUT);
	pinMode(LED_PORT1, OUTPUT);
	pinMode(LED_PORT2, OUTPUT);
	pinMode(LED_PORT3, OUTPUT);

	//メインタスク
	while(1){
		//get value from device
		pres = getPressureValue(i2c_fd);

		//light LED
			if( (PRESSURE_LEVEL_EMP <= pres) && (pres < PRESSURE_EMP_THRESH) ){
				offLED_All();
			} else if ( (PRESSURE_EMP_THRESH <= pres) && (pres < PRESSURE_LEVEL_1) ){
				onLED_firstLevel();
/*			} else if ( (PRESSURE_LEVEL_1 <= pres) && (pres < PRESSURE_LEVEL_2) ){
				onLED_secondLevel();
			} else if ( (PRESSURE_LEVEL_2 <= pres) && (pres < PRESSURE_LEVEL_3) ){
				onLED_thirdLevel();
			} else if ( (PRESSURE_LEVEL_3 <= pres) && (pres < PRESSURE_LEVEL_FULL) ){
				onLED_maxLevel();
*/			}
			delay(500); //なんとなく

		//停止条件
	}
}

int getPressureValue(int fd)
{
	unsigned char dat;

	//圧力ゲージの値を読み込みましょう
 	if(read(fd,&dat,1)!=1){
 		printf("Error reading from i2c slave\n");
 		exit(1);
	}
 
	return dat;
}


void offLED_All(void)
{
	digitalWrite(LED_PORT0, LED_OFF);
	digitalWrite(LED_PORT1, LED_OFF);
	digitalWrite(LED_PORT2, LED_OFF);
	digitalWrite(LED_PORT3, LED_OFF);
}

void onLED_firstLevel(void)
{
	digitalWrite(LED_PORT0, LED_ON);
	digitalWrite(LED_PORT1, LED_OFF);
	digitalWrite(LED_PORT2, LED_OFF);
	digitalWrite(LED_PORT3, LED_OFF);
}

void onLED_secondLevel(void)
{
	digitalWrite(LED_PORT0, LED_ON);
	digitalWrite(LED_PORT1, LED_ON);
	digitalWrite(LED_PORT2, LED_OFF);
	digitalWrite(LED_PORT3, LED_OFF);
}

void onLED_thirdLevel(void)
{
	digitalWrite(LED_PORT0, LED_ON);
	digitalWrite(LED_PORT1, LED_ON);
	digitalWrite(LED_PORT2, LED_ON);
	digitalWrite(LED_PORT3, LED_OFF);
}

void onLED_maxLevel(void)
{
	digitalWrite(LED_PORT0, LED_ON);
	digitalWrite(LED_PORT1, LED_ON);
	digitalWrite(LED_PORT2, LED_ON);
	digitalWrite(LED_PORT3, LED_ON);
}

