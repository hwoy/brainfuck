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


static void usage(const char *path,const char *str)
{
	auto gpath=path::grap(path);
	
	std::cout << std::endl << gpath << " is " << str << std::endl << std::endl
						  << gpath << " input-file" << std::endl
						  << gpath << " input-file output-file" << std::endl << std::endl;
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
