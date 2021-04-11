#include "mesh.h"

//==================================================================
// class Mesh
void Mesh::InitMesh()
{
	GenNodeCood();
	GenElemNode();
	GenEdgeAndBdNode();
	GenNode2Elem();
}

void Mesh::GenNodeCood()
{
	double x_step = (x_max-x_min)/x_num;
	double y_step = (y_max-y_min)/y_num;

	// compute the node_cood matrix
	for(INT i=0;i<=y_num;i++)
	{
		for(INT j=0;j<=x_num;j++)
		{
			INT idx = i*(x_num+1)+j;
			node_cood(idx,0)= j*x_step; 
			node_cood(idx,1)= i*y_step; 
		}
	}

}

void Mesh::GenEdgeAndBdNode()
{
	SpaCOO<INT,int> *coo = new SpaCOO<INT,int>(num_node,num_node,num_elem*node_per_elem);

	long head, tail;
	for(INT i=0;i<num_elem;i++)
	{
		for(INT j=0;j<node_per_elem;j++)
		{
			head = elem_node(i,j%node_per_elem);
			tail = elem_node(i,(j+1)%node_per_elem);
			if(head > tail)
				std::swap(head,tail);

			INT idx = i*node_per_elem+j;
			coo->row_vec[idx] = head;
			coo->col_vec[idx] = tail;
			coo->val[idx] = 1; 
		}
	}

	SpaCSR<INT,int> csr;
	csr.RepeatedCOO2CSR((*coo));
	delete coo;

	INT bd_idx = 0;
	for(INT i=0;i<csr.GetRow();i++)
	{
		INT begin = csr.row_vec[i];
		INT end = csr.row_vec[i+1];
		for(INT j=begin;j<end;j++)
		{
			INT head = i;
			INT tail = csr.col_vec[j];
			edge_node(j,0) = head;
			edge_node(j,1) = tail;
			if(csr.val[j] ==1)
			{
				bd_edge(bd_idx,0) = head;
				bd_edge(bd_idx,1) = tail;
				bd_idx += 1;
			}
		}
	}

}

void Mesh::GenNode2Elem()
{
	SpaCOO<INT,bool> coo(num_node,num_elem,node_per_elem*num_elem,true);
	INT idx = 0;
	for(INT i=0;i<num_elem;i++)
	{
		for(INT j=0;j<node_per_elem;j++)
		{
			coo.row_vec[idx] = elem_node(i,j);
			coo.col_vec[idx] = i;
			idx += 1;
		}
	}

	node2elem = coo;
}

void Mesh::DisplayNodeCood()
{
	std::cout<<"node coodinate ";
	std::cout<<node_cood<<std::endl;
}

void Mesh::DisplayEdgeNode()
{
	std::cout<<"edge nodes ";
	std::cout<<edge_node<<std::endl;
}

void Mesh::DisplayElemNode()
{
	std::cout<<"element nodes ";
	std::cout<<elem_node<<std::endl;
}

void Mesh::DisplayBdNode()
{
	std::cout<<"bd edge ";
	std::cout<<bd_edge<<std::endl;
}

void Mesh::DisplayNode2Elem()
{
	std::cout<<"node to element ";
	node2elem.PrintPartialMat(num_node);
}

//==================================================================
// class QuadMesh

void QuadMesh::GenElemNode()
{
	// compute the elem_node matrix
	for(INT i=0;i<y_num;i++)
	{
		for(INT j=0;j<x_num;j++)
		{
			INT tmp1 = i*(x_num+1)+j;
			INT tmp2 = (i+1)*(x_num+1)+j;
			INT idx = i*x_num+j;

			elem_node(idx,0) = tmp1;
			elem_node(idx,1) = tmp1+1;
			elem_node(idx,2) = tmp2+1;
			elem_node(idx,3) = tmp2;
		}
	}

}

//==================================================================
// class TriMesh
void TriMesh::GenElemNode()
{
	// compute the elem_node matrix
	INT idx = 0;
	for(INT i=0;i<y_num;i++)
	{
		for(INT j=0;j<x_num;j++)
		{
			INT tmp1 = i*(x_num+1)+j;
			INT tmp2 = (i+1)*(x_num+1)+j;

			elem_node(idx,0) = tmp1;
			elem_node(idx,1) = tmp1+1;
			elem_node(idx,2) = tmp2+1;

			elem_node(idx+1,0) = tmp1;
			elem_node(idx+1,1) = tmp2+1;
			elem_node(idx+1,2) = tmp2;

			idx += 2;
		}
	}

}
