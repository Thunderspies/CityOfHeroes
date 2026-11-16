#########################################################
##	Fireweapon
########################################################
FxInfo

#Input
#	InpName	T_Root
#End
#
#Input
#	InpName	Origin
#End
#
#LifeSpan	50



##########################################
##  AUDIO
##########################################

#Condition
#	On	Time
#	Time	0
#
#	Event
#		Type	local
#		At	Origin
#		Sound	rumble2_loop 80 60 .5
#
#	End
#End

##########################################
##########################################

Condition
	On 	Time
	Time 	0

#	Event
#		Ename	ExplosiveForce
#		Type	Start
#		At	Root
#
#		BhvrOverride
#			PhysForceType		Up
#			PhysForceRadius		10
#			PhysForcePower		120
#			PhysForcePowerJitter	40
#		End
#	End
#


	Event
		Ename	Offset
		Type	Start
		At	Origin
#		BhvrOverride
#			StartJitter 0.1 0.1 0.1
#		End
	End


	Event

		EName 	RockBase2
		Type	Start
		At	Offset
		Part	:ViolentSteamCore.part
		Lifespan	7
	End

	Event
		Ename 	Crack
		Type	Start
		At	Root
		Bhvr	POWERS\SuperStrength\Splat_FootStomp.bhvr
		BhvrOverride
			FadeInLength 20
			PyrRotateJitter 0 0 0
			StartColor 100 150 250
			SplatFlags ADDITIVE
			Scale 10.0 10.0 10.0
		End
		Splat	crackedEarth.tga
		Lifespan 60

	End

	Event
		EName 	RockBase2
		Type	Start
		At	Offset

		Lifespan	30
		LifespanJitter	15
		Part	:ViolentSteamAdditive.part
		Part	:ViolentSteamAlpha.part
		Part	:SteamAttack.part
	#	Sound	Arcoflame 80 60 .88


	End


##########################################################

	Event
		EName 	RockBase3
		Type	Start
		At	Offset
		Bhvr	behaviors/VolcanicGasRock1.bhvr
	#	Geom	VolcanicRock01
	#	Part1	:StalagtiteCrack8.part
		Part4	:GasRadiusRocks.part
	End

	Event
		EName 	Dust
		Type	Start
		At	Offset

		Part1	:VolcanicDust.part
		part2	:VolcanicDust2.part
		Part	:LavaBubblePopping2.part
		Part	:LavaSprayx.part
		Lifespan 40

	End

#	Event
#		EName 	crack
#		Type	Local
#		At	Start
#		Bhvr	behaviors/VolcanicGassesCrackScale.bhvr
#	#	geom	Crack01
#
#	End


End
#
#Condition
#	On 	Time
#	Time 	4
#
#	Event
#		EName 	Rock3
#		Type	Local
#		At	Offset
#		Bhvr	behaviors/VolcanicGasRock3.bhvr
#	#	Geom	VolcanicRock03
#		Part1	:StalagtiteCrack8.part
#
#	End
#End
#
#Condition
#	On 	Time
#	Time 	6
#
#	Event
#		EName 	Rock4
#		Type	Local
#		At	Offset
#		Bhvr	behaviors/VolcanicGasRock4.bhvr
#	#	Geom	VolcanicRock04
#		Part1	:StalagtiteCrack8.part
#
#	End





##################################

Condition
	On 	Time
	Time 	300

	Event
		EName	All
		Type	Destroy

	End


End