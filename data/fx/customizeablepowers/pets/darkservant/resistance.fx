#############################################################
## DarkSeventBody.fx
#############################################################

FxInfo

Flags InheritAlpha

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	FootR
		part1	:Smokea.part

		part2	:Smoke2a.part
		Part3	:DarkServentTendril.part
		Part	:DarkServentTendril_Subtractive.part
		Sound miasma_loop 60 60 0.79
	End

	Event
		Type	Local
		At	ULEGR
		part1	:Smokea.part
		part2	:Smoke2a.part
		Part3	:DarkServentTendril.part
		Part	:DarkServentTendril_Subtractive.part
		Part	:ServantCore.part
		Part	:ServantCore_subtractive.part
		PMagnet LLEGR
		Sound obsidian_loop 60 60 0.49
	End

	Event
		Type	Local
		At	LLEGR
		part1	:Smokea.part
		part2	:Smoke2a.part
		Part3	:DarkServentTendril.part
		Part	:DarkServentTendril_Subtractive.part
		Part	:ServantCore.part
		Part	:ServantCore_subtractive.part
		PMagnet FootR
	End

	Event
		Type	Local
		At	FootL
		part1	:Smokea.part
		part2	:Smoke2a.part
		Part3	:DarkServentTendril.part
		Part	:DarkServentTendril_Subtractive.part
		Sound rage 100 100 1.0
	End

	Event
		Type	Local
		At	ULEGL
		part1	:Smokea.part
		part2	:Smoke2a.part
		Part3	:DarkServentTendril.part
		Part	:DarkServentTendril_Subtractive.part
		Part	:ServantCore.part
		Part	:ServantCore_subtractive.part
		PMagnet LlegL
	End

	Event
		Type	Local
		At	LLEGL
		part1	:Smokea.part
		part2	:Smoke2a.part
		Part3	:DarkServentTendril.part
		Part	:DarkServentTendril_Subtractive.part
		Part	:ServantCore.part
		Part	:ServantCore_subtractive.part
		PMagnet FootL
	End

	Event
		Type	Local
		At	Head
		part1	:Smokea.part
		part2	:Smoke2a.part
		Part3	:DarkServentTendril.part
		Part	:DarkServentTendril_Subtractive.part
		Part	:ServantHead.part
	End

	Event
		Type	Local
		At	Chest
		part1	:Smokea.part
		part2	:Smoke2a.part
		Part3	:DarkServentTendril.part
		Part	:DarkServentTendril_Subtractive.part
		Part	:ServantCore.part
		Part	:ServantCore_subtractive.part
		Pmagnet	Waist
	End

	Event
		Type	Local
		At	UArmL
		part1	:Smokea.part
		part2	:Smoke2a.part
		Part3	:DarkServentTendril.part
		Part	:DarkServentTendril_Subtractive.part
		Part	:ServantCore.part
		Part	:ServantCore_subtractive.part
		PMagnet LArmL
	End

	Event
		Type	Local
		At	LArmL
		part1	:Smokea.part
		part2	:Smoke2a.part
		Part3	:DarkServentTendril.part
		Part	:DarkServentTendril_Subtractive.part
		Part	:ServantCore.part
		Part	:ServantCore_subtractive.part
		PMagnet WepL
	End

	Event
		Type	Local
		At	UArmR
		part1	:Smokea.part
		part2	:Smoke2a.part
		Part3	:DarkServentTendril.part
		Part	:DarkServentTendril_Subtractive.part
		Part	:ServantCore.part
		Part	:ServantCore_subtractive.part
		PMagnet LArmR
	End

	Event
		Type	Local
		At	LArmR
		part1	:Smokea.part
		part2	:Smoke2a.part
		Part3	:DarkServentTendril.part
		Part	:DarkServentTendril_Subtractive.part
		Part	:ServantCore.part
		Part	:ServantCore_subtractive.part
		PMagnet WepR
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local
		At	Eyes
		Geom	CircleOfThorns
	End

	Event
		Type	Local
		At	Prime
		AltPiv	1
		Part1	:CircleOfThornsEyes_42Glow.part
		Part2	:CircleOfThornsEyes_42Pupil.part

	End

	Event
		Type	Local
		At	Prime
		AltPiv	2
		Part1	:CircleOfThornsEyes_42Glow.part
		Part2	:CircleOfThornsEyes_42Pupil.part

	End

	Event
		ENAME	Headglow
		Type	Local
		At	Head
		Part1	:CircleOfThornsDieity_42.part
		Part2	:CircleOfThornsDieitySmall_42.part
	End
End

#############################################################

Condition
	On 		TriggerBits
	TriggerBits	DEATH

	Event
		EName 	ALL
		Type	Destroy
	End
End

#############################################################

End