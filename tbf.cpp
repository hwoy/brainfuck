#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <algorithm>

#include "bf.hpp"
#include "bfhelp.hpp"

using num_t = cdata_t;

using factor_t = std::pair<num_t,num_t>;


static std::vector<factor_t> factor(num_t n)
{
	std::vector<factor_t> vec;
	
	for(num_t i=1;i<= (n/2)+1 ;++i)
	{
		if(!(n%i)) vec.push_back({i,n/i});
	}
	return vec;
}

static factor_t minfactor(const std::vector<factor_t> &vec)
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
	while(isprime(n)) n--;
	
	return n;
}

static ip_t a2bfA(num_t n, Cell &cell)
{
	num_t a,b;
	num_t m= n>*(cell+1) ? n-*(cell+1) : *(cell+1)-n;
	ip_t ip;
	std::tie(a,b) = minfactor(factor(findfactor(m)));
	
	(a>*cell)? ip.insert(ip.end(),a-(*cell),'+') : ip.insert(ip.end(),(*cell)-a,'-') ;
	
	{
		const char str[]=" [> ";
		ip.insert(ip.end(),std::begin(str),std::end(str));
	}
	
	*cell=0;
	++cell;
	
	(n>*cell)? ip.insert(ip.end(),b,'+') : ip.insert(ip.end(),b,'-') ;
	
	{
		const char str[]=" <-] > ";
		ip.insert(ip.end(),std::begin(str),std::end(str));
	}
	
	
	if( (n>*cell) && ((*cell+a*b)<n) ) ip.insert(ip.end(),n-(*cell+a*b),'+')  ;
	else if((*cell>=n) && ((n+a*b)<*cell) ) ip.insert(ip.end(),*cell-(n+a*b),'-')  ;
	
	{
		const char str[]=" . < ";
		ip.insert(ip.end(),std::begin(str),std::end(str));
	}

	*cell=n;
	--cell;
	
	return ip;
}

static ip_t a2bfB(num_t n, Cell &cell)
{
	ip_t ip;
	
	{
		const char str[]=" > ";
		ip.insert(ip.end(),std::begin(str),std::end(str));
	}
	
	++cell;
	(n>*cell) ? ip.insert(ip.end(),n-*cell,'+') : ip.insert(ip.end(),*cell-n,'-');
	

	{
		const char str[]=" . < ";
		ip.insert(ip.end(),std::begin(str),std::end(str));
	}	
	
	*cell=n;
	--cell;
	
	return ip;
}

static ip_t a2bf(num_t n, Cell &cell)
{
	if( ((n>*(cell+1))&&((n-*(cell+1))<=11)) || ((n<=*(cell+1))&&((*(cell+1)-n)<=11))) return a2bfB(n,cell);
	
	return a2bfA(n,cell);
}

template <class T>
static ip_t it2bf(T i,T j, Cell &cell)
{
	ip_t ip;
	
	for(;i != j; ++i)
	{
		auto ip2 = a2bf(*i,cell);
		ip.insert(ip.end(),ip2.begin(),ip2.end());
	}
	
	return ip;
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
	fout.open(argv[2],std::ios::out | std::ios::binary);
	if(!fout) 
	{
		showerr(err_fout,err,argv[2]);
		return 1;
	}
}


std::ostream out(argc>2 ? fout.rdbuf() : std::cout.rdbuf() );

Cell cell;
ip_t ip;
std::unique_ptr<cdata_t[]> buff(new cdata_t[BSIZE+1]);

	do
	{
		fin.read(buff.get(),BSIZE);
		out << it2bf(buff.get(),buff.get()+fin.gcount(),cell);
	}while(fin.gcount()>=BSIZE);



return 0;
}