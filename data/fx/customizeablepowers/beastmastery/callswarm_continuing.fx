#
FxInfo

###########################################################


Condition
	On 	Time
	Time	10

	Event
		Type	Local
		At	Root
		Sound 	BM_CallSwarm_Continuing_Loop 50 50 .4
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
			Part :CallSwarm_Hit_SoloBees.part
			Part :CallSwarm_CC_Swirl.part
			Part :CallSwarm_Noise.part
		End

	End

###########################################################

End
