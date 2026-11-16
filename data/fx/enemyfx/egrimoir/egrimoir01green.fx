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
		Part	:EgrColumnSparkles01Green.part
		Part	:EgrColumnClouds02Green.part
		Part	:EgrColumnStreak01Green.part
		Part	:EgrColumnStreak02Green.part
		Part	:EgrColumnClouds01Green.part
		Part	:EgrColumnRing01Green.part
		
	End
	
End

End