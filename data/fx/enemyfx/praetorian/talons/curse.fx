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
		Sound 	TOV_Banshee_Curse 100 100 .8
	End
End

	Condition
		On Time
		Time 12

		Event
			At Head
			Type Local
			#Sound sonicsmallblast3 80 80 0.97
		End

	End

	Condition
		On Time
		Time 0

		Event
			At Head
			Type Local
			EName rigging
			Geom PsiTele02
		End

		Event
			At Head
			Type Local
			EName MussleFlashRigging
			Geom PsionicRigging
		End

	End

	Condition
		On Time
		Time 25

		Event
			AltPiv 3
			At MussleFlashRigging
			Type Local
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part :MussleFlashRingCore.part
			Part :MussleFlashRing.part
			LifeSpan 8
		End

		Event
			At Head
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 0.3 0
			End
			Part ENEMYFX\Praetorian\Talons\DarkCloud.part
			Part ENEMYFX\Praetorian\Talons\DarkTendrils.part
			LifeSpan 50
		End

		Event
			At Head
			Type Local
			Bhvr behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 0.3 0
			End
			LifeSpan 40
		End

	End

	Condition
		On Time
		Time 28

		Event
			AltPiv 1
			At rigging
			Type Start
			EName prime
			LookAt T_Chest
			Magnet T_Chest
			Bhvr CustomizeablePowers\SonicAttack\SMLProjectile.bhvr
			BhvrOverride
				physForceType Up
				physForceRadius 15
				physForcePower 20
				physForcePowerJitter 10
			End
			Geom FrontFacingLocalPivot
		End

		Event
			AltPiv 1
			At Prime
			Type Local
			EName PrimeA
			Part CustomizeablePowers\SonicAttack\SMLSonicRingCore.part
			Part CustomizeablePowers\SonicAttack\SMLSonicRingCoreTrail.part
			Part CustomizeablePowers\SonicAttack\SMLSonicRingGlow.part
			Part CustomizeablePowers\SonicAttack\SMLSonicRingGlowTrail.part
			Part CustomizeablePowers\SonicAttack\SMLSonicRingCoreTrailBrightGreen.part
		End

		Event
			AltPiv 1
			At Prime
			Type Local
			EName PrimeB
			Part CustomizeablePowers\SonicAttack\SonicRingCore.part
			Part CustomizeablePowers\SonicAttack\SonicRingCoreTrail.part
			Part CustomizeablePowers\SonicAttack\SonicRingGlow.part
			Part CustomizeablePowers\SonicAttack\SonicRingGlowTrail.part
			Part CustomizeablePowers\SonicAttack\SonicRing_Trail_Whispy1.part
			Part ENEMYFX\Praetorian\Talons\DarkSmoke_Trail02.part
			#Part ENEMYFX\Praetorian\Talons\SonicRing_CoreFlow.part
			Part ENEMYFX\Praetorian\Talons\SonicRing_Flow.part
		End

	End

	Condition
		On PrimeHit

		Event
			Type Destroy
			EName Prime
		End

		Event
			Type Destroy
			EName PrimeA
		End

		Event
			Type Destroy
			EName PrimeB
		End

	End

End
