#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>
#include <sstream>
#include "Tools.h"
#include <iomanip>  // additional formatting for cout
#include <string>


void printInstruction(std::string instruction);
int hexchar2int(char c);
void printOpcode(std::string instruction);
void printQuadWord(std::string instruction);

int main(int argc, char *argv[]) //file is passed in as a command line argument
{
	using namespace std;
    ifstream inf(argv[1]);
	stringstream stream;
 
    if (!inf)
    {
        cout << "Uh oh, the file could not be opened for reading!" << endl;
        exit(1);
    }
	int count = 0;
    while (inf)	//reads the specified file into stringstreeam stream
    {
		count++;
        std::string strInput;
		getline(inf, strInput);
		stream << strInput << endl;
    }
	count--;
	string temp = "0";
	for(int i = 0; i < count; i++)
	{
		getline(stream, temp);
		cout  << left << setfill(' ') << setw(10) << temp.substr(0,6);
		printInstruction(temp.substr(7));
		cout<<endl;
	}
    return 0;
}

void printInstruction(std::string instruction)//prints the instruction calls other functions to help
{
	using namespace std;
	if(instruction[0] != ' ' && instruction[15] != ' ' && instruction[16] == ' ')//for handling quad instructions
	{
		cout  << left << setw(10) << ".quad";
		string working = instruction.substr(14,2);
		for(int i = 12; i >= 0; i -= 2)
		{
			working += instruction.substr(i,2);
		}
		cout << "0x" << working;
	}
	else if(instruction[0] == ' ')//for empty instruction
	{
		cout  << left << setw(10) << " ";
	}
	else//for handling everything else
	{
		printOpcode(instruction); //this prints the opcodes
		//this next part handles the rest of the instruction
		string regNames[] = {"%rax", "%rcx", "%rdx", "%rbx", "%rsp", "%rbp", "%rsi", "%rdi", "%r8", "%r9", "%r10", "%r11", "%r12", "%r13", "%r14", "RNONE"};
		if(instruction[0] == '2') //rrmovq
		{
			cout << regNames[hexchar2int(instruction[2])] << ", " << regNames[hexchar2int(instruction[3])];//prints rA, rB
		}
		else if(instruction[0] == '3') //irmovq
		{
			cout<<"$";
			printQuadWord(instruction.substr(4,16)); //prints V
			cout << ", " << regNames[hexchar2int(instruction[3])]; //prints rB
		}
		else if(instruction[0] == '4') //rmmovq
		{
			cout << regNames[hexchar2int(instruction[2])] << ", "; //prints rA
			cout<<"$";
			printQuadWord(instruction.substr(4,16)); //prints D
			cout << "(" <<regNames[hexchar2int(instruction[3])] << ")";//prints (rB)
			
		}
		else if(instruction[0] == '5') //mrmovq
		{
			cout<<"$";
			printQuadWord(instruction.substr(4,16)); //prints D
			cout << "(" <<regNames[hexchar2int(instruction[3])] << ")";//prints (rB)
			cout  << ", " << regNames[hexchar2int(instruction[2])]; //prints rA
		}
		else if(instruction[0] == '6') //OPq
		{
			cout  << regNames[hexchar2int(instruction[2])] << ", "; //prints rA
			cout << regNames[hexchar2int(instruction[3])];//prints rB
		}
		else if(instruction[0] == '7') //jxx	 	!!!
		{
			printQuadWord(instruction.substr(2,14)); //prints Dest
		}
		else if(instruction[0] == '2' && instruction[1] != '0') //cmovXX
		{
			cout << regNames[hexchar2int(instruction[2])] << " " << regNames[hexchar2int(instruction[3])]; //prints rA, rB
		}
		else if(instruction[0] == '8') //call	!!!
		{
			printQuadWord(instruction.substr(2,14)); //prints Dest
		}
		else if(instruction[0] == 'a') //pushq
		{
			cout << regNames[hexchar2int(instruction[2])]; //prints rA
		}
		else if(instruction[0] == 'b') //popq
		{
			cout << regNames[hexchar2int(instruction[2])]; //prints rA
		}
	}
}
int hexchar2int(char c)//turns a hex char into a int
{
	std::string s = "0123456789abcdef"; 
	return s.find(c); 
}
void printOpcode(std::string instruction)//prints the opcode or illegal op if its a illegal operation
{
	using namespace std;
	if(instruction[0] == '6')	//handles OPq codes
	{
		string fnCode[] = {"addq", "subq", "andq", "xorq"};
		cout  << left << setw(10) << fnCode[hexchar2int(instruction[1])];
	}
	else if(instruction[0] == '7')	//handles jxx codes
	{
		string fnCode[] = {"jmp", "jle", "jl", "je", "jne", "jge", "jg"};
		cout << left << setw(10) << fnCode[hexchar2int(instruction[1])];
	}
	else if(instruction[0] == '2' && instruction[1] != '0')	//handles cmovXX codes
	{
		string fnCode[] = {"rrmovq", "cmovle", "cmovl", "cmove", "cmovne", "cmovge", "cmovg"};
		cout << left << setw(10) << fnCode[hexchar2int(instruction[1])];
	}
	else if(instruction[1] == '0')//handles regular opcodes
	{
		string opCodes[] = {"halt", "nop", "rrmovq", "irmovq", "rmmovq", "mrmovq", "", "", "call", "ret", "pushq", "popq"};
		if(hexchar2int(instruction[0]) < 12)
		{
			cout  << left << setw(10) << opCodes[hexchar2int(instruction[0])];
		}
		else
		{
			cout  << left << setw(10) << "illegal op";
		}
	}
	else
	{
		cout  << left << setw(10) << "illegal op";
	}
}
void printQuadWord(std::string instruction)//formats string value correctly then turns it into a int then prints
{
	using namespace std;
	//formats it by reversing the string of hexes by byte chunks
	string working = instruction.substr(14,2);
	for(int i = 12; i >= 0; i -= 2)
	{
		working += instruction.substr(i,2);
	}
	//turns the correctly formated string into int value
	stringstream ss(working);
	string x[8]; 
	unsigned long val = 0; 
	for(int i = 0; i < 8; i++) 
	{ 
		ss >> setw(2) >> x[i]; 
		if(ss.fail()) 
			break; 
		const char *buf = x[i].c_str(); 
		val <<= 8; 
		val |= strtoul(buf,NULL,16); 
	}
	cout << "0x" << hex << setfill('0') << val;
}
