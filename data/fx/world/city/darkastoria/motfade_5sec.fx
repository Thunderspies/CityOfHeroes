#
FxInfo

	LifeSpan 121


###########################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	DA_MotFade_01 100 100 .8
	End
End

	Condition
		On Time
		Time 0

		Event
			At Root
			Type StartPositOnly
			CameraSpace
			EName CameraAnchor
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :Tentacle_Edges3.part
			LifeSpan 15
		End
	End

	Condition
		On Time
		Time 15

		Event
			At Root
			Type StartPositOnly
			CameraSpace
			EName CameraAnchor
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :Tentacle_Edges3.part
			LifeSpan 15
		End
	End

	Condition
		On Time
		Time 30

		Event
			At Root
			Type StartPositOnly
			CameraSpace
			EName CameraAnchor
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part :Tentacle_Edges3.part
			LifeSpan 15
		End
	End

#	Condition
#		On Time
#		Time 45
#
#		Event
#			At Root
#			Type StartPositOnly
#			CameraSpace
#			EName CameraAnchor
#			Bhvr Behaviors/GenericParticleFade.bhvr
#			Part :Tentacle_Edges3.part
#			LifeSpan 15
#		End
#	End

	Condition
		On Time
		Time 50

		Event
			At Root
			Type StartPositOnly
			CameraSpace
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part WORLD\City\DarkAstoria\FadeToBlack2.part
			LifeSpan 250
		End

	End

End
