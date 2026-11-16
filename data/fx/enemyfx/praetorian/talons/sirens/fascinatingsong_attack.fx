#
FxInfo

	LifeSpan 150


###########################################################
Condition
	On 	Time
	Time	20
	Event
		Type	Local
		At	Root
		Sound 	TOV_Siren_FascinatingSong_Attack 100 100 .8
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
			Part ENEMYFX\Praetorian\Talons\MussleFlashRingCore.part
			Part ENEMYFX\Praetorian\Talons\MussleFlashRing.part
			LifeSpan 8
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
				TrackRate 1.5
				physForcePower 20
				physForcePowerJitter 10
				physForceRadius 15
				physForceType Up
			End
			Geom FrontFacingLocalPivot
		End

		Event
			AltPiv 1
			At Prime
			Type Local
			EName Prime2
			Part ENEMYFX\Praetorian\Talons\Sirens\Fascinating_NoteA.part
			Part ENEMYFX\Praetorian\Talons\Sirens\Fascinating_NoteB.part
			Part ENEMYFX\Praetorian\Talons\Sirens\Fascinating_Glow.part
			Part ENEMYFX\Praetorian\Talons\Sirens\Fascinating_Glow_Trail.part
			Part ENEMYFX\Praetorian\Talons\Sirens\Fascinating_Specks.part
			Part ENEMYFX\Praetorian\Talons\Sirens\Fascinating_Song.part
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
			EName Prime2
		End

	End

End
