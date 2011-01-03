#include "visitors.h"

//*******      Node		*************
Node::Node(int line, int column)
{
	this->line = line;
	this->column = column;
	this->father = NULL;
	this->Location = -1;
	this->Global_Location = -1;
	this->PushType = -1; // -1: no push, 1: pushl, 2: pushg, 3:load.
}

//void
//Node::AddLocation(Ident *ident, int PushType)
//{
//	this->Location = ident->symbol->location;
//	this->Global_Location = ident->symbol->global_location;
//	this->PushType = PushType;
//}

void
Node::AddLocation(Node *node)
{
	this->Location = node->Location;
	this->Global_Location = node->Global_Location;
	this->PushType = node->PushType;
}

void
Node::AddLocation(int Location, int Global_Location, int PushType)
{
	this->Location = Location;
	this->Global_Location = Global_Location;
	this->PushType = PushType;
}

//*******     Root		*************
Root::Root(int l, int c) : Node(l,c)
{
	this->classes = new vector<Class *>;
}

void
Root::AddClass(Class *cd)
{
	this->classes->push_back(cd);
	cd->father = this;
}

//*******       Class		*********
Class::Class(Ident *n, Members *ms, int l, int c) : Node(l,c)
{
	this->name = n;
	this->basic = NULL;
	this->members = ms;

	this->Parents = new vector<Ident *>;
	this->Childrens = new vector<Ident *>;

	this->Globals = new vector<Ident *>;
	this->Statics = new vector<Ident *>;
	this->Functions = new vector<Ident *>;

	n->father = this;
	ms->father = this;
}

bool
Class::AddParent(Ident *parent_ID)
{
	/* Add Parent Ident to this class Parents.
	** if ident was been added to class then retrun false and don't add it again.
	** if not, add it and return true.
	*/
	for(int i=0; i<this->Parents->size(); i++)
		if(this->Parents->at(i)->name == parent_ID->name) // if this ident was been added to class.
			return false;
	this->Parents->push_back(parent_ID);
	return true;
}

bool
Class::AddChild(Ident *child_ID)
{
	/* Add Child Ident to this class Childrens.
	** if ident was been added to class then retrun false and don't add it again.
	** if not, add it and return true.
	*/
	for(int i=0; i<this->Childrens->size(); i++)
		if(this->Childrens->at(i)->name == child_ID->name) // if this ident was been added to class.
			return false;
	this->Childrens->push_back(child_ID);
	return true;
}

bool
Class::AddGlobal(Ident *Global)
{
	/* Add Global Ident to this class Globals.
	** if ident was been added to class then retrun false and don't add it again.
	** if not, add it and return true.
	*/
	for(int i=0; i<this->Globals->size(); i++)
		if(this->Globals->at(i)->name == Global->name)
			return false;
	this->Globals->push_back(Global); // Add Ident of Global to vector.
	return true;
}

void
Class::AddStatic(Ident *Static)
{
	this->Statics->push_back(Static); // Add Ident of Static Global to vector.
}

void
Class::AddFunc(Ident *Function)
{
	this->Functions->push_back(Function);
}

void
Class::Reverse(vector<Ident *> *Vector)
{
	vector<Ident *> *Reverse = new vector<Ident *>;
	for(int i = Vector->size()-1; i>=0; i--)
	{
		Reverse->push_back(Vector->at(i));
		Vector->pop_back();
	}
	for(int i=0; i<Reverse->size(); i++)
		Vector->push_back(Reverse->at(i));
	Reverse->clear();
}

void
Class::GiveNum(vector<Ident *> *Vector)
{
	// Add location of Ident in vector to Ident symbol global_location.
	for(int i=0; i<Vector->size(); i++)
	{
		Vector->at(i)->symbol->global_location = i;
		Vector->at(i)->symbol->location = -1;
	}
}

ClassInher::ClassInher(Ident *n, Ident *p, Members *ms, int l, int c) : Class(n,ms,l,c)
{
	// CLASS IDENT ':' IDENT '{' Members '}'
	this->basic = p;
	p->father = this;
}

//*******     Members		*********
Members::Members(int l, int c) : Node(l,c)
{
	this->members = new vector<Member *>;
}

void
Members::AddMember(Member *m)
{
	this->members->push_back(m);
	m->father = this;
}

Member::Member(int l, int c) : Node(l,c)
{
	// empty
}

//*******     Global		*********
Global::Global(AccessModif *am, Type *ty, Variables *vs, int l, int c) : Member(l,c)
{
	this->accessModif = am;
	this->type = ty;
	this->variables = vs;

	am->father = this;
	ty->father = this;
	vs->father = this;
}

//*******   Constructor		*********
Constructor::Constructor(AccessModif *am, Ident *n, Args *as, Stats *ss, int l, int c) : Member(l,c)
{
	this->accessModif = am;
	this->name = n;
	this->args = as;
	this->stats = ss;

	am->father = this;
	n->father = this;
	as->father = this;
	ss->father = this;
}

//*******     Function		*********
Function::Function(AccessModif *am, Type *ty, Ident *n, Args *as, Stats *ss, int l, int c) : Member(l,c)
{
	this->accessModif = am;
	this->type = ty;
	this->name = n;
	this->args = as;
	this->stats = ss;

	this->IsCalled = false;

	am->father = this;
	ty->father = this;
	n->father = this;
	as->father = this;
	ss->father = this;
}

Function::Function(AccessModif *am, Ident *n, Args *as, Stats *ss, int l, int c) : Member(l,c)
{
	this->accessModif = am;
	this->type = NULL;
	this->name = n;
	this->args = as;
	this->stats = ss;

	this->IsCalled = false;

	am->father = this;
	n->father = this;
	as->father = this;
	ss->father = this;
}

//*******       Args		*********
Args::Args(int l, int c) : Node(l,c)
{
	this->args = new vector<Arg *>;
}

Args::Args(Arg *ar, int l, int c) : Node(l,c)
{
	this->args = new vector<Arg *>;
	this->AddArg(ar);
}

void
Args::AddArg(Arg *ar)
{
	this->args->push_back(ar);
	ar->father = this;
}

Arg::Arg(Type *ty, Ident *n, int l, int c) : Node(l,c)
{
	this->type = ty;
	this->name = n;

	ty->father = this;
	n->father = this;
}

//*******     Variables		*********
Variables::Variables(int l, int c) : Node(l,c)
{
	this->variables = new vector<Variable *>;
}

Variables::Variables(Variable *va, int l, int c) : Node(l,c)
{
	this->variables = new vector<Variable *>;
	this->AddVariable(va);
}

void
Variables::AddVariable(Variable *va)
{
	this->variables->push_back(va);
	va->father = this;
}

Variable::Variable(Ident *n, int l, int c) : Node(l,c)
{
	this->name = n;
	n->father = this;
	this->expr = NULL;
}

Variable::Variable(Ident *n, Expr *e, int l, int c) : Node(l,c)
{
	this->name = n;
	this->expr = e;
	n->father = this;
	e->father = this;
}

//*******    AccessModif	*********
AccessModif::AccessModif(int at, int l, int c) : Node(l,c)
{
	this->acctype = at;
}

//*******       Type		*********
Type::Type(int l, int c) : Node(l,c)
{
	this->type = 0;
	this->arr_level = 0;
	this->arr_capacity[0] = -1;
	this->arr_capacity[1] = -1;
	this->arr_capacity[2] = -1;
	this->name = NULL;
}

//*******    NoArrayType	*********
NoArrayType::NoArrayType(int t, int l, int c) : Type(l,c)
{
	this->type = t;
	this->arr_level = 0;
}

NoArrayType::NoArrayType(Ident *n, int l, int c) : Type(l,c)
{
	this->type = 6;
	this->arr_level = 0;
	this->name = n;
	n->father = this;
}

//*******     ArrayType		*********
ArrayType::ArrayType(int t, int al, int l, int c) : Type(l,c)
{
	this->type = t;
	this->arr_level = al;
}

ArrayType::ArrayType(int al, Ident *n, int l, int c) : Type(l,c)
{
	this->type = 6;
	this->arr_level = al;
	this->name = n;
	n->father = this;
}

//*******       Ident		*********
Ident::Ident(string n, int l, int c) : Node(l, c)
{
	this->name = n;
	this->symbol = NULL;
}

//***********************************************************************
//					Expression
//***********************************************************************

Expr::Expr(int l, int c) : Node(l, c)
{
	this->type = new Type(l,c);
	this->num = -99999999999999999;
}

//*******       Incr		*********
Incr::Incr(Ident *id, bool ip, int l, int c) : Expr(l, c)
{
	this->name = id;
	this->isPrev = ip;
	id->father = this;
}

//*******       Decr		*********
Decr::Decr(Ident *id, bool ip, int l, int c) : Expr(l, c)
{
	this->name = id;
	this->isPrev = ip;
	id->father = this;
}

//*******        Not		*********
Not::Not(Expr *e, int l, int c) : Expr(l, c)
{
	this->expr = e;
	e->father = this;
}

//*******      Minus		*********
Minus::Minus(Expr *e, int l, int c) : Expr(l, c)
{
	this->expr = e;
	e->father = this;
}

//*******       Plus		*********
Plus::Plus(Expr *e, int l, int c) : Expr(l, c)
{
	this->expr = e;
	e->father = this;
}

//*******      Paren		*********
Paren::Paren(Expr *e, int l, int c) : Expr(l, c)
{
	this->expr = e;
	e->father = this;
}

//*******     IdentExpr		*********
IdentExpr::IdentExpr(Ident *id, int l, int c) : Expr(l, c)
{
	this->ident = id;
	id->father = this;
}

//*******     IdentArr		*********
IdentArr::IdentArr(Ident *id, ArrayIndex *ai, int l, int c) : Expr(l, c)
{
	this->ident = id;
	this-> arrayIndex = ai;
	id->father = this;
	ai->father = this;
}

//*******      Assign		*********
Assign::Assign(Expr *m, Expr *r, int l, int c) : Expr(l, c)
{
	this->left = m;
	this->right = r;
	m->father = this;
	r->father = this;
}

//*******       Invoke		*********
Invoke::Invoke(Ident *id, ExprList *el, int l, int c) : Expr(l,c)
{
	this->ident = id;
	this->exprList = el;
	id->father = this;
	el->father = this;
}

//*******     NewObject		*********
NewObject::NewObject(Ident *id, ExprList *el, int l, int c) : Expr(l,c)
{
	this->ident = id;
	this->exprList = el;

	id->father = this;
	el->father = this;
}

//*******     NewArray		*********
NewArray::NewArray(Type *ty, ArrayIndex *ai, int l, int c) : Expr(l,c)
{
	this->typ = ty;
	this->arrayIndex = ai;
	
	ty->father = this;
	ai->father = this;
}

//*******     IdentCall		*********
IdentCall::IdentCall(Ident *id, Expr *e, int l, int c) : Expr(l,c)
{
	this->ident = id;
	this->expr = e;

	id->father = this;
	e->father = this;
}

//*******   IdentArrCall	*********
IdentArrCall::IdentArrCall(Ident *id, ArrayIndex *ai, Expr *e, int l, int c) : Expr(l,c)
{
	this->ident = id;
	this->arrayIndex = ai;
	this->expr = e;

	id->father = this;
	ai->father = this;
	e->father = this;
}

//*******     ThisCall  	*********
ThisCall::ThisCall(Expr *e, int l, int c) : Expr(l,c)
{
	this->expr = e;
	e->father = this;
}

//*******      Equal		*********
Equal::Equal(Expr *m, Expr *r, int l, int c) : Expr(l,c)
{
	this ->left = m;
	this->right = r;

	m->father = this;
	r->father = this;
}

//*******      NotEq		*********
NotEq::NotEq(Expr *m, Expr *r, int l, int c) : Expr(l,c)
{
	this ->left = m;
	this->right = r;

	m->father = this;
	r->father = this;
}

//*******     Smaller		*********
Smaller::Smaller(Expr *m, Expr *r, int l, int c) : Expr(l,c)
{
	this ->left = m;
	this->right = r;

	m->father = this;
	r->father = this;
}

//*******     SmallerEq		*********
SmallerEq::SmallerEq(Expr *m, Expr *r, int l, int c) : Expr(l,c)
{
	this ->left = m;
	this->right = r;

	m->father = this;
	r->father = this;
}

//*******     Larger		*********
Larger::Larger(Expr *m, Expr *r, int l, int c) : Expr(l,c)
{
	this ->left = m;
	this->right = r;

	m->father = this;
	r->father = this;
}

//*******     LargerEq		*********
LargerEq::LargerEq(Expr *m, Expr *r, int l, int c) : Expr(l,c)
{
	this ->left = m;
	this->right = r;

	m->father = this;
	r->father = this;
}

//*******        Add		*********
Add::Add(Expr *m, Expr *r, int l, int c) : Expr(l,c)
{
	this ->left = m;
	this->right = r;

	m->father = this;
	r->father = this;
}

//*******        Sub		*********
Sub::Sub(Expr *m, Expr *r, int l, int c) : Expr(l,c)
{
	this ->left = m;
	this->right = r;

	m->father = this;
	r->father = this;
}

//*******        Mult		*********
Mult::Mult(Expr *m, Expr *r, int l, int c) : Expr(l,c)
{
	this ->left = m;
	this->right = r;

	m->father = this;
	r->father = this;
}

//*******        Div		*********
Div::Div(Expr *m, Expr *r, int l, int c) : Expr(l,c)
{
	this ->left = m;
	this->right = r;

	m->father = this;
	r->father = this;
}

//*******        Mod		*********
Mod::Mod(Expr *m, Expr *r, int l, int c) : Expr(l,c)
{
	this ->left = m;
	this->right = r;

	m->father = this;
	r->father = this;
}

//*******        And		*********
And::And(Expr *m, Expr *r, int l, int c) : Expr(l,c)
{
	this ->left = m;
	this->right = r;

	m->father = this;
	r->father = this;
}

//*******         Or		*********
Or::Or(Expr *m, Expr *r, int l, int c) : Expr(l,c)
{
	this ->left = m;
	this->right = r;

	m->father = this;
	r->father = this;
}

//*******         Is		*********
Is::Is(Expr *e, Type *ty, int l, int c) : Expr(l,c)
{
	this->expr = e;
	this->typ = ty;

	e->father = this;
	ty->father = this;
}

//*******        Cast		*********
Cast::Cast(Type *t, Expr *e, int l, int c) : Expr(l,c)
{
	this->typ = t;
	this->expr = e;

	t->father = this;
	e->father = this;
}

//*******      Integer		*********
Integer::Integer(int v, int l, int c) : Expr(l,c)
{
	this->value = v;
	this->type->type = 2;
}

//*******       Real		*********
Real::Real(double v, int l, int c) : Expr(l,c)
{
	this->value = v;
	this->type->type = 3;
}

//*******       True		*********
True::True(int l, int c) : Expr(l,c)
{
	this->type->type = 4;
}

//*******      False		*********
False::False(int l, int c) : Expr(l,c)
{
	this->type->type = 4;
}

//*******       This		*********
This::This(int l, int c) : Expr(l,c)
{
}

//*******       Null		*********
Null::Null(int l, int c) : Expr(l,c)
{
	this->type->type = 1;
}

//*******     ArrayIndex	*********
ArrayIndex::ArrayIndex(int l, int c) : Node(l,c)
{
	this->arr_level = 0;
	this->arr_capacity[0] = -1;
	this->arr_capacity[1] = -1;
	this->arr_capacity[2] = -1;
}

ArrayIndex_1::ArrayIndex_1(Expr *exp1, int l, int c) : ArrayIndex(l,c)
{
	this->arr_level = 1;
	this->expr1=exp1;
	exp1->father = this;
}

ArrayIndex_2::ArrayIndex_2(Expr *exp1, Expr *exp2, int l, int c) : ArrayIndex(l,c)
{
	this->arr_level = 2;

	this->expr1=exp1;
	this->expr2=exp2;

	expr1->father = this;
	expr2->father = this;
}

ArrayIndex_3::ArrayIndex_3(Expr *exp1, Expr *exp2, Expr *exp3, int l, int c) : ArrayIndex(l,c)
{
	this->arr_level = 3;

	this->expr1=exp1;
	this->expr2=exp2;
	this->expr3=exp3;

	expr1->father = this;
	expr2->father = this;
	expr3->father = this;
}

//*******     ExprList		*********
ExprList::ExprList(int l, int c) : Node(l,c)
{
	this->exprList = new vector<Expr *>;
}

ExprList::ExprList(Expr *e, int l, int c) : Node(l,c)
{
	this->exprList = new vector<Expr *>;
	this->AddExpr(e);
}

void
ExprList::AddExpr(Expr *e)
{
	this->exprList->push_back(e);
	e->father = this;
}

//***********************************************************************
//					Statement
//***********************************************************************

Stat::Stat(int l, int c) : Node(l,c)
{
	// empty
}

Stats::Stats(int l, int c) : Node(l,c)
{
	this->stats = new vector<Stat *>;
}

Stats::Stats(Stat *i, int l, int c) : Node(l,c)
{
	this->stats = new vector<Stat *>;
	this->AddStat(i);
}

void
Stats::AddStat(Stat *i)
{
	this->stats->push_back(i);
	i->father = this;
}

//*******         If		*********
If::If(Expr *e, Stat *s, int l, int c) : Stat(l,c)
{
	this->expr = e;
	this->stat = s;

	e->father = this;
	s->father = this;
}

//*******      If Else		*********
IfElse::IfElse(Expr *e, Stat *s_if, Stat *s_else, int l, int c) : If(e, s_if, l, c)
{
	this->elseStat = s_else;
	s_else = this;
}

//*******      While		*********
While::While(Expr *e, Stat *s, int l, int c) : Stat(l,c)
{
	this->expr = e;
	this->stat = s;

	e->father = this;
	s->father = this;
}

//*******        For		*********
For::For(Variables_e *vse, Expr *e_cond, Expr *e_count, Stat *s, int l, int c) : Stat(l,c)
{
	this->variables_e = vse;
	this->exprCond = e_cond;
	this->exprCount = e_count;
	this->stat = s;

	if(vse != NULL)
		vse->father = this;
	e_cond->father = this;
	e_count->father = this;
	s->father = this;
}

//*******      ExprStat		*********
ExprStat::ExprStat(Expr *e, int l, int c) : Stat(l,c)
{
	this->expr = e;
	e->father = this;
}

//*******   VariablesStat	*********
VariablesStat::VariablesStat(Type *ty, Variables *vs, int l, int c) : Stat(l,c)
{
	this->type = ty;
	this->variables = vs;
	ty->father = this;
	vs->father = this;
}

//*******     Semicolon		*********
Semi::Semi(int l, int c) : Stat(l,c)
{
}

//*******      Block		*********
Block::Block(Stats *ss, int l, int c) : Stat(l,c)
{
	this->stats = ss;
	ss->father = this;
}

//*******      Return		*********
Return::Return(Expr *e, int l, int c) : Stat(l,c)
{
	this->expr = e;
	this->function = NULL;
	e->father = this;
}

//*******     Variable_e	*********
Variables_e::Variables_e(Type *ty, Variables *vs, int l, int c) : Node(l,c)
{
	this->type = ty;
	this->variables = vs;
	ty->father = this;
	vs->father = this;
}

Variables_e::Variables_e(Variables *vs, int l, int c) : Node(l,c)
{
	//this->type = new Type(l,c);
	//if(vs->variables->at(0)->name->symbol != NULL)
	//	this->type->type = vs->variables->at(0)->name->symbol->type;
	//else
	//	this->type->type = 0;

	this->type = NULL;
	this->variables = vs;

	vs->father = this;
	//this->type->father = this;
}

//***********************************************************************
//					Accept
//***********************************************************************

void
Node::accept(Visitor *v)
{
	// empty
}

void
Root::accept(Visitor *v)
{
	v->Visit(this);
}

void
Class::accept(Visitor *v)
{
	v->Visit(this);
}

void
ClassInher::accept(Visitor *v)
{
	v->Visit(this);
}


void
Members::accept(Visitor *v)
{
	v->Visit(this);
}

void
Member::accept(Visitor *v)
{
	v->Visit(this);
}

void
Global::accept(Visitor *v)
{
	v->Visit(this);
}

void
Constructor::accept(Visitor *v)
{
	v->Visit(this);
}

void
Function::accept(Visitor *v)
{
	v->Visit(this);
}

void
Arg::accept(Visitor *v)
{
	v->Visit(this);
}

void
Args::accept(Visitor *v)
{
	v->Visit(this);
}

void
Variables::accept(Visitor *v)
{
	v->Visit(this);
}

void
Variable::accept(Visitor *v)
{
	v->Visit(this);
}

void
AccessModif::accept(Visitor *v)
{
	v->Visit(this);
}

void
Type::accept(Visitor *v)
{
	v->Visit(this);
}

void
NoArrayType::accept(Visitor *v)
{
	v->Visit(this);
}

void
ArrayType::accept(Visitor *v)
{
	v->Visit(this);
}

void
Ident::accept(Visitor *v)
{
	v->Visit(this);
}

void
Expr::accept(Visitor *v)
{
	v->Visit(this);
}

void
Incr::accept(Visitor *v)
{
	v->Visit(this);
}

void
Decr::accept(Visitor *v)
{
	v->Visit(this);
}

void
Not::accept(Visitor *v)
{
	v->Visit(this);
}

void
Minus::accept(Visitor *v)
{
	v->Visit(this);
}

void
Plus::accept(Visitor *v)
{
	v->Visit(this);
}

void
Paren ::accept(Visitor *v)
{
	v->Visit(this);
}

void
IdentExpr::accept(Visitor *v)
{
	v->Visit(this);
}

void
IdentArr::accept(Visitor *v)
{
	v->Visit(this);
}

void
Assign::accept(Visitor *v)
{
	v->Visit(this);
}

void
Invoke::accept(Visitor *v)
{
	v->Visit(this);
}

void
NewObject::accept(Visitor *v)
{
	v->Visit(this);
}

void
NewArray::accept(Visitor *v)
{
	v->Visit(this);
}

void
IdentCall::accept(Visitor *v)
{
	v->Visit(this);
}

void
IdentArrCall::accept(Visitor *v)
{
	v->Visit(this);
}

void
ThisCall::accept(Visitor *v)
{
	v->Visit(this);
}

void
Equal::accept(Visitor *v)
{
	v->Visit(this);
}

void
NotEq::accept(Visitor *v)
{
	v->Visit(this);
}

void
Smaller::accept(Visitor *v)
{
	v->Visit(this);
}

void
SmallerEq::accept(Visitor *v)
{
	v->Visit(this);
}

void
Larger::accept(Visitor *v)
{
	v->Visit(this);
}

void
LargerEq::accept(Visitor *v)
{
	v->Visit(this);
}

void
Add::accept(Visitor *v)
{
	v->Visit(this);
}

void
Sub::accept(Visitor *v)
{
	v->Visit(this);
}

void
Mult::accept(Visitor *v)
{
	v->Visit(this);
}

void
Div::accept(Visitor *v)
{
	v->Visit(this);
}

void
Mod::accept(Visitor *v)
{
	v->Visit(this);
}

void
And::accept(Visitor *v)
{
	v->Visit(this);
}

void
Or::accept(Visitor *v)
{
	v->Visit(this);
}

void
Is::accept(Visitor *v)
{
	v->Visit(this);
}

void
Cast::accept(Visitor *v)
{
	v->Visit(this);
}

void
Integer::accept(Visitor *v)
{
	v->Visit(this);
}

void
Real::accept(Visitor *v)
{
	v->Visit(this);
}

void
True::accept(Visitor *v)
{
	v->Visit(this);
}

void
False::accept(Visitor *v)
{
	v->Visit(this);
}

void
This::accept(Visitor *v)
{
	v->Visit(this);
}

void
Null::accept(Visitor *v)
{
	v->Visit(this);
}

void
ArrayIndex::accept(Visitor *v)
{
	v->Visit(this);
}

void
ArrayIndex_1::accept(Visitor *v)
{
	v->Visit(this);
}

void
ArrayIndex_2::accept(Visitor *v)
{
	v->Visit(this);
}

void
ArrayIndex_3::accept(Visitor *v)
{
	v->Visit(this);
}

void
ExprList::accept(Visitor *v)
{
	v->Visit(this);
}

void
Stat::accept(Visitor *v)
{
	v->Visit(this);
}

void
Stats::accept(Visitor *v)
{
	v->Visit(this);
}

void
If::accept(Visitor *v)
{
	v->Visit(this);
}

void
IfElse::accept(Visitor *v)
{
	v->Visit(this);
}

void
While::accept(Visitor *v)
{
	v->Visit(this);
}

void
For::accept(Visitor *v)
{
	v->Visit(this);
}

void
ExprStat::accept(Visitor *v)
{
	v->Visit(this);
}

void
VariablesStat::accept(Visitor *v)
{
	v->Visit(this);
}

void
Semi::accept(Visitor *v)
{
	v->Visit(this);
}

void
Block::accept(Visitor *v)
{
	v->Visit(this);
}

void
Return::accept(Visitor *v)
{
	v->Visit(this);
}

void
Variables_e::accept(Visitor *v)
{
	v->Visit(this);
}