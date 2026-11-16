#########################################################
##	
##

FxInfo







#########################################################
#################### BODY GLOW 1 ########################
#########################################################



Condition
	On	Time
	Time	0
	

	Event	
		ENAME	ChestGlow
		Type	Local
		At	Hips
		POther Neck	
	End

	Event	
		ENAME	HeadGlow
		Type	Local
		At	Head
		part	:ShivanBodyStreaks02.part
		POther Chest		
	End
	

	Event	
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		POther LarmR	
	End

	Event	
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		POther LarmL
	End
	
	Event	
		ENAME	LeftULegGlow
		Type	Local
		At	LlegL
		POther Ulegl		
	End

	Event	
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		part	:ShivanBodyStreaks02.part
		POther Llegl
	End

	Event	
		ENAME	RightULegGlow
		Type	Local
		At	LlegR
		POther UlegR	
	End

	Event	
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		part	:ShivanBodyStreaks02.part
		POther LlegR

	End	

End

End				