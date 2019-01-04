#include <iostream>
#include <set>
// -------------------- OpenMesh

#include "OpenMesh/Core/IO/MeshIO.hh"
#include "OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh"
#include "OpenMesh/Core/Mesh/Traits.hh"
#include "OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh"
#include "OpenMesh/Core/Mesh/TriMeshT.hh"

// ----------------------------------------------------------------------------
#include "../src/LoopSubdivision.h"

int main()
{
	MyTriMesh mesh;
	MyTriMesh::VertexHandle vhandle[8];

	vhandle[0] = mesh.add_vertex(MyTriMesh::Point(-1, -1, 1));
	vhandle[1] = mesh.add_vertex(MyTriMesh::Point(1, -1, 1));
	vhandle[2] = mesh.add_vertex(MyTriMesh::Point(1, 1, 1));
	vhandle[3] = mesh.add_vertex(MyTriMesh::Point(-1, 1, 1));
	vhandle[4] = mesh.add_vertex(MyTriMesh::Point(-1, -1, -1));
	vhandle[5] = mesh.add_vertex(MyTriMesh::Point(1, -1, -1));
	vhandle[6] = mesh.add_vertex(MyTriMesh::Point(1, 1, -1));
	vhandle[7] = mesh.add_vertex(MyTriMesh::Point(-1, 1, -1));
	std::cout << "add vertex done" << std::endl;
	// generate (quadrilateral) faces
	std::vector<MyTriMesh::VertexHandle>  face_vhandles;
	face_vhandles.clear();
	face_vhandles.push_back(vhandle[0]);
	face_vhandles.push_back(vhandle[1]);
	face_vhandles.push_back(vhandle[2]);
	face_vhandles.push_back(vhandle[3]);
	mesh.add_face(face_vhandles);

	face_vhandles.clear();
	face_vhandles.push_back(vhandle[7]);
	face_vhandles.push_back(vhandle[6]);
	face_vhandles.push_back(vhandle[5]);
	face_vhandles.push_back(vhandle[4]);
	mesh.add_face(face_vhandles);
	face_vhandles.clear();
	face_vhandles.push_back(vhandle[1]);
	face_vhandles.push_back(vhandle[0]);
	face_vhandles.push_back(vhandle[4]);
	face_vhandles.push_back(vhandle[5]);
	mesh.add_face(face_vhandles);
	face_vhandles.clear();
	face_vhandles.push_back(vhandle[2]);
	face_vhandles.push_back(vhandle[1]);
	face_vhandles.push_back(vhandle[5]);
	face_vhandles.push_back(vhandle[6]);
	mesh.add_face(face_vhandles);
	face_vhandles.clear();
	face_vhandles.push_back(vhandle[3]);
	face_vhandles.push_back(vhandle[2]);
	face_vhandles.push_back(vhandle[6]);
	face_vhandles.push_back(vhandle[7]);
	mesh.add_face(face_vhandles);
	face_vhandles.clear();
	face_vhandles.push_back(vhandle[0]);
	face_vhandles.push_back(vhandle[3]);
	face_vhandles.push_back(vhandle[7]);
	face_vhandles.push_back(vhandle[4]);
	mesh.add_face(face_vhandles);

	MyTriMesh mesh2;
	try
	{
		if (!OpenMesh::IO::read_mesh(mesh2, "output_test_read.obj"))
		{
			std::cerr << "Cannot write mesh to file 'output.off'" << std::endl;
			return 1;
		}
	}
	catch (std::exception& x)
	{
		std::cerr << x.what() << std::endl;
		return 1;
	}
	
	for(int i = 0; i < 7; i++)
		LoopSubDividision(mesh2);

	try
	{
		if (!OpenMesh::IO::write_mesh(mesh2, "output_test_read_LS.off"))
		{
			std::cerr << "Cannot write mesh to file 'output.off'" << std::endl;
			return 1;
		}
	}
	catch (std::exception& x)
	{
		std::cerr << x.what() << std::endl;
		return 1;
	}
	//MyTriMesh2 mymesh22;

	LoopSubDividision(mesh);
	//mesh.
	// write mesh to output.obj
	try
	{
		if (!OpenMesh::IO::write_mesh(mesh, "output1_subdivide.off"))
		{
			std::cerr << "Cannot write mesh to file 'output.off'" << std::endl;
			return 1;
		}
	}
	catch (std::exception& x)
	{
		std::cerr << x.what() << std::endl;
		return 1;
	}
	return 0;
}