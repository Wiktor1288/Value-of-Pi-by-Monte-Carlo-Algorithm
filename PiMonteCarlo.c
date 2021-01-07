#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#define NumberOfThreads 10
#define NumberOfIterations 2000000

#include <time.h>
/*
########################################################################
#                                                                      #
# This program approximates value of PI by Monte Carlo algorithm.      #
# To solve this problem I used threds and final value of PI is an      #
# arithmetic average.                                                  #
# Author: Wiktor Springer                                              #
# Date: 07.01.2021r                                                    #
########################################################################                                                    
*/

struct RandomPoint{

    int InCircle;
    int InSquare;
    double x;
    double y;
};

void *MonteCarloAlgorithm(void *PointStructer){
    struct RandomPoint *Point1;
    int i=0;
    Point1= (struct RandomPoint *)PointStructer;
    Point1->InCircle=0;
    Point1->InSquare=0;
    srand48(time(0));
    for(i=0; i<NumberOfIterations; i++){
        Point1->x=drand48();
        Point1->y=drand48();
        Point1->InSquare++;
        if((pow((Point1->x-0.5),2)+pow((Point1->y-0.5),2)) <= 0.25){
            Point1->InCircle++;
        }

    }
    pthread_exit(NULL);
}

int main(){
    int result, i=0; 
    void *status;
    double ValuePi=0.0, Sum=0.0, Results[NumberOfThreads]={0};
    pthread_t TabOfThreads[NumberOfThreads];  
    struct RandomPoint PointWithData[NumberOfThreads];
    srand48(time(0));
    
    for( i=0; i<NumberOfThreads; i++){
		result = pthread_create(&TabOfThreads[i], NULL, MonteCarloAlgorithm, (void *) &PointWithData[i]);
        if (result != 0) {
            perror("Could not create thread.");
            exit(-1);
        }
    }

    for( i=0; i<NumberOfThreads; i++){
        result = pthread_join(TabOfThreads[i], &status);
        if (result != 0) {
            printf("ERROR; return code from pthread_join() is %d\n", result);
            exit(-1);
         }
         printf("Result from thread number: %d\n", i+1);
         printf("In circle %d\n", PointWithData[i].InCircle);
		 printf("In square %d\n", PointWithData[i].InSquare);
         ValuePi=4.0*(double)PointWithData[i].InCircle/(double)PointWithData[i].InSquare;
         printf("Value PI is %f\n", ValuePi);
         Sum+=ValuePi;
    }
	printf("\n\nAverage value:\n");	
	printf("Result %f \n", Sum/NumberOfThreads);
	pthread_exit(NULL);


}
