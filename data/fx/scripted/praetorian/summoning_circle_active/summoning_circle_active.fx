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
			PositionOffset	0.0 0.5 0.0
			Scale		1.0 1.0 1.0
			Alpha		1
		End
		Geom	FX_Offsets_SummoningProp
	End

	Event
		Type	Local
		At	Root
		BhvrOverride
			Alpha		50
			PositionOffset	0.0 -1.5 0.0
		End
		Part	:Streaks_Central.part
	End

	Event
		Type	Local
		At	Hookup
		AltPiv	1
		Part	:Streaks_Outer.part

	End

	Event
		Type	Local
		At	Hookup
		AltPiv	2
		Part	:Streaks_Outer.part
	End

	Event
		Type	Local
		At	Hookup
		AltPiv	3
		Part	:Streaks_Outer.part
	End

	Event
		Type	Local
		At	Hookup
		AltPiv	1
		Part	:Rune1.part
	End

	Event
		Type	Local
		At	Hookup
		AltPiv	2
		BhvrOverride
			PYRRotate	0 120 0
		End
		Part	:Rune2.part
	End

	Event
		Type	Local
		At	Hookup
		AltPiv	3
		BhvrOverride
			PYRRotate	0 250 0
		End
		Part	:Rune3.part
	End

End


#############################################################

End