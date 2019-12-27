#pragma once

#include <iostream>
#include <list>
#include <iterator>
#include <algorithm>

class Observable;

class Observateur
{
	public:
	virtual void Update(const Observable* observable) = 0;
	void AddObs(Observable* obs);
	void DelObs(Observable* obs);

	protected:
	std::list<Observable*> m_list;
	typedef std::list<Observable*>::iterator iterator; 
	typedef std::list<Observable*>::const_iterator const_iterator;
	virtual ~Observateur();
};

class Observable
{
	public:
	void AddObs( Observateur* obs);
	void DelObs(Observateur* obs);
	virtual ~Observable();
	
	protected:
	void Notify(void);

	private:
	std::list<Observateur*> m_list;
	typedef std::list<Observateur*>::iterator iterator; 
	typedef std::list<Observateur*>::const_iterator const_iterator;
};