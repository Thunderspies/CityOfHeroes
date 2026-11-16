#########################################################
##	chill touch hit
FxInfo

LifeSpan	100

Flags    InheritAlpha

##################################

Condition
	On	Time
	Time	0

	Event	

		Type	Local
		At	Origin
		Sound Aura3 90 90 .8
	End
End



Condition
	On	Time
	Time	43
	
	Event
		
		Type	start
		At	root
		BHVR	Behaviors/OffsetCrystalRotate.bhvr
		part	:DamageStaminaRingOut.part
		part	:DamageJaggedRingOut.part
		part	:DamageJaggedRingOut2.part
		part	:DamageShrapnelRingOut.part
		part	:RedEnergyRing.part
		Sound Atomicblast2 90 90 .8

	End

End

Condition
	On	Time
	Time	50
	
	Event
		
		Type	start
		At	root
		BHVR	Behaviors/OffsetCrystalRotate.bhvr
		part	:DamageStaminaRingOut.part
		part	:DamageJaggedRingOut.part
		part	:DamageJaggedRingOut2.part
		part	:DamageShrapnelRingOut.part
		part	:RedEnergyRing.part

	End

End

Condition
	On	Time
	Time	57
	
	Event
		
		Type	start
		At	root
		BHVR	Behaviors/OffsetCrystalRotate.bhvr
		part	:DamageStaminaRingOut.part
		part	:DamageJaggedRingOut.part
		part	:DamageJaggedRingOut2.part
		part	:DamageShrapnelRingOut.part
		part	:RedEnergyRing.part
		
	End

End

Condition
	On	Time
	Time	28
	
	Event
		EName	1
		Type	Local
		At	Hips
		part1	:DamageGlow.part
		bhvr	Behaviors/GenericParticleFade.bhvr
	End

	Event
		EName	2
		Type	Local
		At	FootR
		part1	Powers/Healing/DamageBodyGlows.part
		part2	Powers/Healing/DamageBodyGlowsFlat.part
	End


#LegL



	Event
		EName	3
		Type	Local
		At	ULEGL
		part1	Powers/Healing/DamageMotionGlows.part
		PMagnet Knee1

	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		part1	Powers/Healing/DamageMotionGlows.part
		PMagnet FootL
	End

	Event
		EName	5		
		Type	Local
		At	FootL
		part1	Powers/Healing/DamageBodyGlows.part
		part2	Powers/Healing/DamageBodyGlowsFlat.part
	End



	Event
		EName	6
		Type	Local
		At	ULEGR
		part1	Powers/Healing/DamageMotionGlows.part
		PMagnet Kneer

	End


#LegR


	Event
		EName	7
		Type	Local
		At	LLEGR
		part1	Powers/Healing/DamageMotionGlows.part
		PMagnet FootR
	End

###############################################################################

	Event
		EName	9
		Type	Local
		At	head
		BHVR	Behaviors/OffsetCrystal.bhvr
		part2	Powers/Healing/DamageHeadGlows.part
		LifeSpan	20
	End

##########################################################################
##Arm
##################################################################

	Event
		EName	10
		Type	Local
		At	UArmL
		part1	Powers/Healing/DamageArmMotionGlows.part
		part2	Powers/Healing/DamageArmMotionGlows2.part
		PMagnet ElbowL
		

	End

	Event
		EName	11
		Type	Local
		At	LArmL
		part1	Powers/Healing/DamageArmMotionGlows.part
		part2	Powers/Healing/DamageArmMotionGlows2.part
		PMagnet HandL
	End

	Event
		EName	12
		Type	Local
		At	WepL
		part1	Powers/Healing/DamageArmGlows.part
	
	End






	Event
		EName	13
		Type	Local
		At	UArmR
		part1	Powers/Healing/DamageArmMotionGlows.part
		part2	Powers/Healing/DamageArmMotionGlows2.part
		PMagnet ElbowL

	End

	Event
		EName	14
		Type	Local
		At	LArmR
		part1	Powers/Healing/DamageArmMotionGlows.part
		part2	Powers/Healing/DamageArmMotionGlows2.part
		PMagnet HandL
	End

	Event
		EName	15
		Type	Local
		At	WepR
		part1	Powers/Healing/DamageArmGlows.part
	
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