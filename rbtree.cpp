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
	unsigned long instruction;
	pageinst* next;

	public:
	pageinst(unsigned long inst);
	unsigned long* getinst(void);
	pageinst* getnext(void);
	void setnext(pageinst* newinst);
};

pageinst::pageinst(unsigned long inst)
{
	instruction = inst;
	next = NULL;
}

unsigned long* pageinst::getinst(void)
{
	return &instruction;
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
	private:
	unsigned long page;
	pageinst* head;
	pageinst* tail;

	public:
	pagechain(unsigned long where);
	unsigned long* getpage(void);
	bool operator==(pagechain&) const;
	bool operator<(pagechain&) const;
	pageinst* gethead(void);
	void sethead(pageinst* newhead);
	pageinst* gettail(void);
	void settail(pageinst* newtail);
};

unsigned long* pagechain::getpage(void)
{
	return &page;
}

bool pagechain::operator==(pagechain& pc) const
{
	return (page == pc.page);
}

bool pagechain::operator<(pagechain& pc) const
{
	return (page < pc.page);
}

pagechain::pagechain(unsigned long where)
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
	fout.write((char*)pc->getpage(), sizeof(long));
	pageinst* pi = pc->gethead();
	while (pi) {
		fout.write((char*)pi->getinst(), sizeof(long));
		pi = pi->getnext();
	}
	unsigned long x = 0;
	fout.write((char*)&x, sizeof(long));
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
void insertinstruction(long pn, long ins, void* tree, void* root)
{
	unsigned long pagenumber = static_cast<unsigned long>(pn);
	unsigned long instruction = static_cast<unsigned long>(ins);
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
	ofstream fileout(filenameout, ios::out | ios::trunc | ios::binary);
	writeoutpages(nodetree->root, fileout);
}

} //end extern "C"




