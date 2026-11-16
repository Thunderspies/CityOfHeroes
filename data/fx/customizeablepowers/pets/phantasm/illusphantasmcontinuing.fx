#########################################################
##	template

FxInfo

#########################################################
####################### AUDIO ###########################
#########################################################


Condition
	On	Time
	Time	0


	Event
		Type	Local
		At 	Origin
		Sound Glow6_loop 80 80 .45
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
		part	:IllusPhantBodyGlow01.part
		part	:IllusPhantBodyRays01.part
		part	:IllusPhantRisingGlows01.part
		POther Neck
		Lifespan 0

	End

	Event
		ENAME	HeadGlow
		Type	Local
		At	Hair
		part	:IllusPhantBodyGlow01.part
		part	:IllusPhantBodyRays01.part
		part	:IllusPhantRisingGlows01.part
		POther Chest
		Lifespan 0

	End


	Event
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part	:IllusPhantBodyGlow01.part
		part	:IllusPhantRisingGlows01.part
		POther LarmR
		Lifespan 0

	End


	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part	:IllusPhantBodyGlow01.part
		part	:IllusPhantBodyRays01.part
		part	:IllusPhantRisingGlows01.part
		POther LarmR
		Lifespan 0

	End


	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part	:IllusPhantBodyGlow01.part
		part	:IllusPhantRisingGlows01.part
		POther LarmL
		Lifespan 0

	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part	:IllusPhantBodyGlow01.part
		part	:IllusPhantBodyRays01.part
		part	:IllusPhantRisingGlows01.part
		POther LarmL
		Lifespan 0

	End


	Event
		ENAME	LeftULegGlow
		Type	Local
		At	LlegL
		part	:IllusPhantBodyGlow01.part
		part	:IllusPhantBodyRays01.part
		part	:IllusPhantRisingGlows01.part
		POther Ulegl
		Lifespan 0

	End

	Event
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		part	:IllusPhantBodyGlow01.part
		part	:IllusPhantBodyRays01.part
		part	:IllusPhantRisingGlows01.part
		POther Llegl
		Lifespan 0

	End

	Event
		ENAME	RightULegGlow
		Type	Local
		At	LlegR
		part	:IllusPhantBodyGlow01.part
		part	:IllusPhantBodyRays01.part
		part	:IllusPhantRisingGlows01.part
		POther UlegR
		Lifespan 0

	End

	Event
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		part	:IllusPhantBodyGlow01.part
		part	:IllusPhantBodyRays01.part
		part	:IllusPhantRisingGlows01.part
		POther LlegR
		Lifespan 0

	End

	Event
		ENAME	HandLGlow
		Type	Local
		At	WepL
		part	:IllusPhantBodyRays01.part
		Lifespan 0

	End

	Event
		ENAME	HandRGlow
		Type	Local
		At	WepR
		part	:IllusPhantBodyRays01.part
		Lifespan 0

	End

	Event
		ENAME	FootRGlow
		Type	Local
		At	FootR
		part	:IllusPhantBodyRays01.part
		Lifespan 0

	End

	Event
		ENAME	FootLGlow
		Type	Local
		At	FootL
		part	:IllusPhantBodyRays01.part
		Lifespan 0

	End

	Event
		ENAME	UarmRGlow
		Type	Local
		At	UarmR
		part	:IllusPhantBodyRays01.part
		Lifespan 0

	End

	Event
		ENAME	UarmLGlow
		Type	Local
		At	UarmL
		part	:IllusPhantBodyRays01.part
		Lifespan 0

	End

End


End