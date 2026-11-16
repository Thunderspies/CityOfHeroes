#                                                          
FxInfo


###########################################################

	Condition
		On TriggerBits
		TriggerBits FORWARD
		DoMany 1

		Event
			At Chest
			Type Local
			EName ForwardAir
			Flags OneAtATime
			While FORWARD
			BhvrOverride
				PositionOffset 0 3 1.5
			End
			Part :Shockwave_Head_Continuing.part
			Part :Shockwave_Continuing.part
			Part :Shockwave_Head_Stream.part
			LifeSpan 20
		End

	End

	Condition
		On TriggerBits
		TriggerBits JUMP
		DoMany 1

		Event
			At Chest
			Type Local
			EName ForwardAir
			Flags OneAtATime
			While JUMP
			BhvrOverride
				PositionOffset 0 3 1.5
			End
			Part :Shockwave_Head_Continuing.part
			Part :Shockwave_Continuing.part
			Part :Shockwave_Head_Stream.part
			LifeSpan 20
		End

	End

	Condition
		On TriggerBits
		TriggerBits AIR
		DoMany 1

		Event
			At Chest
			Type Local
			EName ForwardAir
			Flags OneAtATime
			While AIR
			BhvrOverride
				PositionOffset 0 3 1.5
			End
			Part :Shockwave_Head_Continuing.part
			Part :Shockwave_Continuing.part
			Part :Shockwave_Head_Stream.part
			LifeSpan 20
		End

	End

	Condition
		On TriggerBits
		TriggerBits BACKWARD
		DoMany 1

		Event
			At Chest
			Type Local
			EName ForwardAir
			Flags OneAtATime
			While BACKWARD
			Part :Shockwave_Head_Continuing.part
			Part :Shockwave_Continuing.part
		End

	End

	Condition
		On TriggerBits
		TriggerBits STEPLEFT
		DoMany 1

		Event
			At Chest
			Type Local
			EName ForwardAir
			Flags OneAtATime
			While STEPLEFT
			Part :Shockwave_Head_Continuing_Side.part
			Part :Shockwave_Continuing_Side.part
		End

	End

	Condition
		On TriggerBits
		TriggerBits STEPRIGHT
		DoMany 1

		Event
			At Chest
			Type Local
			EName ForwardAir
			Flags OneAtATime
			While STEPRIGHT
			Part :Shockwave_Head_Continuing_Side.part
			Part :Shockwave_Continuing_Side.part
		End

	End

End
