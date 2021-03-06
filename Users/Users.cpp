// Users.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"

#include <string>
#include<map>
#include<vector>
#include <memory>
#include<iostream>
#include<iomanip>
#include<algorithm>
#include<conio.h>
#include<limits>

using namespace std;

class Person {
	string login;
	string password;
	string name;
	string lastname;
	size_t age = 0;

public:
	static size_t maxLoginSize;
	static size_t maxPasswordSize;
	static size_t maxNameSize;
	static size_t maxLastNameSize;
	static size_t minLoginSize;
	static size_t minPasswordSize;
	static size_t minNameSize;
	static size_t minLastNameSize;
	static size_t maxAge;
	static size_t minAge;
	static size_t NumberColumn;

	string getLogin() const {
		return login;
	}
	string getPassword() const {
		return password;
	}
	string getName() const {
		return name;
	}
	string getLastName() const {
		return lastname;
	}
	size_t getAge() const {
		return age;
	}
	bool setLogin(string login) {
		size_t length = login.length();
		if (length <= maxLoginSize && length >= minLoginSize) {
			this->login = login;
			return true;
		}
		cout << "Login length mast be beetween " << minLoginSize << " and " << maxLoginSize << " characters." << endl;
		return false;
	}
	bool setPassword(string password) {
		size_t length = password.length();
		if (length <= maxPasswordSize && length >= minPasswordSize) {
			this->password = password;
			return true;
		}
		cout << "Password length mast be beetween " << minPasswordSize << " and " << maxPasswordSize << " characters." << endl;
		return false;
	}
	bool setName(string name) {
		size_t length = name.length();
		if (length <= maxNameSize && length >= minNameSize) {
			this->name = name;
			return true;
		}
		cout << "Name length mast be beetween " << minNameSize << " and " << maxNameSize << " characters." << endl;
		return false;
	}
	bool setLastName(string lastname) {
		size_t length = lastname.length();
		if (length <= maxLastNameSize && length >= minLastNameSize) {
			this->lastname = lastname;
			return true;
		}
		cout << "Last Name length mast be beetween " << minLastNameSize << " and " << maxLastNameSize << " characters." << endl;
		return false;
	}
	bool setAge(size_t age) {
		if (age <= maxAge && age >= minAge) {
			this->age = age;
			return true;
		}
		cout << "Age mast be beetween " << minAge << " and " << maxAge << "." << endl;
		return false;
	}
	Person()
	{}
	Person(string login) : login(login)
	{}
	Person(const Person& p)
		: login(p.login)
		, password(p.password)
		, name(p.name)
		, lastname(p.lastname)
		, age(p.age)
	{}
	Person(Person&& p)
	{
		swap(p, *const_cast<Person*>(this));
	}
	Person& operator=(const Person& p) {
		login = p.login;
		password = p.password;
		name = p.name;
		lastname = p.lastname;
		age = p.age;
		return *this;
	}
	Person& operator=(Person&& p) {
		swap(p, *const_cast<Person*>(this));
		return *this;
	}
	friend ostream& operator<<(ostream& o, Person& P) {
		o << setw(maxLoginSize + 1) << P.login
			<< setw(maxPasswordSize + 1) << P.password
			<< setw(maxNameSize + 1) << P.name
			<< setw(maxLastNameSize + 1) << P.lastname
			<< setw(NumberColumn) << P.age << endl;
		return o;
	}

	friend istream& operator>>(istream& i, Person& P) {
		{
			string input;
			do {
				cout << "enter login: ";
				i >> input;
			} while (!P.setLogin(input));
			do {
				cout << "enter password: ";
				i >> input;
			} while (!P.setPassword(input));
			do {
				cout << "enter name: ";
				i >> input;
			} while (!P.setName(input));
			do {
				cout << "enter lastname: ";
				i >> input;
			} while (!P.setLastName(input));
		}
		size_t input;
		do {
			cout << "enter age: ";
			if(!(i >> input)){
				i.clear();
				i.ignore(numeric_limits<streamsize>::max(), '\n');
			}
		} while (!P.setAge(input));


		return i;
	}
};

typedef map<string, Person> Persons;
typedef pair<string, Person> PersonItem;

class ICommand {
protected:
	Persons *collection;
public:
	virtual bool exec() {
		system("cls");
		action();
		system("pause");
		return true;

	}
	virtual void action() = 0;
};

typedef vector<pair<string, shared_ptr<ICommand>>> CmdList;
typedef pair<string, shared_ptr<ICommand>> CmdItem;

class Listener {
	string title;
	string footter;
	CmdList commands;
public:
	Listener & add(string description, ICommand* command) {
		commands.push_back(CmdItem(description, command));
		return *this;
	}
	Listener & print() {
		system("cls");
		cout << title << endl << endl;
		int index = 1;
		for (auto command = commands.begin() + 1; command != commands.end(); ++command, ++index)
			cout << index << ".\t" << command->first << endl;
		cout << "0.\t" << commands[0].first << endl << endl << footter;
		return *this;

	}
	Listener&setTitle(string title) {
		this->title = title;
		return *this;
	}
	Listener(string title, string footter)
		: title(title)
		, footter(footter)
	{}
	Listener& listen() {
		int ch;
		do {
			print();
			ch = _getche() - 48;
		} while (ch < 0 || ch >= static_cast<int>(commands.size()) || commands[ch].second->exec());
		return *this;
	}
};

class Exit : public ICommand {
public:
	bool exec() {
		return false;
	}
	void action() {}
	Exit() {}
};

class Show : public ICommand {
public:
	void printTitle() {
		cout << setw(Person::NumberColumn) << "#"
			<< setw(Person::maxLoginSize + 1) << "Login"
			<< setw(Person::maxPasswordSize + 1) << "Password"
			<< setw(Person::maxNameSize + 1) << "Name"
			<< setw(Person::maxLastNameSize + 1) << "LastName"
			<< setw(Person::NumberColumn) << "Age"
			<< endl;
	}
	void printLine() {
		cout << setfill('-') << setw(
			Person::NumberColumn + 
			Person::maxLoginSize + 1 +
			Person::maxPasswordSize + 1 +
			Person::maxNameSize + 1 +
			Person::maxLastNameSize + 1 +
			Person::NumberColumn
		) << '-' << endl << setfill(' ');
	}
	bool empty() {
		if (this->collection) {
			if (this->collection->empty()) {
				cout << "User list is empty." << endl << endl;
				return true;
			}
		}
		return false;
	}

	void action() {
		if (!empty()) {
			cout << "Users list:" << endl << endl;
			printTitle();
			printLine();
			int index = 0;
			for(auto &person : *collection){
				++index;
				cout << setw(Person::NumberColumn) << index << person.second;
			}
			printLine();
			cout << endl;
		}
	}
	explicit Show(Persons &collection) {
		this->collection = &collection;
	}
};

class Add : public ICommand {
public:
	bool check(const Person& p) {
		if (this->collection->count(p.getLogin()) > 0) {
			cout << endl << "User with login " << p.getLogin() << " allredy exist." << endl;
			return true;
		}
		return false;
	}
	void action() {
		Person p;
		cin >> p;
		while (check(p)) {
			cout << "Login " << p.getLogin() << " is busy. Try to enter another one: ";
			string input;
			do {
				cin >> input;
			} while (!p.setLogin(input));
		}

		(*this->collection)[p.getLogin()] = p;
		cout << endl << "User with login " << p.getLogin() << " added successfully." << endl << endl;
		
	}
	explicit Add(Persons &collection) {
		this->collection = &collection;
	}
};

class Select : public ICommand {
	Person* selected = nullptr;
public:
	bool check(string login) {
		if (this->collection->count(login) <= 0) {
			cout << endl << "User with login " << login << " don`t exist." << endl;
			return false;
		}
		return true;
	}
	void action() {
		cout << "Enter user login: ";
		string login;
		cin >> login;
		if (check(login))selected = &this->collection->find(login)->second;
		cout << endl;

	}
	bool exec() {
		system("cls");
		action();
		return true;
	}

	Person* getSelected() {
		return selected;
	}
	explicit Select(Persons &collection) {
		this->collection = &collection;
		exec();
	}

};

class EditLogin : public ICommand {
	shared_ptr<Person*> selected;
	bool check(string login) {
		if (this->collection->count(login) > 0) {
			cout << endl << "User with login " << login << " allredy exist." << endl;
			return true;
		}
		return false;
	}

public:
	EditLogin(Persons &collection, shared_ptr<Person*> selected)
		: selected(selected)
	{
		this->collection = &collection;
	}

	void action() {
		string login;
		Person newPerson(**selected);
		do {
			cout << "Enter new user login: ";
			cin >> login;
		} while (check(login) || !newPerson.setLogin(login));
		collection->erase((*selected)->getLogin());
		(*collection)[login] = newPerson;
		cout << endl << "Login was changed successfully to " << login << "." << endl << endl;
	}
};

class EditPassword : public ICommand {
	shared_ptr<Person*> selected;

public:
	EditPassword(shared_ptr<Person*> selected)
		: selected(selected)
	{}

	void action() {
		string input;
		do {
			cout << "Enter new user password: ";
			cin >> input;
		} while (!(*selected)->setPassword(input));
		cout << endl << "Password was changed successfully to " << input << "." << endl << endl;
	}
};

class EditName : public ICommand {
	shared_ptr<Person*> selected;

public:
	EditName(shared_ptr<Person*> selected)
		: selected(selected)
	{}

	void action() {
		string input;
		do {
			cout << "Enter new user name: ";
			cin >> input;
		} while (!(*selected)->setName(input));
		cout << endl << "Name was changed successfully to " << input << "." << endl << endl;
	}
};

class EditLastName : public ICommand {
	shared_ptr<Person*> selected;

public:
	EditLastName(shared_ptr<Person*> selected)
		: selected(selected)
	{}

	void action() {
		string input;
		do {
			cout << "Enter new user last name: ";
			cin >> input;
		} while (!(*selected)->setLastName(input));
		cout << endl << "Last name was changed successfully to " << input << "." << endl << endl;
	}
};

class EditAge : public ICommand {
	shared_ptr<Person*> selected;

public:
	EditAge(shared_ptr<Person*> selected)
		: selected(selected)
	{}

	void action() {
		size_t input;
		do {
			cout << "Enter new user age: ";
			if (!(cin >> input)) {
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
			}
		} while (!(*selected)->setAge(input));
		cout << endl << "Age was changed successfully to " << input << "." << endl << endl;
	}
};

class Edit : public ICommand {
	void listen(shared_ptr<Person*> selected) {
		Listener(
			string("Login ") + (*selected)->getLogin() + " is selected. You can:",
			"Press the button like menu number to continue . . ."
		)
			.add("Exit", new Exit)
			.add("Edit login", new EditLogin(*collection, selected))
			.add("Edit password", new EditPassword(selected))
			.add("Edit name", new EditName(selected))
			.add("Edit last name", new EditLastName(selected))
			.add("Edit age", new EditAge(selected))
			.listen();
	}
public:
	void action() {
		shared_ptr<Person*> selected(new Person*);
		*selected = Select(*collection).getSelected();
		if (*selected) listen(selected);
		else system("pause");
	}
	bool exec() {
		system("cls");
		action();
		return true;
	}
	explicit Edit(Persons &collection) 
	{
		this->collection = &collection;
	}
};

class Remove : public ICommand {
public:
	void action() {
		shared_ptr<Person*> selected(new Person*);
		*selected = Select(*collection).getSelected();
		if (*selected) {
			string login = (*selected)->getLogin();
			collection->erase(login);
			cout << endl << "User with login " << login << " deleted successfully." << endl << endl;
		}
	}
	explicit Remove(Persons &collection) {
		this->collection = &collection;
	}
};



size_t Person::maxLoginSize = 20;
size_t Person::maxPasswordSize = 20;
size_t Person::maxNameSize = 20;
size_t Person::maxLastNameSize = 20;
size_t Person::maxAge = 100;
size_t Person::minLoginSize = 3;
size_t Person::minPasswordSize = 6;
size_t Person::minNameSize = 2;
size_t Person::minLastNameSize = 2;
size_t Person::minAge = 16;
size_t Person::NumberColumn = 4;



int main()
{
	Persons collection;
	Listener("Main menu.","Press the button like menu number to continue . . .")
		.add("Exit.", new Exit)
		.add("Show all users.", new Show(collection))
		.add("Add new user.", new Add(collection))
		.add("Edit user.", new Edit(collection))
		.add("Remove user.", new Remove(collection))
		.listen();
	return 0;
}

