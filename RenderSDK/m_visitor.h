#ifndef _m_visitor_h_
#define _m_visitor_h_

#pragma once

#include "m_refcount.h"

// forward declarations
struct m_model;
struct m_node;

class EXPORT m_visitor : public m_refcount
{
public:
	m_visitor();
	virtual ~m_visitor();
	virtual void visit_node(const m_node * node);
	virtual void visit_model(const m_model * model);

protected:
	
};

#endif //_m_visitor_h_