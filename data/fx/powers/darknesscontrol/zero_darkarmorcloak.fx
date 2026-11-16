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
		part1	:Smokex.part
		part2	:Smoke2x.part
		Part3	:DarkBlastHitTendrilx.part
		
		part	:Smokes.part
		part	:Smoke2s.part
		Part	:LiteDarkBlastHitTendril.part
		Sound miasma_loop 60 60 .77

	End


#LegL



	Event
		EName	3
		Type	Local
		At	ULEGL
		part1	:Smokex.part
		part2	:Smoke2x.part
		
		Part3	:DarkBlastHitTendrilx.part
		part	:Smokes.part
		part	:Smoke2s.part
		Part	:LiteDarkBlastHitTendril.part
		#PMagnet LLEGL
		Sound intel2_loop 60 60 .8

	End

	Event
		EName	4
		Type	Local
		At	LLEGL
		part1	:Smokex.part
		part2	:Smoke2x.part
		
		Part3	:DarkBlastHitTendrilx.part
		part	:Smokes.part
		part	:Smoke2s.part
		Part	:LiteDarkBlastHitTendril.part
		Sound darkarmor2 60 60 .75
	End

	Event
		EName	5		
		Type	Local
		At	FootL
		part1	:Smokex.part
		part2	:Smoke2x.part
		
		Part3	:DarkBlastHitTendrilx.part
		part	:Smokes.part
		part	:Smoke2s.part
		Part	:LiteDarkBlastHitTendril.part
		Sound rage 100 100 1.0
	
	End



	Event
		EName	6
		Type	Local
		At	ULEGR
		part1	:Smokex.part
		part2	:Smoke2x.part
		
		Part3	:DarkBlastHitTendrilx.part
		part	:Smokes.part
		part	:Smoke2s.part
		Part	:LiteDarkBlastHitTendril.part
		#PMagnet LlegR

	End


#LegR


	Event
		EName	7
		Type	Local
		At	LLEGR
		part1	:Smokex.part
		part2	:Smoke2x.part
		
		Part3	:DarkBlastHitTendrilx.part
		part	:Smokes.part
		part	:Smoke2s.part
		Part	:LiteDarkBlastHitTendril.part
		#PMagnet FootR
	End

		
###############################################################################

	Event
		EName	9
		Type	Local
		At	Head
		part1	:Smokex.part
		part2	:Smoke2x.part
		
		Part3	:DarkBlastHitTendrilx.part
		part	:Smokes.part
		part	:Smoke2s.part
		Part	:LiteDarkBlastHitTendril.part
	End

		
##########################################################################
##Arm
##################################################################

	Event
		EName	10
		Type	Local
		At	UArmL
		part1	:Smokex.part
		part2	:Smoke2x.part
		
		Part3	:DarkBlastHitTendrilx.part
		part	:Smokes.part
		part	:Smoke2s.part
		Part	:LiteDarkBlastHitTendril.part
		#PMagnet LArmL

	End

	Event
		EName	11
		Type	Local
		At	LArmL
		part1	:Smokex.part
		part2	:Smoke2x.part
		
		Part3	:DarkBlastHitTendrilx.part
		part	:Smokes.part
		part	:Smoke2s.part
		Part	:LiteDarkBlastHitTendril.part
		#PMagnet WepL
	End

	Event
		EName	13
		Type	Local
		At	UArmR
		part1	:Smokex.part
		part2	:Smoke2x.part
		
		Part3	:DarkBlastHitTendrilx.part
		part	:Smokes.part
		part	:Smoke2s.part
		Part	:LiteDarkBlastHitTendril.part
		#PMagnet LArmR

	End

	Event
		EName	14
		Type	Local
		At	LArmR
		part1	:Smokex.part
		part2	:Smoke2x.part
		
		Part5	:DarkBlastHitTendrilx.part
		part	:Smokes.part
		part	:Smoke2s.part
		Part	:LiteDarkBlastHitTendril.part
		#PMagnet WepR
	End

End     

End	

