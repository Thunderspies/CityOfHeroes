#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	120

##################################


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
		Part	:DarkBlastHitTendril_Subtractive2.part
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
		Part	:DarkBlastHitTendril_Subtractive2.part
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
		Part	:DarkBlastHitTendril_Subtractive2.part
		#PMagnet FootL
	End

	Event
		EName	5		
		Type	Local
		At	FootL
		part1	:Smoke.part
		part2	:Smoke2.part
		Part3	:DarkBlastHitTendril.part
		Part	:DarkBlastHitTendril_Subtractive2.part
		Sound rage 100 100 1.0
	
	End



	Event
		EName	6
		Type	Local
		At	ULEGR
		part1	:Smoke.part
		part2	:Smoke2.part
		Part3	:DarkBlastHitTendril.part
		Part	:DarkBlastHitTendril_Subtractive2.part
		#PMagnet LlegR

	End


#LegR


	Event
		EName	7
		Type	Local
		At	LLEGR
		part1	:Smoke.part
		part2	:Smoke2.part
		Part3	:DarkBlastHitTendril.part
		Part	:DarkBlastHitTendril_Subtractive2.part
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
		Part	:DarkBlastHitTendril_Subtractive2.part
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
		Part	:DarkBlastHitTendril_Subtractive2.part
		#PMagnet LArmL

	End

	Event
		EName	11
		Type	Local
		At	LArmL
		part1	:Smoke.part
		part2	:Smoke2.part
		Part3	:DarkBlastHitTendril.part
		Part	:DarkBlastHitTendril_Subtractive2.part
		#PMagnet WepL
	End

	Event
		EName	13
		Type	Local
		At	UArmR
		part1	:Smoke.part
		part2	:Smoke2.part
		Part3	:DarkBlastHitTendril.part
		Part	:DarkBlastHitTendril_Subtractive2.part
		#PMagnet LArmR

	End

	Event
		EName	14
		Type	Local
		At	LArmR
		part1	:Smoke.part
		part2	:Smoke2.part
		Part3	:DarkBlastHitTendril.part
		Part	:DarkBlastHitTendril_Subtractive2.part
		#PMagnet WepR
	End

	Event
		EName	RINGS
		Type	Local
		At	Hips
			
		part1	:RegenerationRing.part
	End
End    

#Condition
#	On	cycle
#	Time	15
#		
#
#	Event	
#
#		ENAME	BaseTrail
#		Type	start
#		At	T_Root
#		
#		BHVR	:DarkSplatShrink2.bhvr
#
#		Splat	generic_Ring.tga
#		LifeSpan	10
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
			
		Part1	:DarkTarRingsIn.Part
	
		LifeSpan	110
	End

End


End