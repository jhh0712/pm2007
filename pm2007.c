#include <stdio.h>

#include <string.h>

#include <errno.h>

#include <wiringPi.h>

#include <wiringSerial.h>


 

 

int main ()

{

	int fd ;
 

	if ((fd = serialOpen ("/dev/ttyAMA0", 9600)) < 0)

  	{

    	fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;

    	return 1 ;

  	}

 

  	if (wiringPiSetup () == -1)

  	{

    	fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;

    	return 1 ;

  	}

 

  	

	char send[5] = {0x11, 0x02, 0x0B, 0x01, 0xE1};

	char receive[20];

	char query[100]; 

	unsigned long pm25, pm10;

	

	unsigned char Checksum(void)

	{

		unsigned char count=0, sum=0;

		for(count =0; count < 19; count ++)

		{

			sum += receive[count];

		}

		if(sum >256)

		{	printf("cs : %d\n", (256 - (sum - 256)));

			return (256 - (sum - 256));

		}

		else

		{

			printf("cs : %d\n", 256-sum);

			return (256-sum);

		}

	}

 

	void Send_data()

	{

		int i;

		for (i = 0 ; i < 5; )

 		{

    		

      			fflush (stdout) ;

      			serialPutchar (fd, send[i]) ;

				++i;

    			

		}	

	}

 

 

	while(1)

	{

		Send_data();

    	delay (1000) ;

		int i=0;

  			

			while(serialDataAvail (fd))

    			{

				receive[i++] = serialGetchar(fd);

				printf (" %d,", receive[i]);

      			fflush (stdout) ;

				if(i == 20){ i=0; break;}	

			}



			delay (1000) ;

 

			printf("\n");

			if(Checksum() == receive[19])

			{

				pm25 = (unsigned long)receive[3]<<24 | (unsigned long)receive[4]<<16 | (unsigned long)receive[5]<<8 | (unsigned long)receive[6];

				pm10 = (unsigned long)receive[7]<<24 | (unsigned long)receive[8]<<16 | (unsigned long)receive[9]<<8 | (unsigned long)receive[10];

		

				printf("%d\n", (int)pm25);

				printf("%d\n", (int)pm10);

 

			}else

			{

				printf("error\n");

				continue;

			}

	

	}

	serialClose(fd);

  return 0 ;

}