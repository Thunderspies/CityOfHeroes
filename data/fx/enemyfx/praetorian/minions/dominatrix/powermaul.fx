#########################################################
##	forceBubble
########################################################
FxInfo



########################################################



Condition
	On 	Time
	Time 	4

	Event
		EName 	Gun
		Type	Local 
		At	WepR
		Geom	GEO_WepR_PowerMaul
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		#Sound	powermaulout3 80 30 .5
	End
	
	Event
		EName 	MaulHead1
		Type	local 
		At	WepR
		Bhvr	:MaulHeadOffset1.bhvr
		Sound thorneyes_loop 35 35 .3
	End

	Event
		EName 	MaulHead2
		Type	local 
		At	WepR
		Bhvr	:MaulHeadOffset2.bhvr
		Sound powermaul_loop 35 35 .2
	End
End

Condition
	On 	Time
	Time 	20
	
	Event
		EName 	MaulHeadFX1
		Type	local 
		At	MaulHead1
		Part	:MaulGlow.Part
		Part	:MaulPop2.Part
		Part	:MaulPop1.Part
		Part	:MaulGlow1.Part
		Part	:MaulGlow2.Part
		POther  MaulHead2	
	End	

End



Condition
	On 	Time
	Time 	10

	Event
		EName 	MaulHeadFX2
		Type	Posit
		At	MaulHead2
		Part	:MaulRays1.Part
		Part	:MaulArcs1.Part
		POther MaulHead1	
		PMagnet MaulHead1	
	End
		
	
End

End

