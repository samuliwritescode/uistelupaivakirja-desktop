#ifndef FISH_H
#define FISH_H
#include "species.h"
#include "method.h"

class Fish
{
public:

    Fish();

    void setSpecies(Species*);
    void setMethod(Method*);
    void setWeight(double);
    void setLength(double);
    void setDepth(double);

    Species* getSpecies();
    Method* getMethod();
    double getWeight();
    double getLength();
    double getDepth();

private:
    Species* m_species;
    Method* m_method;
    double m_weight;
    double m_length;
    double m_depth;

};

#endif // FISH_H