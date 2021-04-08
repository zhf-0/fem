#include "mesh.h"
#include <unordered_map>
#include "format.h"

void QuadMesh::InitMesh()
{
	GenNodeCood();
	GenElemNode();
	GenEdgeAndBdNode();
	GenNode2Elem();
}

void QuadMesh::GenNodeCood()
{
	double x_step = (x_max-x_min)/x_num;
	double y_step = (y_max-y_min)/y_num;

	// compute the node_cood matrix
	for(INT i=0;i<=y_num;i++)
	{
		for(INT j=0;j<=x_num;j++)
		{
			INT idx = i*(x_num+1)+j;
#if debug == 1
			node_cood.at(idx,0)= j*x_step; 
			node_cood.at(idx,1)= i*y_step; 
#endif
			node_cood(idx,0)= j*x_step; 
			node_cood(idx,1)= i*y_step; 
		}
	}

}

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
#if debug == 1
			elem_node.at(idx,0) = tmp1;
			elem_node.at(idx,1) = tmp1+1;
			elem_node.at(idx,2) = tmp2+1;
			elem_node.at(idx,3) = tmp2;
#endif
		}
	}

}


void QuadMesh::GenEdgeAndBdNode()
{
	// compute the edge_node matrix
	std::string key;
	std::unordered_map<std::string, bool> edge_map;
	INT edge_idx = 0;
	INT head, tail;
	for(INT i=0;i<num_elem;i++)
	{
		for(INT j=0;j<4;j++)
		{
			head = elem_node(i,j%4);
			tail = elem_node(i,(j+1)%4);
			if(head > tail)
				std::swap(head,tail);

			key = fmt::format("{},{}",head,tail);

			if(edge_map.find(key) == edge_map.end())
			{
				edge_map.emplace(key,0);
				edge_node(edge_idx,0) = head;
				edge_node(edge_idx,1) = tail;
				edge_idx += 1;
			}
			else
			{
				edge_map[key] = 1;
			}
		}

	}

	// compute the bd_edge matrix
	INT bd_idx = 0;
	for(INT i=0;i<num_edge;i++)
	{
		head = edge_node(i,0);
		tail = edge_node(i,1);
		key = fmt::format("{},{}",head,tail);
		if(edge_map[key] == 0)
		{
			bd_edge(bd_idx,0) = head;
			bd_edge(bd_idx,1) = tail;
			bd_idx += 1;
		}
	}
}

void QuadMesh::GenNode2Elem()
{
	SpaCOO<INT,bool> coo(num_node,num_elem,4*num_elem,true);
	INT idx = 0;
	for(INT i=0;i<num_elem;i++)
	{
		for(INT j=0;j<4;j++)
		{
			coo.row_vec[idx] = elem_node(i,j);
			coo.col_vec[idx] = i;
			idx += 1;
		}
	}

	node2elem = coo;
}

void QuadMesh::DisplayNodeCood()
{
	std::cout<<"node coodinate ";
	std::cout<<node_cood<<std::endl;
}

void QuadMesh::DisplayEdgeNode()
{
	std::cout<<"edge nodes ";
	std::cout<<edge_node<<std::endl;
}

void QuadMesh::DisplayElemNode()
{
	std::cout<<"element nodes ";
	std::cout<<elem_node<<std::endl;
}

void QuadMesh::DisplayBdNode()
{
	std::cout<<"bd edge ";
	std::cout<<bd_edge<<std::endl;
}

void QuadMesh::DisplayNode2Elem()
{
	std::cout<<"node to element ";
	node2elem.PrintPartialMat(num_node);
}
