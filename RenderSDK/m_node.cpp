#include "m_node.h"
#include "m_camera.h"
#include "m_model.h"
#include "m_light.h"
#include "m_streams.h"

//-----------------------------------------------------------------------
m_node::node_type m_node::type = ANONYMOUS;

//-----------------------------------------------------------------------
m_node::m_node()
: parent(BAD_IDX)
, target(BAD_IDX)
, Parent(NULL)
{
}

//-----------------------------------------------------------------------
m_node::~m_node()
{
 
}

//-----------------------------------------------------------------------
m_node::node_type m_node::get_type()const
{
	return type;
}

//-----------------------------------------------------------------------
m_camera* m_node::asCamera()
{
	return static_cast<m_camera*>(this);
}

//-----------------------------------------------------------------------
m_model* m_node::asModel()
{
	return static_cast<m_model*>(this);
}

//-----------------------------------------------------------------------
m_light* m_node::asLight()
{
	return static_cast<m_light*>(this);
}

//-----------------------------------------------------------------------
bool m_node::accept(const m_scene & scene, m_visitor & visitor) const
{
	for (unsigned int Index = 0; Index < childrens.size(); ++Index)
	{
        scene.nodes[childrens[Index]]->accept(scene, visitor);
	}
    
	visitor.visit_node(this);
    return true;
}

//-----------------------------------------------------------------------
m_node* m_node::find_node(const std::string& _name)
{
	if (name == _name){
		return this;
	}

	for (size_t Index = 0; Index < Childrens.size(); ++Index)
	{
		m_node * node_find = Childrens[Index]->find_node(_name);

		if (node_find){
			return node_find;
		}
	}
	return NULL;
}

//-----------------------------------------------------------------------
m_output_stream & operator << (m_output_stream & rOutputStream, const m_node & rNode)
{
   unsigned int nNameLength = (unsigned int)rNode.name.size();
   
   rOutputStream << nNameLength;

   rOutputStream.write(rNode.name.c_str(), nNameLength);

   rOutputStream << rNode.xform;

   rOutputStream << (unsigned int)rNode.childrens.size();

   for (size_t iChild = 0; iChild < rNode.childrens.size(); ++iChild)
   {
	   rOutputStream << rNode.childrens[iChild];
   }

  // rOutputStream << rNode.parent << rNode.target;

   rOutputStream << rNode.anim;

   rOutputStream << rNode.attr;

   return rOutputStream;
}

//-----------------------------------------------------------------------
m_input_stream & operator >> (m_input_stream & rInputStream, m_node & rNode)
{
	unsigned int nNameLength;

	rInputStream >> nNameLength;

	char * _name = new char[nNameLength + 1];
	assert(_name);
	rInputStream.read(_name, nNameLength);
	_name[nNameLength] = '\0';
	rNode.name = _name;
	delete [] _name;

   rInputStream >> rNode.xform;

   unsigned int num_childrens;
   rInputStream >> num_childrens;

   for (unsigned int iChild = 0; iChild < num_childrens; ++iChild)
   {
	   unsigned int Node;
	   rInputStream >> Node;
	   rNode.childrens.push_back(Node);
   }

  // rInputStream >> rNode.parent >> rNode.target;

   rInputStream >> rNode.anim;

  rInputStream >> rNode.attr;

   return rInputStream;
}
