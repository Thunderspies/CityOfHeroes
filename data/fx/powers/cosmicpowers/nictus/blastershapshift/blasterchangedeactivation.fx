#########################################################
##	Inferno FX
##
FxInfo
	
lifespan	190
#################################################################

######################################################################
Condition

	On Time 
	Time 0
	
	Event
		Type	Local 
		At	hips
		Sound Nova4 100.0 100.0 .98
	End

End

Condition

	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local 
		At	hips
		
		Part2	:Inferno.part

	End

End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Primex
		Type	Local 
		At	hips
		
		Bhvr	Behaviors/CameraShake01.bhvr
		Part	:NovaTendrils.part
		Part	:InfernorockBits2.part
	End


End	

End
