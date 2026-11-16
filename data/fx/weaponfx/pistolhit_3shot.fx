#########################################################
##	template

FxInfo


Input
	Inpname Chest
End

LifeSpan 60

Condition
	On Time 
	Time 5
	
	Event
		EName	Prime
		Type 	Local
		At	Chest
		Part1	:Pistolflash.part
		Part2	:PistolflashStar.part
		Lifespan	5
	End

End

Condition
	On	Time
	Time	5
		
	Repeat	3
	RepeatJitter	1
	
	Event
		EName	Sparkup
		Type 	Start
		At	Chest
		BhvrOverride
			PositionOffset	0 0 0
		End
		ChildFX	V_COV\PhysicsEnabled\GunSparkOUT.fx
		LifeSpan	60
		
	End
		
	Event
		HardwareOnly
		EName	Sparkup
		Type 	Start
		At	Chest
		BhvrOverride
			PositionOffset	0 0 0
		End
		ChildFX	V_COV\PhysicsEnabled\GunSparkOUT.fx
		Lifespan	60
		
	End
End

Condition
	On	PrimeHit
		
	Event
		Ename	Prime
		Type	Destroy
	End

End



End	

	