#
FxInfo

	LifeSpan 200


###########################################################


Condition
	On 	Time
	Time	10
	Event
		Type	Local
		At	Root
		Sound 	BM_CallSwarm_Attack_01 300 100 .8
	End
End


	Condition
		On Time
		Time 17

		Event
			EName Prime
			Bhvr :CallSwarm_CC_Projectile.bhvr

			At Head
			Type Start
			LookAt Target
			Magnet Target
		End

		Event
			EName Prime2

			At Head
			Type Start
			Bhvr :CallSwarm_Projectile.bhvr
			LookAt Target
			Magnet Target
		End

		Event
			At Prime
			EName Trail
			BhvrOverride
				PositionOffset	0.5 0.0 1.0
			End
			Type Local
			Part :CallSwarm_CC_Swirl_Customizable.part
			Part :CallSwarm_Noise_Customizable.part

		End

		Event
			At Prime2
			EName Trail2
			BhvrOverride
				PositionOffset	0.5 0.0 0.0
			End
			Type Local
			Part :CallSwarm_Swirl_Customizable.part
		End
	End

	Condition
		On PrimeHit

		Event
			Type 	Destroy
			EName 	Prime

		End
	End

	Condition
		On Prime2Hit

		Event
			Type Destroy
			EName Prime2
		End

	End

End
