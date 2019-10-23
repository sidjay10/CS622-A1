//Program to collect true L1 miss address from the input file

#include<stdio.h>
#include<stdlib.h>
#include<assert.h>

int main(int argc, char ** argv)

{
	int numtraces = atoi(argv[2]);
   char iord,type;
   unsigned long long addr;
   unsigned pc;
	char input_name[100];
   char output_name[100];
   FILE* fp1;
   sprintf(output_name, "%s_output", argv[1]);
   fp1 = fopen(output_name,"wb");
	for (int k=0; k<numtraces; k++) {
		FILE* fp;
      sprintf(input_name, "%s_%d", argv[1], k);
      fp = fopen(input_name, "rb");

      assert(fp != NULL);
      assert(fp1!=NULL);

      while (!feof(fp)) {
         fread(&iord, sizeof(char), 1, fp);
         fread(&type, sizeof(char), 1, fp);
         fread(&addr, sizeof(unsigned long long), 1, fp);
         fread(&pc, sizeof(unsigned), 1, fp);
         if((int)type != 0){
            fwrite(&addr, sizeof(unsigned long long), 1, fp1);
         }
         	
      }
      fclose(fp);
    
}
   fclose(fp1);
	return 0;
}
