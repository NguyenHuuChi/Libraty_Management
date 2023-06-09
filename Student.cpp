#include<fstream>
#include<conio.h>
#include<stdio.h>
#include<process.h>
#include<string.h>
#include<iomanip>
#include<iostream>
#include<vector>
#include<list>
#include <ctime>
#include <algorithm>
#include"Library.h"
#include "Student.h"
using namespace std;

int Calculate_penalty_money(int day, int hour){ //function to calculate penalty money if overdue
    if (day <=2){
        cout << "Do not overdue the day of return of books\n";
        return 0;
    } else{
        cout << "Overdue the day of return of book !\n";
        cout << "Penalty_money :\n";
        int money=30000*(day-3) + hour*1000;
        cout << money;
        return money;
    }
};
string Student :: get_name(){
    return name;
}
string Student :: get_ID(){
    return ID;
};
string Student :: get_Email(){
    return Email;
}
void Student :: get_information(){
    cout << "Name : " << name <<" Email : " << Email << " Student ID : " << ID << "\n";
}
bool Student :: operator==(const Student & other)const{ 
    return ID == other.ID;
};
vector<pair< information_borrow_book ,Book>> Student ::  get_list_book_borrow(){
    return list_borrowed_book;
}
void Student :: get_list_book_borrow_student(){ //get the list of books that student are borrowing
    if (list_borrowed_book.size()!=0){
        cout << "The list of book which student has student ID " << ID <<" borrow are : \n";
        for(auto book : list_borrowed_book){
            book.second.get_information();
        }
    } else {
        cout <<"The Student has ID " << ID <<" do not borrow any book \n";
    }
    
};

Student :: Student(string name , string Email, string ID){// constructor of student
    this->name=name;
    this-> Email=Email;
    this->ID =ID;
}

// This function will find the location of book in library, change the state of book in the library
void Student :: Borrow_book_student(Library & library,Book book){ //borrow book, change the availability of book, get the time borrow
        // Get the current time
    time_t borrowTime = time(nullptr);
    index_of_location Index= library.Find_the_book_availabel(book.getTitle(), book.getAuthor(), book.getGenre());
    information_borrow_book infor_bor_book;
    infor_bor_book.time =borrowTime;
    infor_bor_book.Index= Index;
    if(Index.index != -1){
        // library.get_ALLBOOK()[Index.genre][Index.title][Index.author][Index.index].Borrow_book_student();
        library.Borrow_in_lib(book);
        pair<information_borrow_book,Book> book1(infor_bor_book, book);
        list_borrowed_book.push_back(book1);
    } else {
        cout <<" You can not borrow the book !\n";
    }
};

// This function will change the state of the book in the library it also delete in the list_borrow_book
void Student:: Return_book_student(Library &  Library, Book book){//return book, change the availability of the book, get the time return
    for(int i=0; i< list_borrowed_book.size(); i++){
        if (list_borrowed_book[i].second == book){
            
            information_borrow_book inf_book=list_borrowed_book[i].first;

            //calculate the borrowing time
            time_t returntime= time(nullptr);
            time_t borrowtime= inf_book.time;
            time_t diffSeconds  =difftime(returntime, borrowtime);
            int diffDays = diffSeconds / (60 * 60 * 24);
            int diffHours = (diffSeconds / (60 * 60)) % 24;

            // Output the difference
            cout << "The time the student borrow : \n";
            cout << "Days: " << diffDays << endl;
            cout << "Hours: " << diffHours << endl;
            Calculate_penalty_money(diffDays, diffHours);
            
            // Turn the book in the library to be availale;
            index_of_location ind_loc= inf_book.Index;
            // Library.get_ALLBOOK()[ind_loc.genre][ind_loc.title][ind_loc.author][ind_loc.index].Return_book();
            Library.Return_in_lib(book);
            list_borrowed_book.erase(list_borrowed_book.begin()+i);
            return;
        }
    }
    cout << " You did not borrow the book \"" << book.getTitle() <<"\"";
} 

/*8888888888888888888888888888888888888888888*/

//-------------------------------------------------------
// define the Manage_student class

void Manage_student:: add_student_borrow_book(Student student){  //add student list of students
    List_student_borrow_book.push_back(student);
};
void Manage_student:: remove_student_borrow_book(Student student1){ //remove student
    for(int i= 0; i <List_student_borrow_book.size(); i++){
        if (student1 == List_student_borrow_book[i]){
            List_student_borrow_book.erase(List_student_borrow_book.begin()+i);
        }        
    }
}; 
void Manage_student:: print_out_all_student_who_borrow_book(){ //print out all the students that borrowing a book
    if(List_student_borrow_book.size()!=0){
        for(Student student : List_student_borrow_book){
            student.get_information();
        }
    } else {
        cout << "There is no student borrow book ! \n";
    }    
}
void Manage_student:: print_out_all_student_with_their_borrow_book(){ //print out all student and their borrow book
    if(List_book_is_related_to_ID.size() >0){
        for (auto pair1 : List_book_is_related_to_ID){
            cout << "Student with ID " << pair1.second << ": " << pair1.first <<"\n";
        }
    } else {
        cout << "There are no student borrow book !";
    }
    
}
void Manage_student:: Find_student_borrow_book(Library& library,Book book){ //find the student that borrow a particular book
    for(int i=0; i<List_book_is_related_to_ID.size(); i++){
        string title =List_book_is_related_to_ID[i].first;
        string ID=List_book_is_related_to_ID[i].second;
        if(title== book.getTitle()){
            cout << "The student who borrow the book \""<< book.getTitle() << "\" is :\n";
            for(int j =0; j < List_student_borrow_book.size(); j++){
                if(ID== List_student_borrow_book[i].get_ID()){
                    List_student_borrow_book[i].get_information();
                    return;
                }
            }
        }
    }
    cout << "No one borrowed this book!\n";
};


void Manage_student:: History_borrow_and_return_book(const string& a) { //record borrow and return in history file
    ofstream file("History.out", ios::app); // Open the file in append mode

    if (file.is_open()) {
        file << a << endl; // Write the string to the file
        file.close(); // Close the file
    } else {
        cout << "Failed to open the file." << endl;
    }
}

string trimSentence1(string sentence) { // trim the sentence
    size_t start = sentence.find_first_not_of(" ");
    size_t end = sentence.find_last_not_of(" ");

    if (start == std::string::npos) {
        // No non-space characters found, return an empty string
        return "";
    }

    return sentence.substr(start, end - start + 1);
};

void Manage_student :: Take_infor_student_book(Library & library ){ //take input
    cout << "Enter your information : \n";
    cout << "Enter your name : ";
    string name;
    cin >> name;
    name=trimSentence1(name);
    cout << "Enter your student ID :";
    string ID;
    cin >> ID;
    ID=trimSentence1(ID);
    cout << "Enter your Email :";
    string Email;
    cin >> Email;
    Email=trimSentence1(Email);
    Student student_ob(name, Email, ID);
    while (true){
        cout << "What do you want to do ?\n";
        cout << " 1 :borrow book ! \n 2 :Return book ! \n 3 :Find the book is borrowed \n 4 :Print out all student borrow book \n 5 :Print out all student with their borrow book \n 6 :Find the book you are borrowing\n 7 :Quit !";
        int check_borrow_return;
        cin >> check_borrow_return;
        
        
        if(check_borrow_return== 1){
            cout << "Which book you want to borrow ?\n";
            string title;
            string author;
            string genre;
            string abstract;
            getline(cin,abstract);
            cout << "The title :\n";
            getline(cin , title);
            cout << "The Author :\n";
            getline(cin,author);
            cout << "The Genre: \n";
            getline(cin , genre);
            if (title.empty() ) title="-1";
            if (author.empty()) author="-1";
            if (genre.empty()) genre ="-1";
            // if(title!="-1"&& author!="-1" && genre !="-1"){
            //     index_of_location inde=library.Find_the_book_availabel(title,author,genre);
            //     if (inde.index==-1)break;
            // }
            vector<Book> List_book_available= library.Find_book_with_special_info(title, author, genre);
            // index_of_location inde=library.Find_the_book_availabel(title,author,genre);
            // if (inde.index==-1)break;
            if( List_book_available.size()!=0){
                int num ;
                cout <<" You want to find which book ? \n ";
                cin >> num;
                while (num >= List_book_available.size()){
                    cout << "Enter the number which is smaller than "<< List_book_available.size() << "\n";
                    cin >> num;
                }
                cout << "You want to borrow the book" ;
                Book book1 =List_book_available[num];
                book1.get_information();
                cout << " 1 : Yes \n 2 : No \n";
                int check_borrow;
                cin >> check_borrow;
                
                if(check_borrow == 1){
                    
                    index_of_location ind =library.Find_the_book_availabel(book1.getTitle(), book1.getAuthor(), book1.getGenre());
                    // check whether the book is available
                    if(ind.index != -1){
                        pair<string,string> book_related_to_id(book1.getTitle(), ID);
                        // cout << "test ID, title" << book1.getTitle() <<" " << ID << "\n";
                        List_book_is_related_to_ID.push_back(book_related_to_id);//List_book_is_related_to_ID; // first is title and second is ID
                        
                        auto it = find(List_student_borrow_book.begin(), List_student_borrow_book.end(), student_ob);

                        if (it == List_student_borrow_book.end()) {
                            student_ob.Borrow_book_student(library,book1 );
                            List_student_borrow_book.push_back(student_ob); 
                        } else {
                            int index = std::distance(List_student_borrow_book.begin(), it);
                            cout << index;
                            List_student_borrow_book[index].Borrow_book_student(library,book1);
                        }
                        
                        // append into the History file
                        // Get the current time
                        time_t currentTime = time(nullptr);
                        
                        // Convert the time to a local time string
                        string currentTimeString = ctime(&currentTime);
                        string A= student_ob.get_name()+" , The student ID is "+ student_ob.get_ID()+
                        "borrow the book :" + book1.getTitle() + "   Time : " + currentTimeString;
                        History_borrow_and_return_book(A);

                        // break;
                        }
                        
                    } 
            }
            
        } else if(check_borrow_return==2){
            // cout << "Which book you want to return :\n";
            for(int i=0; i < List_student_borrow_book.size(); i++){
                if(List_student_borrow_book[i] == student_ob){
                    
                    cout << "Which books you want to return ?\n";
                    for (int j=0; j < List_student_borrow_book[i].get_list_book_borrow().size(); j++){
                        cout <<j <<": ";
                        List_student_borrow_book[i].get_list_book_borrow()[j].second.get_information();
                    }
                    int remove_book;
                    cin >> remove_book;

                    // make sure the useer input the small 
                    if(remove_book>=0){
                        while(remove_book >= List_student_borrow_book[i].get_list_book_borrow().size()){
                            cout <<" Enter the number is smaller than " << List_student_borrow_book[i].get_list_book_borrow().size() <<": ";
                            cin >> remove_book;
                        }
                        // return in the library and delete in the student;
                        Book book1= List_student_borrow_book[i].get_list_book_borrow()[remove_book].second;
                        List_student_borrow_book[i].Return_book_student(library,book1);

                        if (List_student_borrow_book[i].get_list_book_borrow().size()==0){
                            List_student_borrow_book.erase(List_student_borrow_book.begin()+i);

                        }
                        // delete in List_book_is related to ID
                        
                        pair<string, string> remove_title_id(book1.getTitle(), ID);
                        auto removalCondition = [&remove_title_id](const pair<string, string>& element) {
                            return element == remove_title_id;
                        };

                        auto it = std::remove_if(List_book_is_related_to_ID.begin(), List_book_is_related_to_ID.end(), removalCondition);
                        List_book_is_related_to_ID.erase(it, List_book_is_related_to_ID.end());

                
    
                        // Get the current time
                        time_t currentTime = time(nullptr);
                        
                        // Convert the time to a local time string
                        string currentTimeString = ctime(&currentTime);
                        
                        // append into the History file
                        string A= student_ob.get_name()+" , The student ID is "+ student_ob.get_ID()+ "return the book :" + book1.getTitle() + "   Time : "+currentTimeString;
                        History_borrow_and_return_book(A);
                    
                    }   
                    
                    break;
                }
            }
        } else if (check_borrow_return== 3){

            cout << "Which book you want to borrow ?\n";
            string title;
            string author;
            string genre;
            string abstract;
            getline(cin,abstract);
            cout << "The title :\n";
            getline(cin , title);
            cout << "The Author :\n";
            getline(cin,author);
            cout << "The Genre: \n";
            getline(cin , genre);
            if (title.empty() ) title="-1";
            if (author.empty()) author="-1";
            if (genre.empty()) genre ="-1";
            
            vector<Book> List_book_available= library.Find_book_with_special_info(title, author, genre);
            // index_of_location inde=library.Find_the_book_availabel(title,author,genre);
            // if (inde.index==-1)break;
            if( List_book_available.size()!=0){
                int num ;
                cout <<" You want to find which book ? \n ";
                cin >> num;
                while (num >= List_book_available.size()){
                    cout << "Enter the number which is smaller than "<< List_book_available.size() << "\n";
                    cin >> num;
                }
                cout << "You want to find Student who borrow the book?" ;
                Book book1 =List_book_available[num];
                book1.get_information();
                cout << " 1 : Yes \n 2 : No \n";
                int check_borrow;
                cin >> check_borrow;
                
                if(check_borrow == 1){
                    Find_student_borrow_book(library, book1);    
                    } 
            }

        } else if ( check_borrow_return==4){
            print_out_all_student_who_borrow_book();
        } else if (check_borrow_return ==5){
            print_out_all_student_with_their_borrow_book();
        } else if(check_borrow_return== 6){
            for(int i=0; i < List_student_borrow_book.size(); i++){
                if(List_student_borrow_book[i] == student_ob){
                    if (List_student_borrow_book[i].get_list_book_borrow().size()!=0){
                        List_student_borrow_book[i].get_list_book_borrow_student();
                        
                    } else {
                        cout << "You do not borrow any book !\n";
                    }
                    // List_student_borrow_book[i].get_list_book_borrow_student();
                    break;
                }
            }
            cout <<" You do not borrow any book!\n";
        } 
        else{
            break;
        };
    };
        
}
