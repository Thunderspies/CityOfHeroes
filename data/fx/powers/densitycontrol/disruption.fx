#########################################################
##	template

FxInfo

Input  
	Inpname	WepR
End

LifeSpan	100

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	Monkey
		Type	Local
		At	WepR
		Bhvr	Behaviors/Scale.bhvr
		Anim	FX_dark
	End

	Event
		EName	Punch1
		Type	Local
		At	Monkey
		 AnimPiv Chest
		Part1	:Electron01.part
		
	End


	Event
		EName	Punch2
		Type	Local
		At	Monkey
		 AnimPiv Neck
		Part1	:Electron01.part
		
	End

	Event
		EName	Punch3
		Type	Local
		At	Monkey
		 AnimPiv Waist
		Part1	:Electron01.part
	
	End

End


Condition
	On 	Time
	Time 	64

	Event
		EName	all
		Type	Destroy
	
	End


End	

End		