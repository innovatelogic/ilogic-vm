#include "m_factory.h"
#include "m_interface.h"

m_factory * m_factory::_the_factory = 0;

//----------------------------------------------------------------------------------
m_factory::m_factory()
{
    ;//
}

//----------------------------------------------------------------------------------
m_factory* m_factory::get_factory()
{
	if (_the_factory)
		return _the_factory;
	_the_factory = new m_factory();
	return _the_factory;
}

//----------------------------------------------------------------------------------
bool m_factory::shutdown()
{
	if (_the_factory)
	{
		delete _the_factory;
		_the_factory = 0;
		return true;
	}
	return false;
}

//----------------------------------------------------------------------------------
m_model * m_factory::new_model()
{
	m_model* model = new m_model;
	model->add_ref();
	return model;
}

//----------------------------------------------------------------------------------
m_node* m_factory::new_node()
{
	m_node * node = new m_node;
	node->add_ref();
	return node;
}

//----------------------------------------------------------------------------------
m_scene * m_factory::new_scene()
{
	m_scene * scene = new m_scene;
	scene->add_ref();
	return scene;
}

//----------------------------------------------------------------------------------
m_light * m_factory::new_light()
{
	m_light * light = new m_light;
	light->add_ref();
	return light;
}

//----------------------------------------------------------------------------------
m_camera * m_factory::new_camera()
{
	m_camera* camera = new m_camera;
	camera->add_ref();
	return camera;
}

//----------------------------------------------------------------------------------
m_drawmodel_visitor* m_factory::new_drawmodel_visitor()
{
    m_drawmodel_visitor * visitor = new m_drawmodel_visitor;
	visitor->add_ref();
	return visitor;
}

