

#include <stdio.h>
#include <math.h>


int main(){
	
	int LIMIT = 10000000;
	
	int nrOfPrimes = 0;
	int boolean = 0;
	
	for(int i = 2; i < LIMIT; i++){
		
		int j = 2;
		boolean = 1;
		
		for(; j <= sqrt(i); j++){
			
			if( (i%j) == 0 ){
				
				boolean = 0;
				break;
				
			}
			
		}
		
		if(boolean){
			
			nrOfPrimes++;
			
		}
		
	}
	
	printf("Number of primes between:%d - %d = %d",1,LIMIT,nrOfPrimes);
	
	return 0;
	
}

