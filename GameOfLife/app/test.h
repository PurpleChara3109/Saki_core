// test.h
#ifndef TEST_H_
#define TEST_H_

/**
 * @brief Exécute la machine à états de test.
 * Cette fonction doit être appelée de manière répétée dans la boucle principale (main) lorsque TEST_MODE est activé.
 * Sinon, les autres tests peuvent être effectuées sans activer cette variables, juste en les décommentant.
 */
void TEST_state_machine_run(void);

void TEST_screen(void);
void TEST_tactile(void);
#endif /* TEST_H_ */
