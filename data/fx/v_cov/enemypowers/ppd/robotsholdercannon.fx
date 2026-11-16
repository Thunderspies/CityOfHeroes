#########################################################
##	
##

FxInfo

Flags InheritAnimScale InheritAlpha


##################################


Condition
	On	Time
	Time	0
	

	Event
		Type	Local
		At 	Chest
		Anim	FX_PPD_RobotSholderCannon
                        BhvrOverride
                                    Behavior
                                    PositionOffset	0 0 0
				    Scale		1.5 1.5 1.5
			End

	End


End

End				