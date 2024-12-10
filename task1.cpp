#include "task1.hpp"
#include "mqtt.hpp"
#include "Tpartage.hpp"
#include "screen.hpp"

TTask1::TTask1(const char *name, void *shared, int32_t policy, int32_t priority, int32_t cpu) : TThread(name, shared, policy, priority, cpu)
{
	std::string nameMqtt;

	screen = (TScreen *)shared;

	partage = TPartage::getInstance();

	nameMqtt = TMqtt::getMqttConfig();
	mqtt = new TMqtt(screen, "MqttBalance", nameMqtt.c_str());

	screen->dispStr(1, 10, "Task MQTT (CPU :  ) :");
	screen->dispStr(30, 10, nameMqtt.c_str());
}

TTask1::~TTask1()
{
	if (mqtt)
		delete mqtt;
}

void TTask1::task(void)
{
	// variable locale
	char strCar[2] = {'-', '\0'};

	// synchronisation démarrage tâche
	signalStart();

	sleep(2);

	while (1)
	{
		// traitement
		if (strCar[0] == '|')
			strCar[0] = '-';
		else
			strCar[0] = '|';
		screen->dispStr(23, 11, strCar);

		char *var = partage->sendChange();
		if (var[0] == 1)
		{
			char val[20];
			sprintf(val, "%.3f", partage->getPoidActuelle());
			mqtt->publish(NULL, "RAM/balance/etats/poids", strlen(val), val);
			screen->dispStr(1, 4, "Poids : " + std::to_string(partage->getPoidActuelle()));
		}
		if (var[1] == 1)
		{
			char val[20];
			sprintf(val, "%f", partage->getTareActuelle());
			mqtt->publish(NULL, "RAM/balance/etats/tare", strlen(val), val);
			screen->dispStr(1, 5, "Tare : " + std::to_string(partage->getTareActuelle()));
		}
		if (var[2] == 1)
		{
			char val[20];
			sprintf(val, "%d", partage->getErreurActuelle());
			mqtt->publish(NULL, "erreur", strlen(val), val);
			screen->dispStr(1, 7, "Erreur : " + std::to_string(partage->getErreurActuelle()));
		}
		if (var[3] == 1)
		{
			char val[20];
			sprintf(val, "%c", partage->getUniteMesure());
			mqtt->publish(NULL, "RAM/balance/etats/unite", strlen(val), val);
			screen->dispStr(1, 8, "Unite : " + std::to_string(partage->getUniteMesure()));
		}

		if (mqtt->loop(100) != 0)
			mqtt->reconnect();

		//usleep(250000); // 250 ms
	}
}
