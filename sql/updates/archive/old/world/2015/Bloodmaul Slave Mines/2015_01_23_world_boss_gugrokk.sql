UPDATE creature_template SET unit_flags = unit_flags & ~0x200000, ScriptName = 'boss_gugrokk', mechanic_immune_mask = 617299839, flags_extra = flags_extra | 0x01 WHERE entry = 74790;
UPDATE creature_template SET modelid1 = 11686, modelid2 = 0, ScriptName = 'npc_gugrokk_magma_eruption' WHERE entry = 74967;
UPDATE creature_template SET ScriptName = 'npc_gugrokk_unstable_slag' WHERE entry = 74927;

DELETE FROM areatrigger_template WHERE spell_id = 150783;
DELETE FROM creature WHERE id IN (74927, 74967, 83808) AND map = 1175;
UPDATE creature SET unit_flags = unit_flags & ~0x200 WHERE id = 68553 AND map = 1175;

DELETE FROM trinity_string WHERE entry = 14064;
INSERT INTO trinity_string (entry, content_default, content_loc2, content_loc6) VALUE
(14064, '|TInterface\\Icons\\spell_fire_elementaldevastation.blp:20|t |cFFF00000|Hspell:150678|h[Molten Core]|h|r causes %s to unleash a |cFFF00000|Hspell:150682|h[Motlen Barrage]|h|r!',
	'|TInterface\\Icons\\spell_fire_elementaldevastation.blp:20|t |cFFF00000|Hspell:150678|h[C�ur de la fournaise]|h|r permet � %s de lib�rer un |cFFF00000|Hspell:150682|h[Barrage de lave]|h|r !',
	'|TInterface\\Icons\\Ability_Druid_EmpoweredTouch:20|t Tarlna comienza a canalizar |cFFFF0404|Hspell:175979|h[G�nesis]|h|r.');

DELETE FROM creature_text WHERE entry = 74790;
INSERT INTO creature_text VALUES
(74790, 0, 0, 'It will be a pleasure to break you.', 14, 0, 100, 0, 0, 42068, 'TalkIntro'),
(74790, 1, 0, 'First, I break your spirit, then, I sell your future.', 14, 0, 100, 0, 0, 42065, 'TalkAggro'),
(74790, 2, 0, 'Burn!', 14, 0, 100, 0, 0, 42071, 'TalkMagmaEruption'),
(74790, 3, 0, 'You will be slaves to the slag!', 14, 0, 100, 0, 0, 42072, 'TalkUnstableSlag1'),
(74790, 3, 1, 'The elements hear me and obey!', 14, 0, 100, 0, 0, 42073, 'TalkUnstableSlag2'),
(74790, 3, 2, 'Your charred remains will make a fine example!', 14, 0, 100, 0, 0, 42074, 'TalkUnstableSlag3'),
(74790, 4, 0, 'Fool.', 14, 0, 100, 0, 0, 42069, 'TalkSlay'),
(74790, 4, 1, 'You will learn to obey.', 14, 0, 100, 0, 0, 42070, 'TalkSlay'),
(74790, 5, 0, 'You will never... be free...', 14, 0, 100, 0, 0, 42067, 'TalkDeath');

DELETE FROM locales_creature_text WHERE entry = 74790;
INSERT INTO locales_creature_text (entry, textGroup, id, text_loc2, text_loc6) VALUES
(74790, 0, 0, 'Je vais me faire un plaisir de vous briser.', 'Ser� un placer romperos.'),
(74790, 1, 0, 'D''abord, je vais briser votre esprit. Ensuite je vendrai votre avenir.', 'Primero, romper� vuestro esp�ritu, y despu�s, vender� vuestro futuro.'),
(74790, 2, 0, 'Br�lez !', '�Arded!'),
(74790, 3, 0, 'Vous serez les esclaves de la mine !', '�Ser�is esclavos de la escoria!'),
(74790, 3, 1, 'Les �l�ments m''�coutent et m''ob�issent !', '�Los elementos me oyen y me obedecen!'),
(74790, 3, 2, 'Vos os calcin�s feront un bel exemple pour les autres !', '�Vuestros restos carbonizados ser�n un buen ejemplo!'),
(74790, 4, 0, 'Imb�cile !', 'Necio.'),
(74790, 4, 1, 'Je vais vous apprendre � ob�ir !', 'Aprender�is a obedecer.'),
(74790, 5, 0, 'Vous ne serez... jamais... libres...', 'Vosotros nunca... ser�is libres...');