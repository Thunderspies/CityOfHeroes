#
FxInfo

	LifeSpan 70


###########################################################
Condition
	On 	Time
	Time	20
	Event
		Type	Local
		At	Root
		Sound 	TOV_Siren_ForcefulSong_Attack 100 100 .8
	End
End

	#Condition
		#On Time
		#Time 4

		#Event
			#At Head
			#Type Local
			#Sound Sonic12 90 90 0.9
		#End

	#End

	#Condition
		#On Time
		#Time 29

		#Event
			#At Head
			#Type Local
			#Sound Sonic11 90 90 1.0
		#End

	#End

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
		Time 20

		Event
			At Root
			Type Local
			BhvrOverride
				PositionOffset 0 4 0
			End
			Part ENEMYFX\Praetorian\Talons\Sirens\PainfulSong_NotesA.part
			Part ENEMYFX\Praetorian\Talons\Sirens\PainfulSong_NotesB.part
			LifeSpan 55
		End

	End

	Condition
		On Time
		Time 32

		Event
			At Root
			Type Local
			ChildFx :PainfulSong_Child.fx
			BhvrOverride
				PositionOffset 0 4 1
			End
		End

	End

	Condition
		On Time
		Time 34

		Event
			At Head
			Type Local
			EName core
			Bhvr POWERS\SonicControl\Blasts\Offset2.bhvr
			Geom LocalPivot01
			LifeSpan 3
		End

		Event
			At Root
			Type Local
			BhvrOverride
				PositionOffset 0 4 2
			End
			Part ENEMYFX\Praetorian\Talons\Sirens\PainfulSong_SongMuzzle_Wide.part
			LifeSpan 10
		End

		Event
			AltPiv 4
			At core
			Type Start
			EName core3
			Part :MassiveMegaSonicRing.part
			Part :MassiveMegaSonicRing2.part
			Part ENEMYFX\Praetorian\Talons\Sirens\PainfulSong_Glow_Core2.part
			LifeSpan 3
		End

		Event
			AltPiv 4
			At core
			Type Local
			EName PushForce
			BhvrOverride
				PositionOffset 0 0 16
				PyrRotate 15 0 0
				physForceType Forward
				physForceRadius 32
				physForcePower 300
				physForcePowerJitter 50
			End
			LifeSpan 3
		End

	End

	Condition
		On Time
		Time 35

		Event
			AltPiv 3
			At MussleFlashRigging
			Type Local
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part ENEMYFX\Praetorian\Talons\MussleFlashRingCore.part
			Part ENEMYFX\Praetorian\Talons\MussleFlashRing.part
			Part ENEMYFX\Praetorian\Talons\Sirens\Exhausting_NoteA.part
			Part ENEMYFX\Praetorian\Talons\Sirens\BrightTendrils.part
			LifeSpan 10
		End

		Event
			At Root
			Type Local
			BhvrOverride
				PositionOffset 0 4 0
			End
			Part ENEMYFX\Praetorian\Talons\Sirens\PainfulSong_Glow_Core.part
		#	Part ENEMYFX\Praetorian\Talons\Sirens\PainfulSong_Glow_Core2.part
			Part ENEMYFX\Praetorian\Talons\Sirens\PainfulSong_GlowTrails.part
			LifeSpan 10
		End

	End

	Condition
		On Time
		Time 38

		Event
			At Head
			Type Local
			EName core
			Bhvr POWERS\SonicControl\Blasts\Offset2.bhvr
			Geom LocalPivot01
			LifeSpan 3
		End

		Event
			AltPiv 4
			At core
			Type Start
			EName core3
			Part :MassiveMegaSonicRing.part
			Part :MassiveMegaSonicRing2.part
			Part ENEMYFX\Praetorian\Talons\Sirens\PainfulSong_Glow_Core2.part
			LifeSpan 3
		End

		Event
			AltPiv 4
			At core
			Type Local
			EName PushForce
			BhvrOverride
				PositionOffset 0 0 16
				PyrRotate 15 0 0
				physForceType Forward
				physForceRadius 32
				physForcePower 300
				physForcePowerJitter 50
			End
			LifeSpan 3
		End

	End

	Condition
		On Time
		Time 40

		Event
			At Root
			Type Local
			BhvrOverride
				PositionOffset 0 4 2
			End
			Part ENEMYFX\Praetorian\Talons\Sirens\PainfulSong_GlowMuzzle.part
			LifeSpan 5
		End

	End

End
