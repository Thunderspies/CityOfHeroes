###########################################################

FxInfo

	Input
		InpName root
	End


###########################################################

Condition
	On	Time

		Event
			EName	MaleWaterfall_Still
			At 	Root
			Type 	Local
			BhvrOverride
				PositionOffset	0.0 0.0 0.0
			End
			Part :Splash_Idle.part
			Part :Splash_Idle_Colored.part
			Part :Splash_Movement.part
			Part :Splash_Idle_Flat.part
			Part :Splash_Idle_Additive.part
			Part :Splash_Movement_Flat.part
		End
End

###########################################################

End
