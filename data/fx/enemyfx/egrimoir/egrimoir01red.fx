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
		Part	:EgrColumnSparkles01Red.part
		Part	:EgrColumnClouds02Red.part
		Part	:EgrColumnStreak01Red.part
		Part	:EgrColumnStreak02Red.part
		Part	:EgrColumnClouds01Red.part
		Part	:EgrColumnRing01Red.part
		
	End
	
End

End