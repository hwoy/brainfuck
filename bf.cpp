#include <iostream>
#include <fstream>
#include <memory>
#include <string>

#include "bf.hpp"
#include "bfhelp.hpp"


int main(int argc , const char *argv[])
{

if(argc<=1) return 0;

std::ofstream fout;
std::ifstream fin;


if(argc > 1)
{
	fin.open(argv[1],std::ios::in | std::ios::binary);
	if(!fin) 
	{
		showerr(err_in,err,argv[1]);
		return 1;
	}
}

if(argc > 2)
{
	fout.open(argv[2],std::ios::out | std::ios::binary);
	if(!fout) 
	{
		showerr(err_out,err,argv[2]);
		return 1;
	}
}

Cell cell;
Brainfuck bf(argc>2 ? fout.rdbuf() : std::cout.rdbuf() );

	
std::string str;
std::unique_ptr<char[]> buff(new char[BSIZE+1]);
std::size_t count=BSIZE;
	
	do
	{
		fin.read(buff.get(),BSIZE);
		str.append(buff.get(),count=fin.gcount());

	}while(count>=BSIZE);

bf(cell,str);
	




return 0;
}