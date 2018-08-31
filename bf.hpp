#ifndef __BF_HPP__

#define __BF_HPP__

#include <iostream>
#include <memory>
#include <string>


class Cell: protected std::unique_ptr<char[]>
{
	protected:
	
	char *ptr;
	std::size_t length;
	
	public:
	
	Cell(std::size_t s=512)
	{
		setcell(s);
	}
	
	void setcell(std::size_t s)
	{
		reset(new char[s]);
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
	
	const char * const getptr() const {return ptr;}
	
	std::size_t getlength() const {return length;}
	
	char * const operator ++ () {return ++ptr;}
	
	char * const operator -- () {return --ptr;}
	
	char & operator * () const {return *ptr;}
	
	char * const operator + (std::size_t n) const {return (ptr+n);}
	
	char * const operator - (std::size_t n) const {return (ptr-n);}
	
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
	
	
	void kernel (Cell &cell,const std::string &str)
	{
		auto i=str.begin();
		
		
		while(i != str.end())
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
					if (*cell == 0) i =fn(1,i+1,str.end());
					break;
					
				case ']': i =fp(-1,i-1,str.begin());
					continue;
			}
			
			if(i!=str.end()) ++i;
		}
	}

	
	void operator() (Cell &cell,const std::string &str)
	{
		kernel(cell,str);
	}
};


#endif