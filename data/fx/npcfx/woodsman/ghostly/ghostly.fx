#########################################################
##	template

FxInfo

#########################################################
####################### AUDIO ###########################
#########################################################

Condition
	On	Time
	Time 	8
	
	Event
		Type	Local
		At 	Origin
		Sound invisible1 100 100 .6
	End
End



#########################################################
#################### BODY GLOW 1 ########################
#########################################################



Condition 
	Event	
		ENAME	ChestGlow
		Type	Local
		At	Hips
		Flags	AdoptParentEntity
		bhvr	:ghostlyWoodsman.bhvr

	End

	Event	
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part	:IllusPhantBodyGlow01.part
		POther Neck
	End

	Event	
		ENAME	HeadGlow
		Type	Local
		At	Hair
		part	:IllusPhantBodyGlow01.part
		POther Chest
	End
	

	Event	
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part	:IllusPhantBodyGlow01.part
		POther LarmR
	End

	
	Event	
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part	:IllusPhantBodyGlow01.part
		POther LarmR
	End


	Event	
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part	:IllusPhantBodyGlow01.part
		POther LarmL
		Lifespan 0
		
	End
	
	Event	
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part	:IllusPhantBodyGlow01.part
		POther LarmL
		Lifespan 0
		
	End


	Event	
		ENAME	LeftULegGlow
		Type	Local
		At	LlegL
		part	:IllusPhantBodyGlow01.part
		POther Ulegl
	End

	Event	
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		part	:IllusPhantBodyGlow01.part
		POther Llegl	
	End

	Event	
		ENAME	RightULegGlow
		Type	Local
		At	LlegR
		part	:IllusPhantBodyGlow01.part
		POther UlegR		
	End

	Event	
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		part	:IllusPhantBodyGlow01.part
		POther LlegR
	End
End


End		