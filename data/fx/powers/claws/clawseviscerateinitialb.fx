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

lifespan	120

Condition
	On 	Time
	Time 	29

	Event
		EName	Streak1
		Type Local
		At	Root
		Geom	RootToChestAdjustment01
		Sound Eviscerate5 100.0 100.0 .8
	End
End



Condition
	On 	Time
	Time 	85

	Event
		EName	Streak
		Type	start
		At	Streak1
		Altpiv	1
		Part1	Powers/Claws/EviscerateStreak.part
	End

	Event
		EName	Streak9
		Type	start
		At	wepL
		Part1	Powers/Claws/ClawEmbersBroadEvicerate.part
		#Part	:ClawEmbersEvicerate.part
		LifeSpan	3
	End

	Event
		EName	Streak8
		Type	start
		At	wepR
		
		Part1	Powers/Claws/ClawEmbersBroadEvicerate.part
		#Part	:ClawEmbersEvicerate.part
		LifeSpan	3
	End
End


Condition
	On 	Time
	Time 	87

	Event
		EName	streak
		Type	Destroy
		
	End
	

End


End
####################################################################
