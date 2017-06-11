/****************************************************************************
 *
 * Spell System
 * Copyright (c) 2007 Antrix Team
 * Copyright (c) 2010 Waad Team
 *
 * This file may be distributed under the terms of the Q Public License
 * as defined by Trolltech ASA of Norway and appearing in the file
 * COPYING included in the packaging of this file.
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#ifndef _SPELLFAILURE_H
#define _SPELLFAILURE_H

// Verif 335.12340 Ok (Branruz)
enum SpellCastError                                  // Message Client
{
	SPELL_FAILED_SUCCESS                        = 0, // (neutralise le spell sans messages)
	SPELL_FAILED_AFFECTING_COMBAT               = 1, // "Vous etes en train de combattre"
	SPELL_FAILED_ALREADY_AT_FULL_HEALTH         = 2, // "Vos PV sont deja au maximum"
	SPELL_FAILED_ALREADY_AT_FULL_MANA           = 3, // "Vous avez deja tout votre mana"
	SPELL_FAILED_ALREADY_AT_FULL_POWER          = 4, // "Vous etes deja au maximum de mana"
	SPELL_FAILED_ALREADY_BEING_TAMED            = 5, // "Cette créature est déja dompté"
	SPELL_FAILED_ALREADY_HAVE_CHARM             = 6, // "Vous controlez deja une créature charmé"
	SPELL_FAILED_ALREADY_HAVE_SUMMON            = 7, // "Vous controlez deja une créature invoqué"
	SPELL_FAILED_ALREADY_OPEN                   = 8, // "Déja ouvert"
	SPELL_FAILED_AURA_BOUNCED                   = 9, // "Un sort plus puissant est déja actif"
	SPELL_FAILED_AUTOTRACK_INTERRUPTED          = 10, // (neutralise le spell sans messages)
	SPELL_FAILED_BAD_IMPLICIT_TARGETS           = 11, // "Vous n'avez pas de cible"
	SPELL_FAILED_BAD_TARGETS                    = 12, // "Cible incorrect" "Cet objet n'est pas une cible autorisée"
	SPELL_FAILED_CANT_BE_CHARMED                = 13, // "La cible ne peut pas etre charmé"
	SPELL_FAILED_CANT_BE_DISENCHANTED           = 14, // "Impossible de désenchanté cet objet"
	SPELL_FAILED_CANT_BE_DISENCHANTED_SKILL     = 15, // "Votre compétence d'enchantement n'est pas suffisant pour désenchanter ceci"
	SPELL_FAILED_CANT_BE_MILLED                 = 16, // "Vous ne pouvez pas piler ca"
	SPELL_FAILED_CANT_BE_PROSPECTED             = 17, // "Il n'y a pas de Gem ici"
	SPELL_FAILED_CANT_CAST_ON_TAPPED            = 18, // "Cible indisponible"
	SPELL_FAILED_CANT_DUEL_WHILE_INVISIBLE      = 19, // "Vous ne pouvez pas commencer un duel si vous etes invisible"
	SPELL_FAILED_CANT_DUEL_WHILE_STEALTHED      = 20, // "On ne peut pas commencer un duel quand on est camouflé"
	SPELL_FAILED_CANT_STEALTH                   = 21, // "Vous etes trop pres des ennemies"
	SPELL_FAILED_CASTER_AURASTATE               = 22, // "Vous ne pouvez pas encore faire cela"
	SPELL_FAILED_CASTER_DEAD                    = 23, // "Vous avez succombé"
	SPELL_FAILED_CHARMED                        = 24, // "Vous ne pouvez pas encore faire cela tant que vous etes charmé"
	SPELL_FAILED_CHEST_IN_USE                   = 25, // "Ceci est déja en cours d'utilisation"
	SPELL_FAILED_CONFUSED                       = 26, // "Impossible sous confusion"
	SPELL_FAILED_DONT_REPORT                    = 27, // (neutralise le spell sans messages)
	SPELL_FAILED_EQUIPPED_ITEM                  = 28, // "Doit equiper l'objet adéquate"
	SPELL_FAILED_EQUIPPED_ITEM_CLASS            = 29, // "Doit avoir équiper (%s)"
	SPELL_FAILED_EQUIPPED_ITEM_CLASS_MAINHAND   = 30, // "Doit avoir équiper en main droite : (%s)"
	SPELL_FAILED_EQUIPPED_ITEM_CLASS_OFFHAND    = 31, // "Doit avoir équiper en main gauche : (%s)"
	SPELL_FAILED_ERROR                          = 32, // "Erreur Interne"
	SPELL_FAILED_FIZZLE                         = 33, // "Echec de l'incantation"
	SPELL_FAILED_FLEEING                        = 34, // "Impossible en fuite"
	SPELL_FAILED_FOOD_LOWLEVEL                  = 35, // "Cette nourriture est d'un niveau trop faible pour votre familier"
	SPELL_FAILED_HIGHLEVEL                      = 36, // "Le niveau de la cible est trop élevé"
	SPELL_FAILED_HUNGER_SATIATED                = 37, // (neutralise le spell sans messages) 
	SPELL_FAILED_IMMUNE                         = 38, // "Insensible"
	SPELL_FAILED_INCORRECT_AREA                 = 39, // "Vous n'etes pas dans la bonne region"
	SPELL_FAILED_INTERRUPTED                    = 40, // "Interrompu"
	SPELL_FAILED_INTERRUPTED_COMBAT             = 41, // "Interrompu"
	SPELL_FAILED_ITEM_ALREADY_ENCHANTED         = 42, // "L'objet est deja enchanté"
	SPELL_FAILED_ITEM_GONE                      = 43, // "L'objet n'est plus disponible"
	SPELL_FAILED_ITEM_NOT_FOUND                 = 44, // "Essaie d'enchanter un objet qui n'existe pas"
	SPELL_FAILED_ITEM_NOT_READY                 = 45, // "L'objet n'est pas encore pret"
	SPELL_FAILED_LEVEL_REQUIREMENT              = 46, // "Votre niveau est insuffisant"
	SPELL_FAILED_LINE_OF_SIGHT                  = 47, // "Cible hors du champ de vision"
	SPELL_FAILED_LOWLEVEL                       = 48, // "Le niveau de la cible est trop bas"
	SPELL_FAILED_LOW_CASTLEVEL                  = 49, // "Niveau de la technique insuffisant"
	SPELL_FAILED_MAINHAND_EMPTY                 = 50, // "Vous n'avez pas d'armes en main"
	SPELL_FAILED_MOVING                         = 51, // "Impossible en cours de deplacement"
	SPELL_FAILED_NEED_AMMO                      = 52, // "Les minutions doivent etre placés...etc", fait apparaitre la fenetre Help sur les munitions
	SPELL_FAILED_NEED_AMMO_POUCH                = 53, // "Requiert (%s)"
	SPELL_FAILED_NEED_EXOTIC_AMMO               = 54, // "Requiert des munitions spéciales (%s)"
	SPELL_FAILED_NEED_MORE_ITEMS                = 55, // (neutralise le spell sans messages) 
	SPELL_FAILED_NOPATH                         = 56, // "Aucun chemin disponible"
	SPELL_FAILED_NOT_BEHIND                     = 57, // "Vous deves etre derriere votre cible"
	SPELL_FAILED_NOT_FISHABLE                   = 58, // "Votre ligne n'a pas atteint d'eau poissoneuses"
	SPELL_FAILED_NOT_FLYING                     = 59, // "Vous volez"
	SPELL_FAILED_NOT_HERE                       = 60, // "Inutilisable ici"
	SPELL_FAILED_NOT_INFRONT                    = 61, // "Vous devez etre en face de votre cible"
	SPELL_FAILED_NOT_IN_CONTROL                 = 62, // "Vous ne controlez pas vos actions"
	SPELL_FAILED_NOT_KNOWN                      = 63, // "Sort non appris"
	SPELL_FAILED_NOT_MOUNTED                    = 64, // "Vous etes sur une monture"
	SPELL_FAILED_NOT_ON_TAXI                    = 65, // "Vous etes en vol"
	SPELL_FAILED_NOT_ON_TRANSPORT               = 66, // "Vous etes sur un transport"
	SPELL_FAILED_NOT_READY                      = 67, // "Le sort n'est pas encore utilisable"
	SPELL_FAILED_NOT_SHAPESHIFT                 = 68, // "Vous etes sous une autre forme"
	SPELL_FAILED_NOT_STANDING                   = 69, // "Vous devez etre debout pour faire ca"
	SPELL_FAILED_NOT_TRADEABLE                  = 70, // "Vous ne pouvez utiliser ceci que sur un objet que vous possedez"
	SPELL_FAILED_NOT_TRADING                    = 71, // "Essaye d'echnager un objet d'artisanat mais aucun echange en cours"
	SPELL_FAILED_NOT_UNSHEATHED                 = 72, // "Il faut avoir dégainé pour faire cela" (message jaune)
	SPELL_FAILED_NOT_WHILE_GHOST                = 73, // "Les fantomes ne peuvent pas lancer de sort"
	SPELL_FAILED_NOT_WHILE_LOOTING              = 74, // "Vous etes occupez a fouiller"
	SPELL_FAILED_NO_AMMO                        = 75, // "Plus de munitions"
	SPELL_FAILED_NO_CHARGES_REMAIN              = 76, // "Plus de charge"
	SPELL_FAILED_NO_CHAMPION                    = 77, // "Vous n'avez pas choisi de champion"
	SPELL_FAILED_NO_COMBO_POINTS                = 78, // "Cette technique requiert des points de combo"
	SPELL_FAILED_NO_DUELING                     = 79, // "Le duel n'est pas autorisé ici"
	SPELL_FAILED_NO_ENDURANCE                   = 80, // "Endurance insufisante"
	SPELL_FAILED_NO_FISH                        = 81, // "Il n'y a aucun poisson ici"
	SPELL_FAILED_NO_ITEMS_WHILE_SHAPESHIFTED    = 82, // "Impossible d'utiliser cet objet lorsqu'on est sous une autre forme"
	SPELL_FAILED_NO_MOUNTS_ALLOWED              = 83, // "Vous ne pouvez pas utiliser une monture ici"
	SPELL_FAILED_NO_PET                         = 84, // "Vous n'avez pas de familier"
	SPELL_FAILED_NO_POWER                       = 85, // "Pas assez de mana"
	SPELL_FAILED_NOTHING_TO_DISPEL              = 86, // "Rien a dissiper"
	SPELL_FAILED_NOTHING_TO_STEAL               = 87, // "Rien à derober"
	SPELL_FAILED_ONLY_ABOVEWATER                = 88, // "Inutilisable quand vous nagez"
	SPELL_FAILED_ONLY_DAYTIME                   = 89, // "A utiliser le jour"
	SPELL_FAILED_ONLY_INDOORS                   = 90, // "A utiliser en interieur"
	SPELL_FAILED_ONLY_MOUNTED                   = 91, // "Utilisable seulement sur un monture"
	SPELL_FAILED_ONLY_NIGHTTIME                 = 92, // "A utiliser la nuit"
	SPELL_FAILED_ONLY_OUTDOORS                  = 93, // "A utiliser en exterieur"
	SPELL_FAILED_ONLY_SHAPESHIFT                = 94, // (neutralise le spell sans messages) 
	SPELL_FAILED_ONLY_STEALTHED                 = 95, // "Vous devez etre en mode camouflage"
	SPELL_FAILED_ONLY_UNDERWATER                = 96, // "Utilisable seulement quand on nage"
	SPELL_FAILED_OUT_OF_RANGE                   = 97, // "Hors de portée"
	SPELL_FAILED_PACIFIED                       = 98, // "Impossible d'utiliser cette technique quand pacifié"
	SPELL_FAILED_POSSESSED                      = 99, // "Quelque chose vous possede"
	SPELL_FAILED_REAGENTS                       = 100, // (neutralise le spell sans messages) 
	SPELL_FAILED_REQUIRES_AREA                  = 101, // "Vous devez etre dans (%s)"
	SPELL_FAILED_REQUIRES_SPELL_FOCUS           = 102, // "Requiert (%s)"
	SPELL_FAILED_ROOTED                         = 103, // "Vous etes cloué sur place"
	SPELL_FAILED_SILENCED                       = 104, // "Impossible lorsque reduit au silence"
	SPELL_FAILED_SPELL_IN_PROGRESS              = 105, // "Une autre action est en cours"
	SPELL_FAILED_SPELL_LEARNED                  = 106, // "Vous avez deja appris ce sort"
	SPELL_FAILED_SPELL_UNAVAILABLE              = 107, // "Le sort ne vous est pas disponible"
	SPELL_FAILED_STUNNED                        = 108, // "Impossible quand etourdi(e)"
	SPELL_FAILED_TARGETS_DEAD                   = 109, // "Votre cible est morte"
	SPELL_FAILED_TARGET_AFFECTING_COMBAT        = 110, // "La cible combat"
	SPELL_FAILED_TARGET_AURASTATE               = 111, // "Vous ne pouvez pas encore faire cela"
	SPELL_FAILED_TARGET_DUELING                 = 112, // "La cible est engagé dans un duel"
	SPELL_FAILED_TARGET_ENEMY                   = 113, // "La cible est hostile"
	SPELL_FAILED_TARGET_ENRAGED                 = 114, // "La cible est trop enragé pour etre charmé"
	SPELL_FAILED_TARGET_FRIENDLY                = 115, // "La cible est amicale"
	SPELL_FAILED_TARGET_IN_COMBAT               = 116, // "La cible ne doit pas etre en train de combatre"
	SPELL_FAILED_TARGET_IS_PLAYER               = 117, // "Impossible de cibler les joueurs"
	SPELL_FAILED_TARGET_IS_PLAYER_CONTROLLED    = 118, // "Impossible de cibler les unites controlé par le personnage"
	SPELL_FAILED_TARGET_NOT_DEAD                = 119, // "La cible est en vie"
	SPELL_FAILED_TARGET_NOT_IN_PARTY            = 120, // "La cible n'appartient pas a votre groupe"
	SPELL_FAILED_TARGET_NOT_LOOTED              = 121, // "Il faut d'abord fouiller les créatures"
	SPELL_FAILED_TARGET_NOT_PLAYER              = 122, // "La cible n'est pas un personnage joueur"
	SPELL_FAILED_TARGET_NO_POCKETS              = 123, // "Pas de poche a vider"
	SPELL_FAILED_TARGET_NO_WEAPONS              = 124, // "La cible n'a pas d'armes equipée"
	SPELL_FAILED_TARGET_NO_RANGED_WEAPONS       = 125, // "La cible n'a pas d'armes à distance equipée"
	SPELL_FAILED_TARGET_UNSKINNABLE             = 126, // "Impossible d'écorcher cette créature"
	SPELL_FAILED_THIRST_SATIATED                = 127, // (neutralise le spell sans messages) 
	SPELL_FAILED_TOO_CLOSE                      = 128, // "Cible trop proche"
	SPELL_FAILED_TOO_MANY_OF_ITEM               = 129, // "Vous avez deja cette element en trop grand nombre"
	SPELL_FAILED_TOTEM_CATEGORY                 = 130, // (pas de message, attente d'un id item comme param) 
	SPELL_FAILED_TOTEMS                         = 131, // (pas de message, attente d'un id item comme param) 
	SPELL_FAILED_TRY_AGAIN                      = 132, // "Echec de la tentative"
	SPELL_FAILED_UNIT_NOT_BEHIND                = 133, // "Les cibles doivent etre derriere vous"
	SPELL_FAILED_UNIT_NOT_INFRONT               = 134, // "Les cibles doivent etre devant vous"
	SPELL_FAILED_WRONG_PET_FOOD                 = 135, // "Votre familier n'aime pas cette nourriture"
	SPELL_FAILED_NOT_WHILE_FATIGUED             = 136, // "La fatigue vous empeche de lancer des sorts"
	SPELL_FAILED_TARGET_NOT_IN_INSTANCE         = 137, // "La cible doit etre dans cette instance"
	SPELL_FAILED_NOT_WHILE_TRADING              = 138, // "Impossible de lancer un sort pendant une transaction"
	SPELL_FAILED_TARGET_NOT_IN_RAID             = 139, // "La cible n'est pas dans groupe ou dans votre groupe de raid"
	SPELL_FAILED_TARGET_FREEFORALL              = 140, // "La cible est actuellement en combat JcJ, chacun pour soi"
	SPELL_FAILED_NO_EDIBLE_CORPSES              = 141, // "Aucun cadavre a devorer a proximité"
	SPELL_FAILED_ONLY_BATTLEGROUNDS             = 142, // "Utilisable uniquement sur des champs de bataille"
	SPELL_FAILED_TARGET_NOT_GHOST               = 143, // "La cible n'est pas un fantome"
	SPELL_FAILED_TRANSFORM_UNUSABLE             = 144, // "Vous ne pouvez pas utiliser le nouvelle objet"
	SPELL_FAILED_WRONG_WEATHER                  = 145, // "La météo ne le permet pas"
	SPELL_FAILED_DAMAGE_IMMUNE                  = 146, // "Vous ne pouvez pas faire cela tant que vous etes insensible"
	SPELL_FAILED_PREVENTED_BY_MECHANIC          = 147, // "Impossible lorsque vous etes (%s)"
	SPELL_FAILED_PLAY_TIME                      = 148, // "Temps de jeux maximum dépassé"
	SPELL_FAILED_REPUTATION                     = 149, // "Votre réputation est insuffisante"
	SPELL_FAILED_MIN_SKILL                      = 150, // "Votre comptétence n'est pas assez élevé : (%d) requis"
	SPELL_FAILED_NOT_IN_ARENA                   = 151, // "Impossible de faire ca dans une arene"
	SPELL_FAILED_NOT_ON_SHAPESHIFT              = 152, // "Ce sort ne peut etre lancer sur une cible ayant changé de forme"
	SPELL_FAILED_NOT_ON_STEALTHED               = 153, // "Ce sort ne peut etre lancé sur une cible camouflé"
	SPELL_FAILED_NOT_ON_DAMAGE_IMMUNE           = 154, // "Ce sort ne peut etre lancé sur une cible insensible aux degats"
	SPELL_FAILED_NOT_ON_MOUNTED                 = 155, // "Ce sort ne peut etre lancé sur une cible chevauchant une monture"
	SPELL_FAILED_TOO_SHALLOW                    = 156, // "L'eau n'est pas assez profonde"
	SPELL_FAILED_TARGET_NOT_IN_SANCTUARY        = 157, // "La cible n'est pas dans un sanctuaire"
	SPELL_FAILED_TARGET_IS_TRIVIAL              = 158, // "Impossible de cibler les éléments négligeables"
	SPELL_FAILED_BM_OR_INVISGOD                 = 159, // "Ce sort ne peut etre lancer sur des invisibles, divains ou maitre des betes"
	SPELL_FAILED_EXPERT_RIDING_REQUIREMENT      = 160, // "Requiert une competence de monte (expert)"
	SPELL_FAILED_ARTISAN_RIDING_REQUIREMENT     = 161, // "Requiert une competence de monte (artisan)"
	SPELL_FAILED_NOT_IDLE                       = 162, // "Impossible a utiliser en état oisif"
	SPELL_FAILED_NOT_INACTIVE                   = 163, // "Impossible a utiliser en étant inactif"
	SPELL_FAILED_PARTIAL_PLAYTIME               = 164, // "Vous ne pouvez pas faire vous avez plus de 3H de temps en ligne"
	SPELL_FAILED_NO_PLAYTIME                    = 165, // "Vous ne pouvez pas faire vous avez plus de 5H de temps en ligne
	SPELL_FAILED_NOT_IN_BATTLEGROUND            = 166, // "Impossible de faire ca dans un champ de bataille"
	SPELL_FAILED_NOT_IN_RAID_INSTANCE           = 167, // "Impossible dans une instance de raid"
	SPELL_FAILED_ONLY_IN_ARENA                  = 168, // "Vous ne pouvez faire ca que dans une arene"
	SPELL_FAILED_TARGET_LOCKED_TO_RAID_INSTANCE = 169, // "La cible est inscrit dans une autre instance de raid"
	SPELL_FAILED_ON_USE_ENCHANT                 = 170, // "Cet objet a deja une technique activé"
	SPELL_FAILED_NOT_ON_GROUND                  = 171, // "Impossible a utiliser au sol"
	SPELL_FAILED_CUSTOM_ERROR                   = 172, // (pas de message, attente d'un id comme param) 
	SPELL_FAILED_TOO_MANY_SOCKETS               = 173, // "Vous ne pouvez pas faire ca maintenant"
	SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW         = 174, // "Cet objet a trop de chasse"
	SPELL_FAILED_INVALID_GLYPH                  = 175, // "Impossible de caligraphier ce glyche dans cet emplacement"
	SPELL_FAILED_UNIQUE_GLYPH                   = 176, // "Ce glyphe est déja caligraphier dans votre grimoire"
	SPELL_FAILED_GLYPH_SOCKET_LOCKED            = 177, // "Vous ne pouvez pas encore caligraphier ce glyphe ici"
	SPELL_FAILED_NO_VALID_TARGETS               = 178, // "Pas de cible autorisé"
	SPELL_FAILED_ITEM_AT_MAX_CHARGES            = 179, // "Cet objet a deja le nombre maximum de charge"
	SPELL_FAILED_NOT_IN_BARBERSHOP              = 180, // "Vous ne pouvez pas faire ca dans un salon de coiffure"
	SPELL_FAILED_FISHING_TOO_LOW                = 181, // "Requiert (%d) en peche"
	SPELL_FAILED_ITEM_ENCHANT_TRADE_WINDOW      = 182, // "Impossible de désenchanter cet objet lors d'un echange"
	SPELL_FAILED_SUMMON_PENDING                 = 183, // "Il y a deja une invocation en cours"
	SPELL_FAILED_MAX_SOCKETS                    = 184, // "Cet objet ne peut recevoir de chasse supplementaire"
	SPELL_FAILED_PET_CAN_RENAME                 = 185, // "Vous pouvez renommer votre familier"
	SPELL_FAILED_TARGET_CANNOT_BE_RESURRECTED   = 186, // "La cible ne peut pas etre réssucité"
	// Apres 186, toujours le meme message
	SPELL_FAILED_UNKNOWN                        = 187, // "Raison inconnue"
	SPELL_FAILED_UNKNOWN2                       = 188, // "Raison inconnue"
	SPELL_FAILED_UNKNOWN3                       = 189, // "Raison inconnue"
	SPELL_FAILED_UNKNOWN4                       = 190, // "Raison inconnue"
	SPELL_FAILED_UNKNOWN5                       = 191, // (pas de message)
	SPELL_FAILED_UNKNOWN6                       = 192, // "Raison inconnue"
	SPELL_FAILED_MAX
};
#define SPELL_CANCAST_OK SPELL_FAILED_SUCCESS

enum PetTameFailure
{
	PETTAME_INVALIDCREATURE                                      = 1,
	PETTAME_TOOMANY                                              = 2,
	PETTAME_CREATUREALREADYOWNED                                 = 3,
	PETTAME_NOTTAMEABLE                                          = 4,
	PETTAME_ANOTHERSUMMONACTIVE                                  = 5,
	PETTAME_UNITSCANTTAME                                        = 6,
	PETTAME_NOPETAVAILABLE                                       = 7,
	PETTAME_INTERNALERROR                                        = 8,
	PETTAME_TOOHIGHLEVEL                                         = 9,
	PETTAME_DEAD                                                 = 10,
	PETTAME_NOTDEAD                                              = 11,
	PETTAME_CANTCONTROLEXOTIC                                    = 12,
	PETTAME_UNKNOWNERROR                                         = 13,
};

enum MoutFailure
{
	ERR_MOUNT_SHAPESHIFTED                                       = 1,
	ERR_MOUNT_RACECANTMOUNT                                      = 2,
	ERR_MOUNT_LOOTING                                            = 3,
	ERR_MOUNT_OTHER                                              = 4,
	ERR_MOUNT_NOTYOURPET                                         = 5,
	ERR_MOUNT_NOTMOUNTABLE                                       = 6,
	ERR_MOUNT_ALREADYMOUNTED                                     = 7,
	ERR_MOUNT_TOOFARAWAY                                         = 8,
	ERR_MOUNT_INVALIDMOUNTEE                                     = 9,
};

enum DismountFailure
{
	ERR_DISMOUNT_NOTYOURPET                                      = 1,
	ERR_DISMOUNT_NOTMOUNTED                                      = 2,
	ERR_DISMOUNT_NOPET                                           = 3,
};

#endif

