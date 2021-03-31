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

class SmartPtr {
public:
	SmartPtr(A* ptr) {
		objptr = ptr;
	}
	~SmartPtr()
	{
		if (objptr != nullptr) {
			delete objptr;
			objptr = nullptr;
		}
	}

	A* operator->() {
		return objptr;
	}

	A& operator*() {
		return *objptr;
	}

	operator bool() {
		return objptr != nullptr;
	}

	//SmartPtr operator=(SmartPtr& a) = delete;
	//SmartPtr (SmartPtr& a) = delete;

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
	A* objptr;
};

int main() {
	SmartPtr sp(new A());
	sp->test();
	(*sp).test();

	SmartPtr sp2 = sp;

	if (sp) {
		cout << "sp is NULL" << endl;
	}

	return 0;
}