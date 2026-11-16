#############################################################
## Empath - Recovery Aura
#############################################################

FxInfo

LifeSpan	90

#############################################################

Condition
	On 	cycle
	Time 	20
	Chance	1

	Event
		EName	1a
		Type	PositOnly
		At	Hips
		part1	:AdrenalineFlowAOERing.part
		part2	:AdrenalineFlowAOERing2.part
		Sound heal3b 60 60 .3
		LifeSpan	20
	End
End

#############################################################

Condition
	Event
		EName	1
		Type	Local
		At	Hips
		part1	:Endurance01.part
	End

	Event
		EName	2
		Type	Local
		At	FootR
		part1	:EnduranceBodyGlows.part
	End

	Event
		EName	3
		Type	Local
		At	ULEGL
		part1	:EnduranceMotionGlows.part
		PMagnet Knee1
	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		part1	:EnduranceArmMotionGlows.part
		part2	:EnduranceMotionGlows.part
		PMagnet FootL
	End

	Event
		EName	5
		Type	Local
		At	FootL
		part1	:EnduranceBodyGlows.part
	End

	Event
		EName	6
		Type	Local
		At	ULEGR
		part1	:EnduranceMotionGlows.part
		PMagnet Kneer
	End

	Event
		EName	7
		Type	Local
		At	LLEGR
		part1	:EnduranceArmMotionGlows.part
		part2	:EnduranceMotionGlows.part
		PMagnet FootR
	End

	Event
		EName	9
		Type	Local
		At	Head
		part1	:EnduranceArmMotionGlows.part
		part2	:EnduranceHeadGlows.part
	End

	Event
		EName	10
		Type	Local
		At	UArmL
		part1	:EnduranceArmMotionGlows.part
		part2	:EnduranceArmMotionGlows2.part
		PMagnet ElbowL
	End

	Event
		EName	11
		Type	Local
		At	LArmL
		part1	:EnduranceArmMotionGlows.part
		part2	:EnduranceArmMotionGlows2.part
		PMagnet HandL
	End

	Event
		EName	12
		Type	Local
		At	WepL
		part1	:EnduranceArmGlows.part
	End

	Event
		EName	13
		Type	Local
		At	UArmR
		part1	:EnduranceArmMotionGlows.part
		part2	:EnduranceArmMotionGlows2.part
		PMagnet ElbowL
	End

	Event
		EName	14
		Type	Local
		At	LArmR
		part1	:EnduranceArmMotionGlows.part
		part2	:EnduranceArmMotionGlows2.part
		PMagnet HandL
	End

	Event
		EName	15
		Type	Local
		At	WepR
		part1	:EnduranceArmGlows.part
	End
End

#############################################################

End