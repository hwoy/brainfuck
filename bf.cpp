#include <iostream>
#include <fstream>
#include <memory>

#include "bf.hpp"
#include "bfhelp.hpp"


static constexpr bool elem(inst_t ch, const inst_t * const inst)
{
	return (*inst)? ( (ch==*inst)? true : elem(ch,inst+1) ) : false;
}


static unsigned int bracket(std::istream &fin,ip_t &ip)
{
	unsigned int n=1;
	inst_t data;
	auto looplimit = ip.capacity();
	auto limit = looplimit-ip.size();
			
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
	usage(argv[0],"a Brainf**k Interpreter.",BFTAPE,sizeof(cell_t));
	return 0;
}

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

std::ofstream fout;

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
	Tape tape(BFTAPE);
	Brainfuck bf(argc>2 ? fout.rdbuf() : std::cout.rdbuf() );
	ip_t ip(LOOPLIMIT*1024);
	inst_t data;
	
	while(fin.read(&data,1), fin.gcount()>=1)
	{
		if(!elem(data,Brainfuck::inst))	continue;
			
		ip.clear();
		ip.push_back(data);
		
		if(data=='[' && bracket(fin,ip))	throw Bfexception(Bfexception::eid_while);
		
		else if(data==']')				throw Bfexception(Bfexception::eid_endwhile);
		
		bf.eval(tape,ip.begin(),ip.end());
	}

}catch(const std::exception &e)
{
	std::cerr 	<< std::endl	<< "Exception:" << std::endl
				<< e.what()	<< std::endl;
}
	

return 0;
}