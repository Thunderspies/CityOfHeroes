#########################################################
##	
##

FxInfo

Flags InheritAlpha

Condition
	On 	Time
	Time 	0

	Event
		EName	GasStream
		Type	Local
		At	Hair
		Part1	:TorchflameA.part
		Part1	:TorchGlow.part
		Part1	:VolcanoSparksB.part
	End

	Event
		EName	GasStream
		Type	Local
		At	Chest
		Part3	:TorchsparksA.part		
	End
End

#########################################################
#################### BODY GLOW 1 ########################
#########################################################

Condition
	On	Time
	Time	0

	Event
		Type	local
		At	Origin
		Sound lavasteam4_loop 60 60 .35
	End

End




Condition
	On	Time
	Time	0
	
	Event	
		ENAME	HeadGlow
		Type	Local
		At	Head
		#part	:InfernalBodyStreaks01.part
		part	:InfernalBodyStreaks02.part
		#part	:InfernalBodyGlows01.part
		POther Chest		
	End
	

	Event	
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		#part1	:InfernalBodyGlows01.part
		POther LarmR	
	End

	
	Event	
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		#part1	:InfernalBodyStreaks01.part
		part	:InfernalBodyStreaks02.part
		#part1	:InfernalBodyGlows01.part
		POther LarmR
		
	End


	Event	
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		#part1	:InfernalBodyGlows01.part
		POther LarmL
	End
	
	Event	
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		#part1	:InfernalBodyStreaks01.part
		part	:InfernalBodyStreaks02.part
		#part1	:InfernalBodyGlows01.part
		POther LarmL
	End

End

End				