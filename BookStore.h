#pragma once
#include "Book.h"
#include "String.h"
class BookStore
{
public:
	BookStore();
	void allBooksBrieflyInfo() const;
	void bookDetailedInfo(const size_t) const;
	void findBook(const String&,const String&) const;
	void sortBooksAscending(const String&);
	void sortBooksDescending(const String&);
	void addBook();
	void removeBook(const String&);
	size_t getSize() const;
private:
	Vector<Book> books;
	size_t id;
};