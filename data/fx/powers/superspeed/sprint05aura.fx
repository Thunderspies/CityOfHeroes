#############################################################
## Sprint05Aura.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	FootR
		part1	:SprintWaves2.part
		part2	:SprintWavesLegs2.part
	End

	Event
		Type	Local
		At	ULEGL
		part1	:SprintWavesTop2.part
		part2	:SprintWavesLegsTop2.part
	End

	Event
		Type	Local
		At	LLEGL
		part1	:SprintWaves2.part
		part2	:SprintWavesLegs2.part
	End

	Event
		Type	Local
		At	FootL
		part1	:SprintWaves2.part
		part2	:SprintWavesLegs2.part
	End

	Event
		Type	Local
		At	ULEGR
		part1	:SprintWavesTop2.part
		part2	:SprintWavesLegsTop2.part
	End

	Event
		Type	Local
		At	LLEGR
		part1	:SprintWaves2.part
		part2	:SprintWavesLegs2.part
	End

	Event
		Type	Local
		At	Head
		part2	:SprintWavesTop.part
		part2	:SprintWavesLegsTop.part
	End

	Event
		Type	Local
		At	UArmL
		part1	:SprintWavesTop.part
		part2	:SprintWavesLegsTop.part
	End

	Event
		Type	Local
		At	LArmL
		part1	:SprintWavesTop.part
		part2	:SprintWavesLegsTop.part
	End

	Event
		Type	Local
		At	WepL
		part1	:SprintWavesTop.part
		part2	:SprintWavesLegsTop.part
	End

	Event
		Type	Local
		At	UArmR
		part1	:SprintWavesTop.part
		part2	:SprintWavesLegsTop.part
	End

	Event
		Type	Local
		At	LArmR
		part1	:SprintWavesTop.part
		part2	:SprintWavesLegsTop.part
	End

	Event
		Type	Local
		At	WepR
		part1	:SprintWavesTop.part
		part2	:SprintWavesLegsTop.part
	End

	Event
		Type	Local
		At	Chest
		part1	:SprintWavesTop.part
		part2	:SprintWavesLegsTop.part
	End
End

#############################################################
## Ground & Physics FX
#############################################################

Condition
	On	Time
	Time	0

	Event
		EName 	GroundFX
		Type	Local
		At	Root
		ChildFX	fx\POWERS\SuperSpeed\Sprint.fx
	End
End

#############################################################

End