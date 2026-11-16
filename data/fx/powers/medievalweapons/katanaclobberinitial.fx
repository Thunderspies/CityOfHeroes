#########################################################
##	template

FxInfo

Input
	Inpname Origin
End

Input
	Inpname Mystic
End

Lifespan 150

Condition
	On 	Time
	Time 	20

	Event
		Type	local
		At	Origin
		Sound Whooshbig2 100.0 100.0 .7
	End

End

Condition
	On 	Time
	Time 	65

	Event
		EName	Streak
		Type	Start
		At	Mystic
		Geom	swoosh
		bhvr	behaviors/Medieval1.bhvr
		Sound Whoosh5 100.0 100.0 .6
		lifespan	1
	End

End	

Condition
	On 	Time
	Time 	68

	Event
		EName	top
		Type	local
		At	WepR
		
		bhvr	behaviors/Katana.bhvr
		Part	:KatanaTrail2.part
		
	End

	Event
		EName	Streak
		Type	local
		At	WepR
		
		Part	:KatanaGlow.part
		Lifespan	108

        End

	
	Event
		
		Type	local
		At	WepR
				
		Part	:KatanaTrail2.part
		Lifespan	110
	End

End
#########################################################
##Banner Particles
###############################################
#

Condition
	On 	Time
	Time 	10

	Event
		
		Type	local
		At	WepR
				
		Part	:KatanaGlowTrail.part
		Part	:KatanaGlowTrailEndsShort.part
		bhvr	behaviors/Katanabanner.bhvr
		Lifespan	65
	End

	Event
		
		Type	local
		At	WepR
		
		Part	:KatanaGlowTrailEndsShort.part
		Part	:KatanaGlowTrailEnds.part
		bhvr	behaviors/KatanabannerBottem.bhvr
		Lifespan	65
	End

	Event
		
		Type	local
		At	WepR
		Part	:KatanaGlowTrailEndsShort.part
		Part	:KatanaGlowTrailEnds.part
		bhvr	behaviors/Katanabannertop.bhvr
		Lifespan	65
	End

End

Condition
	On 	Time
	Time 	68

	
	Event
		
		Type	local
		At	WepR
		
		Part	:KatanaGlowTrailEndsShort.part		
		Part	:KatanaGlowTrailBright.part
		Part	:KatanaGlowTrailBright.part

		bhvr	behaviors/Katanabanner.bhvr
		Lifespan	6
	End

	Event
		
		Type	local
		At	WepR
		
		Part	:KatanaGlowTrailEndsBright.part	
		Part	:KatanaGlowTrailEndsBright.part
		bhvr	behaviors/KatanabannerBottem.bhvr
		Lifespan	6
	End


	Event
		
		Type	local
		At	WepR

		Part	:KatanaGlowTrailEndsBright.part		
		Part	:KatanaGlowTrailEndsBright.part
		bhvr	behaviors/Katanabannertop.bhvr
		Lifespan	6
	End

End
	
End	

	