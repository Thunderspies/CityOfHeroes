#########################################################
##	template

FxInfo

Flags  InheritAlpha   InheritAnimScale

########  Doors  #############################################

Condition
	On 	Time
	Time 	0

	Event
		EName	left
		Type	Local
		At	Col_L	
		Anim	FX_Door_Spider
		Bhvr	Behaviors/V_Flyer_door_offset_L.bhvr
		BhvrOverride
                        Behavior
			PositionOffset    0 -.6 0
			End

	End

	Event
		EName	Right
		Type	Local
		At	Col_R	
		Anim	FX_Door_Spider
		Bhvr	Behaviors/V_Flyer_door_offset_R.bhvr
		BhvrOverride
                        Behavior
			PositionOffset    0 -.6 0
			End
	End



End




End

