#############################################################
## Stealth_Continuing.fx
#############################################################

FxInfo

#############################################################

Input
	InpName	Hips
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
		Sound Glow3_loop 80 80 .2
		bhvr	behaviors/Sound/SoundOut5.bhvr
		Lifespan 300
	End
End


#########################################################
#################### BODY GLOW 1 ########################
#########################################################



Condition
	On	Time
	Time	30


	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part	:InvisStealthBodyGlow01.part
		part	:InvisStealthBodyGlow02.part
		POther Neck
	End

	Event
		ENAME	HeadGlow
		Type	Local
		At	Hair
		part	:InvisStealthBodyGlow01.part
		part	:InvisStealthBodyGlow02.part
		POther Chest
	End


	Event
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part	:InvisStealthBodyGlow01.part
		part	:InvisStealthBodyGlow02.part
		POther LarmR
	End


	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part	:InvisStealthBodyGlow01.part
		part	:InvisStealthBodyGlow02.part
		POther LarmR
	End


	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part	:InvisStealthBodyGlow01.part
		part	:InvisStealthBodyGlow02.part
		POther LarmL

	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part	:InvisStealthBodyGlow01.part
		part	:InvisStealthBodyGlow02.part
		POther LarmL
	End


	Event
		ENAME	LeftULegGlow
		Type	Local
		At	LlegL
		part	:InvisStealthBodyGlow01.part
		part	:InvisStealthBodyGlow02.part
		POther Ulegl
	End

	Event
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		part	:InvisStealthBodyGlow01.part
		part	:InvisStealthBodyGlow02.part
		POther Llegl
	End

	Event
		ENAME	RightULegGlow
		Type	Local
		At	LlegR
		part	:InvisStealthBodyGlow01.part
		part	:InvisStealthBodyGlow02.part
		POther UlegR
	End

	Event
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		part	:InvisStealthBodyGlow01.part
		part	:InvisStealthBodyGlow02.part
		POther LlegR
	End


End

#############################################################

End