#                                                          
FxInfo

	Flags InheritAlpha

	Input
		InpName hips
	End

	LifeSpan 130


###########################################################

	Condition
		On Time
		Time 8

		Event
			At Origin
			Type Local
			Sound SharkSwim2 145 145 1.0
		End

	End

	Condition
		On Time
		Time 0

		Event
			At Root
			Type Local
			EName spinner
			BhvrOverride
				PositionOffset 0 5 -2
				SpinJitter 0.05 0.05 0.05
			End
		End

	End

	Condition
		On Time
		Time 0
		Repeat 7

		Event
			At spinner
			Type Local
			EName charger1
			LookAt target
			PMagnet chest
			BhvrOverride
				StartJitter 6 6 6
				PositionOffset 0 0 0
			End
			Part :V_Mako_Spiritshark_Chargestream.part
			LifeSpan 30
		End

	End

	Condition
		On Time
		Time 60

		Event
			At Chest
			Type StartPositOnly
			EName prime1
			Magnet T_neck
			BhvrOverride
				TrackRate 1.4
			End
		End

		Event
			Anim Shark_Spirit_Charge_Tintable
			At Chest
			Type StartPositOnly
			EName prime
			Magnet T_neck
			Bhvr V_COV\EnemyPowers\ArchVillains\Mako\V_Mako_SpiritsharkFade.bhvr
			BhvrOverride
				TintGeom 1
				Scale .3 .3 .3
				TrackRate 1.5
			End
		End

		Event
			At prime
			Type Local
			EName chunkies
			PMagnet chunkies
			BhvrOverride
				PositionOffset 0 -2 0
			End
			Part :V_Mako_Spiritshark_SharkStream.part
		End

		Event
			AnimPiv chest
			At prime
			Type Local
			EName streamerdorsal
			PMagnet streamerdorsal
			BhvrOverride
				PositionOffset 0 1.5 -1
			End
			Part :V_Mako_Spiritshark_SharkTrails.part
			Part :V_Mako_Spiritshark_SharkStream.part
			#Geom testing_target
		End

		Event
			AnimPiv ToeL
			At prime
			Type Local
			EName streamerTail
			PMagnet prime
			BhvrOverride
				PositionOffset 0 .9 -1.5
			End
			Part :V_Mako_Spiritshark_SharkTrails.part
			#Geom testing_target
		End

		Event
			AnimPiv footL
			At prime
			Type Local
			EName streamerTail2
			PMagnet prime
			BhvrOverride
				PositionOffset 0 -1.5 -1.8
				PyrRotate 0 0 180
			End
			Part :V_Mako_Spiritshark_SharkTrails.part
			#Geom testing_target
		End

		Event
			AnimPiv UarmL
			At prime
			Type Local
			EName streamerFinL
			PMagnet prime
			BhvrOverride
				PositionOffset 0 -2.5 -1
				PyrRotate 30 0 180
			End
			Part :V_Mako_Spiritshark_SharkTrails.part
			#Geom testing_target
		End

		Event
			AnimPiv UarmR
			At prime
			Type Local
			EName streamerFinR
			PMagnet prime
			BhvrOverride
				PositionOffset 0 -2.5 -1
				PyrRotate 30 0 180
			End
			Part :V_Mako_Spiritshark_SharkTrails.part
			#Geom testing_target
		End

	End

	Condition
		On Prime1Hit

		Event
			Type Destroy
			EName prime1
		End

		Event
			Type Destroy
			EName prime
		End

	End

End
