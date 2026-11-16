#########################################################
##
##
FxInfo

Flags InheritAlpha

##Lifespan 1

Condition  
	On Cycle
	Time 120
	Chance .15
	Event
		Type Local 
		At	Root
		Sound Bird_BlackBird_01 550 500 .35
		Sound Bird_BlackBird_02 550 500 .35
		Sound Bird_BlackBird_03 550 500 .35
		Lifespan 1
	End
End

Condition
	On Time
	Time 0

	Event
		EName 	Bird
		Type	Start
		At	Root
		Anim	FX_Crow_circle
		Bhvr	behaviors/rotate_180.bhvr
		Bhvr	behaviors/fadein.bhvr

	End

End

Condition
	On Time
	Time 55
	Event
		EName 	Bird_2
		Type	Start
		At	root
		Anim	FX_Crow_circle
		Bhvr	behaviors/flock_offset_1.bhvr
		Bhvr	behaviors/fadein.bhvr
		BhvrOverride
			StartJitter	50 10 50
		End
	End

End


Condition
	On Time
	Time 80
	Event
		EName 	Bird_3
		Type	Start
		At	root
		Anim	FX_Crow_circle
		Bhvr	behaviors/flock_offset_2.bhvr
		Bhvr	behaviors/rotate_180.bhvr
		Bhvr	behaviors/fadein.bhvr
		BhvrOverride
			StartJitter	50 10 50
		End
	End

End


Condition
	On Time
	Time 120
	Event
		EName 	Bird_4
		Type	Start
		At	root
		Anim	FX_Crow_circle
		Bhvr	behaviors/flock_offset_3.bhvr
		Bhvr	behaviors/fadein.bhvr
		BhvrOverride
			StartJitter	50 10 50
		End
	End

End


Condition
	On Time
	Time 300
	Event
		EName 	Bird_5
		Type	Start
		At	root
		Anim	FX_Crow_circle
		Bhvr	behaviors/flock_offset_4.bhvr
		Bhvr	behaviors/fadein.bhvr
		BhvrOverride
			StartJitter	50 10 50
		End
	End

End


Condition
	On Time
	Time 400
	Event
		EName 	Bird_6
		Type	Start
		At	root
		Anim	FX_Crow_circle
		Bhvr	behaviors/flock_offset_5.bhvr
		Bhvr	behaviors/fadein.bhvr
		BhvrOverride
			StartJitter	50 10 50
		End
	End

End


#######  2nd wave  ################################################


Condition
	On Time
	Time 550
	Event
		EName 	Bird_1
		Type	Start
		At	root
		Anim	FX_Crow_circle
		Bhvr	behaviors/fadein.bhvr
		BhvrOverride
			StartJitter	50 10 50
		End
	End

End

Condition
	On Time
	Time 623
	Event
		EName 	Bird_2
		Type	Start
		At	root
		Anim	FX_Crow_circle
		Bhvr	behaviors/flock_offset_1.bhvr
		Bhvr	behaviors/fadein.bhvr
		BhvrOverride
			StartJitter	50 10 50
		End
	End

End


Condition
	On Time
	Time 675
	Event
		EName 	Bird_3
		Type	Start
		At	root
		Anim	FX_Crow_circle
		Bhvr	behaviors/flock_offset_2.bhvr
		Bhvr	behaviors/rotate_180.bhvr
		Bhvr	behaviors/fadein.bhvr
		BhvrOverride
			StartJitter	50 10 50
		End
	End

End


Condition
	On Time
	Time 700
	Event
		EName 	Bird_4
		Type	Start
		At	root
		Anim	FX_Crow_circle
		Bhvr	behaviors/flock_offset_3.bhvr
		Bhvr	behaviors/fadein.bhvr
		BhvrOverride
			StartJitter	50 10 50
		End
	End

End


Condition
	On Time
	Time 720
	Event
		EName 	Bird_5
		Type	Start
		At	root
		Anim	FX_Crow_circle
		Bhvr	behaviors/flock_offset_4.bhvr
		Bhvr	behaviors/fadein.bhvr
		BhvrOverride
			StartJitter	50 10 50
		End
	End

End


Condition
	On Time
	Time 780
	Event
		EName 	Bird_6
		Type	Start
		At	root
		Anim	FX_Crow_circle
		Bhvr	behaviors/flock_offset_5.bhvr
		Bhvr	behaviors/fadein.bhvr
		BhvrOverride
			StartJitter	50 10 50
		End
	End

End



End