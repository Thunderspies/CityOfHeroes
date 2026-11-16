#
FxInfo

###########################################################


Condition
	On 	Time
	Time	10

	Event
		Type	Local
		At	Root
		Sound 	DD_FrigidBlast_Attack_01 300 100 .8
	End


		Event
			EName SpinAnchor
			Type Local
			At Chest

			BhvrOverride
				Spin	0.0 0.0 0.1
			End
		End

		Event
			EName Prime
			Bhvr :CallSwarm_Continuing.bhvr
			At SpinAnchor
			Type Local
		End

		Event
			At Prime
			EName Trail
			BhvrOverride
				PositionOffset	0.5 0.0 0.5
			End
			Type Local
			Part :CallSwarm_Hit_SoloBees_Customizable.part
			Part :CallSwarm_CC_Swirl_Customizable.part
			Part :CallSwarm_Noise_Customizable.part
		End

	End

###########################################################

End
