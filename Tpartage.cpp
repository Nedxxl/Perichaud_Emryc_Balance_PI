#include "Tpartage.hpp"

TPartage::TPartage()
{
}

TPartage::~TPartage()
{
}

void TPartage::setPoidActuelle(float val, bool change)
{
    mutex.take();
    poidActuelle = val;

    changePoidActuelle = change;
    mutex.release();
}

void TPartage::setTareActuelle(float val, bool change)
{
    mutex.take();
    tareActuelle = val;

    changeTareActuelle = change;
    mutex.release();
}

void TPartage::setErreurActuelle(bool val, bool change)
{
    mutex.take();
    erreurActuelle = val;

    changeErreurActuelle = change;
    mutex.release();
}

void TPartage::setUniteMesure(char val, bool change)
{
    mutex.take();
    uniteMesure = val;

    changeUniteMesure = change;
    mutex.release();
}

float TPartage::getPoidActuelle(void)
{
    float ret;
    mutex.take();
    ret = poidActuelle;
    mutex.release();
    return ret;
}

float TPartage::getTareActuelle(void)
{
    float ret;
    mutex.take();
    ret = tareActuelle;
    mutex.release();
    return ret;
}

bool TPartage::getErreurActuelle(void)
{
    bool ret;
    mutex.take();
    ret = erreurActuelle;
    mutex.release();
    return ret;
}

char TPartage::getUniteMesure(void)
{
    char ret;
    mutex.take();
    ret = uniteMesure;
    mutex.release();
    return ret;
}

char* TPartage::sendChange(void)
{
    mutex.take();
    char ret[4] = {0, 0, 0, 0};
    if (changePoidActuelle)
    {
        ret[0] = 1;
        changePoidActuelle = false;
    }
    if (changeTareActuelle)
    {
        ret[1] = 1;
        changeTareActuelle = false;
    }
    if (changeErreurActuelle)
    {
        ret[2] = 1;
        changeErreurActuelle = false;
    }
    if (changeUniteMesure)
    {
        ret[3] = 1;
        changeUniteMesure = false;
    }

    mutex.release();
    char* result = new char[4];
    std::copy(std::begin(ret), std::end(ret), result);
    mutex.release();
    return result;
}
