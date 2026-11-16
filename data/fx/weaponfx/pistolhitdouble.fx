#########################################################
##	template

FxInfo


Input
	Inpname Chest
End

LifeSpan 60

Condition
	On Time 
	Time 0
	
	Event
		EName	Prime
		Type 	Local
		At	Chest
		Part1	:Pistolflash.part
		Part2	:PistolflashStar2.part
		LifeSpan	10
	End
	
	Event
		EName	Prime
		Type 	Local
		At	Neck
		Part1	:Pistolflash.part
		Part2	:PistolflashStar2.part
		LifeSpan	10
	End

End


Condition
	On	Time
	Time	0
		
	Repeat	2
	RepeatJitter	1
	
	Event
		EName	Sparkup
		Type 	Start
		At	Chest
		BhvrOverride
			PositionOffset	0 0 0
		End
		ChildFX	V_COV\PhysicsEnabled\GunSparkOUT.fx
		Lifespan	60
		
	End

		
	Event
		HardwareONly
		EName	Sparkup
		Type 	Start
		At	Waist
		BhvrOverride
			PositionOffset	0 0 0
		End
		ChildFX	V_COV\PhysicsEnabled\GunSparkOUT.fx
		Lifespan	60
		
	End

End

Condition
	On	Time
	Time	0
		
	Repeat	2
	RepeatJitter	1
	
	Event
		EName	Sparkup
		Type 	Start
		At	Neck
		BhvrOverride
			PositionOffset	0 0 0
		End
		ChildFX	V_COV\PhysicsEnabled\GunSparkOUT.fx
		Lifespan	60
		
	End
	
	Event
		HardwareOnly
		EName	Sparkup
		Type 	Start
		At	Head
		BhvrOverride
			PositionOffset	0 0 0
		End
		ChildFX	V_COV\PhysicsEnabled\GunSparkOUT.fx
		Lifespan	60
		
	End

End

End	

	