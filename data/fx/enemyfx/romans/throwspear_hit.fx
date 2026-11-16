#############################################################
## Header
#############################################################

FxInfo

Lifespan 60

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	ArrowHitHookup
		Type	create
		Inherit	Position
		Update	Position
		At	CHEST
		LookAt	Target
	End

	Event
		EName	ArrowHit
		Type	create
		Inherit	Position rotation
		Update	Position
		At	ArrowHitHookup
		Bhvr	:SpearHitFade.bhvr
		Geom	GEO_RomanPilum_Impaled01
		Lifespan 30
	End
End
#############################################################
Condition
	On	Time
	Time	0
	Random	1

	Event
		EName	Sound1
		Type	Start
		At	Chest
		Sound Pilum_impact_body 80 80 .65
	End

	Event
		EName	Sound2
		Type	Start
		At	Chest
		Sound Pilum_impact_body_2 80 80 .65
	End
		Event
		EName	Sound3
		Type	Start
		At	Chest
		Sound Pilum_impact_body_3 80 80 .65
	End

End
#############################################################

End