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
		EName 	Fountain01
		Type	Start
		At	root
		Geom	Fountain01

	End

	Event
		Type	Start
		At	Fountain01
		AltPiv	1
		Part	:Top_Spray.part
		Part	:Top_Spray_Highlights.part
		Part	:Top_Spray_Highlights_Additive.part
		Part	:WaterFountain_Stem_Alpha.part
		Part	:WaterFountain_Stem_Droplets.part
		Part	:WaterFountain_Stem.part
	End

	Event
		Type	Start
		At	Fountain01
		AltPiv	1
		BhvrOVerride
			PositionOffset	0.0 2.0 0
		End
		Part	:Bottom_Splash.part
		Part	:Bottom_Splash_Additive.part

	End
	Event
		Type	Start
		At	Fountain01
		AltPiv	1
		BhvrOVerride
			PositionOffset	0.0 1.5 0
		End
		Part	:Bottom_Splash_Flat.part
		Part	:Bottom_Splash_Flat_Center.part

	End

End
End