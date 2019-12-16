// Реализовать абстрактный тип данных "множество" на односвязном кольцевом списке

#include <iostream>
#include <cstdlib>
using namespace std;

struct node { // Структура "узел кольцевого списка"
	node(int v=0): val(v), next(0) {} // Конструктор
    void print() const { cout<<val<<" "; } // Вывод значения узла
	int val; // Значение узла (элемента множества)
	node* next;	 // Указатель на следующий элемент
};

class plur { // Класс "множество"
public:
	plur() { tail=0; } // Конструктор по умолчнию
	plur(const plur& p); // Копирующий конструктор
	~plur() { reset(tail); } // Деструктор
	plur operator + (const plur& p); // Оператор объединения множества и множества
	plur operator + (int n); // Оператор объединения множества и элемента
	friend plur operator + (int n, const plur& p); // Дружественная функция для объединения элемента и множества
	plur operator & (const plur& p); // Оператор пересечения множеств
	plur operator - (const plur& p); // Оператор разности множеств
	plur operator - (int n); // Оператор разности множества и элемента
	plur operator ^ (const plur& p); // Оператор симметричной разности двух множеств
	int operator < (const plur& p) const; // Оператор, проверяющий, является ли 1ое множество подмножеством 2го
	friend int operator < (int n,  const plur& p) {return (p.find_elem(p.tail, n) == 0) ? 0 : 1;} // Дружественная функция, проверяющая, содержит ли множество элемент
	int operator > (const plur& p) const; // Оператор, определяющий, является ли 1ое множество надмножеством 2го
	plur& operator -- (); // Оператор префиксной формы декремента (удаление 1-го элемента из множества)
	plur& operator -- (int); // Оператор постфиксной формы декремента (удаление последнего элемента множества)
	plur& operator = (const plur& p); // Оператор присваивания множества множеству
	plur& operator += (const plur& p); // Оператор добавления одному множеству другого
	plur& operator += (int n); // Оператор добавления множеству элемента
	plur& operator -= (const plur& p); // Оператор вычитания из одного множества другое
	plur& operator -= (int n); // Оператор вычитания элемента из множества
	plur& operator &= (const plur& p); // Оператор, присваивающий одному множеству результат его пересечения с другим множеством
	plur& operator ^= (const plur& p); // Оператор, присваивающий одному множеству результат симметричной разности с другим множеством
	int operator == (const plur& p) const; // Оператор, проверяющий, эквивалентны ли друг другу два множества
	int operator != (const plur& p) const; // Оператор, проверяющий, что два множества не эквивалентны друг другу
	operator int () const; // Оператор, возвращающий количество элементов в множестве (мощность множества)
	friend ostream& operator << (ostream& out, const plur& p); // Оператор вывода множества в поток
	
private:
	node* tail; // Указатель на хвост кольцевого списка
	ostream& print(ostream& out, node* const t) const; // Печать множества
	node* reset(node* t); // Сброс множества
	node* find_elem(node* const m, int n) const; // Функция, проверяющая, встречался ли переданное значение в списке, возвращает указатель на прерыдущий элемент
	node* add_elem(node* t, int v); // Добавление элемента в список
	node* add(node* t, int v); // Добавление очередного элемента в множество
	int count_elem(node* const a) const; // Подсчёт количества элементов в множестве
	node* union1(node* const a, node* const b); // Объединение множеств
	node* merge(node* a, node* b); // Слияние двух множеств, не имеющих общих элементов
	node* intersect(node* const a, node* const b); // Пересечение множеств
	node* subtract(node* const a, node* const b); // Разность множеств
	node* subtract_elem(node* const a, int v); // Функция разности множества и элемента
	node* sim_subtract(node* const a, node* const b); // Симметричная разность множеств
	int isSubset(node* const a, node* const b) const; // Проверка, является ли 1ое множество подмножеством 2го, возвращает кол-во эл-тов в а
	int isSuperset(node* const a, node* const b) const; // Проверка, является ли 1ое множество надмножеством 2го
	void delete_first(); // Удаление первого элемента
	void delete_last(); // Удаление последнего элемента
	node* assig(node* const b); // Копирование
	int isEq(node* const a, node* const b) const; // Проверка на эквивалентность
	int isNotEq(node* const a, node* const b) const; // Проверка на неэквивалентность
};

plur::plur(const plur& p){ // Копирующий конструктор
	tail=0;
	tail=assig(p.tail);
}

node* plur::reset(node* t){ // Сброс множества
	if (t!=0){ // Если список не пуст, происходит его удаление
		node* h=t->next;
		while(t!=h){ // Пока не удалятся все элементы, кроме одного
			node* head=t->next; // Голова списка
			t->next=head->next; // Хвост связывается с новой головой
			h=t->next;
			delete head; // Удаление несвязанного элемента
		}
		delete t;
		t=0;
	}
	return t;
}

ostream& print(ostream& out, node* const t){ // Печать множества
	if (t==0)
		cout<<"List is empty"<<endl;
	else {
		node* n=t->next; // Запоминаем голову 
		do {
			out<<n->val<<" ";
			n=n->next;
		} while (n!=t->next); // Пока не пройдёмся по всей очереди, выводим элементы по порядку
	}
	out<<endl;
	return out;
}

node* plur::add(node* t, int v){ // Добавление очередного элемента
	node* n=new node(v);
	if (!find_elem(t, n->val)){
		n->next=t->next; // Задаётся переход от нового хвоста к голове
		t->next=n; // К концу списка присоединяется последний элемент
		t=n; // Перемещается хвост
	}
	return t;
}

node* plur::add_elem(node* t, int v){ // Добавление элемента в список
	node* n=new node(v);
	if (t==0) { // Если список, к которому добавляется элемент, пуст:
		t=n; // Назначается хвост
		t->next=t; // В список помещается один элемент, ссылающийся сам на себя
		return t;
	}
	add(t, v); // Если список не пуст, вызывается функция добавления очередного элемента
	return t;
}

node* plur::find_elem(node* const m, int n) const { // Функция, проверяющая, встречался ли переданное значение n в списке m, возвращает предыдущий элемент
	if (m==0)
		return 0; // Если список пуст, то возвращается ложь
	node* tmp=m->next; // Временный указатель ссылается на голову списка
	node* pred=m; // Указатель на предыдущий элемент
	do {
		if (n==tmp->val)
			return pred;
		tmp=tmp->next;
		pred=pred->next;
	} while(tmp!=m->next); // Пока список не кончится, проверяем на совпадение каждый элемент
	return 0; // Если работа цикла не прервалась, значит, такое значение не встретилось
}

node* plur::union1(node* const a, node* const b){ // Объединение множеств
	node* u=0; // Хвост нового множества
	u=assig(a); // Копирование 1го множества
	if (b==0)
		return u;
	node* tmp=b->next; // Голова списка b
	do {
		node* n=tmp;
		tmp=tmp->next;
		if (!find_elem(a, n->val))
			u=add_elem(u, n->val);
	} while (tmp!=b->next); // В новый список добавляются все значения из b, не встречающиеся в а
	return u;
}

node* plur::merge(node* a, node* b){ // Слияние двух множеств, не имеющих общих элементов
	if (a==0)
		return b;
	if (b==0)
		return a; // Если одно множество пусто, то результатом слияния будет другое множество
	node* h=a->next; // Голова 1го списка
	node* h1=b->next; // Голова 2го списка
	a->next=h1; // Хвост 1го списка ссылается на голову 2го
	b->next=h; // Хвост 2го списка ссылается на голову 1го
	return b;
}

node* plur::intersect(node* const a, node* const b){ // Пересечение множеств
	node* i=0; // Хвост нового множества
	if (b==0)
		return i; // Если множество пустое, значит, результат будет также пустым множеством, и можно выйти из функции
	node* tmp=b->next; // Голова списка b
	do {
		if (find_elem(a, tmp->val))
			i=add_elem(i, tmp->val); 
		tmp=tmp->next;
	} while (tmp!=b->next); // Все значения, встречающиеся в обоих списках, заносятся в новый список
	return i;
}

node* plur::subtract(node* const a, node* const b){ // Разность множеств
	node* s=0; // Хвост нового множества
	if (a==0)
		return s; // Если множ-во, из которого вычитаем, пусто, тогда ничего не делаем
	node* tmp=a->next; // Голова списка а
	do {
		if (!find_elem(b, tmp->val))
			s=add_elem(s, tmp->val); // Если в b не встречается элемент из а, то этот элемент добавляется в новый список
		tmp=tmp->next;
	} while (tmp!=a->next);
	return s;
}

node* plur::subtract_elem(node* const a, int v){ // Функция разности множества и элемента
	node* s=0; // Хвост нового множества
	if (a==0)
		return s;
	s=assig(a);
	node* n=find_elem(s, v); // Поиск предшествующего элемента
	if (n) { // Если в множестве нашёлся тот элемент, который нужно удалить, то производится удаление:
		node* m=n->next; // Элемент, который нужно удалить 
		n->next=m->next; // Предыдущий элемент ссылается на следующий после того, который нужно удалить
		if (s==m)
			s=s->next; // Если удаляется хвост, нужно присвоить ему значение нового хвоста
		delete m;
	}
	return s;
}

node* plur::sim_subtract(node* const a, node* const b){ // Симметричная разность множеств
	node* c=0;
	c=merge(subtract(a, b), subtract(b, a));
	return c;
}

int plur::isSubset(node* const a, node* const b) const { // Проверка, является ли множество a подмножеством b
	int count=0;
	if (a==0)
		return 1; // Если проверяется пустое подмножество, сразу возвращается 1 без выполнения лишних действий
	node* tmp=a->next; // Голова списка а
	do {
		count++;
		if (!find_elem(b, tmp->val))
			return 0; // Если один из элементов не встречается в возможном "надмножестве", то сразу прерывается работа функции
		tmp=tmp->next;
	} while(tmp!=a->next);
	if (count > count_elem(b)) 
		return 0;
 	return count;
} // Возвращает кол-во эл-тов в а

int plur::isSuperset(node* const a, node* const b) const { // Проверка, является ли 1 множетво надмножеством 2го
	return isSubset(b, a);
}

int plur::count_elem(node* const a) const { // Подсчёт количества элементов в множестве
	if (a==0) return 0; // Если множество пустое, сразу возвращается 0
	int k=0;
	node* tmp=a->next; // Голова списка
	do {
		k++;
		tmp=tmp->next;
	} while (tmp!=a->next); // Считаются все элементы
	return k;
}

void plur::delete_first(){ // Удаление 1-го элемента
	if (tail!=0){
		if (tail->next==tail){
			delete tail;
			tail=0; // Если в списке один элемент, то он обнуляется 
		}
		else {
			node* tmp=tail->next; // Голова списка
			tail->next=tmp->next; // Последний элемент ссылается на 2-ой
			delete tmp;
		}
	}
}

void plur::delete_last(){ // Удаление последнего элемента
	if (tail!=0){
		if (tail->next==tail)
			tail=0; // Если в списке один элемент (хвост), то он обнуляется
		else {
			node* t=tail; // Хвост, который нужно удалить
			node* h=tail->next; // Голова
			do {
				h=h->next; 
			} while (h->next!=t); // Двигаемся по списку, пока не дойдём до предпоследнего элемента
			h->next=t->next; // Предпоследний элемент ссылается на голову
			tail=h; 
			delete t;
		}
	}
}

node* plur::assig(node* const b){ // Копирование
	node* n=0;
		if (b!=0) { // Проверка копируемого множества на пустоту
			node* h=b->next; // Голова копируемого множества
			n=add_elem(n, h->val); // Добавление 1го элемента
			h=h->next;
			while (h!=b->next){
				n=add(n, h->val);
				h=h->next;
			} // Пока не пройдём весь список, копируем оставшиеся элементы 
		}
	return n;
}

int plur::isEq(node* const a, node* const b) const { // Проверка на эквивалентность 
	if (a==b || (a==0 && b==0))
		return 1; // Если проверяется одно и то же множество или два пустых множ-ва, сразу возвращается 1
	int c=isSubset(a, b);
	if (c && c==count_elem(b)) // Если множ-вах одинаковое кол-во эл-тов и одно множество входит в другое, то они эквивалентны
		return 1;
	return 0;
}

int plur::isNotEq(node* const a, node* const b) const { // Проверка на неэквивалентность 
	return !isEq(a, b);
}

ostream& operator << (ostream& out, const plur& p){ // Вывод множества в поток
	return print(out, p.tail);
}

plur plur::operator + (const plur& p){ // Объединение множества и множества
	plur y; 
	y.tail=y.union1(tail, p.tail);
	return y;
}

plur plur::operator + (int n){ // Оператор объединения множества и элемента
	plur y;
	y.tail=y.add_elem(y.tail, n);
	return *this+y;
}
plur operator + (int n, const plur& p){ // Дружественная функция для объединения элемента и множества
	node m(n);
	m.next=&m;
	plur y;
	y.tail=y.union1(&m, p.tail);
	return y; 
}

plur plur::operator & (const plur& p){ // Пересечение множеств
	plur y;
	y.tail=y.intersect(tail, p.tail);
	return y; 
}

plur plur::operator - (const plur& p){ // Оператор разности множеств
	plur y;
	y.tail=y.subtract(tail, p.tail);
	return y;
}

plur plur::operator - (int n){ // Оператор разности множества и элемента
	plur y;
	y.tail=subtract_elem(tail, n);
	return y;
}

plur plur::operator ^ (const plur& p){ // Оператор симметричной разности двух множеств
	plur y;
	y.tail=sim_subtract(tail, p.tail);
	return y;
}

int plur::operator < (const plur& p) const { // Оператор, проверяющий, содержит ли одно множество другое множество
	return isSubset(tail, p.tail);
}
/*
int operator < (int n, const plur& p){ // Дружественная функция, проверяющая, содержит ли множество элемент
	return (find_elem(p.tail, n) == 0) ? 0 : 1;
}
*/
int plur::operator > (const plur& p) const { // Оператор, определяющий, является ли 1ое множество надмножеством 2го
	return isSuperset(tail, p.tail);
}

plur& plur::operator -- (){ // Оператор префиксной формы декремента (удаление 1-го элемента из множества)
	delete_first();
	return *this;
}

plur& plur::operator -- (int){ // Оператор постфиксной формы декремента (удаление последнего элемента множества)
	delete_last();
	return *this;
}

plur& plur::operator = (const plur& p){ // Оператор присваивания множества множеству
	if (tail==p.tail)
		return *this; // Если множество присваивается самому себе, никакие действия не производятся
	tail=assig(p.tail);
	return *this;
}

plur& plur::operator += (const plur& p){ // Оператор прибавления одному множеству другого 
	if (tail==p.tail)
		return *this;
	tail=union1(tail, p.tail);
	return *this;
}

plur& plur::operator += (int n){ // Оператор прибавления множеству элемента
	node m(n);
	m.next=&m;
	tail=union1(tail, &m);
	return *this;
}

plur& plur::operator -= (const plur& p){ // Оператор вычитания из одного множества другое
	if (tail==p.tail){
		reset(tail);
		return *this;
	}
	tail=subtract(tail, p.tail);
	return *this;
}

plur& plur::operator -= (int n){ // Оператор вычитания элемента из множества
	node m(n);
	m.next=&m;
	tail=subtract(tail, &m);
	return *this;
}

plur& plur::operator &= (const plur& p){ // Оператор, присваивающий одному множеству результат его пересечения с другим множеством
	if (tail==p.tail)
		return *this;
	tail=intersect(tail, p.tail);
	return *this;
}

plur& plur::operator ^= (const plur& p){ // Оператор, присваивающий одному множеству результат симметричной разности с другим множеством
	if (tail==p.tail){
		reset(tail);
		return *this;
	}
	tail=sim_subtract(tail, p.tail);
	return *this;
}

int plur::operator == (const plur& p) const { // Оператор, проверяющий, эквивалентны ли друг другу два множества
	return isEq(tail, p.tail);
}

int plur::operator != (const plur& p) const { // Оператор, проверяющий, что два множества не эквивалентны друг другу
	return isNotEq(tail, p.tail);
}

plur::operator int () const { // Оператор, возвращающий количество элементов в множестве
	return count_elem(tail);
}

int main(){
	plur a, b, c, d;
	a=a+0+1+2+3+4;
	cout<<a;
	b+=3;
	b=b+6+7;
	cout<<b;
	c=a-b;
	cout<<c;
	// Оператор += для множества и элемента
	/*
	cout<<"1) a+=0\na: "<<(a+=0)<<endl;
	// Оператор + для множества и элемента
	cout<<"2) a=a+1+2+3+4\na: "<<(a=a+1+2+3+4)<<endl;
	// Оператор + для элемента и множества
	cout<<"3) a=5+a\na: "<<(a=5+a)<<endl;
	// Оператор = для двух множеств
	cout<<"4) b=a\nb: "<<(b=a)<<endl;
	// Оператор -= для множества и элемента
	cout<<"5) b-=0\nb: "<<(b-=0)<<endl;
	cout<<"b-=2\nb: "<<(b-=2)<<endl;
	cout<<"b+=11 \nb: "<<(b+=11)<<endl;
	// Оператор &= для двух множеств (пересечение с присваиванием)
	cout<<"6) b&=a\nb: "<<(b&=a)<<endl;
	cout<<"b+=6 \nb: "<<(b+=6)<<endl;
	// Оператор + для двух множеств
	cout<<"7) c=a+b \nc: "<<(c=a+b)<<endl;
	// Оператор -= для двух множеств
	cout<<"8) c-=b \nc: "<<(c-=b)<<endl;
	cout<<"c+=9 \nc: "<<(c+=9)<<endl;
	// Оператор += для двух множеств
	cout<<"9) a+=c \na: "<<(a+=c)<<endl;
	// Оператор ^= для двух множеств (симметричная разность с присваиванием)
	cout<<"10) b^=a \nb: "<<(b^=a)<<endl;
	// Оператор - для двух множеств
	cout<<"11) d=a-b \nd: "<<(d=a-b)<<endl;
	// Оператор - для нможества и элемента
	cout<<"12) d=b-0 \nd: "<<(d=b-0)<<endl;
	// Оператор & для двух множеств (пересечение)
	cout<<"13) d=b&a \nd: "<<(d=b&a)<<endl;
	// Оператор ^ для двух множеств (симметричная разность)
	cout<<"14) d=b^a \nd: "<<(d=b^a)<<endl;
	// Оператор < для элемента и множества (проверка, содержится ли эл-т в множ-ве)
	cout<<"15) 6<d \nresult: "<<(6<d)<<"\n"<<endl;
	// Оператор < для двух множеств (является ли одно множество подмножеством второго)
	cout<<"16) b<d \nresult: "<<(b<d)<<"\n"<<endl;
	cout<<"d-=6 \nd: "<<(d-=6)<<endl;
	// Оператор > для двух множеств(является ли одно множество надмножеством второго)
	cout<<"17) a>d \nresult: "<<(a>d)<<"\n"<<endl;
	// Оператор -- в постфиксной форме (удаление последнего элемента)
	cout<<"18) a-- \na: "<<(a--)<<endl;
	// Оператор -- в префиксной форме (удаление 1го элемента)
	cout<<"19) --a \na: "<<(--a)<<endl;
	// Оператор == для двух множеств (эквивалентность)
	cout<<"20) a==d \nresult: "<<(a==d)<<"\n"<<endl;
	// Оператор != для двух множеств (неэквивалентность)
	cout<<"21) b!=c \nresult:"<<(b!=c)<<"\n"<<endl;
	// Оператор int для определения мощности множества (подсчёт кол-ва эл-тов)
	cout<<"22) int(a): "<<int(a)<<"\n"<<endl;
	plur e(a);
	cout<<"23) Copy constructor: plur e(a) \ne: "<<e<<endl;
	*/
	system("pause");
}
