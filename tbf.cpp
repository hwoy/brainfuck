#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <algorithm>
#include <string>

#include "bf.hpp"
#include "bfhelp.hpp"


using num_t = int;

using factor_t = std::pair<num_t,num_t>;

using listfactor_t = std::vector<factor_t>;



static listfactor_t factor(num_t n)
{
	listfactor_t vec;
	
	for(num_t i=1;i<= (n/2)+1 ;++i)
	{
		if(!(n%i)) vec.push_back({i,n/i});
	}
	return vec;
}

static factor_t minfactor(const listfactor_t &vec)
{
		
	return *std::min_element(vec.begin(),vec.end(),
	[](const factor_t &a,const factor_t &b)
	{
		return (a.first + a.second) < (b.first + b.second);
	});
}

static bool isprime(num_t n)
{
		
	for(num_t i=2;i*i<=n;++i)
	{
		if(!(n%i)) return false;
	}
	
	return true;
}

static num_t findfactor(num_t n)
{
		
	while(isprime(n) && (n>3)) --n;
	
	return n;
}

static void appendip(ip_t &ip,const std::string &str)
{
	ip.insert(ip.end(),str.begin(),str.end());
}

static ip_t a2bfA(num_t n, Cell &cell)
{
	ip_t ip;
	
	num_t a,b;
	num_t m= n>*(cell) ? n-*(cell) : *(cell)-n;
	std::tie(a,b) = minfactor(factor(findfactor(m)));
	
	
	appendip(ip,">");++cell;
	
	(a>*cell)? ip.insert(ip.end(),a-(*cell),'+') : ip.insert(ip.end(),(*cell)-a,'-') ;
	
	*cell=0;
	
	appendip(ip,"[<");--cell;
	
	(n>*cell)? ip.insert(ip.end(),b,'+') : ip.insert(ip.end(),b,'-') ;
	
	appendip(ip,">-]<");
	
	
	
	if( (n>*cell) && ((*cell+a*b)<n) ) ip.insert(ip.end(),n-(*cell+a*b),'+')  ;
	else if((*cell>=n) && ((n+a*b)<*cell) ) ip.insert(ip.end(),*cell-(n+a*b),'-')  ;
	
	*cell=n;
	appendip(ip,".");
	
	return ip;
}

static ip_t a2bfB(num_t n, Cell &cell)
{
	ip_t ip;
	
	(n>*cell) ? ip.insert(ip.end(),n-*cell,'+') : ip.insert(ip.end(),*cell-n,'-');
	
	*cell=n;

	appendip(ip,".");	
	
	
	return ip;
}

static ip_t a2bf(num_t n, Cell &cell)
{
	if( ((n>*cell)&&((n-*cell)<=11)) || ((n<=*cell)&&((*cell-n)<=11)) ) return a2bfB(n,cell);
	
	return a2bfA(n,cell);
}

template <class T>
static const ip_t it2bf(T i,T j, Cell &cell)
{
	ip_t ip;
	
	for(;i != j; ++i)
	{
		auto ip2 = a2bf(*i,cell);
		ip.insert(ip.end(),ip2.begin(),ip2.end());
	}
	
	return ip;
}

static unsigned int printip(std::ostream &out,const ip_t &ip,unsigned int ccol,unsigned int col)
{
	for(const auto &i:ip)
	{
		out << i;
		
		ccol=(ccol+1)%col;
		
		if(!ccol) out << std::endl;
	}
	
	return ccol;
}

int main(int argc , const char *argv[])
{

if(argc<=1)
{
	usage(argv[0],"a Text to Brainf**k Encoder.");
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
	fout.open(argv[2]);
	if(!fout) 
	{
		showerr(err_fout,err,argv[2]);
		return 1;
	}
}


std::ostream out(argc>2 ? fout.rdbuf() : std::cout.rdbuf() );

Cell cell(TBFCELL);
std::unique_ptr<byte_t[]> buff(new byte_t[BSIZE+1]);

unsigned int ccol = 0;

try{
	
	do
	{
		fin.read(buff.get(),BSIZE);
		ccol=printip(out,it2bf(buff.get(),buff.get()+fin.gcount(),cell),ccol,COL);
	}while(fin.gcount()>=BSIZE);
	
}catch(const std::exception &e)
{
	std::cerr	<< std::endl	<< "Exception:"	<< std::endl
				<< e.what()	<< std::endl;
}



return 0;
}