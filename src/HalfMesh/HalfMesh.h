#pragma once
#include <list>
#include <glm/vec3.hpp>

using namespace std;
namespace B_rep
{
	class Vertex;
	class HalfEdge;
	class Edge;
	class Face;

	typedef list<Vertex>::iterator   VertexIter;
	typedef list<Edge>::iterator     EdgeIter;
	typedef list<Face>::iterator     FaceIter;
	typedef list<HalfEdge>::iterator HalfEdgeIter;
	typedef glm::vec3 Vector3f;

	class Face
	{
	public:
		Face() {};
		~Face() {};
		void RenderWireFrame();
		void RenderFace();
		HalfEdgeIter & half_edge() { return _half_edge; };
	private:
		HalfEdgeIter _half_edge;
	};

	class Edge
	{
	public:
		Edge() {};
		~Edge() {};

		HalfEdgeIter & half_edge() { return _half_edge; };
	private:
		HalfEdgeIter _half_edge;
	};

	class HalfEdge
	{
	public:
		HalfEdge() {};
		~HalfEdge() {};

		VertexIter & vertex() { return _vertex; }
		HalfEdgeIter &twin() { return _twin; }
		HalfEdgeIter &next() { return _next; }
		EdgeIter &edge() { return _edge; }
	private:
		HalfEdgeIter _twin;
		HalfEdgeIter _next;
		VertexIter _vertex;
		EdgeIter _edge;
	};

	class Vertex
	{
	public:
		Vertex() {};
		Vertex(Vector3f _pos) : pos(_pos){};
		~Vertex() {};
		Vector3f pos;
		HalfEdgeIter & half_edge() { return _half_edge; };
	private:
		HalfEdgeIter _half_edge;
	};

	class HalfMesh
	{
	public:
		HalfMesh();
		~HalfMesh();

		HalfEdgeIter newHalfedge() { return  _halfedges.insert(_halfedges.end(), HalfEdge()); }
		VertexIter   newVertex() { return   _vertices.insert(_vertices.end(), Vertex()); }
		VertexIter   newVertex(Vector3f _pos) { return   _vertices.insert(_vertices.end(), Vertex(_pos)); }
		EdgeIter     newEdge() { return      _edges.insert(_edges.end(), Edge()); }
		FaceIter     newFace() { return _faces.insert(_faces.end(), Face());}

	private:
		list<Face> _faces;
		list<Edge> _edges;
		list<HalfEdge> _halfedges;
		list<Vertex> _vertices;
	};
}
