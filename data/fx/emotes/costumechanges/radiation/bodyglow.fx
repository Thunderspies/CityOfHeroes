#########################################################
##	template

FxInfo

Lifespan 90
#########################################################
####################### AUDIO ###########################
#########################################################


Condition

	On 	Time
	Time	0

	Event
		Type Local
		At origin
	#	Sound	eleccage_loop 70 30 .25
	End

End


#########################################################
#################### BODY GLOW 1 ########################
#########################################################



Condition
	On	Time
	Time	0


	Event
		ENAME	HeadGlow
		Type	Local
		At	Head
		part	:Glow.part
		part	:Bubble_A.part
		part	:Bubble_B.part
		Pmagnet Chest
		POther  Chest
		Lifespan 	70
		LifespanJitter	20

	End

	Event
		Type	Local
		At	UarmR
		part	:Glow.part
		part	:Bubble_A.part
		part	:Bubble_B.part
		Pmagnet UarmR
		Lifespan 	70
		LifespanJitter	20
	End


	Event
		Type	Local
		At	UarmL
		part	:Glow.part
		part	:Bubble_A.part
		part	:Bubble_B.part
		Pmagnet UarmL
		Lifespan 	70
		LifespanJitter	20
	End

	Event
		ENAME	ChestGlow
		Type	Local
		At	Chest
		part	:Glow.part
		part	:Bubble_A.part
		part	:Bubble_B.part
		Pmagnet Neck
		POther  Neck
		Lifespan 	70
		LifespanJitter	20
	End

	Event
		ENAME	RightUarmGlow
		Type	Local
		At	UarmR
		part	:Glow.part
		part	:Bubble_A.part
		part	:Bubble_B.part
		Pmagnet LarmR
		POther  LarmR
		Lifespan 	70
		LifespanJitter	20

	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	LArmL
		part	:Glow.part
		part	:Bubble_A.part
		part	:Bubble_B.part
		Pmagnet UarmL
		POther  UarmL
		Lifespan 	70
		LifespanJitter	20
	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	LArmR
		part	:Glow.part
		part	:Bubble_A.part
		part	:Bubble_B.part
		Pmagnet UarmL
		POther  UarmL
		Lifespan 	70
		LifespanJitter	20
	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part	:Glow.part
		part	:Bubble_A.part
		part	:Bubble_B.part
		Pmagnet LarmL
		POther  LarmL
		Lifespan 	70
		LifespanJitter	20
	End

	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepR
		part	:Glow.part
		part	:Bubble_A.part
		part	:Bubble_B.part
		Pmagnet RarmL
		POther  RarmL
		Lifespan 	70
		LifespanJitter	20
	End


	Event
		ENAME	LeftULegGlow
		Type	Local
		At	UlegL
		part	:Glow.part
		part	:Bubble_A.part
		part	:Bubble_B.part
		Pmagnet Llegl
		POther  Llegl
		Lifespan 	70
		LifespanJitter	20

	End

	Event
		ENAME	RightULegGlow
		Type	Local
		At	UlegR
		part	:Glow.part
		part	:Bubble_A.part
		part	:Bubble_B.part
		Pmagnet LlegR
		POther  LlegR
		Lifespan 	70
		LifespanJitter	20
	End


	Event
		Type	Local
		At	LlegR
		part	:Glow.part
		part	:Bubble_A.part
		part	:Bubble_B.part
		Pmagnet FootR
		POther  FootR
		Lifespan 	70
		LifespanJitter	20
	End

	Event
		Type	Local
		At	LlegL
		part	:Glow.part
		part	:Bubble_A.part
		part	:Bubble_B.part
		Pmagnet FootL
		POther  FootL
		Lifespan 	70
		LifespanJitter	20
	End

	Event
		Type	Local
		At	FootL
		part	:Glow.part
		part	:Bubble_A.part
		part	:Bubble_A.part
		part	:Bubble_B.part
		Lifespan 	70
		LifespanJitter	20
	End

	Event
		Type	Local
		At	FootR
		part	:Glow.part
		part	:Bubble_A.part
		part	:Bubble_A.part
		part	:Bubble_B.part
		Lifespan 	70
		LifespanJitter	20
	End

End



End