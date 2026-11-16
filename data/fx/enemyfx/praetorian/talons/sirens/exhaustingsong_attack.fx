#
FxInfo

	LifeSpan 250


###########################################################
Condition
	On 	Time
	Time	20
	Event
		Type	Local
		At	Root
		Sound 	TOV_Siren_ExhaustingSong_Attack 100 100 .8
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
		Time 15

		Event
			AltPiv 3
			At MussleFlashRigging
			Type Local
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part ENEMYFX\Praetorian\Talons\MussleFlashRingCore.part
		#	Part ENEMYFX\Praetorian\Talons\MussleFlashRing.part
			Part ENEMYFX\Praetorian\Talons\Sirens\Exhausting_NoteA.part
			Part ENEMYFX\Praetorian\Talons\Sirens\BrightTendrils.part
			LifeSpan 30
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
		Time 20

		Event
			AltPiv 1
			At rigging
			Type Start
			EName prime
			LookAt T_Chest
			Magnet T_Chest
			Bhvr POWERS\SonicControl\Blasts\Projectile.bhvr
			BhvrOverride
				TrackRate 1.5
				physForcePower 20
				physForcePowerJitter 10
				physForceRadius 15
				physForceType Up
			End
			Geom FrontFacingLocalPivot
			Geom FrontFacingLocalPivot
		End

		Event
			AltPiv 1
			At Prime
			Type Local
			EName P1
			Part ENEMYFX\Praetorian\Talons\Sirens\Fascinating_NoteA.part
			Part ENEMYFX\Praetorian\Talons\Sirens\Fascinating_NoteB.part
			Part ENEMYFX\Praetorian\Talons\Sirens\Fascinating_Specks.part
			Part ENEMYFX\Praetorian\Talons\Sirens\Fascinating_BigGlow.part
			Part ENEMYFX\Praetorian\Talons\Sirens\Fascinating_BigGlow_Trail.part
			Part ENEMYFX\Praetorian\Talons\Sirens\Fascinating_BigSong.part
			Part POWERS\SonicControl\Blasts\SonicRingCore.part
			Part POWERS\SonicControl\Blasts\SonicRingGlowTrail.part
			Part POWERS\SonicControl\Blasts\SonicRingGlow.part
			Part POWERS\SonicControl\Blasts\SonicRingCoreTrail.part
			Part POWERS\SonicControl\Blasts\SonicRingCoreTrailBright.part
			Part POWERS\SonicControl\Blasts\SonicRingCoreTrailBrightGreen.part
			Part POWERS\SonicControl\Blasts\SMLSonicRingCoreTrailBright2.part
			Part POWERS\SonicControl\Blasts\SMLSonicRingCoreTrailBrightGreen2.part
		End

	End

	Condition
		On Time
		Time 30

		Event
			AltPiv 1
			At rigging
			Type Start
			EName prime2
			LookAt T_Chest
			Magnet T_Chest
			Bhvr POWERS\SonicControl\Blasts\Projectile.bhvr
			BhvrOverride
				physForceType Up
				physForceRadius 20
				physForcePower 15
				physForcePowerJitter 5
			End
			Geom FrontFacingLocalPivot
		End

		Event
			At Prime2
			Type Local
			EName p2
			Part ENEMYFX\Praetorian\Talons\Sirens\Fascinating_NoteA.part
			Part ENEMYFX\Praetorian\Talons\Sirens\Fascinating_NoteB.part
			Part ENEMYFX\Praetorian\Talons\Sirens\Fascinating_Specks.part
			Part ENEMYFX\Praetorian\Talons\Sirens\Fascinating_BigGlow.part
			Part ENEMYFX\Praetorian\Talons\Sirens\Fascinating_BigGlow_Trail.part
			Part ENEMYFX\Praetorian\Talons\Sirens\Fascinating_BigSong2.part
			Part POWERS\SonicControl\Blasts\SonicRingCore.part
			Part POWERS\SonicControl\Blasts\SonicRingGlowTrail.part
			Part POWERS\SonicControl\Blasts\SonicRingGlow.part
			Part POWERS\SonicControl\Blasts\SonicRingCoreTrail.part
			Part POWERS\SonicControl\Blasts\SonicRingCoreTrailBright.part
			Part POWERS\SonicControl\Blasts\SonicRingCoreTrailBrightGreen.part
			Part POWERS\SonicControl\Blasts\SMLSonicRingCoreTrailBright2.part
			Part POWERS\SonicControl\Blasts\SMLSonicRingCoreTrailBrightGreen2.part
		End

	End

	Condition
		On Time
		Time 40

		Event
			AltPiv 1
			At rigging
			Type Start
			EName prime3
			LookAt T_Chest
			Magnet T_Chest
			Bhvr POWERS\SonicControl\Blasts\Projectile.bhvr
			BhvrOverride
				physForceType Up
				physForceRadius 20
				physForcePower 15
				physForcePowerJitter 5
			End
			Geom FrontFacingLocalPivot
		End

		Event
			At Prime3
			Type Local
			EName p3
			Part ENEMYFX\Praetorian\Talons\Sirens\Fascinating_NoteA.part
			Part ENEMYFX\Praetorian\Talons\Sirens\Fascinating_NoteB.part
			Part ENEMYFX\Praetorian\Talons\Sirens\Fascinating_Specks.part
			Part ENEMYFX\Praetorian\Talons\Sirens\Fascinating_BigGlow.part
			Part ENEMYFX\Praetorian\Talons\Sirens\Fascinating_BigGlow_Trail.part
			Part ENEMYFX\Praetorian\Talons\Sirens\Fascinating_BigSong.part
			Part POWERS\SonicControl\Blasts\SonicRingCore.part
			Part POWERS\SonicControl\Blasts\SonicRingGlowTrail.part
			Part POWERS\SonicControl\Blasts\SonicRingGlow.part
			Part POWERS\SonicControl\Blasts\SonicRingCoreTrail.part
			Part POWERS\SonicControl\Blasts\SonicRingCoreTrailBright.part
			Part POWERS\SonicControl\Blasts\SonicRingCoreTrailBrightGreen.part
			Part POWERS\SonicControl\Blasts\SMLSonicRingCoreTrailBright2.part
			Part POWERS\SonicControl\Blasts\SMLSonicRingCoreTrailBrightGreen2.part
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
			EName P2
		End

	End

	Condition
		On Prime1Hit

		Event
			Type Destroy
			EName Prime1
		End

		Event
			Type Destroy
			EName P1
		End

	End

	Condition
		On Prime2Hit

		Event
			Type Destroy
			EName Prime2
		End

		Event
			Type Destroy
			EName P2
		End

	End

	Condition
		On Prime3Hit

		Event
			Type Destroy
			EName Prime3
		End

		Event
			Type Destroy
			EName P3
		End

	End

	#Condition
		#On Prime4Hit

		#Event
			#Type Destroy
			#EName Prime4
		#End

		#Event
			#Type Destroy
			#EName P4
		#End

	#End

End
