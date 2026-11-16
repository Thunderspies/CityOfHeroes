#########################################################
##	template

FxInfo


Input
	Inpname Chest
End

LifeSpan 30

Condition
	On Time 
	Time 5
	
	Event
		EName	Prime
		Type 	Local
		At	Chest
		Part1	:EnchantedPistolflash.part
		Part2	:EnchantedPistolflashStar.part
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

Condition
	On	PrimeHit
		
	Event
		Ename	Prime
		Type	Destroy
	End

End



End	

	