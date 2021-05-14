#include "Library.h"

Library::Library()
{
	isOpened = false;
	isSaved = false;
	isChanged = false;
	fileName = "";
}

void Library::open(String& filename)
{
	if (isOpened)
	{
		std::cout << "A database is already loaded! Save or close before trying to open a new one!\n";
		return;
	}
	size_t size = filename.getSize();
	if (filename[size - 4] != '.' || filename[size - 3] != 't' || filename[size - 2] != 'x' || filename[size - 1] != 't')
	{
		std::cout << "File name must end with .txt !\n";
		return;
	}
	users.loadUsers(filename.getText());
	this->fileName = filename;
	this->isOpened = true;
}

void Library::save()
{
	if (!isOpened)
	{
		std::cout << "Open a file before executing any other commands!\n";
		return;
	}
	if (!isChanged)
	{
		std::cout << "No changes have been made!\n";
		return;
	}
	users.saveUsers(this->fileName);
}

void Library::logIn()
{
	if (!isOpened)
	{
		std::cout << "Open a file before executing any other commands!\n";
		return;
	}
	size_t size = users.getSize();
	if (users.activeUserIndex() != -1)
	{
		std::cout << "Another user is already logged in! Please log out!\n";
		return;
	}
	size_t counter = 0;
	int index = -1;
	String name;
	String password;
	do
	{
		std::cout << "Enter your username: ";
		std::cin >> name;
		for (size_t i = 0; i < size; ++i)
		{
			if (name == users[i].getName())
			{
				index = i;
				break;
			}
		}
		if (index == -1)
		{
			counter++;
			std::cout << "User with that username not found! Remaining tries: " << 5 - counter << std::endl;
			name = "";
		}
	} while (index == -1 && counter <5);
	if (counter >= 5)
	{
		std::cout << "Try again later!\n";
		return;
	}
	counter = 0;
	do
	{
		std::cout << "Enter your password: ";
		std::cin >> password;
		if (users[index].getPassword() == password)
		{
			users[index].setLoggedIn(true);
			std::cout << "Welcome " << name << "!\n";
			return;
		}
		counter++;
		std::cout << "Incorrect password! Remaining tries: " << 5 - counter << std::endl;
		password = "";
	} while (counter < 5);
	std::cout << "Try again later!\n";
	return;
}

void Library::logOut()
{
	if (!isOpened)
	{
		std::cout << "Open a file before executing any other commands!\n";
		return;
	}
	if (users.activeUserIndex() == -1)
	{
		std::cout << "No one is logged in!\n";
		return;
	}
	users[users.activeUserIndex()].setLoggedIn(false);
	std::cout << "You have successfully logged out!\n";
	return;
}

void Library::exit()
{
	if (isChanged)
	{
		std::cout << "There are unsaved changes! Are you sure you want to exit?\n0 - Save&Exit | 1 - Exit : ";
		bool choice = 0;
		std::cin >> choice;
		if (choice)
		{
			std::cin.ignore();
			std::cout << "Good bye!\n";
			return;
		}
		std::cin.ignore();
		this->save();
		std::cout << "Good bye!\n";
		return;
	}
	std::cout << "Good bye!\n";
	return;
}

void Library::help() const
{
	std::cout << "Available commands:\n";
	std::cout << "help                                       opens the help menu\n";
	std::cout << "open <file name>                           opens <file name>\n";
	std::cout << "close                                      closes the current file\n";
	std::cout << "save                                       saves the current file\n";
	std::cout << "save as <location>                         saves the current file in <location>\n";
	std::cout << "exit                                       exits the program\n";
	std::cout << "login                                      logs in in the system\n";
	std::cout << "logout(*)                                  logs out of the system\n";
	std::cout << "books all(*)                               prints the info of all books\n";
	std::cout << "books find <option> <description>(*)       prints the IDs of those books\n";
	std::cout << "books info <id>(*)                         prints the info of a book\n";
	std::cout << "books sort <by what (asc|desc)>(*)         sorts the books\n";
	std::cout << "books add(*)(^)                            adds a new book\n";
	std::cout << "books remove(*)(^)                         removes a book\n";
	std::cout << "users add <username> <password>(*)(^)      adds a new user\n";
	std::cout << "users remove <username>(*)(^)              removes a user\n\n";
	std::cout << "(*) - requires to be logged in | (^) - requires admin rights\n";
}

void Library::booksAll() const
{
	if (!isOpened)
	{
		std::cout << "Open a file before executing any other commands!\n";
		return;
	}
	if (users.activeUserIndex() == -1)
	{
		std::cout << "You have to be logged in!\n";
		return;
	}
	if (books.getSize() == 0)
	{
		std::cout << "No available books!\n";
		return;
	}
	books.allBooksBrieflyInfo();
}

void Library::booksInfo(size_t id) const
{
	if (!isOpened)
	{
		std::cout << "Open a file before executing any other commands!\n";
		return;
	}
	if (users.activeUserIndex() == -1)
	{
		std::cout << "You have to be logged in!\n";
		return;
	}
	books.bookDetailedInfo(id);
}

void Library::booksFind(String& option, const String& description) const
{
	if (!isOpened)
	{
		std::cout << "Open a file before executing any other commands!\n";
		return;
	}
	if (users.activeUserIndex() == -1)
	{
		std::cout << "You have to be logged in!\n";
		return;
	}
	books.findBook(option, description);
}

void Library::booksSortAscending(const String& option)
{
	if (!isOpened)
	{
		std::cout << "Open a file before executing any other commands!\n";
		return;
	}
	if (users.activeUserIndex() == -1)
	{
		std::cout << "You have to be logged in!\n";
		return;
	}
	books.sortBooksAscending(option);
	this->isChanged = true;
}

void Library::booksSortDescending(const String& option)
{
	if (!isOpened)
	{
		std::cout << "Open a file before executing any other commands!\n";
		return;
	}
	if (users.activeUserIndex() == -1)
	{
		std::cout << "You have to be logged in!\n";
		return;
	}
	books.sortBooksDescending(option);
	this->isChanged = true;
}

void Library::booksAdd()
{
	if (!isOpened)
	{
		std::cout << "Open a file before executing any other commands!\n";
		return;
	}
	if (users.activeUserIndex() == -1)
	{
		std::cout << "You have to be logged in!\n";
		return;
	}
	User active;
	active = activeUser();
	if (active.getAdmin() == false)
	{
		std::cout << "Admin rights required!\n";
		return;
	}
	books.addBook();
	this->isChanged = true;
}

void Library::booksRemove(const String& title )
{
	if (!isOpened)
	{
		std::cout << "Open a file before executing any other commands!\n";
		return;
	}
	if (users.activeUserIndex() == -1)
	{
		std::cout << "You have to be logged in!\n";
		return;
	}
	User active;
	active = activeUser();
	if (active.getAdmin() == false)
	{
		std::cout << "Admin rights required!\n";
		return;
	}
	books.removeBook(title);
	this->isChanged = true;
}

void Library::usersAdd(const String& name, const String& password)
{
	if (!isOpened)
	{
		std::cout << "Open a file before executing any other commands!\n";
		return;
	}
	if (users.activeUserIndex() == -1)
	{
		std::cout << "You have to be logged in!\n";
		return;
	}
	User active;
	active = activeUser();
	if (active.getAdmin() == false)
	{
		std::cout << "Admin rights required!\n";
		return;
	}
	users.addUser(name, password);
	std::cout << "New user successfully added!\n";
	this->isChanged = true;
}

void Library::usersRemove(String& name)
{
	if (!isOpened)
	{
		std::cout << "Open a file before executing any other commands!\n";
		return;
	}
	if (users.activeUserIndex() == -1)
	{
		std::cout << "You have to be logged in!\n";
		return;
	}
	User active;
	active = activeUser();
	if (active.getAdmin() == false)
	{
		std::cout << "Admin rights required!\n";
		return;
	}
	if (name == active.getName())
	{
		std::cout << "You can't remove yourself!\n";
		return;
	}
	users.removeUser(name);
	this->isChanged = true;
}

User& Library::activeUser()
{
	return users[users.activeUserIndex()];
}
