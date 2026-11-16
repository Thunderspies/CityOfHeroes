#########################################################
##	template

FxInfo

Lifespan 80
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
		Part	:DarkBlastHitTendril.part
		Pother	LLegR
		Lifespan	65
		LifespanJitter	15
	End


	Event
		Type	Local
		At	FootL
		Part	:DarkBlastHitTendril.part
		Pother	LLegL
		Lifespan	65
		LifespanJitter	15
	End

End

Condition
	On	Time
	Time	3

	Event
		Type	Local
		At	LlegR
		Part	:DarkBlastHitTendril.part
		POther  UlegR
		Lifespan	65
		LifespanJitter	15
	End

	Event
		Type	Local
		At	LlegL
		Part	:DarkBlastHitTendril.part
		POther  UlegL
		Lifespan	65
		LifespanJitter	15
	End
End

Condition
	On	Time
	Time	7

	Event
		ENAME	LeftULegGlow
		Type	Local
		At	UlegL
		Part	:DarkBlastHitTendril.part
		POther  Hips
		Lifespan	65
		LifespanJitter	15
	End

	Event
		ENAME	RightULegGlow
		Type	Local
		At	UlegR
		Part	:DarkBlastHitTendril.part
		POther  Hips
		Lifespan	65
		LifespanJitter	15
	End
End

Condition
	On	Time
	Time 	12
	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		Part	:DarkBlastHitTendril.part
		POther  Chest
		Lifespan	65
		LifespanJitter	15
	End
End

Condition
	On	Time
	Time	17

	Event
		ENAME	ChestGlow
		Type	Local
		At	Chest
		Part	:DarkBlastHitTendril.part
		POther  Head
		Lifespan	65
		LifespanJitter	15
	End

End

Condition
	On	Time
	Time	23

	Event
		Type	Local
		At	UarmR
		Part	:DarkBlastHitTendril.part
		Pother 	LArmR
		Lifespan	65
		LifespanJitter	15
	End


	Event
		Type	Local
		At	UarmL
		Part	:DarkBlastHitTendril.part
		Pother 	LarmL
		Lifespan	65
		LifespanJitter	15
	End
End


Condition
	On	Time
	Time	25


	Event
		ENAME	RightUarmGlow
		Type	Local
		At	LarmR
		Part	:DarkBlastHitTendril.part
		POther  WepR
		Lifespan	65
		LifespanJitter	15
	End

	Event
		ENAME	RightUarmGlow
		Type	Local
		At	LarmL
		Part	:DarkBlastHitTendril.part
		POther  WepL
		Lifespan	65
		LifespanJitter	15
	End
End

Condition
	On	Time
	Time	27

	Event
		Type	Local
		At	Head
		Part	:DarkBlastHitTendril.part
		Lifespan	65
		LifespanJitter	15
	End
End

Condition
	On	Time
	Time	29

	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		Part	:DarkBlastHitTendril.part
		Lifespan	65
		LifespanJitter	15
	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		Part	:DarkBlastHitTendril.part
		Lifespan	65
		LifespanJitter	15
	End
End