# Polytechnique Montréal

Département de génie informatique et génie logiciel

INF1900: Projet initial de système embarqué

# Grille de correction des programmes:

Identification:
+ Travail    : Mise en commun du code et formation de librairies
+ Section #  : 1
+ Équipe #   : 0626
+ Correcteur : Charles De Lafontaine

# CODE

| Points importants                                        | Poids       |
| -------------------------------------------------------- | ----------- |
| La qualités et le choix de vos portions de code choisies | (4.4/5.0)      |
| La qualités de vos modifications aux Makefiles           | (5/5.0)      |
| __Résultat partiel__                                     | __(9.4/10.0)__ |

# RAPPORT

| Points importants                                                           | Poids      |
| --------------------------------------------------------------------------- | ---------- |
| Explications cohérentes par rapport au code retenu pour former la librairie | (1.8/2.0)     |
| Explications cohérentes par rapport aux Makefiles modifiés                  | (1.8/2.0)     |
| Explications claires avec un bon niveau de détails                          | (1.8/2.0)     |
| Bon français                                                                | (1/1.0)     |
| __Résultat partiel__                                                        | __(6.4/7.0)__ |

# GÉNÉRALITÉS
## Bonne soumission de l'ensemble du code et du rapport selon le format demandé 

| Pénalité par erreur      | -0.5       |
| ------------------------ | ---------- |
| compilation sans erreurs |            |
| etc                      |            |
| __Résultat partiel__     | __(1.5/3.0)__ |


# Résultat

__Total des points: 17.3/20__

# Commentaires du correcteur:

## >>> RAPPORT <<< 

- ＤÉＴＡＩＬＳ:      9️⃣/🔟
- 🇫​​🇴​​🇷​​🇲​​🇦​​🇹​:       ✅
- 𝔽ℝ𝔸ℕℂ𝔸𝕀𝕊 (𝕊𝕋ℝ𝕀ℂ𝕋): ✅

## >>> MAKEFILES <<< 

- Makefile commun:         ❌
- Makefiles synthétisés:   ✅

## >>> COMPILATION <<< 

- Compilation automatique par le grader:  ❌ Attention !
- Aucun *warning*:                        ❌ Revoir librairie et votre définition de F_CPU.
- Aucune *error*:                         ✅
- Aucun fichiers indésirables:            ❌

## >>> CODE <<< 

- Revoir la nomenclature des dossiers, rien n'est compilé sur le grader.
- Main.cpp:             beaucoup de duplication de code et de variables magiques. Attention.
- Bouton.cpp:           ✅
- Bouton.h:             ✅
- Communication.cpp:    ✅
- Communication.h:      ✅
- Debug.cpp:            ✅
- Debug.h:              ✅
- LED.cpp:              attention, Led devrait être le nom de votre fichier pour respecter votre convention en place. Votre fichier Led ne devrait pas avoir la responsabilité de set F_CPU !
- LED.h:                ibid. Pour vos enum classes, la norme est SCREAMING_SNAKE_CASE, utilisez des '_' comme séparateurs (espaces).
- Navigation.cpp:       ✅
- Navigation.h:         ✅
- Timer.cpp:            beaucoup de variables magiques, attention.
- Timer.h:              ✅
- Wheel.cpp:            préférez l'utilisation des enum classes au lieu de variables magiques d'états.
- Wheel.h:              ✅

💧 Watermark: 🄲🄳🄻, 𝟤𝟦/𝟣𝟣/𝟤𝟥 ✅

# Basé sur le commit suivant
```
commit b6dad52019a5a8e17215efc1e72a43e52005d435
Author: Noah Blanchard <noah.blanchard@fastercom.ca>
Date:   Tue Oct 31 11:38:27 2023 -0400

    add doc
```

# Fichiers indésirables pertinents
Aucun

# Tous les fichiers indésirables
```
tp/tp7/lib_dir/libstatique.a
```

# Sorties de `make` dans les sous-répertoires

## Sortie de `make` dans `tp/tp7/lib`
```
make: *** correction_tp7/0626/tp/tp7/lib : Aucun fichier ou dossier de ce type. Arrêt.

```

## Sortie de `make` dans `tp/tp7/exec`
```
make: *** correction_tp7/0626/tp/tp7/exec : Aucun fichier ou dossier de ce type. Arrêt.

```
