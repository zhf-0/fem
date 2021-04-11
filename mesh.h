#ifndef MESH_H
#define MESH_H
#include "tensor.hpp"
#include "sparsemat.hpp"
#include "base.h"
#include <unordered_map>

class Mesh
{
public:
	Mesh() = default;
	Mesh(double x0,double x1,double y0,double y1,INT nx,INT ny,INT n_edge,INT n_elem,INT node_per_element):x_min(x0),x_max(x1),y_min(y0),y_max(y1),x_num(nx),y_num(ny),num_node((nx+1)*(ny+1)),num_edge(n_edge),num_elem(n_elem),node_per_elem(node_per_element),node_cood((nx+1)*(ny+1),2),elem_node(n_elem,node_per_element),edge_node(n_edge,2),bd_edge(2*(nx+ny),2){}
	Mesh(const Mesh& ) = default;
	Mesh(Mesh&& ) = default;
	Mesh& operator=(const Mesh & ) = default;
	Mesh& operator=(Mesh && ) = default;
	virtual ~Mesh(){}

	void InitMesh();

	void GenNodeCood();
	virtual void GenElemNode() = 0;
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
	INT node_per_elem;
public:
	// num_node * 2
	Mat<INT,double> node_cood; 
	// num_elem * 4|3
	Mat<INT,INT> elem_node;
	// num_edge * 2
	Mat<INT,INT> edge_node;
	// num of bd edge * 2
	Mat<INT,INT> bd_edge;
	// from node to elem using csr matrix: num_node, num_elem, 4|3*num_elem  
	// NO initialization by constructor
	SpaCSR<INT,bool> node2elem;
};

class QuadMesh:public Mesh
{
public:
	QuadMesh():Mesh(0.0,1.0,0.0,1.0,2,2,12,4,4){}
	QuadMesh(double x0,double x1,double y0,double y1,INT nx,INT ny):Mesh(x0,x1,y0,y1,nx,ny,2*nx*ny+nx+ny,nx*ny,4){}

	QuadMesh(const QuadMesh &) = default;
	QuadMesh(QuadMesh &&) = default;
	QuadMesh& operator=(const QuadMesh &) = default;
	QuadMesh& operator=(QuadMesh &&) = default;
	virtual ~QuadMesh(){}

	virtual void GenElemNode();

};

class TriMesh:public Mesh
{
public:
	TriMesh():Mesh(0.0,1.0,0.0,1.0,2,2,16,8,3){}
	TriMesh(double x0,double x1,double y0,double y1,int nx,int ny):Mesh(x0,x1,y0,y1,nx,ny,3*nx*ny+nx+ny,2*nx*ny,3){}

	TriMesh(const TriMesh &) = default;
	TriMesh(TriMesh &&) = default;
	TriMesh& operator=(const TriMesh &) = default;
	TriMesh& operator=(TriMesh &&) = default;
	virtual ~TriMesh(){}

	virtual void GenElemNode();
};
#endif
