
#include<iostream>

class pop_error{
	public:
		const char* msg();
};

const char* pop_error::msg(){
	return "Error: you are trying to pop an empty list!";
}

class cross_error{
	public:
		const char* msg();
};

const char* cross_error::msg(){
	return "Error: you've crossed the boundary!";
}
template<class T> class MyList;

template<class T>
MyList<T> operator + (const MyList<T> &l1, const MyList<T> &l2);

template<class T>
MyList<T> operator + (const MyList<T> &l1, const T &item);

template<class T>
std::ostream & operator << (std::ostream &os, const MyList<T> &obj);

template<class T>
class MyList{
private:
	T *a;
	int size;
	int pos;
	void double_space();
public:
	MyList(){
		pos = 0;
		size = 0;
		a = new T [size];
	}
	MyList(int num, const T &item);
	MyList(const MyList<T> &l);
    MyList(T* arr, int len);

	void push(const T &item);
	T pop() throw(pop_error);
	void insert(int index, const T &item) throw(cross_error);
	void clean();
	int get_size();
	void erase(int start, int end) throw(cross_error);
	T get_item(int index) throw(cross_error);
	MyList get_item(int start, int end) throw(cross_error);
	int count(const T &item);
	void remove(const T &item);


	friend MyList<T> operator + <T>(const MyList<T> &l1, const MyList<T> &l2);
    friend MyList<T> operator + <T>(const MyList<T> &l1, const T &item);
	MyList<T> &operator = (const MyList<T> &l);
	MyList<T> &operator += (const T &item);
	MyList<T> &operator += (const MyList<T> &l);
	T &operator [](int index);
	friend std::ostream & operator<< <T>(std::ostream &os, const MyList<T> &obj);

	void sort(bool less=true);
	void reverse();

	~MyList(){delete [] a;}
};
template<class T>
void mysort(T* &a, int start, int end){
	if(start >= end) return;
	int i=start,j=end,temp = start;
	T middle;
	while(j>i){
		while(a[j]>a[temp] && j>start) j--;
		if(j!= start){
			middle = a[j];
			a[j] = a[temp];
			a[temp] = middle;
			temp = j;
		}
		while(a[i]<a[temp] && i<end) i++;
		if(i!=end){
			middle = a[i];
			a[i] = a[temp];
			a[i] = middle;
			temp = i;
		}
	}
	mysort(a,start,temp-1);
	mysort(a,temp+1,end);
}

using namespace std;
int main(){
	MyList<int> a, b;
	int i;
	for (i=0; i<5; ++i)
		a.push(i);
    // a = [0, 1, 2, 3, 4]
	a[3] = 15; // a = [0, 1, 2, 15, 4]
	a.sort(); // a = [0, 1, 2, 4, 15]
	a.reverse(); // a = [15, 4, 2, 1, 0]
	a += 12; // a = [15, 4, 2, 1, 0, 12]
	for (i=0; i<a.get_size(); ++i)
		cout<<a[i]<<endl;
    b = a.get_item(4, -3); // b = []
	b = a.get_item(3, -1); // b = [1, 0, 12] 
	a += b; // a = [15, 4, 2, 1, 0, 12, 1, 0, 12]
	for (i=0; i<a.get_size(); ++i)
		cout<<a.get_item(i)<<endl;
	cout<<a.count(5)<<endl;
	b.clean(); // b = []
	cout<<b.get_size()<<endl;
	a.erase(2, 6); // a = [15, 4, 0, 12]
	b = a + a; // b = [15, 4, 0, 12, 15, 4, 0, 12]
	b.insert(3, 116); // b = [15, 4, 0, 116, 12, 15, 4, 0, 12]
	b.remove(4); // b = [15, 0, 116, ...]
	cout<<b<<endl;
	MyList<double> c(10, 3.14);
	for (i=0; i<100; ++i)
		c.push(1.1*i);
	cout<<c.get_item(100, 105)<<endl;
	
	return 0;
}
template<class T>
void MyList<T>::double_space(){
	T *b;
	if(size == 0) size = 1;
	size *= 2;
	b = new T [size];
	for(int i=0;i<pos;i++) b[i] = a[i];
	delete [] a;
	a = b;
}

template<class T>
MyList<T>::MyList(int num, const T &item){
	size = num;
	pos = 0;
	a = new T [size];
	for(int i=0;i<num;++i) a[pos++] = item; 
}

template<class T>
MyList<T>::MyList(const MyList<T> &l){
	size = l.size;
	pos = l.pos;
	a = new T [size];
	for(int i=0;i<pos;++i) a[i] = l.a[i];
}

template<class T>
MyList<T>::MyList(T* arr, int len){
	size = len;
	pos = len;
	a = new T [size];
	for(int i=0;i<len;++i) a[i] = arr[i];
}

template<class T>
void MyList<T>::push(const T &item){
	if(pos == size) double_space();
	a[pos++] = item;
}

template<class T>
T MyList<T>::pop() throw(pop_error){
	if(pos == 0) throw pop_error();
	T temp;
	temp = a[--pos];
	a[pos].~T();
	return temp;
}

template<class T>
void MyList<T>::insert(int index, const T &item) throw(cross_error){
	if(index > pos) throw cross_error();
	push(item);
	for(int i=pos-1;i>index;--i){
		a[i] = a[i-1];
	}
	a[index] = item;
}

template<class T>	
void MyList<T>::clean(){
	while(pos != 0) a[--pos].~T();
}

template<class T>
int MyList<T>::get_size(){
	return pos;
}

template<class T>
void MyList<T>::erase(int start, int end) throw(cross_error){
	if(end<0) end += pos;
	if(start<0) start += pos;
	if(start < 0 || end < start || end >= pos) throw cross_error();
	int middle = pos;
	pos = start;
	while(pos+end-start+1 < middle) a[pos++] = a[pos+end-start];
	for(int i=pos;i<middle;i++) a[i].~T();
}

template<class T>
T MyList<T>::get_item(int index) throw(cross_error){
	if(index < 0) index += pos;
	if(index >= pos || index < 0) throw cross_error();
	return a[index];
}

template<class T>
MyList<T> MyList<T>::get_item(int start, int end) throw(cross_error){
	if(start < 0) start += pos;
	if(end < 0) end += pos;
	if(start < 0 || end >= pos) throw cross_error();
	MyList<T> x;
	if(end < start) return x;
	MyList<T> y(a+start, end-start+1);
	return y;
}

template<class T>
int MyList<T>::count(const T &item){
	int count = 0;
	for(int i=0;i<pos;++i){
		if(a[i] == item) count++;
	}
	return count;
}

template<class T>
void MyList<T>::remove(const T &item){
	int i;
	for(i=0;i<pos;++i){
		if(a[i] == item) break;
	}
	if(i != pos){
		for(int j=i;j<pos-1;++j) a[j] = a[j+1];
		a[--pos].~T();
	}
}

template<class T>
MyList<T> operator + (const MyList<T> &l1, const MyList<T> &l2){
	MyList<T> x(l1);
	for(int i=0;i<l2.pos;++i) x.push(l2.a[i]);
	return x; 
}

template<class T>
MyList<T> operator + (const MyList<T> &l1, const T &item){
	MyList<T> x(l1);
	x.push(item);
	return x;
}

template<class T>
MyList<T>& MyList<T>::operator = (const MyList<T> &l){
	size = l.size;
	pos = l.pos;
	delete [] a;
	a = new T [size];
	for(int i=0;i<pos;++i) a[i] = l.a[i];
	return *this;
}

template<class T>
MyList<T>& MyList<T>::operator += (const T &item){
	this->push(item);
	return *this;
}

template<class T>
MyList<T>& MyList<T>::operator += (const MyList<T> &l){
	*this = *this + l;
	return *this;
}

template<class T>
T& MyList<T>::operator [](int index){
	return (this->a)[index];
}

template<class T>
std::ostream & operator<<(std::ostream &os, const MyList<T> &obj){
	os << '[';
	int i=0;
	if(obj.pos == 0){
		os << ']';
		return os;
	}
	for(i=0;i<obj.pos-1;++i) os << obj.a[i] << ", ";
	os << obj.a[i] << ']';
	return os;
}

template<class T>
void MyList<T>::sort(bool less){
	mysort(a, 0, pos-1);
	if(!less) reverse();
}

template<class T>
void MyList<T>::reverse(){
	int i=0,j=pos-1;
	T middle;
	while(j>i){
		middle = a[i];
		a[i] = a[j];
		a[j] = middle;
		j--;
		i++;
	}
}
