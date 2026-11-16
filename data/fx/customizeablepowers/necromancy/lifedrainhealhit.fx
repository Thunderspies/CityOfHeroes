#########################################################
##	chill touch hit
FxInfo

LifeSpan	100

#########################################################



Condition
	On	Time
	Time	0
	
	Event
		EName	Ring
		Type	Local
		At	origin
		
		ChildFx	POWERS/Empathy/DarknessAbsorbPain.fx
		LifeSpan	100
	End

	Event
		EName	Ring
		Type	Local
		At	T_Root
		part1	CustomizeablePowers\Necromancy\StaminaRingUp.part
		Sound heal1 70 70 .88
		
	End

End

Condition
	On	Time
	Time	5
	
	Event
		EName	Ring
		Type	Local
		At	T_Root
		part1	CustomizeablePowers\Necromancy\StaminaRingUp.part
		
	End

End

Condition
	On	Time
	Time	15
	
	Event
		EName	Ring
		Type	Local
		At	T_Root
		part1	CustomizeablePowers\Necromancy\StaminaRingUp.part
		
	End

End

Condition
	On	Time
	Time	3
	
	Event
		EName	1
		Type	Local
		At	T_Hips
		part1	CustomizeablePowers\Necromancy\Healing01.part
		SOund	dclifedrain2 80 33 1.0
		
	End

	Event
		EName	2
		Type	Local
		At	T_FootR
		part1	CustomizeablePowers\Necromancy\HealingBodyGlows.part
		part2	CustomizeablePowers\Necromancy\HealingBodyGlowsFlat.part
	End


#LegL



	Event
		EName	3
		Type	Local
		At	T_ULEGL
		part1	CustomizeablePowers\Necromancy\HealingMotionGlows.part
		PMagnet Knee1

	End

	Event
		EName	4
		Type	Local
		At	T_LLEGL
		part1	CustomizeablePowers\Necromancy\HealingMotionGlows.part
		PMagnet FootL
	End

	Event
		EName	5		
		Type	Local
		At	T_FootL
		part1	CustomizeablePowers\Necromancy\HealingBodyGlows.part
		part2	CustomizeablePowers\Necromancy\HealingBodyGlowsFlat.part
	End



	Event
		EName	6
		Type	Local
		At	T_ULEGR
		part1	CustomizeablePowers\Necromancy\HealingMotionGlows.part
		PMagnet Kneer

	End


#LegR


	Event
		EName	7
		Type	Local
		At	T_LLEGR
		part1	CustomizeablePowers\Necromancy\HealingMotionGlows.part
		PMagnet FootR
	End

###############################################################################

	Event
		EName	9
		Type	Local
		At	T_Hair
		part2	CustomizeablePowers\Necromancy\HealingHeadGlows.part
	End

##########################################################################
##Arm
##################################################################

	Event
		EName	10
		Type	Local
		At	T_UArmL
		part1	CustomizeablePowers\Necromancy\HealingArmMotionGlows.part
		part2	CustomizeablePowers\Necromancy\HealingArmMotionGlows2.part
		PMagnet ElbowL

	End

	Event
		EName	11
		Type	Local
		At	T_LArmL
		part1	CustomizeablePowers\Necromancy\HealingArmMotionGlows.part
		part2	CustomizeablePowers\Necromancy\HealingArmMotionGlows2.part
		PMagnet HandL
	End

	Event
		EName	12
		Type	Local
		At	T_WepL
		part1	CustomizeablePowers\Necromancy\HealingArmGlows.part
	
	End

	Event
		EName	13
		Type	Local
		At	T_UArmR
		part1	CustomizeablePowers\Necromancy\HealingArmMotionGlows.part
		part2	CustomizeablePowers\Necromancy\HealingArmMotionGlows2.part
		PMagnet ElbowL

	End

	Event
		EName	14
		Type	Local
		At	T_LArmR
		part1	CustomizeablePowers\Necromancy\HealingArmMotionGlows.part
		part2	CustomizeablePowers\Necromancy\HealingArmMotionGlows2.part
		PMagnet HandL
	End

	Event
		EName	15
		Type	Local
		At	T_WepR
		part1	CustomizeablePowers\Necromancy\HealingArmGlows.part
	
	End

End

Condition
	On	Time
	Time	53
	
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
	Time	55
	
	Event
		EName	all
		Type	Destroy
				
	End

End