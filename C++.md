本文着重介绍C++相较于C的新特性和语法

# 一、面向对象(OOP)

> 面向对象编程是一种编程范式,适用于所有高级语言,相关概念同样适用于其他高级语言;
>
> 面向对象编程提供了对外界事物更好的抽象和模块化,使代码更易于理解



## 1.1 类与对象

- 类描述了对象的数据 ( 成员变量 ) 和 操作 ( 成员函数 ) , 是定义对象的蓝图或者模板 ;
- 对象是根据类创建的实例 , 是类的具体表示 , 每个对象都有其自己的成员变量和成员函数 。

1. 定义类:

```c++
class ClassName{
int Var;//成员变量
void Function(){//函数体
}//成员函数
}
```

2. 创建类的对象

```
MyClass ClassName;  // 创建一个 MyClass 类型的对象
```

3. 构造函数(Constructor)

   > 特殊的函数，在 **对象创建时自动调用**，用于初始化

   - 与类同名
   - 没有返回值
   - 支持重载(多个构造函数),在创建对象的时候编译器会自动根据传参的类型选择合适的构造函数

   ```c++
   class Student{
       std::string name;
       int score;
       Student(){name="未命名";score=0;}//默认构造函数
       Student(std::string n,int s):name(n),score(s){}//带参构造
       //:成员变量(参数)是一种快速赋值的方式,上面等价于{name=n;score=s;}    
   }
   ```

4. 析构函数(Destructor)

   > 在对象销毁时自动调用，用于 **释放资源**（比如关闭文件、释放堆内存）。

   - 结构:`~类名()`,既在类名前面加'~'

   - 无返回值

   - 无参数

   - 一个类只能有一个析构函数

     ```c++
     class File {
         FILE* fp;
     public:
         File(const char* name) {
             fp = fopen(name, "w");
             std::cout << "打开文件" << std::endl;
         }
         ~File() {
             fclose(fp);
             std::cout << "关闭文件" << std::endl;
         }
     };
     
     ```

     

5. 拷贝构造函数(Copy Constructor)

   > 用一个对象初始化另一个对象时调用

   - 默认拷贝构造函数:按成员逐个复制(浅拷贝)
   - 如果对象里有指针，可能会导致 **多个对象共享同一块内存**，引发 **悬垂指针 **(裸指针),通过设计拷贝函数,实现特定的拷贝功能。
   - 本质是一种特殊的***构造函数***(用已有对象来构造新对象)

   ```C++
   class Student {
       char* name;
   public:
       Student(const char* n) {
           name = new char[strlen(n)+1];
           strcpy(name, n);
       }
   
       // 自定义拷贝构造（深拷贝）
       Student(const Student& other) {
           name = new char[strlen(other.name)+1];
           strcpy(name, other.name);
       }
   
       ~Student() { delete[] name; }
   };
   int main(){
       Student Alixe("Alixe");//调用默认的构造函数(Student(const char* n)
       Student Alixe1=Alixe;//拷贝构造(这里虽然使用了'=',但其实是对声明变量的初始化,而不是赋值,实际上也是在调用构造函数,等价于下面的构造方式,而非对运算符的重载)
       Student Alixe3(Alixe);//拷贝构造
   }
   
   ```

   

6. 赋值运算符重载

   > 和拷贝构造类似,不同的是
   >
   > 拷贝构造:创建对象时调用
   >
   > 赋值运算:对象已经存在,再赋值时调用
   >
   > 有关运算符重载的内容→→→[运算符重载](#五、运算符重载)

   ```cpp
   class Student {
       char* name;
   public:
       Student(const char* n) {
           name = new char[strlen(n)+1];
           strcpy(name, n);
       }
   
       // 深拷贝赋值运算符
       Student& operator=(const Student& other) {
           if(this == &other) return *this; // 防止自赋值
           delete[] name; // 释放原有资源
           name = new char[strlen(other.name)+1];
           strcpy(name, other.name);
           return *this;
       }
   
       ~Student() { delete[] name; }//释放内存
   };
   
   int main(){
       Student A("Alicx");
       Student B("Bob");
       //当对象以及存在,再把另一个对象的值赋值给它时,调用的是数值运算符=,而不是拷贝构造
       B=A;//将A的名字赋值给B
       
   }
   
   ```

   

7. 成员函数分类

   - **普通成员函数**：能修改对象属性。

   - **const 成员函数**：承诺不修改对象属性。有关`const`详见 "[关键字](#八、关键字) "对应内容

     ```cpp
     class Test {
         int x;
     public:
         void setX(int v) { x = v; }        // 普通函数
         int getX() const { return x; }     // const成员函数
     };
     
     ```

     

8. 静态成员

- **静态成员变量**：属于整个类，不属于某个对象。所有对象共享一份。

- **静态成员函数**：不依赖对象，可以用 `类名::函数()` 调用。

```cpp
class Counter {
    static int count;  // 静态成员变量
public:
    Counter() { count++; }
    static int getCount() { return count; }
};
int Counter::count = 0; // 类外初始化

int main() {
    Counter c1, c2, c3;
    std::cout << Counter::getCount() << std::endl; // 输出 3
}

```

---





## 1.2对象的封装(**Encapsulation**)

> 将数据和操作封装在类中 , 可以将类内部的实现细节隐藏 , 只暴露有限的接口与外部进行交互 , 从而达到保护类对象的内部状态不被外部随意修改。

1. ***基本语法***

   `class 类名{ 访问权限： 属性 / 行为 };`

   ```c++
   class Circle{
       int r;
       static double Pi=3.14;
       Circle(int length):r(length){}
       double cal_perimeter(){return 2*r*Pi;}
   }
   int main(){
       Circle c1(5);
       std::cout<<"c1的周长为:"<<c1.cal_perimeter;
   }
   ```

   2.***访问权限***

   > 类在设计时，可以把属性和行为放在不同的权限下，加以控制
   >
   > protected和private的主要区别,在于[继承](#1.5继承(Inheritance))

   - 访问权限有3种(如果不写访问权限默认为private,结构体中默认的访问权限为public):
   
     | 访问权限    | 类内(类中的函数,友元函数)访问 | 派生类(子类)访问 | 类外(类的对象)访问 |
     | ----------- | ----------------------------- | ---------------- | ------------------ |
     | `public`    | ✅                             | ✅                | ✅                  |
     | `protected` | ✅                             | ✅                | ❌                  |
     | `private`   | ✅                             | ❌                | ❌                  |
   
   ``` cpp
   #include <iostream>
   using namespace std;
   
   class Person {
   public:
       string name;        // 公有成员：任何地方都能访问
   protected:
       int age;            // 受保护成员：仅类内和子类能访问
   private:
       string password;    // 私有成员：仅类内能访问
   
   public:
       Person(string n, int a, string p)
           : name(n), age(a), password(p) {}
   
       void show() {
           cout << "Name: " << name << ", Age: " << age << endl;
       }
   };
   
   class Student:public Person{
   public:
       void showInfo(){
           cout << name;   // ✅ 可以访问 public
           cout << age;    // ✅ 可以访问 protected
           // cout << password; ❌ 不能访问 private
       }
   }
   
   int main(){
       Person p("Buer",21,"123456");
       Student s("Bob",22,"123456")
       std::cout<<p.name<<std::endl;//✅name是公有成员,可以直接访问
       //std::cout<<p.age<<' '<<p.password<<std::endl;//❌在类外,age和password均无法访问
   	p.show()//show是公有方法可以直接访问,show方法是类内访问,可以访问age和password
       //s.show()//❌不能访问父类的private,但是能访问父类的protected
       
   }
   ```
   
   ---







## 1.3 友元(Friend)

   > 友元函数和友元类统称为友元,此外还有友元成员函数
   >
   > C++类中有三种访问权限,保护类中数据的封闭性
   >
   > 但是，有时需要定义一些函数，这些函数不是类的一部分，但又需要频繁访问类的私有（private）成员和保护（protected）成员，这时可以将这些函数定义为友元函数。
   >
   > **友元（friend）** 是一种允许**非成员函数**或**其他类**访问类的私有成员的机制。
   >
   > 友元函数常用于[运算符重载](#五、运算符重载)

1. 友元的形式

| 类型             | 定义位置            | 作用对象         | 示例                                             |
| ---------------- | ------------------- | ---------------- | ------------------------------------------------ |
| **友元函数**     | 类内声明,类外实现   | 单个函数         | `friend void show(Person p);`                    |
| **友元类**       | 类内指定,友元类访问 | 整个类           | `friend class Student;`                          |
| **友元成员函数** | 类内声明            | 其他类的某个函数 | `friend void School::registerStudent(Person p);` |

2. ***友元函数***

友元函数可以**直接访问类**的私有成员或保护成员，它是定义在类外的普通函数，它不属于任何类，但需要在类的定义中加以声明。

如不在类内声明friend void showPerson(Person p);则showPerson就是个普通函数

```c++
#include <iostream>
using namespace std;

class Person {
private:
    string name;
    int age;

public:
    Person(string n, int a): name(n), age(a) {}

    // 在类中声明友元函数
    friend void showPerson(Person p);
};

// 友元函数定义（注意它不是成员函数）
//在类外实现友元函数
void showPerson(Person p) {
    cout << "Name: " << p.name << ", Age: " << p.age << endl;
}

int main() {
    Person p("Buer", 21);
    showPerson(p); // ✅ 直接访问私有成员
}

```

3. ***友元类***

   友元类的所有成员函数都是另一个类的友元函数，都可以访问另一个类中的保护成员和私有成员。

   ```cpp
   class Engine;
   
   class Car {
   private:
       int power = 500;
       friend class Engine;  // 声明 Engine 为友元类,可以访问power
   };
   
   class Engine {
   public:
       void tune(Car& c) {
           c.power += 100;  // ✅ 可以访问私有成员
       }
   };
   
   ```

   

4. ***友元成员函数***

   只授权**另一个类的特定成员函数**访问自己(可以看成一种特殊的友元函数)

   ```cpp
   class B;  // 提前声明
   
   class A {
   private:
       int secret = 42;
       friend void B::reveal(A&);  // 只让B的reveal函数访问
   };
   
   class B {
   public:
       void reveal(A& a) {
           cout << "Secret is: " << a.secret << endl;
       }
   };
   
   ```

   5. 友元的优缺点

      利用 friend 修饰符，可以让一些普通函数 或 另一个类的成员函数 直接对某个类的保护成员和私有成员进行操作，提高了程序的运行效率；同时避免把类的成员都声明为public，最大限度地保护数据成员的安全。

      但是，即使是最大限度地保护数据成员，友元也破坏了类的封装性。

      如果将类的封装比喻成一堵墙的话，那么友元机制就像墙上开了一个门。所以使用友元时一定要慎重。

---










## 1.4对象的生命周期

> 在C++中，对象的生命周期是指对象存在的时间段，从对象创建到对象销毁的整个过程。正确地管理对象的生命周期是编写高效、可靠C++代码的关键之一



1. ***静态分配***

   - 静态分配是在编译时为对象分配内存的过程。

   - 静态分配的对象通常被声明为全局变量或静态变量(static局部变量,static成员)。

   - 在程序启动时被创建(函数内`static` 在第一次执行到定义处初始化)→→→→→→→→→→程序结束时销毁。

   ```cpp
    class MyClass {
    public:
        MyClass() { std::cout << "MyClass created" << std::endl; }
        ~MyClass() { std::cout << "MyClass destroyed" << std::endl; }
    };
    MyClass myStaticObject;//全局变量
    int main() {
        std::cout << "Main function" << std::endl;
        return 0;
    }
   ```

   

2. ***自动分配***

   - 自动分配是在函数内部为对象分配内存的过程。

   - 自动分配的对象通常被声明为函数局部变量。

   - 自动分配的对象在函数调用时被创建，在函数调用结束时被销毁。

     ```c++
     void function(){
         int temp=0;
         temp+=1;
         print(temp);
     }
     int main(){
         function();//temp在函数运行结束时被销毁
         function();
     }
     ```

     

3. ***动态分配***

   - 动态分配是在程序运行时为对象分配内存的过程。

   - 动态分配的对象在程序运行时被创建，直到程序显式地销毁它们为止。

   - 动态分配的对象通常被声明为指针，使用`new`运算符进行分配,使用`delete`销毁

     ```cpp
      void myFunction() {
          MyClass* myDynamicObject = new MyClass();
          std::cout << "Function body" << std::endl;
          delete myDynamicObject;
      }
      int main() {
          myFunction();
          std::cout << "Main function" << std::endl;
          return 0;
      }
     ```

4. [继承](#1.5继承(Inheritance))对象的声明周期

   构造时：**先基类后派生类**
   析构时：**先派生类后基类**

   ```cpp
   class Base {
   public:
       Base() { cout << "构造Base" << endl; }
       ~Base() { cout << "析构Base" << endl; }
   };
   
   class Derived : public Base {
   public:
       Derived() { cout << "构造Derived" << endl; }
       ~Derived() { cout << "析构Derived" << endl; }
   };
   
   int main() {
       Derived d;
   }
   
   ```

   输出:

   ```
   构造Base
   构造Derived
   析构Derived
   析构Base
   ```

   

5. 对象生命周期的管理

- 确保在构造函数中创建的资源在析构函数中合理释放。
- 避免裸指针,推荐使用只能指针更方便的管理对象的生命周期。
- 使用应用传递避免对对象的拷贝,减少资源占用,并确保对象在函数调用结束的时候得到正确的销毁。



---





## 1.5继承(Inheritance)

> 一个类（子类 / 派生类）从另一个类（父类 / 基类）那里获得成员变量和函数的过程。
>
> 子类还可以添加新的属性和方法以实现更强大的功能
>
> 子类还可以重写父类中的方法以实现不同的行为 ; 
>
> 通过继承，可以构建一个 分层的类层次结构 , 从而促进代码的重用和扩展 。

1. 语法结构和定义

   ``` c++
   class 派生类名 : 继承方式 基类名 {
       // 新增的成员
   };
   ```

   | 继承方式    | 基类的 public 成员在子类中变成 | protected 成员在子类中变成 | private 成员是否继承 |
   | ----------- | ------------------------------ | -------------------------- | -------------------- |
   | `public`    | public                         | protected                  | ❌ 不继承（但存在）   |
   | `protected` | protected                      | protected                  | ❌                    |
   | `private`   | private                        | private                    | ❌                    |

​	通常情况下不使用protected和private继承，而是采用public来继承

 2. 不可继承的方法

    派生类可以继承所有基类的方法，但以下三种情况除外：

    - 基类的构造函数、析构函数和拷贝构造函数：这些是C++中的特殊成员函数，用于初始化对象、销毁对象和复制对象。派生类可以直接调用这些函数，但不能重载它们。如果需要修改或扩展这些函数的行为，可以在派生类中重新定义它们。
    - 基类的重载运算符：C++中的运算符重载允许用户自定义对象的操作行为。派生类可以重载基类的运算符，以实现特定的功能。但是，如果派生类没有提供与基类相同的运算符重载，那么基类的运算符将不可用。
    - 基类的友元函数：友元函数是一种特殊的成员函数，它可以访问类的私有和保护成员。派生类可以声明友元函数，以便在派生类的对象之间共享数据。但是，如果派生类没有提供与基类相同的友元函数声明，那么派生类的友元函数将不可用。要在派生类使用基类中声明友元函数,则必须在派生类中再声明一次该友元函数

​		此外,如果基类有***带参***构造函数，子类必须显式调用它：

 ```cpp
 class Animal {
 public:
     Animal(string n) { cout << "Animal构造：" << n << endl; }
 };
 
 class Dog : public Animal {
 public:
     Dog(string n) : Animal(n) { // 调用基类构造
         cout << "Dog构造完毕！" << endl;
     }
 };
 
 int main() {
     Dog d("旺财");
 }
 /*
 输出:
 Animal构造：旺财
 Dog构造完毕！
 */
 
 ```

3. 多继承

   > 从多个直接基类中产生派生类的能力，多继承的派生类继承了所有父类的成员。尽管概念上非常简单，但是多个基类的相互交织可能会带来错综复杂的设计问题，命名冲突就是不可回避的一个。

多继承时很容易产生命名冲突,比如菱形继承(A、B、C、D的继承关系呈现菱形):

```cpp
#include <iostream>
using namespace std;

// 基类
class A {
public:
    int a;
    A() { cout << "A构造函数\n"; }
    void show() { cout << "A::show()" << endl; }
};

// B和C都继承自A
class B : public A {
public:
    B() { cout << "B构造函数\n"; }
};

class C : public A {
public:
    C() { cout << "C构造函数\n"; }
};

// D继承自B和C
class D : public B, public C {
public:
    D() { cout << "D构造函数\n"; }
};

int main() {
    D d;
    // d.show(); // ❌ 错误！存在二义性，D中有两份A的副本
    d.B::show(); // ✅ 必须指明路径,这里指明是B从A继承来的show方法
    d.C::show(); // ✅ 另一份show方法
    return 0;
}

```

4. 虚继承

	- 为了解决上面多继承时的命名冲突和冗余数据问题，C++ 提出了虚继承，使得在派生类中只保留一份间接基类的成员。
	- 虚继承的目的是让某个类做出声明，承诺愿意共享它的基类。其中，这个被共享的基类就称为***虚基类（Virtual Base Class）***，本例中的 A 就是一个虚基类。在这种机制下，不论虚基类在继承体系中出现了多少次，在派生类中都只包含一份虚基类的成员。
	- 虚派生只影响从指定了虚基类的派生类中进一步派生出来的类，它不会影响派生类本身。

```cpp
#include <iostream>
using namespace std;

// 使用virtual关键字实现虚继承
class A {
public:
    int a;
    A() { cout << "A构造函数\n"; }
    void show() { cout << "A::show()" << endl; }
};

class B : virtual public A {
public:
    B() { cout << "B构造函数\n"; }
};

class C : virtual public A {
public:
    C() { cout << "C构造函数\n"; }
};

class D : public B, public C {
public:
    D() { cout << "D构造函数\n"; }
};

int main() {
    D d;
    d.show(); // ✅ 不再二义性，只有一个A子对象
    return 0;
}

```

此外，如果虚基类的成员只被一条派生路径覆盖，那么仍然可以直接访问这个被覆盖的成员。但是如果该成员被两条或多条路径覆盖了，那就不能直接访问了，此时必须指明该成员属于哪个类。如下面:

```cpp
#include <iostream>
using namespace std;

// 使用virtual关键字实现虚继承
class A {
public:
    int a,b,c,bc;
    A() { cout << "A构造函数\n"; }
    void show() { cout << "A::show()" << endl; }
};

class B : virtual public A {
public:
    int b,bc;
    B() { cout << "B构造函数\n"; }
};

class C : virtual public A {
public:
    int c,bc;
    C() { cout << "C构造函数\n"; }
};

class D : public B, public C {
public:
    D() { cout << "D构造函数\n"; }
};

int main() {
    D d;
    d.show(); // ✅ 不再二义性，只有一个A子对象
    cout<<d.c<<endl;//✅可以访问派生类(C)的优先级比虚基类更高
    cout<<d.b<<endl;//✅可以访问派生类(B)的优先级比虚基类更高
    cout<<d.bc<<endl;//❌不能访问,会产生二义性问题
    return 0;
}

```

可以看到，使用多继承经常会出现二义性问题，必须十分小心。上面的例子是简单的，如果继承的层次再多一些，关系更复杂一些，程序员就很容易陷人迷魂阵，程序的编写、调试和维护工作都会变得更加困难。

---







## 1.6多态(Polymorphism)

> - 多态是指不同对象收到相同的的消息时（即调用相同的函数），产生不同的动作。
> - 直观的说，多态性是指用一个名字定义不同的函数（这里只要函数名相同就行），这些函数执行不同但又类似的操作，从而可以使用相同的方式来调用这些具有不同功能的同名函数。
> - 在面向对象的程序设计中使用多态，能够增强程序的**可扩充性**，即程序需要修改或增加功能的时候，需要**改动和增加的代码较少**。

1. ***编译时多态***

 - 编译时的多态是通过函数重载和运算符重载（重载采用的是静态连编）实现。（注意：函数重载实现的多态是一个类的行为的多态，要在同一个类内才能体现出来的多态，是面向方法的多态）。

 - 静态连编时，系统用实参与形参进行匹配，对于同名的重载函数变根据参数上的差异进行区分，然后进行连编，从而实现编译时的多态。（注意：函数的选择是基于指向对象的***指针类型***或者***引用类型***,这是与动态连编的主要区别）

   ```cpp
   #include <iostream>
   using namespace std;
   class Calculater {
   public:
   	int max(const int& a,const int& b) { return a > b ? a : b; }
   	double max(const double& a,const double& b) { return a > b ? a : b; }
   };
   int main() {
   	Calculater a;
   	cout << a.max(1, 2) << endl;//根据传参类型匹配行为
   	cout << a.max(1.2, 1.5) << endl;
   }
   
   ```

   ```cpp
   #include<iostream>
   using namespace std;
   class A{
       public:
           void show()
           {
               cout<<"我是A"<<endl;
           }
   };
    
   class B:public A{
       public:
       void show()
       {
           cout<<"我是B"<<endl;
       }
   };
    
   int main()
   {
       A a,*pc;
       B b;
    
       pc=&a;
       pc->show();
    
       pc=&b;   //尽管赋值为b,类型仍然为A*调用的是A::show,基于对象的指针或引用类型
       pc->show();
   }
    
   运行结果：
       我是A
       我是A
   ```

   

2. ***虚函数***

   - 当你在基类中声明函数为`virtual`，编译器就会在对象中生成一个“**虚函数表（vtable）**”。
   - 运行时根据实际对象类型，**动态绑定**正确的函数版本。
   - 动态连编是运行阶段完成的连编。即当程序调用到某一函数的时候，系统会根据当前的***对象类型***去寻找和连接其程序的代码，对面向对象的程序而言，就是当对象收到某一消息的时候，才去寻找和连接相应的方法。（函数的选择是基于***对象的类型***）。
   - 一旦基类中某个函数被声明为 `virtual`，那么这个函数在派生类中**自动就是虚函数**，即使你不写 `virtual`。

   ```cpp
   #include <iostream>
   using namespace std;
   
   class Animal {
   public:
       virtual void speak() { cout << "动物在叫\n"; }  // 虚函数
   };
   
   class Dog : public Animal {
   public:
       //override是一种编程规范,在派生类重写时加上，可以让编译器检查函数签名是否匹配，防止误拼写。
       //派生类中也可以写vitual,如这里写成virtual void speak() { cout << "狗在汪汪叫\n"; },这样写和override一样只是保证可读性
       //结果上等价于什么都不写:void speak() { cout << "狗在汪汪叫\n"; }
       void speak() override { cout << "狗在汪汪叫\n"; } // 重写
   };
   
   class Cat : public Animal {
   public:
       void speak() override { cout << "猫在喵喵叫\n"; }
   };
   
   int main() {
       Animal* p1 = new Dog();
       Animal* p2 = new Cat();
       p1->speak();  // 输出：狗在汪汪叫
       p2->speak();  // 输出：猫在喵喵叫
       //这里的p1,p2虽然都是Animal*的类型,但实际调用的确实派生类中的speak函数,调用的函数取决于实际对象类型（Dog/Cat），而不是指针类型（Animal*）。这就是运行时多态
       delete p1;
       delete p2;
   }
   
   ```

   

3. ***虚析构函数***

> - 如果你打算通过**基类指针释放派生类对象**，必须让基类析构函数是虚的，否则会内存泄漏。

```cpp
class Animal {
public:
    virtual ~Animal() { cout << "销毁Animal\n"; }
};

class Dog : public Animal {
public:
    ~Dog() { cout << "销毁Dog\n"; }
};

int main() {
    Animal* a = new Dog();//这里a的指针类型时Animal,如果不写virtual,释放时则Dog不会析构
    delete a;
}
/*输出:
销毁Dog
销毁Animal
*/
```

4. ***纯虚函数与抽象类***

> 纯虚函数（Pure Virtual Function）让一个类变成**抽象类（abstract class）**，不能直接实例化，只能被继承,这相当于是定义了一个接口。

```cpp
class Animal {
public:
    virtual void speak() = 0;  // 纯虚函数
};

class Dog : public Animal {
public:
    void speak() override { cout << "狗叫！\n"; }
};

```

此时：

- `Animal` 不能实例化；
- `Dog` 必须实现 `speak()` 才能创建对象。

5. ***虚表(vtable)机制原理***

   ```cpp
   #include <iostream>
   using namespace std;
   
   class Base {
   public:
       virtual void say() { cout << "Base\n"; }
   };
   
   class Derived : public Base {
   public:
       void say() override { cout << "Derived\n"; }
   };
   
   int main() {
       Base* p = new Derived;
       p->say(); // 输出 Derived
   }
   
   ```

   当类中**第一次出现虚函数**时，
    编译器会自动为该类生成一个隐藏的数据结构，叫：

   > **虚函数表（vtable）**

   它其实就是一个 **函数指针数组**。
   每一个虚函数在表中占一个位置，指向实际的函数实现。

   此外,这里Derived中的say函数也是虚函数,这是因为基类中的say函数是虚函数,尽管这里重写了

   | 类        | 虚表指针 | 虚表内容               |
   | --------- | -------- | ---------------------- |
   | `Base`    | vptr →   | `[0] → Base::say()`    |
   | `Derived` | vptr →   | `[0] → Derived::say()` |

    每个带虚函数的对象内部（对象头部）会额外存放一个隐藏指针指向虚函数表：

   ```cpp
   Base b;
   b.vptr -> vtable(Base)
   
   Derived d;
   d.vptr -> vtable(Derived)
   ```

   执行过程中

   `p->say(); // 输出 Derived`这一行编译器实际会编译成伪代码`(*(p->vptr)[0])();`

​		实际就是:

​		`p->vptr`：拿到对象内部的“虚表指针”；

​		`[0]`：取表中第一个函数地址；

​		`(*)()`：解引用并调用它。

​	于是：

   - 如果 `p` 指向的是 `Base` 对象，`vptr` 指向 `Base` 的表 → 调用 `Base::say()`

   - 如果 `p` 指向的是 `Derived` 对象，`vptr` 指向 `Derived` 的表 → 调用 `Derived::say()`

​	就这样，C++ 实现了“**根据对象类型，动态决定调用哪个函数**”的魔法。

此外需要注意

- **只有有虚函数的类才有 vtable / vptr**,没有虚函数的类，不会多出那8字节的 vptr。

- **vptr 通常是每个对象独有的**，但表是共享的（每种类一张）。

- 多继承时每个父类各有一张虚表
- 可以自己查看虚表

```cpp
#include <iostream>
using namespace std;

class Base {
public:
    virtual void say() { cout << "Base\n"; }
};

class Derived : public Base {
public:
    void say() override { cout << "Derived\n"; }
};

int main() {
    Derived d;
    // 强行取出虚表指针
    void** vptr = *(void***)&d;
    void(*fun)() = (void(*)())vptr[0];
    fun(); // 输出 Derived
}

```

## 1.7 面向对象项目实践

模拟一个简化的“操作系统任务调度器”，
管理多个用户（User），每个用户可以提交多个任务（Task），
系统通过不同的调度策略（Scheduling Policy）决定任务的执行顺序。

详见Project文件夹

问题:

1. ***对象切片***

将一个子类对象赋值给一个基类对象（不是指针或引用），只会保留基类部分，子类特有的数据和行为（如重载的 printInfo()）会被“切掉”。

```cpp
vector<Task> tasks; // 存储的是 Task 对象，不是 Task*
IOtask task();//IOtask是tasks的一个派生类
user.addTask(task); // IOtask 被切片成 Task,派生类的功能失去
```

解决方案:

使用指针存储对象任务







# 二 、模板编程（Generics）





















# 三、标准库





















# 四、资源管理









# 五、运算符重载





# 六、异常处理





# 七、现代C++



# 八、关键字



## 1.Const

Const修饰的成员函数其内部也只能调用其他被Const修饰的成员函数













