#
FxInfo


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Root
			Type StartPositOnly
			CameraSpace
			Bhvr Behaviors/GenericParticleFade.bhvr
			EName StormParts
			Flags OneAtATime
			EName CameraAnchor
		#	PArt	:Tentacle_Edges.part
			PArt	:Tentacle_Edges2.part
			PArt	:Tentacle_Edges3.part
		End

	End

	Condition
		On Cycle
		Time 5
		Chance 0.8

		Event
			At CameraAnchor
			Type Local
			BhvrOverride
				#PositionOffset 0 0 0
				StartJitter 0 0 0
			End
		#	Part :Tentacle.part
			LifeSpan 2
		End

	End

End
