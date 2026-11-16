#########################################################
##	Inferno FX
##
FxInfo
	
lifespan	90
#################################################################

######################################################################
Condition

	On Time 
	Time 10
	
	Event
		Type	Local 
		At	hips
		Sound Nova4 100.0 100.0 .98
	End


End

Condition
	On 	Time
	Time 	30

	Event
		
		Type	Local 
		At	hips
		
		Part	:NovaTendrilsBuildUp.part
		
	End

End

Condition
	On 	Time
	Time 	36

	Event
		
		Type	Local 
		At	hips
		
		#Bhvr	Behaviors/CameraShake01.bhvr
		Part	:NovaTendrils.part
		Part	:InfernorockBits2.part
		Part2	:Inferno.part
	End


End	

Condition
	On 	Time
	Time 	55

	Event
		Type	Local 
		At	hips
		
		Bhvr	Behaviors/CameraShake01.bhvr
	End


End

End
