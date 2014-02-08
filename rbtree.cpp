#include <iostream>
#include "redblack.hpp"
#include "pagechain.hpp"

using namespace std;

//Copyright Adrian McMenamin 2014
//acm538@york.ac.uk
//C wrap around C++ redblack tree code
//Licensed under GNU GPL verison 2
//or any later version at your discretion

class pageinst
{
	private:
	long instruction;
	pageinst* next;

	public:
	pageinst(long inst);
	long getinst(void);
	pageinst* getnext(void);
	void setnext(pageinst* newinst);
};

pageinst::pageinst(long inst)
{
	instruction = inst;
	next = NULL;
}

long pageinst::getinst(void)
{
	return instruction;
}

pageinst* pageinst::getnext(void)
{
	return next
}

void pageinst::setnext(pageinst* newinst)
{
	next = newinst;
}

class pagechain
{
	private:
	long page;
	pageinst* head;
	pageinst* tail;

	public:
	pagechain(long where);
	pageinst* gethead(void);
	void sethead(pageinst* newhead);
	pageinst* gettail(void);
	void settail(pageinst* newtail);
};

pagechain::pagechain(long where)
{
	page = where;
	head = NULL;
	tail = NULL;
}

pageinst* pagechain::gethead(void)
{
	return head;
}

void pagechain::sethead(pageinst* newhead)
{
	pageinst* oldhead = gethead();
	head = newhead;
	if (oldhead) {
		newhead->setnext(oldhead);
	}
	if (!gettail()) {
		tail = newhead;
	}
}

pageinst* pagechain::gettail(void)
{
	return tail;
}

void pagechain::settail(pageinst* newtail)
{
	pageinst* oldtail = gettail();
	tail = newtail;
	if (oldtail) {
		oldtail->setnext(newtail);
	}
	if (!gethead()) {
		head = newtail;
	}
}


extern "C" {

void* createtree(void)
{
	redblacktree<redblacknode<struct pagechain> >* tree;
	tree = new redblacktree<redblacknode<struct pagechain> >();
	return tree;
}

void deletetree(void* tree)
{
	delete tree;
}

struct pagechain* getroot(void* tree)
{
	return tree->root;
}

struct pagechain* getnode(long pagenumber, struct pagechain* root, void* tree)
{
	struct pagechain testchain;
	testchain.page = pagenumber
	return tree->locatenode(&testchain, root);
}

bool addpage(long pagenumber, struct pagechain* root, void* tree)
{
	





