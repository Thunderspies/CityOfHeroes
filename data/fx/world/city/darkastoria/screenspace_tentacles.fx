#                                                          
FxInfo

	LifeSpan 61


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Root
			Type StartPositOnly
			CameraSpace 
			#ChildFx :Screenspace_Tentacles_CHILD.fx
			Bhvr Behaviors/GenericParticleFade.bhvr
			LifeSpan 30
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
			LifeSpan 30
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
			Part WORLD\City\DarkAstoria\FadeToBlack.part
			LifeSpan 30
		End

	End

	Condition
		On Time
		Time 40

		Event
			At Root
			Type StartPositOnly
			CameraSpace 
			Bhvr Behaviors/GenericParticleFade.bhvr
			Part WORLD\City\DarkAstoria\FadeToBlack2.part
			LifeSpan 30
		End

	End

End
