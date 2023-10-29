#include <iostream>
#include <vector>
using namespace std;

class CharStack;
class Node
{
    char info;
    Node *next;
public:
    Node() : info(0), next(nullptr) {}
    Node(char info) : info(info), next(nullptr) {}
    Node(char info, Node *next) : info(info), next(next) {}
    ~Node() { // cout << "\nDestructorul nodului a fost apelat\n";
            } // Destructorul se apeleaza numai cand se face 'delete' la obiect fiind pointer
              // lucru pe care il folosim in clasa CharStack. Puteam de asemenea sa efectuez
              // delete pe pointerul next, astfel stergand toate nodurile legate ele, dar
              // data fiind natura clasei CharStack, nu am dorit acest lucru.
    Node(const Node &other);

    void setNodeInfo(char info);
    void setNodeNext(Node *next);
    char getNodeInfo();
    Node *getNodeNext(); 

    Node &operator=(const Node &other);
    friend ostream& operator<<(ostream &out, CharStack s);
    friend class CharStack;
};

//informatia din nod este copiata, iar daca nodul urmator nu este nul,
//se initializeaza si acesta cu respectivul. 
Node::Node(const Node &other)
{
    info = other.info;
    next = nullptr;
    if (other.next != nullptr)
        next = new Node(*other.next);
    cout << "Const Copiere Node\n";
}

//asemanator cu copy construct, doar verificam daca 
// this != other ca sa nu se efectueze linii de cod in zadar.
Node &Node::operator=(const Node &other)
{
    if (this != &other)
    {
        info = other.info;
        delete next;
        if (other.next != nullptr)
            next = new Node(*other.next);
        else
            next = nullptr;
    }
    return *this;
}

//setteri si getteri
void Node::setNodeInfo(char info) { this->info = info; }
void Node::setNodeNext(Node *next) { this->next = next; }
char Node::getNodeInfo() { return info; }
Node* Node::getNodeNext() { return next; }


class CharStack
{
    Node *top;
public:
    CharStack()
    {
        Node *n = new Node();
        this->top = n;
    }
    
    CharStack(char info)
    {
        Node *n = new Node(info);
        this->top = n;
    }
    ~CharStack()
    {
        while (this->isEmpty() == 0)
            this->Pop();
        // cout << "\nDestructorul Stackului a fost apelat\n";
    }
    CharStack(CharStack& other); 
    void Push(char info);
    char Pop();
    char Top();
    bool isEmpty();
    CharStack& operator=(CharStack s);
    friend ostream& operator<<(ostream &out, CharStack s);
    friend istream& operator>>(istream &in, CharStack &s);
};



//in acest copy constructor verificam daca lista din care copiem este vida, iar daca nu
//creem un nod nou pentru top-ul nodului in care copiem, apoi mergem prin stiva 
//si punem in fiecare nod nou informatia din celelalte noduri.
CharStack::CharStack(CharStack &other)
{
    if (other.top == nullptr) 
        top = nullptr;
    else
    {
        Node *n = new Node(other.top->info);
        Node *m = other.top->next;
        this->top = n;
        while (m != nullptr) 
        {
            n->next = new Node(m->info);
            n = n->next;
            m = m->next;
        }
    }
    cout << "~Constructorul de copiere a fost apelat\n";
}

//Asemanator cu copy constructor, doar ca golim intai stack-ul in care copiem 
//prin pop-uri succesive.
CharStack& CharStack::operator=(CharStack s)
{
    while (!this->isEmpty())
        this->Pop();

    if (s.top == nullptr)
        this->top = nullptr;
    else
    {
        Node *n = new Node(s.top->info);
        this->top = n;
        s.top = s.top->next;
        while (!s.isEmpty())
        {
            n->next = new Node(s.top->info);
            n = n->next;
            s.top = s.top->next;
        }
    }
    return *this;
}

//in aceasta functie de overload al operatorului << la fiecare cout se creeaza o copie a obiectului
//si il parcurge prin s.top preluand informatia si afisand-o, iar apoi intervine destructorul la finalul functiei.
//a fost declarata friend in ambele clase, pentru a avea acces atat la top din CharStack s, dar si la
//info si next, bineinteles.
ostream& operator<<(ostream &out, CharStack s)
{ 
    while (s.isEmpty() != 1)
    {
        out << s.top->info << '\n';
        s.top = s.top->next;
    }
    return out;
}

//dam ca input un string reprezentand totalitatea pushurilor, din care luam fiecare char 
//si ii dam push.
//deasemenea puteam sa dau ca input un char array cu spatii (' ') pentru un input mai intuitiv,
//sa il delimitez prin strtok si sa ii dau push, dar as fi fost limitat din punct de vedere
//al flexibilitatii marimii sirului.  
istream& operator>>(istream &in, CharStack &s)
{
    string str;
    in >> str;
    for (int c = 0; str[c]; ++c)
        s.Push(str[c]);

    return in;
}

string invS(string s)
{
    CharStack l;
    for (int c = 0; s[c]; ++c)
        l.Push(s[c]);
    string inv = s; //un string nu poate fi creat de la 0, doar modificat. 
                    //Deci il initializam cu s pentru ca inversul are aceeasi marime
    int nr = 0;
    for (int c = 0; s[c]; ++c)
    {
        inv[c] = l.Pop();
        nr ++;
    }
    inv[nr] = 0;
    return inv;    
}

bool CharStack::isEmpty()
{
        return (this->top == nullptr);
}

char CharStack::Top()
{
    if (top != nullptr)
        return top->info;
    else
        cout << "The value on top is NULL because the stack is empty\n";
    return 0;
}

char CharStack::Pop()
{
    if (top != nullptr)
    {
        char info;
        info = this->top->info;
        Node *temp = new Node();
        temp = top;
        this->top = this->top->next;
        delete temp;
        return info;
    }
    else
    {
        cout << "Cannot Pop...The stack is empty\n";
        return 0;
    }
}

void CharStack::Push(char info)
{
        Node *n = new Node(info);
        n->next = this->top;
        this->top = n;
}

void citireMemorareAfisareNObiecte()
{
    int n;
    cin >> n;
    vector<CharStack> l(n);
    for (int i = 0; i < n; ++i)
        cin >> l[i];

    for (int i = 0; i < n; ++i)
        cout << l[i];
}

void testCitireSiAfisare()
{
    CharStack l;
    cin >> l;
    cout << l;
}

void testPushSiPop()
{
    CharStack l;
    l.Push('a');
    l.Push('b');
    l.Push('c');
    l.Push('d');
    l.Push('e');
    l.Push('f');
    l.Pop();
    l.Pop();
    cout << l;
    cout << l.Top(); //test si pentru Top() aici
    
}

void testConstructorInit()
{
    CharStack l;
    CharStack m('a');
    cout << l << '\n' << m;

}

void paramPrinVal(CharStack l)
{
    cout << l;
}

void paramPrinRef(CharStack &l)
{
    cout << l;
}

void testConstructorCopiere()
{
    CharStack l;
    cin >> l;
    paramPrinVal(l);
    paramPrinRef(l);
}

void testOperatorEgal()
{
    CharStack l, m;
    m.Push('a');
    m.Push('b');
    cin >> l;
    m = l;
    cout << m;
}

void testConstInitNode()
{
    Node *n = new Node();
    Node *m = new Node('x');
    Node *p = new Node('x', m);

    cout << n->getNodeInfo() << '\n' << m->getNodeInfo() << '\n' << p->getNodeInfo();

    delete n;
    delete m;
    delete p;
}

void testSetSiGetNode()
{
    Node *n = new Node();
    Node *m = new Node();
    n->setNodeInfo('a');
    cout << n->getNodeInfo() << '\n' << n << '\n';
    m->setNodeNext(n);
    cout << m->getNodeNext();

    delete n;
    delete m;
}

void paramPrinValNode(Node n)
{
    cout << n.getNodeInfo();
}

void paramPrinRefNode(Node &n)
{
    cout << n.getNodeInfo();
}

void testConstructorCopiereNode()
{
    Node n;
    n.setNodeInfo('a');
    paramPrinValNode(n);
    paramPrinRefNode(n);
}

void testOperatorEgalNode()
{
    Node *n = new Node();
    Node m, p;
    m.setNodeInfo('b');
    m.setNodeNext(n);
    p.setNodeInfo('x');
    p = m;
    cout << p.getNodeInfo() << ' ' << p.getNodeNext();
}

int main()
{       
    // testCitireSiAfisare(); //citire ca string e.g. "abcdef"
    // testPushSiPop();
    // testConstructorInit();
    // testConstructorCopiere();
    // testOperatorEgal();
    // cout << invS("salut");

    // testConstInitNode();
    // testSetSiGetNode();
    // testConstructorCopiereNode();
    // testOperatorEgalNode();

    citireMemorareAfisareNObiecte();

    return 0;
}
