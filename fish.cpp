#include "fish.h"

Fish::Fish():
        m_species(NULL),
        m_method(eTrolling),
        m_weight(0),
        m_width(0),
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

void Fish::setWidth(double p_width)
{
    m_width = p_width;
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

double Fish::getWidth()
{
    return m_width;
}

double Fish::getDepth()
{
    return m_depth;
}
