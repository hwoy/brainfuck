#include <iostream>
#include <fstream>
#include <memory>

#include "bf.hpp"
#include "bfhelp.hpp"


static bool elem(byte_t ch, const byte_t *inst)
{
	while(*inst)
		if(ch==*inst++) return true;

	return false;
}

static unsigned int bracket(std::istream &fin,ip_t &ip, std::size_t looplimit=(LOOPLIMIT*1024))
{
	unsigned int n=1;
	byte_t data;
	auto limit = looplimit;
			
	while(fin.read(&data,1), fin.gcount()>=1)
		{
			if(!elem(data,Brainfuck::inst)) continue;
			
			if(data=='[') ++n;
			else if(data==']') --n;
				
			ip.push_back(data);
			
			if(!n) break;
			if(!(--limit)) throw Bfexception("Large [] loop Over the limit:" + std::to_string(looplimit) + " ,Please check your code.");
		}
		
	return n;
}


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


try{
	Cell cell(BFCELL);
	Brainfuck bf(argc>2 ? fout.rdbuf() : std::cout.rdbuf() );
	ip_t ip;
	byte_t data;
	
	while(fin.read(&data,1), fin.gcount()>=1)
	{
		if(!elem(data,Brainfuck::inst))	continue;
			
		ip.clear();
		ip.push_back(data);
		
		if(data=='[' && bracket(fin,ip))	throw Bfexception(Bfexception::eid_while);
		
		else if(data==']')				throw Bfexception(Bfexception::eid_endwhile);
		
		bf.kernel(cell,ip.begin(),ip.end());
	}

}catch(const std::exception &e)
{
	std::cerr 	<< std::endl	<< "Exception:" << std::endl
				<< e.what()	<< std::endl;
}
	

return 0;
}