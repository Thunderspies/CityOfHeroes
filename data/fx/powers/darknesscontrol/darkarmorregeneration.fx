#############################################################
## DarkArmorRegeneration.fx
#############################################################

FxInfo
LifeSpan	120

#############################################################

Input
	Inpname	Hips
End

#############################################################


Condition
	On	Time
	Time	60

	Event
		EName	2
		Type	Local
		At	FootR
		part1	:Smoke.part
		part2	:Smoke2.part
		Part3	:DarkBlastHitTendril.part
		Sound miasma_loop 60 60 .77

	End


#LegL



	Event
		EName	3
		Type	Local
		At	ULEGL
		part1	:Smoke.part
		part2	:Smoke2.part
		Part3	:DarkBlastHitTendril.part
		#PMagnet LLEGL
		Sound darkregen 60 60 .75

	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		part1	:Smoke.part
		part2	:Smoke2.part
		Part3	:DarkBlastHitTendril.part
		#PMagnet FootL
	End

	Event
		EName	5
		Type	Local
		At	FootL
		part1	:Smoke.part
		part2	:Smoke2.part
		Part3	:DarkBlastHitTendril.part
		#Sound	rage 80 30 .88

	End



	Event
		EName	6
		Type	Local
		At	ULEGR
		part1	:Smoke.part
		part2	:Smoke2.part
		Part3	:DarkBlastHitTendril.part

	End


#LegR


	Event
		EName	7
		Type	Local
		At	LLEGR
		part1	:Smoke.part
		part2	:Smoke2.part
		Part3	:DarkBlastHitTendril.part
		#PMagnet FootR
	End


###############################################################################

	Event
		EName	9
		Type	Local
		At	Head
		part1	:Smoke.part
		part2	:Smoke2.part
		Part3	:DarkBlastHitTendril.part
	End


##########################################################################
##Arm
##################################################################

	Event
		EName	10
		Type	Local
		At	UArmL
		part1	:Smoke.part
		part2	:Smoke2.part
		Part3	:DarkBlastHitTendril.part
		#PMagnet LArmL

	End

	Event
		EName	11
		Type	Local
		At	LArmL
		part1	:Smoke.part
		part2	:Smoke2.part
		Part3	:DarkBlastHitTendril.part
		#PMagnet WepL
	End

	Event
		EName	13
		Type	Local
		At	UArmR
		part1	:Smoke.part
		part2	:Smoke2.part
		Part3	:DarkBlastHitTendril.part
		#PMagnet LArmR

	End

	Event
		EName	14
		Type	Local
		At	LArmR
		part1	:Smoke.part
		part2	:Smoke2.part
		Part3	:DarkBlastHitTendril.part
		#PMagnet WepR
	End

	Event
		EName	RINGS
		Type	Local
		At	Hips

		part1	:RegenerationRing.part
	End
End

Condition
	On	Time
	Time	0

	Event
		EName	9a5
		Type	Local
		At	root

		Part1	:DarkTarRingsIn.Part
		Sound regen8 80 80 .56
		LifeSpan	110
	End

End

End

