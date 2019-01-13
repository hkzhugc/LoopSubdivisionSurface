#include <iostream>
#include <set>
#include <string>
// -------------------- OpenMesh

#include "OpenMesh/Core/IO/MeshIO.hh"
#include "OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh"
#include "OpenMesh/Core/Mesh/Traits.hh"
#include "OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh"
#include "OpenMesh/Core/Mesh/TriMeshT.hh"

// ----------------------------------------------------------------------------
#include "../src/LoopSubdivision.h"
using namespace std;
int main(int argc, char* argv[])
{
	string outputfile_name;
	int sub_times;
	MyTriMesh mesh;
	if (argc < 4)
	{
		std::cerr << "command is too short\n"
			<< "--cube <subdivision times> <output_name> for default cube\n" 
			<< "--plane <subdivision times> <output_name> for default plane\n" 
			<< "--file <subdivision times> <input_name> <output_name> for customize input\n" << std::endl;
		return 1;
	}
	sub_times = atoi(argv[2]);
	if (string(argv[1]) == "--cube")
	{
		std::cout << "loop subdivide a default cube" << std::endl;
		outputfile_name = argv[3];
		BuildCube(mesh);
	}
	else if (string(argv[1]) == "--plane")
	{
		std::cout << "loop subdivide a default plane" << std::endl;
		outputfile_name = argv[3];
		BuildPlane(mesh);
	}
	else if (string(argv[1]) == "--file" && argc >= 5)
	{
		outputfile_name = argv[4];
		std::cout << "loop subdivide a input mesh : " << argv[3] << std::endl;
		try
		{
			if (!OpenMesh::IO::read_mesh(mesh, argv[3]))
			{
				std::cerr << "Cannot read mesh from file " << argv[3] << std::endl;
				return 1;
			}
		}
		catch (std::exception& x)
		{
			std::cerr << x.what() << std::endl;
			return 1;
		}
	}
	else
	{
		std::cerr << "unknown command\n"
			<< "--cube <subdivision times> <output_name> for default cube\n"
			<< "--plane <subdivision times> <output_name> for default plane\n"
			<< "--file <subdivision times> <input_name> <output_name> for customize input\n" << std::endl;
		return 1;
	}
	
	
	for(int i = 0; i < sub_times; i++)
		LoopSubDividision(mesh);

	try
	{
		if (!OpenMesh::IO::write_mesh(mesh, outputfile_name.c_str()))
		{
			std::cerr << "Cannot read mesh from file " << outputfile_name << std::endl;
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