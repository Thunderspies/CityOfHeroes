#
FxInfo


###########################################################

	Condition
		On Time
		Time 0

		Event
			Type Start
			CameraSpace
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part :Camera_Dark.part
			Part :Camera_Dark_Clouds.part
			LifeSpan 10
		End

	End

	Condition
		On Time
		Time 35

		Event
			Type Start
			CameraSpace
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part :CAMERA_Debris.part
			LifeSpan 40
		End

		Event
			Type Start
			CameraSpace
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part :CAMERA_Dust.part
			LifeSpan 80
		End

		Event
			Type Start
			CameraSpace
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part :CAMERA_Dust.part
			LifeSpan 60
		End

	End

	Condition
		On Time
		Time 55

		Event
			Type Start
			CameraSpace
			Bhvr Behaviors\GenericParticleFade.bhvr
			Part :Camera_Dust_Still.part
			LifeSpan 30
		End

	End

End
