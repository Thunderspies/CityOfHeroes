#############################################################
## SonicPylons_Hit.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	Anchor1
		Type	Local
		At	WepL
		BhvrOverride
			PositionOffset	0.0 0.5 -2.5
		End
	End

	Event
		EName	Anchor2
		Type	Local
		At	WepL
		BhvrOverride
			PositionOffset	0.0 0.5 3.5
		End
	End

	Event
		EName	Anchor1B
		Type	Local
		At	WepL
		BhvrOverride
			PositionOffset	0.0 -0.5 -2.5
		End
	End

	Event
		EName	Anchor2B
		Type	Local
		At	WepL
		BhvrOverride
			PositionOffset	0.0 -0.5 3.5
		End
	End

	Event
		EName	RAnchor1
		Type	Local
		At	WepR
		BhvrOverride
			PositionOffset	0.0 0.5 -2.5
		End
	End

	Event
		EName	RAnchor2
		Type	Local
		At	WepR
		BhvrOverride
			PositionOffset	0.0 0.5 3.5
		End
	End

	Event
		EName	RAnchor1B
		Type	Local
		At	WepR
		BhvrOverride
			PositionOffset	0.0 -0.5 -2.5
		End
	End

	Event
		EName	RAnchor2B
		Type	Local
		At	WepR
		BhvrOverride
			PositionOffset	0.0 -0.5 3.5
		End
	End



	Event
		Type	Local
		At	Anchor1
		Part	:BeamGlow01.part
		Part	:BeamGlow02.part
		POther	Anchor2
		Lifespan 1
	End

	Event
		Type	Local
		At	Anchor1B
		Part	:BeamGlow01.part
		Part	:BeamGlow02.part
		POther	Anchor2B
		Lifespan 1
	End

	Event
		Type	Local
		At	RAnchor1
		Part	:BeamGlow01.part
		Part	:BeamGlow02.part
		POther	RAnchor2
		Lifespan 1
	End

	Event
		Type	Local
		At	RAnchor1B
		Part	:BeamGlow01.part
		Part	:BeamGlow02.part
		POther	RAnchor2B
		Lifespan 1
	End
End

Condition
	On	Time
	Time	2

	Event
		Type	Local
		At	WepL
		BhvrOverride
			PositionOffset	0.0 0.0 -5.5
		End
		Part	:SparkGlow01.part
		Part	:SparkGlow02.part
		Lifespan 1
	End

	Event
		Type	Local
		At	WepR
		BhvrOverride
			PositionOffset	0.0 0.0 -5.5
		End
		Part	:SparkGlow01.part
		Part	:SparkGlow02.part

		Lifespan 1

	End
End

#############################################################

End