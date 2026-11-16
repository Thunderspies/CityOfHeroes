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
		Type	Start
		At	Root

		Part	:Top_Spray_Large.part
		Part	:Top_Spray_Highlights_Large.part
		Part	:Top_Spray_Highlights_Additive_Large.part
		Part	:WaterFountain_Stem_Alpha_Large.part
		Part	:WaterFountain_Stem_Droplets_Large.part
		Part	:WaterFountain_Stem_Large.part
	End

	Event
		Type	Start
		At	Root
		BhvrOVerride
			PositionOffset	0.0 0.75 0
		End
		Part	:Bottom_Splash_Large_Fountain.part
		Part	:Bottom_Splash_Additive_Large.part
		Part	:Bottom_Spray_Additive_Large.part
		Part	:Bottom_Splash_Flat_Large_Fountain.part
		Part	:Bottom_Splash_Flat_Center.part

	End


End
End