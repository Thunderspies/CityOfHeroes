#########################################################
##	template

FxInfo

Input
	InpName Origin
End

Input  
	InpName	Target
End


#Lifespan 200

#########################################################
####################### AUDIO ###########################
#########################################################


Condition
	On	Time
	Time	15
	

	Event
		Type	Local
		At 	Origin
		Sound kinetic2 80 80 .5
	End
End


Condition
	On	Time
	Time	15
	

	Event
		Type	Local
		At 	Origin
		Sound egrimoir2_loop 90 90 .66
	End
End


#########################################################
#################### BODY GLOW 1 ########################
#########################################################


Condition
	On	Time
	Time	0
	
	Event	
		ENAME	Column
		Type	Start
		At	Origin	
		Part	:EgrColumnSparkles01Purple.part
		Part	:EgrColumnClouds02Purple.part
		Part	:EgrColumnStreak01Purple.part
		Part	:EgrColumnStreak02Purple.part
		Part	:EgrColumnClouds01Purple.part
		Part	:EgrColumnClouds03Purple.part
		Part	:EgrColumnRing01Purple.part
		
	End
	
End

End