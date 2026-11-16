#############################################################
## BodyGlow.fx
#############################################################

FxInfo
Lifespan 100

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
		part	:Glow2.part
		part	:BodyArcsLarge01.part
		part	:BodyArcsSmall01.part

		Pmagnet LLegR
		Lifespan 	30
		LifespanJitter 	10
	End

	Event
		Type	Local
		At	FootL
		part	:Glow2.part
		part	:BodyArcsLarge01.part
		part	:BodyArcsSmall01.part

		Pmagnet LLegL
		Lifespan 	30
		LifespanJitter 	10
	End

	Event
		Type	Local
		At	LlegR
		part	:Glow2.part
		part	:BodyArcsLarge01.part
		part	:BodyArcsSmall01.part

		Pmagnet FootR
		POther  FootR
		Lifespan 	40
		LifespanJitter 	10
	End

	Event
		Type	Local
		At	LlegL
		part	:Glow2.part
		part	:BodyArcsLarge01.part
		part	:BodyArcsSmall01.part

		Pmagnet FootL
		POther  FootL
		Lifespan 	40
		LifespanJitter 	10
	End

	Event
		ENAME	LeftULegGlow
		Type	Local
		At	UlegL
		part	:Glow2.part
		part	:BodyArcsLarge01.part
		part	:BodyArcsSmall01.part

		Pmagnet Llegl
		POther  Llegl
		Lifespan 	50
		LifespanJitter 	10
	End


	Event
		ENAME	RightULegGlow
		Type	Local
		At	UlegR
		part	:Glow2.part
		part	:BodyArcsLarge01.part
		part	:BodyArcsSmall01.part

		Pmagnet LlegR
		POther  LlegR
		Lifespan 	50
		LifespanJitter 	10
	End



	Event
		ENAME	HipsGlow
		Type	Local
		At	Hips
		part	:Glow2.part
		part	:BodyArcsLarge01.part
		part	:BodyArcsSmall01.part

		Pmagnet SpadL
		POther  SpadL
		Lifespan 	60
		LifespanJitter 	10
	End

	Event
		ENAME	ChestGlow2
		Type	Local
		At	Hips
		part	:Glow2.part
		part	:BodyArcsLarge01.part
		part	:BodyArcsSmall01.part

		Pmagnet SpadR
		POther  SpadR
		Lifespan 	60
		LifespanJitter 	10
	End

	Event
		ENAME	ChestGlow
		Type	Local
		At	Chest
		part	:Glow2.part
		part	:BodyArcsLarge01.part
		part	:BodyArcsSmall01.part

		POther  Neck
		Lifespan 	70
		LifespanJitter 	10
	End


	Event
		ENAME	ChestGlow
		Type	Local
		At	Chest
		part	:Glow2.part

		Pmagnet Hips
		POther  Hips
		Lifespan 	70
		LifespanJitter 	10
	End

	Event
		ENAME	RightUarmGlow
		Type	Local
		At	UarmR
		part	:Glow2.part
		part	:BodyArcsLarge01.part
		part	:BodyArcsSmall01.part

		Pmagnet LarmR
		POther  LarmR
		Lifespan 	75
		LifespanJitter 	10
	End

	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	UarmL
		part	:Glow2.part
		part	:BodyArcsLarge01.part
		part	:BodyArcsSmall01.part

		Pmagnet LarmL
		POther  LarmL
		Lifespan 	75
		LifespanJitter 	10
	End

	Event
		ENAME	HeadGlow
		Type	Local
		At	Head
		part	:Glow2.part
		part	:BodyArcsLarge01.part
		part	:BodyArcsSmall01.part

		Pmagnet Chest
		POther  Chest
		Lifespan 	80
		LifespanJitter 	10
	End

	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part	:Glow2.part
		part	:BodyArcsLarge01.part
		part	:BodyArcsSmall01.part

		Pmagnet LarmR
		POther  LarmR
		Lifespan 	80
		LifespanJitter 	10
	End





	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part	:Glow2.part
		part	:BodyArcsLarge01.part
		part	:BodyArcsSmall01.part

		Pmagnet LarmL
		POther  LarmL
		Lifespan 	80
		LifespanJitter 	10
	End




End



End