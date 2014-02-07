//Copyright Adrian McMenamin 2014
//Licensed under GPL v2 or later

#ifndef _PAGECHAIN_
#define _PAGECHAIN_

class pageinst
{
	private:
	long instruction;
	pageinst* next;
};

class pagechain
{
	private:
	long page;
	pageinst* head;
	pageinst* tail;

	public:
	pagechain(long where)
	{
		page = where;
	}

};


#endif
