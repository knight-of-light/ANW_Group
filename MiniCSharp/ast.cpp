#include "ast.h"

//*******      Node		*************
Node::Node(int line, int column)
{
	this->line = line;
	this->column = column;
	this->father = NULL;
}

//*******     Root		*************
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

//*******     ClassDef		*********
ClassDef::ClassDef(Ident *n, Members *fs, int l, int c): Node(l, c)
{
	this->name = n;
	this->supName = NULL;
	this->members = fs;
	
	n->father = this;
	fs->father = this;
}

ClassDef::ClassDef(Ident *n, Ident *sn, Members *fs, int l, int c) : Node(l, c)
{
	this->name = n;
	this->supName = sn;
	this->members = fs;

	n->father = this;
	sn->father = this;
	fs->father = this;
}

//*******     Members		*********
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

//*******     Global		*********
Global::Global(AccessModif	*am, Type *et, Variables *vs, int l, int c) : Member(l, c)
{
	this->accessModif = am;
	this->exprType = et;
	this->varDecls = vs;
	am->father = this;
	et->father = this;
	vs->father = this;
}

//*******   Constructor		*********
Constructor::Constructor(AccessModif * am, Ident *n, Args *ps, Stats *is, int l, int c) : Member(l, c)
{
	this->accessModif = am;
	this->name = n;
	this->params = ps;
	this->insts = is;
	am->father = this;
	n->father = this;
	ps->father = this;
	is->father = this;
}

//*******     Function		*********
Function::Function(AccessModif *am, Type *et, Ident *n, Args *ps, Stats *is, int l, int c) : Member(l, c)
{
	this->accessModif = am;
	this->exprType = et;
	this->name = n;
	this->params = ps;
	this->insts = is;
	am->father = this;
	et->father = this;
	n->father = this;
	ps->father = this;
	is->father = this;
}

Function::Function(AccessModif *am, Ident *n, Args *ps, Stats *is, int l, int c) : Member(l, c)
{
	this->accessModif = am;
	this->exprType = NULL;
	this->name = n;
	this->params = ps;
	this->insts = is;
	am->father = this;
	n->father = this;
	ps->father = this;
	is->father = this;
}

//*******     Variables		*********
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

//*******       Args		*********
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

//*******    AccessModif	*********
AccessModif::AccessModif(int at, int l, int c) : Node(l, c)
{
	this->acctype = at;
}

//*******       Type		*********
Type::Type(int l, int c) : Node(l,c)
{
	this->name = NULL;
}

//*******    NoArrayType	*********
NoArrayType::NoArrayType(int t, int l, int c) : Type(l,c)
{
	this->type = t;
}

NoArrayType::NoArrayType(Ident *n, int l, int c) : Type(l,c)
{
	this->type = 6;
	this->name = n;
	n->father = this;
}

//*******     ArrayType		*********
ArrayType::ArrayType(int t, int l, int c) : Type(l,c)
{
	this->type = t;
}

ArrayType::ArrayType(int t, Ident *n, int l, int c) : Type(l,c)
{
	this->type = t + (6*10); /* 6: Ident type number , 10: Add 10 for Array type number */
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

}

//*******       Incr		*********
Incr::Incr(Ident *id, bool ip, int l, int c) : Expr(l, c)
{
	this->ident = id;
	this->isPrev = ip;
	id->father = this;
}

//*******       Decr		*********
Decr::Decr(Ident *id, bool ip, int l, int c) : Expr(l, c)
{
	this->ident = id;
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

//*******      Assign		*********
Assign::Assign(Ident *id, Expr *e, int l, int c) : Expr(l, c)
{
	this->ident = id;
	this->expr = e;
	id->father = this;
	e->father = this;
}

//*******       Invoke		*********
Invoke::Invoke(Ident *id, ExprList *el, int l, int c) : Expr(l, c)
{
	this->ident = id;
	this->exprList = el;
	id->father = this;
	el->father = this;
}

//*******      Equal		*********
Equal::Equal(Expr *f, Expr *r, int l, int c) : Expr(l, c)
{
	this ->left = f;
	this->right = r;
	f->father = this;
	r->father = this;
}

//*******      NotEq		*********
NotEq::NotEq(Expr *f, Expr *r, int l, int c) : Expr(l, c)
{
	this ->left = f;
	this->right = r;
	f->father = this;
	r->father = this;
}

//*******     Smaller		*********
Smaller::Smaller(Expr *f, Expr *r, int l, int c) : Expr(l, c)
{
	this ->left = f;
	this->right = r;
	f->father = this;
	r->father = this;
}

//*******     SmallerEq		*********
SmallerEq::SmallerEq(Expr *f, Expr *r, int l, int c) : Expr(l, c)
{
	this ->left = f;
	this->right = r;
	f->father = this;
	r->father = this;
}

//*******     Larger		*********
Larger::Larger(Expr *f, Expr *r, int l, int c) : Expr(l, c)
{
	this ->left = f;
	this->right = r;
	f->father = this;
	r->father = this;
}

//*******     LargerEq		*********
LargerEq::LargerEq(Expr *f, Expr *r, int l, int c) : Expr(l, c)
{
	this ->left = f;
	this->right = r;
	f->father = this;
	r->father = this;
}

//*******        Add		*********
Add::Add(Expr *f, Expr *r, int l, int c) : Expr(l, c)
{
	this ->left = f;
	this->right = r;
	f->father = this;
	r->father = this;
}

//*******        Sub		*********
Sub::Sub(Expr *f, Expr *r, int l, int c) : Expr(l, c)
{
	this ->left = f;
	this->right = r;
	f->father = this;
	r->father = this;
}

//*******        Mult		*********
Mult::Mult(Expr *f, Expr *r, int l, int c) : Expr(l, c)
{
	this ->left = f;
	this->right = r;
	f->father = this;
	r->father = this;
}

//*******        Div		*********
Div::Div(Expr *f, Expr *r, int l, int c) : Expr(l, c)
{
	this ->left = f;
	this->right = r;
	f->father = this;
	r->father = this;
}

//*******        Mod		*********
Mod::Mod(Expr *f, Expr *r, int l, int c) : Expr(l, c)
{
	this ->left = f;
	this->right = r;
	f->father = this;
	r->father = this;
}

//*******        And		*********
And::And(Expr *f, Expr *r, int l, int c) : Expr(l, c)
{
	this ->left = f;
	this->right = r;
	f->father = this;
	r->father = this;
}

//*******         Or		*********
Or::Or(Expr *f, Expr *r, int l, int c) : Expr(l, c)
{
	this ->left = f;
	this->right = r;
	f->father = this;
	r->father = this;
}

//*******      Integer		*********
Integer::Integer(int v, int l, int c) : Expr(l, c)
{
	this->value = v;
	this->type = 1;
}

//*******       Real		*********
Real::Real(int v, int l, int c) : Expr(l, c)
{
	this->value = v;
	this->type = 2;
}

//*******       True		*********
True::True(int l, int c) : Expr(l, c)
{
	this->type = 3;
}

//*******      False		*********
False::False(int l, int c) : Expr(l, c)
{
	this->type = 3;
}

//*******       Null		*********
Null::Null(int l, int c) : Expr(l, c)
{
	this->type = 0;
}

//*******     ArrayIndex	*********
ArrayIndex::ArrayIndex(int l, int c) : Node(l, c)
{
}

ArrayIndex_1::ArrayIndex_1(Expr *exp1,int l, int c) : ArrayIndex(l, c)
{
	this->expr1=exp1;
}

ArrayIndex_2::ArrayIndex_2(Expr *exp1,Expr *exp2,int l, int c) : ArrayIndex(l, c)
{
	this->expr1=exp1;
	this->expr2=exp2;
}

ArrayIndex_3::ArrayIndex_3(Expr *exp1,Expr *exp2,Expr *exp3,int l, int c) : ArrayIndex(l, c)
{
	this->expr1=exp1;
	this->expr2=exp2;
	this->expr3=exp3;
}

//*******     QualName		*********
QualName::QualName( int l , int  c ) : Node(l ,c)
{
}

QualName_ID::QualName_ID(Ident *ident, int l , int c ) :QualName(l ,c)
{
	this->ident=ident;
}

QualName_Exp::QualName_Exp(Ident *ident ,Expr *expr , int l , int c):QualName(l ,c)
{
	this->ident=ident;
	this->expr=expr;
}

//*******     QualNArray	*********
QualNArray::QualNArray( int l, int c ):Node(l  ,c)
{
}

QualNArray_ID::QualNArray_ID(Ident* ident, int l, int c ):QualNArray(l ,c)
{
	this->ident=ident;
}

QualNArray_ID_Index::QualNArray_ID_Index( Ident* ident,ArrayIndex* index ,int l , int c ):QualNArray(l ,c)
{
	this->ident=ident;
	this->index=index;
}

QualNArray_Exp::QualNArray_Exp(Ident* ident, Expr* expr,int l, int c):QualNArray(l ,c)
{
	this->ident=ident;
	this->expr=expr;
}

QualNArray_Exp_Index::QualNArray_Exp_Index(Ident* ident, Expr* expr,ArrayIndex* index ,int l, int c ):QualNArray(l ,c)
{
	this->ident=ident;
	this->expr=expr;
	this->index=index;
}

//*******     ExprList		*********
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

//***********************************************************************
//					Statement
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

//*******         If		*********
If::If(Expr *e, Stat *i, int l, int c) : Stat(l, c)
{
	this->expr = e;
	this->inst = i;
	e->father = this;
	i->father = this;
}

//*******      If Else		*********
IfElse::IfElse(Expr *e, Stat *i_if, Stat *i_else, int l, int c) : If(e, i_if, l, c)
{
	this->elseStat = i_else;
	i_else = this;
}

//*******      While		*********
While::While(Expr *e, Stat *i, int l, int c) : Stat(l, c)
{
	this->expr = e;
	this->inst = i;
	e->father = this;
	i->father = this;
}

//*******        For		*********
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

//*******      ExprStat		*********
ExprStat::ExprStat(Expr *e, int l, int c) : Stat(l, c)
{
	this->expr = e;
	e->father = this;
}

//*******   VariablesStat	*********
VariablesStat::VariablesStat(Type *et, Variables *vds, int l, int c) : Stat(l, c)
{
	this->exprType = et;
	this->vardecls = vds;
	et->father = this;
	vds->father = this;
}

//*******      Block		*********
Block::Block(Stats *is, int l, int c) : Stat(l, c)
{
	this->insts = is;
	is->father = this;
}

//*******      Return		*********
Return::Return(Expr *e, int l, int c) : Stat(l, c)
{
	this->expr = e;
	e->father = this;
}

//*******     Variable_e	*********
Variables_e::Variables_e(Type *et, Variables *vds, int l,int c) : Node(l, c)
{
	this->exprType = et;
	this->varDecls = vds;
	et->father = this;
	vds->father = this;
}


//***********************************************************************
//					Error
//***********************************************************************
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


//***********************************************************************
//					Accept
//***********************************************************************

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
Member::accept(Visitor *v)
{
	v->Visit(this);
}

void
Global::accept(Visitor *v)
{
	v->Visit(this);
}
/*
void
Constructor::accept(Visitor *)
{
	v->Visit(this);
}
*/
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
QualName::accept(Visitor *v)
{
	v->Visit(this);
}

void
QualName_ID::accept(Visitor *v)
{
	v->Visit(this);
}

void
QualName_Exp::accept(Visitor *v)
{
	v->Visit(this);
}

void
QualNArray::accept(Visitor *v)
{
	v->Visit(this);
}

void
QualNArray_ID::accept(Visitor *v)
{
	v->Visit(this);
}

void
QualNArray_ID_Index::accept(Visitor *v)
{
	v->Visit(this);
}

void
QualNArray_Exp::accept(Visitor *v)
{
	v->Visit(this);
}

void
QualNArray_Exp_Index::accept(Visitor *v)
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