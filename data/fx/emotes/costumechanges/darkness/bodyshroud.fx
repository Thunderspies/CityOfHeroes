#########################################################
##	template

FxInfo

Lifespan 40
#########################################################
####################### AUDIO ###########################
#########################################################


Condition

	On 	Time
	Time	0

	Event
		Type Local
		At origin
#		Sound	eleccage_loop 70 30 .25
	End

End


#########################################################
#################### BODY GLOW 1 ########################
#########################################################



Condition
	On	Time
	Time	0


	Event
		Type	Local
		At	FootR
		part1	:Smoke.part
		Pother	LLegR
	End


	Event
		Type	Local
		At	FootL
		part1	:Smoke.part
		Pother	LLegL
	End

End

Condition
	On	Time
	Time	3

	Event
		Type	Local
		At	LlegR
		part	:Smoke.part
		part	:Smoke.part

		POther  UlegR
	End

	Event
		Type	Local
		At	LlegL
		part	:Smoke.part
		part	:Smoke.part

		POther  UlegL
	End
End

Condition
	On	Time
	Time	7

	Event
		ENAME	LeftULegGlow
		Type	Local
		At	UlegL
		part1	:Smoke.part
		POther  Hips
	End

	Event
		ENAME	RightULegGlow
		Type	Local
		At	UlegR
		part1	:Smoke.part
		POther  Hips
	End
End

Condition
	On	Time
	Time 	12
	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part1	:Smoke.part
		POther  Chest
	End
End

Condition
	On	Time
	Time	17

	Event
		ENAME	ChestGlow
		Type	Local
		At	Chest
		part1	:Smoke.part
		POther  Head
	End

End

Condition
	On	Time
	Time	23

	Event
		Type	Local
		At	UarmR
		part1	:Smoke.part
		Pother 	LArmR
	End


	Event
		Type	Local
		At	UarmL
		part1	:Smoke.part
		Pother 	LarmL
	End
End


Condition
	On	Time
	Time	25


	Event
		ENAME	RightUarmGlow
		Type	Local
		At	LarmR
		part1	:Smoke.part
		POther  WepR
	End

	Event
		ENAME	RightUarmGlow
		Type	Local
		At	LarmL
		part1	:Smoke.part
		POther  WepL
	End
End

Condition
	On	Time
	Time	27

	Event
		Type	Local
		At	Head
		part1	:Smoke.part
	End
End

Condition
	On	Time
	Time	29

	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part1	:Smoke.part
	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part1	:Smoke.part
	End
End