#include "TCom1.hpp"

TCom1::TCom1(const char *name, void *shared, int priority, baudrate_t baudRate, parity_t parity, dimData_t dimData, int32_t timeoutRxMs)
	: TCom(name, shared, priority, baudRate, parity, dimData, timeoutRxMs), screen(nullptr)
{
	screen = (TScreen *)shared;
	screen->dispStr(1, 2, name);
	screen->dispStr(1, 3, com);
}

TCom1::~TCom1()
{
}

void TCom1::rxCar(unsigned char car)
{
	if (car == 0x02)
	{
		receiving = true;
		sum = 0;
		i = 0;
	}

	if (receiving)
	{
		recivedData[i] = car;
		sum += car;
		i++;
		if ((i == 18) && ((sum & 0b01111111) == 0) && ((recivedData[2] & 0b00000100) == 0))
		{
			receiving = false;
			i = 0;
			sum = 0;
			for (int n = 0; n < 6; n++)
			{
				poidRecu[n] = recivedData[n + 4];
				tareRecu[n] = recivedData[n + 10];
			}
			poidRecu[6] = (char)0;
			tareRecu[6] = (char)0;
			poids = atof(poidRecu);
			tare = atof(tareRecu);
			switch (recivedData[1] & 0b00000111)
			{
			case 1:
				screen->dispStr(1, 6, "Mode 1");
				// ignorer
				break;
			case 2:
				screen->dispStr(1, 6, "Mode 2");
				// ignorer
				break;
			case 3:
				screen->dispStr(1, 6, "Mode 3");
				poids = poids / 10;
				tare = tare / 10;
				if (partage->getPoidActuelle() != poids)
				{
					partage->setPoidActuelle(poids);
				}
				if (partage->getTareActuelle() != tare)
				{
					partage->setTareActuelle(tare);
				}
				break;
			case 4:
				screen->dispStr(1, 6, "Mode 4");
				poids = poids / 100;
				tare = tare / 100;
				if (partage->getPoidActuelle() != poids)
				{
					partage->setPoidActuelle(poids);
				}
				if (partage->getTareActuelle() != tare)
				{
					partage->setTareActuelle(tare);
				}
				break;
			case 5:
				screen->dispStr(1, 6, "Mode 5");
				poids = poids / 1000;
				tare = tare / 1000;
				if (partage->getPoidActuelle() != poids)
				{
					partage->setPoidActuelle(poids);
				}
				if (partage->getTareActuelle() != tare)
				{
					partage->setTareActuelle(tare);
				}
				break;
			default:
				screen->dispStr(1, 6, "Mode ?");
			}
			switch (recivedData[2] & 0b00010000)
			{
			case 0:

				if (partage->getUniteMesure() != 'l')
				{
					partage->setUniteMesure('l');
				}
				break;
			case 16:
				if (partage->getUniteMesure() != 'k')
				{
					partage->setUniteMesure('k');
				}
				break;
			default:
				screen->dispStr(1, 8, "Unite ?");
				break;
			}
		}
	}
}

void TCom1::rxTimeout(void)
{
	if (screen)
	{
		screen->dispStr(1, 4, "Receive timeout.");
	}
}

std::string TCom1::getComConfig(std::string nameFichierConfig)
{
	TConfiguration config(nameFichierConfig);
	return config.getParametreConfiguration();
}