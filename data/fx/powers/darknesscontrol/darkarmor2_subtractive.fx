#############################################################
## DarkArmor2_Subtractive.fx
#############################################################

FxInfo

Input
	Inpname	Hips
End

#############################################################


Condition
	On	Time
	Time	60

	Event
		EName 	BottleRocket01
		Type	Local
		At	Root
		Geom	roottochestadjustment

	End

	Event
		EName 	BottleRocket02
		Type	Local
		At	BottleRocket01
		altpiv	1

		Part	:Ghosts2.part
		Part	:Ghosts.part
		Part	:GhostStreaks.part
	End

#######################################################

	Event
		EName	2
		Type	Local
		At	FootR
		part1	:LiteSmoke.part
		part2	:LiteSmoke2.part
		Part3	:LiteDarkBlastHitTendril.part
		Part	:literDarkBlastHitTendril_Subtractive.part
		Sound forcefield5_loop 60 60 .45
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 330


	End


#LegL



	Event
		EName	3
		Type	Local
		At	ULEGL
		part1	:LiteSmoke.part
		part2	:LiteSmoke2.part
		Part3	:LiteDarkBlastHitTendril.part
		Part	:literDarkBlastHitTendril_Subtractive.part
		#PMagnet LLEGL
		Sound miasma_loop 60 60 .65
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 400


	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		part1	:LiteSmoke.part
		part2	:LiteSmoke2.part
		Part3	:LiteDarkBlastHitTendril.part
		Part	:literDarkBlastHitTendril_Subtractive.part
		#PMagnet FootL
		#Sound	darkfear_loop 60 20 .65
		#bhvr	behaviors/soundFade3.bhvr
		#Flags	PowerLoopingSound
		#Lifespan 370

	End

	Event
		EName	5
		Type	Local
		At	FootL
		part1	:LiteSmoke.part
		part2	:LiteSmoke2.part
		Part3	:LiteDarkBlastHitTendril.part
		Part	:literDarkBlastHitTendril_Subtractive.part
		Sound fear2c_loop 60 60 .65
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 400


	End



	Event
		EName	6
		Type	Local
		At	ULEGR
		part1	:LiteSmoke.part
		part2	:LiteSmoke2.part
		Part3	:LiteDarkBlastHitTendril.part
		Part	:literDarkBlastHitTendril_Subtractive.part
		#PMagnet LlegR

	End


#LegR


	Event
		EName	7
		Type	Local
		At	LLEGR
		part1	:LiteSmoke.part
		part2	:LiteSmoke2.part
		Part3	:LiteDarkBlastHitTendril.part
		Part	:literDarkBlastHitTendril_Subtractive.part
		#PMagnet FootR
	End


###############################################################################

	Event
		EName	9
		Type	Local
		At	Head
		part1	:LiterSmoke.part
		part2	:LiterSmoke2.part
		Part3	:LiterDarkBlastHitTendril.part
		Part	:literDarkBlastHitTendril_Subtractive.part

	End


##########################################################################
##Arm
##################################################################

	Event
		EName	10
		Type	Local
		At	UArmL
		part1	:LiterSmoke.part
		part2	:LiterSmoke2.part
		Part3	:LiterDarkBlastHitTendril.part
		Part	:literDarkBlastHitTendril_Subtractive.part
		#PMagnet LArmL

	End

	Event
		EName	11
		Type	Local
		At	LArmL
		part1	:LiterSmoke.part
		part2	:LiterSmoke2.part
		Part3	:LiterDarkBlastHitTendril.part
		Part	:literDarkBlastHitTendril_Subtractive.part
		#PMagnet WepL
	End

	Event
		EName	13
		Type	Local
		At	UArmR
		part1	:LiterSmoke.part
		part2	:LiterSmoke2.part
		Part3	:LiterDarkBlastHitTendril.part
		Part	:literDarkBlastHitTendril_Subtractive.part
		#PMagnet LArmR

	End

	Event
		EName	14
		Type	Local
		At	LArmR
		part1	:LiterSmoke.part
		part2	:LiterSmoke2.part
		Part3	:LiterDarkBlastHitTendril.part
		Part	:literDarkBlastHitTendril_Subtractive.part
		#PMagnet WepR
	End


End


#Condition
#
#	Event
#		Type	Local
#		At	root
#
#		Bhvr	:DarkRingQuicker.bhvr
#
#		Splat	Generic_Ring.tga
#
#		LifeSpan	60
#
#	End
#
#End
#
#Condition
#	On	Cycle
#	Time	50
#
#	Event
#		Type	Local
#		At	root
#
#		Bhvr	:DarkRingQuicker.bhvr
#
#		Splat	Generic_Ring.tga
#
#		LifeSpan	60
#
#	End
#
#End

Condition
	On	Time
	Time	0

	Event
		EName	9a5
		Type	Local
		At	root

#		Part1	:DarkArmorRing.Part
		Sound darkarmor2 60 60 .75

	End

End

End

