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
		#Sound 	kinetic2 80 60 .85
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
		Part	:EgrColumnSparkles01.part
		Part	:EgrColumnClouds02.part
		Part	:EgrColumnStreak01.part
		Part	:EgrColumnStreak02.part
		Part	:EgrColumnClouds01.part
		Part	:EgrColumnRing01.part
		#Bhvr	:KineticScale.bhvr
		
	End
	
End

End