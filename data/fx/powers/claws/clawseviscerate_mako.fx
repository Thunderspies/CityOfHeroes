#########################################################
##	Fireweapon
########################################################
FxInfo

Input
	Inpname	Root
End

Input
	Inpname	WepR
End

Input
	Inpname	WepL
End

Input
	Inpname	T_Chest
End

lifespan	80

Condition
	On 	Time
	Time 	10

	Event
		EName	Streak1
		Type Local
		At	Root
		Geom	RootToChestAdjustment01
		Sound Eviscerate5 100.0 100.0 .8
	End
	
	Event
		EName	Streak9
		Type	local
		At	wepL
		Part	:V_Mako_SpinTrails.part
		LifeSpan	40
	End

	Event
		EName	Streak8
		Type	local
		At	wepR
		
		Part	:V_Mako_SpinTrails.part
		LifeSpan	40
	End
End



Condition
	On 	Time
	Time 	52

	Event
		EName	Streak
		Type	start
		At	Streak1
		Altpiv	1
		Part	:EviscerateStreak.part
		Part	:EviscerateStreak.part
		Part	:EviscerateStreak.part
	End

End


Condition
	On 	Time
	Time 	51

	Event
		EName	streak
		Type	Destroy
		
	End
	

End



End
##################################
