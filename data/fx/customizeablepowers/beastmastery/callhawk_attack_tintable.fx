#
FxInfo

	LifeSpan 150


###########################################################

Condition
	On 	Time
	Time	15
	Event
		Type	Local
		At	Root
		Sound 	BM_CallHawk_Attack_01 100 100 .8
	End
End

	Condition
		On Time
		Time 0

		Event
			At Root
			Type Local
			EName Hawkward
			BhvrOverride
				PositionOffset 10 15 -30
			End
		End
	End

	Condition
		On Time
		Time 20

		Event
			At Hawkward
			Type Start
			EName Prime
			LookAt Target
			Magnet Target
			POther Target
			Bhvr behaviors\Braz\Projectile_FastWithUp_01.bhvr
			BhvrOverride
				InitialVelocity 0 -0.25 0
				Scale 1 1 1
				Spin 0 0 0
				TrackRate 3
			End
		End

		Event
			EName Trail
			Anim hawk_Tintable
			SetState COMBAT
			At Prime
			Type Local
			Bhvr CustomizeablePowers\BeastMastery\Hawk_fadein.bhvr
			BhvrOverride
				TintGeom 1
				PyrRotate -45 0 0
				Animscale 3
			End
		End

	End

	Condition
		On PrimeHit

		Event
			Type Destroy
			EName Prime
		End

	End

End
