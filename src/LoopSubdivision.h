#include "OpenMesh/Core/Mesh/Traits.hh"
#include "OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh"
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

void LoopSubDividision(MyTriMesh& mesh);