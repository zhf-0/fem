#ifndef SPARSEMAT_H
#define SPARSEMAT_H
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

template<typename INT, typename DOUBLE>
class SpaMat
{
public:
	// constructor
	SpaMat():row_len(0),col_len(0),val_len(0),row_vec(nullptr),col_vec(nullptr),val(nullptr){}
	SpaMat(INT r,INT c, INT n);
	SpaMat(INT r,INT c, INT n,DOUBLE x);

	// copy constructor
	SpaMat(const SpaMat<INT,DOUBLE> & mat);

	// swap 
	friend void MatSwap(SpaMat<INT,DOUBLE> &a,SpaMat<INT,DOUBLE> &b)
	{
		std::swap(a.row_len,b.row_len);
		std::swap(a.col_len,b.col_len);
		std::swap(a.val_len,b.val_len);

		std::swap(a.row_vec,b.row_vec);
		std::swap(a.col_vec,b.col_vec);
		std::swap(a.val,b.val);
	}

	// move copy constructor
	SpaMat(SpaMat<INT,DOUBLE> && mat);

	// copy and move assignment operator
	SpaMat & operator= (SpaMat<INT,DOUBLE> mat);

	// destructor 
	virtual ~SpaMat();

	// initialize the vec 
	void InitRowVec(INT a,INT num);
	void InitColVec(INT a,INT num);
	void InitValVec(DOUBLE a, INT num);

	// resize the vec
	void ResizeRowVec(INT new_len);
	void ResizeColVec(INT new_len);
	void ResizeValVec(INT new_len);

	// delete the vec
	void DeleteRowVec();
	void DeleteColVec();
	void DeleteValVec();

protected:
	INT row_len;

	INT col_len;

	INT val_len;

public:
	// row vector
	INT * row_vec;

	// col vector
	INT * col_vec;

	// val vector 
	DOUBLE * val;
};

template<typename INT, typename DOUBLE>
class SpaCOO:public SpaMat<INT,DOUBLE>
{
public:
	// constructor
	SpaCOO():SpaMat<INT,DOUBLE>(),row(0),col(0),nnz(0){}
	SpaCOO(INT r,INT c, INT n):SpaMat<INT,DOUBLE>(n,n,n),row(r),col(c),nnz(n){}
	SpaCOO(INT r,INT c, INT n,DOUBLE x):SpaMat<INT,DOUBLE>(n,n,n,x),row(r),col(c),nnz(n){}

	// copy constructor
	SpaCOO(const SpaCOO<INT,DOUBLE> & coo);

	// swap 
	friend void COOSwap(SpaCOO<INT,DOUBLE> &a,SpaCOO<INT,DOUBLE> &b)
	{
		std::swap(a.row,b.row);
		std::swap(a.col,b.col);
		std::swap(a.nnz,b.nnz);

		MatSwap(a,b);
	}

	// move copy constructor
	SpaCOO(SpaCOO<INT,DOUBLE> && coo);

	// copy and move  assignment operator
	SpaCOO<INT,DOUBLE> & operator= (SpaCOO<INT,DOUBLE> coo);

	// destructor 
	virtual ~SpaCOO(){}

	// overload the virtual function from base class, and print num entries in coo matrix 
	void  PrintPartialMat(INT num);

	// read matrix from file, index in file begin with 0
	void ReadMat0(std::string file_name);

	// read matrix from file, index in file begin with 1
	void ReadMat1(std::string file_name);

	inline INT GetRow() const
	{
		return row;
	}

	inline INT GetCol() const
	{
		return col;
	}

	inline INT GetNnz() const
	{
		return nnz;
	}

protected:
	// number of row
	INT row;

	// number of col
	INT col;

	// number of nnz
	INT nnz;
};


template<typename INT, typename DOUBLE>
class SpaCSR:SpaMat<INT,DOUBLE>
{
public:
	// constructor
	SpaCSR():SpaMat<INT,DOUBLE>(),row(0),col(0),nnz(0){}
	SpaCSR(INT r,INT c, INT n):SpaMat<INT,DOUBLE>(r+1,n,n),row(r),col(c),nnz(n){}
	SpaCSR(INT r,INT c, INT n,DOUBLE x):SpaMat<INT,DOUBLE>(r+1,n,n,x),row(r),col(c),nnz(n){}

	// copy constructor
	SpaCSR(const SpaCSR<INT,DOUBLE> & csr);

	// swap 
	friend void CSRSwap(SpaCSR<INT,DOUBLE> &a,SpaCSR<INT,DOUBLE> &b)
	{
		std::swap(a.row,b.row);
		std::swap(a.col,b.col);
		std::swap(a.nnz,b.nnz);

		MatSwap(a,b);
	}

	// move copy constructor 
	SpaCSR(SpaCSR<INT,DOUBLE> && csr);

	// copy and move assignment operator
	SpaCSR<INT,DOUBLE> & operator=(SpaCSR<INT,DOUBLE> csr);

	// from coo to csr
	void COO2CSR(const SpaCOO<INT,DOUBLE> & coo);
	SpaCSR(const SpaCOO<INT,DOUBLE> & coo);
	SpaCSR & operator=(const SpaCOO<INT,DOUBLE> & coo);

	// destructor 
	virtual ~SpaCSR(){}

	// print num rows in csr matrix 
	void PrintPartialMat(INT num);

	// overload the operator * for spmv
	/* friend Vec operator*(const SpaCSR & csr, const Vec & vec); */

	inline INT GetRow() const
	{
		return row;
	}

	inline INT GetCol() const
	{
		return col;
	}

	inline INT GetNnz() const
	{
		return nnz;
	}

protected:
	// number of row 
	INT row;

	// number of col
	INT col;

	// number of nnz
	INT nnz;
};

//================================================================
// class SpaMat

template<typename INT, typename DOUBLE>
SpaMat<INT,DOUBLE>::SpaMat(INT r,INT c, INT n):row_len(r),col_len(c),val_len(n)
{
	if(r > 0 && c > 0 && n > 0 )
	{
		row_vec = new INT[r];
		col_vec = new INT[c];
		val = new DOUBLE[n];
	}
	else
	{
		std::cout<<"the dim in the constructor of SpaMat is wrong"<<std::endl;
		exit(1);
	}
}

template<typename INT, typename DOUBLE>
SpaMat<INT,DOUBLE>::SpaMat(INT r,INT c, INT n,DOUBLE x):row_len(r),col_len(c),val_len(n)
{
	if(r > 0 && c > 0 && n > 0 )
	{
		row_vec = new INT[r];
		col_vec = new INT[c];
		val = new DOUBLE[n];
		if(x == 0.0)
		{
			memset(val,0,sizeof(DOUBLE)*n);
		}
		else
		{
			for(INT i=0;i<n;i++)
				val[i] = x;
		}
	}
	else
	{
		std::cout<<"the dim in the constructor of SpaMat is wrong"<<std::endl;
		exit(1);
	}
}

// copy constructor
template<typename INT, typename DOUBLE>
SpaMat<INT,DOUBLE>::SpaMat(const SpaMat<INT,DOUBLE> & mat):row_len(mat.row_len),col_len(mat.col_len),val_len(mat.val_len)
{
#if debug == 1
	std::cout<<"copy constructor of SpaMat"<<std::endl;
#endif
		row_vec = new INT[mat.row_len];
		col_vec = new INT[mat.col_len];
		val = new DOUBLE[mat.val_len];

		/* for(INT i=0;i<mat.row_len;i++) */
		/* 	row_vec[i] = mat.row_vec[i]; */
		memcpy(row_vec, mat.row_vec, sizeof(INT)*mat.row_len);

		/* for(INT i=0;i<mat.col_len;i++) */
		/* 	col_vec[i] = mat.col_vec[i]; */
		memcpy(col_vec, mat.col_vec, sizeof(INT)*mat.col_len);

		/* for(INT i=0;i<mat.val_len;i++) */
		/* 	val[i] = mat.val[i]; */
		memcpy(val, mat.val, sizeof(DOUBLE)*mat.val_len);
}


// move copy constructor
template<typename INT, typename DOUBLE>
SpaMat<INT,DOUBLE>::SpaMat(SpaMat<INT,DOUBLE> && mat)
{
#if debug == 1
	std::cout<<"move copy constructor of SpaMat"<<std::endl;
#endif
	MatSwap(*this,mat);
}


// copy and move assignment operator
template<typename INT, typename DOUBLE>
SpaMat<INT,DOUBLE> & SpaMat<INT,DOUBLE>::operator= (SpaMat<INT,DOUBLE> mat)
{
#if debug == 1
	std::cout<<"copy and move assignment operator of SpaMat"<<std::endl;
#endif
	MatSwap(*this,mat);
	return *this;
}

template<typename INT, typename DOUBLE>
void SpaMat<INT,DOUBLE>::DeleteRowVec()
{
	if(row_vec != nullptr)
	{
		delete[] row_vec;
		row_vec = nullptr;
		row_len = 0;
	}
}

template<typename INT, typename DOUBLE>
void SpaMat<INT,DOUBLE>::DeleteColVec()
{
	if(col_vec != nullptr)
	{
		delete[] col_vec;
		col_vec = nullptr;
		col_len = 0;
	}
}

template<typename INT, typename DOUBLE>
void SpaMat<INT,DOUBLE>::DeleteValVec()
{
	if(val != nullptr)
	{
		delete[] val;
		val = nullptr;
		val_len = 0;
	}
}

template<typename INT, typename DOUBLE>
SpaMat<INT,DOUBLE>::~SpaMat()
{
	DeleteRowVec();
	DeleteColVec();
	DeleteValVec();	
}

template<typename INT, typename DOUBLE>
void SpaMat<INT,DOUBLE>::InitRowVec(INT x,INT num)
{
	/* memset(row_vec, a, sizeof(INT)*num); */
	if(x == 0)
	{
		memset(row_vec,0,sizeof(INT)*num);
	}
	else
	{
		for(INT i=0;i<num;i++)
			row_vec[i] = x;
	}
}

template<typename INT, typename DOUBLE>
void SpaMat<INT,DOUBLE>::InitColVec(INT x,INT num)
{
	/* memset(col_vec, a, sizeof(INT)*num); */
	if(x == 0)
	{
		memset(col_vec,0,sizeof(INT)*num);
	}
	else
	{
		for(INT i=0;i<num;i++)
			row_vec[i] = x;
	}
}

template<typename INT, typename DOUBLE>
void SpaMat<INT,DOUBLE>::InitValVec(DOUBLE x, INT num)
{
	/* memset(val, a, sizeof(DOUBLE)*num); */
	if(x == 0.0)
	{
		memset(val,0,sizeof(DOUBLE)*num);
	}
	else
	{
		for(INT i=0;i<num;i++)
			val[i] = x;
	}
}

template<typename INT, typename DOUBLE>
void SpaMat<INT,DOUBLE>::ResizeRowVec(INT new_len)
{
	if(new_len <= row_len)
	{
		std::cout<<"new row length <= original row length"<<std::endl;
	}
	row_len = new_len;
	if(row_vec != nullptr)
		delete[] row_vec;
	row_vec = new INT[new_len];
}

template<typename INT, typename DOUBLE>
void SpaMat<INT,DOUBLE>::ResizeColVec(INT new_len)
{
	if(new_len <= col_len)
	{
		std::cout<<"new col length <= original col length"<<std::endl;
	}
	col_len = new_len;
	if(col_vec != nullptr)
		delete[] col_vec;
	col_vec = new INT[new_len];
}

template<typename INT, typename DOUBLE>
void SpaMat<INT,DOUBLE>::ResizeValVec(INT new_len)
{
	if(new_len <= val_len)
	{
		std::cout<<"new val length <= original val length"<<std::endl;
	}
	val_len = new_len;
	if(val != nullptr)
		delete[] val;
	val = new DOUBLE[new_len];
}

//================================================================
// class SpaCOO

// copy constructor
template<typename INT, typename DOUBLE>
SpaCOO<INT,DOUBLE>::SpaCOO(const SpaCOO<INT,DOUBLE> & coo):SpaMat<INT,DOUBLE>(coo),row(coo.row),col(coo.col),nnz(coo.nnz)
{
#if debug == 1
	std::cout<<"copy constructor of SpaCOO"<<std::endl;
#endif
}


// move copy constructor
template<typename INT, typename DOUBLE>
SpaCOO<INT,DOUBLE>::SpaCOO(SpaCOO<INT,DOUBLE> && coo)
{
#if debug == 1
	std::cout<<"move copy constructor of SpaCOO"<<std::endl;
#endif
	COOSwap(*this,coo);
}


// copy and move assignment operator
template<typename INT, typename DOUBLE>
SpaCOO<INT,DOUBLE> & SpaCOO<INT,DOUBLE>::operator= (SpaCOO<INT,DOUBLE> coo)
{
#if debug == 1
	std::cout<<"copy and move assignment operator of SpaCOO"<<std::endl;
#endif
	COOSwap(*this,coo);
	return *this;
}


template<typename INT, typename DOUBLE>
void SpaCOO<INT,DOUBLE>::ReadMat0(std::string file_name)
{
	std::ifstream myfile(file_name);
	if(!myfile.is_open())
	{
		std::cout<<"can't open the matrix file!"<<std::endl;
		exit(1);
	}

	std::string line;
	getline(myfile,line);
	std::istringstream split_line(line);
	INT tmp_row, tmp_col, tmp_nnz;
	split_line >> tmp_row >> tmp_col >> tmp_nnz;

	/* if(row_vec != nullptr) */
	/* 	delete[] row_vec; */
	/* row_vec = new INT[tmp_nnz]; */
	ResizeRowVec(tmp_nnz);

	/* if(col_vec != nullptr) */
	/* 	delete[] col_vec; */
	/* col_vec = new INT[tmp_nnz]; */
	ResizeColVec(tmp_nnz);

	/* if(val != nullptr) */
	/* 	delete[] val; */
	/* val = new DOUBLE[tmp_nnz]; */
	ResizeValVec(tmp_nnz);

	row = tmp_row;
	col = tmp_col;
	nnz = tmp_nnz;

	for(INT i=0;i<nnz;i++)
	{
		getline(myfile,line);
		std::istringstream tmp_line(line);
		tmp_line >> this->row_vec[i] >> this->col_vec[i] >> this->val[i];
	}

	getline(myfile,line);
	if(!myfile.eof())
		std::cout<<"Reading file is not finished!"<< std::endl;
	
	myfile.close();
}

template<typename INT, typename DOUBLE>
void SpaCOO<INT,DOUBLE>::ReadMat1(std::string file_name)
{
	std::ifstream myfile(file_name);
	if(!myfile.is_open())
	{
		std::cout<<"can't open the matrix file!"<<std::endl;
		exit(1);
	}

	std::string line;
	getline(myfile,line);
	std::istringstream split_line(line);
	INT tmp_row, tmp_col, tmp_nnz;
	split_line >> tmp_row >> tmp_col >> tmp_nnz;

	/* if(row_vec != nullptr) */
	/* 	delete[] row_vec; */
	/* row_vec = new INT[tmp_nnz]; */
	ResizeRowVec(tmp_nnz);

	/* if(col_vec != nullptr) */
	/* 	delete[] col_vec; */
	/* col_vec = new INT[tmp_nnz]; */
	ResizeColVec(tmp_nnz);

	/* if(val != nullptr) */
	/* 	delete[] val; */
	/* val = new DOUBLE[tmp_nnz]; */
	ResizeValVec(tmp_nnz);

	row = tmp_row;
	col = tmp_col;
	nnz = tmp_nnz;

	for(INT i=0;i<nnz;i++)
	{
		getline(myfile,line);
		std::istringstream tmp_line(line);

		INT tmp_i,tmp_j;
		tmp_line >> tmp_i >> tmp_j >> this->val[i];
		this->row_vec[i] = tmp_i - 1;
		this->col_vec[i] = tmp_j - 1;
	}

	getline(myfile,line);
	if(!myfile.eof())
		std::cout<<"Reading file is not finished!"<< std::endl;
	
	myfile.close();
}

template<typename INT, typename DOUBLE>
void  SpaCOO<INT,DOUBLE>::PrintPartialMat(INT num)
{
	for(INT i=0;i<num;i++)
		std::cout<<this->row_vec[i]<<"   "<<this->col_vec[i]<<"   "<<this->val[i]<<std::endl;
}


//================================================================
// class SpaCSR
template<typename INT, typename DOUBLE>
SpaCSR<INT,DOUBLE>::SpaCSR(const SpaCSR<INT,DOUBLE> & csr):SpaMat<INT,DOUBLE>(csr),row(csr.row),col(csr.col),nnz(csr.nnz)
{
#if debug == 1
	std::cout<<"copy constructor of SpaCSR"<<std::endl;
#endif
}


template<typename INT, typename DOUBLE>
SpaCSR<INT,DOUBLE>::SpaCSR(SpaCSR<INT,DOUBLE> && csr)
{
#if debug == 1
	std::cout<<"move copy constructor of SpaCSR"<<std::endl;
#endif
	CSRSwap(*this, csr);
}

template<typename INT, typename DOUBLE>
SpaCSR<INT,DOUBLE> & SpaCSR<INT,DOUBLE>::operator=(SpaCSR<INT,DOUBLE> csr)
{
#if debug == 1
	std::cout<<"move and copy assignment operator of SpaCSR"<<std::endl;
#endif
	CSRSwap(*this, csr);
	return *this;
}

template<typename INT, typename DOUBLE>
void SpaCSR<INT,DOUBLE>::COO2CSR(const SpaCOO<INT,DOUBLE> & coo)
{
	InitRowVec(0,row+1);
	for(INT i=0;i<nnz;i++)
		this->row_vec[coo.row_vec[i] + 1] += 1;

	INT * num_per_row = new INT[row+1];
	memcpy(num_per_row, this->row_vec, sizeof(INT)*(row+1));

	for(INT i=2;i<row+1;i++)
		this->row_vec[i] = this->row_vec[i] + this->row_vec[i-1];

	for(INT i=0;i<nnz;i++)
	{
		INT r_idx = coo.row_vec[i];
		INT begin_idx = this->row_vec[r_idx]; 
		INT end_idx = this->row_vec[r_idx + 1];
		INT offset = end_idx - begin_idx - num_per_row[1 + r_idx];

		num_per_row[1 + r_idx] -= 1;
		
		if(offset == end_idx - begin_idx)
		{
			std::cout<<"Converting COO to CSR(assignment function): the index is wrong!"<<std::endl;
			delete[] num_per_row;
			exit(1);
		}
		else
		{
			this->col_vec[begin_idx + offset] = coo.col_vec[i];
			this->val[begin_idx + offset] = coo.val[i];
		}
	}

	delete[] num_per_row;
}

template<typename INT, typename DOUBLE>
SpaCSR<INT,DOUBLE>::SpaCSR(const SpaCOO<INT,DOUBLE> & coo):SpaMat<INT,DOUBLE>(coo.GetRow()+1,coo.GetNnz(),coo.GetNnz())
{
	row = coo.GetRow();
	col = coo.GetCol();
	nnz = coo.GetNnz();

	COO2CSR(coo);
}

template<typename INT, typename DOUBLE>
SpaCSR<INT,DOUBLE> & SpaCSR<INT,DOUBLE>::operator=(const SpaCOO<INT,DOUBLE> & coo)
{
	row = coo.GetRow();
	col = coo.GetCol();
	nnz = coo.GetNnz();
	
	/* if(row_vec != nullptr) */
	/* 	delete[] row_vec; */
	/* row_vec = new INT[row+1]; */
	ResizeRowVec(row+1);

	/* if(col_vec != nullptr) */
	/* 	delete[] col_vec; */
	/* col_vec = new INT[nnz]; */
	ResizeColVec(nnz);

	/* if(val != nullptr) */
	/* 	delete[] val; */
	/* val = new DOUBLE[nnz]; */
	ResizeValVec(nnz);

	COO2CSR(coo);

	return *this;
}


template<typename INT, typename DOUBLE>
void SpaCSR<INT,DOUBLE>::PrintPartialMat(INT num)
{
	for(INT i=0;i<num;i++)
	{
		INT begin_idx = this->row_vec[i];
		INT num_entry = this->row_vec[i+1] - begin_idx;
		for(INT j=0;j<num_entry;j++)
			std::cout<<i<<"   "<<this->col_vec[begin_idx+j]<<"   "<<this->val[begin_idx+j]<<std::endl;
	}
}
#endif
