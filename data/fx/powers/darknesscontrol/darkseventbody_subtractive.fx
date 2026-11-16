#############################################################
## DarkSeventBody_Subtractive.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	FootR
		part2	powers\DarknessContro\Smoke2a.part
		Part3	powers\DarknessContro\DarkServentTendril.part
		Part	powers\DarknessContro\DarkServentTendril_Subtractive.part
		Sound miasma_loop 60 60 0.79
	End

	Event
		Type	Local
		At	ULEGR
		part1	powers\DarknessContro\Smokea.part
		part2	powers\DarknessContro\Smoke2a.part
		Part3	powers\DarknessContro\DarkServentTendril.part
		Part	powers\DarknessContro\DarkServentTendril_Subtractive.part
		Part	powers\DarknessContro\ServantCore.part
		Part	powers\DarknessContro\ServantCore_subtractive.part
		PMagnet LLEGR
		Sound obsidian_loop 60 60 0.49
	End

	Event
		Type	Local
		At	LLEGR
		part1	powers\DarknessContro\Smokea.part
		part2	powers\DarknessContro\Smoke2a.part
		Part3	powers\DarknessContro\DarkServentTendril.part
		Part	powers\DarknessContro\DarkServentTendril_Subtractive.part
		Part	powers\DarknessContro\ServantCore.part
		Part	powers\DarknessContro\ServantCore_subtractive.part
		PMagnet FootR
	End

	Event
		Type	Local
		At	FootL
		part1	powers\DarknessContro\Smokea.part
		part2	powers\DarknessContro\Smoke2a.part
		Part3	powers\DarknessContro\DarkServentTendril.part
		Part	powers\DarknessContro\DarkServentTendril_Subtractive.part
		Sound rage 100 100 1.0
	End

	Event
		Type	Local
		At	ULEGL
		part1	powers\DarknessContro\Smokea.part
		part2	powers\DarknessContro\Smoke2a.part
		Part3	powers\DarknessContro\DarkServentTendril.part
		Part	powers\DarknessContro\DarkServentTendril_Subtractive.part
		Part	powers\DarknessContro\ServantCore.part
		Part	powers\DarknessContro\ServantCore_subtractive.part
		PMagnet LlegL
	End

	Event
		Type	Local
		At	LLEGL
		part1	powers\DarknessContro\Smokea.part
		part2	powers\DarknessContro\Smoke2a.part
		Part3	powers\DarknessContro\DarkServentTendril.part
		Part	powers\DarknessContro\DarkServentTendril_Subtractive.part
		Part	powers\DarknessContro\ServantCore.part
		Part	powers\DarknessContro\ServantCore_subtractive.part
		PMagnet FootL
	End

	Event
		Type	Local
		At	Head
		part1	powers\DarknessContro\Smokea.part
		part2	powers\DarknessContro\Smoke2a.part
		Part3	powers\DarknessContro\DarkServentTendril.part
		Part	powers\DarknessContro\DarkServentTendril_Subtractive.part
		Part	powers\DarknessContro\ServantHead.part
	End

	Event
		Type	Local
		At	Chest
		part1	powers\DarknessContro\Smokea.part
		part2	powers\DarknessContro\Smoke2a.part
		Part3	powers\DarknessContro\DarkServentTendril.part
		Part	powers\DarknessContro\DarkServentTendril_Subtractive.part
		Part	powers\DarknessContro\ServantCore.part
		Part	powers\DarknessContro\ServantCore_subtractive.part
		Pmagnet	Waist
	End

	Event
		Type	Local
		At	UArmL
		part1	powers\DarknessContro\Smokea.part
		part2	powers\DarknessContro\Smoke2a.part
		Part3	powers\DarknessContro\DarkServentTendril.part
		Part	powers\DarknessContro\DarkServentTendril_Subtractive.part
		Part	powers\DarknessContro\ServantCore.part
		Part	powers\DarknessContro\ServantCore_subtractive.part
		PMagnet LArmL
	End

	Event
		Type	Local
		At	LArmL
		part1	powers\DarknessContro\Smokea.part
		part2	powers\DarknessContro\Smoke2a.part
		Part3	powers\DarknessContro\DarkServentTendril.part
		Part	powers\DarknessContro\DarkServentTendril_Subtractive.part
		Part	powers\DarknessContro\ServantCore.part
		Part	powers\DarknessContro\ServantCore_subtractive.part
		PMagnet WepL
	End

	Event
		Type	Local
		At	UArmR
		part1	powers\DarknessContro\Smokea.part
		part2	powers\DarknessContro\Smoke2a.part
		Part3	powers\DarknessContro\DarkServentTendril.part
		Part	powers\DarknessContro\DarkServentTendril_Subtractive.part
		Part	powers\DarknessContro\ServantCore.part
		Part	powers\DarknessContro\ServantCore_subtractive.part
		PMagnet LArmR
	End

	Event
		Type	Local
		At	LArmR
		part1	powers\DarknessContro\Smokea.part
		part2	powers\DarknessContro\Smoke2a.part
		Part3	powers\DarknessContro\DarkServentTendril.part
		Part	powers\DarknessContro\DarkServentTendril_Subtractive.part
		Part	powers\DarknessContro\ServantCore.part
		Part	powers\DarknessContro\ServantCore_subtractive.part
		PMagnet WepR
	End
End

#############################################################

End