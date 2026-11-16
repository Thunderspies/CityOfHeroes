#########################################################
##	template

FxInfo


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
		part	:Glow.part
		part	:BodyFire.part
		part	:FlameAdditive.part
		Pmagnet Head
		Lifespan	70
		LifespanJitter	20
	End

	Event
		Type	Local
		At	UarmR
		part	:Glow.part
		part	:BodyFire.part
		part	:FlameAdditive.part
		Pmagnet UarmR
		Lifespan	70
		LifespanJitter	20
	End

	Event
		Type	Local
		At	FootR
		part	:Glow.part
		part	:BodyFire.part
		part	:FlameAdditive.part
		Pmagnet LLegR
		Lifespan	70
		LifespanJitter	20
	End

	Event
		Type	Local
		At	FootL
		part	:Glow.part
		part	:BodyFire.part
		part	:FlameAdditive.part
		Pmagnet LLegL
		Lifespan	70
		LifespanJitter	20
	End


	Event
		Type	Local
		At	UarmL
		part	:Glow.part
		part	:BodyFire.part
		part	:FlameAdditive.part
		Pmagnet UarmL
		Lifespan	70
		LifespanJitter	20
	End


	Event
		ENAME	ChestGlow
		Type	Local
		At	Chest
		part	:Glow.part
		part	:Glow3.part
		part	:BodyFire.part
		part	:FlameAdditive.part
		POther  Neck
		Lifespan	70
		LifespanJitter	20

	End

	Event
		ENAME	ChestGlow
		Type	Local
		At	Chest
		part	:Glow.part
		part	:BodyFire.part
		part	:FlameAdditive.part
		Pmagnet Hips
		POther  Hips
		Lifespan	70
		LifespanJitter	20
	End

	Event
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part	:Glow.part
		part	:BodyFire.part
		part	:FlameAdditive.part
		Pmagnet SpadL
		POther  SpadL
		Lifespan	70
		LifespanJitter	20
	End



	Event
		ENAME	ChestGlow2
		Type	Local
		At	Hips
		part	:Glow.part
		part	:BodyFire.part
		part	:FlameAdditive.part
		Pmagnet SpadR
		POther  SpadR
		Lifespan	70
		LifespanJitter	20
	End


	Event
		ENAME	HeadGlow
		Type	Local
		At	Head
		part	:Glow.part
		part	:BodyFire.part
		part	:FlameAdditive.part
		Pmagnet Chest
		POther  Chest
		Lifespan	70
		LifespanJitter	20
	End


	Event
		ENAME	RightUarmGlow
		Type	Local
		At	UarmR
		part	:Glow.part
		part	:BodyFire.part
		part	:FlameAdditive.part
		Pmagnet LarmR
		POther  LarmR
		Lifespan	70
		LifespanJitter	20
	End


	Event
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part	:Glow.part
		part	:BodyFire.part
		part	:FlameAdditive.part

		Pmagnet LarmR
		POther  LarmR
		Lifespan	90
		LifespanJitter	20


	End


	Event
		ENAME	LeftUarmGlow
		Type	Local
		At	UarmL
		part	:Glow.part
		part	:BodyFire.part
		part	:FlameAdditive.part

		Pmagnet LarmL
		POther  LarmL
		Lifespan	70
		LifespanJitter	20


	End


	Event
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part	:Glow.part
		part	:BodyFire.part
		part	:FlameAdditive.part

		Pmagnet LarmL
		POther  LarmL
		Lifespan	90
		LifespanJitter	20


	End


	Event
		ENAME	LeftULegGlow
		Type	Local
		At	UlegL
		part	:Glow.part
		part	:BodyFire.part
		part	:FlameAdditive.part

		Pmagnet Llegl
		POther  Llegl
		Lifespan	70
		LifespanJitter	20

	End


	Event
		ENAME	RightULegGlow
		Type	Local
		At	UlegR
		part	:Glow.part
		part	:BodyFire.part
		part	:FlameAdditive.part

		Pmagnet LlegR
		POther  LlegR
		Lifespan	70
		LifespanJitter	20

	End

	Event
		Type	Local
		At	LlegR
		part	:Glow.part
		part	:BodyFire.part
		part	:FlameAdditive.part

		Pmagnet FootR
		POther  FootR
		Lifespan	70
		LifespanJitter	20
	End

	Event
		Type	Local
		At	LlegL
		part	:Glow.part
		part	:BodyFire.part
		part	:FlameAdditive.part
		Pmagnet FootL
		POther  FootL
		Lifespan	70
		LifespanJitter	20
	End

#############################################################
######################## FIRE BASE ##########################
#############################################################

	Event
		Type	Local
		At	Head
		BhvrOverride
			PositionOffset 0.0 .5 0.0
		End
		part	:FlameBase.part
		Pmagnet Head
		Lifespan	60
		LifespanJitter	10
	End

	Event
		Type	Local
		At	UarmR
		part	:FlameBase.part

		Pmagnet UarmR
		Lifespan	60
		LifespanJitter	10

	End

	Event
		Type	Local
		At	FootR
		part	:FlameBase.part
		part	:FlameBase.part
		Pmagnet LLegR
		Lifespan	60
		LifespanJitter	10

	End

	Event
		Type	Local
		At	FootL
		part	:FlameBase.part
		part	:FlameBase.part
		Pmagnet LLegL
		Lifespan	60
		LifespanJitter	10

	End


	Event
		Type	Local
		At	UarmL
		part	:FlameBase.part
		Pmagnet UarmL
		Lifespan	60
		LifespanJitter	10

	End


	Event
		ENAME	ChestGlow2
		Type	Local
		At	Chest
		part	:FlameBase.part
		POther  Neck
		Lifespan	60
		LifespanJitter	10

	End



	Event
		ENAME	HipsNode2
		Type	Local
		At	Hips
		Lifespan	60
		LifespanJitter	10

	End

	Event
		ENAME	ChestGlow2
		Type	Local
		At	Chest
		part	:FlameBase.part
		Pmagnet Hips
		POther  Hips
		Lifespan	60
		LifespanJitter	10
	End

	Event
		ENAME	ChestGlow2
		Type	Local
		At	Hips
		part	:FlameBase.part
		Pmagnet SpadL
		POther  SpadL
		Lifespan	60
		LifespanJitter	10
	End



	Event
		ENAME	ChestGlow3
		Type	Local
		At	Hips
		part	:FlameBase.part
		Pmagnet SpadR
		POther  SpadR
		Lifespan	60
		LifespanJitter	10
	End


	Event
		ENAME	HeadGlow2
		Type	Local
		At	Head
		part	:FlameBase.part
		Pmagnet Chest
		POther  Chest
		Lifespan	60
		LifespanJitter	10
	End


	Event
		ENAME	RightUarmGlow2
		Type	Local
		At	UarmR
		part	:FlameBase.part
		Pmagnet LarmR
		POther  LarmR
		Lifespan	60
		LifespanJitter	10
	End


	Event
		ENAME	RightLarmGlow2
		Type	Local
		At	WepR
		part	:FlameBase.part
		part	:FlameBase.part
		Pmagnet LarmR
		POther  LarmR
		Lifespan	75
		LifespanJitter	10
	End


	Event
		ENAME	LeftUarmGlow2
		Type	Local
		At	UarmL
		part	:FlameBase.part
		Pmagnet LarmL
		POther  LarmL
		Lifespan	60
		LifespanJitter	10
	End


	Event
		ENAME	LeftLarmGlow2
		Type	Local
		At	WepL
		part	:FlameBase.part
		part	:FlameBase.part
		Pmagnet LarmL
		POther  LarmL
		Lifespan	75
		LifespanJitter	10
	End


	Event
		ENAME	LeftULegGlow2
		Type	Local
		At	UlegL
		part	:FlameBase.part
		Pmagnet Llegl
		POther  Llegl
		Lifespan	60
		LifespanJitter	10
	End


	Event
		ENAME	RightULegGlow2
		Type	Local
		At	UlegR
		part	:FlameBase.part
		Pmagnet LlegR
		POther  LlegR
		Lifespan	60
		LifespanJitter	10
	End

	Event
		Type	Local
		At	LlegR
		part	:FlameBase.part
		Pmagnet FootR
		POther  FootR
		Lifespan	60
		LifespanJitter	10
	End

	Event
		Type	Local
		At	LlegL
		part	:FlameBase.part
		Pmagnet FootL
		POther  FootL
		Lifespan	60
		LifespanJitter	10
	End

End


End