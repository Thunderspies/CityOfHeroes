#############################################################
## BodyAura_NoTrail.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Chest
		part	:BodySmoke01.part
		part	:BodySmokeAdd.part
		part	:FireEmbersBody01.part
		Pmagnet Neck
		POther  Neck
		Lifespan 	70
		LifespanJitter	20
	End

	Event
		Type	Local
		At	Chest
		part	:BodySmoke01.part
		part	:BodySmokeAdd.part
		Pmagnet Hips
		POther  Hips
		Lifespan 	70
		LifespanJitter	20
	End

	Event
		Type	Local
		At	Hair
		part	:BodySmoke01.part
		part	:BodySmokeAdd.part
		part	:FireEmbers01.part
		Pmagnet Chest
		POther  Chest
		Lifespan 	70
		LifespanJitter	20
	End
End

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Hips
		part	:BodySmoke01.part
		part	:BodySmokeAdd.part
		Pmagnet UArmL
		POther  UArmL
		Lifespan 	70
		LifespanJitter	20
	End

	Event
		Type	Local
		At	Hips
		part	:BodySmoke01.part
		part	:BodySmokeAdd.part
		part	:FireEmbersBody01.part
		Pmagnet UarmR
		POther  UarmR
		Lifespan 	70
		LifespanJitter	20
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	UarmL
		part	:BodySmoke01.part
		part	:BodySmokeAdd.part
		part	:FireEmbers01.part
		Pmagnet UarmL
		Lifespan 	70
		LifespanJitter	20
	End

	Event
		Type	Local
		At	UarmR
		part	:BodySmoke01.part
		part	:BodySmokeAdd.part
		part	:FireEmbers01.part
		Pmagnet UarmR
		Lifespan 	70
		LifespanJitter	20
	End

	Event
		Type	Local
		At	UarmR
		part	:BodySmoke01.part
		part	:BodySmokeAdd.part
		Pmagnet LarmR
		POther  LarmR
		Lifespan 	70
		LifespanJitter	20
	End

	Event
		Type	Local
		At	WepR
		part	:BodySmoke01.part
		part	:BodySmokeAdd.part
		part	:FireEmbers01.part
		Pmagnet LarmR
		POther  LarmR
		Lifespan 	70
		LifespanJitter	20
	End
End

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	UarmL
		part	:BodySmoke01.part
		part	:BodySmokeAdd.part
		Pmagnet LarmL
		POther  LarmL
		Lifespan 	70
		LifespanJitter	20
	End

	Event
		Type	Local
		At	WepL
		part	:BodySmoke01.part
		part	:BodySmokeAdd.part
		part	:FireEmbers01.part
		Pmagnet LarmL
		POther  LarmL
		Lifespan 	70
		LifespanJitter	20

	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	UlegL
		part	:BodySmoke01.part
		part	:BodySmokeAdd.part
		Pmagnet LLegL
		POther  LLegL
		Lifespan 	70
		LifespanJitter	20
	End

	Event
		Type	Local
		At	UlegR
		part	:BodySmoke01.part
		part	:BodySmokeAdd.part
		Pmagnet LlegR
		POther  LlegR
		Lifespan 	70
		LifespanJitter	20
	End
End

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	LlegR
		part	:BodySmoke01.part
		part	:BodySmokeAdd.part
		Pmagnet FootR
		POther  FootR
		Lifespan 	70
		LifespanJitter	20
	End

	Event
		Type	Local
		At	LlegL
		part	:BodySmoke01.part
		part	:BodySmokeAdd.part
		Pmagnet FootL
		POther  FootL
		Lifespan 	70
		LifespanJitter	20
	End

	Event
		Type	Local
		At	FootR
		part	:BodySmoke01.part
		part	:BodySmokeAdd.part
		part	:FireEmbers01.part
		Pmagnet FootR
		Lifespan 	70
		LifespanJitter	20
	End

	Event
		Type	Local
		At	FootL
		part	:BodySmoke01.part
		part	:BodySmokeAdd.part
		part	:FireEmbers01.part
		Pmagnet FootL
		Lifespan 	70
		LifespanJitter	20
	End
End

#############################################################

End