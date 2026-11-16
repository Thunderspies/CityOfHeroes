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
		Sound 	TOV_Siren_SorrowfulSong_Attack 100 100 .8
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

		Event
			At Root
			Type Local
			EName EndAnchor
			BhvrOverride
				PositionOffset 0 3 40
			End
		End

		Event
			At EndAnchor
			Type Local
			EName NoteAnchor
			BhvrOverride
				StartJitter 1 1 0
			End
		End

	End

	Condition
		On Time
		Time 13

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
		Time 16

		Event
			At Root
			Type Local
			POther NoteAnchor
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 4 2
				PyrRotate -8 0 0
			End
			Part ENEMYFX\Praetorian\Talons\Sirens\PainfulSong_SongMuzzle_Slim.part
			Part ENEMYFX\Praetorian\Talons\Sirens\Sorrowful_NoteA.part
			LifeSpan 30
		End

		Event
			At Root
			Type Local
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 4.2 1
				PyrRotate -8 0 0
			End
			Part :SorrowDust_Initial.part
			LifeSpan 35
		End

	End

	Condition
		On Time
		Time 18

		Event
			At Root
			Type Local
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0.0 0.5 10
			End
			Part :Sorrow_GroundDust.part
			Part :Ground_NoteA.part
			LifeSpan 35
		End

	End

	Condition
		On Time
		Time 23

		Event
			At Root
			Type Local
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0.0 0.5 15
				#PyrRotate 0 0 180
			End
			Part :SorrowDust_Traveling.part
			LifeSpan 10
		End

	End

	Condition
		On Time
		Time 35

		Event
			At Root
			Type Local
			EName AirRushAnchor
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 2 0 30
				PyrRotate 0 20 0
			End
			Part :Sploding_NoteA.part
			Part :Sploding_NoteB.part
			LifeSpan 25
		End

		Event
			At Root
			Type Local
			EName AirRushAnchor
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 0 30
				PyrRotate 0 0 0
			End
			Part :Sploding_NoteA.part
			Part :Sploding_NoteB.part
			LifeSpan 25
		End

		Event
			At Root
			Type Local
			EName AirRushAnchor
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset -2 0 30
				PyrRotate 0 -20 0
			End
			Part :Sploding_NoteA.part
			Part :Sploding_NoteB.part
			LifeSpan 25
		End

	End

	Condition
		On Time
		Time 50

		Event
			At Root
			Type Local
			EName AirRushAnchor
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 2 0 40
				PyrRotate 0 20 0
			End
			Part :RisingDust_Traveling.part
			LifeSpan 25
		End

		Event
			At Root
			Type Local
			EName AirRushAnchor
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset 0 0 43
				PyrRotate 0 0 0
			End
			Part :RisingDust_Traveling.part
			LifeSpan 25
		End

		Event
			At Root
			Type Local
			EName AirRushAnchor
			Bhvr Behaviors/GenericParticleFade.bhvr
			BhvrOverride
				PositionOffset -2 0 41
				PyrRotate 0 -20 0
			End
			Part :RisingDust_Traveling.part
			LifeSpan 25
		End

	End

End
