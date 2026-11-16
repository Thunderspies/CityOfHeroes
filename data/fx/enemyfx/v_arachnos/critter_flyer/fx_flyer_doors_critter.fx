#########################################################
##	template

FxInfo

Flags  InheritAlpha   InheritAnimScale

########  Doors  #############################################

Condition
	On 	Time
	Time 	0

	Event
		ename	left
		Type	Local
		At	Col_L	
		Anim	FX_Door_Spider
		Bhvr	Behaviors/V_Flyer_door_offset_L.bhvr
		BhvrOverride
                        Behavior
			PositionOffset    -1.65 -1.04 -.04
			Scale		.1 .1 .1
			End

	End

	Event
		ename	Right
		Type	Local
		At	Hips	
		Anim	FX_Door_Spider
		Bhvr	Behaviors/V_Flyer_door_offset_R.bhvr
		BhvrOverride
                        Behavior
			PositionOffset    1.65 -1.04 -.04
			Scale		.1 .1 .1
			End
	End



End




End

