#############################################################
## Decal_Antimatter_Burn.fx
#############################################################

FxInfo
LifeSpan	50

#############################################################


Condition
	On	Time
	Time 	0

	Event
		EName	Offset
		Type	Startpositonly
		At	Root
		BhvrOverride
			PositionOffset	0 .1 0
		End
		Part	:Decal_Antimatter_Shockwave_Additive.part
		LifeSpan	20
	End
End

Condition
	On	Time
	Time 	2

	Event
		Type	Startpositonly
		At	Offset

		Part	:Hit_Rings_Subtractive.part
		Part	:Hit_Rings_Alpha.part
		Part	:Hit_Shockwave.part
		Part	:Hit_Flash_Large.part
		Part	:Decal_Antimatter_Shockwave.part
		Lifespan 5
	End
End

#############################################################

End
