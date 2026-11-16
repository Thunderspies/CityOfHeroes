FxInfo

Input  
	InpName	chest
End

##################################

Condition
	Time	0	
	Event		
		Type	Local
		At	chest
		Part3   :ChillingTouchFlash01.part 
	End
End

Condition
	Time	0	
	Event	
		Ename   Ice
		Type	posit
		At	chest
		Part1	:IceChunks.part
		Part2	:IceChunksLarge.part	
	End
End

Condition
	Time	1	
	Event	
		Ename   targetchill
		Type	Posit
		At	chest
		Part1	:ChillingTouchMist03.part 
		Part2	:ChillingTouchMist01.part
	End
End

Condition
	Time	20	
	Event	
		Ename   targetchill
		Type	Destroy
	End
End

Condition
	Time	60	
	Event	
		Ename   Ice
		Type	Destroy
	End
End

Condition
	Time	10	
	Event	
		Ename   LeftHandAurA
		Type	Destroy
	End
End

Condition
	Time	0	
	Event	
		Ename   IceShards
		Type	Posit
		At	chest
		Part1	:ChillTouchSnow.part
		Part2	:IceScatter.part
	End
End

Condition
	Time	70	
	Event	
		Ename  All
		Type	Destroy
	End
End

End				