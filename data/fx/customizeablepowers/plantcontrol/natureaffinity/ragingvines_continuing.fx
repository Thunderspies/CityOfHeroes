FXInfo

	Condition
		On TriggerBits
		TriggerBits JUSTFUCKINDOIT
		DoMany 1

		Event
			Ename FloatHold
			type Local
			at root
			while JUSTFUCKINDOIT
			Flags OneAtATime
			childFX :RagingVines.fx
		End
	End
End