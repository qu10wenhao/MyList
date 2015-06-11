/*
 * 实现一个list类命名为MyList，其功能与python数组类似。可以用数组实现，也可以用链表实现。
 * 必须使用template <class T>，不允许使用<iostream>以外的库。
 * 需要实现的功能与测试代码见下。请务必搞清楚每一项的意义，比如什么时候需要用友元函数什么时候需要const什么时候需要&。
 * 当某些由用户正常操作可能产生的run time error（如pop一个空数组，或访问一个超出下标的元素），
 * 请使用try-catch-throw处理错误。
 * 以下类定义不保证完全正确，请根据实际需求修改，以实现样例功能为主。
 */

/* 
 * 本练习不涉及到虚类和类继承，但是期末考试我就不知道了。如果有时间可以用书上的shape类来练习。
 */

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
	void double_space();//数组大小不够的时候将数组大小翻倍的操作。
public:
	MyList(){
		pos = 0;
		size = 0;
		a = new T [size];
	}
	MyList(int num, const T &item);//将item重复num次填入数组中。
	MyList(const MyList<T> &l);//深复制另外一个MyList。
    MyList(T* arr, int len);//以arr的前len个元素构造数组

	void push(const T &item);//将item添加在MyList最后。
	T pop() throw(pop_error);//将MyList中最后一个元素删除，并返回这个删除的元素。
	void insert(int index, const T &item) throw(cross_error);//将item插入到place处。
	void clean();//清空数组。
	int get_size();//返回MyList中元素的数量。
	void erase(int start, int end) throw(cross_error); //删除MyList中第start到第end位的元素，包括两边。
	T get_item(int index) throw(cross_error);//返回第index个元素。
	MyList get_item(int start, int end) throw(cross_error);//返回MyList中第start到第end位的元素，包括两边。此处需要像python一样接受负数，具体见测试代码。
	int count(const T &item);//返回MyList中和item相等的元素的个数。
	void remove(const T &item);//删除MyList中第一个和item相等的元素。


	friend MyList<T> operator + <T>(const MyList<T> &l1, const MyList<T> &l2); //合并两个MyList
    friend MyList<T> operator + <T>(const MyList<T> &l1, const T &item); //同push(T item)，但不修改l1，返回一个新数组
	MyList<T> &operator = (const MyList<T> &l);//赋值
	MyList<T> &operator += (const T &item);//同push(T item)
	MyList<T> &operator += (const MyList<T> &l);//将一个MyList加入到本个MyList之后。
	T &operator [](int index);//返回第index个元素。
	friend std::ostream & operator<< <T>(std::ostream &os, const MyList<T> &obj);//如果T是可以直接cout的类的话（比如int），按Python数组的格式输出MyList中的每一个元素，例如：
    // [1, 2, 5, 4, 1]

	void sort(bool less=true);//实现一个快速排序或归并排序，对支持比较运算符（>=<）的类进行排序。
    // 若less=true，按照从小到大排列，否则按从大到小排列
	void reverse();//将MyList的元素倒过来。

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
    b = a.get_item(4, -3); // b = [] *若start > end，返回空数组
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
MyList<T>::MyList(int num, const T &item){//将item重复num次填入数组中。
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
