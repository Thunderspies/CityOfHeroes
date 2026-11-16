#########################################################
##	template

FxInfo


Input
	Inpname Chest
End

LifeSpan 100

Condition
	On Time 
	Time 0
	
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
	Time	0

	Repeat	3
	
	Event
		EName	Sparkup
		Type 	Start
		At	Chest
		BhvrOverride
			PositionOffset	0 0 0
		End
		ChildFX	V_COV\PhysicsEnabled\ShrapnelSparkOut.fx
	End		
	Event
		HardwareOnly
		EName	Sparkup
		Type 	Start
		At	Chest
		BhvrOverride
			PositionOffset	0 0 0
		End
		ChildFX	V_COV\PhysicsEnabled\ShrapnelSparkExplode.fx
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

	Repeat	3
	
	Event
		EName	Sparkup
		Type 	Start
		At	Chest
		BhvrOverride
			PositionOffset	0 0 0
		End
		ChildFX	V_COV\PhysicsEnabled\ShrapnelSparkOut.fx
	End		
	Event
		HardwareOnly
		EName	Sparkup
		Type 	Start
		At	Chest
		BhvrOverride
			PositionOffset	0 0 0
		End
		ChildFX	V_COV\PhysicsEnabled\ShrapnelSparkExplode.fx
	End
End

Condition
	On Time 
	Time 18
	
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
	Time	18

	Repeat	3
	
	Event
		EName	Sparkup
		Type 	Start
		At	Chest
		BhvrOverride
			PositionOffset	0 0 0
		End
		ChildFX	V_COV\PhysicsEnabled\ShrapnelSparkOut.fx
	End		
	Event
		HardwareOnly
		EName	Sparkup
		Type 	Start
		At	Chest
		BhvrOverride
			PositionOffset	0 0 0
		End
		ChildFX	V_COV\PhysicsEnabled\ShrapnelSparkExplode.fx
	End
End

Condition
	On Time 
	Time 27
	
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
	Time	27

	Repeat	3
	
	Event
		EName	Sparkup
		Type 	Start
		At	Chest
		BhvrOverride
			PositionOffset	0 0 0
		End
		ChildFX	V_COV\PhysicsEnabled\ShrapnelSparkOut.fx
	End		
	Event
		HardwareOnly
		EName	Sparkup
		Type 	Start
		At	Chest
		BhvrOverride
			PositionOffset	0 0 0
		End
		ChildFX	V_COV\PhysicsEnabled\ShrapnelSparkExplode.fx
	End
End

Condition
	On Time 
	Time 36
	
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
	Time	36

	Repeat	3
	
	Event
		EName	Sparkup
		Type 	Start
		At	Chest
		BhvrOverride
			PositionOffset	0 0 0
		End
		ChildFX	V_COV\PhysicsEnabled\ShrapnelSparkOut.fx
	End		
	Event
		HardwareOnly
		EName	Sparkup
		Type 	Start
		At	Chest
		BhvrOverride
			PositionOffset	0 0 0
		End
		ChildFX	V_COV\PhysicsEnabled\ShrapnelSparkExplode.fx
	End
End

Condition
	On Time 
	Time 40
	
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
	Time	40

	Repeat	3
	
	Event
		EName	Sparkup
		Type 	Start
		At	Chest
		BhvrOverride
			PositionOffset	0 0 0
		End
		ChildFX	V_COV\PhysicsEnabled\ShrapnelSparkOut.fx
	End		
	Event
		HardwareOnly
		EName	Sparkup
		Type 	Start
		At	Chest
		BhvrOverride
			PositionOffset	0 0 0
		End
		ChildFX	V_COV\PhysicsEnabled\ShrapnelSparkExplode.fx
	End
End

Condition
	On Time 
	Time 48
	
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
	Time	48

	Repeat	3
	
	Event
		EName	Sparkup
		Type 	Start
		At	Chest
		BhvrOverride
			PositionOffset	0 0 0
		End
		ChildFX	V_COV\PhysicsEnabled\ShrapnelSparkOut.fx
	End

		
	Event
		HardwareOnly
		EName	Sparkup
		Type 	Start
		At	Chest
		BhvrOverride
			PositionOffset	0 0 0
		End
		ChildFX	V_COV\PhysicsEnabled\ShrapnelSparkExplode.fx
	End
End

End	

	