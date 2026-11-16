#############################################################
## FrozenFists_Attack.fx
#############################################################

FxInfo

Lifespan 35

#############################################################

Condition
	On 	Time
	Time 	0

		Event
		Type	Local
		At	LARMR
		Bhvr	CustomizeablePowers\IceManipulation\icehands.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	tintable_IceHandR
		Part2	CustomizeablePowers\IceManipulation\ColdArmMist2.part
		Sound Icepunch3 70 70 .7
	End

	Event
		Type	Local
		At	LARML
		Bhvr	CustomizeablePowers\IceManipulation\iceHands.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	tintable_IceHandL
		Part2	CustomizeablePowers\IceManipulation\ColdArmMist2.part
	End

	Event
		Type	Local
		At	WepL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\IceManipulation\Snowfall.part
		part3	CustomizeablePowers\IceManipulation\IceArmorHandBitsA.part
		LifeSpan 35
	End

	Event
		Type	Local
		At	WepR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part1	CustomizeablePowers\IceManipulation\Snowfall.part
		part3	CustomizeablePowers\IceManipulation\IceArmorHandBitsA.part
		LifeSpan 35
	End
End

#############################################################

End