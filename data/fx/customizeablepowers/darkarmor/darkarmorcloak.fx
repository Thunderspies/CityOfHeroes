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
		part1	CustomizeablePowers\DarkArmor\Smokex.part
		part2	CustomizeablePowers\DarkArmor\Smoke2x.part
		Part3	CustomizeablePowers\DarkArmor\DarkBlastHitTendrilx.part
		
		part	CustomizeablePowers\DarkArmor\Smokes.part
		part	CustomizeablePowers\DarkArmor\Smoke2s.part
		Part	CustomizeablePowers\DarkArmor\LiteDarkBlastHitTendril.part
		Sound miasma_loop 60 60 .65
		Bhvr	Behaviors\soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 380

	End


#LegL



	Event
		EName	3
		Type	Local
		At	ULEGL
		part1	CustomizeablePowers\DarkArmor\Smokex.part
		part2	CustomizeablePowers\DarkArmor\Smoke2x.part
		
		Part3	CustomizeablePowers\DarkArmor\DarkBlastHitTendrilx.part
		part	CustomizeablePowers\DarkArmor\Smokes.part
		part	CustomizeablePowers\DarkArmor\Smoke2s.part
		Part	CustomizeablePowers\DarkArmor\LiteDarkBlastHitTendril.part
		#PMagnet LLEGL
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
		part2	CustomizeablePowers\DarkArmor\Smoke2x.part
		
		Part3	CustomizeablePowers\DarkArmor\DarkBlastHitTendrilx.part
		part	CustomizeablePowers\DarkArmor\Smokes.part
		part	CustomizeablePowers\DarkArmor\Smoke2s.part
		Part	CustomizeablePowers\DarkArmor\LiteDarkBlastHitTendril.part
		
	End

	Event
		EName	5		
		Type	Local
		At	FootL
		part1	CustomizeablePowers\DarkArmor\Smokex.part
		part2	CustomizeablePowers\DarkArmor\Smoke2x.part
		
		Part3	CustomizeablePowers\DarkArmor\DarkBlastHitTendrilx.part
		part	CustomizeablePowers\DarkArmor\Smokes.part
		part	CustomizeablePowers\DarkArmor\Smoke2s.part
		Part	CustomizeablePowers\DarkArmor\LiteDarkBlastHitTendril.part
		Sound rage 100 100 1.0
	
	End



	Event
		EName	6
		Type	Local
		At	ULEGR
		part1	CustomizeablePowers\DarkArmor\Smokex.part
		part2	CustomizeablePowers\DarkArmor\Smoke2x.part
		
		Part3	CustomizeablePowers\DarkArmor\DarkBlastHitTendrilx.part
		part	CustomizeablePowers\DarkArmor\Smokes.part
		part	CustomizeablePowers\DarkArmor\Smoke2s.part
		Part	CustomizeablePowers\DarkArmor\LiteDarkBlastHitTendril.part
		#PMagnet LlegR

	End


#LegR


	Event
		EName	7
		Type	Local
		At	LLEGR
		part1	CustomizeablePowers\DarkArmor\Smokex.part
		part2	CustomizeablePowers\DarkArmor\Smoke2x.part
		
		Part3	CustomizeablePowers\DarkArmor\DarkBlastHitTendrilx.part
		part	CustomizeablePowers\DarkArmor\Smokes.part
		part	CustomizeablePowers\DarkArmor\Smoke2s.part
		Part	CustomizeablePowers\DarkArmor\LiteDarkBlastHitTendril.part
		#PMagnet FootR
	End

		
###############################################################################

	Event
		EName	9
		Type	Local
		At	Head
		part1	CustomizeablePowers\DarkArmor\Smokex.part
		part2	CustomizeablePowers\DarkArmor\Smoke2x.part
		
		Part3	CustomizeablePowers\DarkArmor\DarkBlastHitTendrilx.part
		part	CustomizeablePowers\DarkArmor\Smokes.part
		part	CustomizeablePowers\DarkArmor\Smoke2s.part
		Part	CustomizeablePowers\DarkArmor\LiteDarkBlastHitTendril.part
	End

		
##########################################################################
##Arm
##################################################################

	Event
		EName	10
		Type	Local
		At	UArmL
		part1	CustomizeablePowers\DarkArmor\Smokex.part
		part2	CustomizeablePowers\DarkArmor\Smoke2x.part
		
		Part3	CustomizeablePowers\DarkArmor\DarkBlastHitTendrilx.part
		part	CustomizeablePowers\DarkArmor\Smokes.part
		part	CustomizeablePowers\DarkArmor\Smoke2s.part
		Part	CustomizeablePowers\DarkArmor\LiterDarkBlastHitTendril.part
		#PMagnet LArmL

	End

	Event
		EName	11
		Type	Local
		At	LArmL
		part1	CustomizeablePowers\DarkArmor\Smokex.part
		part2	CustomizeablePowers\DarkArmor\Smoke2x.part
		
		Part3	CustomizeablePowers\DarkArmor\DarkBlastHitTendrilx.part
		part	CustomizeablePowers\DarkArmor\Smokes.part
		part	CustomizeablePowers\DarkArmor\Smoke2s.part
		Part	CustomizeablePowers\DarkArmor\LiterDarkBlastHitTendril.part
		#PMagnet WepL
	End

	Event
		EName	13
		Type	Local
		At	UArmR
		part1	CustomizeablePowers\DarkArmor\Smokex.part
		part2	CustomizeablePowers\DarkArmor\Smoke2x.part
		
		Part3	CustomizeablePowers\DarkArmor\DarkBlastHitTendrilx.part
		part	CustomizeablePowers\DarkArmor\Smokes.part
		part	CustomizeablePowers\DarkArmor\Smoke2s.part
		Part	CustomizeablePowers\DarkArmor\LiterDarkBlastHitTendril.part
		#PMagnet LArmR

	End

	Event
		EName	14
		Type	Local
		At	LArmR
		part1	CustomizeablePowers\DarkArmor\Smokex.part
		part2	CustomizeablePowers\DarkArmor\Smoke2x.part
		
		Part5	CustomizeablePowers\DarkArmor\DarkBlastHitTendrilx.part
		part	CustomizeablePowers\DarkArmor\Smokes.part
		part	CustomizeablePowers\DarkArmor\Smoke2s.part
		Part	CustomizeablePowers\DarkArmor\LiterDarkBlastHitTendril.part
		#PMagnet WepR
	End

End     

End	

