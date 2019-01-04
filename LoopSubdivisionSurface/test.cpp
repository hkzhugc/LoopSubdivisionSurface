#include <iostream>
#include <set>
// -------------------- OpenMesh

#include "OpenMesh/Core/IO/MeshIO.hh"
#include "OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh"
#include "OpenMesh/Core/Mesh/Traits.hh"
#include "OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh"
#include "OpenMesh/Core/Mesh/TriMeshT.hh"

// ----------------------------------------------------------------------------
struct MyTraits : public OpenMesh::DefaultTraits
{
	// store new position of this vertex
	VertexTraits
	{
	private:
		Point  _newPosition; //endpoint new position
		bool _isNew;
	public:
		VertexT() : _newPosition(Point(0.0f, 0.0f, 0.0f)), _isNew(true) { }
		const Point& newPosition() const { return _newPosition; }
		void set_newPosition(const Point& _p) { _newPosition = _p; }
		const bool& isNew() const { return _isNew; }
		void set_isNew(const bool& _b) { _isNew = _b; }
		void update() { }
	};

	EdgeTraits
	{
	private:
		Point  _newPosition; //midpoint new position
		bool _isNew;
	public:
		EdgeT() : _newPosition(Point(0.0f, 0.0f, 0.0f)), _isNew(true) { }
		const Point& newPosition() const { return _newPosition; }
		const bool& isNew() const { return _isNew; }
		void set_newPosition(const Point& _p) { _newPosition = _p; }
		void set_isNew(const bool& _b) { _isNew = _b; }
	};
};

typedef OpenMesh::DefaultTraits::Point Point;
typedef OpenMesh::TriMesh_ArrayKernelT<MyTraits>  MyTriMesh;

void LoopSubDividision(MyTriMesh& mesh)
{
	// set the edges and vertices to be old ones;
	std::cout << "there are " << mesh.n_edges() << " edges before split" << std::endl;
	for (auto it = mesh.edges_begin(); it != mesh.edges_end(); ++it)
	{
		mesh.data(*it).set_isNew(false);
		Point newPosition(0, 0, 0);
		if (!mesh.is_boundary(*it))
		{
			//std::cout << "!is_boundary " << std::endl;
			//std::cout << "1cal newposition " << newPosition[0] << " , " << newPosition[1] << " , " << newPosition[2] << std::endl;
			auto he = mesh.halfedge_handle(it, 0);
			auto he_twin = mesh.opposite_halfedge_handle(he);

			auto v_connect1 = mesh.to_vertex_handle(he);
			auto temp_pos = mesh.point(v_connect1);
			//std::cout << "v_connect1 newposition " << temp_pos[0] << " , " << temp_pos[1] << " , " << temp_pos[2] << std::endl;
			auto v_connect2 = mesh.to_vertex_handle(he_twin);
			temp_pos = mesh.point(v_connect2);
			//std::cout << "v_connect2 newposition " << temp_pos[0] << " , " << temp_pos[1] << " , " << temp_pos[2] << std::endl;
			newPosition += 3.0 / 8.0 * (mesh.point(v_connect1) + mesh.point(v_connect2));
			//std::cout << "2cal newposition " << newPosition[0] << " , " << newPosition[1] << " , " << newPosition[2] << std::endl;

			auto he_far = mesh.next_halfedge_handle((he));
			auto he_twin_far = mesh.next_halfedge_handle((he_twin));

			auto v_far1 = mesh.to_vertex_handle(he_far);
			temp_pos = mesh.point(v_far1);
			//std::cout << "v_far1 newposition " << temp_pos[0] << " , " << temp_pos[1] << " , " << temp_pos[2] << std::endl;
			auto v_far2 = mesh.to_vertex_handle(he_twin_far);
			temp_pos = mesh.point(v_far2);
			//std::cout << "v_far2 newposition " << temp_pos[0] << " , " << temp_pos[1] << " , " << temp_pos[2] << std::endl;
			newPosition += 1.0 / 8.0 * (mesh.point(v_far1) + mesh.point(v_far2));
			//std::cout << "3cal newposition " << newPosition[0] << " , " << newPosition[1] << " , " << newPosition[2] << std::endl;
		}
		else
		{
			//std::cout << "is_boundary ";
			auto he = mesh.halfedge_handle(it, 0);
			auto he_twin = mesh.opposite_halfedge_handle(he);

			auto v_connect1 = mesh.to_vertex_handle(he);
			auto v_connect2 = mesh.to_vertex_handle(he_twin);
			newPosition += 1.0 / 2.0 * (mesh.point(v_connect1) + mesh.point(v_connect2));
		}
		mesh.data(*it).set_newPosition(newPosition);
		//std::cout << "cal newposition " << newPosition[0] << " , " << newPosition[1] << " , " << newPosition[2] << std::endl;
	}

	//for each old vertex, cal it's new pos
	for (auto it = mesh.vertices_begin(); it != mesh.vertices_end(); ++it)
	{
		mesh.data(*it).set_isNew(false);
		Point newPosition;
		if (!mesh.is_boundary(*it))
		{
			int degree = 0;
			Point sum(0, 0, 0);
			float beta;
			for (auto vvit = mesh.vv_iter(*it); vvit.is_valid(); ++vvit)
			{
				Point p = mesh.point(*vvit);
				sum += p;
				degree++;
			}
			if (degree == 3)
				beta = 3. / 16.;
			else
				beta = 3. / (8. * degree);
			newPosition = (1. - degree * beta) * mesh.point(*it) + beta * sum;
		}
		else
		{
			Point sum(0, 0, 0);
			int cnt = 0;
			for (auto vvit = mesh.vv_iter(*it); vvit.is_valid(); ++vvit)
			{
				if (mesh.is_boundary(*vvit))
				{
					sum += mesh.point(*vvit);
					cnt++;
				}
			}
			assert(cnt == 2);
			newPosition = 3.0 / 4.0 * mesh.point(*it) + 1.0 / 4 * sum;
		}
		mesh.data(*it).set_newPosition(newPosition);
	}

	//1st split the edge
	auto edge_end = mesh.edges_end();
	for (auto it = mesh.edges_begin(); it != edge_end; ++it)
	{
		if (!mesh.data(*it).isNew())
		{
			//std::cout << "is a old edge, split it" << std::endl;
			mesh.data(*it).set_isNew(false);
			auto e = mesh.edge(*it);
			auto he_handle = mesh.halfedge_handle(it, 0);

			auto to_v = mesh.to_vertex_handle(he_handle);
			auto from_v = mesh.from_vertex_handle(he_handle);
			auto to_p = mesh.point(to_v);
			auto from_p = mesh.point(from_v);
			auto mid_p = (to_p + from_p) / 2;
			//std::cout << " mid_point " << mid_p[0] << " , " << mid_p[1] << " , " << mid_p[2] << std::endl;
			mid_p = mesh.data(*it).newPosition();
			//std::cout << " newposition " << mid_p[0] << " , " << mid_p[1] << " , " << mid_p[2] << std::endl;
			auto new_v = mesh.split(it, mid_p);
			mesh.set_point(new_v, mesh.data(*it).newPosition());
			auto new_he1 = mesh.find_halfedge(new_v, from_v);
			auto new_he2 = mesh.find_halfedge(new_v, to_v);
			auto new_e1 = mesh.edge_handle(new_he1);
			auto new_e2 = mesh.edge_handle(new_he2);

			mesh.data(new_e1).set_isNew(false);
			mesh.data(new_e2).set_isNew(false);
		}
	}

	//2nd flip the edge
	int cnt_ = 0;
	for (auto it = mesh.edges_begin(); it != mesh.edges_end(); ++it)
	{
		if (mesh.data(*it).isNew())
		{
			auto he_handle = mesh.halfedge_handle(it, 0);
			auto to_v = mesh.to_vertex_handle(he_handle);
			auto from_v = mesh.from_vertex_handle(he_handle);
			//flip an edge if It is a new Edge and the two end points one is new, one is old
			if (mesh.is_flip_ok(it) && ((mesh.data(to_v).isNew() && !mesh.data(from_v).isNew()) || (!mesh.data(to_v).isNew() && mesh.data(from_v).isNew())))
			{
				//std::cout << "flip " << ++cnt_ << " edge" << std::endl;
				mesh.flip(it);
				//break;
			}
		}
	}

	//update the vertex new position
	for (auto it = mesh.vertices_begin(); it != mesh.vertices_end(); ++it)
	{
		if (!mesh.data(*it).isNew())
			mesh.set_point(it, mesh.data(*it).newPosition());
	}
	std::cout << "there are " << mesh.n_edges() << " edges after split" << std::endl;
}

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