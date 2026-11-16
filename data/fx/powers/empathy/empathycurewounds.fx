#############################################################
## Empath - Regeneration Aura
#############################################################

FxInfo

LifeSpan	65

#############################################################

Condition
	On 	cycle
	Time 	20
	Chance	1
	
	Event
		EName	1
		Type	PositOnly
		At	Hips
		part1	:CureWoundsAOERing.part
		part2	:CureWoundsAOERing2.part
		Sound regenaura 60 60 .3
		LifeSpan	20
	End
End

#############################################################

Condition
	On	Time
	Time	10
					
	Event
		EName	1
		Type	Local
		At	Hips
		part1	Powers/Healing/Healing01a.part
	End

	Event
		EName	2
		Type	Local
		At	FootR
		part1	Powers/Healing/HealingBodyGlows.part
		#part2	Powers/Healing/HealingBodyGlowsFlat.part
	End

	Event
		EName	3
		Type	Local
		At	ULEGL
		part1	Powers/Healing/HealingMotionGlows.part
		PMagnet Knee1
	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		part1	Powers/Healing/HealingMotionGlows.part
		PMagnet FootL
	End

	Event
		EName	5		
		Type	Local
		At	FootL
		part1	Powers/Healing/HealingBodyGlows.part
		#part2	Powers/Healing/HealingBodyGlowsFlat.part
	End

	Event
		EName	6
		Type	Local
		At	ULEGR
		part1	Powers/Healing/HealingMotionGlows.part
		PMagnet Kneer
	End

	Event
		EName	7
		Type	Local
		At	LLEGR
		part1	Powers/Healing/HealingMotionGlows.part
		PMagnet FootR
	End

	Event
		EName	9
		Type	Local
		At	Hair
		part2	Powers/Healing/HealingHeadGlows.part
	End

	Event
		EName	10
		Type	Local
		At	UArmL
		part1	Powers/Healing/HealingArmMotionGlows.part
		part2	Powers/Healing/HealingArmMotionGlows2.part
		PMagnet ElbowL
	End

	Event
		EName	11
		Type	Local
		At	LArmL
		part1	Powers/Healing/HealingArmMotionGlows.part
		part2	Powers/Healing/HealingArmMotionGlows2.part
		PMagnet HandL
	End

	Event
		EName	12
		Type	Local
		At	WepL
		part1	Powers/Healing/HealingArmGlows.part
	
	End

	Event
		EName	13
		Type	Local
		At	UArmR
		part1	Powers/Healing/HealingArmMotionGlows.part
		part2	Powers/Healing/HealingArmMotionGlows2.part
		PMagnet ElbowL

	End

	Event
		EName	14
		Type	Local
		At	LArmR
		part1	Powers/Healing/HealingArmMotionGlows.part
		part2	Powers/Healing/HealingArmMotionGlows2.part
		PMagnet HandL
	End

	Event
		EName	15
		Type	Local
		At	WepR
		part1	Powers/Healing/HealingArmGlows.part
	End
End

#############################################################

End			