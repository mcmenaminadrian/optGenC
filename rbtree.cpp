#include <iostream>
#include "redblack.hpp"
#include "pagechain.hpp"

using namespace std;

//Copyright Adrian McMenamin 2014
//acm538@york.ac.uk
//C wrap around C++ redblack tree code
//Licensed under GNU GPL verison 2
//or any later version at your discretion

class 

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
	





