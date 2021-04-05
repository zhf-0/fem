#ifndef TENSOR_H
#define TENSOR_H
#include <iostream>

//=========================================================
// class Vec
template<typename INT, typename DOUBLE>
class Vec
{
public:
	// constructor
	Vec():len(0),val(nullptr){}
	Vec(INT l);
	Vec(INT l,DOUBLE num);

	// copy constructor
	Vec(const Vec<INT,DOUBLE> & x);

	// copy assignment and move assignment operator
	Vec<INT,DOUBLE>& operator=(Vec<INT,DOUBLE> x);

	// move copy constructor
	Vec(Vec<INT,DOUBLE> && x);

	// friend: swap the element in class
	friend void VecSwap(Vec<INT,DOUBLE> & a, Vec<INT,DOUBLE> & b)
	{
		std::swap(a.len, b.len);
		std::swap(a.val, b.val);
	}

	// overload the ()
	DOUBLE & operator()(INT i);
	DOUBLE & operator()(INT i) const;

	// add boundary check 
	DOUBLE & at(INT i);
	DOUBLE & at(INT i) const;

	// get length
	INT GetLen() const;

	// destructor
	virtual ~Vec();

protected:
	INT len;

public:
	DOUBLE *val;

};

// constructor
template<typename INT, typename DOUBLE>
Vec<INT,DOUBLE>::Vec(INT l)
{
	if(l > 0)
	{
		len = l;
		val = new DOUBLE[l];
	}
	else
	{
		std::cout<<"the length of the Vec <= 0"<<std::endl;
		exit(1);
	}
}


// constructor
template<typename INT, typename DOUBLE>
Vec<INT,DOUBLE>::Vec(INT l,DOUBLE num)
{
	if(l > 0)
	{
		len = l;
		val = new DOUBLE[l];
		for(INT i=0;i<l;i++)
			val[i]=num;
	}
	else
	{
		std::cout<<"the length of the Vec <= 0"<<std::endl;
		exit(1);
	}
}
	
// copy constructor
template<typename INT, typename DOUBLE>
Vec<INT,DOUBLE>::Vec(const Vec<INT,DOUBLE> & x)
{
#if debug == 1
	std::cout<<"copy constructor of Vec"<<std::endl;
#endif
	len = x.len;
	val = new DOUBLE[len];
	for(INT i=0;i<len;i++)
		val[i]=x.val[i];
}

// copy assignment and move assignment operator
template<typename INT, typename DOUBLE>
Vec<INT,DOUBLE>& Vec<INT,DOUBLE>::operator=(Vec<INT,DOUBLE> x)
{
#if debug == 1
	std::cout<<"copy and move assignment operator of Vec"<<std::endl;
#endif
	VecSwap(*this,x);
	return *this;	
}


// move copy constructor
template<typename INT, typename DOUBLE>
Vec<INT,DOUBLE>::Vec(Vec<INT,DOUBLE> && x)
{
#if debug == 1
	std::cout<<"move copy constructor of Vec"<<std::endl;
#endif
	VecSwap(*this, x);
}


template<typename INT, typename DOUBLE>
DOUBLE & Vec<INT,DOUBLE>::operator()(INT i)
{
	return val[i];
}

template<typename INT, typename DOUBLE>
DOUBLE & Vec<INT,DOUBLE>::operator()(INT i) const
{
	return val[i];
}

template<typename INT, typename DOUBLE>
DOUBLE & Vec<INT,DOUBLE>::at(INT i)
{
	if(i >= 0 && i<len)
	{
		return val[i];
	}	
	else
	{
		std::cout<<"Vec index is out of range"<<std::endl;
		std::cout<<"free the memory and exit"<<std::endl;
		delete[] val;
		exit(1);
	}
}

template<typename INT, typename DOUBLE>
DOUBLE & Vec<INT,DOUBLE>::at(INT i) const
{
	if(i >= 0 && i<len)
	{
		return val[i];
	}	
	else
	{
		std::cout<<"Vec index is out of range"<<std::endl;
		std::cout<<"free the memory and exit"<<std::endl;
		delete[] val;
		exit(1);
	}
}

template<typename INT, typename DOUBLE>
INT Vec<INT,DOUBLE>::GetLen() const
{
	return len;
}


template<typename INT, typename DOUBLE>
Vec<INT,DOUBLE>::~Vec()
{
	if(val != nullptr)
	{
		delete[] val;
		val = nullptr;
	}
}
//=========================================================
// class Mat

template<typename INT, typename DOUBLE>
class Mat:public Vec<INT,DOUBLE>
{
public:
	//constructor
	Mat():Vec<INT,DOUBLE>(),row(0),col(0){}
	Mat(INT x,INT y):Vec<INT,DOUBLE>(x*y),row(x),col(y){}
	Mat(INT x,INT y,DOUBLE num):Vec<INT,DOUBLE>(x*y,num),row(x),col(y){}

	//copy constructor
	Mat(const Mat<INT,DOUBLE> &x):Vec<INT,DOUBLE>(x),row(x.row),col(x.col)
	{
#if debug == 1
	std::cout<<"copy constructor of Mat"<<std::endl;
#endif
	}
	//move copy constructor
	Mat(Mat<INT,DOUBLE> && x);

	//copy and move assignment operator
	Mat<INT,DOUBLE> & operator=(Mat<INT,DOUBLE> x);

	// overload the ()
	using Vec<INT,DOUBLE>::operator();
	DOUBLE & operator()(INT i, INT j);
	DOUBLE & operator()(INT i, INT j) const;

	// add boundary check
	using Vec<INT,DOUBLE>::at;
	DOUBLE & at(INT i, INT j);
	DOUBLE & at(INT i, INT j) const;

	// overload *
	friend Mat<INT,DOUBLE> operator* (const Mat<INT,DOUBLE> & a ,const Mat<INT,DOUBLE> &b)
	{

		INT a_row = a.GetRow();
		INT a_col = a.GetCol();
		INT b_row = b.GetRow();
		INT b_col = b.GetCol();
		
		if(a_col != b_row)
		{
			std::cout<<"the dimension doesn't match"<<std::endl;
			exit(1);
		}

		Mat<INT,DOUBLE> res(a_row,b_col);
		/* std::cout<<"the address of res:"<<&res<<std::endl; */

		for(INT i=0;i<a_row;i++)
		{
			for(INT j =0;j<b_col;j++)
			{
				DOUBLE tmp = 0;
				for(INT k=0;k<a_col;k++)
				{
					/* tmp += a(i,k)*b(k,j); */
					tmp += a.val[i*a_row+k] * b.val[k*b_row+j];
				}
				/* res(i,j) = tmp; */
				res.val[i*a_row+j] = tmp;
			}
		}

		return res;
	}

	//overload <<
	friend std::ostream & operator<<(std::ostream &output,Mat<INT,DOUBLE> &x)
	{
		
		output<<"Mat= "<<std::endl;

		for(int i=0;i<x.row;i++)
		{
			output<<"     ";
			for(int j=0;j<x.col;j++)
				output<<x.val[i*x.col+j]<<" ";
			output<<std::endl;
		}

		return output;
	}

	// friend 
	friend void MatSwap(Mat<INT,DOUBLE> & a,Mat<INT,DOUBLE> & b)
	{
		VecSwap(static_cast<Vec<INT,DOUBLE>&>(a),static_cast<Vec<INT,DOUBLE>&>(b));
		a.row = b.row;
		a.col = b.col;
	}

	// get row and col
	INT GetRow() const;
	INT GetCol() const;

	//destructor
	virtual ~Mat(){}

protected:
	INT row;
	INT col;
};

//move copy constructor
template<typename INT, typename DOUBLE>
Mat<INT,DOUBLE>::Mat(Mat<INT,DOUBLE> && x)
{
#if debug == 1
	std::cout<<"move copy constructor of Mat"<<std::endl;
#endif
	MatSwap(*this,x);
}


//copy and move assignment operator
template<typename INT, typename DOUBLE>
Mat<INT,DOUBLE> & Mat<INT,DOUBLE>::operator=(Mat<INT,DOUBLE> x)
{
#if debug == 1
	std::cout<<"copy and move assignment operator of Mat"<<std::endl;
#endif
	MatSwap(*this,x);
	return *this;
}


template<typename INT, typename DOUBLE>
DOUBLE & Mat<INT,DOUBLE>::operator()(INT i, INT j)
{
	return this->val[i*row+j];
}

template<typename INT, typename DOUBLE>
DOUBLE & Mat<INT,DOUBLE>::operator()(INT i, INT j) const
{
	return this->val[i*row+j];
}

template<typename INT, typename DOUBLE>
DOUBLE & Mat<INT,DOUBLE>::at(INT i, INT j)
{
	if(i >= 0 && i<row && j >= 0 && j < col)
	{
		return this->val[i*row+j];
	}
	else
	{
		std::cout<<"Matrix index is out of range"<<std::endl;
		std::cout<<"free the memory and exit"<<std::endl;
		delete[] this->val;
		exit(1);
	}
}

template<typename INT, typename DOUBLE>
DOUBLE & Mat<INT,DOUBLE>::at(INT i, INT j) const
{
	if(i >= 0 && i<row && j >= 0 && j < col)
	{
		return this->val[i*row+j];
	}
	else
	{
		std::cout<<"Matrix index is out of range"<<std::endl;
		std::cout<<"free the memory and exit"<<std::endl;
		delete[] this->val;
		exit(1);
	}
}


template<typename INT, typename DOUBLE>
INT Mat<INT,DOUBLE>::GetRow() const
{
	return row;
}

template<typename INT, typename DOUBLE>
INT Mat<INT,DOUBLE>::GetCol() const 
{
	return col;
}



#endif
