#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>


//to store all words in a line
char label[4][10];
int location_counter =0;
int literal_no1=1;
int literal_no2=1;

int L=0; // keeping track of the literals
int C=0; // keeping track of the constants
int cnt=0;

int s=0;	// for symbols


FILE  *fp2,*fp3;


struct mnemonic_tab
{
	int mno;
	char mcode[10];
	int ocode; 	// instruction opcode
	int length; // by default it is 1
	char type[5]; // TYPE OF THE STATEMENT;
};

struct mnemonic_tab mtable[20];

struct sym_tab
{
	int sno;  // Symbol no
	char sname[10]; //symbol name
	int saddr;  // symbol Address
	int slength;
};

struct sym_tab symbol_tab[10];


void initialize_mtable()
{
	int i;
	int j=1;
// listing all the assembler directives
	strcpy(mtable[0].mcode, "START");
	strcpy(mtable[1].mcode, "END");
	strcpy(mtable[2].mcode,"ORIGIN");
	strcpy(mtable[3].mcode,"LTORG");
	strcpy(mtable[4].mcode,"EQU");


	for(i = 0; i<=4;i++)
	{
		mtable[i].mno=i;
		mtable[i].ocode=i+1;
		mtable[i].length=0;
		strcpy(mtable[i].type,"AD");
	}

// NOW FOR DECLARATIVE STATMENTS

	strcpy(mtable[5].mcode,"DC");
	strcpy(mtable[6].mcode,"DS");


	while(i<=6)
	{
		mtable[i].mno=i+1;
		mtable[i].ocode=i+1;
		strcpy(mtable[i].type,"DL");
		i++;
	}

// now imerative statemnts
	strcpy(mtable[7].mcode, "MOVER");
	strcpy(mtable[8].mcode, "MOVEM");
	strcpy(mtable[9].mcode, "ADD");
	strcpy(mtable[10].mcode,"MUL");
	strcpy(mtable[11].mcode,"SUB");
	strcpy(mtable[12].mcode, "DIV");
	strcpy(mtable[13].mcode,"BC");
	strcpy(mtable[14].mcode, "COMP ");

	while(i<=14)
	{
		mtable[i].mno=i;
		mtable[i].ocode=j; //jkfjdd  .ocode=j;
		mtable[i].length=1;
		strcpy(mtable[i].type, "IS");
		i++;
		j++;
	}

}

int find(char check[])
{
	int i;

	for(i=0;i<15;i++)
	{
		if( strcmp (check,mtable[i].mcode) == 0 )
			return i;
	}

	if(strcmp(check, "AREG")==0)
		return 51;
	if(strcmp(check, "BREG")==0)
		return 52;
	if(strcmp(check,"CREG")==0)
		return 53;
	if(strcmp(check,"DREG")==0)
		return 54;

	return -1;

}

void label_initialize()
{
	int i; 
	for(i=0;i<4;i++)
	{
		strcpy(label[i]," ");
	}

}

void labels()
{
	
	FILE *fp1,*fp4;
	char temp[30],tmp[10];
	int i=0,j=0;
	int flag=0;
	initialize_mtable();
	
	fp1=fopen("input2.txt","r");
	fp2=fopen("intermediate_code.txt","a+");
	fp3=fopen("literal.txt","a+");
 
	while( !feof(fp1) )
			 //while(2)
	{
		i=0;
		cnt=0;
		L=0;
		C=0;
		fgets(temp,30,fp1);
		label_initialize();
		
		while(temp[i]!='\n') //while(1)
		{
			j=0;
			while(isalnum(temp[i]))
			{
				flag=1;
				tmp[j]=temp[i];
			
				if(isdigit(tmp[0]) && temp[i-1]=='\'') //For literals
						L=1;
				else if(isdigit(tmp[0]) && temp[i-1]==' ') //For Constants
						C=1;
		
				i++;
				j++;
			}
			if(flag==1)
			{
				flag=0;
				tmp[j]='\0';
				strcpy(label[cnt],tmp);
				cnt++;
				j=0;
			}

			if(temp[i]=='\n')
				break;

			i++;
			}//end of while(1)
			
			pass1();

		}//end of while(2)

	fclose(fp1);
	fclose(fp2);
	fclose(fp3);

	fp4=fopen("symbol.txt","w");

	fputs("No\tSymbol\t\tlength\t\tAddress\n\n",fp4);

	for(i=0;i<s;i++)
	{
		fprintf(fp4,"%d\t",symbol_tab[i].sno);
		fprintf(fp4,"%s\t\t",symbol_tab[i].sname);
		fprintf(fp4,"%d\t\t",symbol_tab[i].slength);
		fprintf(fp4,"%d\n\n",symbol_tab[i].saddr);
	}
	
	fclose(fp4);
}

void pass1()
{
	int check,count=0,value=0,length=1,flag=0,i=0;
	char c;

	while(count<cnt)
	{
		check=find(label[count]);
		if(isdigit(label[count][0])) //To convert a string with digits
			value=atoi(label[count]);//into integer value

		if(check==-1 && isalpha(label[count][0]) ) //for symbols
		{
			for(i=0;i<s;i++)
			{
				if(strcmp(label[count],symbol_tab[i].sname)==0)
				{
					symbol_tab[i].saddr=location_counter;
				
					flag=1;
					break;
				}
			}
			if(flag==0)
			{	
				symbol_tab[s].sno=s+1;
				strcpy(symbol_tab[s].sname,label[count]);
				symbol_tab[s].saddr=location_counter;
				symbol_tab[s].slength=length;
				
				s++;
			}

			if(count!=0)
			{					//To print symbols
				c='S';
				fprintf(fp2,"\t%c\t",c);
				if(flag==0)
					fprintf(fp2,"%d",s);	
				else
					fprintf(fp2,"%d",symbol_tab[i].sno);
			}
		}
		else if(check==0) //for Start(AD)
		{
			fprintf(fp2,"\t");
			fprintf(fp2,"%s\t",mtable[check].type);
			fprintf(fp2,"%d",mtable[check].ocode);
			count++;
	
			value=atoi(label[count]);
 
			location_counter=value;
			c='C';
			fprintf(fp2,"\t\t%c\t",c);
			fprintf(fp2,"%d",value);
			location_counter--;
		}
		else if(check>=1 && check<=4)     //for Assembler Directives
		{
			if(check==4)//for equ AD
			{
				count++;
				for(i=0;i<s;i++)
				{
					if(strcmp(label[count],symbol_tab[i].sname)==0)
							break;
				}
			symbol_tab[s-1].saddr=symbol_tab[i].saddr;
			}
			location_counter--;
			break;
		}
		else if(check>=7 && check<=14)//for Imperative Statements
		{
			fprintf(fp2,"%d",location_counter); //location counter
		
			fprintf(fp2,"\t");
			fprintf(fp2,"%s\t",mtable[check].type); //IS
			fprintf(fp2,"%d",mtable[check].ocode); //Opcode
		}
		else if(check==5 && check==6)//for Declarative Statements
		{
			fprintf(fp2,"%d",location_counter); //location counter
		
			fprintf(fp2,"\t");
			fprintf(fp2,"%s\t",mtable[check].type); //DL
			fprintf(fp2,"%d",mtable[check].ocode); //Opcode
		}

		else if(check>=50 && check<=54) //for registers(A,B,C,D)
		{				//register number
			fprintf(fp2,"\t");
			fprintf(fp2,"%d",check-50);
		}
		else if(isdigit(label[count][0]) && count==0) //For literal table
		{
			fprintf(fp3,"%d \t",literal_no2); //To put in literal text file
			fprintf(fp3,"%d\t\t",value);
			fprintf(fp3,"%d\n\n",location_counter);
 
			fprintf(fp2,"%d",location_counter);//To put in IC file
			
			fprintf(fp2,"\t\t\t");
			fprintf(fp2,"\t00%d",value);
			literal_no2++;
		}	
	
		else if(isdigit(label[count][0])) //For literals and constants
		{
			if(L==1)
			{
				c='L';
				fprintf(fp2,"\t%c\t",c); //literal L
				fprintf(fp2,"%d",literal_no1); //literal no
				literal_no1++;
			}

			else
			{
				c='C';fprintf(fp2,"\t\t%c\t",c); //Constant C
				fprintf(fp2,"%d",value); //Value
			}

	  	}
	   
	    count++;
	}

   location_counter++;
   fputs("\n\n",fp2);

}


int main()
{
	FILE *fp1, *fp2;
	char temp[30];

	fp1=fopen("input2.txt","r");
	if(fp1==NULL)
	{	printf("INPUT FILE CAN'T BE OPENED");
		exit(1);
	}

// reading input.txt file

while(!feof(fp1))
{
	fgets(temp,30,fp1);
	printf("%s",temp);
}
//

fp2=fopen("intermediate_code.txt","w");
fputs("LC\tIC\tOP\tOPE1\tIC->OPE2\n\n",fp2);

fp3=fopen("literal.txt","w");
fputs("NO\tLiteral\t\tAddress\n\n",fp3);
fcloseall();// closing all above file
labels();

printf("\nInput code-> input.txt\n");
printf("intermediate code->intermediate_code.txt\n");
printf("\nSymbol table->symbol.txt\n");
printf("Literal table->literal.txt\n");

//getch();

return 0;
}
