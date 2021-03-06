

struct path{
	static constexpr const char * const grap(const char * const path)
	{
		return grap(path,path);
	}
	
	private:
	static constexpr const char * const grap(const char * const gpath,const char * const path)
	{
		return *path?  grap( (*path=='\\' || *path =='/') ? path+1 : gpath,path+1) : gpath;
	}
};

static void showerr (unsigned int id,const char *err[],const char *str)
{
	std::cerr << "\nERROR ID " << id  << " : " << err[id] << (str?str:"") << std::endl;
}


static void usage(const char *path,const char *str,std::size_t tapesize,std::size_t cellsize)
{
	auto gpath=path::grap(path);
	
	std::cerr << std::endl << gpath << " is " << str << std::endl << std::endl
						  << gpath << " input-file" << std::endl
						  << gpath << " input-file output-file" << std::endl << std::endl;
	
	std::cerr << "TAPESIZE = " << tapesize << ", CELLSIZE = " << cellsize*8 << " bits"<< std::endl << std::endl;
	
	#ifdef BFPROG
	
	std::cerr << "*** "
	
	#if  defined(EOF_UNCHANGED)
			"If takes an EOF, unchanges a current value of cell."
					
	#elif defined(EOF_MINUS1) 
			"If takes an EOF, changes a current value of cell to -1"
					
	#elif defined(EOF_0) 
			"If takes an EOF, changes a current value of cell to 0"
					
	#else
			"If takes an EOF, changes a current value of cell to this EOF value"

				
	#endif
	
	<< " ***" << std::endl << std::endl
	
	<< "***** Suggest EOF = " << std::char_traits<char>::eof() << " *****" << std::endl << std::endl;
	
	#endif
}

static const char *err[] = {"Can not access INPUT FILE: ","Can not access OUTPUT FILE: ",nullptr};

enum{
	err_fin,err_fout
};

#define BSIZE (4*1024)

#ifndef COL
#define COL (64*2)
#endif

#ifndef LOOPLIMIT
#define LOOPLIMIT 64
#endif

#ifndef BFTAPE
#define BFTAPE 512
#endif

#ifndef TBFTAPE
#define TBFTAPE 2
#endif