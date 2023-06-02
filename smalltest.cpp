#include<fstream>
#include<conio.h>
#include<stdio.h>
#include<process.h>
#include<string.h>
#include<iomanip>
#include<iostream>
#include<vector>
#include<list>

using namespace std;
class  Book{
    private :
        string title;
        string author;
        string genre;
        bool available;
    public:
        Book(){};
        Book(string title, string author,string genre){
            this-> title =title;
            this -> author =author;
            this -> genre =genre;
        }
        string getTitle() const {
            return title;
        }
        string getAuthor() const {
            return author;
        }
        string getGenre() const {
            return genre;
        };
        bool operator==(const Book & other)const{    // the "const make sure that there is no change in attributes "
            return (title==other.getTitle() && author== other.getAuthor());
        }
        bool operator<(const Book & other ) const{
            if(title == other.getTitle()){
                return author <other.getAuthor();
            }
            return (title <other.getTitle());
        }
        bool getAvailable(){
            return available;
        }
        void Borrow_book(){
            available =1;
        }
        void Return_book(){
            available=0;
        }
        void get_information(){
            cout << "The book :" << title << "belong to " << author << " is " << genre << "type book \n";
        }
};
// This part is for split the information from file input purpose only
struct information_book{
    string title;
    string author;
    string genre;
};
//This function used to solve the space character in first and final of a string
string trimSentence(string sentence) {
    size_t start = sentence.find_first_not_of(" ");
    size_t end = sentence.find_last_not_of(" ");

    if (start == std::string::npos) {
        // No non-space characters found, return an empty string
        return "";
    }

    return sentence.substr(start, end - start + 1);
};
information_book read_line(string line){
    string delimiter1="*Title:";
    string delimiter2= "*Author:";
    string delimiter3="*Genre:";
    information_book book;
    string genre;
    string title;
    string author;
    size_t start = 0;
    size_t end1 = line.find(delimiter1); // Find the position of the word delimiter
    size_t end2 = line.find(delimiter2);
    size_t end3= line.find(delimiter3);
    title =line.substr(end1 + delimiter1.length(), end2 -delimiter2.length() );
    author= line.substr(end2 + delimiter2.length(),end3 -end2-delimiter2.length());
    genre=line.substr(end3 +delimiter3.length(), line.length() );     
    book.title=trimSentence(title);
    book.author=trimSentence(author);
    book.genre=trimSentence(genre);
    return book;
}

struct check_index{
    int check_title;
    int check_author;
    int check_genre;
};
check_index hash_function(string title, string author, string genre) {
    check_index result;
    title = trimSentence(title);
    author = trimSentence(author);
    genre = trimSentence(genre);

    int check_author1 = author[0];
    int check_author2 = author[author.length() / 2];
    int check_title1 = title[0];
    int check_title2 = title[title.length() / 2];
    int check_genre1 = genre[0];
    int check_genre2 = genre[genre.length() / 2];

    if (isupper(check_author1))
        check_author1 += 'a' - 'A';
    if (isupper(check_author2))
        check_author2 += 'a' - 'A';
    if (isupper(check_title1))
        check_title1 += 'a' - 'A';
    if (isupper(check_title2))
        check_title2 += 'a' - 'A';
    if (isupper(check_genre1))
        check_genre1 += 'a' - 'A';
    if (isupper(check_genre2))
        check_genre2 += 'a' - 'A';

    int check_author = check_author1 + check_author2;
    if (check_author >200)check_author-=200;
    if (check_author >100)check_author-=100;
    int check_title = check_title1 + check_title2;
    if (check_title >200)check_title-=200;
    if (check_title>100)check_title-=100;
    int check_genre = check_genre1 + check_genre2;
    if (check_genre>200)check_genre-=200;
    if (check_genre >100) check_genre-=100;
    if (check_genre>50) check_genre-=50;

    result.check_title = check_title;
    result.check_author = check_author;
    result.check_genre = check_genre;

    return result;
};

#include <vector>
#include <iostream>
#include <fstream>
#include "Main1.h"

using namespace std;
class Libary {
    private:
        vector<vector<vector<vector<Book>>>> ALL_BOOK;

        void locate_the_book(Book book){
            string title=book.getTitle();
            string author =book.getAuthor();
            string genre =book.getGenre();
            check_index locate_book =hash_function(title, author, genre);
            // cout << locate_book.check_genre << " " << locate_book.check_title <<" "<< locate_book.check_author<<"\n";
            ALL_BOOK[locate_book.check_genre][locate_book.check_title][locate_book.check_author].push_back(book);
            Book boo1=ALL_BOOK[locate_book.check_genre][locate_book.check_title][locate_book.check_author][0];

        }
    public :    
        Libary(){
            ALL_BOOK = vector<vector<vector<vector<Book>>>>(51, vector<vector<vector<Book>>>(101, vector<vector<Book>>(101, vector<Book>(0))));
            string title_name;
            string author_name;
            ifstream Data_base_book("book.inp");
            if(Data_base_book.is_open()){
                string line ;
                while (getline(Data_base_book, line)){          
                    information_book book =read_line(line);
                    Book book1(book.title,book.author,book.genre);
                    locate_the_book(book1);
                };
                Data_base_book.close();
            }else {
            std::cerr << "Failed to open the file book.inp." << std::endl;
            }
        };
        bool Find_the_book_availabel(string title,string author,string genre ){
            title=trimSentence(title);
            author=trimSentence(author);
            genre=trimSentence(genre);
            Book book_find(title,author,genre);
            check_index index=hash_function(title, author, genre);
            int check_title= index.check_title;
            int check_author=index.check_author;
            int check_genre=index.check_genre;
            cout << check_genre <<" "<< check_title <<" "<< check_author <<"\n";
            book_find.get_information();
            for(auto book : ALL_BOOK[check_genre][check_title][check_author]){
                book.get_information();
                if(book== book_find){
                    if(book.getAvailable()){
                        cout << "The book :" << title << "of " << author << "is now available !";
                        return 1; 
                    }
                } 
            };
            cout <<"The book :" << title << "of " << author << "is now not available !";
            return 0;
        }
        vector<Book> Find_book_with_special_info(string title="-1", string author="-1", string genre="-1"){
            check_index index=hash_function(title, author, genre);
            int check_title= index.check_title;
            int check_author=index.check_author;
            int check_genre=index.check_genre;
            string up_title=title;
            up_title[0]= toupper(up_title[0]);
            string up_author=author;
            up_author[0]=toupper(up_author[0]);
            string up_genre= genre;
            up_genre[0]=toupper(up_genre[0]);
            vector<Book> related_title;
            vector<Book> related_author;
            vector<Book> related_genre;
            if(title=="-1"){
                // if we do not have information about genre and title but have author informaion
                if (genre =="-1"){
                    for(int i =0; i< ALL_BOOK.size(); i++){
                        for (int j=0 ; j <ALL_BOOK[i].size(); j++){
                            if (check_author < ALL_BOOK[i][j].size()){
                                for(Book book : ALL_BOOK[i][j][check_author]){
                                    if(book.getAuthor()== author|| book.getAuthor()== up_author){
                                        related_author.push_back(book);
                                    }
                                }
                            }  
                        }
                    }
                }
                // if we do not have information about author and title but have genre information
                else if (author=="-1"){
                    cout << check_genre<<"\n";
                    if (check_genre < ALL_BOOK.size()){
                        cout << ALL_BOOK[check_genre].size();
                        for(int i=0; i<ALL_BOOK[check_genre].size(); i++){           
                            for(int j= 0; j< ALL_BOOK[check_genre][i].size(); j++){
                                for(Book book: ALL_BOOK[check_genre][i][j]){
                            
                                    // because the fiction !=Fiction
                                    if(book.getGenre()== genre || book.getGenre()== up_genre){
                                        
                                        related_genre.push_back(book);
                                    }
                                }
                            }
                        }
                    }
                    
                }// if we have information about genre and author but not have information of title  
                else {
                    if(check_genre < ALL_BOOK.size()){
                        for (int i=0; i<ALL_BOOK[check_genre].size(); i++ ){
                            if (check_author< ALL_BOOK[check_author][i].size()){
                                for(Book book : ALL_BOOK[check_genre][i][check_author]){
                                    if((book.getAuthor()== author || book.getAuthor()== up_author) && ( book.getGenre()== genre || book.getGenre()== up_genre) ){
                                        related_author.push_back(book);
                                    }
                                }
                            }
                        }
                    } 
                }                
            }
            // if we have information of title
            if(author=="-1" && title!="-1"){
                // if we do not have information of genre
                if (genre =="-1"){
                    for(int i=0; i < ALL_BOOK.size(); i++){
                        if (check_title <ALL_BOOK[i].size()){
                            for(int j=0; j< ALL_BOOK[i][check_title].size(); j++){
                                for(Book book : ALL_BOOK[i][check_title][j]){
                                    if (book.getTitle()==title ||  book.getTitle()== up_title){
                                        related_title.push_back(book);
                                    }
                                }
                            }
                        } 
                    }
                }
                // if we have information of genre
                else{
                    if (check_genre < ALL_BOOK.size() && check_title< ALL_BOOK[check_genre].size()){
                        for(int i =0; i < ALL_BOOK[check_genre][check_title].size(); i++){
                            for(Book book : ALL_BOOK[check_genre][check_title][i]){
                                if((book.getTitle()==title || book.getTitle()== up_title) && (book.getGenre()==genre || book.getGenre()== up_genre)){
                                    related_title.push_back(book);
                                }
                            }
                        }
                    } 
                }
            }

            // print out the available book;
            if (related_title.size()!=0){
                
                cout << "List of books whose title is "<< title << " are:\n";
                for (Book book : related_title){
                    book.get_information();
                }
                return related_title;
            } else if (related_author.size()!= 0){
                
                cout <<"List of books whose author is "<< author <<"are :\n";
                for (Book book : related_author){
                    book.get_information();
                }
                return related_author;
            } else if(related_genre.size()!=0){
                
                cout<< "List of books whose genre is "<< genre << "are :\n";
                for(Book book : related_genre){
                    book.get_information();
                }
                return related_genre;
            } else{
                cout<< "Can not find any book";
                return related_author;
            }
        } 
};
int main() {
    Libary Libary;
    // Libary.Find_the_book_availabel("To Kill a Mockingbird" ," Harper Lee "," Fiction");
    Libary.Find_book_with_special_info("-1","-1","fiction");
    // Book bookk= Li
    return 0;
}