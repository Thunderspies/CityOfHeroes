#
FxInfo

	Flags InheritAlpha DontSuppress


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Root
			Type Local
			Sound Tumbleweed_Wind_Loop 100 100 0.1
		End

	End

	Condition
		On TimeOfDay
		DayStart 5.0000
		DayEnd 19.0000
		on time
		time 0

		Event
			At Root
			Type Local
			EName TumbleAnchor
			Flags oneatatime
		End

	End

	Condition
		On Cycle
		Time 60
		Chance 0.200

		Event
			At TumbleAnchor
			Type Start
			CEvent :Tumbleweed_CHILD.fx
			EName Tumble
			PhysicsOnly
			Bhvr behaviors\Generic_FadeIn-FadeOut.bhvr
			BhvrOverride
				InitialVelocity -0.35 0.15 0.05
				InitialVelocityJitter 0.1 0.100 0.1
				PositionOffset 0 3 0
				Scale 0.2 0.200 0.2000
				Spin 0.1 0.1 0.1
				SpinJitter 0.08 0.08 0.08
				FadeInLength 45
				FadeOutLength 45
				physics 1
				physDensity 0.6
				physGravity 0.40
				physRestitution 1
				physSFriction 0
			End
			Geom FX_Geo_Tumbleweed__Tumbleweed
			LifeSpan 200
		End

	End

End
