#include <iostream>
#include <fstream>
#include <memory>
#include <string>

#include "bf.hpp"

#define BSIZE (1)

int main(int argc , const char **argv)
{

if(argc>1)
{

Cell cell;
Brainfuck bf;

std::ofstream fout;
std::ifstream fin(argv[1],std::ios::in | std::ios::binary);

if (argc>2) fout.open(argv[2],std::ios::out | std::ios::binary);


if(fin && ((argc > 2)? !fout.fail():true))
{
	Cell cell;
	Brainfuck bf(argc>2 ? fout.rdbuf() : std::cout.rdbuf() );
	
	std::string str("");
	std::unique_ptr<char[]> buff(new char[BSIZE+1]);
	std::size_t count=BSIZE;
	
	do
	{
		fin.read(buff.get(),BSIZE);
		str.append(buff.get(),count=fin.gcount());

	}while(count>=BSIZE);

	bf(cell,str);
	
}
else
{
	if(fin) std::cerr << "can not open file: " << argv[1] << "\n";
}

}


return 0;
}