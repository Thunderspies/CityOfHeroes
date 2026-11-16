#############################################################
## IllusDecoyContinuing.fx
#############################################################

FxInfo

#########################################################

Input
	Inpname	Hips
End

#########################################################


#########################################################
####################### AUDIO ###########################
#########################################################


Condition
	On	Time
	Time	0


	Event
		Type	Local
		At 	Origin
		Sound Invisible1 80 80 .46
	End
End




Condition
	On	Time
	Time	10


	Event
		Type	Local
		At 	Origin
		Sound Glow3_loop 80 80 .26
	End
End





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
		part	:IllusDecoyChestGlow01.part
		part	:IllusDecoyBodyGlow02.part
		part	:IllusDecoyBodyNoise01.part
		POther Neck
		Lifespan 0
	End

	Event
		ENAME	HeadGlow
		Type	Local
		At	Hair
		part	:IllusDecoyBodyGlow01.part
		part	:IllusDecoyBodyGlow02.part
		POther Chest
		Lifespan 0

	End


	Event
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part	:IllusDecoyBodyGlow01.part
		part	:IllusDecoyBodyGlow02.part
		part	:IllusDecoyBodyNoise01.part
		POther LarmR
		Lifespan 0

	End


	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part	:IllusDecoyBodyGlow01.part
		part	:IllusDecoyBodyGlow02.part
		part	:IllusDecoyBodyNoise01.part
		POther LarmR
		Lifespan 0

	End


	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part	:IllusDecoyBodyGlow01.part
		part	:IllusDecoyBodyGlow02.part
		part	:IllusDecoyBodyNoise01.part
		POther LarmL
		Lifespan 0

	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part	:IllusDecoyBodyGlow01.part
		part	:IllusDecoyBodyGlow02.part
		part	:IllusDecoyBodyNoise01.part
		POther LarmL
		Lifespan 0

	End


	Event
		ENAME	LeftULegGlow
		Type	Local
		At	LlegL
		part	:IllusDecoyBodyGlow01.part
		part	:IllusDecoyBodyGlow02.part
		part	:IllusDecoyBodyNoise01.part
		POther Ulegl
		Lifespan 0

	End

	Event
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		part	:IllusDecoyBodyGlow01.part
		part	:IllusDecoyBodyGlow02.part
		part	:IllusDecoyBodyNoise01.part
		POther Llegl
		Lifespan 0

	End

	Event
		ENAME	RightULegGlow
		Type	Local
		At	LlegR
		part	:IllusDecoyBodyGlow01.part
		part	:IllusDecoyBodyGlow02.part
		part	:IllusDecoyBodyNoise01.part
		POther UlegR
		Lifespan 0

	End

	Event
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		part	:IllusDecoyBodyGlow01.part
		part	:IllusDecoyBodyGlow02.part
		part	:IllusDecoyBodyNoise01.part
		POther LlegR
		Lifespan 0

	End


End


End