#########################################################
##	iceweapon
########################################################
FxInfo

Input  
	Inpname	Target
End

LifeSpan	100

Condition
	On 	Time
	Time 	0

	Event
		EName 	Mallet
		Type	Local 
		At	Chest
	End

End

Condition
	On 	Time
	Time 	0
	Repeat	20

	Event
		EName	debris
		Type	StartPositOnly
		At	Mallet
		geom	GEO_Slag_Golem_Junk_01
		geom	GEO_Slag_Golem_Junk_02
		geom	GEO_Slag_Golem_Junk_14
		geom	GEO_Slag_Golem_Junk_26
		geom	GEO_Slag_Golem_Junk_45
		bhvr	:GenerateDebris.bhvr
		Lifespan	150
		LifespanJitter	30
				
	End

End

Condition
	On 	Time
	Time 	0
	Repeat	20

	Event
		EName	debris
		Type	StartPositOnly
		At	Mallet
		geom	GEO_Slag_Golem_Junk_19
		geom	GEO_Slag_Golem_Junk_34
		geom	GEO_Slag_Golem_Junk_35
		geom	GEO_Slag_Golem_Junk_39
		geom	GEO_Slag_Golem_Junk_40
		geom	GEO_Slag_Golem_Junk_44
		bhvr	:GenerateDebris.bhvr
		Lifespan	150
		LifespanJitter	30
				
	End

End


End
