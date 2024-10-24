#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>

using namespace std;

const string ClientsFileName = "Clients.txt";

enum enMainMenueOptions { eListClients = 1, eAddNewClient = 2, eDeleteClient = 3, eUpdateClient = 4, eFindClient = 5,eTransactions = 6 , eExit = 7 };
enum enTransactionOptions { eDeposit = 1, eWithDraw = 2, eTotalBalances = 3, eBackToMainMenue = 4};

struct sClient
{
	string AccountNumber;
	string PinCode;
	string Name;
	string Phone;
	double AccountBalance;
	bool MarkForDelete = false;
};

void ShowMainMenu();

void ShowTransactionsScreen();

string ReadClientAccountNumber()
{
	string AccountNumber;

	cout << "Please Enter Your Account Number ? ";
	cin >> AccountNumber;

	return AccountNumber;
}

vector <string> SplitString(string S1, string Delim)
{
	vector <string> vString;

	short Pos = 0;
	string sWord;

	while ((Pos = S1.find(Delim)) != string::npos)
	{
		sWord = S1.substr(0, Pos);
		if (sWord != " ")
		{
			vString.push_back(sWord);
		}
		S1.erase(0, Pos + Delim.length());
	}

	if (S1 != "")
	{
		vString.push_back(S1);
	}

	return vString;
}

sClient ConvertLineToRecord(string S1, string Seperator = "#//#")
{
	sClient Client;

	vector <string> vClientData = SplitString(S1, Seperator);

	Client.AccountNumber = vClientData[0];
	Client.PinCode = vClientData[1];
	Client.Name = vClientData[2];
	Client.Phone = vClientData[3];
	Client.AccountBalance = stod(vClientData[4]);

	return Client;

}

void PrintClientRecord(sClient Client)
{


	cout << "| " << left << setw(15) << Client.AccountNumber;
	cout << "| " << left << setw(10) << Client.PinCode;
	cout << "| " << left << setw(40) << Client.Name;
	cout << "| " << left << setw(12) << Client.Phone;
	cout << "| " << left << setw(12) << Client.AccountBalance;

}

void PrintAllClientsData(vector <sClient> vClients)
{
	cout << "\n\t\t\t\t Client List (" << vClients.size() << ") Client (s)\n";
	cout << "\n_____________________________________________________";
	cout << "___________________________________________\n\n";

	cout << "| " << left << setw(15) << "AccountNumber";
	cout << "| " << left << setw(10) << "PinCode";
	cout << "| " << left << setw(40) << "Name";
	cout << "| " << left << setw(12) << "Phone";
	cout << "| " << left << setw(12) << "Balance";


	cout << "\n_____________________________________________________";
	cout << "___________________________________________\n\n";

	if (vClients.size() == 0)
		cout << "\tNo Clients Availabale in the system!\n";
	else
	{
		for (sClient& Client : vClients)
		{
			PrintClientRecord(Client);
			cout << endl;
		}
	}

	cout << "\n_____________________________________________________";
	cout << "___________________________________________\n\n";
}

vector <sClient> LoadClientsDataFromFile(string FileName)
{
	fstream MyFile;
	vector <sClient> vClients;

	MyFile.open(FileName, ios::in);

	if (MyFile.is_open())
	{
		string Line;
		sClient Client;

		while (getline(MyFile, Line))
		{

			Client = ConvertLineToRecord(Line);

			vClients.push_back(Client);
		}
		MyFile.close();
	}

	return vClients;
}

bool isTheClientExistByAccountNumber(string AccountNumber, string FileName)
{

	vector <sClient> vClients;
	fstream MyFile;
	MyFile.open(FileName, ios::in);//read Mode
	if (MyFile.is_open())
	{
		string Line;
		sClient Client;
		while (getline(MyFile, Line))
		{
			Client = ConvertLineToRecord(Line);
			if (Client.AccountNumber == AccountNumber)
			{
				MyFile.close();
				return true;
			}
			vClients.push_back(Client);
		}
		MyFile.close();
	}
	return false;
}

sClient ReadNewClient()
{
	sClient Client;

	cout << "Enter Account Number ? ";
	getline(cin >> ws, Client.AccountNumber);

	while (isTheClientExistByAccountNumber(Client.AccountNumber, ClientsFileName))
	{
		cout << "\nClient With (" << Client.AccountNumber << ") is AllReady Exists, Enter Account Number? ";
		getline(cin >> ws, Client.AccountNumber);
	}

	cout << "Enter PinCode ? ";
	getline(cin, Client.PinCode);

	cout << "Enter Name ? ";
	getline(cin, Client.Name);

	cout << "Enter Phone ? ";
	getline(cin, Client.Phone);

	cout << "Enter AccountBalance ? ";
	cin >> Client.AccountBalance;

	return Client;
}

string ConvertRecordToLine(sClient Client, string Seperator = "#//#")
{
	string sClientRecord = "";

	sClientRecord += Client.AccountNumber + Seperator;
	sClientRecord += Client.PinCode + Seperator;
	sClientRecord += Client.Name + Seperator;
	sClientRecord += Client.Phone + Seperator;
	sClientRecord += to_string(Client.AccountBalance);

	return sClientRecord;
}

void AddDataLineToFile(string FileName, string DataLine)
{
	fstream MyFile;

	MyFile.open(FileName, ios::out | ios::app);

	if (MyFile.is_open())
	{
		MyFile << DataLine << endl;

		MyFile.close();
	}
}

void AddNewClient()
{
	sClient Client = ReadNewClient();

	AddDataLineToFile(ClientsFileName, ConvertRecordToLine(Client));

}

void AddClients()
{
	char AddMore = 'Y';

	do
	{
		system("cls");
		cout << "Adding Client : \n\n";

		AddNewClient();

		cout << "Client Added Successfully , do you want add more client ? Y/N ? ";
		cin >> AddMore;

	} while (toupper(AddMore) == 'Y');
}

vector <sClient> SaveClientsDataToFile(string FileName, vector <sClient>& vClients)
{
	fstream MyFile;

	MyFile.open(FileName, ios::out);

	if (MyFile.is_open())
	{
		string Line;

		for (sClient& C : vClients)
		{

			if (C.MarkForDelete == false)
			{
				Line = ConvertRecordToLine(C);

				MyFile << Line << endl;
			}

		}
	}
	return vClients;
}

sClient ChangeClientRecord(string AccountNumber)
{
	sClient ClientInfo;

	ClientInfo.AccountNumber = AccountNumber;

	cout << "Enter PinCode ? ";
	getline(cin >> ws, ClientInfo.PinCode);

	cout << "Enter Name ? ";
	getline(cin, ClientInfo.Name);

	cout << "Enter Phone ? ";
	getline(cin, ClientInfo.Phone);

	cout << "Enter AccountBalance ? ";
	cin >> ClientInfo.AccountBalance;

	return ClientInfo;
}

bool FindClientByAccountNumber(string AccountNumber, vector <sClient> &vClients,  sClient& Client)
{

	for (sClient& C : vClients)
	{
		if (C.AccountNumber == AccountNumber)
		{
			Client = C;
			return true;
		}
	}
	return false;
}

void PrintClientCard(sClient Client)
{
	cout << "\n\nThe following are the Client details :\n\n";
	cout << "Account Number  : " << Client.AccountNumber << endl;
	cout << "Pin Code        : " << Client.PinCode << endl;
	cout << "Name            : " << Client.Name << endl;
	cout << "Phone           : " << Client.Phone << endl;
	cout << "Account Balance : " << Client.AccountBalance << endl;
}

void PrintClientRecordBalanceLine(sClient Client)
{
	cout << "| " << left << setw(15) << Client.AccountNumber;
	cout << "| " << left << setw(40) << Client.Name;
	cout << "| " << left << setw(12) << Client.AccountBalance;
}

bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{
	for (sClient& C : vClients)
	{
		if (C.AccountNumber == AccountNumber)
		{
			C.MarkForDelete = true;
			return true;
		}
	}
	return false;
}

bool DeleteClientByAccountNumber(string AccountNumber, vector <sClient> vClients)
{
	vClients = LoadClientsDataFromFile(ClientsFileName);
	sClient Client;
	char Answer = 'Y';

	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		PrintClientCard(Client);

		cout << "Do You want to Delete this Client ? Y/N ?";
		cin >> Answer;

		if (Answer == 'Y' || Answer == 'y')
		{
			MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
			SaveClientsDataToFile(ClientsFileName, vClients);

			vClients = LoadClientsDataFromFile(ClientsFileName);

			cout << "\nDeleted Seccessfully";
			return true;

		}
	}
	else
	{
		cout << "\n\nClient With Account Number (" << AccountNumber << ") Not Found !";
		return false;
	}
}

bool UpdateClientByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{
	sClient Client;
	char Answer = 'Y';

	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		PrintClientCard(Client);

		cout << "Do You Want to Update This Client ? Y/N ?";
		cin >> Answer;

		if (Answer == 'Y' || Answer == 'y')
		{

			for (sClient& C : vClients)
			{
				if (C.AccountNumber == AccountNumber)
				{

					C = ChangeClientRecord(AccountNumber);

					break;
				}
			}

			SaveClientsDataToFile(ClientsFileName, vClients);

			vClients = LoadClientsDataFromFile(ClientsFileName);

			cout << "\nUpdated Seccessfully";
			return true;
		}
	}


	else
	{
		cout << "\n\nClient With Account Number (" << AccountNumber << ") Not Found !";
		return false;
	}
}

void ShowFindClientScreen()

{
	cout << "\n---------------------------------\n";
	cout << "\tFind Client Screen";
	cout << "\n---------------------------------\n\n";

	vector <sClient> vClients;
	string AccountNumber = ReadClientAccountNumber();
	sClient Client;

	if(FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		PrintClientCard(Client);
	}
	else
	{
		cout << "\n\nClient With Account Number (" << AccountNumber << ") Not Found !";
	}
}

void ShowDeleteClientScreen()
{
	cout << "\n---------------------------------\n";
	cout << "\tDelete Client Screen";
	cout << "\n---------------------------------\n\n";

	string AccountNumber = ReadClientAccountNumber();
	vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);

	DeleteClientByAccountNumber(AccountNumber, vClients);
}

void ShowUpdateClientScreen()
{
	cout << "\n---------------------------------\n";
	cout << "\tUpdate Client info Screen";
	cout << "\n---------------------------------\n\n";

	string AccountNumber = ReadClientAccountNumber();
	vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);

	UpdateClientByAccountNumber(AccountNumber, vClients);
}

void ShowEndScreen()
{
	cout << "\n----------------------------\n";
	cout << "\tProgram End :-)\n";
	cout << "----------------------------\n";
}

void ShowPrintClienstScreen()
{
	vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);

	PrintAllClientsData(vClients);
}

void ShowAddNewClientScreen()
{
	cout << "\n---------------------------------\n";
	cout << "\tAdd New Clients Screen";
	cout << "\n---------------------------------\n\n";

	AddClients();
}

void GoBackToMainMenue() {
	cout << "\nPress any key to go back to the main menu...";
	system("pause>0");

	ShowMainMenu();
}

enMainMenueOptions ReadMainMenueOption()
{
	short Choose = 0;

	do
	{
		cout << "Choose What do you want to do ? [1 to 7] ?";
		cin >> Choose;

	} while (Choose <= 0 || Choose >= 8);

	return (enMainMenueOptions)Choose;
}

enTransactionOptions ReadTransactionOptions()
{
	short Choose = 0;

	do
	{
		cout << "Choose What do you want to do ? [1 to 4] ?";
		cin >> Choose;

	} while (Choose <= 0 || Choose >= 5);

	return (enTransactionOptions)Choose;
}

void PreformMainMenueOption(enMainMenueOptions MainMenueOptions) {

	switch (MainMenueOptions) {
	case enMainMenueOptions::eListClients:
		system("cls");
		ShowPrintClienstScreen();
		GoBackToMainMenue();
		break;
	case enMainMenueOptions::eAddNewClient:
		system("cls");
		ShowAddNewClientScreen();
		GoBackToMainMenue();
		break;
	case enMainMenueOptions::eDeleteClient:
		system("cls");
		ShowDeleteClientScreen();
		GoBackToMainMenue();
		break;
	case enMainMenueOptions::eUpdateClient:
		system("cls");
		ShowUpdateClientScreen();
		GoBackToMainMenue();
		break;
	case enMainMenueOptions::eFindClient:
		system("cls");
		ShowFindClientScreen();
		GoBackToMainMenue();
		break;
	case enMainMenueOptions::eTransactions:
		system("cls");
		ShowTransactionsScreen();
		GoBackToMainMenue();
		break;
	case enMainMenueOptions::eExit:
		system("cls");
		ShowEndScreen();
		break;
	}
}

void ShowMainMenu()
{
	system("cls");
	cout << "=====================================\n";
	cout << "\t  Main Menue Screen\n";
	cout << "=====================================\n";
	cout << "[1] Show Client List." << endl;
	cout << "[2] Add New Client." << endl;
	cout << "[3] Delete Client." << endl;
	cout << "[4] Update Client Info." << endl;
	cout << "[5] Find Client." << endl;
	cout << "[6] Transactions." << endl;
	cout << "[7] Exit." << endl;
	cout << "=====================================\n";
	PreformMainMenueOption(ReadMainMenueOption());
}

double ReadWithDrawAmount(string Message)
{
	double WithDrawAmount = 0;

	cout << Message ;
	cin >> WithDrawAmount;

	return WithDrawAmount;
}

bool DepositBalancetoClientByAccountNumber(string AccountNumber, double Amount, vector < sClient >& vClients)
{
	char Answer = 'Y';

	cout << "\nAre you sure you want preform this transaction? Y/N? ";
	cin >> Answer;

	if (toupper(Answer) == 'Y')
	{

		for (sClient& C : vClients)
		{
			if (C.AccountNumber == AccountNumber)
			{
				C.AccountBalance += Amount;
				SaveClientsDataToFile(ClientsFileName, vClients);
				cout << "\nDone Successfully. New Balance is : " << C.AccountBalance << endl;

				return true;
			}
		}
	}

	return false;

	
}

bool WithdrawBalancetoClientByAccountNumber(string AccountNumber, double Amount, vector < sClient >& vClients)
{
	char Answer = 'Y';

	cout << "\nAre you sure you want preform this transaction? Y/N? ";
	cin >> Answer;

	if (toupper(Answer) == 'Y')
	{

		for (sClient& C : vClients)
		{
			if (C.AccountNumber == AccountNumber)
			{
				C.AccountBalance += Amount;
				cout << "\nDone Successfully. Your Balance is: " << C.AccountBalance << endl;
				SaveClientsDataToFile(ClientsFileName, vClients);
				return true;
			}
		}
	}
	
	return false;
}

void ShowDepositScreen()
{
	cout << "\n==============================\n";
	cout << "\tDeposit Screen\n";
	cout << "==============================\n";

	sClient Client;

	vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();

	while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		cout << "\nClient With [" << AccountNumber << "] Does Not Exist.\n\n";
		AccountNumber = ReadClientAccountNumber();
	}

	PrintClientCard(Client);

	double Amount = 0;
	cout << "\nPlease Enter Deposit Amount? ";
	cin >> Amount;

	DepositBalancetoClientByAccountNumber(AccountNumber, Amount, vClients);


}

void ShowWithdrawScreen()
{

	cout << "\n==============================\n";
	cout << "\tWithDraw Screen\n";
	cout << "==============================\n";


	sClient Client;

	vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();

	while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		cout << "\nClient With [" << AccountNumber << "] Does Not Exist.\n\n";
		AccountNumber = ReadClientAccountNumber();
	}

	PrintClientCard(Client);

	double Amount = 0;
	cout << "\nPlease Enter Withdraw Amount? ";
	cin >> Amount;

	while (Amount > Client.AccountBalance)
	{
		cout << "\nAmount Exceeds the balance, you can withdraw up to : " << Client.AccountBalance << endl;
		cout << "Please enter another amount? ";
		cin >> Amount;
	}

	WithdrawBalancetoClientByAccountNumber(AccountNumber, Amount * -1, vClients);

}

void TotalBalances()
{
	vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
	double TotalBalances = 0;

	cout << "\n\t\t\t\t Client List (" << vClients.size() << ") Client (s)\n";
	cout << "\n_____________________________________________________";
	cout << "___________________________________________\n\n";

	cout << "| " << left << setw(15) << "AccountNumber";
	cout << "| " << left << setw(40) << "Name";
	cout << "| " << left << setw(12) << "Balance";


	cout << "\n_____________________________________________________";
	cout << "___________________________________________\n\n";

	if (vClients.size() == 0)
		cout << "\tNo Clients Availabale in the system!\n";
	else
	{
		for (sClient& Client : vClients)
		{

			PrintClientRecordBalanceLine(Client);
			TotalBalances += Client.AccountBalance;
			cout << endl;
		}
	}

	cout << "\n_____________________________________________________";
	cout << "___________________________________________\n\n";

	cout << "\t\t\t\t Total Balances = " << TotalBalances << "" << endl;
}

void ShowTotalAccountBalancesScreen()
{
	TotalBalances();
}

void MainMenueScreen()
{

}

void GoToTransactionMenue()
{
	cout << "\nPress any key to go back to the main menu...";
	system("pause>0");

	ShowTransactionsScreen();
}

void PreformTransactionsMenue(enTransactionOptions TransactionOptions)
{
	switch (TransactionOptions)
	{
	case enTransactionOptions::eDeposit:
		system("cls");
		ShowDepositScreen();
		GoToTransactionMenue();
		break;
	case enTransactionOptions::eWithDraw:
		system("cls");
		ShowWithdrawScreen();
		GoToTransactionMenue();
		break;
	case enTransactionOptions::eTotalBalances:
		system("cls");
		ShowTotalAccountBalancesScreen();
		GoToTransactionMenue();
		break;
	case enTransactionOptions::eBackToMainMenue:
		system("cls");
		ShowMainMenu();
		break;
	}
}

void ShowTransactionsScreen()
{
	system("cls");
	cout << "=====================================\n";
	cout << "\t  Transactions Menue Screen\n";
	cout << "=====================================\n";
	cout << "[1] Deposit." << endl;
	cout << "[2] WithDraw." << endl;
	cout << "[3] Total Balances." << endl;
	cout << "[4] Main Menue." << endl;
	cout << "=====================================\n";

	PreformTransactionsMenue(ReadTransactionOptions());
}

int main()
{
	ShowMainMenu();
	system("pause>0");

	return 0;
}