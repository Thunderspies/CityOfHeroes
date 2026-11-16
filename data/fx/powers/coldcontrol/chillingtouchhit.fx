#########################################################
##	chill touch hit
FxInfo

Input  
	InpName	chest
End


##################################
Condition
	On	Time
	Time	0
		

	Event	
		Ename   targetchill
		Type	Posit
		At	chest
		Part1	:IceControlTargetBurst.part
		
	End
	

End

Condition
	On	Time
	Time	20
		

	Event	
		Ename   targetchill
		Type	Destroy

	End
End

Condition
	On	Time
	Time	0
		

	Event	
		Ename   IceShards
		Type	Local
		At	chest
		Part1	:ChillTouchSnow.part
	End
End

Condition
	On	Time
	Time	0
		

	Event	
		Ename   IceShards
		Type	Local
		At	chest
		Part1	:Generic_hitflash.part
		Part2	:Generic_hitflash2.part
	
		
	
	
	End

End




Condition
	On	Time
	Time	70
		

	Event	
		Ename   all
		Type	Destroy

	End


End

End				