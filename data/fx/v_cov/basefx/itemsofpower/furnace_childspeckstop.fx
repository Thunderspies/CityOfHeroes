FxInfo

Condition
	On	Time
	Time	0

	Event
		EName	EmitterStaticC
		Type	Local
		At	Origin
	End
End

Condition
	On	Time
	Time	1

	Event
		EName	SpecsBot01
		Type	Local
		At	EmitterStaticC
		bhvrOverride
			behavior
			PositionOffset		-6.261 -5.953 0
		End
#		Geom	Testing_TargetB
	End

	Event
		EName	SpecsTop01
		Type	Local
		At	EmitterStaticC
		bhvrOverride
			behavior
			PositionOffset		-6.261 5.953 0
		End
#		Geom	Testing_TargetB
	End

	Event
		EName	Specs01
		Type	Local
		At	SpecsBot01
		
		Part1	V_COV\BaseFx\ItemsOfPower\True_Furnace_P_SpeckTop.part
		PMagnet	EmitterStaticC
		POther	SpecsTop01
	End

End

End