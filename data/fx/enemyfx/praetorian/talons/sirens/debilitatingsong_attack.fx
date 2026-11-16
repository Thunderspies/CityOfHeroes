#
FxInfo

	LifeSpan 300


###########################################################

#Condition
#	On 	Time
#	Time	30
#	Event
#		Type	Local
#		At	Root
#		Sound 	ToV_EarthSibyl_UpThrust_Attack_01 200 100 .8
#	End
#End

Condition
	On 	Time
	Time	30
	Event
		Type	Local
		At	Root
		Sound 	TOV_Siren_DebilitatingSong_Attack 100 100 1.0
	End
End


	Condition
		On Time
		Time 25

		Event
			At Head
			Type Local
			Bhvr Behaviors\GenericParticleFade.bhvr
			BhvrOverride
				PyrRotate -90 0 0
			End
			Part ENEMYFX\Praetorian\Talons\MussleFlashRingCore.part
			Part ENEMYFX\Praetorian\Talons\Sirens\Exhausting_NoteA.part
		#	Part ENEMYFX\Praetorian\Talons\Sirens\BrightTendrils.part
			LifeSpan 30
		End

		Event
			At Head
			Type Local
			EName Anchor
			Bhvr Behaviors/FadeOut.bhvr
			Part ENEMYFX\Praetorian\Talons\Sirens\MusicNotes_FallingC.part
			Part ENEMYFX\Praetorian\Talons\Sirens\MusicNotes_FallingD.part
			Part ENEMYFX\Praetorian\Talons\Sirens\Globs_Falling.part
			Lifespan 35
		End

	End

	Condition
		On Time
		Time 40

		Event
			At Anchor
			Type Local
			Bhvr ENEMYFX\Praetorian\Talons\Sirens\DebilitatingStream.bhvr
			BhvrOverride
				PositionOffset 0 0 0.3
				PyrRotate -50 0 0
				Scale 1 0.2 0.2
			End
			Geom FX_MusicStream
			LifeSpan 30
		End

		Event
			At Anchor
			Type Local
			Bhvr ENEMYFX\Praetorian\Talons\Sirens\DebilitatingStream.bhvr
			BhvrOverride
				PositionOffset 0 0 0.3
				PyrRotate -50 0 90
				Scale 1 0.2 0.2
			End
			Geom FX_MusicStream
			LifeSpan 30
		End

	End

	Condition
		On Time
		Time 55

		Event
			Ename Anchor2
			At Root
			Type Local
			BhvrOverride
				PositionOffset 0 0 3.5
			End
		End

		Event
			At Anchor2
			Type Start
			ChildFX	:Debilitating_ProjectileChild.fx
			Lifespan 10
		End
	End

	Condition
		On Time
		Time 65

		Event
			At Anchor2
			Type Start
			EName Prime
			LookAt T_Root
			Magnet T_Root
			Bhvr CustomizeablePowers\BeamRifle\Tracer_Fade.bhvr
			BhvrOverride
				TrackRate 1.0
			End
			Part :GroundTrail_Dust.part
			Part :GroundTrail_EnergyStreaks.part
			ChildFX	:Debilitating_ProjectileChild.fx
			LifeSpan 300
		End

	End

	Condition
		On PrimeHit

		Event
			Type Destroy
			EName Prime
		End

		Event
			At T_Root
			Type Local
			ChildFx ENEMYFX\Praetorian\Talons\Sirens\DebilitatingSong_Hit.fx
		End

	End

End
