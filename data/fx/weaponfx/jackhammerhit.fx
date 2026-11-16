#########################################################
##	template

FxInfo
LifeSpan	15

Input
	Inpname Chest
End

Input
	Inpname Origin
End

#############################################################



Condition
	On	Time	
	Time	4
	
	Event
		Type	Local
		At	Origin
		Sound mallethit 60 60 .45
		
	End
End



Condition
	On	Time	
	Time	9
	
	Event
		Type	Local
		At	Origin
		Sound quillexplo2 60 60 .45
		
	End
End


Condition
	On	Time	
	Time	14
	
	Event
		Type	Local
		At	Origin
		Sound quillexplo2 60 60 .45
		
	End
End
Condition
	On 	cycle
	Time 	2
	Chance	1
	
	Event
		EName	Prime
		Type 	Local
		At	Chest
		Part1	:PistolflashA.part
		Part2	:PistolflashStar2A.part
		LifeSpan	10
	End

End

Condition
	On	Time
	Time	0
		
	Repeat	1
	RepeatJitter	1
	
	Event
		EName	Sparkup
		Type 	Start
		At	Chest
		BhvrOverride
			PositionOffset	0 0 0
		End
		ChildFX	V_COV\PhysicsEnabled\GunSparkOUT.fx
		
	End

End



End	

	