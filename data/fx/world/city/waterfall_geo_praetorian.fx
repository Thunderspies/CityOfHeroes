#############################################################
## Fountain_Praetorian.fx
#############################################################

FxInfo

#############################################################

Input
	InpName	root
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Waterfall_Anchor
		Type	Local
		At	Root
		GEOM	FX_WaterFall_Offsets
		BhvrOverride
			Alpha	1
		End
	End

	Event
		EName	Splash_Anchor
		Type	Local
		At	Waterfall_Anchor
		AltPiv	2
	End

	Event
		Type	Local
		At	Root
		BhvrOVerride
			PositionOffset	28 -22.0 0
			Scale		1.5 1.1 0
			Alpha		255
			StartColor	240 245 255
		End
		Geom	Praetoria_Waterfall_Wide
	End

	Event
		Type	Local
		At	Root
		BhvrOVerride
			PositionOFfset	28 -22.0 0
			Scale		1.5 1.1 0
			Alpha		200
			StartColor	240 245 255
		End
		Geom	Praetoria_Waterfall_Wide_BG

	End

	Event
		Type	Start
		At	Waterfall_Anchor
		AltPiv	1
		BhvrOVerride
			PositionOffset	0.0 0.0 0
		End
		Part	:Falling_Spray.part
		POther	Splash_Anchor
	End

End

#############################################################

End