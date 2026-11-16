#
FxInfo

	LifeSpan 200


###########################################################
Condition
	On 	Time
	Time	20
	Event
		Type	Local
		At	Root
		Sound 	TOV_Banshee_Castigation 100 100 .8
	End
End

	Condition
		On Time
		Time 0

		Event
			At Root
			Type Local
	#		Sound sonicgale 100 100 0.88
		End

		Event
			Ename	ConeOffset
			Type	Start
			At	Root
			GEOM	FX_ConeOffsets
		End

		Event
			EName 	Anchor
			Type	Local
			At	ConeOffset
			BhvrOverride
				PositionOffset	0 5 0
			End
			AltPiv	1
		End

		Event
			Ename	TargetOffset2
			Type	Start
			At	ConeOffset
			AltPiv	2
		End
End

	Condition
		On Time
		Time 23

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
			Part	CustomizeablePowers\SonicAttack\MussleFlashRingCore.part
			Part	CustomizeablePowers\SonicAttack\MussleFlashRing.part
			Lifespan 45
		End

	End

	Condition
		On Time
		Time 25

		Event
			EName Prime
			At Anchor
			Type Start
			BhvrOverride
				PyrRotate 90 0 0
			End
			Part :SonicCoreElipse.part
			Part :SonicElipse.part
			Part CustomizeablePowers\SonicAttack\GaleMist.part
			Part ENEMYFX\Praetorian\Talons\DarkSmoke_Trail03.part
			Part ENEMYFX\Praetorian\Talons\DarkSmoke_Trail04.part
			Lifespan 30
		End

	End


End
