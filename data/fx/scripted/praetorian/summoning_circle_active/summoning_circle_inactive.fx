#############################################################
## SequestrationField_Token3.fx
#############################################################

FxInfo
Flags DontSuppress

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName 	Hookup
		Type	PositOnly
		At	Root
		BhvrOverride
			Alpha		1
			PositionOffset	0.0 0.0 0.0
			Scale		1.0 1.0 1.0
		End
		Geom	FX_Offsets_SummoningProp
	End



	Event
		Type	Local
		At	Hookup
		BhvrOverride
			PositionOffset	0.0 5.0 0.0
		End
		Part	:Body_SmokeAura.part
	End


End


#############################################################

End