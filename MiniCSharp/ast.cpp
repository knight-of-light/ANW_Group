
#include "ast.h"

Node::Node(int line, int column)
{
	this->line = line;
	this->column = column;
	this->father = NULL;
}

Root::Root(int l, int c):Node(l,c)
{
	this->classes = new vector<ClassDef *>;
}

void
Root::AddClass(ClassDef *cd)
{
	this->classes->push_back(cd);
	cd->father = this;
}

ClassDef::ClassDef(Ident *n, Members *fs, int l, int c): Node(l, c)
{
	this->name = n;
	this->members = fs;
	
	n->father = this;
	fs->father = this;
}

Members::Members(int l, int c) : Node(l, c)
{
	this->members = new vector<Member *>;
}

void
Members::AddMember(Member *f)
{
	this->members->push_back(f);
	f->father = this;
}

Member::Member(int l, int c) : Node(l, c)
{
	
}

Global::Global(Type *et, Variables *vs, int l, int c) : Member(l, c)
{
	this->exprType = et;
	this->varDecls = vs;
	et->father = this;
	vs->father = this;
}

Function::Function(Type *et, Ident *n, Args *ps, Stats *is, int l, int c) : Member(l, c)
{
	this->exprType = et;
	this->name = n;
	this->params = ps;
	this->insts = is;
	et->father = this;
	n->father = this;
	ps->father = this;
	is->father = this;
}

Function::Function(Ident *n, Args *ps, Stats *is, int l, int c) : Member(l, c)
{
	this->exprType = NULL;
	this->name = n;
	this->params = ps;
	this->insts = is;
	n->father = this;
	ps->father = this;
	is->father = this;
}

Variables::Variables(int l, int c) : Node(l, c)
{
	this->varDecls = new vector<Variable *>;
}

Variables::Variables(Variable *vd, int l, int c) : Node(l, c)
{
	this->varDecls = new vector<Variable *>;
	this->AddVariable(vd);
}

void
Variables::AddVariable(Variable *vd)
{
	this->varDecls->push_back(vd);
	vd->father = this;
}

Variable::Variable(Ident *n, int l, int c) : Node(l, c)
{
	this->name = n;	
	n->father = this;
	this->expr = NULL;
}

Variable::Variable(Ident *n, Expr *e, int l, int c) : Node(l, c)
{
	this->name = n;
	this->expr = e;
	n->father = this;
	e->father = this;
}

Args::Args(int l, int c) : Node(l, c)
{
	this->params = new vector<Arg *>;
}

Args::Args(Arg *p, int l, int c) : Node(l, c)
{
	this->params = new vector<Arg *>;
	this->AddArg(p);
}

void
Args::AddArg(Arg *p)
{
	this->params->push_back(p);
	p->father = this;
}

Arg::Arg(Type *et, Ident *n, int l, int c) : Node(l, c)
{
	this->exprType = et;
	this->name = n;
	et->father = this;
	n->father = this;
}

Type::Type(int l, int c) : Node(l,c)
{
}

Type::Type(int t, int l, int c) : Node(l, c)
{
	this->type = t;
}

NoArrayType::NoArrayType(int t, int l, int c) : Type(l,c)
{
	this->type = t;
	this->name = NULL;
}

NoArrayType::NoArrayType(Ident *n, int l, int c) : Type(l,c)
{
	this->type = 6;
	this->name = n;
	n->father = this;
}

ArrayType::ArrayType(int t, int l, int c) : Type(l,c)
{
	this->type = t;
	this->name = NULL;
}

ArrayType::ArrayType(int t, Ident *n, int l, int c) : Type(l,c)
{
	this->type = t + (6*10); /* 6: Ident type number , 10: Add 10 for Array type number */
	this->name = n;
	n->father = this;
}

Ident::Ident(string n, int l, int c) : Node(l, c)
{
	this->name = n;
	this->symbol = NULL;
}

//***********************************************************************
//					Expr
//***********************************************************************

Expr::Expr(int l, int c) : Node(l, c)
{

}

ExprList::ExprList(int l, int c) : Node(l, c)
{
	this->exprList = new vector<Expr *>;
}

ExprList::ExprList(Expr *e, int l, int c) : Node(l, c)
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


Integer::Integer(int v, int l, int c) : Expr(l, c)
{
	this->value = v;
	this->type = 1;
}

Real::Real(int v, int l, int c) : Expr(l, c)
{
	this->value = v;
	this->type = 2;
}

True::True(int l, int c) : Expr(l, c)
{
	this->type = 3;
}

False::False(int l, int c) : Expr(l, c)
{
}

Null::Null(int l, int c) : Expr(l, c)
{
	this->type = 3;
}

Paren::Paren(Expr *e, int l, int c) : Expr(l, c)
{
	this->expr = e;
	e->father = this;
}

IdentExpr::IdentExpr(Ident *id, int l, int c) : Expr(l, c)
{
	this->ident = id;
	id->father = this;
}

Assign::Assign(Ident *id, Expr *e, int l, int c) : Expr(l, c)
{
	this->ident = id;
	this->expr = e;
	id->father = this;
	e->father = this;
}

Invoke::Invoke(Ident *id, ExprList *el, int l, int c) : Expr(l, c)
{
	this->ident = id;
	this->exprList = el;
	id->father = this;
	el->father = this;
}

Not::Not(Expr *e, int l, int c) : Expr(l, c)
{
	this->expr = e;
	e->father = this;
}

Plus::Plus(Expr *e, int l, int c) : Expr(l, c)
{
	this->expr = e;
	e->father = this;
}

Minus::Minus(Expr *e, int l, int c) : Expr(l, c)
{
	this->expr = e;
	e->father = this;
}

Incr::Incr(Ident *id, bool ip, int l, int c) : Expr(l, c)
{
	this->ident = id;
	this->isPrev = ip;
	id->father = this;
}

Decr::Decr(Ident *id, bool ip, int l, int c) : Expr(l, c)
{
	this->ident = id;
	this->isPrev = ip;
	id->father = this;
}

Add::Add(Expr *f, Expr *r, int l, int c) : Expr(l, c)
{
	this ->left = f;
	this->right = r;
	f->father = this;
	r->father = this;
}

Sub::Sub(Expr *f, Expr *r, int l, int c) : Expr(l, c)
{
	this ->left = f;
	this->right = r;
	f->father = this;
	r->father = this;
}

Mult::Mult(Expr *f, Expr *r, int l, int c) : Expr(l, c)
{
	this ->left = f;
	this->right = r;
	f->father = this;
	r->father = this;
}

Div::Div(Expr *f, Expr *r, int l, int c) : Expr(l, c)
{
	this ->left = f;
	this->right = r;
	f->father = this;
	r->father = this;
}

Mod::Mod(Expr *f, Expr *r, int l, int c) : Expr(l, c)
{
	this ->left = f;
	this->right = r;
	f->father = this;
	r->father = this;
}

Smaller::Smaller(Expr *f, Expr *r, int l, int c) : Expr(l, c)
{
	this ->left = f;
	this->right = r;
	f->father = this;
	r->father = this;
}

Largser::Largser(Expr *f, Expr *r, int l, int c) : Expr(l, c)
{
	this ->left = f;
	this->right = r;
	f->father = this;
	r->father = this;
}

SmallerEq::SmallerEq(Expr *f, Expr *r, int l, int c) : Expr(l, c)
{
	this ->left = f;
	this->right = r;
	f->father = this;
	r->father = this;
}

LargserEq::LargserEq(Expr *f, Expr *r, int l, int c) : Expr(l, c)
{
	this ->left = f;
	this->right = r;
	f->father = this;
	r->father = this;
}

Equal::Equal(Expr *f, Expr *r, int l, int c) : Expr(l, c)
{
	this ->left = f;
	this->right = r;
	f->father = this;
	r->father = this;
}

NotEq::NotEq(Expr *f, Expr *r, int l, int c) : Expr(l, c)
{
	this ->left = f;
	this->right = r;
	f->father = this;
	r->father = this;
}

Or::Or(Expr *f, Expr *r, int l, int c) : Expr(l, c)
{
	this ->left = f;
	this->right = r;
	f->father = this;
	r->father = this;
}

And::And(Expr *f, Expr *r, int l, int c) : Expr(l, c)
{
	this ->left = f;
	this->right = r;
	f->father = this;
	r->father = this;
}

//***********************************************************************
//					Statruction
//***********************************************************************

Stat::Stat(int l, int c) : Node(l, c)
{
}

Stats::Stats(int l, int c) : Node(l, c)
{
	this->insts = new vector<Stat *>;
}

Stats::Stats(Stat *i, int l, int c) : Node(l, c)
{
	this->insts = new vector<Stat *>;
	this->AddStat(i);
}

void
Stats::AddStat(Stat *i)
{
	this->insts->push_back(i);
	i->father = this;
}

ExprStat::ExprStat(Expr *e, int l, int c) : Stat(l, c)
{
	this->expr = e;
	e->father = this;
}

VariablesStat::VariablesStat(Type *et, Variables *vds, int l, int c) : Stat(l, c)
{
	this->exprType = et;
	this->vardecls = vds;
	et->father = this;
	vds->father = this;
}

If::If(Expr *e, Stat *i, int l, int c) : Stat(l, c)
{
	this->expr = e;
	this->inst = i;
	e->father = this;
	i->father = this;
}

IfElse::IfElse(Expr *e, Stat *i_if, Stat *i_else, int l, int c) : If(e, i_if, l, c)
{
	this->elseStat = i_else;
	i_else = this;
}

While::While(Expr *e, Stat *i, int l, int c) : Stat(l, c)
{
	this->expr = e;
	this->inst = i;
	e->father = this;
	i->father = this;
}

Variables_e::Variables_e(Type *et, Variables *vds, int l,int c) : Node(l, c)
{
	this->exprType = et;
	this->varDecls = vds;
	et->father = this;
	vds->father = this;
}

For::For(Variables_e *vsd, Expr *e_cond, Expr *e_count, Stat *i, int l, int c) : Stat(l, c)
{
	this->varsDecl = vsd;
	this->exprCond = e_cond;
	this->exprCount = e_count;
	this->inst = i;
	vsd->father = this;
	e_cond->father = this;
	e_count->father = this;
	i->father = this;
}

Block::Block(Stats *is, int l, int c) : Stat(l, c)
{
	this->insts = is;
	is->father = this;
}

Return::Return(Expr *e, int l, int c) : Stat(l, c)
{
	this->expr = e;
	e->father = this;
}


//***********************
Error::Error(string message, int line, int column)
{
	this->message = message;
	this->line = line;
	this->column = column;
}

Errors::Errors()
{
	this->messages = new vector<Error *>;
}

void
Errors::AddError(std::string message, int line, int column)
{
	Error *error = new Error(message, line, column);
	this->messages->push_back(error);
}

void
Errors::Print()
{
	int size = this->messages->size();
	if(size == 0)
		cout << "Semntic analysis was done successfully! " << endl;
	else
	{
		cout << "There are " << size << " semantic errors: " << endl;
		for(int i = 0; i < size; i++)
		{ 
			cout << this->messages->at(i)->message << " at line: "
				<< this->messages->at(i)->line << " , column: "
				<< this->messages->at(i)->column << endl;
		}
	}
}




void
Node::accept(Visitor *v)
{
	
}

void
Root::accept(Visitor *v)
{
	
}

void
ClassDef::accept(Visitor *v)
{
	v->Visit(this);
}

void
Members::accept(Visitor *v)
{
	v->Visit(this);
}

void
Member ::accept(Visitor *v)
{
	v->Visit(this);
}

void
Global ::accept(Visitor *v)
{
	v->Visit(this);
}

void
Function::accept(Visitor *v)
{
	v->Visit(this);
}

void
Variables::accept(Visitor *v)
{
	v->Visit(this);
}

void
Variable ::accept(Visitor *v)
{
	v->Visit(this);
}

void
Args::accept(Visitor *v)
{
	v->Visit(this);
}

void
Arg ::accept(Visitor *v)
{
	v->Visit(this);
}

void
Type::accept(Visitor *v)
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
ExprList::accept(Visitor *v)
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
Incr::accept(Visitor *v)
{
	v->Visit(this);
}

void
Add::accept(Visitor *v)
{
	v->Visit(this);
}

void
Mult::accept(Visitor *v)
{
	v->Visit(this);
}

void
Largser::accept(Visitor *v)
{
	v->Visit(this);
}

void
LargserEq::accept(Visitor *v)
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
Variables_e::accept(Visitor *v)
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
Null::accept(Visitor *v)
{
	v->Visit(this);
}

void
Plus::accept(Visitor *v)
{
	v->Visit(this);
}

void
Decr::accept(Visitor *v)
{
	v->Visit(this);
}

void
Sub::accept(Visitor *v)
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
Or::accept(Visitor *v)
{
	v->Visit(this);
}

void
And::accept(Visitor *v)
{
	v->Visit(this);
}

void
For::accept(Visitor *v)
{
	v->Visit(this);
}
