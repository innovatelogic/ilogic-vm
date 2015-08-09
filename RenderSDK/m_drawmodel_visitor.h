#ifndef _m_drawmodel_visitor_h_
#define _m_drawmodel_visitor_h_ 

#pragma once

#include "m_visitor.h"
#include <vector>


class EXPORT m_drawmodel_visitor : public m_visitor
{

public:
	m_drawmodel_visitor();

	virtual void visit_model(const m_model * model);

	unsigned int    get_num_models() const;
	const m_model * get_model(unsigned int idx) const;
	const m_model * operator[](unsigned int) const;    

protected:
	std::vector<const m_model *> _models;

};


#endif //_m_drawmodel_visitor_h_
