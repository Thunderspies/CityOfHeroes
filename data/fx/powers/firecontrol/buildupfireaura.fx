#########################################################
##	FireAura
FxInfo


Input  
	InpName	Hips
End

Input  
	InpName	LlegL
End

Input  
	InpName	LlegR
End

Input  
	InpName	Chest
End

Input  
	InpName	Head
End

LifeSpan	120
##################################
Condition
	On	Time
	Time	60
	Event
		EName	Pivot
		Type	Local
		At	head
		
		part2	:FireCore.part
		Sound firesword6 80 80 .7

	End

	Event
		EName	Hips
		Type	Local
		At	Hips
		part1	:FireAura.part
		part2	:FireCore2.part
		part3	:FireCoreTrail1.part
	End

	Event
		EName	Chest
		Type	Local
		At	Chest
		
		part2	:FireCore3.part
		part3	:FireCoreTrail2.part
		Part4	:FireCoreSparks.part
	End
	
	Event
		EName	Chest
		Type	Local
		At	Head
		part1	:FireCore4.part
		part2	:FireCoreTrail3.part
	End


	Event
		EName	Lleg
		Type	Local
		At	LlegL
		part1	:FireCore1.part
		part2	:FireCoreTrail.part
	End

	Event
		EName	Rleg
		Type	Local
		At	LlegR
		part1	:FireCore1.part
		part2	:FireCoreTrail.part
	End

End


End				