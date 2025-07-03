#include<iostream>
#include<string>
#include<fstream>
#include<ctime>
using namespace std;
//base class for user information
class User{
	public:
		string name, cnic, password, confirm_password, phone_number;
		//default constructor
		User(){
			name="";
			cnic="";
			phone_number="";
			password="";
			confirm_password="";
		}
		
		//function for getting user information
		void get_inputs(){
            cout<<"\t\t\t\t\t\t\t\tENTER DETAILS FOR YOUR E-WALLET AACCOUNT\t\t\t"<<endl;
            cout<<"\t\t\t\t\t\t\t\t========================================\n\n\n\n"<<endl;
			cout<<"\t~| Enter Your Name: ";
			getline(cin, name);
			cout<<"\t~| Enter Your CNIC: ";
			getline(cin, cnic);
			cout<<"\t~| Enter Your Phone Number: ";
			getline(cin,phone_number);
			cout<<"\t~| Enter Your Password: ";
			getline(cin, password);
			cout<<"\t~| Confirm Your Password: ";
			getline(cin, confirm_password);
			if(confirm_password==password){
				cout<<"\t\t\t\t\t\t\t\tCONGRATULATION YOUR ACCOUNT HAS BEEN CREATED"<<endl;
				cout<<"\t\t\t\t\t\t\t\t============================================\n\n\n\n"<<endl;
			}
			else{
				cout<<"\tPassword Do Not Match.Exiting."<<endl;
				exit(1);
			}
		}		
};


//derived "Account" class that inherited from base class "User"
class Account:public User{
	public:
		string id;
		double balance;
		//constructor
		Account():User(){
			id="A1001";
			balance=0.0;
			}	
		
		//function for password
		bool verify_password()const{
			string enteredpassword;
			cout<<"\n\t\t\tEnter Your Password For Security Checking: ";
			getline(cin,enteredpassword);
			return password==enteredpassword;	
		}
		
		//function for balance
       void input_balance(){
			string for_continue;
			cout<<"\n\tEnter The Amount You Want To Add: ";
			cin>>balance;
			cin.ignore();
            cout<<"\n\n\t\t\t\t\t\t\t\t"<<balance<<" Has Been Added To Your Account"<<endl;
		    cout<<"\t\t\t\t\t\t\t\t======================================"<<endl;
			cout<<"\n\tPress Any Key To Continue: ";
			cin>>for_continue;

		}

		//function for checking balance
		bool deduct_balance(double amount){
			if(balance>=amount){
				balance-=amount;
				return true;
			}
			return false;
		}
	void display_user(){
    cout<<"\n\t~| User's Name: "<<name<<endl;
    cout<<"\n\t~| User's CNIC: "<<cnic<<endl;
    cout<<"\n\t~| User's Phone Number: "<<phone_number<<endl;
    cout<<"\n\t~| User's ID: " <<id<<endl;
    cout<<"\n\t~| User's balance: "<<balance<<endl;
}	
};
//abstract class for services in this application
class Transaction{
	public:
		virtual void display_details(Account& acc)=0;
		void log_transaction(const string& log){
		ofstream file("transaction_history.txt", ios::app);
			if(file.is_open()){
				time_t now=time(0);
				tm* ltm=localtime(&now);
				file<<(ltm->tm_year+1900)<<"-"<<(ltm->tm_mon+1)<<"-"<<ltm->tm_mday<<" "<<ltm->tm_hour<<":"<<
				ltm->tm_min<<":"<<ltm->tm_sec<<" - "<<log<<endl;
				file.close();
			}
		}
		static void read_history(){
			ifstream file("transaction_history.txt");
			if(file.is_open()){
			string line;
			  while(getline(file,line)){
			  	cout<<"\t\t\t"<<line<<endl;
			  }
			  file.close();
			}
			else{
				cout<<"\n\t\t\tNo Transaction History found."<<endl;
			}
		}
		//virtual ~Transaction()=default;
};



//derived class "Send Money" for handling money transfer
class SendMoney:public Transaction{
	public:
		string receiver_name, receiver_phone;
		double amount=0.0;
		
		//function overriding
		void display_details(Account& acc)override{
			cout<<"\n\tSending Money To: "<<receiver_name<<" ("<<receiver_phone<<") "<<endl;
		}
		
		//function for handle the working for money transfer
		void handle_send_money(Account &acc) {
			display_details(acc);
			string answer;
			cout<<"\n\t\t\tDo You Want To Send Money? (yes/no): ";
			getline(cin,answer);
			if(answer=="yes"||answer=="YES"||answer=="Yes"){
			if(acc.verify_password()){
				cout<<"\n\t\t\tEnter The Amount You Want To Send: ";
				cin>>amount;
				cin.ignore();
				if(acc.deduct_balance(amount)){
	    cout<<"\n\n\n\n\t\t\t\t\t\t\t\t====================================================" << endl;
				cout<<"\t\t\t\t\t\t\t\tTransaction Successful. Remaining Balance: "<<acc.balance<<endl;
				cout<<"\t\t\t\t\t\t\t\t====================================================\n\n\n\n" << endl;
				log_transaction("Send Money: "+receiver_name+" ("+receiver_phone+") Amount: "+ to_string(amount));
			    }
			    else{
		cout<<"\n\n\n\n\t\t\t\t\t\t\t\t==============================================" << endl;
				cout<<"\t\t\t\t\t\t\t\tTransaction Failed Due To Insufficient Balance "<<endl;
				cout<<"\t\t\t\t\t\t\t\t==============================================\n\n\n\n" << endl;
			    }
	    	}
	    	else{
	    cout<<"\n\n\n\n\t\t\t\t\t\t\t\t============================================" << endl;
	    		cout<<"\t\t\t\t\t\t\t\tTransaction Failed Due To Incorrect Password"<<endl;
	    		cout<<"\t\t\t\t\t\t\t\t============================================\n\n\n\n" << endl;
			}
		}
		else{
cout<<"\n\n\n\n\t\t\t\t\t\t\t\t======================="<<endl;
	    cout<<"\t\t\t\t\t\t\t\tMoney Transfer Canceled"<<endl;
		cout<<"\t\t\t\t\t\t\t\t======================="<<endl;
			}
        }
};

//derived class "Bill Payment" for handling payment of bill
class BillPayment:public Transaction{
	public:
		int bill_type;	
	    double amount;
	    string bill_name;
	    
	    //function overriding 
	    void display_details(Account& acc)override{
	    	switch(bill_type){
	    		case 1:
	    			bill_name="LESCO Electricity.";
	    			amount=1000.0;
	    			break;
	    		case 2:
	    			bill_name="SUI Gas";
	    			amount=700.0;
	    			break;
	    		case 3:
	    			bill_name="Water";
	    			amount=530.0;
	    			break;
	    		default:
	    			cout<<"\tInvalid Bill Type.\n";
	    			return;
			}
			
			cout<<"\n\n\n\t\t\tPaying The Bill Of "<<bill_name<<endl;
			cout<<"\n\t\t\tYour Bill Amount is: "<<amount<<endl;
		}
		
		//function for handle the working for bill payment
		void handle_bill_payment(Account& acc){
			display_details(acc);
			string answer;
			cout<<"\n\t\t\tDo You Want To Pay Your Bill?(yes/no): ";
			getline(cin,answer);
			
			if(answer=="yes"||answer=="YES"||answer=="Yes"){
				if(acc.verify_password()){
					if(acc.deduct_balance(amount)){
                        cout<<"\n\n\n\n\t\t\t\t\t\t\t\t=============================="<<endl;
					            cout<<"\t\t\t\t\t\t\t\t    Bill Paid Succesfully"<<endl;
					            cout<<"\t\t\t\t\t\t\t\tRemaining Balance Is: "<<acc.balance<<endl;
                                cout<<"\t\t\t\t\t\t\t\t=============================="<<endl;
                                log_transaction("Bill Payment: "+bill_name+ "Amount: "+to_string(amount));
					}
					else{
                        cout<<"\n\n\n\n\t\t\t\t\t\t\t\t====================" << endl;
						        cout<<"\t\t\t\t\t\t\t\tInsufficient Balance"<<endl;
	                            cout<<"\t\t\t\t\t\t\t\t====================" << endl;
					}
				}
				else{
                        cout<<"\n\n\n\n\t\t\t\t\t\t\t\t======================================="<<endl;
					            cout<<"\t\t\t\t\t\t\t\tIncorrect Password. Bill Payment Failed"<<endl;
		                        cout<<"\t\t\t\t\t\t\t\t======================================="<<endl;
				}
			}
			else{
            cout<<"\n\n\n\n\t\t\t\t\t\t\t\t====================="<<endl;
				    cout<<"\t\t\t\t\t\t\t\tBill Paymeny Canceled"<<endl;
		            cout<<"\t\t\t\t\t\t\t\t====================="<<endl;
			}
		}	
};

//derived class "Mobile Package" for handling Mobile package
class MobilePackage:public Transaction{
	public:
		int package_type;
		double amount;
		string package_name,package_details; 
		
		//function overriding
		void display_details(Account& acc)override{
			switch(package_type){
				case 1:
					package_name="Day Package: ";
					package_details="50 Mins, 200 SMS, 3 GB Data (WA, IMO, YouTube)";
					amount=430.0;
					break;
				case 2:
					package_name="Weekly Package: ";
					package_details="500 Mins, 2000 SMS, 10 GB Data (WA, IMO, YouTube)";
					amount=730.0;
					break;
				case 3:
					package_name="Monthly Package: ";
					package_details="1000 Mins, 5000 SMS, 30 GB Data (WA, IMO, YouTube)";
					amount=1130.0;
					break;
				default:
					cout<<"\tInvalid Package Type.\n";
					return;
			}
			
			cout<<"\n\n\n\t\t\t"<<package_name<<"The Package includes "<<package_details<<endl;
			cout<<"\n\t\t\tPackage Amount is : "<<amount<<endl;
		}
		
		//function for handle the working for Mobile package
		void handle_mobile_package(Account& acc){
			display_details(acc);
			string answer;
			cout<<"\n\t\t\tDo You Want To Get this Package?(yes/no): ";
			getline(cin,answer);
			if(answer=="yes"||answer=="YES"||answer=="Yes"){
				if(acc.verify_password()){
					if(acc.deduct_balance(amount)){
						cout<<"\n\n\n\n\t\t\t\t\t\t\t\t============================="<<endl;
						        cout<<"\t\t\t\t\t\t\t\t Package Payment Successfully"<<endl;
						        cout<<"\t\t\t\t\t\t\t\tRemaining Balance Is: "<<acc.balance<<endl;
						        cout<<"\t\t\t\t\t\t\t\t============================="<<endl;
						        log_transaction("Mobile Package: "+package_name+ "Amount: "+to_string(amount));
					}
					else{
						cout<<"\n\n\n\n\t\t\t\t\t\t\t\t===================="<<endl;
						        cout<<"\t\t\t\t\t\t\t\tInsufficient Balance"<<endl;
						        cout<<"\t\t\t\t\t\t\t\t===================="<<endl;
					}
				}
				else{
					cout<<"\n\n\n\n\t\t\t\t\t\t\t\t========================================="<<endl;
					        cout<<"\t\t\t\t\t\t\t\tIncorrect Password.Package Payment Failed"<<endl;
					        cout<<"\t\t\t\t\t\t\t\t========================================="<<endl;
				}
			}
			else{
				cout<<"\n\n\n\n\t\t\t\t\t\t\t\t========================"<<endl;
				        cout<<"\t\t\t\t\t\t\t\tPackage Payment Canceled"<<endl;
				        cout<<"\t\t\t\t\t\t\t\t========================"<<endl;
			}
		}	
};

class AddingBalance: public Transaction{
	public:
	 
         void display_details(Account& acc)override{
	if(acc.verify_password()){
			string answer;
			cout<<"\n\t\t\tDo You Want To Add More Amount?(yes/no): ";
			getline(cin,answer);
			if(answer=="yes"||answer=="YES"||answer=="Yes"){		
            double new_amount;
			cout<<"\n\t\t\tEnter the Amount You Want to Add in Your Account: ";
			cin>>new_amount;
			acc.balance+=new_amount;
			cout<<"\n\n\n\n\t\t\t\t\t\t\t\t============================"<<endl;
					cout<<"\t\t\t\t\t\t\t\tYour New Balance is: "<<acc.balance<<endl;
					cout<<"\t\t\t\t\t\t\t\t============================"<<endl;	
		    }
	        else{  
		    cout<<"\n\n\n\n\t\t\t\t\t\t\t\t============================"<<endl;
					cout<<"\t\t\t\t\t\t\t\tYour Balance is: "<<acc.balance<<endl;
					cout<<"\t\t\t\t\t\t\t\t============================"<<endl;	
        	}
    }
    else{
		cout<<"\n\n\n\n\t\t\t\t\t\t\t\t======================================="<<endl;
			    cout<<"\t\t\t\t\t\t\t\tIncorrect Password.Adding Amount Failed"<<endl;
				cout<<"\t\t\t\t\t\t\t\t======================================="<<endl;
	}
}
};

int main(){
    cout<<"\n\n\n\n\t\t\t\t\t\t\t\t==================================="<<endl;
    cout<<"\t\t\t\t\t\t\t\tWELCOME TO THE E-WALLET APPLICATION"<<endl;
    cout<<"\t\t\t\t\t\t\t\t===================================\n\n\n\n"<<endl;
    
    string for_continue;
    
    //calling of functions
	Account acc;
	acc.get_inputs();
	acc.input_balance();
	
	//creation of objects
	SendMoney sendmon;
	BillPayment bilpay;
	MobilePackage mblpkg;
	AddingBalance addbaln;
	
	int choice;
	do{
	  system("CLS");
	  cout<<"\n\t\t\t\t\t\t\t\t===================================="<<endl;
		cout<<"\t\t\t\t\t\t\t\tLet's Enjoy The E-WALLET Application"<<endl;
	 	cout<<"\t\t\t\t\t\t\t\t===================================="<<endl;
	 	cout<<"\n\n\n\n\n\n\n\t\t\t\t\t\t* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *"<<endl;
              cout<<"\t\t\t\t\t\t|\t\t\t\tMAIN MENU\t\t\t\t|" << endl;
              cout<<"\t\t\t\t\t\t* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\n"<<endl;
              cout<<"\t\t\t\t\t\t|\t\t\t1 ~| Send Money                                 |\n"<<endl;
              cout<<"\t\t\t\t\t\t|\t\t\t2 ~| Pay Bills                                  |\n"<<endl;
              cout<<"\t\t\t\t\t\t|\t\t\t3 ~| Buy Mobile Package                         |\n"<<endl;
              cout<<"\t\t\t\t\t\t|\t\t\t4 ~| Display Account Details                    |\n"<<endl;
              cout<<"\t\t\t\t\t\t|\t\t\t5 ~| Add Amount                                 |\n"<<endl;
              cout<<"\t\t\t\t\t\t|\t\t\t6 ~| Display Transaction History                |\n"<<endl;
              cout<<"\t\t\t\t\t\t|\t\t\t7 ~| Exit                                       |\n"<<endl;
              cout<<"\t\t\t\t\t\t* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\n"<<endl;
              cout<<"\t\t\t\t\t\t\t\t~| Enter your choice from(1 to 6): ";
		cin>>choice;
		cin.ignore();
		switch(choice){
			case 1:
			//for sending money
				system("CLS");
			cout<<"\n\n\t\t\t\t\t\t\t\t=================================="<<endl;
				cout<<"\t\t\t\t\t\t\t\tE-WALLET Money Transaction Service"<<endl;
		        cout<<"\t\t\t\t\t\t\t\t==================================\n\n"<<endl;
				cout<<"\n\t~| Enter The Name Of Receiver: ";
				getline(cin,sendmon.receiver_name);
				cout<<"\n\t~| Enter The Phone Number Of Receiver: ";
				getline(cin, sendmon.receiver_phone);
				sendmon.handle_send_money(acc);
				cout<<"\n\tPress Any Key To Continue: ";
				cin>>for_continue;
				break;
			
			case 2:
			//for bill payment
				system("CLS");
			cout<<"\n\n\t\t\t\t\t\t\t\t============================="<<endl;
				cout<<"\t\t\t\t\t\t\t\tE-WALLET Bill Payment Service"<<endl;
		        cout<<"\t\t\t\t\t\t\t\t=============================\n\n"<<endl;
				cout<<"\n\t~| Choose bill Type To Pay: "<<endl;
				cout<<"\n\t~| 1. Electricity"<<endl;
				cout<<"\n\t~| 2. Gas"<<endl;
				cout<<"\n\t~| 3. Water"<<endl;
				cout<<"\n\t~| Enter Your Choice( from 1 to 3): ";
				cin>>bilpay.bill_type;
				cin.ignore();
				if(bilpay.bill_type==1||bilpay.bill_type==2||bilpay.bill_type==3){
				bilpay.handle_bill_payment(acc);
			}
			else{
				cout<<"\nInvalid choose. Try from 1 to 3"<<endl;
			}
				cout<<"\n\tPress Any Key To Continue: ";
				cin>>for_continue;
				
				break;
			
			case 3:
			//for mobile package
				system("CLS");
			cout<<"\n\n\t\t\t\t\t\t\t\t================================"<<endl;
				cout<<"\t\t\t\t\t\t\t\tE-WALLET Mobile Pacakges Service"<<endl;
		        cout<<"\t\t\t\t\t\t\t\t================================\n\n"<<endl;
				cout<<"\n\t~| Choose Mobile Package Type To Buy: "<<endl;
				cout<<"\n\t~| 1. Day Package"<<endl;
				cout<<"\n\t~| 2. Weekly Package"<<endl;
				cout<<"\n\t~| 3. Monthly Package"<<endl;
				cout<<"\n\t~| Enter Your Choice:(1 to 3)";
				cin>>mblpkg.package_type;
				cin.ignore();
				if(mblpkg.package_type==1||mblpkg.package_type==2||mblpkg.package_type==3){
					mblpkg.handle_mobile_package(acc);
				}
				else{
				cout<<"\nInvalid choose. Try from 1 to 3"<<endl;
		     	}
				cout<<"\n\tPress Any Key To Continue: ";
				cin>>for_continue;
				break;
				
			case 4:
			//for the details of user
				system("CLS");
			cout<<"\n\n\t\t\t\t\t\t\t\t======================="<<endl;
				cout<<"\t\t\t\t\t\t\t\tE-WALLET User's Account"<<endl;
		        cout<<"\t\t\t\t\t\t\t\t=======================\n\n"<<endl;
		        if(acc.verify_password()){
				acc.display_user();
			    }
			    else{
					cout<<"\n\n\n\n\t\t\t\t\t\t\t\t====================================="<<endl;
					        cout<<"\t\t\t\t\t\t\t\tIncorrect Password.Can't Show Details"<<endl;
					        cout<<"\t\t\t\t\t\t\t\t====================================="<<endl;
				}
			
				cout<<"\n\tPress Any Key To Continue: ";
				cin>>for_continue;
				break;
				
			case 5:
			//for adding amount in account
				system("CLS");
			cout<<"\n\n\t\t\t\t\t\t\t\t=============================="<<endl;
				cout<<"\t\t\t\t\t\t\t\tE-WALLET Adding Amount Service"<<endl;
		        cout<<"\t\t\t\t\t\t\t\t==============================\n\n"<<endl;
		        addbaln.display_details(acc);
				cout<<"\n\tPress Any Key To Continue: ";
				cin>>for_continue;
				break;
				
			case 6:
			system("CLS");	
			  cout<<"\n\n\t\t\t==================="<<endl;
			cout<<"  \t\t\tTransaction History"<<endl;
			  cout<<"\t\t\t==================="<<endl;
				Transaction::read_history();
				cout<<"\n\tPress Any Key To Continue: ";
				cin>>for_continue;
				break;	
							
			case 7:
			//for exiting from application
				system("CLS");
				cout<<"\n\n\t\t\t\t================================================"<<endl;
				    cout<<"\t\t\t\tThank You So Much For Using E-WALLET Application"<<endl;
				    cout<<"\t\t\t\t================================================\n\n"<<endl;
				break;
				
			//default
			default:
				cout<<"\tInvalid Choice. Please Try Again.\n";
				break;
	    }
	}while(choice!=7);
		return 0;
}
