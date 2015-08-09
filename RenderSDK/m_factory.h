#ifndef _m_factory_h_
#define _m_factory_h_

#pragma once

#include "m_stdafx.h"

struct m_model;
struct m_node;
struct m_scene;
struct m_light;
struct m_camera;
class  m_drawmodel_visitor;


class EXPORT m_factory
{

public:
	static m_factory *     get_factory();
	static bool            shutdown();

	m_scene *              new_scene();
	m_node *               new_node();
	m_model*               new_model();
	m_light *              new_light();
	m_camera *             new_camera();
	m_drawmodel_visitor *  new_drawmodel_visitor();


protected:
    m_factory(); 

	static m_factory*     _the_factory;
};


#endif //_m_factory_h_