#########################################################
##	
##

FxInfo
LifeSpan  100

Input  
	InpName	Target
End

Input  
	InpName	Mystic
End

Input  
	InpName	WepR
End

##################################
Condition
	On 	Time
	Time 	44

	Event
		EName 	Prime
		Type	start 
		At	Mystic
	
		Magnet	Target
		LookAt	Target
		
	End

	Event
		ENAME   p2
		Type	Local 
		At	WepR
		Geom	FlameThrower
	End

	Event
		EName 	Blast1
		Type	local
		At	p2
		Altpiv 1
		
		
		Sound flamethrower 120.0 120.0 .98
		
		Part1	:FlameThrowerFlame.part		
		Part2	WEAPONFX/TorchflamethrowerB.part
		
			#Part	:FireSpit.part
		Part	:FireSpecksSpit.part
		Part	:FireSpecksFalling.part
		Part	:FireSpit2.part
		Part	:FireSpitStart.part
	End

	Event
		EName 	Blast2
		Type	local
		At	p2
		Altpiv 1
		
		Part1	WEAPONFX/TorchflamethrowerA.part
		
	End

End

Condition
	On 	Time
	Time 	58

	Event
		EName 	Blast2
		Type	Destroy
				
	End
End


Condition
	On 	Time
	Time 	78

	Event
		EName 	Blast1
		Type	Destroy
				
	End
End

Condition
	On 	Time
	Time 	63

	Event
		EName 	tracer
		Type	Destroy
				
	End

End

End