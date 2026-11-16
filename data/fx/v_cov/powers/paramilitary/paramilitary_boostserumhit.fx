
FxInfo

Flags	InheritAnimScale

Lifespan 125

Condition
	On	Time
	Time	12

	Event
		EName	NodeSpin
		Type	Local
		At	WepL

		bhvr	V_COV\Powers\Paramilitary\Paramilitary_B_BoostSerumPos.bhvr
		Geom	GEO_BoostSerum
	End
End

Condition
	On	Time
	Time	60
	
	Event
		EName	Glow
		Type	Local
		At	Root

		ChildFX	V_COV\Powers\Paramilitary\Paramilitary_BoostGlow.fx
	End
End

End

