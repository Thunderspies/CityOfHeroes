#########################################################
##	
##

FxInfo
LifeSpan  200

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
	Time 	39

	Event
		EName 	Blast1
		Type	local
		At	WepR
		
		
		Sound flamethrower2 120.0 120.0 .95
		
		Part1	:FlameThrowerFlame.part		
		Part	:FireSpecksSpit.part
		Part	:FireSpecksFalling.part
		Part	:FireSpit2.part
		Part	:FireSpitStart.part
	End

	Event
		EName 	Blast2
		Type	local
		At	WepR
		
		Part1	WEAPONFX/TorchflamethrowerA.part
		
	End

	Event
		EName 	Blast3
		Type	local
		At	WepR
		
		
		Part2	:TorchflamethrowerB.part
	End

End

Condition
	On 	Time
	Time 	150

	Event
		EName 	Blast2
		Type	Destroy
				
	End
End

Condition
	On 	Time
	Time 	164

	Event
		EName 	Blast3
		Type	Destroy
				
	End
End

Condition
	On 	Time
	Time 	170

	Event
		EName 	Blast1
		Type	Destroy
				
	End
End

Condition
	On 	Time
	Time 	155

	Event
		EName 	tracer
		Type	Destroy
				
	End

End

End