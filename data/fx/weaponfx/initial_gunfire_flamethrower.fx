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
	Time 	49

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
		At	p2
		Altpiv 1
		
		Part1	WEAPONFX/TorchflamethrowerA.part
		
	End
	
	Event
		EName 	Blast3
		Type	local
		At	p2
		Altpiv 1
		
		
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
	Time 	165

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