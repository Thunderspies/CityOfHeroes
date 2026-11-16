FxInfo

LifeSpan 190

Condition
	On Time
	Time 1

	Event
		EName 	ManHole
		Type	Start
		At	Mystic 
		
		bhvr	V_COV\Powers\Thugs\Thugs_B_Fadein.bhvr
		GEOM	GEO_Manhole
		Sound manhole2 60 60 .45
	End

	Event
		EName 	ManHoleCover
		Type	Local
		At	Mystic 
		
		bhvr	V_COV\Powers\Thugs\Thugs_B_EntFadein.bhvr
		GEOM	GEO_ManHoleCover
		Lifespan 109
	End
End

Condition
	On	Time
	Time	110

	Event
		EName	ManHoleCoverStay
		Type	Start
		At	ManHoleCover

		bhvr	V_COV\Powers\Thugs\Thugs_B_EntFadeOut.bhvr
		GEOM	GEO_ManHoleCover
End			