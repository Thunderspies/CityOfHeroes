#########################################################
##	template

FxInfo

Input  
	Inpname	Root
End

Input
	Inpname WepR
End

Input
	Inpname WepL
End

Input
	Inpname Chest
End
#########################################################

Condition
	On Time 
	Time 0
	
	Event
		EName	Right
		Type 	Local
		At	WepR
		Part1	:speedattack1.part
		Sound flurrypower 50 50 .53
	End

	Event
		Ename	Left
		Type	Local
		At	WepL
		Part1	:speedattack1.part
	End
	
	Event
		EName	Right2
		Type 	Posit
		At	WepR
		Part1	:SpeedAttackStreaks.part
		Part2	:speedattack2.part
	End

	Event
		Ename	Left2
		Type	Posit
		At	WepL
		Part1	:SpeedAttackStreaks.part
		Part2	:speedattack2.part
	End

	Event
		Ename	aura
		Type	local
		At	Chest
		Part2	:FlurryAura.part
		Part1	:FlurryAuraOrange.part
		PMagnet	Chest

	End

End


Condition
	On Time 
	Time 3

	Event
		Ename	BubblingSpots
		Type	start
		At	Chest
		Part2	:BubblingSpots.part
	End

End

###################################################################


Condition
	On 	Time
	Time 	1

	Event
		Type	Local
		Bhvr	behaviors/speed.bhvr
		At	Root
		Anim	Mblur

	End
End

Condition
	On 	Time
	Time 	3

	Event
		Type	Local
		Bhvr	behaviors/speed.bhvr
		At	Root
		Anim	Mblur
	End
End

Condition
	On 	Time
	Time 	6

	Event
		Type	Local
		Bhvr	behaviors/speed.bhvr
		At	Root
		Anim	Mblur
		Sound Flurry 50 50 .75
	End
End

Condition
	On 	Time
	Time 	9

	Event
		Type	Local
		Bhvr	behaviors/speed.bhvr
		At	Root
		Anim	Mblur
	End
End

Condition
	On 	Time
	Time 	83
	
	Event
		Type	Posit
		At	WepR
		EName	FistStar
		Part1	:SpeedAttackstar.part
		Sound whoosh13 80 80 .82
	End

End

Condition
	On 	Time
	Time 	83
	
	Event
		Type	Destroy
		EName	BubblingSpots
	End
End

Condition
	On 	Time
	Time 	90
	
	Event
		Type	Destroy
		EName	All
	End
End	

End		