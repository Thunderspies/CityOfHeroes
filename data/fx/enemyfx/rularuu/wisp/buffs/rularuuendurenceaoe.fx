#########################################################
##	chill touch hit
FxInfo

LifeSpan	100

Flags    InheritAlpha

##################################

Condition
	On	Time
	Time	17
	
	Event
		
		Type	local
		At	root
		Sound BruteCharge2 80 80 .86

	End

End





Condition
	On	Time
	Time	43
	
	Event
		
		Type	start
		At	root
		BHVR	Behaviors/OffsetCrystalBoss.bhvr
		part	:ENDStaminaRingOut.part
		part	:ENDJaggedRingOut.part
		part	:ENDJaggedRingOut2.part
		part	:ENDShrapnelRingOut.part
		part	:PurpleEnergyRing.part

	End

End

Condition
	On	Time
	Time	50
	
	Event
		
		Type	start
		At	root
		BHVR	Behaviors/OffsetCrystalBoss.bhvr
		part	:ENDStaminaRingOut.part
		part	:ENDJaggedRingOut.part
		part	:ENDJaggedRingOut2.part
		part	:ENDShrapnelRingOut.part
		part	:PurpleEnergyRing.part

	End

End


Condition
	On	Time
	Time	57
	
	Event
		
		Type	start
		At	root
		BHVR	Behaviors/OffsetCrystalBoss.bhvr
		part	:ENDStaminaRingOut.part
		part	:ENDJaggedRingOut.part
		part	:ENDJaggedRingOut2.part
		part	:ENDShrapnelRingOut.part
		part	:PurpleEnergyRing.part
		
	End

End

Condition
	On	Time
	Time	28
	
	Event
		EName	1
		Type	Local
		At	Hips
		part1	:EnduranceGlow.part
		bhvr	Behaviors/GenericParticleFade.bhvr
	End

	Event
		EName	2
		Type	Local
		At	FootR
		part1	Powers/Healing/enduranceBodyGlows.part
		part2	Powers/Healing/enduranceBodyGlowsFlat.part
	End


#LegL



	Event
		EName	3
		Type	Local
		At	ULEGL
		part1	Powers/Healing/enduranceMotionGlows.part
		PMagnet Knee1

	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		part1	Powers/Healing/enduranceMotionGlows.part
		PMagnet FootL
	End

	Event
		EName	5		
		Type	Local
		At	FootL
		part1	Powers/Healing/enduranceBodyGlows.part
		part2	Powers/Healing/enduranceBodyGlowsFlat.part
	End



	Event
		EName	6
		Type	Local
		At	ULEGR
		part1	Powers/Healing/enduranceMotionGlows.part
		PMagnet Kneer

	End


#LegR


	Event
		EName	7
		Type	Local
		At	LLEGR
		part1	Powers/Healing/enduranceMotionGlows.part
		PMagnet FootR
	End

###############################################################################

	Event
		EName	9
		Type	Local
		At	hair
		BHVR	Behaviors/OffsetCrystal.bhvr
		part2	Powers/Healing/EndureHeadGlows.part
		LifeSpan	20
	End

##########################################################################
##Arm
##################################################################

	Event
		EName	10
		Type	Local
		At	UArmL
		part1	Powers/Healing/enduranceArmMotionGlows.part
		part2	Powers/Healing/enduranceArmMotionGlows2.part
		PMagnet ElbowL

	End

	Event
		EName	11
		Type	Local
		At	LArmL
		part1	Powers/Healing/enduranceArmMotionGlows.part
		part2	Powers/Healing/enduranceArmMotionGlows2.part
		PMagnet HandL
	End

	Event
		EName	12
		Type	Local
		At	WepL
		part1	Powers/Healing/enduranceArmGlows.part
	
	End






	Event
		EName	13
		Type	Local
		At	UArmR
		part1	Powers/Healing/enduranceArmMotionGlows.part
		part2	Powers/Healing/enduranceArmMotionGlows2.part
		PMagnet ElbowL

	End

	Event
		EName	14
		Type	Local
		At	LArmR
		part1	Powers/Healing/enduranceArmMotionGlows.part
		part2	Powers/Healing/enduranceArmMotionGlows2.part
		PMagnet HandL
	End

	Event
		EName	15
		Type	Local
		At	WepR
		part1	Powers/Healing/enduranceArmGlows.part
	
	End



End



Condition
	On	Time
	Time	68
	
	Event
		EName	1
		Type	Destroy
				
	End

	Event
		EName	2
		Type	Destroy
				
	End

	Event
		EName	3
		Type	Destroy
				
	End

	Event
		EName	4
		Type	Destroy
				
	End

	Event
		EName	5
		Type	Destroy
				
	End

	Event
		EName	6
		Type	Destroy
				
	End

	Event
		EName	7
		Type	Destroy
				
	End

	Event
		EName	9
		Type	Destroy
				
	End

	Event
		EName	10
		Type	Destroy
				
	End

	Event
		EName	11
		Type	Destroy
				
	End

	Event
		EName	12
		Type	Destroy
				
	End

	Event
		EName	13
		Type	Destroy
				
	End

	Event
		EName	14
		Type	Destroy
				
	End

	Event
		EName	15
		Type	Destroy
				
	End

End

Condition
	On	Time
	Time	70
	
	Event
		EName	all
		Type	Destroy
				
	End

End


End				