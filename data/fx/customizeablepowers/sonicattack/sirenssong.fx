#############################################################
## SirensSong.fx
#############################################################

FxInfo

Lifespan 80

#############################################################

Condition
	On 	Time
	Time 	9

	Event
		Type	local
		At	head
		Sound SirenSong 80 80 .99
	End
End

Condition
	On 	Time
	Time 	14

	Event
		EName 	MussleFlashRigging
		Type	local
		At	head
		BHVR	Behaviors\GenericParticleFade.bhvr
		Geom	PsionicRigging
		Lifespan 80
	End

	Event
		Type	local
		At	MussleFlashRigging
		altpiv	3
		BHVR	Behaviors\GenericParticleFade.bhvr
	#	Part	CustomizeablePowers\SonicAttack\MussleFlashRingCore.part
	#	Part	CustomizeablePowers\SonicAttack\MussleFlashRing.part
		Lifespan 80
	End
End

#############################################################

Condition
	On 	cycle
	Time 	8

	Event
		EName	core1
		Type	local
		At	root
		bhvr	CustomizeablePowers\SonicAttack\OrientScale.bhvr
		BhvrOverride
			Alpha			255
			TintGeom		1
		End
		Geom	Tintable_SonicGaleRing1
		LifeSpan 8
	End
End

Condition
	On 	cycle
	Time 	8

	Event
		EName	core2
		Type	Local
		At	root
		bhvr	Powers\SonicControl\SirensSong\OrientScaleOpposite.bhvr
		BhvrOverride
			Alpha			255
			TintGeom		1
		End
		Geom	Tintable_SonicGaleRing2
		LifeSpan 8
	End
End

#############################################################


Condition
	On 	cycle
	Time 	8

	Event
		EName	core1dark
		Type	local
		At	root
		bhvr	Powers\SonicControl\SirensSong\OrientScaledark.bhvr
		BhvrOverride
			Alpha		12

			TintGeom	1
		End
		Geom	Tintable_SonicGaleRing1Bright
		LifeSpan	8
	End

End

Condition
	On 	cycle
	Time 	8

	Event
		EName	core2dark
		Type	Local
		At	root
		bhvr	Powers\SonicControl\SirensSong\OrientScaleOppositedark.bhvr
		BhvrOverride
			Alpha		12
			TintGeom	1
		End
		Geom	Tintable_SonicGaleRing2Bright
		LifeSpan	8
	End

End

Condition
	On 	Time
	Time 	30

	Event
		EName 	core1
		type	Destroy
	End

	Event
		EName 	core2
		type	Destroy
	End
End

#############################################################

End