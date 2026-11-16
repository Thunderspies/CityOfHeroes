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
		part2	powers\DarknessControl\Smoke2a.part
		Part3	powers\DarknessControl\DarkServentTendril.part
		Sound miasma_loop 60 60 0.79
	End

	Event
		Type	Local
		At	ULEGL
		part1	powers\DarknessControl\Smokea.part
		part2	powers\DarknessControl\Smoke2a.part
		Part3	powers\DarknessControl\DarkServentTendril.part
		Part	powers\DarknessControl\ServantCore.part
		PMagnet LLEGL
		Sound obsidian_loop 60 60 0.49
	End

	Event
		Type	Local
		At	LLEGL
		part1	powers\DarknessControl\Smokea.part
		part2	powers\DarknessControl\Smoke2a.part
		Part3	powers\DarknessControl\DarkServentTendril.part
		Part	powers\DarknessControl\ServantCore.part
		PMagnet FootL
	End

	Event
		Type	Local
		At	FootL
		part1	powers\DarknessControl\Smokea.part
		part2	powers\DarknessControl\Smoke2a.part
		Part3	powers\DarknessControl\DarkServentTendril.part
		Sound rage 100 100 1.0
	End

	Event
		Type	Local
		At	ULEGR
		part1	powers\DarknessControl\Smokea.part
		part2	powers\DarknessControl\Smoke2a.part
		Part3	powers\DarknessControl\DarkServentTendril.part
		Part	powers\DarknessControl\ServantCore.part
		PMagnet LlegR
	End

	Event
		Type	Local
		At	LLEGR
		part1	powers\DarknessControl\Smokea.part
		part2	powers\DarknessControl\Smoke2a.part
		Part3	powers\DarknessControl\DarkServentTendril.part
		Part	powers\DarknessControl\ServantCore.part
		PMagnet FootR
	End

	Event
		Type	Local
		At	Head
		part1	powers\DarknessControl\Smokea.part
		part2	powers\DarknessControl\Smoke2a.part
		Part3	powers\DarknessControl\DarkServentTendril.part
		Part	powers\DarknessControl\ServantHead.part
	End

	Event
		Type	Local
		At	Chest
		part1	powers\DarknessControl\Smokea.part
		part2	powers\DarknessControl\Smoke2a.part
		Part3	powers\DarknessControl\DarkServentTendril.part
		Part	powers\DarknessControl\ServantCore.part
		Pmagnet	Waist
	End

	Event
		Type	Local
		At	UArmL
		part1	powers\DarknessControl\Smokea.part
		part2	powers\DarknessControl\Smoke2a.part
		Part3	powers\DarknessControl\DarkServentTendril.part
		Part	powers\DarknessControl\ServantCore.part
		PMagnet LArmL
	End

	Event
		Type	Local
		At	LArmL
		part1	powers\DarknessControl\Smokea.part
		part2	powers\DarknessControl\Smoke2a.part
		Part3	powers\DarknessControl\DarkServentTendril.part
		Part	powers\DarknessControl\ServantCore.part
		PMagnet WepL
	End

	Event
		Type	Local
		At	UArmR
		part1	powers\DarknessControl\Smokea.part
		part2	powers\DarknessControl\Smoke2a.part
		Part3	powers\DarknessControl\DarkServentTendril.part
		Part	powers\DarknessControl\ServantCore.part
		PMagnet LArmR
	End

	Event
		Type	Local
		At	LArmR
		part1	powers\DarknessControl\Smokea.part
		part2	powers\DarknessControl\Smoke2a.part
		Part3	powers\DarknessControl\DarkServentTendril.part
		Part	powers\DarknessControl\ServantCore.part
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
		Part1	ENEMYFX\Circle\EntityEffects\CircleOfThorns\CircleOfThornsEyes_42Glow.part
		Part2	ENEMYFX\Circle\EntityEffects\CircleOfThorns\CircleOfThornsEyes_42Pupil.part

	End

	Event
		Type	Local
		At	Prime
		AltPiv	2
		Part1	ENEMYFX\Circle\EntityEffects\CircleOfThorns\CircleOfThornsEyes_42Glow.part
		Part2	ENEMYFX\Circle\EntityEffects\CircleOfThorns\CircleOfThornsEyes_42Pupil.part

	End

	Event
		ENAME	Headglow
		Type	Local
		At	Head
		Part1	ENEMYFX\Circle\EntityEffects\CircleOfThorns\CircleOfThornsDieity_42.part
		Part2	ENEMYFX\Circle\EntityEffects\CircleOfThorns\CircleOfThornsDieitySmall_42.part
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