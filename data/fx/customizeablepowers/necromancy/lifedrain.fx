#########################################################
##	chill touch hit
FxInfo

LifeSpan	100

#########################################################

Condition
	On 	Time
	Time 	5

	Event
		EName 	Hand3
		Type	Local
		At	UarmL
		Part1	CustomizeablePowers\Necromancy\DarkBlastTenticle.part
		Part3	CustomizeablePowers\Necromancy\DarkHandsEmberb.part
				
		PMagnet	LarmL
		
	End

	Event
		EName 	Hand4
		Type	Local
		At	LarmL
		Part1	CustomizeablePowers\Necromancy\DarkBlastTenticle.part
		Part3	CustomizeablePowers\Necromancy\DarkHandsEmberb.part
		
		PMagnet	WepL
		
	End

End

Condition
	On 	Time
	Time 	21

	Event
		EName 	HandGlow
		Type	Local
		At	WepL
		Part1	CustomizeablePowers\Necromancy\DarkDrainGlow.part
		#Part3	:DarkHandsEmberb.part
		
		
	End

End

Condition
	On	Time
	Time	40
	
	Event
		EName	Hand3
		Type	Destroy
				
	End

	Event
		EName	Hand4
		Type	Destroy
				
	End

End

Condition
	On	Time
	Time	50
	Event
		EName	HandGlow
		Type	Destroy
				
	End
End
##################################

#Condition
#	On	Time
#	Time	25
#	
#	Event
#		EName	Ring
#		Type	Local
#		At	Root
#		part1	Powers/Ability/StaminaRingUp.part
#		Sound	heal1 70 25 .88
#		
#	End
#
#End
#
#Condition
#	On	Time
#	Time	30
#	
#	Event
#		EName	Ring
#		Type	Local
#		At	Root
#		part1	Powers/Ability/StaminaRingUp.part
#		
#	End
#
#End
#
#Condition
#	On	Time
#	Time	45
#	
#	Event
#		EName	Ring
#		Type	Local
#		At	Root
#		part1	Powers/Ability/StaminaRingUp.part
#		
#	End
#
#End
#
#Condition
#	On	Time
#	Time	28
#	
#	Event
#		EName	1
#		Type	Local
#		At	Hips
#		part1	Powers/Healing/Healing01.part
#		SOund	dclifedrain2 80 33 1.0
#		
#	End
#
#	Event
#		EName	2
#		Type	Local
#		At	FootR
#		part1	Powers/Healing/HealingBodyGlows.part
#		part2	Powers/Healing/HealingBodyGlowsFlat.part
#	End
#
#
##LegL
#
#
#
#	Event
#		EName	3
#		Type	Local
#		At	ULEGL
#		part1	Powers/Healing/HealingMotionGlows.part
#		PMagnet Knee1
#
#	End
#
#	Event
#		EName	4
#		Type	Local
#		At	LLEGL
#		part1	Powers/Healing/HealingMotionGlows.part
#		PMagnet FootL
#	End
#
#	Event
#		EName	5		
#		Type	Local
#		At	FootL
#		part1	Powers/Healing/HealingBodyGlows.part
#		part2	Powers/Healing/HealingBodyGlowsFlat.part
#	End
#
#
#
#	Event
#		EName	6
#		Type	Local
#		At	ULEGR
#		part1	Powers/Healing/HealingMotionGlows.part
#		PMagnet Kneer
#
#	End
#
#
##LegR
#
#
#	Event
#		EName	7
#		Type	Local
#		At	LLEGR
#		part1	Powers/Healing/HealingMotionGlows.part
#		PMagnet FootR
#	End
#
################################################################################
#
#	Event
#		EName	9
#		Type	Local
#		At	Hair
#		part2	Powers/Healing/HealingHeadGlows.part
#	End
#
###########################################################################
###Arm
###################################################################
#
#	Event
#		EName	10
#		Type	Local
#		At	UArmL
#		part1	Powers/Healing/HealingArmMotionGlows.part
#		part2	Powers/Healing/HealingArmMotionGlows2.part
#		PMagnet ElbowL
#
#	End
#
#	Event
#		EName	11
#		Type	Local
#		At	LArmL
#		part1	Powers/Healing/HealingArmMotionGlows.part
#		part2	Powers/Healing/HealingArmMotionGlows2.part
#		PMagnet HandL
#	End
#
#	Event
#		EName	12
#		Type	Local
#		At	WepL
#		part1	Powers/Healing/HealingArmGlows.part
#	
#	End
#
#	Event
#		EName	13
#		Type	Local
#		At	UArmR
#		part1	Powers/Healing/HealingArmMotionGlows.part
#		part2	Powers/Healing/HealingArmMotionGlows2.part
#		PMagnet ElbowL
#
#	End
#
#	Event
#		EName	14
#		Type	Local
#		At	LArmR
#		part1	Powers/Healing/HealingArmMotionGlows.part
#		part2	Powers/Healing/HealingArmMotionGlows2.part
#		PMagnet HandL
#	End
#
#	Event
#		EName	15
#		Type	Local
#		At	WepR
#		part1	Powers/Healing/HealingArmGlows.part
#	
#	End
#
#End
#
#Condition
#	On	Time
#	Time	68
#	
#	Event
#		EName	1
#		Type	Destroy
#				
#	End
#
#	Event
#		EName	2
#		Type	Destroy
#				
#	End
#
#	Event
#		EName	3
#		Type	Destroy
#				
#	End
#
#	Event
#		EName	4
#		Type	Destroy
#				
#	End
#
#	Event
#		EName	5
#		Type	Destroy
#				
#	End
#
#	Event
#		EName	6
#		Type	Destroy
#				
#	End
#
#	Event
#		EName	7
#		Type	Destroy
#				
#	End
#
#	Event
#		EName	9
#		Type	Destroy
#				
#	End
#
#	Event
#		EName	10
#		Type	Destroy
#				
#	End
#
#	Event
#		EName	11
#		Type	Destroy
#				
#	End
#
#	Event
#		EName	12
#		Type	Destroy
#				
#	End
#
#	Event
#		EName	13
#		Type	Destroy
#				
#	End
#
#	Event
#		EName	14
#		Type	Destroy
#				
#	End
#
#	Event
#		EName	15
#		Type	Destroy
#				
#	End
#
#End

Condition
	On	Time
	Time	70
	
	Event
		EName	all
		Type	Destroy
				
	End

End


End	


			