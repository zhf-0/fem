#include "fem.h"

void QuadMesh::InitMesh()
{
	double x_step = (x_max-x_min)/x_num;
	double y_step = (y_max-y_min)/y_num;

	for(int i=0;i<=y_num;i++)
	{
		for(int j=0;j<=x_num;j++)
		{
			node_cood(i*(x_num+1)+j,0)= j*x_step; 
			node_cood(i*(x_num+1)+j,1)= i*y_step; 
		}
	}
}
