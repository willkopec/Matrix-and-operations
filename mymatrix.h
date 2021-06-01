/*mymatrix.h*/

// 
// << William Kopec >>
// U. of Illinois, Chicago
// CS 251: Spring 2020
// Project #02
//

//
// mymatrix
//
// The mymatrix class provides a matrix (2D array) abstraction.
// The size can grow dynamically in both directions (rows and 
// cols).  Also, rows can be "jagged" --- i.e. rows can have 
// different column sizes, and thus the matrix is not necessarily 
// rectangular.  All elements are initialized to the default value
// for the given type T.

#pragma once

#include <iostream>
#include <exception>
#include <stdexcept>

using namespace std;

template<typename T>
class mymatrix
{
private:
  int Size;  //Size of the matrix
  struct ROW
  {
    T*  Cols;     // dynamic array of column elements
    int NumCols;  // total # of columns (0..NumCols-1)
  };

  ROW* Rows;     // dynamic array of ROWs
  int  NumRows;  // total # of rows (0..NumRows-1)

public:
  //
  // default constructor:
  //
  // Called automatically by C++ to construct a 4x4 matrix.  All 
  // elements are initialized to the default value of T.
  //
  mymatrix()
  {
    Size = 0;
    Rows = new ROW[4];  // an array with 4 ROW structs:
    NumRows = 4;

    // initialize each row to have 4 columns:
    for (int r = 0; r < NumRows; ++r)
    {
      Rows[r].Cols = new T[4];  // an array with 4 elements of type T:
      Rows[r].NumCols = 4;

      // initialize the elements to their default value:
      for (int c = 0; c < Rows[r].NumCols; ++c)
      {
        Rows[r].Cols[c] = T{};  // default value for type T:
        //cout << Rows[r].Cols[c] << "\t";
        Size++;
      }
      //cout << endl;
    }
  }

  //
  // parameterized constructor:
  //
  // Called automatically by C++ to construct a matrix with R rows, 
  // where each row has C columns. All elements are initialized to 
  // the default value of T.
  //
  mymatrix(int R, int C)
  {
    //if rows of cols are less than one, give error, no matrix can have 0 cols or rows
    if (R < 1)
      throw invalid_argument("mymatrix::constructor: # of rows");
    if (C < 1)
      throw invalid_argument("mymatrix::constructor: # of cols");

    Size = 0;  //declare new matrix to size 0
    Rows = new ROW[R];
    NumRows = R;

    for (int r = 0; r < NumRows; ++r)
    {
        Rows[r].Cols = new T[C];  // an array with C elements of type T:
        Rows[r].NumCols = C;      //setting this rows numcols to C aswell
      

      // initialize the elements to their default value:
      for (int c = 0; c < Rows[r].NumCols; ++c)
      {
        Rows[r].Cols[c] = T{};  // default value for type T:
        //cout << Rows[r].Cols[c] << "\t";
        Size++;
      }
    }
  }


  //
  // copy constructor:
  //
  // Called automatically by C++ to construct a matrix that contains a 
  // copy of an existing matrix.  Example: this occurs when passing 
  // mymatrix as a parameter by value
  //
  //   void somefunction(mymatrix<int> M2)  <--- M2 is a copy:
  //   { ... }
  //
  mymatrix(const mymatrix<T>& other)
  {

    NumRows = other.NumRows;
    Rows = new ROW[NumRows];
    
    for (int r = 0; r < NumRows; ++r)
    {
        Rows[r].NumCols = other.Rows[0].NumCols;    
        Rows[r].Cols = new T[ Rows[r].NumCols ];     // an array with C elements of type T:
        

      // initialize the elements to the passed values of other matrix:
      for (int c = 0; c < Rows[r].NumCols; ++c)
      {
        Rows[r].Cols[c] = other(r,c);  // default value for type T:
        Size++;
      }
    }
  

  }


  //
  // numrows
  //
  // Returns the # of rows in the matrix.  The indices for these rows
  int numrows() const
  {
    return NumRows;
  }
  

  //
  // numcols
  //
  // Returns the # of columns in row r.  The indices for these columns
  // are 0..numcols-1.  Note that the # of columns can be different 
  // row-by-row since "jagged" rows are supported --- matrices are not
  // necessarily rectangular.
  //
  int numcols(int r) const
  {
    if (r < 0 || r >= NumRows)
      throw invalid_argument("mymatrix::numcols: row");

    return Rows[r].NumCols;
  }


  //
  // growcols
  //
  // Grows the # of columns in row r to at least C.  If row r contains 
  // fewer than C columns, then columns are added; the existing elements
  // are retained and new locations are initialized to the default value 
  // for T.  If row r has C or more columns, then all existing columns
  // are retained -- we never reduce the # of columns.
  //
  // Jagged rows are supported, i.e. different rows may have different
  // column capacities -- matrices are not necessarily rectangular.
  //
  void growcols(int r, int C)
  {
    if (r < 0 || r >= NumRows)
      throw invalid_argument("mymatrix::growcols: row");
    if (C < 1)
      throw invalid_argument("mymatrix::growcols: columns");
      
     if(C <= Rows[r].NumCols){ //if the cols are less than the cols before calling grow cols, do nothing
         return;
     }
     
     T* newRow = new T[C]; //make new 
     
     for(int i = 0; i<Rows[r].NumCols; i++){
         newRow[i] = Rows[r].Cols[i];
     }
     for(int j = Rows[r].NumCols; j < C; j++){
         newRow[j] = T{};
     }
     delete[] Rows[r].Cols;
     Rows[r].Cols = newRow;
     Size += C - Rows[r].NumCols;
     Rows[r].NumCols = C;

  }


  //
  // grow
  //
  // Grows the size of the matrix so that it contains at least R rows,
  // and every row contains at least C columns.
  // 
  // If the matrix contains fewer than R rows, then rows are added
  // to the matrix; each new row will have C columns initialized to 
  // the default value of T.  If R <= numrows(), then all existing
  // rows are retained -- we never reduce the # of rows.
  //
  // If any row contains fewer than C columns, then columns are added
  // to increase the # of columns to C; existing values are retained
  // and additional columns are initialized to the default value of T.
  // If C <= numcols(r) for any row r, then all existing columns are
  // retained -- we never reduce the # of columns.
  // 
  void grow(int R, int C)
  {
    if (R < 1)
      throw invalid_argument("mymatrix::grow: # of rows");
    if (C < 1)
      throw invalid_argument("mymatrix::grow: # of cols");
    if(this->NumRows > R){
      return;
    } else {
      Size = 0;
      int prevNumRows = NumRows;
      ROW *newRow = new ROW[R];

      NumRows = R;
      //newRow[r].NumCols = C;
      for(int i = 0; i < prevNumRows; i++){
        growcols(i,C);
        newRow[i] = Rows[i];
      }//THIS FUNCTION IS NOT PERFECt
      //TRY GROWING grow(1,8)
      for (int i = prevNumRows; i < R; i++){
        newRow[i].NumCols = C;
        newRow[i].Cols = new T[C];
        for(int c = 0; c < C; c++){
          newRow[i].Cols[c] = T{};
        }
      }
        delete [] Rows;
        Rows = newRow;
        Size = R * C;
    }
  }


  //
  // size
  //
  // Returns the total # of elements in the matrix.
  //
  int size() const
  {
    return Size;
  }

    void display(){
      for (int r = 0; r < NumRows; ++r){
        for (int c = 0; c < Rows[r].NumCols; ++c){
          cout << Rows[r].Cols[c] << "\t";
        }
        cout << endl;
      }
    }


  //
  // at
  //
  // Returns a reference to the element at location (r, c); this
  // allows you to access the element or change it:
  //
  //    M.at(r, c) = ...
  //    cout << M.at(r, c) << endl;
  //
  T& at(int r, int c) const
  {
    if (r < 0 || r >= NumRows)
      throw invalid_argument("mymatrix::at: row");
    if (c < 0 || c >= Rows[r].NumCols)
      throw invalid_argument("mymatrix::at: col");
    return Rows[r].Cols[c];
  }


  //
  // ()
  //
  // Returns a reference to the element at location (r, c); this
  // allows you to access the element or change it:
  //
  //    M(r, c) = ...
  //    cout << M(r, c) << endl;
  //
  T& operator()(int r, int c) const
  {
    if (r < 0 || r >= NumRows)
      throw invalid_argument("mymatrix::(): row");
    if (c < 0 || c >= Rows[r].NumCols)
      throw invalid_argument("mymatrix::(): col");

    return Rows[r].Cols[c];
  }

  //
  // scalar multiplication
  //
  // Multiplies every element of this matrix by the given scalar value,
  // producing a new matrix that is returned.  "This" matrix is not
  // changed.
  //
  // Example:  M2 = M1 * 2;
  //
mymatrix<T> operator*(T scalar) {
  mymatrix<T> result(this->NumRows, 1);
  Size = 0;
    for(int r = 0; r < this->NumRows; r++){
      result.Rows[r].NumCols = this->Rows[r].NumCols;
      result.Rows[r].Cols = new T[this->Rows[r].NumCols];
        for(int c = 0; c < this->Rows[r].NumCols; c++){
            result.Rows[r].Cols[c] = this->Rows[r].Cols[c] * scalar;
            if(c >= 1){
              result.Size++;
            }
        }
    }
    return result;
}

  //
  // matrix multiplication
  //
  // Performs matrix multiplication M1 * M2, where M1 is "this" matrix and
  // M2 is the "other" matrix.  This produces a new matrix, which is returned.
  // "This" matrix is not changed, and neither is the "other" matrix.
  //
  // Example:  (result) M3 = M1 (this) * M2 (other);
  //
  // NOTE: M1 and M2 must be rectangular, if not an exception is thrown.  In
  // addition, the sizes of M1 and M2 must be compatible in the following sense:
  // M1 must be of size RxN and M2 must be of size NxC.  In this case, matrix
  // multiplication can be performed, and the resulting matrix is of size RxC.
  //
  mymatrix<T> operator*(mymatrix<T>& other)
  {

    //
    // both matrices must be rectangular for this to work:
    //
        int column1 = Rows[0].NumCols;
        for(int r = 0; r < NumRows; r++){
            if(Rows[r].NumCols != column1){
                throw runtime_error("mymatrix::*: this not rectangular");
            }
        }

        int column2 = (other.size()/other.NumRows);
        for(int r = 0; r < other.NumRows; r++){
            if(other.NumRows != column2){
                throw runtime_error("mymatrix::*: other not rectangular");
            }
        }
        // if (this matrix's # of columns != other matrix's # of rows)
        //throw runtime_error("mymatrix::*: size mismatch");
        for(int r = 0; r< NumRows; r++){
          if(Rows[r].NumCols != other.Rows[r].NumCols){
            throw runtime_error("mymatrix::*: size mismatch");
          }
        }

        //
        // Okay, we can multiply:
        //

        mymatrix<T> result(NumRows,other.Rows[0].NumCols);
        for (int r = 0; r < Rows[0].NumCols; r++) {
            for (int c =Rows[0].NumCols ; c < other.NumRows; c++) {
                result(r, c) = T{};//intializing to default
            }
        }
        result.NumRows = (this->NumRows);
        int value = 0;
        for(int i = 0; i < NumRows; ++i) {//goes through the rows after end of one row multiplication
            for (int j = 0; j < result.Rows[0].NumCols; ++j) {//goes through each column in other matrix
                for (int k = 0; k < other.NumRows; ++k) {
                    value += (Rows[i].Cols[k] * other.at(k,j));
                }
                result(i, j) = value;
                value = 0;//resetting value to 0
            }

        }
      return result;
  }

  //
  // _output
  //
  // Outputs the contents of the matrix; for debugging purposes.
  //
  void _output()
  {
    for (int r = 0; r < this->NumRows; ++r)
    {
      for (int c = 0; c < this->Rows[r].NumCols; ++c)
      {
        cout << this->Rows[r].Cols[c] << " ";
      }
      cout << endl;
    }
  }

};