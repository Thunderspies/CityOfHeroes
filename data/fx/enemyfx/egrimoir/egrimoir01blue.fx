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
		Part	:EgrColumnSparkles01Blue.part
		Part	:EgrColumnClouds02Blue.part
		Part	:EgrColumnStreak01Blue.part
		Part	:EgrColumnStreak02Blue.part
		Part	:EgrColumnClouds01Blue.part
		Part	:EgrColumnRing01Blue.part
		
	End
	
End

End