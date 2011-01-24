#ifndef FISH_H
#define FISH_H
#include "species.h"

class Fish
{
public:

    enum EMethod {eTrolling, eCasting, eAngling, eIcefishing, eNet};
    Fish();

    void setSpecies(Species*);
    void setMethod(EMethod);
    void setWeight(double);
    void setLength(double);
    void setDepth(double);

    Species* getSpecies();
    EMethod getMethod();
    double getWeight();
    double getLength();
    double getDepth();

private:
    Species* m_species;
    EMethod m_method;
    double m_weight;
    double m_length;
    double m_depth;

};

#endif // FISH_H
