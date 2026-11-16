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
	Time 	1

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
		
		Part1	:FlameThrowerFlame.part
		Sound flamethrower 100.0 100.0 .85
		
	End

	Event
		EName 	tracer
		Type	local
		At	p2
		Altpiv 1
		Part1	WEAPONFX/TorchflamethrowerA.part
		Part2	WEAPONFX/TorchflamethrowerB.part
		#Part2	World/City/DirectionalSteamSM.part
		#Part1	World/City/DirectionalSteamSM2.part
	End

End


Condition
	On 	Time
	Time 	32

	Event
		EName 	Blast1
		Type	Destroy
				
	End
End

Condition
	On 	Time
	Time 	17

	Event
		EName 	tracer
		Type	Destroy
				
	End

End

End