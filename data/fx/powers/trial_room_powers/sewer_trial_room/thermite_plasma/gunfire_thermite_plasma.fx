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
		
		
		Sound thermitecannon 100.0 100.0 .95
		
		Part1	:Thermite_FlameThrowerFlame.part		
		Part	:Thermite_FireSpecksSpit.part
		Part	:Thermite_FireSpecksFalling.part
		Part	:Thermite_FireSpit2.part
		Part	:Thermite_FireSpitStart.part
		Part	:Thermite_Spray.part
	End

	Event
		EName 	Blast2
		Type	local
		At	p2
		Altpiv 1
		
		#Part1	:Thermite_TorchflamethrowerA.part
		
	End

	Event
		EName 	Blast3
		Type	local
		At	p2
		Altpiv 1
		
		
		#Part2	:Thermite_TorchflamethrowerB.part
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