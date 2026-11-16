#                                                          
FxInfo


###########################################################

	Condition
		On Cycle
		Time 50

		Event
			At Hips
			Type Local
			ChildFx :Bats_Idle_Child02.fx
			EName Bats
			LookAt Hips
			Magnet Hips
			PMagnet Hips
			BhvrOverride
				#Scale 1 1 1
			End
			LifeSpan 150
		End

	End

End
