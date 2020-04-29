/*
*
* axi-ddr-access app
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <time.h>
//typedef  uint32_t uint32_t;

#define printf(...) \
 fprintf(stdout, __VA_ARGS__); \
 fflush(stdout);

#define ddr_SIZE 0x32000
#define ddr_READ_INTERVAL 0x4
#define ddr_BYTES_PER_WORD 0x4
int sharedmemreadback(void);

long currentTimeMillis() {
  struct timeval time;
  gettimeofday(&time, NULL);

  return time.tv_sec * 1000 + time.tv_usec / 1000;
}


int sharedmemreadback()
{
	off_t ddr_pbase[] = { 0x1cf00000 };
	unsigned int ddr_segments = sizeof(ddr_pbase)/sizeof(ddr_pbase[0]);
   	uint32_t *ddr64_vptr, testval;
   	volatile  *read64_vptr;

   	int fd, ddr_index, i;

  	fprintf(stdout, "--------------------------------\n");
  	fprintf(stdout, "- Total ddr Segments: %d\n", ddr_segments);
  	fprintf(stdout, "--------------------------------\n");
	for(i=0;i<ddr_segments;i++) {
		fprintf(stdout, "- ddr %3d: [%x]\n",i,ddr_pbase[i]);
	}
  	fprintf(stdout, "--------------------------------\n");
	fflush(stdout);

	// Map the ddr physical address into user space getting a virtual address for it
   	if ((fd = open("/dev/mem", O_RDWR | O_SYNC)) != -1) {

		// Loop through ddr segments
		for ( ddr_index = 0; ddr_index < ddr_segments; ddr_index++) {

	      	//Init ddr segment map
	      	ddr64_vptr = (uint32_t *)mmap(NULL, ddr_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, ddr_pbase[ddr_index]);

			if ((ddr64_vptr < 0) || (ddr64_vptr == 0xffffffff)) {
				fprintf(stderr, "Errno(%s): mmap failed to map memory at 0x%lx (size: 0x%x)\n", strerror(errno), ddr_pbase[ddr_index], ddr_SIZE);
				continue;
				//return -1;
			}

		    //Fill mapped memory with a visually discernible incrementing value
	      	fprintf(stdout, "--------------------------------\n");
	      	fprintf(stdout, "- ddr %3d:\n- Base Address    [%x]\n- Virtual Address [%x]\n", ddr_index, ddr_pbase[ddr_index], ddr64_vptr);
      		fprintf(stdout, "--------------------------------\n");
			fflush(stdout);

		    // Loop through remaining memory bank and fill with counter value
		//   testval = 0x100004;

		//    printf("- Filling ddr %3d: [%x]\n", ddr_index, &(ddr64_vptr[0]));

		    //Known initial pattern at beginning of ddr segment
		 //   ddr64_vptr[0] = 0xDEADBEAA;
        //
		    // for (i = 2; i < ((ddr_SIZE / ddr_BYTES_PER_WORD) -1); i++) {
		    // 	ddr64_vptr[i] = testval;
		    // 	testval+=0x100004;
		    // }
        //
		    // //Known initial pattern at end of ddr segment
		    // ddr64_vptr[i] = 0xDEFACED0;

      		fprintf(stdout, "--------------------------------\n");

		    // Loop through ddr bank and display values
		   	read64_vptr = ddr64_vptr;

		    printf("Starting ddr %d Read\n", ddr_index);

		    for (i = 0; i < ((ddr_SIZE / ddr_BYTES_PER_WORD) - 1 ); i+=(ddr_READ_INTERVAL/ddr_BYTES_PER_WORD)) {
		    	testval = read64_vptr[i];
//		    	printf("Index[%4d]@[0x%x]=0x%x \n", i, &(read64_vptr[i]), testval);
		    }

		    // read last location in ddr segment
		    i = (ddr_SIZE/ddr_BYTES_PER_WORD) - 1;
		   	testval = read64_vptr[i];
	    	fprintf(stdout, "Index[%4d]@[0x%x]=0x%x \n", i, &(read64_vptr[i]), testval);
        time_t mytime;
    //    		mytime = time(NULL);
      //  		printf(ctime(&mytime));
        printf("%ld\n", currentTimeMillis());
    //    printf("Timestap ddr read end %d\n", (int)time(NULL));

		    printf("\nFinished ddr %d Read\n",ddr_index);
       		fprintf(stdout, "--------------------------------\n");

			if ( munmap(ddr64_vptr, ddr_SIZE) < 0) {
					fprintf(stderr, "Errno(%s): munmap failed to map memory at 0x%lx (size: 0x%x)\n", strerror(errno), ddr_pbase[ddr_index], ddr_SIZE);
				return -1;
			}
		}
	    printf("Closing /dev/mem\n");
	    close(fd);
 	}
}
