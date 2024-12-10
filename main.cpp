
//============================================================================
// Name        : TemplateThreadCom.cpp
// Author      : SG
// Version     :
// Copyright   : Your copyright notice
// Description : Template Thread Com
//============================================================================

#include "clavier.hpp"
#include "screen.hpp"
#include "Tpartage.hpp"
#include "TCom1.hpp"
#include "com.hpp"
#include "task1.hpp"

int main(int argc, char *argv[])
{
  char car;

  // Initialisation task Principal
  TThread::initTaskMain(SCHED_FIFO, 0);

  // Création Clavier et console
  TClavier *clavier = TClavier::getInstance();
  TScreen *screen = new TScreen();
  TTask1 *task1 = new TTask1("Task1", screen, SCHED_FIFO, 10);
  screen->start();

  // Création tâches
  TCom1 *com1 = new TCom1(TCom1::getComConfig("com.def").c_str(), screen, 10, TCom::b9600, TCom::pPAIR, TCom::dS7, 10000);

  // Démarrage tâches
  com1->start();
  task1->start();
  //com1->setSignalTimeout(1);
  // Traitement tâche principale
  screen->dispStr(1, 1, "Test Com (SG  09/09/2024)");

  do
  {
    // Traitement

    if (clavier->kbhit())
    {
      car = clavier->getch();

    }
  } while ((car != '|') && (car != '~'));

  // Destruction tâches
  if (com1)
    delete com1;

  if (task1)
    delete task1;

  if (screen)
    delete screen;

  return 0;

  // Destru    char str;
}