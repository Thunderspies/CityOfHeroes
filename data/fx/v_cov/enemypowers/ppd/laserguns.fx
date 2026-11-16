#########################################################
##	
##

FxInfo

Flags InheritAnimScale

##################################


Condition
	On	Time
	Time	0
	

	Event
		Type	Local
		At 	LarmR
		Anim	FX_PPD_Laser_Right

	End

	Event
		Type	Local
		At 	LarmL
		Anim	FX_PPD_Laser_Left
                        BhvrOverride
                                    Behavior
				PyrRotate		180 180 0

				PositionOffset		0 0 0
			End

	End


End

End				