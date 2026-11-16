#
FxInfo

	LifeSpan 20


###########################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	BM_CallSwarm_Hit_01 100 100 .8
	End
End

Condition

	On	Time
	Time	0

		Event
			Type Local
			At Chest
			Part :CallSwarm_Hit_Customizable.part
			Part :CallSwarm_Hit_Ring_Customizable.part
			Part :CallSwarm_Hit_Ring_Noise_Customizable.part
			Part :CallSwarm_Hit_Noise_Customizable.part
			Lifespan 20
		End
	End

###########################################################

End
