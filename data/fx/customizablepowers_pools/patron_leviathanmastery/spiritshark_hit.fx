#                                                          
FxInfo

	Flags InheritAlpha

	Input
		InpName hips
	End

	LifeSpan 120


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Origin
			Type Local
			Sound SharkBite5 95 95 0.75
		End

	End

	Condition
		On Time
		Time 0

		Event
			At Neck
			Type PositOnly
			EName throatspot
			LookAt T_Chest
		End

		Event
			At throatspot
			Type Local
			EName attackdir
			BhvrOverride
				PositionOffset 0 1 5
			End
		End

		Event
			At Root
			Type Local
			EName front
			BhvrOverride
				PositionOffset 0 0 15
			End
		End

	End

	Condition
		On Time
		Time 1

		Event
			At attackdir
			Type Start
			EName sharkdir
			Magnet front
			BhvrOverride
				TrackRate 1
			End
		End

		Event
			At Neck
			Type PositOnly
			EName sharkaim
			LookAt sharkdir
		End

		Event
			Anim Shark_Spirit_Frenzy_Tintable
			At sharkaim
			Type Local
			EName shark
			BhvrOverride
				TintGeom 1
				Scale .3 .3 .3
				PyrRotate 0 190 30
				PositionOffset 0 -.62 .75
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
			#Geom testing_target
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
