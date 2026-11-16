#############################################################
## Invisibility_Activation.fx
#############################################################

FxInfo
LifeSpan 60

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
		Sound Invisible1 80 80 .5
	End

	Event
		Type	Local
		At 	Origin
		Sound rumble1 80 80 .5
	End
End





#########################################################
#################### BODY GLOW 1 ########################
#########################################################

Condition
	On	Time
	Time	10

	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part	:InvisBodyGlow02.part
		POther LarmR
	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part	:InvisBodyGlow02.part
		POther LarmL
	End
End

Condition
	On	Time
	Time	20

	Event
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part	:InvisBodyGlow02.part
		POther LarmR
	End

	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part	:InvisBodyGlow02.part
		POther LarmL
	End
End

Condition
	On	Time
	Time	25


	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part	:InvisBodyGlow02.part
		POther Neck
	End

End

Condition
	On	Time
	Time	30

	Event
		ENAME	HeadGlow
		Type	Local
		At	Hair
		part	:InvisBodyGlow02.part
		POther Chest
	End
End

Condition
	On	Time
	Time	35

	Event
		ENAME	LeftULegGlow
		Type	Local
		At	LlegL
		part	:InvisBodyGlow02.part
		POther Ulegl
	End

	Event
		ENAME	RightULegGlow
		Type	Local
		At	LlegR
		part	:InvisBodyGlow02.part
		POther UlegR
	End
End

Condition
	On	Time
	Time	40

	Event
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		part	:InvisBodyGlow02.part
		POther Llegl
	End

	Event
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		part	:InvisBodyGlow02.part
		POther LlegR
	End
End

#############################################################

End