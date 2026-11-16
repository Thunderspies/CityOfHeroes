#############################################################
## InvisibilityPhaseShiftContinuing.fx
#############################################################

FxInfo

#############################################################

Input
	Inpname	Hips
End

#############################################################



#########################################################
####################### AUDIO ###########################
#########################################################


Condition
	On	Time
	Time	0


	Event
		Type	Local
		At 	Origin
		Sound phase_loop 80 80 .44
	End
End


#########################################################
#################### BODY GLOW 1 ########################
#########################################################



Condition
	On	Time
	Time	15


	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part1	:InvisPhaseBodyGlow01.part
		part1	:InvisPhaseBodyGlow02.part
		POther Neck
		Lifespan 0

	End

	Event
		ENAME	HeadGlow
		Type	Local
		At	Hair
		part1	:InvisPhaseBodyGlow01.part
		part1	:InvisPhaseBodyGlow02.part
		POther Chest
		Lifespan 0

	End


	Event
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part1	:InvisPhaseBodyGlow01.part
		part1	:InvisPhaseBodyGlow02.part
		POther LarmR
		Lifespan 0

	End


	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part1	:InvisPhaseBodyGlow01.part
		part1	:InvisPhaseBodyGlow02.part
		POther LarmR
		Lifespan 0

	End


	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part1	:InvisPhaseBodyGlow01.part
		part1	:InvisPhaseBodyGlow02.part
		POther LarmL
		Lifespan 0

	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part1	:InvisPhaseBodyGlow01.part
		part1	:InvisPhaseBodyGlow02.part
		POther LarmL
		Lifespan 0

	End


	Event
		ENAME	LeftULegGlow
		Type	Local
		At	LlegL
		part1	:InvisPhaseBodyGlow01.part
		part1	:InvisPhaseBodyGlow02.part
		POther Ulegl
		Lifespan 0

	End

	Event
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		part1	:InvisPhaseBodyGlow01.part
		part1	:InvisPhaseBodyGlow02.part
		POther Llegl
		Lifespan 0

	End

	Event
		ENAME	RightULegGlow
		Type	Local
		At	LlegR
		part1	:InvisPhaseBodyGlow01.part
		part1	:InvisPhaseBodyGlow02.part
		POther UlegR
		Lifespan 0

	End

	Event
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		part1	:InvisPhaseBodyGlow01.part
		part1	:InvisPhaseBodyGlow02.part
		POther LlegR
		Lifespan 0

	End


End


End