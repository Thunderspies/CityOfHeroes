#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	InpName	Root
End

Input  
	InpName	Origin
End


##########################################
##  AUDIO
##########################################

Condition
	On	Time
	Time	0

	Event
		Type	local
		At	Origin
		Sound rumble2_loop 80 80 .5

	End
End

#########################################################

Condition
	On 	Cycle
	Time 	25

	Event
		EName 	Hurricane
		Type	start 
		At	Root
#		Part	:VolcanicGasses.part
		Bhvr	:VolcanicGasSplat.bhvr

		splat	SSHurricane01.tga
		
		LifeSpan	50
		
	End

End

Condition
	On 	Cycle
	Time 	25

	Event
		EName 	Hurricane
		Type	start 
		At	Root
#		Part	:VolcanicGasses.part
		Bhvr	:VolcanicGasSplatDark.bhvr

		splat	Smoke2.tga
		
		LifeSpan	50
		
	End

End

Condition
	On 	Time
	Time 	0

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Mystic
		
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		30
			PhysForcePower		25
			PhysForcePowerJitter	20
		End
	End

#	Event
#		EName 	RockBase
#		Type	start
#		At	Root
#		Bhvr	behaviors/VolcanicGasRock1.bhvr
#		Geom	VolcanicRock01
#		Part1	:StalagtiteCrack8.part
#		Part4	:GasRadiusRocks.part
#	End
#
#	Event
#		EName 	Dust
#		Type	start
#		At	Root
#
#		Part1	:VolcanicDust.part
#		part2	:VolcanicDust2.part
#		Part	:LavaBubblePopping2.part
#		Part	:LavaSprayx.part
#		Part	:LavaBubbleFlat2.part	
#		
#	End
#
#	Event
#		EName 	crack
#		Type	start
#		At	root
#		Bhvr	behaviors/VolcanicGassesCrackScale.bhvr
#		geom	Crack01	
#			
#	End
#End
#
#Condition
#	On 	Time
#	Time 	4
#
#	Event
#		EName 	Rock2
#		Type	start 
#		At	Root
#		Bhvr	behaviors/VolcanicGasRock5.bhvr
#		Geom	VolcanicRock02
#		Part1	:StalagtiteCrack8.part
#		
#		
#	End
#End
#
#Condition
#	On 	Time
#	Time 	3
#
#	Event
#		EName 	Rock3
#		Type	start 
#		At	Root
#		Bhvr	behaviors/VolcanicGasRock3.bhvr
#		Geom	VolcanicRock03
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
#		Type	start 
#		At	Root
#		Bhvr	behaviors/VolcanicGasRock4.bhvr
#		Geom	VolcanicRock04
#		Part1	:StalagtiteCrack8.part
#		
#	End
#
#
End

End
##################################

