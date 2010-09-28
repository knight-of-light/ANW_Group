#include "visitors.h"

//*******      Node		*************
Node::Node(int line, int column)
{
	this->line = line;
	this->column = column;
	this->father = NULL;
}

//*******     File		*************
File::File(int l, int c) : Node(l,c)
{
}

//*******     Root		*************
Root::Root(int l, int c) : File(l,c)
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
	this->members = ms;
	this->Parents = new vector<Ident *>;

	n->father = this;
	ms->father = this;
}
void
Class::AddParent(Ident *pa) 
{
	this->Parents->push_back(pa);
	pa->father = this;
}
ClassInher::ClassInher(Ident *n, Ident *b, Members *ms, int l, int c) : Class(n,ms,l,c)
{
	this->base = b;
	b->father = this;
	this->Parents=new vector<Ident *>;
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

	am->father = this;
	ty->father = this;
	n->father = this;
	as->father = this;
	ss->father = this;
}

Function::Function(AccessModif *am, Ident *n, Args *as, Stats *ss, int l, int c) : Member(l,c)
{
	this->accessModif = am;
	this->type = NULL;	/* +++++++++++++++=== we shuld enter value like 0 to match void ====++++++++++++++++++++++++++++++++++++++++++++ */
	this->name = n;
	this->args = as;
	this->stats = ss;

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
ArrayType::ArrayType(int t, int at, int l, int c) : Type(l,c)
{
	this->type = t;
	this->arrayType = at;
}

ArrayType::ArrayType(int at, Ident *n, int l, int c) : Type(l,c)
{
	this->type = 6;
	this->arrayType = at;
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
Incr::Incr(QualNArray *qu, bool ip, int l, int c) : Expr(l, c)
{
	this->qual = qu;
	this->isPrev = ip;
	qu->father = this;
}

//*******       Decr		*********
Decr::Decr(QualNArray *qu, bool ip, int l, int c) : Expr(l, c)
{
	this->qual = qu;
	this->isPrev = ip;
	qu->father = this;
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

Paren::Paren(Ident *n, int l, int c) : Expr(l,c)
{
	this->name = n;
	n->father = this->expr;
}

//*******      Assign		*********
Assign::Assign(QualNArray *qna, Expr *e, int l, int c) : Expr(l,c)
{
	this->qualNArray = qna;
	this->expr = e;
	qna->father = this;
	e->father = this;
}

//*******       Invoke		*********
Invoke::Invoke(QualName *qn, ExprList *el, int l, int c) : Expr(l,c)
{
	this->qualName = qn;
	this->exprList = el;
	qn->father = this;
	el->father = this;
}

//*******     InvokeArr		*********
InvokeArr::InvokeArr(QualName *qn, ExprList *el, ArrayIndex *ai, int l, int c) : Expr(l,c)
{
	this->qualName = qn;
	this->exprList = el;
	this->arrayIndex = ai;

	qn->father = this;
	el->father = this;
	ai->father = this;
}

//*******     NewObject		*********
NewObject::NewObject(Ident *id, ExprList *el, int l, int c) : Expr(l,c)
{
	this->ident = id;
	this->exprList = el;

	id->father = this;
	el->father = this;
}

//*******      NewArr		*********
NewArr::NewArr(Ident *id, ArrayIndex *ai, int l, int c) : Expr(l,c)
{
	this->ident = id;
	this->arrayIndex = ai;

	id->father = this;
	ai->father = this;
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

Cast::Cast(Ident *n, Expr *e, int l, int c) : Expr(l,c)
{
	this->typ->type = 6;
	this->typ->name = n;
	this->expr = e;

	n->father = this->typ;
	typ->father = this;

	e->father = this;
}

//*******      Integer		*********
Integer::Integer(int v, int l, int c) : Expr(l,c)
{
	this->value = v;
	this->type = 1;
}

//*******       Real		*********
Real::Real(double v, int l, int c) : Expr(l,c)
{
	this->value = v;
	this->type = 2;
}

//*******       True		*********
True::True(int l, int c) : Expr(l,c)
{
	this->type = 3;
}

//*******      False		*********
False::False(int l, int c) : Expr(l,c)
{
	this->type = 3;
}

//*******       This		*********
This::This(int l, int c) : Expr(l,c)
{
}

//*******       Null		*********
Null::Null(int l, int c) : Expr(l,c)
{
	this->type = 0;
}

//*******     ArrayIndex	*********
ArrayIndex::ArrayIndex(int l, int c) : Node(l,c)
{
}

ArrayIndex_1::ArrayIndex_1(Expr *exp1, int l, int c) : ArrayIndex(l,c)
{
	this->expr1=exp1;
	exp1->father = this;
}

ArrayIndex_2::ArrayIndex_2(Expr *exp1, Expr *exp2, int l, int c) : ArrayIndex(l,c)
{
	this->expr1=exp1;
	this->expr2=exp2;

	expr1->father = this;
	expr2->father = this;
}

ArrayIndex_3::ArrayIndex_3(Expr *exp1, Expr *exp2, Expr *exp3, int l, int c) : ArrayIndex(l,c)
{
	this->expr1=exp1;
	this->expr2=exp2;
	this->expr3=exp3;

	expr1->father = this;
	expr2->father = this;
	expr3->father = this;
}

//*******   QualNameOrArr	*********
QualNArray::QualNArray(int l, int c):Expr(l ,c)
{
	this->ident = NULL;
}

QualNArray_ID_Index::QualNArray_ID_Index(Ident *ident, ArrayIndex *index, int l, int c) : QualNArray(l,c)
{
	this->ident=ident;
	this->index=index;

	ident->father = this;
	index->father = this;
}

QualNArray_Exp_Index::QualNArray_Exp_Index(Ident* ident, Expr* expr,ArrayIndex* index ,int l, int c ):QualNArray(l,c)
{
	this->ident=ident;
	this->expr=expr;
	this->index=index;

	ident->father = this;
	expr->father = this;
	index->father = this;
}

//*******   QualifiedName	*********
QualName::QualName(int l , int  c) : QualNArray(l,c)
{
	this->ident = NULL;
}

QualName_ID::QualName_ID(Ident *ident, int l, int c) : QualName(l,c)
{
	this->ident = ident;

	ident->father = this;
}

QualName_Exp::QualName_Exp(Ident *ident, Expr *expr, int l, int c) : QualName(l,c)
{
	this->ident=ident;
	this->expr=expr;

	ident->father = this;
	expr->father = this;
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
	e->father = this;
}

//*******     Variable_e	*********
Variables_e::Variables_e(Type *ty, Variables *vs, int l,int c) : Node(l,c)
{
	this->type = ty;
	this->variables = vs;
	ty->father = this;
	vs->father = this;
}


//***********************************************************************
//					Accept
//***********************************************************************

void
Node::accept(Visitor *v)
{

}

void
File::accept(Visitor *v)
{
	v->Visit(this);
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
InvokeArr::accept(Visitor *v)
{
	v->Visit(this);
}

void
NewObject::accept(Visitor *v)
{
	v->Visit(this);
}

void
NewArr::accept(Visitor *v)
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
QualNArray_ID_Index::accept(Visitor *v)
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