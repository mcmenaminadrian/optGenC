#include <iostream>
#include <fstream>
#include <cstdio>
#include "redblack.hpp"

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
	return next;
}

void pageinst::setnext(pageinst* newinst)
{
	next = newinst;
}

class pagechain
{
	friend ostream& operator<<(ostream& os, pagechain& pc);

	private:
	long page;
	pageinst* head;
	pageinst* tail;

	public:
	pagechain(long where);
	long getpage(void);
	bool operator==(pagechain&) const;
	bool operator<(pagechain&) const;
	pageinst* gethead(void);
	void sethead(pageinst* newhead);
	pageinst* gettail(void);
	void settail(pageinst* newtail);
};

ostream& operator<<(ostream& os, pagechain& pc)
{
	os << pc.page;
	return os;
}

long pagechain::getpage(void)
{
	return page;
}

bool pagechain::operator==(pagechain& pc) const
{
	return (page == pc.page);
}

bool pagechain::operator<(pagechain& pc) const
{
	return (page < pc.page);
}

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
	if (oldhead) 
		newhead->setnext(oldhead);
	if (!gettail())
		tail = newhead;
}

pageinst* pagechain::gettail(void)
{
	return tail;
}

void pagechain::settail(pageinst* newtail)
{
	pageinst* oldtail = gettail();
	tail = newtail;
	if (oldtail)
		oldtail->setnext(newtail);
	if (!gethead())
		head = newtail;
}

void killchain(pageinst* pi)
{
	if (pi == NULL)
		return;
	pageinst* next = pi->getnext();
	delete pi;
	killchain(next);
}

void killtree(redblacknode<pagechain>* node)
{
	if (node == NULL)
		return;
	killtree(node->left);
	killtree(node->right);
	killchain(node->getvalue().gethead());
}

void writechain(pagechain* pc, ofstream& fout)
{
	if (pc == NULL)
		return;
	fout << pc->getpage();
	pageinst* pi = pc->gethead();
	while (pi) {
		fout << pi->getinst();
		pi = pi->getnext();
	}
	fout << 0L;
}

void writeoutpages(redblacknode<pagechain>* node, ofstream& fout)
{
	if (node == NULL)
		return;
	writeoutpages(node->left, fout);
	writechain(&node->getvalue(), fout);
	writeoutpages(node->right, fout);
}

extern "C" {

void* createtree(void)
{
	redblacktree<redblacknode<pagechain> >* tree;
	tree = new redblacktree<redblacknode<pagechain> >();
	return static_cast<void*>(tree);
}

void removetree(void* tree)
{
	redblacktree<redblacknode<pagechain> >* rbtree;
	rbtree = (static_cast<redblacktree<redblacknode<pagechain> >* >(tree));
	killtree(rbtree->root);
	delete rbtree;
}

void* getroot(void* tree)
{
	redblacktree<redblacknode<pagechain> >* nodetree =
		static_cast<redblacktree<redblacknode<pagechain> >*>(tree);
	return static_cast<void*>(nodetree->root);
}

//if node for this page exists add to its tail
//if node for this page does not exist, create and add to tail
void insertinstruction(long pagenumber, long instruction, 
	void* tree, void* root)
{
	redblacknode<pagechain> *rootnode, *pagenode, *dummynode;
	redblacktree<redblacknode<pagechain> >* nodetree;
		
	pagechain dummychain = pagechain(pagenumber);
	rootnode = static_cast<redblacknode<pagechain>*>(root);
	nodetree = static_cast<redblacktree<redblacknode<pagechain> >* >(tree);

	dummynode = new redblacknode<pagechain>(dummychain);
	pagenode = nodetree->locatenode(dummynode, rootnode);
	pageinst* nextinstruction = new pageinst(instruction);

	if (pagenode) {
		pagenode->getvalue().settail(nextinstruction);
		delete dummynode;
	} else {
		dummynode->getvalue().settail(nextinstruction);
		nodetree->insertnode(dummynode, rootnode);
	}
}

void writeinorder(void* tree, char* filenameout)
{
	redblacktree<redblacknode<pagechain> >* nodetree =
	static_cast<redblacktree<redblacknode<pagechain> >*>
		(tree);
	ofstream fileout(filenameout);
	writeoutpages(nodetree->root, fileout);
}

} //end extern "C"




