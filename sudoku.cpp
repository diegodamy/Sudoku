#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <windows.h>

using namespace std;

int CheckFile(string textname2){ //Simple code to check whether file was found. It must be located in smae place as .cpp file!
  ifstream Sudoku;
  Sudoku.open(textname2.c_str());

  if (Sudoku.is_open()) {
    cout << "File was opened succesfully!" << endl;
    return 1;
    } else {
    return 0;
  }
}


vector <int>  ReadDisplayFile(string textname1){ //My logic tells me this function could work without outputting the sudoku, but it won´t display correctly if I take it out
  vector<int>numbers(81);
  int counter = 1;
  ifstream Sudoku;
  Sudoku.open(textname1.c_str());

  cout << "          A B C  D E F  G H I" << endl << endl << "          ";
  for (int i = 0; i < numbers.size(); i++) {
      Sudoku >> numbers[i];
      cout << numbers[i] << ' ';

        if (i%3 == 2){
          cout <<" ";
        }

        if (i%9 == 8){
          cout << ' ' << counter << endl << "          ";
          counter++;
            if (counter%3 == 1){
              cout << endl << "          ";
        }
      }
    }
    return numbers;
    Sudoku.close();
}


vector <int> CreateFixedValues(vector<int>numbers){ //This function returns the value and position of original values, so they cannont be overwritten by user when playing
  vector <int> OriginalNumbers(numbers.size());
  for (int i = 0; i < numbers.size(); i++){
    OriginalNumbers[i] = numbers[i];
    }
    return OriginalNumbers;
}


int IsFixedValue(vector <int> numbers, int coordinates){ //And now this functions checks that the user doesn´t overwrite the original values
  vector <int> NumbersFixed(numbers.size());

  for (int i = 0; i < numbers.size(); i++){
    NumbersFixed[i] = numbers[i];
    }
    if (NumbersFixed[coordinates] == 0) {
      return 1;
      }
  }


int IsChangePossibleHor(vector<int>numbers, int number, char row) { //Not so raightforward to check, we have to do some math first
  int rownumber = row - '0';
  int lowerlimit = rownumber*9-9;
  int upperlimit = rownumber*9;

  for (int i = lowerlimit; i < upperlimit; i++) {
    if ((numbers[i] == number)) {
      cout << "Repetition in line." << endl;
      return 0;
    }
  }
  return 1;
}


int IsChangePossibleVert (vector<int>numbers, char col, int number) { // This is more straight forward, it just checks for numbers above and below its position
int coln = col - '0' -49;
  for (int n = coln; n <= coln + 72; n+= 9) {
  if (numbers[n] == number) {
    cout << "Repetition in column." << endl;
    return 0;
    }
  }
  return 1;
}


int AidPossibleSquare (char row){ //I had to create this function to locate individual matrices
  int n;
  if ((row >= '1') && (row <='3')) {
    n = 0;
  } else if ((row >= '4') && (row <='6')) {
    n = 27;
  }
   else if ((row >= '7') && (row <='9')){
    n = 27 + 27;
  }
  return n;
  }


int IsChangePossibleSquare (vector<int>numbers, int number, char col, char row){ //And now I check for numbers in said matrix, skipping rows
  int coln = col - '0' -49; // same as col -48
  int a = coln/3;
  int n = AidPossibleSquare (row);

  for (int i = a*3+n; i <= a*3+2+n; i++) {
    if (numbers [i] == number) {
      cout << "Repetition in matrix." << endl;
      return 0;
    }
  }

    for (int j = a*3+9+n; j <= a*3+2+9+n; j++) {
        if (numbers [j] == number) {
          cout << "Repetition in matrix." << endl;
          return 0;
        }
      }

    for (int k = a*3+9+9+n; k <= a*3+2+9+9+n; k++) {
            if (numbers [k] == number) {
              cout << "Repetition in matrix." << endl;
              return 0;
            }
          }
return 1;
}


void DisplaySudoku (vector<int>numbers){
  int counter = 1;
  cout << endl << "          A B C  D E F  G H I" << endl << endl << "          ";
  for (int i = 0; i < numbers.size(); i++) {
      cout << numbers[i] << ' ';

        if (i%3 == 2){
          cout <<" ";
        }

        if (i%9 == 8){
          if (counter>9){
            counter = 1;
          }
          cout << ' ' << counter << endl << "          ";
          counter++;
            if (counter%3 == 1){
              cout << endl << "          ";
        }
      }
    }
}


int CheckIfDone (vector <int> numbers){ //Just keeps track of how many numbers are there left to fill
  int n = 0;

  for (int i = 0; i <81; i++){
    if ( numbers[i] != 0 ){
      n++;
    } if (n == 81){

      return 0;
    }
  }
return 1;
}


int GetCoordinates(vector <int> numbers){//The biggest function of them all, this one kinda multitasks
  int coordinates = -1;
  int number;
  char row;
  char col;

  vector <int> CompareOriginal = CreateFixedValues(numbers);

  do{

    cout << endl << "Please enter the coodinates of the number you wish to change:" << endl; //First it asks the user for coordinates
    cout << "Enter column (Lowercase):" << endl;
    cin >> col;
    cout << "Enter row:" << endl;
    cin >> row;

    while ((col < 'a') || (col > 'i') || (row < '1') || (row > '9')) { //Then it validates the input
      cout << "Please check your input." << endl;
      DisplaySudoku(numbers);
      cout << endl << "Enter column:" << endl;
      cin >> col;
      cout << "Enter row:" << endl;
      cin >> row;
    }

    coordinates = ((col-97) + (row - 49)*9); //Then transform input into coordinates

    if (IsFixedValue(CompareOriginal, coordinates) == 1) { //This whole part checks for the sudoku's rules

        cout << "Enter number:" << endl;
        cin >> number;
        if ((IsChangePossibleHor(numbers, number, row) == 1)
        && ((IsChangePossibleVert(numbers, col, number) == 1))
        && ((IsChangePossibleSquare(numbers, number, col, row) == 1))) {

        numbers[coordinates] = number;

        DisplaySudoku(numbers);

        } else {
            cout << endl;
            DisplaySudoku(numbers);
        }
      } else {
        cout << endl << "Number is fixed. Try again." << endl;
        cout << endl;
        DisplaySudoku(numbers);

        }
    } while ((CheckIfDone(numbers) == 1)); //Keeps ong going until all numbers are filled out

    cout << endl << "Congratulations, you won!" << endl;

  }

int main(){
  string TextName;
  cout << "Please enter the name of the text:" << endl;
  cin >> TextName;

  while (CheckFile(TextName) == 0) { //Validates .txt input
    cout << "Unable to open file." << endl;
    cout <<"Please enter the name of the text:" << endl;
    cin >> TextName;
  }
  cout << endl;

  vector <int> DinNumbers = ReadDisplayFile(TextName);
  GetCoordinates(DinNumbers);//Sets everything into motion
}
