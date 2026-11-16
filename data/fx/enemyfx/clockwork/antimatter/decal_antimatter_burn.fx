#############################################################
## Decal_Antimatter_Burn.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time 	0

	Event
		Type	Local
		At	Root
		sound Antimatter_Ray_Hit 200 200 .8
	End

	Event
		Type	Local
		At	Root
		Sound Antimatter_Ray_Hit_Loop 200 200 .4
		bhvr	behaviors\sound\SoundIn05Out5.bhvr
		Lifespan 550
	End

	Event
		EName	Offset
		Type	Local
		At	T_Root
		BhvrOverride
			PositionOffset	0 .1 0
		End
		Part	:Decal_Antimatter_Shockwave_Additive.part
		LifeSpan	50
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

Condition
	On	Time
	Time	5

	Event
		Type	Start
		At	T_Root
		BhvrOverride
			PositionOffset		0 0.1 0
		End
		Part	:Decal_Antimatter_Scorch.part
		LifeSpan 15
	End

	Event
		Type	Start
		At	T_Root
		BhvrOverride
			PositionOffset		0 0.1 0
		End
		Part	:Decal_Antimatter_Swirl.part
		LifeSpan 160
	End

	Event
		Type	Start
		At	T_Root
		Part5	:Continuing_Antimatter_BubbleA.part
		Part5	:Continuing_Antimatter_BubbleB.part
		LifeSpan 550
	End
End

#############################################################

End
