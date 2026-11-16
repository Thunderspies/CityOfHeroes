#############################################################
## MindWall.fx
#############################################################

FxInfo

LifeSpan 165

#########################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On	Time
	Time	25

	Event
		EName	Ring
		Type	Local
		At	Root
		part1	CustomizeablePowers\Empathy\StrengthRingUp.part
	End
End

Condition
	On	Time
	Time	30

	Event
		EName	Ring
		Type	Local
		At	Root
		part1	CustomizeablePowers\Empathy\StrengthRingUp.part
	End
End

Condition
	On	Time
	Time	45

	Event
		EName	Ring
		Type	Local
		At	Root
		part1	CustomizeablePowers\Empathy\StrengthRingUp.part

	End
End

Condition
	On	Time
	Time	20

	Event
		EName	2
		Type	Local
		At	FootR
		part1	CustomizeablePowers\Empathy\MindWallBodyGlows.part
	End

	Event
		EName	3
		Type	Local
		At	ULEGL
		part1	CustomizeablePowers\Empathy\MindWallMotionGlows.part
		PMagnet Knee1
	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		part1	CustomizeablePowers\Empathy\MindWallArmMotionGlows.part
		part2	CustomizeablePowers\Empathy\MindWallMotionGlows.part
		PMagnet FootL
	End

	Event
		EName	5
		Type	Local
		At	FootL
		part1	CustomizeablePowers\Empathy\MindWallBodyGlows.part
	End

	Event
		EName	6
		Type	Local
		At	ULEGR
		part1	CustomizeablePowers\Empathy\MindWallMotionGlows.part
		PMagnet Kneer
	End

	Event
		EName	7
		Type	Local
		At	LLEGR
		part1	CustomizeablePowers\Empathy\MindWallArmMotionGlows.part
		part2	CustomizeablePowers\Empathy\MindWallMotionGlows.part
		PMagnet FootR
	End

	Event
		EName	9
		Type	Local
		At	Head
		part1	CustomizeablePowers\Empathy\MindWallArmMotionGlows.part
		part2	CustomizeablePowers\Empathy\MindWallHeadGlows.part
	End

	Event
		EName	10
		Type	Local
		At	UArmL
		part1	CustomizeablePowers\Empathy\MindWallArmMotionGlows.part
		part2	CustomizeablePowers\Empathy\MindWallArmMotionGlows2.part
		PMagnet ElbowL
	End

	Event
		EName	11
		Type	Local
		At	LArmL
		part1	CustomizeablePowers\Empathy\MindWallArmMotionGlows.part
		part2	CustomizeablePowers\Empathy\MindWallArmMotionGlows2.part
		PMagnet HandL
	End

	Event
		EName	12
		Type	Local
		At	WepL
		part1	CustomizeablePowers\Empathy\MindWallArmGlows.part
	End

	Event
		EName	13
		Type	Local
		At	UArmR
		part1	CustomizeablePowers\Empathy\MindWallArmMotionGlows.part
		part2	CustomizeablePowers\Empathy\MindWallArmMotionGlows2.part
		PMagnet ElbowL
	End

	Event
		EName	14
		Type	Local
		At	LArmR
		part1	CustomizeablePowers\Empathy\MindWallArmMotionGlows.part
		part2	CustomizeablePowers\Empathy\MindWallArmMotionGlows2.part
		PMagnet HandL
	End

	Event
		EName	15
		Type	Local
		At	WepR
		part1	CustomizeablePowers\Empathy\MindWallArmGlows.part
	End
End

###############AUDIO#################################################
Condition

	On	Time
	Time	15

	Event
		EName	Ring
		Type	Local
		At	Origin
		Sound heal1 60 60 .55
	End

End

Condition

	On	Time
	Time	20

	Event
		EName	Ring
		Type	Local
		At	Origin
		Sound regenaura 60 60 .55
	End

End

End