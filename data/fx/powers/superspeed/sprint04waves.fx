#############################################################
## Sprint04Waves.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	FootR
		part1	:SprintWaves1.part
		part2	:SprintWavesLegs1.part
	End

	Event
		Type	Local
		At	ULEGL
		part1	:SprintWaves1.part
		part2	:SprintWavesLegs1.part
	End

	Event
		Type	Local
		At	LLEGL
		part1	:SprintWaves1.part
		part2	:SprintWavesLegs1.part
	End

	Event
		Type	Local
		At	FootL
		part1	:SprintWaves1.part
		part2	:SprintWavesLegs1.part
	End

	Event
		Type	Local
		At	ULEGR
		part1	:SprintWaves1.part
		part2	:SprintWavesLegs1.part
	End

	Event
		Type	Local
		At	LLEGR
		part1	:SprintWaves1.part
		part2	:SprintWavesLegs1.part
	End

	Event
		Type	Local
		At	Head
		part2	:SprintWaves.part
		part2	:SprintWavesLegs.part
	End

	Event
		Type	Local
		At	UArmL
		part1	:SprintWaves.part
		part2	:SprintWavesLegs.part
	End

	Event
		Type	Local
		At	LArmL
		part1	:SprintWaves.part
		part2	:SprintWavesLegs.part
	End

	Event
		Type	Local
		At	WepL
		part1	:SprintWaves.part
		part2	:SprintWavesLegs.part
	End

	Event
		Type	Local
		At	UArmR
		part1	:SprintWaves.part
		part2	:SprintWavesLegs.part
	End

	Event
		Type	Local
		At	LArmR
		part1	:SprintWaves.part
		part2	:SprintWavesLegs.part
	End

	Event
		Type	Local
		At	WepR
		part1	:SprintWaves.part
		part2	:SprintWavesLegs.part
	End

	Event
		Type	Local
		At	Chest
		part1	:SprintWaves.part
		part2	:SprintWavesLegs.part
	End
End

#############################################################
## Ground & Physics FX
#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Root
		ChildFX	fx\POWERS\SuperSpeed\Sprint.fx
	End
End

#############################################################

End