Comprendre concept interruption avec UART:
L'analogie du Secrétaire
Imagine que tu es un grand patron (le CPU) et que tu as un secrétaire (le module UART). Tu dois envoyer une lettre de 7 pages ("wassim\n").
1. Le mode Normal (Polling) :
Tu restes debout devant l'imprimante. Tu poses la page 1, tu attends qu'elle s'imprime. Tu poses la page 2, tu attends...
Problème : Pendant ce temps, ton entreprise s'arrête. Tu ne réponds plus au téléphone, tu ne signes plus de contrats. Tu es bloqué.
2. Le mode Interruption (IT) :
Tu donnes les 7 pages à ton secrétaire et tu lui dis : "Tiens, envoie ça. Moi, je retourne travailler dans mon bureau."
Tu retournes à tes occupations (ton while(1)).
Le secrétaire envoie la page 1. Dès qu'elle est finie, il vient frapper à ta porte : "Patron, j'ai fini la page 1, je peux mettre la 2 ?".
Tu lèves la tête 1 microseconde, tu dis "Oui", et tu replonges dans ton travail.
Une fois les 7 pages envoyées, il vient une dernière fois : C'est le Callback. Il te dit : "C'est fini, tout est envoyé !"

Ce qu'il faut retenir pour ton code :
Le programme principal ne s'arrête pas : Il continue de lire tes capteurs ou de faire clignoter tes LED pendant que le texte s'envoie.
C'est automatique : C'est le matériel (l'UART) qui "tape à la porte" du CPU pour lui demander l'octet suivant.
Le danger : Si tu essaies d'envoyer un nouveau message alors que le secrétaire n'a pas fini les 7 pages précédentes, il va te dire qu'il est occupé (HAL_BUSY).
