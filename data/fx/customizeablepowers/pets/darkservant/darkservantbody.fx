#########################################################
##	Fire_Ball
##
FxInfo

##################################

Condition
	On	Time
	Time	0

	Event
		EName	2
		Type	Local
		At	FootR
		part2	:Smoke2a.part
		Part3	:DarkServentTendril.part
		Sound miasma_loop 60 60 .79

	End


#LegL



	Event
		EName	3
		Type	Local
		At	ULEGL
		part1	:Smokea.part
		part2	:Smoke2a.part
		Part3	:DarkServentTendril.part
		Part	:ServantCore.part
		PMagnet LLEGL
		Sound obsidian_loop 60 60 .49

	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		part1	:Smokea.part
		part2	:Smoke2a.part
		Part3	:DarkServentTendril.part
		Part	:ServantCore.part
		PMagnet FootL
	End

	Event
		EName	5
		Type	Local
		At	FootL
		part1	:Smokea.part
		part2	:Smoke2a.part
		Part3	:DarkServentTendril.part
		Sound rage 100 100 1.0

	End



	Event
		EName	6
		Type	Local
		At	ULEGR
		part1	:Smokea.part
		part2	:Smoke2a.part
		Part3	:DarkServentTendril.part
		Part	:ServantCore.part
		PMagnet LlegR

	End



	Event
		EName	7
		Type	Local
		At	LLEGR
		part1	:Smokea.part
		part2	:Smoke2a.part
		Part3	:DarkServentTendril.part
		Part	:ServantCore.part
		PMagnet FootR
	End


###############################################################################

	Event
		EName	9
		Type	Local
		At	Head
		part1	:Smokea.part
		part2	:Smoke2a.part
		Part3	:DarkServentTendril.part
		Part	:ServantHead.part

	End

	Event
		Type	Local
		At	Chest
		part1	:Smokea.part
		part2	:Smoke2a.part
		Part3	:DarkServentTendril.part
		Part	:ServantCore.part
		Pmagnet	Waist
	End

##########################################################################
##Arm
##################################################################

	Event
		EName	10
		Type	Local
		At	UArmL
		part1	:Smokea.part
		part2	:Smoke2a.part
		Part3	:DarkServentTendril.part
		Part	:ServantCore.part
		PMagnet LArmL

	End

	Event
		EName	11
		Type	Local
		At	LArmL
		part1	:Smokea.part
		part2	:Smoke2a.part
		Part3	:DarkServentTendril.part
		Part	:ServantCore.part
		PMagnet WepL
	End

	Event
		EName	13
		Type	Local
		At	UArmR
		part1	:Smokea.part
		part2	:Smoke2a.part
		Part3	:DarkServentTendril.part
		Part	:ServantCore.part
		PMagnet LArmR

	End

	Event
		EName	14
		Type	Local
		At	LArmR
		part1	:Smokea.part
		part2	:Smoke2a.part
		Part3	:DarkServentTendril.part
		Part	:ServantCore.part
		PMagnet WepR
	End

End


End

