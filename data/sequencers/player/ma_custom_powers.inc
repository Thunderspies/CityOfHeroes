#############################################################
## MA_custom_powers.inc
#############################################################

###################
###### MA_TechWindup + Radiation Blast Power ######
###################

Move Pre_MA_ProtonVolley
	Requires	COMBAT STRONG CAST ARCHITECT ONE
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        MA_ProtonVolley
	Sets		CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\sniperblast 1 40
	  PitchStart	5
	  PitchAngle	-0.5
	#Playfx		"POWERS\RadiationControl\ProtonBlastHands.fx" 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\sniperblast 1 40
	  PitchStart	5
	  PitchAngle	-0.5
	#Playfx		"POWERS\RadiationControl\ProtonBlastHands.fx" 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\sniperblast 1 40
	  PitchStart	5
	  PitchAngle	-0.5
	#Playfx		"POWERS\RadiationControl\ProtonBlastHands.fx" 0 CONSTANT
	TEnd
MEnd

Move MA_ProtonVolley
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	Flags		PitchToTarget NoSizeScale
	NextMove        SniperBlast

	Type		male
	  Anim          male\sniperblast 40 60
	  PitchAngle	-0.5
	#Playfx		"POWERS\RadiationControl\ProtonBlastHands.fx" 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\sniperblast 40 60
	  PitchAngle	-0.5
	#Playfx		"POWERS\RadiationControl\ProtonBlastHands.fx" 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\sniperblast 40 60
	  PitchAngle	-0.5
	#Playfx		"POWERS\RadiationControl\ProtonBlastHands.fx" 0 CONSTANT
	TEnd
MEnd

###################
###### MA_TechWindup + Electric Blast Power ######
###################

Move Pre_MA_Zapp
	Requires	COMBAT STRONG CAST ARCHITECT TWO
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        MA_Zapp
	Sets		CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\sniperblast 1 40
	  PitchStart	5
	  PitchAngle	-0.5
	#Playfx		"V_COV\Powers\ElectricalBlast\ZappWindup.fx" 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\sniperblast 1 40
	  PitchStart	5
	  PitchAngle	-0.5
	#Playfx		"V_COV\Powers\ElectricalBlast\ZappWindup.fx" 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\sniperblast 1 40
	  PitchStart	5
	  PitchAngle	-0.5
	#Playfx		"V_COV\Powers\ElectricalBlast\ZappWindup.fx" 0 CONSTANT
	TEnd
MEnd

Move MA_Zapp
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	Flags		PitchToTarget NoSizeScale
	NextMove        SniperBlast

	Type		male
	  Anim          male\sniperblast 40 60
	  PitchAngle	-0.5
	#Playfx		"V_COV\Powers\ElectricalBlast\ZappWindup.fx" 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\sniperblast 40 60
	  PitchAngle	-0.5
	#Playfx		"V_COV\Powers\ElectricalBlast\ZappWindup.fx" 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\sniperblast 40 60
	  PitchAngle	-0.5
	#Playfx		"V_COV\Powers\ElectricalBlast\ZappWindup.fx" 0 CONSTANT
	TEnd
MEnd

###################
###### MA_TechWindup + Energy Sniper Blast Power ######
###################

Move Pre_MA_EnergyBlast
	Requires	COMBAT STRONG CAST ARCHITECT THREE
	Priority        5
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        MA_EnergyBlast
	Sets		CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\sniperblast 1 40
	  PitchStart	5
	  PitchAngle	-0.5
	#Playfx		"POWERS\EnergyBlast\SniperWindup.fx" 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\sniperblast 1 40
	  PitchStart	5
	  PitchAngle	-0.5
	#Playfx		"POWERS\EnergyBlast\SniperWindup.fx" 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\sniperblast 1 40
	  PitchStart	5
	  PitchAngle	-0.5
	#Playfx		"POWERS\EnergyBlast\SniperWindup.fx" 0 CONSTANT
	TEnd
MEnd

Move MA_EnergyBlast
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	Flags		PitchToTarget NoSizeScale
	NextMove        SniperBlast

	Type		male
	  Anim          male\sniperblast 40 60
	  PitchAngle	-0.5
	#Playfx		"POWERS\EnergyBlast\SniperWindup.fx" 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\sniperblast 40 60
	  PitchAngle	-0.5
	#Playfx		"POWERS\EnergyBlast\SniperWindup.fx" 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\sniperblast 40 60
	  PitchAngle	-0.5
	#Playfx		"POWERS\EnergyBlast\SniperWindup.fx" 0 CONSTANT
	TEnd
MEnd

###################
###### MA WindUp + Blazing Bolt  ######
###################

Move Pre_MA_BlazingBolt
	Requires	COMBAT 2HAND CAST ARCHITECT
	Priority        4
	Interrupts	<AttackIRQ> <Defiance>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        MA_BlazingBolt
	Sets            CANTMOVE
	Flags		NoSizeScale

	Type		male
	  Anim          male\windup_power 1 42
	#Playfx		"powers\FireControl\FireHands.fx" 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\windup_power 1 42
	#Playfx		"powers\FireControl\FireHands.fx" 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\windup_power 1 42
	#Playfx		"powers\FireControl\FireHands.fx" 0 CONSTANT
	TEnd
MEnd

Move MA_BlazingBolt
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	Flags		NoSizeScale
	NextMove        2HandCast

	Type		male
	  Anim          male\windup_power 42 63
	#Playfx		"powers\FireControl\FireHands.fx" 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\windup_power 42 63
	#Playfx		"powers\FireControl\FireHands.fx" 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\windup_power 42 63
	#Playfx		"powers\FireControl\FireHands.fx" 0 CONSTANT
	TEnd
MEnd

###################
###### MA_Psionic Windup + Psionic Lance ######
###################

Move Pre_MA_PsiWindup
	Requires	COMBAT QUICK PSIONICBLAST ARCHITECT
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        MA_PsiWindup
	Sets            CANTMOVE
	Flags		PitchToTarget NoSizeScale

	Type		male
	  Anim          male\psionic 1 50
	  PitchStart	13
	  PitchAngle	0.2
	#Playfx		"POWERS\Psionics\PsionicLanceBlastWindUp.fx" 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\psionic 1 50
	  PitchStart	13
	  PitchAngle	0.2
	#Playfx		"POWERS\Psionics\PsionicLanceBlastWindUp.fx" 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\psionic 1 50
	  PitchStart	13
	  PitchAngle	0.2
	#Playfx		"POWERS\Psionics\PsionicLanceBlastWindUp.fx" 0 CONSTANT
	TEnd
MEnd

Move MA_PsiWindup
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	Flags		PitchToTarget NoSizeScale
	NextMove        PsiQuick

	Type		male
	  Anim          male\psionic 50 89
	  PitchAngle	0.2
	#Playfx		"POWERS\Psionics\PsionicLanceBlastWindUp.fx" 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\psionic 50 89
	  PitchAngle	0.2
	#Playfx		"POWERS\Psionics\PsionicLanceBlastWindUp.fx" 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\psionic 50 89
	  PitchAngle	0.2
	#Playfx		"POWERS\Psionics\PsionicLanceBlastWindUp.fx" 0 CONSTANT
	TEnd
MEnd

## MA_AID SELF (Includes FX) ###########################################################

Move MA_AidSelf_Pre
	Requires	MEDSELF TWO ARCHITECT
	Priority        3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        MA_AidSelf
	Sets		CANTMOVE
	Flags		NoSizeScale
	Scale		1.5

	Type		male
	  Anim          male\med_self 1 15
	TEnd

	Type		huge
	  Anim          huge\med_self 1 15
	TEnd

	Type		fem
	  Anim          fem\med_self 1 15
	TEnd
MEnd

Move MA_AidSelf
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	Flags		NoSizeScale
	Sets		CANTMOVE
	NextMove        MA_AidSelf_Post
	Scale		1.5

	Type		male
	  Anim          male\med_self 15 118
	  PlayFX	POWERS\Medicine\MSelf.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\med_self 15 118
	  PlayFX	POWERS\Medicine\MSelf.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\med_self 15 118
	  PlayFX	POWERS\Medicine\MSelf.fx 0 CONSTANT
	TEnd
MEnd

Move MA_AidSelf_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Ready
	Flags		NoSizeScale

	Type		male
	  Anim          male\med_self 118
	  PlayFX	POWERS\Medicine\MSelf.fx 0 CONSTANT
	TEnd

	Type		huge
	  Anim          huge\med_self 118
	  PlayFX	POWERS\Medicine\MSelf.fx 0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\med_self 118
	  PlayFX	POWERS\Medicine\MSelf.fx 0 CONSTANT
	TEnd
MEnd

## PLANT DEVICE ###########################################################

Move MA_PlantDevice_Pre
	Requires	LOOT QUICK ARCHITECT
	Priority	3
	Interrupts	<AttackIRQ>
	Member		<DeathIRQ> <AttackIRQ>
	NextMove        MA_PlantDevice_Main
	Scale           0.70
	Sets		CANTMOVE

	Type		male
	  Anim          male\Loot 3 19
	TEnd
MEnd

Move MA_PlantDevice_Main
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        MA_PlantDevice_Post
	Scale		0.5
	Sets		CANTMOVE

	Type		male
	  Anim          male\examine_hero 1 50
	TEnd
MEnd

Move MA_PlantDevice_Post
	Requires	COMBAT
	Include sequencers\Members\Combat.inc
	NextMove        Ready

	Type		male
	  Anim          male\Loot 26
	TEnd

	Type		huge
	  Anim          huge\Loot 26
	TEnd

	Type		fem
	  Anim          fem\Loot 26
	TEnd
MEnd