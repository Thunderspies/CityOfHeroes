#########################################################
##	template

FxInfo

Input
	Inpname Mystic
End

Input
	Inpname Origin
End


Lifespan 90


Condition
	On 	Time
	Time 	20

	Event
		Type	local
		At	Origin
		Sound clobberwhoosh 100.0 100.0 .78
	End

End

Condition
	On 	Time
	Time 	35

	Event
		EName	Streak
		Type	local
		At	Mystic
		Geom	swoosh
		bhvr	behaviors/Medieval1.bhvr
		Lifespan	2
	End

End

Condition
	On 	Time
	Time 	50

	Event
		EName	topPoint
		Type	local
		At	WepR
		

		bhvr	behaviors/Katana.bhvr
		Part	:KatanaTrail2.part
		
	End

	Event
		
		Type	local
		At	WepR
				
		Part	:KatanaTrail2.part
		
	End

End

Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	WepR
		
		Part	:KatanaGlow.part
		Lifespan	83
	End

#########################################################
##Banner Particles
###############################################

	Event
		
		Type	local
		At	WepR
				
		Part	:KatanaGlowTrail.part
		Part	:KatanaGlowTrailEndsShort.part
		bhvr	behaviors/Katanabanner.bhvr

		Lifespan	36	#85
	End

	Event
		
		Type	local
		At	WepR
		Part	:KatanaGlowTrailEndsShort.part		
		Part	:KatanaGlowTrailEnds.part
		bhvr	behaviors/KatanabannerBottem.bhvr
		Lifespan	36	#85
	End


	Event
		
		Type	local
		At	WepR
		Part	:KatanaGlowTrailEndsShort.part		
		Part	:KatanaGlowTrailEnds.part
		bhvr	behaviors/Katanabannertop.bhvr
		Lifespan	36	#85
	End
	
	Event
		
		Type	local
		At	WepR
		Part	:KatanaGlowTrailEndsShort.part		
		Part	:KatanaGlowTrailEnds.part
		bhvr	behaviors/Katanabannertop.bhvr
		Lifespan	36
	End

End

#######################################################################3
##Glow
######################################################3

Condition
	On 	Time
	Time 	33

	Event
		
		Type	local
		At	WepR
				
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

	