static const char *err[] = {"Can not access INPUT FILE: ","Can not access OUTPUT FILE: ",nullptr};

enum{
	err_in,err_out
};

static void showerr (std::size_t id,const char *err[],const char *str)
{
	std::cerr << "\nERROR ID " << id  << " : " << err[id] << (str?str:"") << std::endl;
}

#define BSIZE (4*1024)