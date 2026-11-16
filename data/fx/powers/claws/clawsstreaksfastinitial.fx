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
	Time 	34

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
	Time 	37
	
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
	Time 	40
	
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
	Time 	43
	
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
	Time 	46
	
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
	Time 	49
	
	Event
		Type	Destroy
		EName	swipe04
	End

End

#################################################################

Condition
	On 	Time
	Time 	48

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
	Time 	51
	
	Event
		Type	Destroy
		EName	swipe05
	End

End

Condition
	On 	Time
	Time 	51

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
	Time 	54
	
	Event
		Type	Destroy
		EName	swipe06
	End

End



Condition
	On 	Time
	Time 	64

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
	Time 	71

	Event
		Type	Destroy
		EName	all
	End

End

End		