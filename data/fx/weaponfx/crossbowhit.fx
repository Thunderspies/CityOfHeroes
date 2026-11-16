#########################################################
##	template

FxInfo
LifeSpan	105

Input
	Inpname chest
End


Condition
	On Time 
	Time 1
	
	Event
		EName	Prime
		Type 	Local
		At	chest
		geom	FX_Arrow
		bhvr	:CrossbowBoltHit.bhvr
		
		Part2	:SmallProjectileflashStar.part
		Part1	:Pistolflash.part
		Sound arrowhit 50 50 .67
	End

End

Condition
	On 	Time
	Time	105
	
	Event
		Ename 	All
		Type	Destroy
	End

End



End	

	