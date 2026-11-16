
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
		Geom	GEO_BoostUp_GunClip
		Lifespan 35
	End
End

Condition
	On	Time
	Time	20

	Event
		EName	Glow
		Type	Local
		At	Root

		Childfx	V_COV\Powers\Paramilitary\Paramilitary_EquipGlow.fx
	End
End

End

