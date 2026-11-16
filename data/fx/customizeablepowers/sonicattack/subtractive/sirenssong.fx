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
		Part	:MussleFlashRingCore.part
		Part	:MussleFlashRing.part
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
		Geom	Tintable_SonicGaleCircle1
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
		bhvr	CustomizeablePowers\SonicAttack\OrientScaleOpposite.bhvr
		BhvrOverride
			Alpha			255
			TintGeom		1
		End
		Geom	Tintable_SonicGaleCircle2
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
		bhvr	CustomizeablePowers\SonicAttack\OrientScaledark.bhvr
		BhvrOverride
			Alpha		13

			TintGeom	1
		End
		Geom	Tintable_SonicGaleRing1Alpha
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
		bhvr	CustomizeablePowers\SonicAttack\OrientScaleOppositedark.bhvr
		BhvrOverride
			Alpha		13
			TintGeom	1
		End
		Geom	Tintable_SonicGaleRing2Alpha
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