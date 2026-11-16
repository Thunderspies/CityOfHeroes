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
		Type	local
		At	Origin
		Sound lavasteam3_loop 40 40 .29
	End

End

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
		part	:InfernalBodyStreaks02.part
		POther Chest		
	End
	

	Event	
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		POther LarmR	
	End

	
	Event	
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part	:InfernalBodyStreaks02.part
		POther LarmR
		
	End


	Event	
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		POther LarmL
	End
	
	Event	
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part	:InfernalBodyStreaks02.part
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
		part	:InfernalBodyStreaks02.part
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
		part	:InfernalBodyStreaks02.part
		POther LlegR

	End	

End

End				