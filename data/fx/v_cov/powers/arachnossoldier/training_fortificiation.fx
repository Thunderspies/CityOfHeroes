#########################################################
##  Unyielding - Continuing FX (Villain)
#########################################################

FxInfo

#########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Hips
		Sound Training_Fortification_01 60 60 .9
	End
End

## CAGE EFFECT #######################################################

Condition
	On	Time
	Time	20

	Event
		EName 	Hookup
		Type	Local
		At	root
		Bhvr	behaviors\Invulnerability_Spin.bhvr
		Geom	FX_RuneStreak
	End

	Event
		Type	Local
		At	Hookup
		AltPiv  1
		Part	V_COV\Powers\ArachnosSoldier\Fortification\Fortification_VerticalStreaks1.part
	End

	Event
		Type	Local
		At	Hookup
		AltPiv  2
		Part	V_COV\Powers\ArachnosSoldier\Fortification\Fortification_VerticalStreaks1.part
	End
End

## BODY GLOW #######################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Hair
		part1	V_COV\Powers\ArachnosSoldier\Fortification\Fortification_Glows2.part
		POther	Head
	End

	Event
		Type	Local
		At	UArmL
		part1	V_COV\Powers\ArachnosSoldier\Fortification\Fortification_Glows2.part
		POther	LArmL
	End

	Event
		Type	Local
		At	UArmR
		part1	V_COV\Powers\ArachnosSoldier\Fortification\Fortification_Glows2.part
		POther	LArmR
	End

	Event
		Type	Local
		At	LArmL
		part1	V_COV\Powers\ArachnosSoldier\Fortification\Fortification_Glows1.part
		POther	HandL
	End

	Event
		Type	Local
		At	LArmR
		part1	V_COV\Powers\ArachnosSoldier\Fortification\Fortification_Glows1.part
		POther	HandR
	End

	Event
		Type	Local
		At	ULegL
		part1	V_COV\Powers\ArachnosSoldier\Fortification\Fortification_Glows2.part
		POther	ULegR
	End

	Event
		Type	Local
		At	ULegL
		part1	V_COV\Powers\ArachnosSoldier\Fortification\Fortification_Glows2.part
		POther	LLegL
	End

	Event
		Type	Local
		At	ULegR
		part1	V_COV\Powers\ArachnosSoldier\Fortification\Fortification_Glows2.part
		POther	LLegR
	End

	Event
		Type	Local
		At	LLegL
		part1	V_COV\Powers\ArachnosSoldier\Fortification\Fortification_Glows2.part
		POther	FootL
	End

	Event
		Type	Local
		At	LLegR
		part1	V_COV\Powers\ArachnosSoldier\Fortification\Fortification_Glows2.part
		POther	FootR
	End

	Event
		Type	Local
		At	ToeL
		part1	V_COV\Powers\ArachnosSoldier\Fortification\Fortification_Glows1.part
		POther	FootL
	End

	Event
		Type	Local
		At	ToeR
		part1	V_COV\Powers\ArachnosSoldier\Fortification\Fortification_Glows1.part
		POther	FootR
	End
End

#########################################################

End