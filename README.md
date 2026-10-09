# Jeu-de-tir
Le jeu a réaliser est un jeu simple de tir qui se jouant en plusieurs rondes. Le joueur dispose d'une quantité de vie limité afin d'effectuer des tirs sur des cibles distinctes.
Ce projet consiste à créer un jeu de tir interactif avec un microcontrôleur ESP8266. Le joueur doit appuyer sur un bouton pour charger la puissance, puis le relâcher pour tirer sur une cible. La puissance est affichée à l’aide de DEL contrôlées par deux registres à décalage 74HC595. Le jeu utilise également des DEL pour indiquer les cibles, les points et les tirs manqués. Un buzzer actif produit des effets sonores pendant la partie. Un deuxième bouton permet d’activer ou de désactiver le son. Le programme gère les différentes étapes du jeu, le calcul du score, la victoire et la défaite.


# BOM – Jeu de tir

## Liste des composants

| Nom du composant | Quantité | Coût unitaire | Coût total |
|---|---:|---:|---:|
| ESP8266 | 1 | 6,00 $ | 6,00 $ |
| 74HC595 | 2 | 0,75 $ | 1,50 $ |
| Bouton-poussoir | 2 | 0,53 $ | 1,06 $ |
| DEL jaune | 3 | 0,25 $ | 0,75 $ |
| DEL verte | 3 | 0,28 $ | 0,85 $ |
| DEL rouge | 1 | 0,28 $ | 0,28 $ |
| Buzzer | 1 | 1,45 $ | 1,45 $ |
| Barre de DEL | 1 | 1,29 $ | 1,29 $ |
| Résistance de 220 Ω | 12 | 0,15 $ | 1,80 $ |
| **Prix total avant taxes** | | | **15,60 $** |

## Références

1. [Shift Register 8-Bit – DIP-16 IC (74HC595)](https://www.pishop.ca/product/shift-register-8-bit-sn74hc595/?searchid=0&search_query=74HC595)
2. [Colorful Square Tactile Button Switch Assortment – 15 pack](https://www.pishop.ca/product/colorful-square-tactile-button-switch-assortment-15-pack/?searchid=0&search_query=button)
3. [Diffused Yellow 3mm LED – 25 pack](https://www.pishop.ca/product/diffused-yellow-3mm-led-25-pack/)
4. [Diffused Green 3mm LED – 25 pack](https://www.pishop.ca/product/diffused-green-3mm-led-25-pack/)
5. [Diffused Red 3mm LED – 25 pack](https://www.pishop.ca/product/diffused-red-3mm-led-25-pack/)
6. [Passive Buzzer – 6V to 24V Tone Generator](https://www.pishop.ca/product/passive-buzzer/?searchid=0&search_query=buzze)
7. [10-Segment LED Graph Bar – Green](https://www.canadarobotix.com/products/1513?variant=32098818523185&country=CA&currency=CAD&utm_medium=product_sync&utm_source=google&utm_content=sag_organic&utm_campaign=sag_organic&srsltid=AU7gw4UWotsL3KkM3c5V_Z4c9jqRF5CBZTXxM_xxwoN64cN47M5C1zMzYKc)
8. [Resistor 220 Ω 1/4 W 5% Axial – Pack of 10](https://www.pishop.ca/product/resistor-220-ohm-14w-5-axial-pack-of-10/)

