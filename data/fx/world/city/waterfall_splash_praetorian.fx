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

			PositionOffset	0.0 1.5 0.0
			PyrRotate				0 210 0
		End
		Part	:Bottom_Mist_Large.part
		Part	:Bottom_Spray.part
		Part	:Bottom_Splash_Large.part
		Part	:Bottom_Splash_Large_Additive.part
		Part	:Bottom_Splash_Flat_Large.part
		Part	:Bottom_Foam_Flat.part
	#	POther	Splash_Anchor
	End


End

#############################################################

End