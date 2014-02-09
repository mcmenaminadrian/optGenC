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
	redblacktree<redblacknode<pagechain> >* tree;
	tree = new redblacktree<redblacknode<pagechain> >();
	return static_cast<void*> tree;
}

void deletetree(void* tree)
{
	delete static_cast<redbacktree<redblacknode<pagechain> >* > tree;
}

void* getroot(void* tree)
{
	reblacktree<redblacknode<pagechain> >* nodetree =
		static_cast<redblacktree<redblacknode<pagechain> >*> tree;
	return static_cast<void*>(nodetreetree->root);
}

//if node for this page exists add to its tail
//if node for this page does not exist, create and add to tail
void insertinstruction(long pagenumber, long instruction, 
	void* tree, void* root)
{
	redblacknode<pagechain>* rootnode;
	redblacktree<redblacknode<pagechain> >* nodetree;
	redblacknode<pagechain>* pagenode;
	redblacknode<pagechain>* dummynode;
		
	rootnode = static_cast<redblacknode<pagechain>* > root;
	nodetree = static_cast<redblacktree<redblacknode<pagechain> >* > tree;

	dummynode = new redblacknode<pagechain>(instruction);
	pagenode = tree->locatenode(dummynode, root);
	pageinst* nextinstruction = new pageinst(instruction);

	if (pagenode) {
		pagenode->settail(nextinstruction);
		delete dummynode;
	} else {
		dummynode->settail(nextinstruction);
		tree->insertnode(dummynode, root);
	}
}

} //end extern "C"




