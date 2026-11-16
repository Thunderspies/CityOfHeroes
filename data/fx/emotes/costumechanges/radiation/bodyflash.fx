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
		At	Head
		BhvrOverride
			PositionOffset 0.0 .5 0.0
		End
		Part	:Flash.part
		Pmagnet Head
		Lifespan	50
		LifespanJitter	20
	End

	Event
		Type	Local
		At	UarmR
		Part	:Flash.part
		Pmagnet Neck
		Lifespan	50
		LifespanJitter	20
	End



	Event
		Type	Local
		At	UarmL
		Part	:Flash.part
		Pmagnet Neck
		Lifespan	50
		LifespanJitter	20
	End


	Event
		ENAME	ChestGlow
		Type	Local
		At	Chest
		Part	:Flash.part
		Pmagnet Hips
		POther  Hips
		Lifespan	50
		LifespanJitter	20
	End

	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		Part	:Flash.part
		Pmagnet UarmL
		POther  UarmL
		Lifespan	50
		LifespanJitter	20
	End



	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		Part	:Flash.part
		Pmagnet LarmR
		POther  LarmR
		Lifespan	50
		LifespanJitter	20
	End


	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		Part	:Flash.part
		Pmagnet LarmL
		POther  LarmL
		Lifespan	50
		LifespanJitter	20
	End


	Event
		ENAME	LeftULegGlow
		Type	Local
		At	UlegL
		Part	:Flash.part
		Pmagnet Llegl
		POther  Llegl
		Lifespan	50
		LifespanJitter	20
	End


	Event
		ENAME	RightULegGlow
		Type	Local
		At	UlegR
		Part	:Flash.part
		Pmagnet LlegR
		POther  LlegR
		Lifespan	50
		LifespanJitter	20
	End

	Event
		Type	Local
		At	LlegR
		Part	:Flash.part
		Pmagnet FootR
		POther  FootR
		Lifespan	50
		LifespanJitter	20
	End

	Event
		Type	Local
		At	LlegL
		Part	:Flash.part
		Pmagnet FootL
		POther  FootL
		Lifespan	50
		LifespanJitter	20
	End

	Event
		Type	Local
		At	FootR
		Part	:Flash.part
		Pmagnet LLegR
		Lifespan	50
		LifespanJitter	20
	End

	Event
		Type	Local
		At	FootL
		Part	:Flash.part
		Pmagnet LLegL
		Lifespan	50
		LifespanJitter	20
	End


End



End