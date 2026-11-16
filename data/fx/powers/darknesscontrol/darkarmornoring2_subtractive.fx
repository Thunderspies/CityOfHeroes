#########################################################
##	Fire_Ball
##
FxInfo

##################################
#
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
	Time	20
	
	Event
		EName	2
		Type	Local
		At	FootR
		part1	:LiteSmoke.part
		part2	:LiteSmoke2.part
		Part3	:LiteDarkBlastHitTendril.part
		Part	:literDarkBlastHitTendril_Subtractive.part
		Sound miasma_loop 60 60 .5
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 440
		Part    :sparklies.part
		Part    :sparkliesStar.part
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
		Part    :sparklies.part
		Part    :sparkliesStar.part
		Sound chime3_loop 60 60 .2
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
		Part    :sparklies.part
		Part    :sparkliesStar.part
	End

	Event
		EName	5		
		Type	Local
		At	FootL
		part1	:LiteSmoke.part
		part2	:LiteSmoke2.part
		Part3	:LiteDarkBlastHitTendril.part
		Part	:literDarkBlastHitTendril_Subtractive.part
		Part    :sparklies.part
		Part    :sparkliesStar.part
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
		Part    :sparklies.part
		Part    :sparkliesStar.part

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
		Part    :sparklies.part
		Part    :sparkliesStar.part
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
		Part    :sparklies.part
		Part    :sparkliesStar.part
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
		Part    :sparklies.part
		Part    :sparkliesStar.part

	End

	Event
		EName	11
		Type	Local
		At	LArmL
		part1	:LiterSmoke.part
		part2	:LiterSmoke2.part
		Part3	:LiterDarkBlastHitTendril.part
		Part	:literDarkBlastHitTendril_Subtractive.part
		Part    :sparklies.part
		Part    :sparkliesStar.part
	End

	Event
		EName	13
		Type	Local
		At	UArmR
		part1	:LiterSmoke.part
		part2	:LiterSmoke2.part
		Part3	:LiterDarkBlastHitTendril.part
		Part	:literDarkBlastHitTendril_Subtractive.part
		Part    :sparklies.part
		Part    :sparkliesStar.part

	End

	Event
		EName	14
		Type	Local
		At	LArmR
		part1	:LiterSmoke.part
		part2	:LiterSmoke2.part
		Part3	:LiterDarkBlastHitTendril.part
		Part	:literDarkBlastHitTendril_Subtractive.part
		Part    :sparklies.part
		Part    :sparkliesStar.part
	End

End    

################################################################################
##Lens Flares
###########################################################

Condition
	On 	cycle
	Time 	20
	Chance	.3
	
	Event
		EName	3
		Type	Local
		At	LarmL
		part1	Powers/Empathy/FortitudeRaysSmall.part
		part2	Powers/Empathy/FortitudeRing.part
		part3	Powers/Empathy/FortitudeRing.part
		part4	Powers/Empathy/FortitudeDots.part

		lifespan	25
	End
	
End

Condition
	On 	cycle
	Time 	19
	Chance	.3
	
	Event
		EName	3
		Type	Local
		At	LarmR
		part1	Powers/Empathy/FortitudeRaysSmall.part
		part2	Powers/Empathy/FortitudeRing.part
		part3	Powers/Empathy/FortitudeRing.part
		part4	Powers/Empathy/FortitudeDots.part

		lifespan	25
	End
	
End

Condition
	On 	cycle
	Time 	21
	Chance	.3
	
	Event
		EName	3
		Type	Local
		At	UarmL
		part1	Powers/Empathy/FortitudeRaysSmall.part
		part2	Powers/Empathy/FortitudeRing.part
		part3	Powers/Empathy/FortitudeRing.part
		part4	Powers/Empathy/FortitudeDots.part

		lifespan	25
	End
	
End


Condition
	On 	cycle
	Time 	22
	Chance	.3
	
	Event
		EName	3
		Type	Local
		At	UarmR
		part1	Powers/Empathy/FortitudeRaysSmall.part
		part2	Powers/Empathy/FortitudeRing.part
		part3	Powers/Empathy/FortitudeRing.part
		part4	Powers/Empathy/FortitudeDots.part

		lifespan	25
	End
	

End

Condition
	On 	cycle
	Time 	17
	Chance	.3

	Event
		EName	3
		Type	Local
		At	LlegL
		part1	Powers/Empathy/FortitudeRaysSmall.part
		part2	Powers/Empathy/FortitudeRing.part
		part3	Powers/Empathy/FortitudeRing.part
		part4	Powers/Empathy/FortitudeDots.part

		lifespan	25
	End

End

Condition
	On 	cycle
	Time 	18
	Chance	.3

	Event
		EName	3
		Type	Local
		At	LlegR
		part1	Powers/Empathy/FortitudeRaysSmall.part
		part2	Powers/Empathy/FortitudeRing.part
		part3	Powers/Empathy/FortitudeRing.part
		part4	Powers/Empathy/FortitudeDots.part

		lifespan	25
	End


End

#######################################

End	

