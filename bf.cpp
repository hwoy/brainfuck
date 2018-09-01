#include <iostream>
#include <fstream>
#include <memory>
#include <string>

#include "bf.hpp"
#include "bfhelp.hpp"


int main(int argc , const char *argv[])
{

if(argc<=1)
{
	usage(argv[0],"a Brainf**k Interpreter.");
	return 0;
}

std::ofstream fout;
std::ifstream fin;


if(argc > 1)
{
	fin.open(argv[1],std::ios::in | std::ios::binary);
	if(!fin) 
	{
		showerr(err_fin,err,argv[1]);
		return 1;
	}
}

if(argc > 2)
{
	fout.open(argv[2],std::ios::out | std::ios::binary);
	if(!fout) 
	{
		showerr(err_fout,err,argv[2]);
		return 1;
	}
}

Cell cell;
Brainfuck bf(argc>2 ? fout.rdbuf() : std::cout.rdbuf() );

try{	
	ip_t ip;
	std::unique_ptr<cdata_t[]> buff(new cdata_t[BSIZE+1]);
	
	do
	{
		fin.read(buff.get(),BSIZE);
		ip.append(buff.get(),fin.gcount());

	}while(fin.gcount()>=BSIZE);

	bf(cell,ip);
}catch(const std::exception &e)
{
	std::cerr << std::endl << "Exception:"  << std::endl
			  << e.what() << std::endl;
}
	

return 0;
}