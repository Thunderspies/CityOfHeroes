#
FxInfo

	Flags InheritAlpha

	LifeSpan 150


###########################################################

	Condition
		On Time
		Time 0

		Event
			At Origin
			Type Start
			EName pointer
		End

		Event
			At pointer
			Type Local
			EName hitarg
			BhvrOverride
				PositionOffset 0 65 100
				StartJitter 0 30 0
				TintGeom 1
			End
		End

		Event
			At pointer
			Type Local
			EName histart
			BhvrOverride
				PositionOffset 0 0 100
			End
		End

		Event
			At histart
			Type Start
			EName prime
			LookAt origin
			Magnet hitarg
			BhvrOverride
				TrackRate 1
			End
		End

		Event
			At prime
			Type PositOnly
			EName spin1
			Bhvr V_COV\EnemyPowers\ArchVillains\Mako\V_Mako_Wandershrink.bhvr
		End

		Event
			At spin1
			Type Local
			EName spot1
			BhvrOverride
				PositionOffset 0 0 20
			End
		End

		Event
			At spot1
			Type Local
			EName counter
			BhvrOverride
				InitialVelocity 0 -.98 0
			End
		End

		Event
			Anim Shark_Spirit_Charge_Tintable
			At Origin
			Type Start
			EName shark
			Magnet counter
			BhvrOverride
				TintGeom	1
				TrackRate .8
				Scale .3 .3 .3
			End
		End

		Event
			AnimPiv head
			At shark
			Type Local
			ChildFx :CHILD_Ripple.fx
			EName nosetip
			BhvrOverride
				PositionOffset 0 0 3
				Scale 3 3 3
			End
		End

	End

	Condition
		On PrimeHit

		Event
			At nosetip
			Type StartPositOnly
			EName spot1
			BhvrOverride
				TrackRate .8
				Scale .5 .5 .5
			End
			LifeSpan 10
		End

		Event
			At Root
			Type StartPositOnly
			EName prime1
			Magnet head
			BhvrOverride
				TrackRate .4
				Scale .5 .5 .5
			End
		End

		Event
			At nosetip
			Type StartPositOnly
			ChildFx :CHILD_Splash.fx
			BhvrOverride
				Scale 2 1 2
			End
		End

	End

	Condition
		On Prime1Hit

		Event
			Type Destroy
			EName shark
		End

	End

End
