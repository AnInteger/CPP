#include<iostream>
using namespace std;

class A {
public:
	void test() {
		cout << "A::test()" << endl;
	}

	A() {
		cout << "A() call" << endl;
	}

	~A() {
		cout << "~A() call" << endl;
	}
};

/*
template <class T>
class SmartPtr {
public:

	SmartPtr() {
		objptr = nullptr;
		RefCount = 0;
	}

	SmartPtr(T* ptr) {
		objptr = ptr;
		RefCount = 1;
	}
	~SmartPtr()
	{
		this->Release();
	}

	T* operator->() {
		return objptr;
	}

	T& operator*() {
		return *objptr;
	}

	operator bool() {
		return objptr != nullptr;
	}
	//1、
	//SmartPtr operator=(SmartPtr& a) = delete;
	//SmartPtr (SmartPtr& a) = delete;

	//2、
	//参照了移动语义的做法，当类的资源比较昂贵，不易申请时，直接修改指针的做法会更有效率。
	SmartPtr& operator=(SmartPtr& a) {
		if (a.objptr) {
			objptr = a.objptr;
			a.objptr = nullptr;
		}
		return *this;
	}

	SmartPtr(SmartPtr& a) {
		if (a.objptr) {
			objptr = a.objptr;
			a.objptr = nullptr;
		}
	}

private:

	void AddRef() {
		RefCount++;
	}

	void Release() {
		if (--RefCount == 0) {
			delete objptr;
			objptr = nullptr;
		}
	}

	T* objptr;
	int RefCount;
};
*/

template <typename T>
class SmartPtr;

template <typename T>
class RefCount {
	friend class SmartPtr<T>;
public:
	RefCount(T* a) {
		_mobj = a;
		_mCount = 1;
	}

	~RefCount() {
		delete _mobj;
		_mobj = nullptr;
		_mCount = 0;
	}

	void AddRef() {
		_mCount++;
	}

	void Release() {
		if (--_mCount == 0) {
			delete this;
		}
	}

private:
	T* _mobj;
	int _mCount;
};

template <typename T>
class SmartPtr {
private:
	RefCount<T>* _mref;
public:

	SmartPtr() {
		_mref = nullptr;
	}

	SmartPtr(T* ptr) {
		_mref = new RefCount<T>(ptr);
	}

	~SmartPtr()
	{
		_mref->Release();
	}

	T* operator->() {
		return _mref->_mobj;
	}

	T& operator*() {
		return *_mref->_mobj;
	}

	//operator bool() {
	//	return _mref != nullptr;
	//}

	SmartPtr<T>& operator=(SmartPtr& a) {
		if (_mref == a._mref) {
			return *this;
		}

		if (_mref != nullptr) {
			_mref->Release();
		}

		_mref = a._mref;
		_mref->AddRef();

		return *this;

	}
	SmartPtr(SmartPtr& a) = delete;
};

int main() {

	SmartPtr<A> sp(new A());

	sp->test();
	(*sp).test();

	SmartPtr<A> sp1;
	sp1 = sp;

	sp1->test();
}
