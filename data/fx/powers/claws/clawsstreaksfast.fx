#########################################################
##	template

FxInfo

Input  
	Inpname	Root
End

Input
	Inpname Waist
End

Input
	Inpname Chest
End

Input
	Inpname Head
End
#########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	local
##		Bhvr	behaviors/speed.bhvr
		At	Root
		Anim	FX_clawspeed
	End
End

#################################################################

Condition
	On 	Time
	Time 	15

	Event
		EName	swipe
		Type	start
		At	waist
		Geom	swoosh
		bhvr	behaviors/Claws.bhvr
		Sound ClawSwipe 100.0 100.0 .8
	End

End

Condition
	On 	Time
	Time 	18
	
	Event
		Type	Destroy
		EName	swipe
	End


	Event
		EName	swipe01
		Type	start
		At	Chest
		Geom	swoosh
		bhvr	behaviors/Claws.bhvr
	End

End

Condition
	On 	Time
	Time 	21
	
	Event
		Type	Destroy
		EName	swipe01
	End


	Event
		EName	swipe02
		Type	start
		At	waist
		Geom	swoosh01
		bhvr	behaviors/Claws.bhvr
	End

End

Condition
	On 	Time
	Time 	24
	
	Event
		Type	Destroy
		EName	swipe02
	End


	Event
		EName	swipe03
		Type	start
		At	Chest
		Geom	swoosh
		bhvr	behaviors/Claws.bhvr
	End

End

Condition
	On 	Time
	Time 	27
	
	Event
		Type	Destroy
		EName	swipe03
	End


	Event
		EName	swipe04
		Type	start
		At	waist
		Geom	swoosh01
		bhvr	behaviors/Claws.bhvr
	End

End

Condition
	On 	Time
	Time 	30
	
	Event
		Type	Destroy
		EName	swipe04
	End

End

#################################################################

Condition
	On 	Time
	Time 	29

	Event
		EName	swipe05
		Type	start
		At	Chest
		Geom	swoosh
		bhvr	behaviors/Claws.bhvr
	End

End

Condition
	On 	Time
	Time 	32
	
	Event
		Type	Destroy
		EName	swipe05
	End

End

Condition
	On 	Time
	Time 	32

	Event
		EName	swipe06
		Type	start
		At	waist
		Geom	swoosh01
		bhvr	behaviors/Claws.bhvr
	End

End

Condition
	On 	Time
	Time 	35
	
	Event
		Type	Destroy
		EName	swipe06
	End

End



Condition
	On 	Time
	Time 	45

	Event
		EName	swipe07
		Type	start
		At	waist
		Geom	swoosh
		bhvr	behaviors/Claws.bhvr
	End

End

Condition
	On 	Time
	Time 	52

	Event
		Type	Destroy
		EName	all
	End

End

End		