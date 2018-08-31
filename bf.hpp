#ifndef __BF_HPP__

#define __BF_HPP__

#include <iostream>
#include <memory>
#include <string>

using cdata_t = char;
using ip_t = std::basic_string<cdata_t>;

class Cell: protected std::unique_ptr<cdata_t[]>
{
	protected:
	
	cdata_t *ptr;
	std::size_t length;
	
	public:
	
	Cell(std::size_t s=512)
	{
		setcell(s);
	}
	
	void setcell(std::size_t s)
	{
		reset(new cdata_t[s]);
		ptr=get();
		length=s;
		
		for(std::size_t i=0;i<length;++i)
			get()[i] = 0;	
	}
	
	void destroycell(void)
	{
		release();
		ptr=nullptr;
		length=0;
	}
	
	const cdata_t * const getptr() const {return ptr;}
	
	std::size_t getlength() const {return length;}
	
	cdata_t * const operator ++ () {return ++ptr;}
	
	cdata_t * const operator -- () {return --ptr;}
	
	cdata_t & operator * () const {return *ptr;}
	
	cdata_t * const operator + (std::size_t n) const {return (ptr+n);}
	
	cdata_t * const operator - (std::size_t n) const {return (ptr-n);}
	
};

class Brainfuck
{
	std::ostream out;

	template <class T>
	static T fn(int n, T i, T j)
	{
		while(n && i!=j)
		{
			if(*i == '[') ++n;
			else if (*i == ']') --n;
			
			if(n) ++i;
		}
		
		return i;
	}

	template <class T>
	static T fp(int n, T i, T j)
	{
		while(n && i!=j)
		{
			if(*i == '[') ++n;
			else if (*i == ']') --n;
			
			if(n) --i;
		}
		
		return i;
	}
	
	public:
	
	Brainfuck(std::streambuf *rd=std::cout.rdbuf()):out(rd){}

	template <class T>
	void kernel (Cell &cell,T begin, T end)
	{
		auto i=begin;
		
		
		while(i != end)
		{
			switch (*i)
			{
				case '>': ++cell;  break;
				case '<': --cell;  break;
				
				case '+': ++(*cell); break;
				case '-': --(*cell); break;

				case '.': out.write(cell.getptr(),1); out.flush(); break;
				case ',': *cell = std::cin.get(); break;
				
				case '[': 
					if (*cell == 0) i =fn(1,++i,end);
					break;
					
				case ']': i =fp(-1,--i,begin);
					continue;
			}
			
			if(i!=end) ++i;
		}
	}	
	
	
	
	void kernel (Cell &cell,const ip_t &ip)
	{
		kernel (cell,ip.begin(),ip.end());
	}

	
	void operator() (Cell &cell,const ip_t &ip)
	{
		kernel(cell,ip);
	}
};


#endif