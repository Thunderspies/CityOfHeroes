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
		part	:FlameAdditive_Huge.part
		Pmagnet Head
		Lifespan	70
		LifespanJitter	20
	End

	Event
		Type	Local
		At	UarmR
		part	:Glow.part
		part	:BodyFire.part
		part	:FlameAdditive_Huge.part

		Pmagnet UarmR
		Lifespan	70
		LifespanJitter	20
	End

	Event
		Type	Local
		At	FootR
		part	:Glow.part
		part	:BodyFire.part
		part	:FlameAdditive_Huge.part
		Pmagnet LLegR
		Lifespan	70
		LifespanJitter	20
	End

	Event
		Type	Local
		At	FootL
		part	:Glow.part
		part	:BodyFire.part
		part	:FlameAdditive_Huge.part
		Pmagnet LLegL
		Lifespan	70
		LifespanJitter	20
	End


	Event
		Type	Local
		At	UarmL
		part	:Glow.part
		part	:BodyFire.part
		part	:FlameAdditive_Huge.part
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
		part	:FlameAdditive_Huge.part
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
		part	:FlameAdditive_Huge.part
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
		part	:FlameAdditive_Huge.part
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
		part	:FlameAdditive_Huge.part
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
		part	:FlameAdditive_Huge.part
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
		part	:FlameAdditive_Huge.part
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
		part	:FlameAdditive_Huge.part

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
		part	:FlameAdditive_Huge.part

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
		part	:FlameAdditive_Huge.part

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
		part	:FlameAdditive_Huge.part

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
		part	:FlameAdditive_Huge.part

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
		part	:FlameAdditive_Huge.part

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
		part	:FlameAdditive_Huge.part
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
		part	:FlameBase_Huge.part
		Pmagnet Head
		Lifespan	60
		LifespanJitter	10
	End

	Event
		Type	Local
		At	UarmR
		part	:FlameBase_Huge.part

		Pmagnet UarmR
		Pother	Neck
		Lifespan	60
		LifespanJitter	10

	End

	Event
		Type	Local
		At	FootR
		part	:FlameBase_Huge.part
		part	:FlameBase_Huge.part
		Pmagnet LLegR
		Lifespan	60
		LifespanJitter	10

	End

	Event
		Type	Local
		At	FootL
		part	:FlameBase_Huge.part
		part	:FlameBase_Huge.part
		Pmagnet LLegL
		Lifespan	60
		LifespanJitter	10

	End


	Event
		Type	Local
		At	UarmL
		part	:FlameBase_Huge.part
		Pmagnet UarmL
		Pother 	Neck
		Lifespan	60
		LifespanJitter	10

	End


	Event
		ENAME	ChestGlow2
		Type	Local
		At	Chest
		part	:FlameBase_Huge.part
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
		part	:FlameBase_Huge.part
		Pmagnet Hips
		POther  Hips
		Lifespan	60
		LifespanJitter	10
	End

	Event
		ENAME	ChestGlow2
		Type	Local
		At	Hips
		part	:FlameBase_Huge.part
		Pmagnet SpadL
		POther  SpadL
		Lifespan	60
		LifespanJitter	10
	End



	Event
		ENAME	ChestGlow3
		Type	Local
		At	Hips
		part	:FlameBase_Huge.part
		Pmagnet SpadR
		POther  SpadR
		Lifespan	60
		LifespanJitter	10
	End


	Event
		ENAME	HeadGlow2
		Type	Local
		At	Head
		part	:FlameBase_Huge.part
		Pmagnet Chest
		POther  Chest
		Lifespan	60
		LifespanJitter	10
	End


	Event
		ENAME	RightUarmGlow2
		Type	Local
		At	UarmR
		part	:FlameBase_Huge.part
		Pmagnet LarmR
		POther  LarmR
		Lifespan	60
		LifespanJitter	10
	End


	Event
		ENAME	RightLarmGlow2
		Type	Local
		At	WepR
		part	:FlameBase_Huge.part
		part	:FlameBase_Huge.part
		Pmagnet LarmR
		POther  LarmR
		Lifespan	75
		LifespanJitter	10
	End


	Event
		ENAME	LeftUarmGlow2
		Type	Local
		At	UarmL
		part	:FlameBase_Huge.part
		Pmagnet LarmL
		POther  LarmL
		Lifespan	60
		LifespanJitter	10
	End


	Event
		ENAME	LeftLarmGlow2
		Type	Local
		At	WepL
		part	:FlameBase_Huge.part
		part	:FlameBase_Huge.part
		Pmagnet LarmL
		POther  LarmL
		Lifespan	75
		LifespanJitter	10
	End


	Event
		ENAME	LeftULegGlow2
		Type	Local
		At	UlegL
		part	:FlameBase_Huge.part
		Pmagnet Llegl
		POther  Llegl
		Lifespan	60
		LifespanJitter	10
	End


	Event
		ENAME	RightULegGlow2
		Type	Local
		At	UlegR
		part	:FlameBase_Huge.part
		Pmagnet LlegR
		POther  LlegR
		Lifespan	60
		LifespanJitter	10
	End

	Event
		Type	Local
		At	LlegR
		part	:FlameBase_Huge.part
		Pmagnet FootR
		POther  FootR
		Lifespan	60
		LifespanJitter	10
	End

	Event
		Type	Local
		At	LlegL
		part	:FlameBase_Huge.part
		Pmagnet FootL
		POther  FootL
		Lifespan	60
		LifespanJitter	10
	End

End


End