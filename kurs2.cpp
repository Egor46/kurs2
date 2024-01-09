#include <fstream>
#include <iostream>

using namespace std;
int CURRENT_HEAP_SIZE = 10;
int NEW_BLOCK_SIZE = 50;

template<class T>
struct node {
	T info;
	int next, prev;
};

node<int>* heap; // change heap type here first

int stack_pointer;

template<class T>
void realloc() {
	int temp = CURRENT_HEAP_SIZE * 2;
	node<T>* newmem = new node<T>[temp];
	for (int i = 0; i < CURRENT_HEAP_SIZE; i++) {
		newmem[i] = heap[i];
	}
	for (int i = CURRENT_HEAP_SIZE; i < temp; i++) {
		newmem[i].info = 0;
		newmem[i].next = -1;
		newmem[i].prev = stack_pointer;
		stack_pointer = i;
	}
	delete[] heap;
	heap = newmem;
	CURRENT_HEAP_SIZE = temp;
}

template <class T>
int heap_new() {
	if (stack_pointer < 0) realloc<T>();
	int a = stack_pointer;
	stack_pointer = heap[stack_pointer].prev;
	heap[a].next = -1;
	heap[a].prev = -1;
	return a;
}

void heap_free(int idx) {
	heap[idx].next = -1;
	heap[idx].prev = stack_pointer;
	stack_pointer = idx;
}

template<class T>
void initialize_heap() {
	heap = new node<T>[CURRENT_HEAP_SIZE];
	for (int i = 0; i < CURRENT_HEAP_SIZE; i++) {
		heap[i].info = 0;
		heap[i].prev = i - 1;
		heap[i].next = -1;
	}
	stack_pointer = CURRENT_HEAP_SIZE - 1;
}

template<class T>
class Double_Linked_List {
protected:
	int head, cur;

	void deletelist() {
		cur = head;
		while (!isEnd()) {
			remove();
		}
		heap_free(cur);
	}

	void ifempty(T el) {
		head = heap_new<T>();
		cur = head;
		heap[head].info = el;
	}

	friend istream& operator>>(istream& in, Double_Linked_List<T>& a) {
		if (!isEmpty()) deletelist();
		while (!in.eof()) {
			T t;
			in >> t;
			a.addAfter(t);
			a.next();
		}
		a.cur = a.head;
		return in;
	}

	friend ostream& operator<<(ostream& out, Double_Linked_List<T>& a) {
		int t = a.head;
		while (t != -1) {
			out << heap[t].info << ' ';
			t = heap[t].next;
		}
		return out;
	}

public:

	Double_Linked_List() {
		head = cur = -1;
	}

	Double_Linked_List(int idx) {
		head = idx;
		cur = head;
	}

	Double_Linked_List(Double_Linked_List& a) {
		head = heap_new<T>();
		cur = head;
		int A = a.head;
		heap[cur].info = heap[a.cur].info;
		A = heap[A].next;
		while (A != -1) {
			heap[cur].next = heap_new<T>();
			heap[heap[cur].next].prev = cur;
			cur = heap[cur].next;
			heap[cur].info = heap[a.cur].info;
			A = heap[A].next;
		}
	}

	Double_Linked_List& operator=(Double_Linked_List<T>& a) {
		if (&a == this) return *this;
		if (a.isEmpty()) {
			head = -1;
			cur = -1;
			return *this;
		}
		if (!isEmpty()) deletelist();
		head = heap_new<T>();
		cur = head;
		int A = a.head;
		heap[cur].info = heap[A].info;
		A = heap[A].next;
		while (A != -1) {
			heap[cur].next = heap_new<T>();
			heap[heap[cur].next].prev = cur;
			cur = heap[cur].next;
			heap[cur].info = heap[a.cur].info;
			A = heap[A].next;
		}
		return *this;
	}

	~Double_Linked_List() {
		if (head != -1) cur = head; {
			while (!isEnd()) {
				int t = cur;
				cur = heap[cur].next;
				heap_free(t);
			}
			heap_free(cur);
			heap_free(head);
		}
	}

	T& get() {
		if (!isEmpty()) return heap[cur].info;
	}

	bool isEnd() const {
		return heap[cur].next == -1;
	}

	void next() {
		if (!isEnd()) cur = heap[cur].next;
	}

	void prev() {
		if (cur != head) cur = heap[cur].prev;
	}

	void begin() {
		cur = head;
	}

	bool isEmpty() {
		return head == -1;
	}

	void addAfter(T el) {
		if (head == -1) {
			ifempty(el);
			return;
		}
		if (!isEnd()) {
			int temp = heap_new<T>();
			heap[temp].info = el;
			heap[temp].next = heap[cur].next;
			heap[temp].prev = cur;
			heap[cur].next = temp;
			heap[heap[temp].next].prev = temp;
		}
		else {
			int temp = heap_new<T>();
			heap[cur].next = temp;
			heap[temp].prev = cur;
			heap[temp].info = el;
		}
	}

	void addBefore(T el) {
		if (head == -1) {
			ifempty(el);
			return;
		}
		if (cur == head) {
			head = heap_new<T>();
			heap[head].next = cur;
			heap[cur].prev = head;
			heap[head].info = el;
		}
		else {
			int temp = heap_new<T>();
			heap[temp].info = el;
			heap[temp].next = cur;
			heap[temp].prev = heap[cur].prev;
			heap[cur].prev = temp;
			heap[heap[temp].prev].next = temp;
		}
	}

	void remove() {
		if (head == -1) return;
		if (isEnd()) {
			if (cur == head) {
				head = -1;
				heap_free(cur);
				cur = head;
			}
			else {
				int temp = cur;
				cur = heap[cur].prev;
				heap[cur].next = -1;
				heap_free(temp);
			}
		}
		else {
			if (cur == head) {
				head = heap[head].next;
				heap[head].prev = -1;
				heap_free(cur);
				cur = head;
			}
			else {
				int temp = cur;
				cur = heap[cur].prev;
				heap[cur].next = heap[temp].next;
				heap[heap[temp].next].prev = cur;
				heap_free(temp);
			}
		}
	}

	bool find(T el) {
		int t = head;
		while (t != -1) {
			if (heap[t].info == el) return true;
			t = heap[t].next;
		}
		return false;
	}
};

template<class T>
class Sorted_DLL : public Double_Linked_List<T> {

	typedef Double_Linked_List<T> DLL_;

	friend istream& operator>>(istream& in, Sorted_DLL& a) {
		if (!a.isEmpty()) a.deletelist();
		while (!in.eof()) {
			T t;
			in >> t;
			a.add(t);
		}
		a.cur = a.head;
		return in;
	}

	friend ostream& operator<<(ostream& out, Sorted_DLL<T>& a) {
		int t = a.head;
		while (t != -1) {
			out << heap[t].info << ' ';
			t = heap[t].next;
		}
		return out;
	}

	void addBefore(T);
	void addAfter(T);
public:

	Sorted_DLL() : DLL_() {

	}

	Sorted_DLL(DLL_& other) {
		int t = other.head;
		while (t != -1) {
			add(heap[t].info);
			t = heap[t].next;
		}
	}

	Sorted_DLL(Sorted_DLL& other) {
		int t = other.head;
		while (t != -1) {
			add(heap[t].info);
			t = heap[t].next;
		}
	}

	Sorted_DLL& operator=(Sorted_DLL& other) {
		if (&other == this) return *this;
		if (other.isEmpty()) {
			DLL_::head = -1;
			DLL_::cur = -1;
			return *this;
		}
		if (!DLL_::isEmpty()) DLL_::deletelist();
		int t = other.head;
		while (t != -1) {
			add(heap[t].info);
			t = heap[t].next;
		}
		return *this;
	}

	void add(T el) {
		if (DLL_::isEmpty()) {
			DLL_::ifempty(el);
		}
		else {
			int temp = DLL_::cur;
			DLL_::begin();
			while (!DLL_::isEnd() && DLL_::get() < el) {
				DLL_::next();
			}
			if (!DLL_::isEnd()) {
				DLL_::addBefore(el);
			}
			else {
				if (DLL_::get() > el) DLL_::addBefore(el);
				else DLL_::addAfter(el);
			}
			DLL_::cur = temp;
		}
	}
};

void task1() {
	Sorted_DLL<int> l1, l2, l3, l4, l5, temp1, temp2;
	ifstream in1("in1.txt"), in2("in2.txt"), in3("in3.txt"), in4("in4.txt");
	in1 >> l1; in2 >> l2; in3 >> l3; in4 >> l4;
	cout << l1 << endl << l2 << endl << l3 << endl << l4 << endl;
	l1.begin(); l2.begin(); l3.begin(); l4.begin();
	{ // l1 U l2
		while (!l1.isEnd() && !l2.isEnd()) {
			if (l1.get() > l2.get()) {
				temp1.add(l2.get());
				l2.next();
			}
			else if (l1.get() < l2.get()) {
				temp1.add(l1.get());
				l1.next();
			}
			else {
				temp1.add(l1.get());
				l1.next();
			}
		}
		while (!l1.isEnd()) {
			temp1.add(l1.get());
			l1.next();
		}
		while (!l2.isEnd()) {
			temp1.add(l2.get());
			l2.next();
		}
		temp1.add(l1.get());
		temp1.add(l2.get());
	}
	l1.begin(); l2.begin();
	{ // l3 П l4
		while (!l3.isEnd() && !l4.isEnd()) {
			if (l3.get() > l4.get()) {
				l4.next();
			}
			else if (l3.get() < l4.get()) {
				l3.next();
			}
			else {
				temp2.add(l3.get()); l3.next(); l4.next();
			}
		}
		while (!l3.isEnd()) {
			if (l4.get() == l3.get()) {
				temp2.add(l3.get()); l3.next();
			}
		}
		while (!l4.isEnd()) {
			if (l4.get() == l3.get()) {
				temp2.add(l4.get()); l4.next();
			}
		}
		if (l3.get() == l4.get()) temp2.add(l3.get());
	}
	temp1.begin(); temp2.begin();
	{ // temp1 - temp2
		do {
			if (temp1.get() < temp2.get()) {
				l5.add(temp1.get());
				temp1.next();
			}
			else if (temp1.get() > temp2.get()) {
				temp2.next();
			}
			else {
				temp1.next();
			}
		} while (!temp1.isEnd() && !temp2.isEnd());
		while (!temp1.isEnd()) {
			l5.add(temp1.get());
			temp1.next();
		}
	}
	cout << l5;
}


int main() {
	initialize_heap<int>();
	task1();
	delete[] heap;
	return 0;
}