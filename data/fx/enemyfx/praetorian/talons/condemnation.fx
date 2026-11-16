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
		Sound 	TOV_Banshee_Condemnation 100 100 .8
	End
End

	Condition
		On Time
		Time 18

		Event
			At Head
			Type Start
			EName MussleFlashRigging
			Bhvr Behaviors\GenericParticleFade.bhvr
			BhvrOverride
				PyrRotate 0 -5 0
			End
			Geom PsionicRigging
			LifeSpan 80
		End

		Event
			AltPiv 3
			At MussleFlashRigging
			Type Local
			Bhvr Behaviors\GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 0 -0.5
			End
			Part :MussleFlashRingCore2.part
			Part :MussleFlashRing2.part
			Part :DarkBreath.part
			LifeSpan 30
		End

	End

	Condition
		On Time
		Time 30

		Event
			At Head
			Type Local
			EName LocknarDummyHookup
			BhvrOverride
				PositionOffset 0 1.5 3
			End
			#Sound radblast 80 80 0.85
			LifeSpan 35
		End

		Event
			At LocknarDummyHookup
			Type Local
			Bhvr CustomizeablePowers\RadiationBlast\InfectOthersFadeIn.bhvr
			BhvrOverride
				Scale 0.01 0.01 0.01
				EndScale 0.25 0.25 0.25
				StartColor 1 1 1
			End
			Part :SonicBall_RingsOut_Local.part
			Part :SonicBall_RingsOut_Thick_Local.part
			Part :SonicBall_DarkSplotch_Local.part
			Part :SonicBall_DarkSplotches_Local.part
			Geom Tintable_ShadowField
			LifeSpan 33
		End

	End

	Condition
		On Time
		Time 63

		Event
			At LocknarDummyHookup
			Type Start
			EName Prime
			LookAt Target
			Magnet Target
			Bhvr CustomizeablePowers\RadiationBlast\MorterLobbprojectile.bhvr
			BhvrOverride
				StartColor 1 1 1
				Scale .25 .25 .25
			End
			Part :SonicBall_RingsOut_Local.part
			Part :SonicBall_RingsOut_Thick_Local.part
			Part :SonicBall_DarkSplotch_Local.part
			Part :DarkSmoke_Trail02.part
			Part :SonicBall_DarkSplotches_Local.part
			Part :SonicRing_Flow.part
			Geom Tintable_ShadowField
		End

		Event
			At Prime
			Type Local
			EName PrimeB
			Part ENEMYFX\Praetorian\Talons\DarkSmoke_Trail02.part
			Part :SonicRing_Flow2.part
		End

	End

	Condition
		On PrimeHit

		Event
			Type Destroy
			EName Prime
		End

		Event
			At T_Chest
			Type Local
			ChildFx :SonicDark_Hit_Heavy.fx
			EName Mushroom
		End

	End

End
