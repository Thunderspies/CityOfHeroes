#############################################################
## IceMeleeFollowUpHit.fx
#############################################################

FxInfo

Lifespan 75

#############################################################

Condition
	On	Time
	Time	3

	Event
		Type	Local
		At	CHEST
		Bhvr	CustomizeablePowers\IceArmor\icearmor.bhvr
		Part	:bodychillbig.part
		Part	:SnowBurst.part
		geom	Tintable_FX_IceArmor_CHEST
		Lifespan 10
	End

	Event
		Type	Local
		At	Hips
		Bhvr	CustomizeablePowers\IceArmor\icearmor.bhvr
		Part	:bodychillbig.part
		Part	:SnowBurst.part
		geom	Tintable_FX_IceArmor_HIPS
		Lifespan 10
	End

	Event
		Type	Local
		At	UARML
		Bhvr	CustomizeablePowers\IceArmor\icearmor.bhvr
		Part	:bodychillbig.part
		Part	:SnowBurst.part
		geom	Tintable_FX_IceArmor_UARML
		Lifespan 10
	End

	Event
		Type	Local
		At	UARMR
		Bhvr	CustomizeablePowers\IceArmor\icearmor.bhvr
		Part	:bodychillbig.part
		Part	:SnowBurst.part
		geom	Tintable_FX_IceArmor_UARMR
		Lifespan 10
	End

	Event
		Type	Local
		At	LARMR
		Bhvr	CustomizeablePowers\IceArmor\icearmor.bhvr
		Part	:bodychillbig.part
		Part	:SnowBurst.part
		geom	Tintable_FX_IceArmor_LARMR
		Lifespan 10
	End

	Event
		Type	Local
		At	LARML
		Bhvr	CustomizeablePowers\IceArmor\icearmor.bhvr
		Part	:bodychillbig.part
		Part	:SnowBurst.part
		geom	Tintable_FX_IceArmor_LARML
		Lifespan 10
	End

	Event
		Type	Local
		At	WepL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Snowfall.part
		Lifespan 10
	End

	Event
		Type	Local
		At	WepR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Snowfall.part
		Lifespan 10
	End

	Event
		Type	Local
		At	UlegL
		Bhvr	CustomizeablePowers\IceArmor\icearmor.bhvr
		Part	:bodychillbig.part
		Part	:SnowBurst.part
		geom	Tintable_FX_IceArmor_ULEGL
		Lifespan 7
	End

	Event
		Type	Local
		At	UlegR
		Bhvr	CustomizeablePowers\IceArmor\icearmor.bhvr
		Part	:bodychillbig.part
		Part	:SnowBurst.part
		geom	Tintable_FX_IceArmor_ULEGR
		Lifespan 7
	End

	Event
		Type	Local
		At	LlegR
		Bhvr	CustomizeablePowers\IceArmor\icearmor.bhvr
		Part	:bodychillbig.part
		Part	:SnowBurst.part
		geom	Tintable_FX_IceArmor_LLEGR
		Lifespan 5
	End

	Event
		Type	Local
		At	LlegL
		Bhvr	CustomizeablePowers\IceArmor\icearmor.bhvr
		Part	:bodychillbig.part
		Part	:SnowBurst.part
		geom	Tintable_FX_IceArmor_LLEGL
		Lifespan 5
	End

	Event
		Type	Local
		At	CHEST
		Bhvr	:icearmor.bhvr
		Part	:bodychillbig.part
		Part	:SnowBurst.part
		Lifespan 20
	End

	Event
		Type	Local
		At	Hips
		Bhvr	:icearmor.bhvr
		Part	:bodychillbig.part
		Part	:SnowBurst.part
		Lifespan 20
	End


	Event
		Type	Local
		At	UARML
		Bhvr	:icearmor.bhvr
		Part	:bodychillbig.part
		Part	:SnowBurst.part
		Lifespan 20
	End

	Event
		Type	Local
		At	UARMR
		Bhvr	:icearmor.bhvr
		Part	:bodychillbig.part
		Part	:SnowBurst.part
		Lifespan 20
	End

	Event
		Type	Local
		At	LARMR
		Bhvr	:icearmor.bhvr
		Part	:bodychillbig.part
		Part	:SnowBurst.part
		Lifespan 20
	End

	Event
		Type	Local
		At	LARML
		Bhvr	:icearmor.bhvr
		Part	:bodychillbig.part
		Part	:SnowBurst.part
		Lifespan 20
	End

	Event
		Type	Local
		At	WepL
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Snowfall.part
		Lifespan 40
	End

	Event
		Type	Local
		At	WepR
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Part	:Snowfall.part
		Lifespan 40
	End
End

#############################################################

Condition
	On	Time
	Time	15

	Event
		Type 	local
		At	Root
		ChildFX	POWERS\Child_BuildUp.fx
	End
End

#############################################################

End