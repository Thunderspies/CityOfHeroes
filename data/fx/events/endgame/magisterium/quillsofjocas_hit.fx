#
FxInfo
Lifespan 150


###########################################################

	Condition
		On Time
		Time 0

		Event
			At chest
			Type Local
			Bhvr Behaviors\QuillProjectile4.bhvr
			BhvrOverride
				Scale .4 .4 .7
				StartColor 5 5 5
				PositionOffset 0 0 0.3
				StartJitter 0.3 0.5 0
			End
			Geom quill01
		End

		Event
			At chest
			Type Local
			Bhvr Behaviors\QuillProjectile4.bhvr
			BhvrOverride
				Scale .4 .4 .65
				StartColor 5 5 5
				PositionOffset 0 0 0.3
				StartJitter 0.3 0.5 0
			End
			Geom quill03
		End

		Event
			At chest
			Type Local
			Bhvr Behaviors\QuillProjectile4.bhvr
			BhvrOverride
				Scale .4 .4 .6
				StartColor 5 5 5
				PositionOffset 0 0 0.3
				StartJitter 0.3 0.5 0
			End
			Geom quill07
		End

		Event
			At chest
			Type Local
			Bhvr Behaviors\QuillProjectile4.bhvr
			BhvrOverride
				Scale .4 .4 .5
				StartColor 5 5 5
				PositionOffset 0 0 0.3
				StartJitter 0.3 0.5 0
			End
			Geom quill06
		End

	End

	Condition
		On Time
		Time 0
		Repeat 4

		Event
			At chest
			Type Create
			ChildFx :QuillsOfJocas_Hit_Child.fx
			Inherit Position Scale
			Update Position
			LifeSpan 75
		End

		Event
			At chest
			Type Create
			ChildFx :QuillsOfJocas_Hit_Child.fx
			HardwareOnly
			Inherit Position Scale
			Update Position
			LifeSpan 75
		End

	End

End
