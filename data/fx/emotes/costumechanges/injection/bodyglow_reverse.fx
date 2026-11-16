#############################################################
## BodyGlow.fx
#############################################################

FxInfo
Lifespan 200

#########################################################
#################### BODY GLOW 1 ########################
#########################################################



Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	UArmR
		Part	:Glow_Linger.part
		Pmagnet LArmR
		POther	LArmR
		Lifespan	100
		LifespanJitter	20
	End

	Event
		Type	Local
		At	LArmR
		Part	:Glow_Linger.part
		Pmagnet WepR
		POther  WepR
		Lifespan	100
		LifespanJitter	20
	End
End

Condition
	On	Time
	Time	20

	Event
		Type	Local
		At	Head
		BhvrOverride
			PositionOffset 0.0 .5 0.0
		End
		Part	:Glow_Linger.part
		Pmagnet Chest
		POther	Chest
		Lifespan	100
		LifespanJitter	20
	End
End

Condition
	On	Time
	Time	40

	Event
		Type	Local
		At	UArmL
		Part	:Glow_Linger.part
		Pmagnet LArmL
		POther	LArmL
		Lifespan	100
		LifespanJitter	20
	End


	Event
		Type	Local
		At	LArmL
		Part	:Glow_Linger.part
		Pmagnet WepL
		POther  WepL
		Lifespan	100
		LifespanJitter	20
	End
End

Condition
	On	Time
	Time	60

	Event
		Type	Local
		At	Hips
		Part	:Glow_Linger.part
		Pmagnet LLegR
		POther  LLegR
		Lifespan	100
		LifespanJitter	20
	End

	Event
		Type	Local
		At	LLegR
		Part	:Glow_Linger.part
		Pmagnet FootR
		POther  FootR
		Lifespan	100
		LifespanJitter	20
	End
End

Condition
	On	Time
	Time	70

	Event
		Type	Local
		At	Hips
		Part	:Glow_Linger.part
		Pmagnet LLegL
		POther  LLegL
		Lifespan	100
		LifespanJitter	20
	End

	Event
		Type	Local
		At	LLegL
		Part	:Glow_Linger.part
		Pmagnet FootL
		POther  FootL
		Lifespan	100
		LifespanJitter	20
	End


End

#########################################################

End