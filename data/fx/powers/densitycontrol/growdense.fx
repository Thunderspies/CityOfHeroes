#########################################################
##	forceBubble
########################################################
FxInfo

Input  
	Inpname	chest
End

Input  
	Inpname	Hips
End

Input  
	Inpname	WepR
End

Input  
	Inpname	WepL
End

#LifeSpan	200
##############################################################


Condition
	On 	Time
	Time 	0

	Event
		EName 	EnergyBalls
		Type	Local 
		At	chest
				
		Part1	:GrowDenseElectrons.part
		
		PMagnet	chest
		
	End

	Event
		EName 	Aura
		Type	Local 
		At	hips
				
		Part3	:GrowDenseAura.part

		PMagnet	chest
		
		
	End

	Event
		EName 	ring1
		Type	Local 
		At	hips
				
		Part1	:GrowDenseRing.part
		
		PMagnet	chest
	
	End

End


Condition
	On	Time
	Time	95
	
	Event
		EName   Hit
		Type	Local
		At	Hips
		part1	:GrowDenseAura02.part
		
	End
	
	Event
		EName   Hand2
		Type	Local
		At	WepR
		part1	:GrowDenseHands.part
		part2	:GrowDenseHandsTrails.part
	End

	Event
		EName   Hand1
		Type	Local
		At	WepL
		part1	:GrowDenseHands.part
		part2	:GrowDenseHandsTrails.part
	End

	Event
		EName   Hand1
		Type	Local
		At	Chest
		part1	:GrowDenseChest.part
	
	End

End


Condition
	On	Time
	Time	110
	
	Event
		EName   glows
		Type	Local
		At	Chest
		part1	:GrowDenseFallingElectrons.part
	
	End

End


Condition
	On	Time
	Time	119

	Event
		Ename	Monkey
		Type	Local
		At	Chest
		Anim	FX_dark
	End

	Event
		EName	Punch1
		Type	Local
		At	Monkey
		 AnimPiv Chest
		Part1	:Electron01_Large.part
		
	End


	Event
		EName	Punch2
		Type	Local
		At	Monkey
		 AnimPiv Neck
		Part1	:Electron01_Large.part
		
	End

	Event
		EName	Punch3
		Type	Local
		At	Monkey
		 AnimPiv Waist
		Part1	:Electron01_Large.part
	
	End



End


Condition
	On	Time
	Time	85
	
	Event
		EName   Hit
		Type	Local
		At	Hips
		part1	:GrowDenseRingOutwards.part
		
	End
End


Condition
	On 	Time
	Time 	70

	Event
		
		EName 	EnergyBalls
		Type	destroy 
						
	End
End


Condition
	On 	Time
	Time 	85

	Event
		
		EName 	Aura
		Type	destroy 
						
	End

End

End
