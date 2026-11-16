#
FxInfo

	Flags InheritAlpha


###########################################################

	Condition
		On Time
		Time 5

		Event
			At Origin
			Type Local
			Sound Jaws1 80 80 0.9
		End

	End

	Condition
		On Time
		Time 0

		Event
			At Origin
			Type Local
			Sound sharkbite_loop 80 80 0.7
		End

	End

	Condition
		On Time
		Time 2

		Event
			At Root
			Type Start
			ChildFx :CHILD_DeepSplash.fx
		End

	End

	Condition
		On Time
		Time 1

		Event
			Anim Shark_Spirit_Hold_Tintable
			At Root
			Type Local
			EName shark
			Bhvr V_COV\EnemyPowers\ArchVillains\Mako\V_Mako_SpiritsharkFadeout.bhvr
			BhvrOverride
				PositionOffset 0 4 0
				PyrRotate 90 0 0
				TintGeom 1
			End
		End

	End

	Condition
		On Time
		Time 1

		Event
			At shark
			Type Local
			EName chunkies
			PMagnet chunkies
			BhvrOverride
				PositionOffset 0 0 -10
			End
			Part :V_Mako_Spiritshark_SharkStream_back.part
		End

		Event
			AnimPiv chest
			At shark
			Type Local
			EName streamerdorsal
			PMagnet prime
			BhvrOverride
				PositionOffset 0 1.5 -1
			End
			Part :V_Mako_Spiritshark_SharkTrails.part
			#Geom testing_target
		End

		Event
			AnimPiv ToeL
			At shark
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
			At shark
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
			At shark
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
			At shark
			Type Local
			EName streamerFinR
			PMagnet prime
			BhvrOverride
				PositionOffset 0 -2.5 -1
				PyrRotate 30 0 180
			End
			Part :V_Mako_Spiritshark_SharkTrails.part
		End

	End

	Condition
		On Cycle
		Time 21

		Event
			AnimPiv jaw
			At shark
			Type Local
			PMagnet chunkies
			BhvrOverride
				PyrRotate 0 180 0
			End
			Part :V_Mako_Spiritshark_Mouthburst.part
			LifeSpan 60
		End

		Event
			AnimPiv jaw
			At shark
			Type Local
			PMagnet chunkies
			Part :V_Mako_Spiritshark_Mouthburst.part
			LifeSpan 60
		End

	End

	Condition
		On Cycle
		Time 16

		Event
			AnimPiv jaw
			At shark
			Type Local
			PMagnet chunkies
			BhvrOverride
				PyrRotate 0 180 0
			End
			Part :V_Mako_Spiritshark_Mouthburst.part
			LifeSpan 60
		End

		Event
			AnimPiv jaw
			At shark
			Type Local
			PMagnet chunkies
			Part :V_Mako_Spiritshark_Mouthburst.part
			LifeSpan 60
		End

	End

End
