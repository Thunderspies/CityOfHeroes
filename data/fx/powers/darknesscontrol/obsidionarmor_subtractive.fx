#########################################################
##	Fire_Ball
##
FxInfo
##################################

#Condition
#	On Cycle
#	Time 120
#	Event
#		EName 	Prime
#		#At	Origin
#		Type	SetLight
#		BHVR	GENERIC\LowGlow\Darkpulseclamp.bhvr
#		LifeSpan 120
#	End
#
#End


Condition
	On	Time	
	Time 	20

	Event
		Type 	Local
		At	FootR
		Sound miasma_loop 60 60 .6
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 440
	End

End

Condition
	On	Time
	Time	20
	
	Event
		EName	2
		Type	Local
		At	FootR
		part1	:LiteSmoke.part
		part2	:LiteSmoke2.part
		Part3	:LiteDarkBlastHitTendril.part
		

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
		Sound obsidian_loop 60 60 .42
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 280

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
	End

	Event
		EName	5		
		Type	Local
		At	FootL
		part1	:LiteSmoke.part
		part2	:LiteSmoke2.part
		Part3	:LiteDarkBlastHitTendril.part
		Part	:literDarkBlastHitTendril_Subtractive.part
		Sound rage 100 100 1.0
	
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
		Part	:InfuseRings.part
		Part	:HitRaysBig.part
		Part	:PsionicBubblesHit.part
		Part	:DarkPops.part
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


End	

