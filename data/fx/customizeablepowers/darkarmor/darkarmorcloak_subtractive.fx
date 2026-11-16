#########################################################
##	Fire_Ball
##
FxInfo

##################################
Condition

	On	Time
	Time	24

	Event
		Type	Local
		At	Origin
		Sound darkarmor2 60 60 .99
	End
End

Condition
	On	Time
	Time	60

	Event
		EName	2
		Type	Local
		At	FootR
		part	CustomizeablePowers\DarkArmor\Smokex.part
		part	CustomizeablePowers\DarkArmor\Smokex_Additive.part
		part	CustomizeablePowers\DarkArmor\Smoke2x.part
		Part	CustomizeablePowers\DarkArmor\DarkBlastHitTendrilx.part
		part	CustomizeablePowers\DarkArmor\Smokes.part
		part	CustomizeablePowers\DarkArmor\Smokes_Additive.part
		part	CustomizeablePowers\DarkArmor\Smoke2s.part
		Part	CustomizeablePowers\DarkArmor\Smoke2s_Subtractive.part
		Part	CustomizeablePowers\DarkArmor\LiteDarkBlastHitTendril.part
		Part	CustomizeablePowers\DarkArmor\literDarkBlastHitTendril_Subtractive.part
		Sound miasma_loop 60 60 .65
		Bhvr	Behaviors\soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 380

	End

	Event
		EName	3
		Type	Local
		At	ULEGL
		part1	CustomizeablePowers\DarkArmor\Smokex.part
		part1	CustomizeablePowers\DarkArmor\Smokex_Additive.part
		part2	CustomizeablePowers\DarkArmor\Smoke2x.part
		Part3	CustomizeablePowers\DarkArmor\DarkBlastHitTendrilx.part
		part	CustomizeablePowers\DarkArmor\Smokes.part
		part	CustomizeablePowers\DarkArmor\Smokes_Additive.part
		part	CustomizeablePowers\DarkArmor\Smoke2s.part
		Part	CustomizeablePowers\DarkArmor\Smoke2s_Subtractive.part
		Part	CustomizeablePowers\DarkArmor\LiteDarkBlastHitTendril.part
		Part	CustomizeablePowers\DarkArmor\literDarkBlastHitTendril_Subtractive.part
		Sound intel2_loop 60 60 .8
		Bhvr	Behaviors\soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 300

	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		part1	CustomizeablePowers\DarkArmor\Smokex.part
		part1	CustomizeablePowers\DarkArmor\Smokex_Additive.part
		part2	CustomizeablePowers\DarkArmor\Smoke2x.part
		Part3	CustomizeablePowers\DarkArmor\DarkBlastHitTendrilx.part
		part	CustomizeablePowers\DarkArmor\Smokes.part
		part	CustomizeablePowers\DarkArmor\Smokes_Additive.part
		part	CustomizeablePowers\DarkArmor\Smoke2s.part
		Part	CustomizeablePowers\DarkArmor\Smoke2s_Subtractive.part
		Part	CustomizeablePowers\DarkArmor\LiteDarkBlastHitTendril.part
		Part	CustomizeablePowers\DarkArmor\literDarkBlastHitTendril_Subtractive.part

	End

	Event
		EName	5
		Type	Local
		At	FootL
		part1	CustomizeablePowers\DarkArmor\Smokex.part
		part1	CustomizeablePowers\DarkArmor\Smokex_Additive.part
		part2	CustomizeablePowers\DarkArmor\Smoke2x.part
		Part3	CustomizeablePowers\DarkArmor\DarkBlastHitTendrilx.part
		part	CustomizeablePowers\DarkArmor\Smokes.part
		part	CustomizeablePowers\DarkArmor\Smokes_Additive.part
		part	CustomizeablePowers\DarkArmor\Smoke2s.part
		Part	CustomizeablePowers\DarkArmor\Smoke2s_Subtractive.part
		Part	CustomizeablePowers\DarkArmor\LiteDarkBlastHitTendril.part
		Part	CustomizeablePowers\DarkArmor\literDarkBlastHitTendril_Subtractive.part
		Sound rage 100 100 1.0

	End



	Event
		EName	6
		Type	Local
		At	ULEGR
		part1	CustomizeablePowers\DarkArmor\Smokex.part
		part1	CustomizeablePowers\DarkArmor\Smokex_Additive.part
		part2	CustomizeablePowers\DarkArmor\Smoke2x.part
		Part3	CustomizeablePowers\DarkArmor\DarkBlastHitTendrilx.part
		part	CustomizeablePowers\DarkArmor\Smokes.part
		part	CustomizeablePowers\DarkArmor\Smokes_Additive.part
		part	CustomizeablePowers\DarkArmor\Smoke2s.part
		Part	CustomizeablePowers\DarkArmor\Smoke2s_Subtractive.part
		Part	CustomizeablePowers\DarkArmor\LiteDarkBlastHitTendril.part
		Part	CustomizeablePowers\DarkArmor\literDarkBlastHitTendril_Subtractive.part
		#PMagnet LlegR

	End

	Event
		EName	7
		Type	Local
		At	LLEGR
		part1	CustomizeablePowers\DarkArmor\Smokex.part
		part1	CustomizeablePowers\DarkArmor\Smokex_Additive.part
		part2	CustomizeablePowers\DarkArmor\Smoke2x.part
		Part3	CustomizeablePowers\DarkArmor\DarkBlastHitTendrilx.part
		part	CustomizeablePowers\DarkArmor\Smokes.part
		part	CustomizeablePowers\DarkArmor\Smokes_Additive.part
		part	CustomizeablePowers\DarkArmor\Smoke2s.part
		Part	CustomizeablePowers\DarkArmor\Smoke2s_Subtractive.part
		Part	CustomizeablePowers\DarkArmor\LiteDarkBlastHitTendril.part
		Part	CustomizeablePowers\DarkArmor\literDarkBlastHitTendril_Subtractive.part
		#PMagnet FootR
	End


###############################################################################

	Event
		EName	9
		Type	Local
		At	Head
		part1	CustomizeablePowers\DarkArmor\Smokex.part
		part1	CustomizeablePowers\DarkArmor\Smokex_Additive.part
		part2	CustomizeablePowers\DarkArmor\Smoke2x.part
		Part3	CustomizeablePowers\DarkArmor\DarkBlastHitTendrilx.part
		part	CustomizeablePowers\DarkArmor\Smokes.part
		part	CustomizeablePowers\DarkArmor\Smokes_Additive.part
		part	CustomizeablePowers\DarkArmor\Smoke2s.part
		Part	CustomizeablePowers\DarkArmor\Smoke2s_Subtractive.part
		Part	CustomizeablePowers\DarkArmor\LiteDarkBlastHitTendril.part
		Part	CustomizeablePowers\DarkArmor\literDarkBlastHitTendril_Subtractive.part
	End


##########################################################################
##Arm
##################################################################

	Event
		EName	10
		Type	Local
		At	UArmL
		part1	CustomizeablePowers\DarkArmor\Smokex.part
		part1	CustomizeablePowers\DarkArmor\Smokex_Additive.part
		part2	CustomizeablePowers\DarkArmor\Smoke2x.part
		Part3	CustomizeablePowers\DarkArmor\DarkBlastHitTendrilx.part
		part	CustomizeablePowers\DarkArmor\Smokes.part
		part	CustomizeablePowers\DarkArmor\Smokes_Additive.part
		part	CustomizeablePowers\DarkArmor\Smoke2s.part
		Part	CustomizeablePowers\DarkArmor\Smoke2s_Subtractive.part
		Part	CustomizeablePowers\DarkArmor\LiterDarkBlastHitTendril.part
		Part	CustomizeablePowers\DarkArmor\literDarkBlastHitTendril_Subtractive.part
		#PMagnet LArmL

	End

	Event
		EName	11
		Type	Local
		At	LArmL
		part	CustomizeablePowers\DarkArmor\Smokex.part
		part	CustomizeablePowers\DarkArmor\Smokex_Additive.part
		part	CustomizeablePowers\DarkArmor\Smoke2x.part
		Part	CustomizeablePowers\DarkArmor\DarkBlastHitTendrilx.part
		part	CustomizeablePowers\DarkArmor\Smokes.part
		part	CustomizeablePowers\DarkArmor\Smokes_Additive.part
		part	CustomizeablePowers\DarkArmor\Smoke2s.part
		Part	CustomizeablePowers\DarkArmor\Smoke2s_Subtractive.part
		Part	CustomizeablePowers\DarkArmor\LiterDarkBlastHitTendril.part
		Part	CustomizeablePowers\DarkArmor\literDarkBlastHitTendril_Subtractive.part
		#PMagnet WepL
	End

	Event
		EName	13
		Type	Local
		At	UArmR
		part	CustomizeablePowers\DarkArmor\Smokex.part
		part	CustomizeablePowers\DarkArmor\Smokex_Additive.part
		part	CustomizeablePowers\DarkArmor\Smoke2x.part
		Part	CustomizeablePowers\DarkArmor\DarkBlastHitTendrilx.part
		part	CustomizeablePowers\DarkArmor\Smokes.part
		part	CustomizeablePowers\DarkArmor\Smokes_Additive.part
		part	CustomizeablePowers\DarkArmor\Smoke2s.part
		Part	CustomizeablePowers\DarkArmor\Smoke2s_Subtractive.part
		Part	CustomizeablePowers\DarkArmor\LiterDarkBlastHitTendril.part
		Part	CustomizeablePowers\DarkArmor\literDarkBlastHitTendril_Subtractive.part
		#PMagnet LArmR

	End

	Event
		EName	14
		Type	Local
		At	LArmR
		part1	CustomizeablePowers\DarkArmor\Smokex.part
		part1	CustomizeablePowers\DarkArmor\Smokex_Additive.part
		part2	CustomizeablePowers\DarkArmor\Smoke2x.part
		Part5	CustomizeablePowers\DarkArmor\DarkBlastHitTendrilx.part
		part	CustomizeablePowers\DarkArmor\Smokes.part
		part	CustomizeablePowers\DarkArmor\Smokes_Additive.part
		part	CustomizeablePowers\DarkArmor\Smoke2s.part
		Part	CustomizeablePowers\DarkArmor\Smoke2s_Subtractive.part
		Part	CustomizeablePowers\DarkArmor\LiterDarkBlastHitTendril.part
		Part	CustomizeablePowers\DarkArmor\literDarkBlastHitTendril_Subtractive.part
	End

End

End

