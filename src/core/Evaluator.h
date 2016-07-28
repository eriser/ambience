#ifndef EVALUATOR_H
#define EVALUATOR_H

namespace ambience
{

class Individual;

class Evaluator
{
public:
    virtual ~Evaluator(){}
    virtual float evaluate( const Individual & individual ) = 0;
	virtual std::string getName()
	{
		return typeid(*this).name();
	}
};

}

#endif
