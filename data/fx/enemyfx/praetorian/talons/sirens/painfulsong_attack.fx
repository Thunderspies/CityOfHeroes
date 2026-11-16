#
FxInfo

	LifeSpan 120


###########################################################
Condition
	On 	Time
	Time	20
	Event
		Type	Local
		At	Root
		Sound 	TOV_Siren_PainfulSong_Attack 100 100 .8
	End
End

	Condition
		On Time
		Time 0

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
			Part ENEMYFX\Praetorian\Talons\MussleFlashRing.part
			Part ENEMYFX\Praetorian\Talons\Sirens\Exhausting_NoteA.part
			LifeSpan 30
		End

	End

	Condition
		On Time
		Time 20

		Event
			At Origin
			Type Local
			Bhvr behaviors\fadeInOut.bhvr
			BhvrOverride
				PyrRotate 0 15 0
				PyrRotateJitter 0 0 0
				Scale 2 1 2
			End
			Geom FX_MusicStream
			LifeSpan 40
		End

		Event
			At Origin
			Type Local
			Bhvr behaviors\fadeInOut.bhvr
			BhvrOverride
				PyrRotate 0 -15 0
				PyrRotateJitter 0 0 0
				Scale 2 1 2
			End
			Geom FX_MusicStream
			LifeSpan 40
		End

		Event
			At Root
			Type Local
			ChildFx :PainfulSong_Child.fx
			BhvrOverride
				PositionOffset 0 5 1
			End
		End

	End

	Condition
		On Time
		Time 10

		Event
			At Root
			Type Local
			BhvrOverride
				PositionOffset 0 5 0
			End
			Part ENEMYFX\Praetorian\Talons\Sirens\PainfulSong_NotesA.part
			Part ENEMYFX\Praetorian\Talons\Sirens\PainfulSong_NotesB.part
			LifeSpan 55
		End

	End

	Condition
		On Time
		Time 25

		Event
			At Root
			Type Local
			BhvrOverride
				PositionOffset 0 5 0
			End
			Part ENEMYFX\Praetorian\Talons\Sirens\PainfulSong_Glow_Core.part
			Part ENEMYFX\Praetorian\Talons\Sirens\PainfulSong_Glow_Core2.part
			Part ENEMYFX\Praetorian\Talons\Sirens\PainfulSong_GlowTrails.part
			LifeSpan 55
		End

		Event
			At Root
			Type Local
			BhvrOverride
				PositionOffset 0 5 2
			End
			Part ENEMYFX\Praetorian\Talons\Sirens\PainfulSong_SongMuzzle.part
			Part ENEMYFX\Praetorian\Talons\Sirens\PainfulSong_GlowMuzzle.part
			LifeSpan 40
		End

	End

End
