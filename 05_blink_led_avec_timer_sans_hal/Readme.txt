mon document est vierge, est ce qu'il implémenté a la mémoir flash de la carte ou quoi ?
 C'est la question magique ! La réponse se cache dans un petit fichier "invisible" que STM32CubeIDE a ajouté pour toi : le Startup File (souvent nommé startup_stm32f103c8tx.s). Voici comment le processeur fait le lien entre ton code et le matériel : 
1. La Table des Vecteurs (Le GPS du processeur) Au tout début de la mémoire Flash (à l'adresse 0x08000000), il existe une liste d'adresses fixes appelée la Table des Vecteurs. C'est comme un annuaire :
 Position 0 : Adresse de la pile (Stack) 
Position 1 : Adresse du Reset ... 
Position 28 (pour le TIM2) : Adresse de la fonction TIM2_IRQHandler
 2. Le mécanisme du "Faible" (Weak Linkage) Dans le fichier de démarrage (en assembleur), les ingénieurs de ST ont déjà écrit tous les noms des fonctions (TIM2_IRQHandler, USART1_IRQHandler, etc.). Ils les ont déclarées comme [WEAK]. Cela signifie : "Cher compilateur, si l'utilisateur n'écrit pas de fonction nommée TIM2_IRQHandler, utilise une boucle infinie par défaut." "Mais si l'utilisateur écrit une fonction avec ce nom exact dans son main.c, ignore la mienne et utilise la sienne."
 3. Le saut matériel Quand ton Timer 2 arrive à 1000 : Le matériel (le circuit électrique interne) envoie un signal au NVIC. Le NVIC regarde dans la Table des Vecteurs à la 28ème ligne. Il y trouve l'adresse de ta fonction que tu as écrite en C.
 Il force le processeur à sauter à cette adresse. Où voir cela ?