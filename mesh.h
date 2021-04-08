#ifndef MESH_H
#define MESH_H
#include "tensor.hpp"
#include "sparsemat.hpp"
#include "base.h"

class Mesh
{
public:
	Mesh() = default;
	Mesh(const Mesh& ) = default;
	Mesh(Mesh&& ) = default;
	Mesh& operator=(const Mesh & ) = default;
	Mesh& operator=(Mesh && ) = default;
	virtual ~Mesh(){}

	virtual void InitMesh() = 0;

};

class QuadMesh:public Mesh
{
public:
	QuadMesh():Mesh(),x_min(0.0),x_max(1.0),y_min(0.0),y_max(1.0),x_num(2),y_num(2),num_node(9),num_edge(12),num_elem(4),node_cood(9,2),elem_node(4,4),edge_node(12,2),bd_edge(8,2){}
	QuadMesh(double x0,double x1,double y0,double y1,int nx,int ny):Mesh(),x_min(x0),x_max(x1),y_min(y0),y_max(y1),x_num(nx),y_num(ny),num_node((nx+1)*(ny+1)),num_edge(2*nx*ny+nx+ny),num_elem(nx*ny),node_cood((nx+1)*(ny+1),2),elem_node(nx*ny,4),edge_node(2*nx*ny+nx+ny,2),bd_edge(2*(nx+ny),2){}

	QuadMesh(const QuadMesh &) = default;
	QuadMesh(QuadMesh &&) = default;
	QuadMesh& operator=(const QuadMesh &) = default;
	QuadMesh& operator=(QuadMesh &&) = default;
	virtual ~QuadMesh(){}

	virtual void InitMesh();

	void GenNodeCood();
	void GenElemNode();
	void GenEdgeAndBdNode();
	void GenNode2Elem();

	void DisplayNodeCood();
	void DisplayEdgeNode();
	void DisplayElemNode();
	void DisplayBdNode();
	void DisplayNode2Elem();



protected:
	double x_min,x_max;
	double y_min,y_max;
	INT x_num,y_num;
	INT num_node;
	INT num_edge;
	INT num_elem;
public:
	// num_node * 2
	Mat<INT,double> node_cood; 
	// num_elem * 4
	Mat<INT,INT> elem_node;
	// num_edge * 2
	Mat<INT,INT> edge_node;
	// num of bd edge * 2
	Mat<INT,INT> bd_edge;
	// from node to elem using csr matrix: num_node, num_elem, 4*num_elem  
	SpaCSR<INT,bool> node2elem;
};

#endif
