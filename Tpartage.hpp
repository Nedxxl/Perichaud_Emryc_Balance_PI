#ifndef TPARTAGE_HPP
#define TPARTAGE_HPP

#include "mutex.hpp"
#include "singleton.hpp"
#include "sem.hpp"
#include <stdint.h>
#include <string.h>
#include "screen.hpp"
#include "mqtt.hpp"

class TPartage : public TSingleton<TPartage>
{
    friend class TSingleton<TPartage>;

private:
    float poidActuelle = 0;
    float tareActuelle = 0;
    bool erreurActuelle = false;
    char uniteMesure = 'k';

    bool changePoidActuelle = false;
    bool changeTareActuelle = false;
    bool changeErreurActuelle = false;
    bool changeUniteMesure = false;

public:
    TMutex mutex;
    TScreen *screen = new TScreen();
    TPartage();
    ~TPartage();

    void setPoidActuelle(float val, bool change = true);
    void setTareActuelle(float val, bool change = true);
    void setErreurActuelle(bool val, bool change = true);
    void setUniteMesure(char val, bool change = true);

    float getPoidActuelle(void);
    float getTareActuelle(void);
    bool getErreurActuelle(void);
    char getUniteMesure(void);

    char* sendChange(void);
};

#endif // TPARTAGE_HPP