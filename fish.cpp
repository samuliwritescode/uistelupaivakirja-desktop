#include "fish.h"

Fish::Fish():
        m_species(NULL),
        m_method(eTrolling),
        m_weight(0),
        m_length(0),
        m_depth(0)
{
}

void Fish::setSpecies(Species* p_species)
{
    m_species = p_species;
}

void Fish::setMethod(EMethod p_method)
{
    m_method = p_method;
}

void Fish::setWeight(double p_weight)
{
    m_weight = p_weight;
}

void Fish::setLength(double p_length)
{
    m_length = p_length;
}

void Fish::setDepth(double p_depth)
{
    m_depth = p_depth;
}

Species* Fish::getSpecies()
{
    return m_species;
}

Fish::EMethod Fish::getMethod()
{
    return m_method;
}

double Fish::getWeight()
{
    return m_weight;
}

double Fish::getLength()
{
    return m_length;
}

double Fish::getDepth()
{
    return m_depth;
}
