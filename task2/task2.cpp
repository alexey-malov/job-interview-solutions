// task2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <unordered_map>
#include <deque>
#include <vector>

using namespace std;

class Book
{
public:
	Book(string id, string title, string author)
		: id_(std::move(id))
		, title_(std::move(title))
		, author_(std::string(author))
	{}

	const string& GetId() const { return id_; }
	const string& GetTitle() const { return title_; }
	const string& GetAuthor() const { return author_; }

private:
	std::string id_;
	std::string title_;
	std::string author_;
};

ostream& operator<<(ostream& os, const Book& book) {
	return os << "["sv << book.GetId() << "] '"sv << book.GetTitle() << "' by "sv << book.GetAuthor();
}

class Library
{
public:
	Library() = default;

	Library(const Library&) = delete;
	Library& operator=(const Library&) = delete;

	void AddBook(const Book& book) {
		if (auto it = id_to_book_.find(book.GetId());
			it != id_to_book_.end()) {
			throw std::invalid_argument("Book already exists");
		}

		books_.emplace_back(book);

		IdToBook::iterator id_it = id_to_book_.end();
		TitleToBook::iterator title_it = title_to_book_.end();

		try {
			id_it = id_to_book_.emplace(book.GetId(), &books_.back()).first;
			title_it = title_to_book_.emplace(book.GetTitle(), &books_.back());
		}
		catch (...) {
			books_.pop_back();
			if (id_it != id_to_book_.end()) {
				id_to_book_.erase(id_it);
			}
			if (title_it != title_to_book_.end()) {
				title_to_book_.erase(title_it);
			}
			throw;
		}
	}

	const Book* FindBookById(const string& id) const {
		if (auto it = id_to_book_.find(id); it != id_to_book_.end()) {
			return it->second;
		}
		return nullptr;
	}

	std::vector<const Book*> FindAllBookByTitle(const string& title) const {
		auto [start, end] = title_to_book_.equal_range(title);
		std::vector<const Book*> result;
		for (; start != end; ++start) {
			result.emplace_back(start->second);
		}
		return result;
	}

private:
	std::deque<Book> books_;
	using IdToBook = std::unordered_map<string, Book*>;
	using TitleToBook = std::unordered_multimap<string, Book*>;
	IdToBook id_to_book_;
	TitleToBook title_to_book_;
};


void PrintBooks(const vector<const Book*> books) {
	for (auto* b : books) {
		cout << *b << endl;
	}
}

int main()
{
	Library lib;
	lib.AddBook(Book{ "id1"s, "Harry Potter and the Sorcerer's Stone"s, "Joanne Rowling"s });
	lib.AddBook(Book{ "id2"s, "Harry Potter and the Chamber of Secrets"s, "Joanne Rowling"s });
	lib.AddBook(Book{ "id3"s, "Person of Interest"s, "Susan Choi"s });
	lib.AddBook(Book{ "id4"s, "Person of Interest"s, "Theresa Schwegel"s });

	PrintBooks(lib.FindAllBookByTitle("Person of Interest"s));

	cout << "----"sv << endl;

	if (const auto* b = lib.FindBookById("id1"s)) {
		cout << *b << endl;
	}
}
