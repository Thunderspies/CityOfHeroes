#############################################################
## BodyFrost.fx
#############################################################

FxInfo

Lifespan 30

#########################################################
####################### AUDIO ###########################
#########################################################

Condition
	On 	Time
	Time	5
	Event
		Type	Local
		At	Root
		Sound 	EndGame_IceBlock 100 100 .8
	End
End


#########################################################
#################### BODY GLOW 1 ########################
#########################################################




Condition
	On	Time
	Time	5

	Event
		ENAME	ChestGlow
		Type	Local
		At	Chest
		part	:Frost.part
		POther  Head
	End

End

Condition
	On	Time
	Time 	6
	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part	:Frost.part
		POther  Chest
	End
End

Condition
	On	Time
	Time	8

	Event
		ENAME	LeftULegGlow
		Type	Local
		At	UlegL
		part	:Frost.part
		POther  Hips
	End

	Event
		ENAME	RightULegGlow
		Type	Local
		At	UlegR
		part	:Frost.part
		POther  Hips
	End
End

Condition
	On	Time
	Time	10

	Event
		Type	Local
		At	LlegR
		part	:Frost.part
		POther  UlegR
	End

	Event
		Type	Local
		At	LlegL
		part	:Frost.part
		POther  UlegL
	End

	Event
		Type	Local
		At	LlegR
		part	:Frost.part
		POther  UlegR
	End

	Event
		Type	Local
		At	LlegL
		part	:Frost.part
		POther  UlegL
	End
End

Condition
	On	Time
	Time	8

	Event
		Type	Local
		At	UarmR
		part	:Frost.part
		Pother 	LArmR
	End


	Event
		Type	Local
		At	UarmL
		part	:Frost.part
		Pother 	LarmL
	End
End


Condition
	On	Time
	Time	10


	Event
		ENAME	RightUarmGlow
		Type	Local
		At	LarmR
		part	:Frost.part
		POther  WepR
	End

	Event
		ENAME	RightUarmGlow
		Type	Local
		At	LarmL
		part	:Frost.part
		POther  WepL
	End
End

Condition
	On	Time
	Time	8

	Event
		Type	Local
		At	Head
		part	:Frost.part
	End
End

Condition
	On	Time
	Time	12

	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part	:Frost.part
	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part	:Frost.part
	End
End


Condition
	On	Time
	Time	12
	Repeat	2

	Event
		Type	Local
		At	FootR
		part	:Frost.part
	End

	Event
		Type	Local
		At	FootL
		part	:Frost.part
	End

	Event
		Type	Local
		At	FootR
		part	:Frost.part
		Pother	LLegR
	End


	Event
		Type	Local
		At	FootL
		part	:Frost.part
		Pother	LLegL
	End

End

#########################################################

End