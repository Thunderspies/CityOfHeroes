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
		Part1	:Missleflash.part
		Part2	:MissleflashStar.part
		Sound machgunhit4 50 50 .6
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
		
	End
End

Condition
	On Time 
	Time 6
	
	Event
		EName	Prime
		Type 	Local
		At	Chest
		Part1	:Missleflash.part
		Part2	:MissleflashStar.part
	End

End


Condition
	On	Time
	Time	6
		
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
		
	End
End

End	

	